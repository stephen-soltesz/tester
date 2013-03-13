//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <stdio.h>

using namespace boost::posix_time;
using boost::asio::ip::tcp;
#include "lib.hpp"

int main()
{
    try
    {
        boost::asio::io_service io_service;
        tcp::acceptor           acceptor(io_service, tcp::endpoint(tcp::v4(), 1313));

        SampleTestClient stc("");
        while (1) 
        {
            tcp::socket         socket(io_service);
            acceptor.accept(socket);
            stc.run_server_test(socket);
            socket.close();
        }

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
