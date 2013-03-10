CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(ex CXX)

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
find_package( Boost COMPONENTS system thread )
#find_package( Boost )

# == Using actual libraries from within Boost: ==
#   find_package( Boost 1.36.0 COMPONENTS date_time filesystem system ... )
#   if(Boost_FOUND)
#      target_link_libraries(foo ${Boost_LIBRARIES})
#   endif()

if (Boost_FOUND) 
    include_directories(${Boost_INCLUDE_DIRS})
    #add_executable(ex ex.cpp)
    add_executable(server server.cpp lib.cpp)
    add_executable(client client.cpp lib.cpp)
    target_link_libraries(server ${Boost_LIBRARIES})
    target_link_libraries(client ${Boost_LIBRARIES})
else()
    message(FATAL_ERROR "Could not find boost libraries")
endif()

add_definitions(-g -O2 -fsigned-char -Wall -Wshadow 
		-Wpointer-arith -Wcast-qual -Winline -Werror)

