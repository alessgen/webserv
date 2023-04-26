#ifndef UTILS_CPP
# define UTILS_CPP

#include "Webserv.hpp"
#include "Response.hpp"

void setConnection(std::string &connect) {
	connect = "Connection: keep-alive";
}

void setServName(std::string &serv_name) {
	serv_name = "Server Name: \U0001F47D WeedServ \U0001F47D ♛1.0(Unix)♛";
}

void setDate(std::string &date) {
	char			buffer[100];
	struct timeval	tv;
	struct tm		*tm;
	
	gettimeofday(&tv, NULL);
	tm = gmtime(&tv.tv_sec);
	strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
	date = "Date: " + std::string(buffer);
}

void setLastModified(std::string location_, std::string &last_modif) {
	char			buffer[100];
	struct stat		stats;
	struct tm		*tm;

	if (stat(location_.c_str(), &stats) == 0)
	{
		tm = gmtime(&stats.st_mtime);
		strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", tm);
		last_modif = "Last Modified: " + std::string(buffer);
	}
	else
		perror("Error");
}

#endif