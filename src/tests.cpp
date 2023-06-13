#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/thread.hpp>

// Include the file containing the code to test
#include "router.cpp"
#include "request.cpp"
#include "response.cpp"
#include "mimetypes.cpp"

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/thread.hpp>

void makeRequest(const std::string& host, const std::string& port, const std::string& target) {
	try {
		boost::asio::io_context io_context;

		// Resolve the host name and port number
		boost::asio::ip::tcp::resolver resolver(io_context);
		boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, port);

		// Establish a connection
		boost::asio::ip::tcp::socket socket(io_context);
		boost::asio::connect(socket, endpoints);

		// Prepare the HTTP request
		boost::beast::http::request<boost::beast::http::empty_body> req(boost::beast::http::verb::get, target, 11);
		req.set(boost::beast::http::field::host, host);
		req.set(boost::beast::http::field::user_agent, "Boost HTTP Client");

		// Send the HTTP request
		boost::beast::http::write(socket, req);

		// Receive and print the HTTP response
		boost::beast::flat_buffer buffer;
		boost::beast::http::response<boost::beast::http::dynamic_body> res;
		boost::beast::http::read(socket, buffer, res);

		std::cout << res << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

TEST_CASE("Request Test Case") {
	std::string request = "GET / HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/7.55.1\r\nAccept: */*\r\n\r\n";
	Request r = CreateRequest(request);

	CHECK(r.getMethod() == "GET");
	CHECK(r.getPath().getPath() == "/");
}

TEST_CASE("Request Exception Test Case") {
	CHECK_THROWS_AS(CreateRequest("Some invalid request"), ProcessingException);
	CHECK_THROWS_AS(CreateRequest("\r\n\r\n"), ProcessingException);
}

TEST_CASE("Response creation testcase") {
	Response r = Response(std::vector<char>{'8', 'b', 'y', 't', 'e', ' ', 'h', 'i'}, 200, std::map<std::string, std::string>{});
	auto resp = r.getHTTPResponse();
	std::string asStr = std::string(resp.begin(), resp.end());
	std::cout << asStr << std::endl;
	CHECK(asStr == "HTTP/1.1 200 OK\r\nContent-Length: 8\r\n\r\n8byte hi");
}

TEST_CASE("Response creation from string") {
	std::string body = "8byte hi";
	Response r = Response(body);
	auto resp = r.getHTTPResponse();
	std::string asStr = std::string(resp.begin(), resp.end());
	CHECK(asStr == "HTTP/1.1 200 OK\r\nContent-Length: 8\r\n\r\n8byte hi");
}

TEST_CASE("Response creationg with headers") {
	std::string body = "<b>Some html</b>";
	Response r = Response(body, 200, std::map<std::string, std::string>{ {"Content-Type", "text/html"} });
	auto resp = r.getHTTPResponse();
	std::string asStr = std::string(resp.begin(), resp.end());
	CHECK(asStr == "HTTP/1.1 200 OK\r\nContent-Length: 16\r\nContent-Type: text/html\r\n\r\n<b>Some html</b>");
}

TEST_CASE("Path parsing test") {
	Path p1 = Path("/some");
	CHECK(p1.getPath() == "/some");
	Path p2 = Path("/some?a=1&b=2");
	CHECK(p2.getPath() == "/some");
	CHECK(p2.getQuery()["a"] == "1");
	CHECK(p2.getQuery()["b"] == "2");
	Path p3 = Path("/some?a=1#hash");
	CHECK(p3.getPath() == "/some");
	CHECK(p3.getQuery()["a"] == "1");
	CHECK(p3.getHash() == "hash");
	Path p4 = Path("/some#hash");
	CHECK(p4.getPath() == "/some");
	CHECK(p4.getHash() == "hash");
}

TEST_CASE("Test URL Decoding") {
	std::string enc = "Hello%20world%20%28hi%29";
	std::string dec = DecodeURI(enc);

	CHECK(dec == "Hello world (hi)");
}


TEST_CASE("Test respond") {
	std::ofstream testFile;
	testFile.open("test.txt");
	testFile << "Hello world";
	testFile.close();

	Request req = Request("GET", "/test.txt", std::map<std::string, std::string>{}, "");

	Response r = respondFile(req);
	
	auto resp = r.getHTTPResponse();
	std::string asStr = std::string(resp.begin(), resp.end());

	CHECK(asStr == "HTTP/1.1 200 OK\r\nContent-Length: 11\r\nContent-Type: text/plain\r\n\r\nHello world");

	std::remove("test.txt");
}

TEST_CASE("Test responding 404") {
	std::ofstream testFile;
	Request req = Request("GET", "/test.txt", std::map<std::string, std::string>{}, "");

	Response r = respondFile(req);

	auto resp = r.getHTTPResponse();
	std::string asStr = std::string(resp.begin(), resp.end());
	
	CHECK(asStr == "HTTP/1.1 404 Not Found\r\nContent-Length: 14\r\n\r\nFile not found");
}


TEST_CASE("Check mimetype") {
	std::string mimetype = MimeByExtension("txt");
	CHECK(mimetype == "text/plain");
	std::string mimetype2 = MimeByExtension("abracadabra");
	CHECK(mimetype2 == "application/octet-stream");
}

TEST_CASE("Check file extension cut") {
	std::string fileName = "train_on_fire.mp3";
	std::string ext = getFileExtension(fileName);
	CHECK(ext == "mp3");
}

TEST_CASE("Check multiple connections at once") {
	const std::string host = "127.0.0.1";
	const std::string port = "8080";
	const std::string target = "/train.jpg";

	const int numRequests = 100;
	boost::thread_group threads;

	for (int i = 0; i < numRequests; ++i) {
		threads.create_thread(boost::bind(makeRequest, host, port, target));
	}

	threads.join_all();
}