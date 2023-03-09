#include "InfoServer.hpp"

InfoServer::InfoServer() : root(""), buffer_size(0), server_name("default") {}

InfoServer::InfoServer(parsedconf tokens) {
	// for (auto it = tokens.begin(); it != tokens.end(); it++)
	// 	std::cout << it->first << " " << it->second << std::endl;
	addServerName(tokens["server_name"]);
	addRoot(tokens["root"]);
	addMethod(tokens);
	addBufferSize(tokens["buffer_size"]);
	addErrorPage(tokens);
	addServerInfo(tokens);
}

void InfoServer::addErrorPage(parsedconf args) {
	std::string key;
	int mapkey;
	for (std::map<std::string, std::string>::iterator it = args.begin(); it != args.end(); it++)
	{
		if (!it->first.compare("errorpage_404"))
			key = it->first;
	}
	mapkey = atoi(key.substr(strlen(key.c_str()) - 3, strlen(key.c_str())).c_str());
	this->error_page.insert(std::pair<int, std::string>(mapkey, args["errorpage_404"]));
}

void InfoServer::addServerName(std::string arg) {
	this->server_name = arg;
}

void InfoServer::addServerInfo(parsedconf args) {
	this->server_info.host = "localhost";
	this->server_info.port = atoi(args["port"].c_str());
}

void InfoServer::addBufferSize(std::string args) {
	const char* str;
	str = args.c_str();
	this->buffer_size = atoi(str);
}

void InfoServer::addMethod(parsedconf args) {
	std::string s = args["method"];
	std::string delimiter = " ";

	size_t pos = 0;
	std::set<std::string> token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token.insert(s.substr(0, pos));
		s.erase(0, pos + delimiter.length());
	}
	token.insert(s);
}

void InfoServer::addRoot(std::string arg) {
	this->root = arg;
}

std::string InfoServer::getServerName() {
	return (this->server_name);
}

std::string InfoServer::getRoot() {
	return (this->root);
}

int InfoServer::getBufferSize() {
	return (this->buffer_size);
}

t_address InfoServer::getServerInfo() {
	return (this->server_info);
}

std::map<int, std::string> InfoServer::getErrorPage() {
	return (this->error_page);
}

std::set<std::string> InfoServer::getMethod() {
	return (this->method);
}