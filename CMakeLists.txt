cmake_minimum_required(VERSION 3.16)
project(WeeklyReport VERSION 1.0.0 LANGUAGES CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

include_directories(/usr/local/Cellar/boost/1.83.0/include)
link_directories(/usr/local/Cellar/boost/1.83.0/lib)
include_directories(/usr/local/Cellar/asio/1.28.1/include)
link_directories(/usr/local/Cellar/asio/1.28.1/lib)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Crow/include)

add_executable(cpp_weekly_report main.cpp)
find_package(SQLite3 REQUIRED)  
target_link_libraries(cpp_weekly_report ${SQLite3_LIBRARIES})
find_package(Threads REQUIRED)
target_link_libraries(cpp_weekly_report pthread)
find_package(Boost REQUIRED COMPONENTS system)
target_link_libraries(cpp_weekly_report ${Boost_LIBRARIES})


set(CMAKE_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


