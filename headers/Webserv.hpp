#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include <stdlib.h>
#include <sys/wait.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <limits>
#include "Structures.hpp"
#include "Exception.hpp"

std::string read_file(std::string cwd_, int &ext_code);

#endif
