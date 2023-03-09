#include "Parser.hpp"

int main(int argc, char **argv){

	if (argc != 2)
	{
		std::cout << "Need two parameter!" << std::endl;
		return (1);
	}
	try {
		Parser parse(argv[1]);
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}