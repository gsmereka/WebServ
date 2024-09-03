#include "Post.hpp"
#include "Response.hpp"

Post::Post(Response &objectRef) : response(objectRef) {}

Post::~Post(void) {}

int	Post::handleRequest(void)
{
	if (response.requestInfo.pathType == CGI)
		return responseCGI();
	// return upload();
	return (404);
}

void	Post::upload(void)
{
	// Handle file upload logic here
}

void	Post::buildBody(void) {}

int	Post::responseCGI(void) {
	htmlResponse	htmlResponse;
	CGIServer		cgi(response.requestInfo);

	cgi.setEnv();
	htmlResponse = cgi.executeScript();


	response.setBody(htmlResponse.body);
	return(htmlResponse.code);
}
