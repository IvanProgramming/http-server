#include <boost/asio.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>

#include "request.h"
#include "response.h"
#include "mimetypes.h"

Response returnHelloWorld() {
	return Response("Hello, world!");
}

std::string getFileExtension(const std::string& filePath) {
	std::size_t lastDotIndex = filePath.find_last_of(".");
	if (lastDotIndex != std::string::npos) {
		return filePath.substr(lastDotIndex + 1);
	}
	return "";  // Return an empty string if no extension found
}

std::string getFileDirectory(const std::string& filePath) {
	std::size_t lastSlashIndex = filePath.find_last_of("/");
	if (lastSlashIndex != std::string::npos) {
		return filePath.substr(0, lastSlashIndex);
	}
	return "";  // Return an empty string if no directory found
}

Response respondFile(Request request) {
	std::string filePath = request.getPath().getPath();
	// Cutting off the leading slash
	filePath = filePath.substr(1);
	std::streampos size;
	std::ifstream file;
	file.open(filePath, std::ios::binary);
	std::string extension = getFileExtension(filePath);

	if (file.is_open())
	{
		// Get the file size
		file.seekg(0, std::ios::end);
		std::streampos fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		// Read the file content into a vector
		std::vector<char> buffer(fileSize);
		if (!file.read(buffer.data(), fileSize)) {
			std::cerr << "Failed to read file." << std::endl;
			return Response("Forbidden", 403, std::map<std::string, std::string>{});
		}

		std::string mime = MimeByExtension(extension);

		std::map<std::string, std::string> headers = {
			{"Content-Type", mime},
			{"Content-Length", std::to_string(size)}
		};

		// Checking for query parameters named "cache"
		std::map<std::string, std::string> query = request.getPath().getQuery();
		
		if (query.find("cache") != query.end() && query["cache"] == "1") {
			headers["Cache-Control"] = "max-age=3600";
		}
		
		return Response(buffer, 200, headers);
	}
	else {
		if (filePath == getFileDirectory(filePath)) {
			std::cout << "Serving index.html" << std::endl;
			return RedirectResponse(filePath + "index.html");
		}
		std::cout << "File not found" << std::endl;
		return Response("File not found", 404, std::map<std::string, std::string>{});
	}
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
	
	Response res = respondFile(req);
	std::vector<char> resAsVec = res.getHTTPResponse();
	boost::asio::write(socket, boost::asio::buffer(resAsVec));

	std::chrono::microseconds reqEnd = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
	std::cout << reqEnd.count() - reqStartTime.count() << "us" << std::endl;
}

