#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>
#include "router.h"

using boost::asio::ip::tcp;

bool shouldExit = false;

/*
    Signal handler for SIGINT (Ctrl+C)
    @param signal The signal that was received
*/
void handleSignal(const boost::system::error_code& error, int signal) {
    if (!error && signal == SIGINT) {
        shouldExit = true;
    }
}

// Start accepting incoming connections
void startAccept(boost::asio::ip::tcp::acceptor& acceptor, boost::asio::io_context& context)
{
    // Create a new socket for each accepted connection
    auto socket = std::make_shared<tcp::socket>(context);

    acceptor.async_accept(*socket,
        [&, socket](const boost::system::error_code& ec) {
            if (!ec) {
                router(*socket);
            }
            else {
                // Handle the error or log it
                std::cerr << "Error accepting connection: " << ec.message() << std::endl;
            }

            // Start accepting the next connection
            startAccept(acceptor, context);
        });
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
        boost::asio::signal_set signals(io_context, SIGINT);
        signals.async_wait(boost::bind(handleSignal,
            boost::asio::placeholders::error, boost::asio::placeholders::signal_number));

        std::cout << "Press Ctrl+C to exit" << std::endl;
        std::cout << "Method" << "\t\t" << "Path" << "\t\t" << "Parsed in" << "\t" << "Response in" << std::endl;
        std::cout << "-----------------------------------------------------------------" << std::endl;

        tcp::socket socket(io_context);
        startAccept(acceptor, io_context);

        // Run the IO context in a loop until shouldExit is set to true
        while (!shouldExit) {
            io_context.run();
            io_context.restart();
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
