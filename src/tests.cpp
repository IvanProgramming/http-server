#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

// Include the file containing the code to test
#include "request.cpp"
#include "response.cpp"

TEST_CASE("Request Test Case") {
	std::string request = "GET / HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: curl/7.55.1\r\nAccept: */*\r\n\r\n";
	Request r = CreateRequest(request);

	CHECK(r.getMethod() == "GET");
	CHECK(r.getPath() == "/");
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