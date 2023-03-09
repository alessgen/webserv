#include "Parser.hpp"

#define READSIZE 128

Parser::Parser(char *arg){
	if (openfile(arg) == 1)
		return;
}

parsedconf Parser::fileread(const char *arg) {
	parsedconf		tokens;
	char			buffer[READSIZE + 1];
	int				res = 15;
	int				fd, counter = -1;
	std::string		temp_line = "";

	for (int cn = 0; cn <= READSIZE; cn++)
		buffer[cn] = 0;
	if (strcmp(arg + (strlen(arg) - 5), ".conf"))
		throw BadExtension();
	if ((fd = open(arg, O_RDONLY)) < 1)
		throw Parser::FileNotOpen();
	for (res = 1; res > 0; res = read(fd, buffer, READSIZE)){
		buffer[res] = '\0';
		temp_line += buffer;
		counter += res;
	}
	temp_line[counter] = 0;
	if (res == -1)
		throw Parser::ReadError();
	tokens = Parser::newsplit(temp_line, "\n\t");
	return(tokens);
}

std::string Parser::cc_string(std::string str){
	for (int i = 0; str.find('\t', 0) != std::string::npos; i++)
	{
		str = str.substr(i, strlen(str.c_str()));
	}
	return str;
}

std::map<std::string, std::string> Parser::newsplit(std::string str, std::string delimiter) {
	std::map<std::string, std::string>		tokens;
	std::string::size_type pos = 0;
	std::string::size_type last_pos = 0;
	std::string temp_str, temp_str1;
	int i, temp, k, n = 0;
	while ((pos = str.find_first_of(delimiter, last_pos)) != std::string::npos) {
		if (pos > last_pos)
		{
			temp = pos + 2;
			for (i = 0; str[temp + i] != 32 && str[temp + i] != '\0' && str[temp + i] != 10; i++);
			for (k = 0; str[temp + i + k] != 10 && str[temp + i + k] != '\0'; k++);
			if (i > 0 && k > 0)
			{
				temp_str = cc_string(str.substr(temp + n, i));
				if (!temp_str.compare("location"))
					break;
				temp_str1 = cc_string(str.substr(temp + i + 1, k - 1));
				tokens.insert(std::pair<std::string, std::string>(temp_str, temp_str1));
			}
		}
		last_pos = pos + 2;
	}
	return(tokens);
}

int Parser::openfile(char *arg){
	parsedconf			parsed_conf;

	parsed_conf = fileread(arg);
	setupservers(parsed_conf);
	return (0);
}
