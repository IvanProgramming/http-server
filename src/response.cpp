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

/*!
*	@brief Constructor for Response class
*	@param body The body of the response.
*	@param statusCode The status code of the response.
*	@param headers The headers of the response.
*/
Response::Response(std::vector<char> body, int statusCode, std::map<std::string, std::string> headers) {
	this->SetResponseData(body, statusCode, headers);
}


/*!
*	@brief Set the data of the response.
*	@param body The body of the response.
*	@param statusCode The status code of the response.
*	@param headers The headers of the response.
*/
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


/*!
*	@brief Constructor for Response class
*	@param body The body of the response.
*	@param statusCode The status code of the response.
*	@param headers The headers of the response.
*/
Response::Response(std::string body, int statusCode, std::map<std::string, std::string> headers) {
	std::vector<char> bodyVector(body.begin(), body.end());
	this->SetResponseData(bodyVector, statusCode, headers);
}

/*!
*	@brief Constructor for Response class
*	@param body The body of the response.
*	@param statusCode The status code of the response.
*	@param headers The headers of the response.
*/
Response::Response(std::string body) {
	std::vector<char> bodyVector(body.begin(), body.end());
	this->SetResponseData(bodyVector, 200, std::map<std::string, std::string>());
}


/*!
*	@brief Get the HTTP response for the response object.
*	@return The HTTP response in a vector of chars (bytes).
*/
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

/*!
*	@brief Redirect to a new location.
*	@param location The location to redirect to.
*/
Response RedirectResponse(std::string location)
{
	return Response("Moving to " + location, 301, { {"Location", location} });
}
