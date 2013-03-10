//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <stdio.h>

using namespace boost::posix_time;
using boost::asio::ip::tcp;

#include "lib.hpp"
#define _ITERATOR_DEBUG_LEVEL 0
int main(int argc, char* argv[])
{
    ptime       t1,t2;
    std::size_t total=0;
    int         time;
    int         direction = 0;
    try {
        std::cout << "std::size_t: " << sizeof(std::size_t) << std::endl;

        if (argc != 3) {
            std::cerr << "Usage: client <host> [up|down] <time>" << std::endl;
            return 1;
        }            

        if ( boost::iequals("up", argv[2]) ) {
            direction = DIRECTION_CLIENT_UPLOAD;
        } else if ( boost::iequals("down", argv[2])) {
            direction = DIRECTION_CLIENT_DOWNLOAD;
        } else {
            std::cerr << "Error: unknown direction: " << argv[2] << std::endl;
            return 1;
        }

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(std::string(argv[1]), "1313"); 
        tcp::resolver::iterator end, endpoint_iterator = resolver.resolve(query);

        time = boost::lexical_cast<int>(argv[3]);

        tcp::socket socket(io_service);
        boost::system::error_code error = boost::asio::error::host_not_found;
        while (error && endpoint_iterator != end)
        {
            socket.close();
            socket.connect(*endpoint_iterator++, error);
        }
        if (error)
            throw boost::system::system_error(error);

        std::cout  << "sending <time>: " << time;
        std::cout  << " <direction>: " << argv[2] << std::endl;
        if ( send_value(socket, time) != OK ) {
            std::cout  << "error sending data" << std::endl;
            return 1;
        }
        if ( send_value(socket, direction) != OK ) {
            std::cout  << "error sending data" << std::endl;
            return 1;
        }

        t1=get_pts();
        if ( direction == DIRECTION_CLIENT_UPLOAD ) {
            total = send_data(socket, time);
        } else if ( direction == DIRECTION_CLIENT_DOWNLOAD ) {
            total = recv_data(socket, time);
        } else {
            std::cerr << "Error: unknown direction: " << argv[2] << std::endl;
            return 1;
        }
        t2=get_pts();
        std::cout << "done" << std::endl;
        status(get_diff(t1,t2), total);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
