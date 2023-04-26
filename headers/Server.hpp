#ifndef SERVER_HPP
# define SERVER_HPP

#include "Webserv.hpp"
#include "Response.hpp"
#include "Request.hpp"

class Server{
	public:
		Server(){};
		~Server(){};
		Server(const Server &src);
		Server(t_address addr, const InfoServer &src);
		void	setaddr();
		long	setup();
		long	getfd();
		long	accept();
		long	readfd(long sock);
		long	send(long fd);
		void	close(long fd);
		void	process(long fd);

	private:
		std::map<long, std::vector<char> >	requests;
		struct sockaddr_in					_addr;
		int									_fd;
		t_address							ports;
		InfoServer							_infoserver;
};

#endif