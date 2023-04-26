#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "Webserv.hpp"
#include "Request.hpp"
#include "InfoServer.hpp"

std::string 				content_type(std::string extension);
std::string					check_index(std::string location_, const InfoServer &src);
std::vector<std::string> 	create_cgi_env(char *path, std::string method_, std::string protocol, std::map<std::string, std::string> _headers);
char** 						vec_to_env(std::vector<std::string> arg);
void						setDate(std::string &date);
void						setLastModified(std::string location_, std::string &last_modif);
void	 					setServName(std::string &serv_name);
void		 				setConnection(std::string &connect);

class Response {

	public:
	// Costructor and Destructor
		Response(){};
		~Response(){};
		Response(Request &other, InfoServer &src);
	//Member Function
		int									check();
		void								generate();
		void								GETmethod();
		void								POSTmethod();
		void								DELETEmethod();
		void								error_response(int code, char *str);
		bool								isResponseOk();
		void								DELETE_success_response();
		void								GET_success_response();
		void								POST_success_response();
		std::string							getResponse();
		int									get_code();
		t_address							getServerPort();
		bool 								constructor_util(Request &other, InfoServer &src);
		void								constructor_assign(Request &other, InfoServer &src);
		bool								constructor_chk_locations(Request &request,  InfoServer &src);
	//GETTERS
		std::map<std::string, std::string> 	getHeaders();
		std::string							getMethod();
		std::string							getProtocol();
		size_t								getContentlength();
		char								*getPostarg();

	private:
		std::map<std::string, std::string> _headers;
		std::map<int, std::string>			errorpages;
		std::string							location_;
		std::string							method_;
		std::string							c_type;
		std::string							connect;
		std::string							serv_name;
		std::string							date;
		std::string							protocol;
		std::string							last_modif;
		std::string							ext_messg;
		std::string							body;
		std::string							response;
		InfoServer							_infoserver;
		t_address 							server_port;
		size_t								c_lenght;
		bool								response_ok;
		char								*postArg;
		int									ext_code;
};

std::string									execPost(char *path, Response &src);

#endif
