#include "Response.hpp"

int Response::check(){
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) == NULL) {
		std::cerr << BHYELLOW << "Error getting current working directory." << RESET << std::endl;
		return (500);
	}
	std::string cwd_ = std::string(cwd) + "/" + this->location_;
	// Set the Content Type
	std::size_t extension = cwd_.find_last_of('.');
	if (extension != std::string::npos)
	{
		c_type = "Content-Type: " + content_type(cwd_.substr(extension, (cwd_.length() - extension)));
		if (!c_type.compare("Content-Type: Not Found"))
			return (500);
	}
	else
		c_type = "Content-Type: NO FILE";

	this->body = read_file(cwd_, this->ext_code);
	if (this->body.empty())
		return (404);
	c_lenght = body.length();
	return (0);
}

void Response::generate(){
	if (!method_.compare("GET"))
		GETmethod();
	else if (!method_.compare("POST"))
		POSTmethod();
	else if (!method_.compare("DELETE"))
		DELETEmethod();
	else
		ext_code = 405;
}

// Get Method Function that genereta the successfully response
void Response::GETmethod() {
	int ret_check = check();
	if (ret_check)
	{
		this->ext_code = ret_check;
		return;
	}
	setDate(date);
	setConnection(connect);
	setServName(serv_name);
	setLastModified(location_, last_modif);
	GET_success_response();
}

// Post Method Function that genereta the successfully response
void Response::POSTmethod() {
	setDate(date);
	setConnection(connect);
	setServName(serv_name);
	std::string path = "./cgi-script/upload.py";
	std::fstream file(path.c_str());
	if (file) {
		body = execPost((char *)path.c_str(), *this);
		c_lenght = body.length();
	}
	POST_success_response();
}

// Delete Method Function that genereta the successfully response
void Response::DELETEmethod() {
	std::string path = location_;

	if (!remove(path.c_str())) {
		setDate(date);
		setConnection(connect);
		setServName(serv_name);
		DELETE_success_response();
	}
	else {
		response_ok = false;
		ext_code = 400;
	}
}

// Response generator for GET 
void Response::GET_success_response(){
	response.clear();
	ext_messg = "OK";
	std::string code, lenght;
	std::stringstream temp, temp_;
	temp << ext_code;
	temp_ << c_lenght;
	code = temp.str();
	lenght = temp_.str();
	response = protocol + ' ' + code + ' ' + ext_messg + "\n" + c_type + "\n" + "Content-Length: " + lenght + "\n" + this->serv_name + "\n" + this->date + "\n" + this->last_modif + "\n" + this->connect + "\n\n" + body + "\n";
	response_ok = true;
}

// Response generator for POST
void Response::POST_success_response(){
	response.clear();
	ext_messg = "OK";
	std::string code, lenght;
	std::stringstream temp, temp_;
	temp << ext_code;
	temp_ << c_lenght;
	code = temp.str();
	lenght = temp_.str();
	response = protocol + ' ' + code + ' ' + ext_messg + "\n" + "Content-Type: text/html" + "\n" + "Content-Length: " + lenght + "\n" + this->serv_name + "\n" + this->date + "\n" + this->connect + "\n\n" + body + "\n";
	response_ok = true;
}

// Response generator for DELETE
void Response::DELETE_success_response() {
	response.clear();
	ext_messg = "OK";
	body = "The file " + location_ + " was deleted successfully\n";
	c_lenght = body.length();
	std::string code, lenght;
	std::stringstream temp, temp_;
	temp << ext_code;
	temp_ << c_lenght;
	code = temp.str();
	lenght = temp_.str();
	response = protocol + ' ' + code + ' ' + ext_messg + "\n" + "Content-Type: text/html" + "\n" + "Content-Length: " + lenght + "\n" + this->serv_name + "\n" + this->date + "\n" + this->connect + "\n\n" + body + "\n";
	response_ok = true;
}

// Response generator in case of ERROR
void Response::error_response(int code, char *str) {
	location_ = "./" + this->errorpages[ext_code];
	if (!code){
		switch(ext_code)
		{
			case 400:
				ext_messg = "Bad Request";
				break;
			case 403:
				ext_messg = "Forbidden";
				break;
			case 404:
				ext_messg = "Not Found";
				break;	
			case 405:
				ext_messg = "Method Not Allowed";
				break;
			case 501:
				ext_messg = "Not Implemented";
				break;
		}
	}
	else
	{
		ext_code = code;
		ext_messg = str;
	}
	setDate(date);
	setConnection(connect);
	setServName(serv_name);
	setLastModified(location_, last_modif);
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) == NULL) {
		std::cerr << BHYELLOW << "Error getting current working directory." << RESET << std::endl;
		return ;
	}
	std::string cwd_ = std::string(cwd) + "/" + this->errorpages[ext_code];
	this->body = read_file(cwd_, ext_code);
	c_lenght = body.length();
	c_type = "Content-Type: text/html";
	std::string code_str, lenght;
	std::stringstream temp, temp_;
	temp << ext_code;
	temp_ << c_lenght;
	code_str = temp.str();
	lenght = temp_.str();
	response += protocol + ' ' + code_str + ' ' + ext_messg + "\n" + c_type + "\n" + "Content-Length: " + lenght + "\n" + this->serv_name + "\n" + this->date + "\n" + this->last_modif + "\n" + this->connect + "\n\n" + body + "\n";
}

std::string Response::getMethod() {
	return (this->method_);
}

std::string Response::getProtocol() {
	return (this->protocol);
}

char* Response::getPostarg() {
	return (this->postArg);
}

std::map<std::string, std::string> Response::getHeaders() {
	return (this->_headers);
}

size_t Response::getContentlength() {
	return (this->c_lenght);
}

std::string Response::getResponse() {
	return (this->response);
}

int Response::get_code() {
	return (this->ext_code);
}

bool Response::isResponseOk() {
	return response_ok;
}