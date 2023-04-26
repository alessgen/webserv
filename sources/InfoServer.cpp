#include "InfoServer.hpp"

InfoServer &InfoServer::operator=(const InfoServer &src) {
	this->buffer_size = src.buffer_size;
	this->error_page = src.error_page;
	this->server_info = src.server_info;
	this->server_name = src.server_name;
	this->method = src.method;
	this->root = src.root;
	this->locations = src.locations;
	return (*this);
}

InfoServer::InfoServer(parsedconf tokens, int num_servers) {
	addErrorPage(tokens, num_servers);
	addServerName(tokens, num_servers);
	addRoot(tokens, num_servers);
	addBufferSize(tokens, num_servers);
	addServerInfo(tokens, num_servers);
	addMethod(tokens, num_servers);
	addLocations(tokens, num_servers);
}

void InfoServer::addLocations(parsedconf args, int num_servers) {
	int i = 0;
	for (vector_pair::iterator it = args[num_servers].begin(); it != args[num_servers].end(); it++)
	{
		if (!it->first.compare(0, 8, "location"))
		{
			this->locations.push_back(t_location());
			this->locations[i].locationPath = it->second.substr(0, strlen(it->second.c_str()) - 2);
			it++;
			for (;it != args[num_servers].end() && it->first.compare(0, 8, "location"); it++)
			{
				if (!it->first.compare(0, 4, "root"))
					this->locations[i].locationRoot = it->second;
				if (!it->first.compare(0, 5, "index"))
					this->locations[i].locationIndex = it->second;
				if (!it->first.compare(0, 6, "method"))
					this->locations[i].locationMethod = it->second;
			}
			it--;
			i++;
		}
	}
}

void InfoServer::addErrorPage(parsedconf args, int num_servers) {
	std::string key = "";
	int mapkey;
	for (vector_pair::iterator it = args[num_servers].begin(); it != args[num_servers].end(); it++)
	{
		if (!it->first.compare(0, 5, "error"))
		{
			key = it->first;
			mapkey = atoi(key.substr(strlen(key.c_str()) - 3, strlen(key.c_str())).c_str());
			this->error_page.insert(std::pair<int, std::string>(mapkey, it->second));
		}
	}
	if (!key.compare("")) {
		// Prendere l'errorpages di default nel .conf
		char cwd[PATH_MAX];
		if (getcwd(cwd, sizeof(cwd)) == NULL) {
			std::cerr << BHYELLOW << "Error getting current working directory." << RESET << std::endl;
			return;
		}
		// Setup correct path
		std::string cwd_ = std::string(cwd) + "/ourserver/errorpages.conf";
		// Try open file
		std::ifstream inputFile(cwd_.c_str());
		if (!inputFile) {
			std::cerr << BHYELLOW << "Unable to open file" << RESET << std::endl;
			return ;
		}
		std::string line;
		while (std::getline(inputFile, line)) {
			std::stringstream temp(line);
			std::string temp_l;
			getline(temp, temp_l);
			if (temp_l != ""){
				std::string key = temp_l.substr(temp_l.find("_") + 1, 3);
				std::string value = temp_l.substr(temp_l.find(" ") + 1, temp_l.length());
				this->error_page.insert(std::pair<int, std::string>(atoi(key.c_str()), value));
			}
		}
		for(std::map<int, std::string>::iterator it = this->error_page.begin(); it != this->error_page.end(); ++it) {
			cwd_ = std::string(cwd) + "/" + it->second;
			std::ifstream inputFile(cwd_.c_str());
			if (!inputFile) {
				std::cerr << BHYELLOW << "Unable to open file" << RESET << std::endl;
				return ;
			}
		}
	}
}

void InfoServer::addServerName(parsedconf arg, int num_servers) {
	for (vector_pair::iterator it = arg[num_servers].begin(); it != arg[num_servers].end(); it++)
	{
		if (!it->first.compare(0, 11, "server_name"))
			this->server_name = it->second;
	}
}

void InfoServer::addServerInfo(parsedconf args, int num_servers) {
	t_address test;
	for (vector_pair::iterator it = args[num_servers].begin(); it != args[num_servers].end(); it++)
	{
		if (!it->first.compare(0, 4, "port"))
		{
			test.host = "localhost";
			test.port = atoi(it->second.c_str());
			this->server_info.push_back(test);
		}
	}
	test.host = "EXIT_CHECK";
	test.port = -1;
	this->server_info.push_back(test);
}

void InfoServer::addBufferSize(parsedconf args, int num_servers) {
	for (vector_pair::iterator it = args[num_servers].begin(); it != args[num_servers].end(); it++)
	{
		if (!it->first.compare(0, 11, "buffer_size"))
			this->buffer_size = atoi(it->second.c_str());
	}
}

void InfoServer::addMethod(parsedconf args, int num_servers) {
	std::string s;
	for (vector_pair::iterator it = args[num_servers].begin(); it != args[num_servers].end(); it++)
	{
		if (!it->first.compare(0, 6, "method"))
		{
			s = it->second;
			break ;
		}
	}
	std::string delimiter = " ";

	size_t pos = 0;
	std::set<std::string> token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token.insert(s.substr(0, pos));
		s.erase(0, pos + delimiter.length());
	}
	token.insert(s);
	this->method = token;
}

void InfoServer::addRoot(parsedconf arg, int num_servers) {
	for (vector_pair::iterator it = arg[num_servers].begin(); it != arg[num_servers].end(); it++)
	{
		if (!it->first.compare(0, 4, "root"))
		{
			this->root = it->second;
			break;
		}
	}
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

std::vector<t_address> InfoServer::getServerInfo() {
	return (this->server_info);
}

std::map<int, std::string> InfoServer::getErrorPage() {
	return (this->error_page);
}

std::set<std::string> InfoServer::getmethod() {
	return (this->method);
}

const std::vector<t_location> InfoServer::getLocation() const{
	return (this->locations);
}