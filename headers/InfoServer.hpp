#ifndef INFOSERVER_HPP
# define INFOSERVER_HPP

// Root =        Sends all request to the path specified by the root directive.
// Buffer_size = Limit client body size.
// Error_page =  Default error page.
// Method =      Method supported by the server.

#include "Webserv.hpp"

class InfoServer {
	public:
	//Costructor and Destructor
		InfoServer() : root(""), buffer_size(0), server_name("default"){};
		~InfoServer(){};
		InfoServer(parsedconf tokens, int num_servers);
		InfoServer(InfoServer const &src) {*this = src;};
		InfoServer &operator=(InfoServer const &src);
	// Getters
		std::string						getServerName();
		std::string						getRoot();
		int								getBufferSize();
		std::vector<t_address>			getServerInfo();
		std::map<int, std::string>		getErrorPage();
		std::set<std::string>			getmethod();
		const std::vector<t_location>	getLocation() const;

	private:
	// Members
		std::string						root;
		std::set<std::string>			method;
		std::map<int, std::string>		error_page;
		int								buffer_size;
		std::string						server_name;
		std::vector<t_address>			server_info;
		std::vector<t_location>			locations;
	// Setters
		void							addServerName(parsedconf arg, int num_servers);
		void							addRoot(parsedconf arg, int num_servers);
		void							addMethod(parsedconf arg, int num_servers);
		void							addErrorPage(parsedconf arg, int num_servers);
		void							addServerInfo(parsedconf arg, int num_servers);
		void							addBufferSize(parsedconf arg, int num_servers);
		void							addLocations(parsedconf arg, int num_servers);
	};

#endif