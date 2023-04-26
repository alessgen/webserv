#include "Parser.hpp"
#include "SetupCluster.hpp"

int main(int argc, char **argv){
	if (argc != 2)
	{
		std::cout << BHRED << "Need two parameter!" << RESET << std::endl;
		return (1);
	}
	try
	{
		Parser parse(argv[1]);
		SetupCluster cluster(parse._ourservers);
		try {
			cluster.setup();
		}
		catch (const ServerEX::exception &res) {
			std::cerr << BHRED << res.what() << RESET << std::endl;
		}
		cluster.run();
	}
	catch (const ParserEX::exception &res) {
		std::cerr << BHRED << res.what() << RESET << std::endl;
		return (1);
	}
	return (0);
}
