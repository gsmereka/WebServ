#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#define DEFAULT_PORT 8080

// Definições de macros para RouteConfig
#define DEFAULT_ROUTE_PATH "/"
#define DEFAULT_REDIRECTION ""
#define DEFAULT_ROOT_DIRECTORY "./serverRoot"
#define DEFAULT_DIRECTORY_LISTING false
#define DEFAULT_FILE "index.html"
#define DEFAULT_CGI_EXTENSION ".py"
#define DEFAULT_UPLOAD_DIRECTORY ""
#define DEFAULT_ACCEPTED_METHODS "GET", "DELETE"

// Definições de macros para CGIConfig
#define DEFAULT_PATH_INFO "cgi-bin"
#define DEFAULT_SCRIPT_PATH "cgi-bin"

// Definições de macros para ServerConfig
#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_SERVER_PORT -1
#define DEFAULT_ERROR_PAGE "/error.html"
#define DEFAULT_CLIENT_BODY_LIMIT 1048576
#define DEFAULT_SERVER_NAME "webserv.com"

struct RouteConfig
{
	std::string					route;
	std::vector<std::string>	accepted_methods;
	std::string					redirection;
	std::string					root_directory;
	bool 						directory_listing;
	std::string 				default_file;
	std::string 				cgi_extension;
	std::string					upload_directory;

	RouteConfig();
};

struct CGIConfig
{
	std::string path_info;
	std::string script_path;

	CGIConfig();
};

struct ServerConfig
{
	std::string					host;
	int 						port;
	std::vector<std::string>	server_names;
	std::map<int, std::string>	default_error_page;
	size_t						client_body_limit;
	CGIConfig					cgi;
	std::map<std::string, RouteConfig>	routes;

	ServerConfig();
};

class Config
{
	private:
		ServerConfig		currentServer;
		RouteConfig			currentRoute;
		bool				inServer;
		bool				inRoute;
		std::vector<int>	usedPorts;
		bool				isReservedPort(int port);
		void	finishServer(void);
		void	finishRoute(void);
		void	processConfigLine(const std::string &line);
		void	processServerConfig(const std::string& key, const std::string& value, std::istringstream &iss);
		void	processRouteConfig(const std::string& key, const std::string& value, std::istringstream &iss);
		void	processCGIConfig(const std::string& key, const std::string& value);
		void	addServer(const ServerConfig& server);
		void	configReset();
		void	finalizeConfigParsing();
		void	handleEndRoute();
		void	handleStartRoute();
		void	handleEndServer();
		void	handleStartServer();

		// Construtor privado para impedir a criação de instâncias fora da classe
		Config() {}
		// Construtor de cópia e operador de atribuição privados para impedir a cópia
		Config(const Config&);
		Config& operator=(const Config&);

    public:
        std::vector<ServerConfig> servers;

        // Ponteiro estático para a instância
        static Config* instance;

        // Método estático para acessar a única instância
        static Config*	getInstance();
		void			freeInstance();
		void			loadDefaultConfig(void);
		void			loadConfig(const std::string& configFilePath);
};

void	printConfig(void);
void	printServerConfig(ServerConfig& serverConfig, std::ofstream &fd);

#endif //CONFIG_HPP
