
#include "EventLoop.hpp"
#include "Config.hpp"
#include <csignal>
// #define PORT 8080 // ainda precisa disto ?


int initializeConfig(Config	&config, int argc, char* argv[]) {
	if (argc == 1) {
		config.loadDefaultConfig();
	} else if (argc == 2) {
		config.loadConfig(argv[1]);
	} else {
		std::cerr << "Usage: ./webserv [config_file.conf]" << std::endl;
		return (0);
	}
	return (1);
}

int main(int argc, char* argv[])
{
	Config *config = Config::getInstance();

	if (!initializeConfig(*config, argc, argv) != 0) {
		return (1);
	}

	N_SERVERS = config->servers.size();
	std::vector<Server>	servers;
	EventLoop			loop;

	for (uint i = 0; i < N_SERVERS; i++)
	{
		struct pollfd	server_poll_fd;
		Server			tmp(config->servers[i]);

		servers.push_back(tmp);
		server_poll_fd.fd = servers[i].fd;
		server_poll_fd.events = POLLIN;
		loop.connection->poll_fds.push_back(server_poll_fd);
	}

	// printConfig(config);
	loop.run(servers);
	return 0;
}
