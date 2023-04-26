#ifndef SETUPCLUSTER_H
# define SETUPCLUSTER_H

#include "Webserv.hpp"
#include "Server.hpp"

class SetupCluster{
	public:
		SetupCluster();
		~SetupCluster(){};
		SetupCluster(std::vector<InfoServer> vec);
		void	setup();
		void	run();

	private:
		std::vector<Server> servers_;
		fd_set				set_fds;
		long				_maxfd;
};

#endif