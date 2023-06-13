#include <map>
#include <string>
#include <iostream>
#include <vector>

#include "request.h"

ProcessingException::ProcessingException(std::string message, int httpCode) {
	this->message = message;
	this->httpCode = httpCode;
}

// Allowed HTTP methods
const std::vector<std::string> ALLOWED_METHODS = { "GET", "POST", "PUT", "DELETE" };


/*
	Constructor for Request.
	@param method The HTTP method of the request.
	@param path The path of the request.
	@param headers The headers of the request.
	@param body The body of the request.
*/
Request::Request(std::string method, std::string path, std::map<std::string, std::string> headers, std::string body) {
	if (std::find(ALLOWED_METHODS.begin(), ALLOWED_METHODS.end(), method) == ALLOWED_METHODS.end()) {
		throw(ProcessingException("Invalid method", 405));
	}
	this->method = method;
	this->path = path;
	this->headers = headers;
	this->body = body;
}

/*
	Get the method of the request.
	@return The method of the request.
*/
std::string Request::getMethod() {
	return this->method;
}

/*
	Get the path of the request.
	@return The path of the request.
*/
std::string Request::getPath() {
	return this->path;
}

/*
	Get the headers of the request.
	@return The headers of the request.
*/
std::map<std::string, std::string> Request::getHeaders() {
	return this->headers;
}

/*
	Serialize the request to a string.
	@return The request as a string.
*/
std::string Request::toString() {
	std::string debugValue = "";

	debugValue += "Method: " + this->method + "\n";
	debugValue += "Path: " + this->path + "\n";
	for (std::map<std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); ++it) {
		debugValue += it->first + ": " + it->second + "\n";
	}

	return debugValue;
}


/* 
	Creates request from the given string
	@param requestString The string to be parsed
	@return The request object
*/
Request CreateRequest(std::string requestString) {

	std::string method;
	std::string path;
	std::map<std::string, std::string> headers;
	std::string body;

	// Parsing request metod and path
	std::string::size_type pos = requestString.find(' ');

	if (pos == std::string::npos)  {
		throw(ProcessingException("Error parsing request", 400));

		// Invalid request
		return Request("", "", headers, body);
	}

	method = requestString.substr(0, pos);

	// Removing method from request string, so we can parse path
	requestString.erase(0, pos + 1);
	pos = requestString.find(' ');

	if (pos == std::string::npos) {
		throw(ProcessingException("Error parsing request", 400));

		// Invalid request
		return Request("", "", headers, body);
	}

	path = requestString.substr(0, pos);
	
	// Removing path from request string, so we can parse headers
	requestString.erase(0, pos + 1);
	
	// Any other data is in headers
	while (true) {
		if (requestString.find("\r\n") == 0) {
			// End of headers
			requestString.erase(0, 2);
			break;
		}
		else {
			std::string headerName;
			std::string headerValue;

			headers[headerName] = headerValue;

			int lineEndPos = requestString.find("\r\n");

			if (lineEndPos == std::string::npos) {
				// End of headers
				break;
			}

			// Removing header from request string, so we can parse next header
			requestString.erase(0, lineEndPos + 2);
		}
	}

	// Body is the rest of the request string
	body = requestString;
	return Request(method, path, headers, body);
}