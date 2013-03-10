#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <sys/time.h>
#include <stdio.h>

using boost::asio::ip::tcp;
#include "lib.hpp"

double get_ts() {
    struct timeval tv1; 
    gettimeofday(&tv1, NULL);
    return (tv1.tv_sec + tv1.tv_usec/1.0e6);
} 

int status(double tdiff, std::size_t amount) {
    fprintf(stderr, "% 7.3f sec, %8zd bytes -- %7.3f Mbps\n", tdiff, amount, ((8.0*amount)/tdiff)/1.0e6);
    return 0;
}

int send_data(tcp::socket& socket, int t_length)
{
    double      t1,t2;
    double      tdiff=0;
    double      last_status = 0;
    std::size_t h_sent=0;
    std::size_t ret=0;

    boost::array<char, DATASIZE>    data;
    boost::system::error_code       error;

    t1 = get_ts();
    t2 = t1;
    h_sent = 0;

    fprintf(stdout, "requested: %d\n", t_length);
    while ( (t2-t1) < t_length ) {
        ret = socket.send(boost::asio::buffer(data), 0, error);
        if ( ret == 0 ) {
            perror("sending error:");
            fprintf(stdout, "ret==0\n");
            if ( error ) {
                std::cout << error << std::endl;
            }
            break;
        }
        h_sent += ret;
        t2 = get_ts();
        tdiff = t2-t1;
        if ( last_status + 1 < tdiff ) {
            status(tdiff, h_sent);
            //fprintf(stdout, "status: %.6f %zd\n", tdiff, h_sent);
            last_status = tdiff;
        }
    }
    status(tdiff, h_sent);
    return 0;
}

int recv_data(tcp::socket& socket, int t_length) {
    double      t1,t2;
    double      tdiff=0;
    double      last_status = 0;

    std::size_t h_recvd=0;
    std::size_t ret=0;

    boost::array<char, DATASIZE> data;
    boost::system::error_code error;

    t1 = get_ts();
    t2 = t1;
    h_recvd = 0;

    fprintf(stdout, "requested: %d\n", t_length);
    while ( (t2-t1) < t_length ) {
        ret = socket.receive(boost::asio::buffer(data), 0, error);
        if ( ret == 0 ) {
            fprintf(stdout, "ret==0\n");
            if ( error ) {
                std::cout << error << std::endl;
            }
            break;
        }
        h_recvd = ret + h_recvd;
        t2 = get_ts();
        tdiff = t2-t1;
        if ( last_status + 1 < tdiff ) {
            status(tdiff, h_recvd);
            last_status = tdiff;
        }
    }
    status(tdiff, h_recvd);

    return 0;
}
