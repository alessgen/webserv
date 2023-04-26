#ifndef PARSER_HPP
# define PARSER_HPP

#include "Webserv.hpp"
#include "InfoServer.hpp"

class Parser
{
	public:
		Parser(){};
		~Parser(){};
		Parser(char *arg);
		void			openfile(char *arg);
		parsedconf		fileread(const char *arg);
		parsedconf		newsplit(std::string str, std::string delimiter);
		vector_pair		splitlocation(std::string str, std::string delimiter, vector_pair tokens, size_t &pos);
		std::string		cc_string(std::string dirty_str);
		void			setupservers(parsedconf tokens, int num_servers);

		std::fstream 				_file;
		std::vector<InfoServer>		_ourservers;
		parsedconf					tokens;
		int							num_servers;
		int							num_locations;
};

#endif