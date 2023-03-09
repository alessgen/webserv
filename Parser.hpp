#ifndef PARSER_HPP
# define PARSER_HPP

#include "webserv.hpp"

class Parser
{
	public:
	// Attributes
		std::fstream 			_file;
		std::vector<InfoServer>	_ourservers;
		parsedconf				tokens;
	// Member Function
		Parser(){};
		Parser(char *arg);
		~Parser(){};
		int									openfile(char *arg);
		parsedconf							fileread(const char *arg);
		std::map<std::string, std::string>	newsplit(std::string str, std::string delimiter);
		std::string							cc_string(std::string dirty_str);
		//da vedere
		void								setupservers(parsedconf tokens){
			_ourservers.push_back(InfoServer(tokens));
		};

		class BadExtension : public std::exception {
			public:
				const char* what() const throw() {
					return ("Bad Extension, file must be a .conf file!");
				}
		};

		class FileNotOpen : public std::exception {
			public:
				const char *what() const throw() {
					return "Could not open the file";
				}
		};

		class ReadError : public std::exception {
			public:
				const char *what() const throw() {
					return "Error in reading file.";
				}
		};
};

#endif