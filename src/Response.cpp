#include "Response.hpp"
#include "Request.hpp"
#include <sys/socket.h>
#include <unistd.h>

Response::Response(void) {}

Response::~Response(void) {}

void Response::setStatusLine(const std::string& version, int statusCode, const std::string& reasonPhrase) {
	std::ostringstream statusStream;
	statusStream << version << " " << statusCode << " " << reasonPhrase;
	statusLine = statusStream.str();
}

void Response::setHeader(const std::string& key, const std::string& value) {
	headers[key] = value;
}

void Response::setBody(const std::string& bodyFile) {
    std::ifstream file(bodyFile.c_str());
    if (!file) {
        body = NOT_FOUND_PAGE_ERROR;
		return ;
    }
    std::ostringstream oss;
    oss << file.rdbuf();
    body = oss.str();
	file.close();
}

std::string Response::buildResponse() {
	std::ostringstream responseStream;
	responseStream << statusLine << "\r\n";
	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
		responseStream << it->first << ": " << it->second << "\r\n";
	}
	responseStream << "\r\n" << body;
	return responseStream.str();
}

void Response::sendResponse(int client_fd) {
	std::string response = buildResponse();
	send(client_fd, response.c_str(), response.size(), 0);
}

int Response::treatActionAndResponse(std::map<int, std::string> request, int client_fd, e_httpMethodActions action) {
	Config	*config = Config::getInstance();

	ServerConfig	server = config->ClientServerMap[client_fd];
	if (!request.empty() && !request[client_fd].empty()) {
		switch (action) {
			case RESPONSE:
				responseGET(server, client_fd);
				break;
			case CLOSE:
				request.erase(request.find(client_fd));
				close(client_fd);
				break;
		}
		return 1;
	}
	return 0;
}

void	Response::responseGET(ServerConfig &server, int client_fd)
{
	std::map<std::string, RouteConfig>::iterator routeIt = server.routes.find(routeRequested);
	RouteConfig &route = routeIt->second;
	if (routeIt != server.routes.end()) {
		
		setStatusLine("HTTP/1.1", 200, "OK");
		setHeader("Content-Type", "text/html");
		(void)route;
		setBody(route.default_file);
	} else {
		setStatusLine("HTTP/1.1", 404, "OK");
		setHeader("Content-Type", "text/html");
		setBody(server.default_error_page);
	}

	std::ostringstream sizeStream;
	sizeStream << body.size();
	setHeader("Content-Length", sizeStream.str());
	sendResponse(client_fd);
}


void    Response::setClientRequest(std::map< std::string, std::vector<std::string> >      &request)
{
	routeRequested = request["request"][ROUTE];
}
