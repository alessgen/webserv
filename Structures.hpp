#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include "webserv.hpp"

#define serversMap std::map<std::string, InfoServer>

typedef std::map<std::string, std::string>	parsedconf;

// Host - Port of the server

typedef struct s_address
{
	std::string host;
	int			port;
}	t_address;

#endif