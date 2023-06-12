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

# Include directories for boost
include_directories(${Boost_INCLUDE_DIRS})

# Link against boost libraries
target_link_libraries(HTTPServer ${Boost_LIBRARIES})
