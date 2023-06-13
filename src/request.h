#ifndef REQUEST_H
#define REQUEST_H

#include <map>
#include <string>

std::string EncodeURI(std::string uri);
std::string DecodeURI(std::string uri);

class Path {
private:
	std::string path;
	std::map<std::string, std::string> query;
	std::string hash;
public:
	Path(std::string path);
	Path();
	std::string getPath();
	std::map<std::string, std::string> getQuery();
	std::string getHash();
	void setPath(std::string path);
};


/*
	Request represents a request from a client to a server.
*/
class Request {
private:
	std::string method;
	Path path;
	std::map<std::string, std::string> headers;
	std::string body;

public:
	Request(std::string method, std::string encPath, std::map<std::string, std::string> headers, std::string body);
	std::string getMethod();
	Path getPath();
	std::map<std::string, std::string> getHeaders();
	std::string toString();

};

Request CreateRequest(std::string requestString);

class ProcessingException {
public:
	ProcessingException(std::string message, int httpCode);
private:
	std::string message;
	int httpCode;
};

#endif