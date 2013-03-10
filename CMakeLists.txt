CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(ex CXX)


set(Boost_DEBUG 1)

if(WIN32)
    SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/Program Files/boost/boost_1_51/boost")
    SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/Program Files/boost/boost_1_51/lib")
endif(WIN32)

set(Boost_USE_STATIC_LIBS    ON)
set(Boost_USE_STATIC_RUNTIME ON)
set(Boost_USE_MULTITHREADED  ON)

find_package( Boost COMPONENTS system )

if (Boost_FOUND) 
    include_directories(${Boost_INCLUDE_DIRS})
    #add_executable(ex ex.cpp)
    add_executable(server server.cpp lib.cpp)
    add_executable(client client.cpp lib.cpp)
    target_link_libraries(server ${Boost_LIBRARIES})
    target_link_libraries(client ${Boost_LIBRARIES})
    message("LIBPATH: " ${CMAKE_LIBRARY_PATH})
else()
    message(FATAL_ERROR "Could not find boost libraries here!")
endif()


if(WIN32)
    message("What compiler flags do we want?")
    add_definitions(-D_WIN32_WINNT=0x0501)
else()
    add_definitions(-g -O2 -fsigned-char -Wall -Wshadow 
		        -Wpointer-arith -Wcast-qual -Winline -Werror)
endif(WIN32)
