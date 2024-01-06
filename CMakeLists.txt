cmake_minimum_required(VERSION 3.27)
project(mysql)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)

# Include directories for MySQL Connector/C++
include_directories("mysql-connector/include")

# Find the MySQL Connector/C++ library
find_library(CONN_LIB NAMES mysqlcppconn PATHS "mysql-connector/lib")

add_executable(mysql main.cpp)

target_link_libraries(mysql ${CONN_LIB})

# Automatically copy DLL files from the specified directory
# When in production you can only copy "mysqlcppconn-9-vs14.dll"
set(DLL_DIRECTORY "mysql-connector/lib")
file(GLOB DLL_FILES "${DLL_DIRECTORY}/*.dll")
foreach(DLL_FILE IN LISTS DLL_FILES)
    add_custom_command(TARGET mysql POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DLL_FILE} $<TARGET_FILE_DIR:mysql>)
endforeach()
