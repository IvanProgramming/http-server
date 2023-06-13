#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

// Include the file containing the code to test
#include "router.cpp"
#include "request.cpp"
#include "response.cpp"
#include "mimetypes.cpp"

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
