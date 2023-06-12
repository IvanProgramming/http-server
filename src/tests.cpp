#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

// Include the file containing the code to test
#include "request.cpp"

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