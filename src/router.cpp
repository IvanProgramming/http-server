#include <boost/asio.hpp>
#include <vector>
#include <iostream>
#include <chrono>

#include "request.h"
#include "response.h"

Response returnHelloWorld() {
	return Response("Hello, world!");
}

/*
	Handle a request from a client
	@param socket The socket to read from and write to
*/
void router(boost::asio::ip::tcp::socket& socket) {
	boost::asio::streambuf request;
	std::chrono::microseconds reqStartTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
	boost::asio::read_until(socket, request, "\r\n");
	std::chrono::microseconds reqParse = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());

	std::string requestAsStr = boost::asio::buffer_cast<const char*>(request.data());

	Request req = CreateRequest(requestAsStr);
	std::cout << req.getMethod() << "\t\t" << req.getPath().getPath() << "\t\t" << (reqParse.count() - reqStartTime.count()) << "us\t\t";
	
	Response res = returnHelloWorld();
	std::vector<char> resAsVec = res.getHTTPResponse();
	boost::asio::write(socket, boost::asio::buffer(resAsVec));

	std::chrono::microseconds reqEnd = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
	std::cout << reqEnd.count() - reqStartTime.count() << "us" << std::endl;
}

