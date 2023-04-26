#include "SetupCluster.hpp"

SetupCluster::SetupCluster(){}

SetupCluster::SetupCluster(std::vector<InfoServer> vec){
	for (std::vector<InfoServer>::iterator it = vec.begin(); it != vec.end(); it++){
		std::vector<t_address> test_ = it->getServerInfo();
		for (std::vector<t_address>::iterator it_ = test_.begin(); it_ != test_.end(); it_++){
			if (it_->port == -1 && !it_->host.compare("EXIT_CHECK"))
				break;
			Server test(*it_, *it);
			servers_.push_back(test);
		}
	}
}

void SetupCluster::setup(){
	long fd;
	_maxfd = 0;
	FD_ZERO(&set_fds);
	for (std::vector<Server>::iterator it = servers_.begin(); it != servers_.end(); it++){
		fd = it->setup();
		FD_SET(fd, &set_fds);
		if (fd > _maxfd)
			_maxfd = fd;
	}
}

void SetupCluster::run(){
	fd_set read_fds, write_fds;
	struct timeval timeout;
	std::map<long, Server> _sockets;
	std::vector<int> _responses;
	int reqs = 0;
	fd_set backup = set_fds;
	while (1)
	{
		reqs = 0;
		while (reqs == 0){
			timeout.tv_sec = 1;
			timeout.tv_usec = 0;
			memcpy(&read_fds, &set_fds, sizeof(set_fds));
			FD_ZERO(&write_fds);
			for (std::vector<int>::iterator it = _responses.begin() ; it != _responses.end() ; it++)
				FD_SET(*it, &write_fds);

			reqs = select(_maxfd + 1, &read_fds, &write_fds, NULL, &timeout);
		}
		if (reqs > 0)
		{
			for (std::vector<int>::iterator it = _responses.begin(); it != _responses.end(); it++){
				if (FD_ISSET(*it, &write_fds)){
					long ret = _sockets[*it].send(*it);
					if (ret == 0)
						_responses.erase(it);
					else if (ret == -1)
					{
						FD_CLR(*it, &set_fds);
						FD_CLR(*it, &read_fds);
						_sockets.erase(*it);
						_responses.erase(it);
					}
					reqs = 0;
					break;
				}
			}

			for (std::map<long, Server>::iterator it = _sockets.begin(); it != _sockets.end(); it++){
				long sock = it->first;
				if (FD_ISSET(sock, &read_fds)){
					long ret = it->second.readfd(sock);
					if (ret == 0){
						it->second.process(sock);
						_responses.push_back(sock);
					}
					if (ret == -1){
						FD_CLR(sock, &set_fds);
						FD_CLR(sock, &read_fds);
						_sockets.erase(sock);
					}
					reqs = 0;
					break;
				}
			}

			for (std::vector<Server>::iterator it = servers_.begin(); it != servers_.end(); it++){
				if (FD_ISSET(it->getfd(), &read_fds)){
					long sock = it->accept();
					if (sock != -1){
						FD_SET(sock, &set_fds);
						_sockets.insert(std::make_pair(sock, *it));
						if (sock > _maxfd)
							_maxfd = sock;
					}
					reqs = 0;
					break;
				}
			}
		}
		else
		{
			std::cerr << BHYELLOW << "Error in select" << RESET << std::endl;
			for (std::map<long, Server>::iterator it = _sockets.begin(); it != _sockets.end(); it++)
				::close(it->first);
			_responses.clear();
			_sockets.clear();
			FD_ZERO(&set_fds);
			set_fds = backup;
		}
	}
}



