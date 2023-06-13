![Lines of code](https://img.shields.io/tokei/lines/github/ivanprogramming/http-server)

# Async HTTP Server

This repository contains an implementation of an async HTTP server that serves files from the current directory. The server is built using CMake and includes testing using `doctest.h`.

## Usage

1. Clone the repository:

   ```
   git clone https://github.com/IvanProgramming/http-server.git
   ```

2. Build the server using CMake:

   ```
   cd http-server
   cmake .
   make
   ```

3. Run the server:

   ```
   ./HttpServer
   ```

   The server will start listening on the default port 8080.

4. Open a web browser and enter `http://localhost:8080` to access the server. The server will serve files from the current directory.

## Testing

The server includes testing using `doctest.h`, a C++ testing framework. To run the tests, follow these steps:

1. Build the server as described in the previous section.

2. Run the tests:

   ```
   ./HttpServerTests
   ```

   The tests will be executed, and the results will be displayed in the console.

## Contributing

Contributions to this project are welcome. If you find any issues or have suggestions for improvements, please open an issue or submit a pull request on the [GitHub repository](https://github.com/IvanProgramming/http-server). Make sure to follow the existing coding style and include appropriate tests for your changes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.