cmake_minimum_required(VERSION 3.5)

# set the project name
project(HTTPServer)
set(BOOST_ROOT "C:\\Program Files\\Boost")
set(Boost_INCLUDE_DIR "C:\\Program Files\\Boost")
set(Boost_LIBRARY_DIR "C:\\Program Files\\Boost\\libs")
link_directories(${Boost_LIBRARY_DIR})
find_package(Boost REQUIRED COMPONENTS)

# add the executable
add_executable(HTTPServer src/main.cpp src/router.cpp src/router.cpp src/request.cpp)


# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Download doctest
execute_process(
  COMMAND git clone https://github.com/onqtam/doctest.git
)

# Include the doctest header
include_directories(${CMAKE_SOURCE_DIR}/doctest)

# Add the test executable
add_executable(HTTPServerTests src/tests.cpp)

enable_testing()
# Specify the C++ standard
set_target_properties(HTTPServerTests PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)


# Include directories for boost
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/doctest)

# Link against boost libraries
target_include_directories(HTTPServerTests PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(HTTPServerTests PRIVATE ${Boost_LIBRARIES})
target_link_libraries(HTTPServer ${Boost_LIBRARIES})
