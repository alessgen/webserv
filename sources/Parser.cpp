#include "Parser.hpp"

#define READSIZE 128

Parser::Parser(char *arg){
	openfile(arg);
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
		throw ParserEX::BadExtension();
	if ((fd = open(arg, O_RDONLY)) < 1)
		throw ParserEX::FileNotOpen();
	for (res = 1; res > 0; res = read(fd, buffer, READSIZE)){
		buffer[res] = '\0';
		temp_line += buffer;
		counter += res;
	}
	temp_line[counter] = 0;
	if (res == -1)
		throw ParserEX::ReadError();
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

parsedconf Parser::newsplit(std::string str, std::string delimiter) {
	this->num_servers = 1;
	this->num_locations = 0;
	parsedconf		tokens;
	vector_pair		vec;
	std::string		temp_str, temp_str1, checker, del_ = "server {";
	std::string::size_type pos = 0, last_pos = 0, temp = 0;
	int i, k, check = 0;
	while ((pos = str.find_first_of(delimiter, last_pos)) != std::string::npos) {
		checker = str.substr(pos - 8, pos);
		if (!checker.compare(0, 8, del_) && check == 1)
		{
			tokens.push_back(vec);
			vec.erase(vec.begin(), vec.end());
			this->num_servers++;
		}
		if (pos >= last_pos)
		{
			temp = pos + 2;
			for (i = 0; str[temp + i] != 32 && str[temp + i] != '\0' && str[temp + i] != 10; i++);
			for (k = 0; str[temp + i + k] != 10 && str[temp + i + k] != '\0'; k++);
			if (i > 0 && k > 0)
			{
				temp_str = "";
				temp_str1 = "";
				if (!cc_string(str.substr(temp, i)).compare("location"))
				{
					temp_str = cc_string(str.substr(temp, i));
					temp_str1 = cc_string(str.substr(temp + i + 1, k - 1));
					vec.push_back(make_pair(temp_str, temp_str1));
					vec = splitlocation(str, delimiter, vec, temp);
					this->num_locations++;
				}
				else
				{
					check = 1;
					temp_str = cc_string(str.substr(temp, i));
					temp_str1 = cc_string(str.substr(temp + i + 1, k - 1));
					vec.push_back(make_pair(temp_str, temp_str1));
				}
				last_pos = temp;
			}
		}
		else
			last_pos = pos + 2;
	}
	tokens.push_back(vec);
	return(tokens);
}

vector_pair Parser::splitlocation(std::string str, std::string delimiter, vector_pair tokens, size_t &pos)
{
	pos += 8;
	std::string::size_type last_pos = pos;
	std::string temp_str, temp_str1;
	int i, temp, k, n = 0;
	while ((pos = str.find_first_of(delimiter, last_pos)) != std::string::npos) {
	size_t a = str.find("server {", last_pos);
	if (a < pos && a != std::string::npos)
		break;
	if (pos > last_pos)
	{
		temp = pos + 2;
		for (i = 0; str[temp + i] != 32 && str[temp + i] != '\0' && str[temp + i] != 10; i++);
		for (k = 0; str[temp + i + k] != 10 && str[temp + i + k] != '\0'; k++);
		if (i > 0 && k > 0)
		{
			temp_str = "";
			temp_str1 = "";
			temp_str = cc_string(str.substr(temp + n, i));
			temp_str1 = cc_string(str.substr(temp + i + 1, k - 1));
			tokens.push_back(make_pair(temp_str, temp_str1));
		}
	}
		last_pos = pos + 2;
	}
	if (pos == std::string::npos)
		pos = last_pos;
	return (tokens);
}

void Parser::setupservers(parsedconf tokens, int num_servers) {
	_ourservers.reserve(num_servers);
	for (int i = 0; i < num_servers; i++)
		_ourservers.push_back(InfoServer(tokens, i));
}

void Parser::openfile(char *arg){
	parsedconf		parsed_conf;
	parsed_conf = fileread(arg);

	setupservers(parsed_conf, this->num_servers);
}
