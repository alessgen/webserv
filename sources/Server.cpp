#include "Server.hpp"

Server::Server(const Server &src){
	*this = src;
}

Server::Server(t_address addr, const InfoServer &src){
	this->ports = addr;
	_infoserver = src;
}

void Server::setaddr(){
	unsigned int ip_addr = 1;
	if (!ports.host.compare("localhost"))
		ip_addr = 0;

	uint32_t network_ip_address = (BYTE_ORDER == LITTLE_ENDIAN ? htonl(ip_addr) : ip_addr);

	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = network_ip_address;
	_addr.sin_port = htons(ports.port);
}

char *vec_string(std::vector<char> &vec){
	char *str = new char[vec.size() + 1];
	for (size_t i = 0; i < vec.size(); i++)
		str[i] = vec[i];
	str[vec.size()] = '\0';
	return str;
}

long Server::setup(){
	int port = this->ports.port;
	std::stringstream ss;
	ss << port;
	std::string str = ss.str();
	if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw ServerEX::SockError(str);
	this->setaddr();
	int optval = 1;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) 
		throw ServerEX::SetSockError(str);
	if (bind(_fd, (const sockaddr *)&_addr, sizeof(_addr)) == -1)
		std::cerr << BHYELLOW << "Error in bind: " << this->ports.port << RESET << std::endl;
	if (listen(_fd, 100) == -1)
		throw ServerEX::ListenError(str);
	return (_fd);
}

long Server::getfd(){
	return this->_fd;
}

long Server::accept(){
	long sock;

	if ((sock = ::accept(_fd, NULL, NULL)) == -1)
		std::cerr << BHYELLOW << "Error in accept: " << this->ports.port << RESET << std::endl;
	else
		fcntl(sock, F_SETFL, O_NONBLOCK);
	return (sock);
}

long Server::readfd(long sock){
	size_t k;
	long ret;
	char buffer[READFD_SIZE] = {0};
	ret = recv(sock, buffer, READFD_SIZE - 1, 0);

	if (ret == 0 || ret == -1){
		this->close(sock);
		if (ret == -1){
			std::cerr << BHYELLOW << "Error in RECV(): " << this->ports.port << RESET << std::endl;
		}
		return -1;
	}
	for (unsigned int i = 0; i < ret; i++)
	 	requests[sock].push_back(buffer[i]);
	std::string request(requests[sock].begin(), requests[sock].end());
	if ((k  = request.find("\r\n\r\n")) != std::string::npos)
	{
		if (request.find("Content-Length: ") == std::string::npos)
		{
			if (request.find("Transfer-Encoding: chunked") != std::string::npos)
			{
				size_t k = request.size();
				std::string end = "0\r\n\r\n";
				for (size_t j = end.size(); j > 0; j--, k--){
					if (k == std::numeric_limits<std::size_t>::max() || end[j] != request[k])
						return 1;
				}
				return 0;
			}
			else
				return 0;
		}
		std::string cont_lenght = request.substr(request.find("Content-Length: ") + 16, 10);
		size_t cont_len = std::atoi(cont_lenght.c_str());
		if (request.size() >= cont_len + k + 4)
			return 0;
		else
			return 1;
	}
	return 1;
}

long Server::send(long fd){
	static std::map<long, size_t> sent;
	std::string str(requests[fd].begin(), requests[fd].end());

	if (sent.find(fd) == sent.end())
		sent[fd] = 0;

	if (sent[fd] == 0)
	{
		if (str.size() < 1000)
			std::cout << HCYAN<< str << RESET << std::endl;
		else
			std::cout << HMAGENTA << str << RESET << std::endl;
	}
	char *request = vec_string(this->requests[fd]);
	long ret = ::send(fd, request, strlen(request), 0);

	if (request)
		delete [] request;

	if (ret == -1) {
		this->close(fd);
		sent[fd] = 0;
		return -1;
	}
	else {
		sent[fd] += ret;
		if (sent[fd] >= requests[fd].size())
		{
			requests.erase(fd);
			sent[fd] = 0;
			return 0;
		}
		else
			return 1;
	}
}

void Server::process(long fd){
	std::string str(requests[fd].begin(), requests[fd].end());

	if (str.size() < 1000)
		std::cout << HYELLOW << str << RESET << std::endl;
	else
		std::cout << HMAGENTA << str << RESET << std::endl;
	char *request = vec_string(requests[fd]);

	Request _request(static_cast<const char *>(request));
	if (request)
		delete [] request;

	_request.setServerPort(this->ports);
	Response _response(_request, _infoserver);
	_response.generate();
	if (_response.isResponseOk());
	else
		_response.error_response(0, NULL);

	this->requests.erase(fd);
	_response.getResponse();
	std::string str_tmp = _response.getResponse();
	std::vector<char> vec(str_tmp.begin(), str_tmp.end());
	this->requests[fd] = vec;
}

void Server::close(long fd){
	if (fd > 0)
		::close(fd);
	requests.erase(fd);
}
