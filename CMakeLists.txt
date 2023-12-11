cmake_minimum_required(VERSION 3.16)
project(WeeklyReport VERSION 1.0.0 LANGUAGES CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

include_directories(/usr/local/include)
link_directories(/usr/local/lib)
include_directories(/usr/include)
link_directories(/usr/lib)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/Crow/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers/doctors)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers/patients)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers/appointments)

file(GLOB HEADERS_FILES ./headers/*.hpp 
                        ./headers/doctors/*.hpp
                        ./headeers/patients/*.hpp
                        ./headers/appointments/*.hpp)
                        

file(GLOB SOURCES_FILES ./src/*.cpp
                        ./src/doctors/*.cpp
                        ./src/patients/*.cpp
                        ./src/appointments/*.cpp)

add_executable(cpp_weekly_report main.cpp ${SOURCES_FILES} ${HEADERS_FILES})

find_package(SQLite3 REQUIRED)  
target_link_libraries(cpp_weekly_report ${SQLite3_LIBRARIES})
find_package(Threads REQUIRED)
target_link_libraries(cpp_weekly_report pthread)
find_package(Boost REQUIRED COMPONENTS system)
target_link_libraries(cpp_weekly_report ${Boost_LIBRARIES})
target_link_libraries(cpp_weekly_report uuid)


set(CMAKE_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)



