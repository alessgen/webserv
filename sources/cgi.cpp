#ifndef CGI_CPP
# define CGI_CPP

#include "Webserv.hpp"
#include "Response.hpp"


// Transform a Vector of string into a char** that contain the Environmental variable of CGI ENV

char** vec_to_env(std::vector<std::string> arg) {
	char	**vec_env;

	vec_env = new char*[arg.size() + 1];
	for (size_t i = 0; i < arg.size(); i++)
	{
		vec_env[i] = new char[arg[i].size() + 1];
		strcpy(vec_env[i], arg[i].c_str());
	}
	vec_env[arg.size()] = NULL;
	return (vec_env);
}

// This function setup the CGI ENV 

std::vector<std::string> create_cgi_env(char *path, std::string method_, std::string protocol, std::map<std::string, std::string> _headers) {
	std::vector<std::string> 								vec_env;
	std::map<std::string, std::string>::const_iterator		it;

	it = _headers.find("Host");
	if (it != _headers.end())
		vec_env.push_back(std::string("SERVER_NAME=") + it->second.substr(1));
	vec_env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	vec_env.push_back("SERVER_PROTOCOL=" + protocol);
	vec_env.push_back("SERVER_PORT=" + _headers["Host"].substr(11, 4));
	vec_env.push_back("REQUEST_METHOD=" + method_);
	char pwd[PATH_MAX];
	getcwd(pwd, PATH_MAX);
	vec_env.push_back(std::string("PATH_TRANSLATED=") + pwd + "/" + path);
	vec_env.push_back("SCRIPT_NAME=upload.py");	
	it = _headers.find("Content-Type");
		vec_env.push_back(std::string("CONTENT_TYPE=" + it->second.substr(1)));
	it = _headers.find("Content-Length");
	if (it != _headers.end())
	vec_env.push_back("CONTENT_LENGTH=" + it->second.substr(1));
	it = _headers.find("Accept");
	if (it != _headers.end())
	{
		vec_env.push_back("HTTP_ACCEPT=" + it->second.substr(1));
	}
	it = _headers.find("Accept-Language");
	if (it != _headers.end())
	{
		vec_env.push_back("HTTP_ACCEPT_LANGUAGE=" + it->second.substr(1));
	}
	it = _headers.find("User-Agent");
	if (it != _headers.end())
	{
		vec_env.push_back("HTTP_USER_AGENT=" + it->second.substr(1));
	}
	it = _headers.find("Referer");
	if (it != _headers.end())
	{
		vec_env.push_back("HTTP_REFERER=" + it->second.substr(1));
	}
	return (vec_env);
}

void exec_cgi(char *path, int fd_in[2],int fd_out[2], char **env) {
	char	*av[3];
	
	av[0] = (char *)"/usr/bin/python3";
	av[1] = path;
	av[2] = NULL;
	dup2(fd_in[0], 0);
	dup2(fd_out[1], 1);
	close(fd_out[0]);
	close(fd_out[1]);
	close(fd_in[1]);
	close(fd_in[0]);
	execve(av[0], av, env);
	std::string temp = "That's the path received :";
	std::string temp2 = "\nThe error is";
	temp += av[0];
	temp += temp2;
	perror(temp.c_str());
	exit(1);
}

//  Function that call a Exec_CGI function to execve the cgi script.
std::string execPost(char *path, Response &src){
	int			fd_in[2];
	int			fd_out[2];
	char		tmp[11] = {0};
	std::string	return_string;

	pipe(fd_in);
	pipe(fd_out);
	if (!fork())
	{
		char **env = vec_to_env(create_cgi_env(path, src.getMethod(), src.getProtocol(), src.getHeaders()));
		exec_cgi(path, fd_in, fd_out, env);
	}
	dup2(fd_in[0], 0);
	if (src.getPostarg())
		write(fd_in[1], src.getPostarg(), src.getContentlength());
	close(fd_in[1]);
	close(fd_in[0]);
	close(fd_out[1]);
	wait(0);
	int r = read(fd_out[0], tmp, 10);
	return_string += std::string(tmp);
	while (r > 0)
	{
		r = read(fd_out[0], tmp, 10);
		tmp[r] = 0;
		return_string += std::string(tmp);
	}
	return (return_string);
}

#endif
