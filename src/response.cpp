#include "response.h"
#include <iostream>
#include "response.h"

const std::map<int, std::string> MessageByCode{
	{200, "OK"},
	{201, "Created"},
	{202, "Accepted"},
	{204, "No Content"},
	{301, "Moved Permanently"},
	{302, "Moved Temporarily"},
	{304, "Not Modified"},
	{400, "Bad Request"},
	{401, "Unauthorized"},
	{403, "Forbidden"},
	{404, "Not Found"},
	{405, "Method Not Allowed"},
	{422, "Unprocessable Entity"},
	{500, "Internal Server Error"},
	{501, "Not Implemented"},
	{502, "Bad Gateway"},
	{503, "Service Unavailable"}
};


Response::Response(std::vector<char>, int statusCode, std::map<std::string, std::string> headers) {
	this->SetResponseData(body, statusCode, headers);
}

void Response::SetResponseData(std::vector<char> body, int statusCode, std::map<std::string, std::string> headers) {
	this->body = body;
	this->statusCode = statusCode;
	this->headers = headers;
	this->version = "HTTP/1.1";
	try {
		this->statusMessage = MessageByCode.at(statusCode);

	}
	catch (const std::out_of_range& oor) {
		this->statusMessage = "Unknown code";
	}
	std::string bodyAsStr = std::string(body.begin(), body.end());
	this->headers["Content-Length"] = std::to_string(body.size());
}

Response::Response(std::string body, int statusCode, std::map<std::string, std::string> headers) {
	// Convert string to vector<char>
	std::vector<char> bodyVector(body.begin(), body.end());
	std::cout << "Body(str): " << body << std::endl;
	this->SetResponseData(bodyVector, statusCode, headers);
}

Response::Response(std::string body) {
	std::vector<char> bodyVector(body.begin(), body.end());
	this->SetResponseData(bodyVector, 200, std::map<std::string, std::string>());
}

std::vector<char> Response::getHTTPResponse() {
	std::vector<char> response;
	std::string statusLine = this->version + " " + std::to_string(this->statusCode) + " " + this->statusMessage + "\r\n";
	response.insert(response.end(), statusLine.begin(), statusLine.end());
	for (auto const& header : this->headers) {
		std::string headerLine = header.first + ": " + header.second + "\r\n";
		response.insert(response.end(), headerLine.begin(), headerLine.end());
	}

	response.insert(response.end(), '\r');
	response.insert(response.end(), '\n');
	response.insert(response.end(), this->body.begin(), this->body.end());
	return response;
}