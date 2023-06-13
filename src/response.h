#ifndef RESPONSE_H
#define RESPONSE_H

#include <map>
#include <string>
#include <vector>
#include <boost/asio.hpp>


class Response {
private:
	std::string version;
	int statusCode;
	std::string statusMessage;
	std::map<std::string, std::string> headers;
	std::vector<char> body;
public:
	Response(std::vector<char> body, int statusCode, std::map<std::string, std::string> headers);
	void SetResponseData(std::vector<char>, int statusCode, std::map<std::string, std::string> headers);
	Response(std::string body, int statusCode, std::map<std::string, std::string> headers);
	Response(std::string body);
	std::vector<char> getHTTPResponse();
};
#endif