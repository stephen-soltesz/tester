#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <stdio.h>
#include <boost/date_time.hpp>

using namespace boost::posix_time;
using boost::asio::ip::tcp;
#include "lib.hpp"

ptime get_pts() {
    return microsec_clock::local_time();
}

double get_diff(ptime t1, ptime t2) {
    time_duration td = t2-t1;
    return td.total_microseconds()/1.0e6;
}

double get_ts() {
/*    struct timeval tv1; 
    gettimeofday(&tv1, NULL);
    return (tv1.tv_sec + tv1.tv_usec/1.0e6); */
    return 0;
} 
#ifdef WIN32
    #define SIZEOF(x) sizeof(x)
    #define SNPRINTF _snprintf_s
#else
    #define SIZEOF(x) sizeof(x)
    #define SNPRINTF snprintf
#endif

void status(double tdiff, std::size_t amount) {
    // convert size_t to %lld for cross-platform compatibility with printf()
    char buf[128];
    SNPRINTF(buf, SIZEOF(buf), "% 7.3f sec, %8lld bytes -- %7.3f Mbps", 
             tdiff, ((long long)amount), ((8.0*amount)/tdiff)/1.0e6);
    //fprintf(stderr, "% 7.3f sec, %8lld bytes -- %7.3f Mbps\n", tdiff, ((long long)amount), ((8.0*size)/tdiff)/1.0e6);
    //fflush(stderr);
    std::cout << std::string(buf) << std::endl;
    return;
}

std::size_t send_data(tcp::socket& socket, int t_length)
{
    ptime       t1,t2;
    double      tdiff=0;
    double      last_status = 0;
    std::size_t h_sent=0;
    std::size_t ret=0;

    boost::array<char, DATASIZE>    data;
    boost::system::error_code       error;

    t1 = get_pts();
    t2 = t1;
    h_sent = 0;

    fprintf(stdout, "requested: %d\n", t_length);
    while ( get_diff(t1,t2) < t_length ) {
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
        t2 = get_pts();
        tdiff = get_diff(t1,t2);
        if ( last_status + 1 < tdiff ) {
            status(tdiff, h_sent);
            last_status = tdiff;
        }
    }
    status(tdiff, h_sent);
    return h_sent;
}

std::size_t recv_data(tcp::socket& socket, int t_length) {
    ptime       t1,t2;
    double      tdiff=0;
    double      last_status = 0;

    std::size_t h_recvd=0;
    std::size_t ret=0;

    boost::array<char, DATASIZE> data;
    boost::system::error_code error;

    t1 = get_pts();
    t2 = t1;
    h_recvd = 0;

    fprintf(stdout, "requested: %d\n", t_length);
    while ( get_diff(t1,t1) < t_length ) {
        ret = socket.receive(boost::asio::buffer(data), 0, error);
        if ( ret == 0 ) {
            fprintf(stdout, "ret==0\n");
            if ( error ) {
                std::cout << error << std::endl;
            }
            break;
        }
        h_recvd = ret + h_recvd;
        t2 = get_pts();
        tdiff = get_diff(t1, t2);
        if ( last_status + 1 < tdiff ) {
            status(tdiff, h_recvd);
            last_status = tdiff;
        }
    }
    status(tdiff, h_recvd);
    return h_recvd;
}
