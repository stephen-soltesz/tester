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
    int         ret;
    int         direction = 0;
    try {
        if (argc != 4) {
            std::cerr << "Usage: client <host> [up|down] <duration>" << std::endl;
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
        ret = run_client_test(std::string(argv[1]), 
                              boost::lexical_cast<int>(argv[3]), 
                              direction);
        if ( OK != ret ) {
            std::cerr << "Error: run_client_error" << std::endl;
            return 1;
        }

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
