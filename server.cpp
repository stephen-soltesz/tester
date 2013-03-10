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
    ptime t1,t2;
    std::size_t total;
    uint32_t time=0;
    uint32_t direction=0;
    try
    {
        boost::asio::io_service io_service;
        tcp::acceptor           acceptor(io_service, tcp::endpoint(tcp::v4(), 1313));

        for (;;)
        {
            tcp::socket         socket(io_service);
            acceptor.accept(socket);

            if ( check_client_version(socket) != OK ) {
                send_version(socket);
                std::cout  << "failed to recieve current client version" << std::endl;
                continue;
            } else {
                std::cout << "Client version matches server" << std::endl;
            }
            if ( recv_value(socket, &time) != OK ) {
                std::cout  << "error receiving duration" << std::endl;
                continue;
            }
            if ( recv_value(socket, &direction) != OK ) {
                std::cout  << "error receiving direction" << std::endl;
                continue;
            }
            time = min(time, 30);
            std::cout << "Requested: <time>: " << time << 
                         " <direction>: " << direction << std::endl;

            t1 = get_pts();
            if ( direction == DIRECTION_CLIENT_UPLOAD ) {
                total = recv_data(socket, time);
            } else if ( direction == DIRECTION_CLIENT_DOWNLOAD ) {
                total = send_data(socket, time);
            } else {
                std::cerr << "Error: unknown direction: " << direction << std::endl;
                continue;
            }
            t2 = get_pts();
            std::cout << "done" << std::endl;
            status(get_diff(t1,t2), total);
            socket.close();
        }

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
