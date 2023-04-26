#ifndef CTYPE_CPP
# define CTYPE_CPP

#include "Webserv.hpp"
#include "Response.hpp"

std::map<std::string, std::string> genContTypes() {
	
	std::map<std::string, std::string>	CTypes;
	CTypes.insert(strPair(".aac", "audio/aac"));
	CTypes.insert(strPair(".abw", "application/x-abiword"));
	CTypes.insert(strPair(".arc", "application/octet-stream"));
	CTypes.insert(strPair(".avi", "video/x-msvideo"));
	CTypes.insert(strPair(".azw", "application/vnd.amazon.ebook"));
	CTypes.insert(strPair(".bin", "application/octet-stream"));
	CTypes.insert(strPair(".bz", "application/x-bzip"));
	CTypes.insert(strPair(".bz2", "application/x-bzip2"));
	CTypes.insert(strPair(".csh", "application/x-csh"));
	CTypes.insert(strPair(".css", "text/css"));
	CTypes.insert(strPair(".csv", "text/csv"));
	CTypes.insert(strPair(".doc", "application/msword"));
	CTypes.insert(strPair(".epub", "application/epub+zip"));
	CTypes.insert(strPair(".gif", "image/gif"));
	CTypes.insert(strPair(".htm", "text/html"));
	CTypes.insert(strPair(".html", "text/html"));
	CTypes.insert(strPair(".ico", "image/x-icon"));
	CTypes.insert(strPair(".ics", "text/calendar"));
	CTypes.insert(strPair(".jar", "application/java-archive"));
	CTypes.insert(strPair(".jpeg", "image/jpeg"));
	CTypes.insert(strPair(".jpg", "image/jpeg"));
	CTypes.insert(strPair(".js", "application/javascript"));
	CTypes.insert(strPair(".json", "application/json"));
	CTypes.insert(strPair(".mid", "audio/midi"));
	CTypes.insert(strPair(".midi", "audio/midi"));
	CTypes.insert(strPair(".mpeg", "video/mpeg"));
	CTypes.insert(strPair(".mpkg", "application/vnd.apple.installer+xml"));
	CTypes.insert(strPair(".odp", "application/vnd.oasis.opendocument.presentation"));
	CTypes.insert(strPair(".ods", "application/vnd.oasis.opendocument.spreadsheet"));
	CTypes.insert(strPair(".odt", "application/vnd.oasis.opendocument.text"));
	CTypes.insert(strPair(".oga", "audio/ogg"));
	CTypes.insert(strPair(".ogv", "video/ogg"));
	CTypes.insert(strPair(".ogx", "application/ogg"));
	CTypes.insert(strPair(".pdf", "application/pdf"));
	CTypes.insert(strPair(".png", "image/png"));
	CTypes.insert(strPair(".ppt", "application/vnd.ms-powerpoint"));
	CTypes.insert(strPair(".rar", "application/x-rar-compressed"));
	CTypes.insert(strPair(".rtf", "application/rtf"));
	CTypes.insert(strPair(".sh", "application/x-sh"));
	CTypes.insert(strPair(".svg", "image/svg+xml"));
	CTypes.insert(strPair(".swf", "application/x-shockwave-flash"));
	CTypes.insert(strPair(".tar", "application/x-tar"));
	CTypes.insert(strPair(".tif", "image/tiff"));
	CTypes.insert(strPair(".tiff", "image/tiff"));
	CTypes.insert(strPair(".ttf", "font/ttf"));
	CTypes.insert(strPair(".vsd", "application/vnd.visio"));
	CTypes.insert(strPair(".wav", "audio/x-wav"));
	CTypes.insert(strPair(".weba", "audio/webm"));
	CTypes.insert(strPair(".webm", "video/webm"));
	CTypes.insert(strPair(".webp", "image/webp"));
	CTypes.insert(strPair(".woff", "font/woff"));
	CTypes.insert(strPair(".woff2", "font/woff2"));
	CTypes.insert(strPair(".xhtml", "application/xhtml+xml"));
	CTypes.insert(strPair(".xls", "application/vnd.ms-excel"));
	CTypes.insert(strPair(".xml", "application/xml"));
	CTypes.insert(strPair(".xul", "application/vnd.mozilla.xul+xml"));
	CTypes.insert(strPair(".zip", "application/zip"));
	CTypes.insert(strPair(".3gp", "video/3gpp"));
	CTypes.insert(strPair(".3g2", "video/3gpp2"));
	CTypes.insert(strPair(".7z", "application/x-7z-compressed"));
	return (CTypes);
}

std::string content_type(std::string extension) {
	std::map<std::string, std::string> CTypes = genContTypes();

	for (std::map<std::string, std::string>::iterator it = CTypes.begin(); it != CTypes.end(); it++) {
		if (it->first == extension)
			return (it->second);
	}
	return ("No Found");
}

std::string read_file(std::string cwd_, int &ext_code){
	std::ifstream inputFile(cwd_.c_str());
	if (!inputFile) {
		std::cerr << BHYELLOW << "Unable to open file" << RESET << std::endl;
		ext_code = 404;
		return ("");
	}
	std::string line, text;
	while (std::getline(inputFile, line)) {
		std::stringstream temp(line);
		std::string temp_l;
		getline(temp, temp_l);
		if (temp_l != "")
			text += temp_l + "\n";
	}
	inputFile.close();
	return (text);
}

std::string	check_index(std::string location_, const InfoServer &src) {
	std::string location;
	std::vector<t_location> tp = src.getLocation();
	for (std::vector<t_location>::iterator it = tp.begin(); it != tp.end(); it++)
	{
		if (!it->locationPath.compare(location_))
		{
			if (it->locationIndex != "")
			{
				location = "." + it->locationRoot + "/" + it->locationIndex;
				return location;
			}
		}
	}
	return NULL;
}

#endif