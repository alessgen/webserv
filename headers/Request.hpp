#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "Webserv.hpp"

class Request{
	public: // Getters
		Request(){};
		~Request();
		Request(const char *request);
		std::string 						getReqMethod();
		std::string 						getReqRoot();
		std::string 						getReqProtocol();
		std::map<std::string, std::string>	getReqHeaders();
		char  								*getPostargs();
		void 								setServerPort(t_address port);
		t_address 							getServerPort();
		size_t								getContentLength();

	private:
		std::map<std::string, std::string>	_headers;
		std::string							_method;
		std::string							_root;
		std::string							_protocol;
		std::vector<std::string>			post_args;
		char								*postArg;
		t_address							_serverport;
		size_t								_content_length;
};

#endif