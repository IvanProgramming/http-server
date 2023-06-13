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
	/*
		Constructor for Request.
		@param method The HTTP method of the request.
		@param path The path of the request.
		@param headers The headers of the request.
		@param body The body of the request.
	*/
	Request(std::string method, std::string encPath, std::map<std::string, std::string> headers, std::string body);

	/*
		Get the method of the request.
		@return The method of the request.
	*/
	std::string getMethod();

	/*
		Get the path of the request.
		@return The path of the request.
	*/
	Path getPath();

	/*
		Get the headers of the request.
		@return The headers of the request.
	*/
	std::map<std::string, std::string> getHeaders();

	/*
		Serialize the request to a string.
		@return The request as a string.
	*/
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