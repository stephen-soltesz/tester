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
#include <sys/time.h>
#include <stdio.h>
using boost::asio::ip::tcp;

#include "lib.hpp"

int main(int argc, char* argv[])
{
    double t1=0;
    double t2=0;
    std::size_t total;
    try {
        std::cout << "std::size_t: " << sizeof(std::size_t) << std::endl;

        if (argc != 3) {
            std::cerr << "Usage: client <host>" << std::endl;
            return 1;
        }            
        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(std::string(argv[1]), "1313"); 
        tcp::resolver::iterator end, endpoint_iterator = resolver.resolve(query);

        tcp::socket socket(io_service);
        boost::system::error_code error = boost::asio::error::host_not_found;
        while (error && endpoint_iterator != end)
        {
            socket.close();
            socket.connect(*endpoint_iterator++, error);
        }
        if (error)
            throw boost::system::system_error(error);

        t1=get_ts();
        total = recv_data(socket, 10);
        t2=get_ts();
        status(t2-t1, total);
        std::cout << "done" << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
