#include "main.h"
#include "router.h"

bool shouldExit = false;

/*
	Signal handler for SIGINT (Ctrl+C)
	@param signal The signal that was received
*/
void handleSignal(int signal) {
	if (signal == SIGINT) {
		shouldExit = true;
	}
}


/*
	Main function, where the program starts.
*/
int main() {
	try {
		boost::asio::io_context io_context;
		// Getting port from environment variable
		int port = 8080;

		// Creating acceptor
		tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));

		std::cout << "Server started on port " << port << std::endl;

		// Register the signal handler for SIGINT (Ctrl+C)
		std::signal(SIGINT, handleSignal);

		std::cout << "Press Ctrl+C to exit" << std::endl;
		std::cout << "Method" << "\t\t" << "Path" << "\t\t" << "Parsed in" << "\t" << "Response in" << std::endl;
		std::cout << "-----------------------------------------------------------------" << std::endl;

		while (!shouldExit) {
			// Wait for a new connection
			tcp::socket socket(io_context);
			acceptor.accept(socket);

			// Handle the request from the connected client
			router(socket);

			// Close the socket
			socket.close();
		}


	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}