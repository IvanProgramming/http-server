#include <iostream>
#include <boost/asio.hpp>
#include <vector>

/*
	Request handler should response in bytes
*/
std::string returnHelloWorld() {
	return "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello world!";
}

/*
	Handle a request from a client
	@param socket The socket to read from and write to
*/
void router(boost::asio::ip::tcp::socket& socket) {
	boost::asio::streambuf request;
	boost::asio::read_until(socket, request, "\r\n");

	std::string requestAsStr = boost::asio::buffer_cast<const char*>(request.data());

	std::cout << "Handling request: \n" << requestAsStr << std::endl;
	boost::asio::write(socket, boost::asio::buffer(returnHelloWorld()));
}

