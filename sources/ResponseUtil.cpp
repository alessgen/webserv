#include "Response.hpp"

void Response::constructor_assign(Request &request, InfoServer &src){
	location_ = "." + src.getRoot() + request.getReqRoot();
	method_ = request.getReqMethod();
	protocol = request.getReqProtocol();
	ext_code = 200;
}

bool Response::constructor_util(Request &request,  InfoServer &src){
	_infoserver = src;
	this->errorpages = src.getErrorPage();
	c_lenght = request.getContentLength();
	server_port = request.getServerPort();
	_headers = request.getReqHeaders();
	postArg = request.getPostargs();
	response_ok = false;
	method_ = request.getReqMethod();
	protocol = request.getReqProtocol();
	location_ = "";
	if (!request.getReqRoot().compare("/favicon.ico")) {
		constructor_assign(request, src);
		return false;
	}
	return true;
}

bool Response::constructor_chk_locations(Request &request,  InfoServer &src){
	std::vector<t_location> tp = src.getLocation();
	for (std::vector<t_location>::iterator it = tp.begin(); it != tp.end(); it++)
	{
		std::string serv_root = src.getRoot();
		std::string req_root = request.getReqRoot();
		if (it->locationPath != "/")
		{
			req_root = serv_root + req_root;
			serv_root = serv_root + it->locationPath;
		}
		else
			req_root = serv_root + req_root;
		if (!req_root.compare(serv_root))
		{
			location_ = serv_root;
			if (request.getReqMethod() == it->locationMethod) {
				method_ = request.getReqMethod();
				protocol = request.getReqProtocol();
				ext_code = 200;
				return true;
			}
			break;
		}
	}
	return false;
}

Response::Response(Request &request, InfoServer &src) {
	if (constructor_util(request, src) == false)
		return;
	std::set<std::string> temp = src.getmethod();
	for (std::set<std::string>::iterator it = temp.begin(); it != temp.end(); it++)
	{
		if (!((*it).compare(request.getReqMethod())) && !((*it).compare("DELETE")))
		{
			constructor_assign(request, src);
			return;
		}
		else if (request.getReqMethod() == *it)
		{
			std::vector<t_location> check = src.getLocation();
			for (std::vector<t_location>::iterator it = check.begin(); it != check.end(); it ++) {
				if (!request.getReqRoot().compare(it->locationPath + it->locationIndex)) {
					constructor_assign(request, src);
					return;
				}
			}
			if (!request.getReqRoot().compare("/")) {
				location_ = src.getRoot();
				this->location_ = check_index("/", src);
				method_ = request.getReqMethod();
				protocol = request.getReqProtocol();
				ext_code = 200;
				return;
			}
			else
				if (constructor_chk_locations(request, src) == true)
					return;	
		}
	}
	ext_code = 403;
}