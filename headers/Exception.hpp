#ifndef EXCEPTION_HPP
#	define EXCEPTION_HPP

#include <iostream>

class ParserEX : public std::exception{
	public:
		class BadExtension : public std::exception{
			public:
				const char* what() const throw() {
					return "Bad Extension, argument file must be a .conf file!";
				}
		};

		class FileNotOpen : public std::exception{
			public:
				const char* what() const throw() {
					return "Could not open the file";
				}
		};

		class ReadError : public std::exception{
			public:
				const char* what() const throw() {
					return "Error in reading file";
				}
		};
};

class ServerEX : public std::exception{
	public:
		class SockError{
			public:
				SockError(const std::string temp) : _str("Error in socket: " + temp){};
				const char* what() const throw() {
					return _str.c_str();
				}
			public:
				std::string _str;
		};

		class SetSockError{
			public:
				SetSockError(const std::string temp) : _str("Error in setsockopt: " + temp){};
				const char* what() const throw() {
					return _str.c_str();
				}
			public:
				std::string _str;
		};

		class ListenError {
			public:
				ListenError(const std::string temp) : _str("Error in listen: " + temp){};
				const char* what() const throw() {
					return _str.c_str();
				}
			public:
				std::string _str;
		};
};

#endif