#include "Request.hpp"

Request::~Request(){
	if (postArg)
		delete [] postArg;
}

Request::Request(const char	*request){
	std::stringstream read_stream(request);
	std::string buff;
	read_stream >> _method;
	read_stream >> _root;
	read_stream >> _protocol;
	read_stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	_headers.insert(std::make_pair("VALUES", _method + _root + _protocol));
	while (std::getline(read_stream, buff) && buff != "\r") {
		std::stringstream temp(buff);
		std::string key;
		std::string value;
		std::getline(temp, key, ':');
		std::getline(temp, value);
		_headers.insert(std::make_pair(key, value));
	}
	if (!_method.compare("POST"))
	{
		std::string post = _headers["Content-Length"];
		std::istringstream(post) >> _content_length;
		postArg = new char[_content_length];
		memcpy(postArg, request + read_stream.tellg(), _content_length);
	}
}

void Request::setServerPort(t_address port) {
	this->_serverport = port;
}

size_t	Request::getContentLength(){
	return (this->_content_length);
}

char *Request::getPostargs() {
	return (this->postArg);
}

std::string Request::getReqMethod() {
	return (this->_method);
}

std::string Request::getReqRoot() {
	return (this->_root);
}

std::string Request::getReqProtocol() {
	return (this->_protocol);
}

std::map<std::string, std::string> Request::getReqHeaders() {
	return (this->_headers);
}

t_address Request::getServerPort() {
	return (this->_serverport);
}
