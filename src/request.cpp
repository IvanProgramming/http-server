#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>

#include "request.h"

/*
	@brief Constructor for Path.
	@param path The path of the request.
*/
Path::Path(std::string path) {
	std::string pathPart = "";
	std::string queryPart = "";
	std::string hash = "";

	std::string::size_type pos = path.find('?');
	std::string::size_type pos2 = path.find('#');
	if (pos == std::string::npos && pos2 == std::string::npos) {
		pathPart = path;
	}
	else if (pos != std::string::npos && pos2 == std::string::npos) {
		pathPart = path.substr(0, pos);
		queryPart = path.substr(pos + 1);
	}
	else if (pos == std::string::npos && pos2 != std::string::npos) {
		pathPart = path.substr(0, pos2);
		hash = path.substr(pos2 + 1);
	}
	else {
		pathPart = path.substr(0, pos);
		queryPart = path.substr(pos + 1, pos2 - pos - 1);
		hash = path.substr(pos2 + 1);
	}

	this->path = DecodeURI(pathPart);
	this->query = std::map<std::string, std::string>{};
	if (queryPart != "") {
		std::string::size_type nextPos;
		do {
			nextPos = queryPart.find('&');
			std::string::size_type pos = queryPart.find('=');
			std::string key = queryPart.substr(0, pos);
			std::string value = queryPart.substr(pos + 1, nextPos - pos - 1);
			this->query[key] = DecodeURI(value);
			queryPart.erase(0, nextPos + 1);
		} while (nextPos != std::string::npos
);
	}
	this->hash = hash;
}


/*
	@brief Default constructor for Path.
*/
Path::Path() {
	// Initialize member variables with appropriate default values
	this->path = "";
	this->query = std::map<std::string, std::string>{};
	this->hash = "";
}

/*
	@brief Get the path of the request.
	@return The path of the request.
*/
std::string Path::getPath() {
	return path;
}


/*
	@brief Get the query of the request.
	@return The query of the request.
*/
std::map<std::string, std::string> Path::getQuery() {
	return query;
}

/*
	@brief Get the hash of the request.
	@return The hash of the request.
*/
std::string Path::getHash() {
	return hash;
}


/*
	@brief Set the path of the request.
	@param path The path of the request.
*/
void Path::setPath(std::string path)
{
	this->path = path;
}

/*
	@brief Set the query of the request.
	@param query The query of the request.
*/
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
Request::Request(std::string method, std::string encPath, std::map<std::string, std::string> headers, std::string body) {
	this->path = Path(encPath);
	this->headers = headers;
	this->body = body;
	if (std::find(ALLOWED_METHODS.begin(), ALLOWED_METHODS.end(), method) == ALLOWED_METHODS.end()) {
		throw(ProcessingException("Invalid method", 405));
	}
	this->method = method;
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
Path Request::getPath() {
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
	debugValue += "Path: " + this->path.getPath() + "\n";
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


/*
	@brief Encode URI.
	@param uri The URI to encode.
	@return The encoded URI.
*/
std::string EncodeURI(std::string uri) {
	// (Start) Source: https://stackoverflow.com/questions/154536/encode-decode-urls-in-c
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (std::string::const_iterator i = uri.begin(), n = uri.end(); i != n; ++i) {
		std::string::value_type c = (*i);

		// Keep alphanumeric and other accepted characters intact
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
			escaped << c;
			continue;
		}

		// Any other characters are percent-encoded
		escaped << std::uppercase;
		escaped << '%' << std::setw(2) << int((unsigned char)c);
		escaped << std::nouppercase;
	}

	return escaped.str();
	// (End) Source: https://stackoverflow.com/questions/154536/encode-decode-urls-in-c
}


/*
	@brief Decode URI.
	@param uri The URI to decode.
	@return The decoded URI.
*/
std::string DecodeURI(std::string uri) {
	// (Start) Source: https://stackoverflow.com/questions/154536/encode-decode-urls-in-c
	std::string ret;
	char ch;
	int i, ii, len = uri.length();

	for (i = 0; i < len; i++) {
		if (uri[i] != '%') {
			if (uri[i] == '+')
				ret += ' ';
			else
				ret += uri[i];
		}
		else {
			sscanf(uri.substr(i + 1, 2).c_str(), "%x", &ii);
			ch = static_cast<char>(ii);
			ret += ch;
			i = i + 2;
		}
	}
	return ret;
	// (End) Source: https://stackoverflow.com/questions/154536/encode-decode-urls-in-c
}


