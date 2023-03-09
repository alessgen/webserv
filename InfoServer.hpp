#ifndef INFOSERVER_HPP
# define INFOSERVER_HPP

// Root =        Sends all request to the path specified by the root directive.
// Buffer_size = Limit client body size.
// Error_page =  Default error page.
// Method =      Method supported by the server.

#include "webserv.hpp"

class InfoServer {

	public:

	//Costructor and Destructor
		InfoServer();
		~InfoServer() {};
		InfoServer(parsedconf tokens);
		InfoServer(InfoServer const &src) {};
		InfoServer &operator=(InfoServer const &src) {return (*this);};
	// Member Function

	// Getters
		std::string						getServerName();
		std::string						getRoot();
		int								getBufferSize();
		t_address						getServerInfo();
		std::map<int, std::string>		getErrorPage();
		std::set<std::string>			getMethod();

	private:

	// Members
		std::string						server_name;
		std::string						root;
		std::set<std::string>			method;
		std::map<int, std::string>		error_page;
		int								buffer_size;
		t_address						server_info;

	// Setters
		void							addServerName(std::string arg);
		void							addRoot(std::string arg);
		void							addMethod(parsedconf args);
		void							addErrorPage(parsedconf args);
		void							addServerInfo(parsedconf args);
		void							addBufferSize(std::string arg);
	};

	#endif