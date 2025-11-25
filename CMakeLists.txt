cmake_minimum_required(VERSION 3.10)
project(cpp_express_server)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system thread)
find_package(OpenSSL REQUIRED)

add_executable(server main.cpp)

target_include_directories(server PRIVATE core/ ${Boost_INCLUDE_DIRS})

target_link_libraries(server
    Boost::system
    Boost::thread
    OpenSSL::SSL
    OpenSSL::Crypto
    pthread
)
