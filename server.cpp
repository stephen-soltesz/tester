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

using boost::asio::ip::tcp;
#include "lib.hpp"

std::string make_daytime_string()
{
    using namespace std; // For time_t, time and ctime;
    time_t now = time(0);
    return ctime(&now);
}

int main()
{
    double t1=0;
    double t2=0;
    std::size_t total;
    try
    {
        boost::asio::io_service io_service;

        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1313));

        //boost::array<char, 1024> buf;
        for (;;)
        {
            tcp::socket socket(io_service);
            acceptor.accept(socket);

            //std::string message = make_daytime_string();
            //boost::system::error_code ignored_error;
            printf("staring to send 10M\n");
            t1 = get_ts();
            total = send_data(socket, 10);
            t2 = get_ts();
            std::cout << "done" << std::endl;
            status(t2-t1, total);
            //boost::asio::write(socket, boost::asio::buffer(buf), boost::asio::transfer_all(), ignored_error);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
