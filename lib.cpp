#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <stdio.h>
#include <boost/date_time.hpp>
#include <boost/algorithm/string.hpp>

using namespace boost::posix_time;
using boost::asio::ip::tcp;
#include "lib.hpp"

ptime SampleTestClient::get_pts() {
    return microsec_clock::local_time();
}

double SampleTestClient::get_diff(ptime t1, ptime t2) {
    time_duration td = t2-t1;
    return td.total_microseconds()/1.0e6;
}

int SampleTestClient::send_version(tcp::socket& socket) {
    return sure_send(socket, CLIENT_VERSION, sizeof(CLIENT_VERSION));
}
int SampleTestClient::check_client_version(tcp::socket& socket) {
    boost::array<char, sizeof(CLIENT_VERSION)>    data;
    int     ret=0;
    ret = sure_recv(socket, data.data(), data.size());
    if ( ret != OK ) {
        return -1;
    }
    if ( boost::iequals(CLIENT_VERSION, data.data()) == 1 ){
        return OK;
    }
    return -1;
}

int SampleTestClient::sure_recv(tcp::socket& socket, void *data, std::size_t size) {

    boost::system::error_code       error;
    std::size_t                     ret = 0;

    ret = socket.receive(boost::asio::buffer(data, size), 0, error);
    if ( ret != size ) {
        if ( error && error != boost::asio::error::eof) {
            std::cout << error.message() << std::endl;
        }
        return -1;
    }
    return OK;
}

/* returns success or failure. */
int SampleTestClient::sure_send(tcp::socket& socket, const void *data, std::size_t size) {

    boost::system::error_code       error;
    std::size_t                     ret = 0;

    ret = socket.send(boost::asio::buffer(data, size), 0, error);
    if ( ret != size ) {
        if ( error && error != boost::asio::error::eof) {
            std::cout << error.message() << std::endl;
        }
        return -1;
    }
    return OK;

}

int SampleTestClient::send_value(tcp::socket& socket, uint32_t value) {
    boost::system::error_code       error;
    uint32_t                        n_value=0;
    int                             ret=0;
    n_value = htonl(value); 
    ret = socket.send(boost::asio::buffer(&n_value, sizeof(n_value)), 0, error);
    if ( ret != sizeof(uint32_t) ) {
        if ( error && error != boost::asio::error::eof) {
            std::cout << error.message() << std::endl;
        }
        return -1;
    }
    return OK;
}

int SampleTestClient::recv_value(tcp::socket& socket, uint32_t *value) {
    boost::system::error_code       error;
    uint32_t                        n_value=0;
    int                             ret = 0;
    ret = socket.receive(boost::asio::buffer(&n_value, sizeof(n_value)), 0, error);
    if ( ret != sizeof(uint32_t) ) {
        if ( error && error != boost::asio::error::eof) {
            std::cout << error.message() << std::endl;
        }
        return -1;
    }
    *value = ntohl(n_value);
    return OK;
}

#ifdef WIN32
    #define SNPRINTF _snprintf_s
#else
    #define SNPRINTF snprintf
#endif

void SampleTestClient::status(double tdiff, std::size_t amount) {
    // convert size_t to %lld for cross-platform compatibility with printf()
    char buf[128];
    SNPRINTF(buf, sizeof(buf), "% 7.3f sec, %8lld bytes -- %7.3f Mbps", 
             tdiff, ((long long)amount), ((8.0*amount)/tdiff)/1.0e6);
    std::cout << std::string(buf) << std::endl;
    return;
}

std::size_t SampleTestClient::send_data(tcp::socket& socket, int t_length)
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

    while ( get_diff(t1,t2) < t_length ) {
        ret = socket.send(boost::asio::buffer(data), 0, error);
        if ( ret == 0 ) {
            if ( error && error != boost::asio::error::eof) {
                std::cout << error.message() << std::endl;
            }
            break;
        }
        h_sent += ret;
        t2 = get_pts();
        tdiff = get_diff(t1, t2);
        if ( last_status + 1 < tdiff ) {
            status(tdiff, h_sent);
            last_status = tdiff;
        }
    }
    status(tdiff, h_sent);
    return h_sent;
}

std::size_t SampleTestClient::recv_data(tcp::socket& socket, int t_length) 
{
    ptime       t1,t2;
    double      tdiff=0;
    double      last_status = 0;

    std::size_t h_recvd=0;
    std::size_t ret=0;

    boost::array<char, DATASIZE>    data;
    boost::system::error_code       error;

    t1 = get_pts();
    t2 = t1;
    h_recvd = 0;

    while ( get_diff(t1,t2) < t_length ) {
        ret = socket.receive(boost::asio::buffer(data), 0, error);
        if ( ret == 0 ) {
            if ( error && error != boost::asio::error::eof) {
                std::cout << error.message() << std::endl;
            }
            break;
        }
        h_recvd += ret;
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

int SampleTestClient::run_client_test(const std::string& hostname, int time, int direction) {
    ptime                       t1,t2;
    std::size_t                 total=0;
    boost::asio::io_service     io_service;

    tcp::resolver               resolver(io_service);
    tcp::resolver::query        query(hostname, "1313"); 
    tcp::resolver::iterator     end, endpoint_iterator = resolver.resolve(query);
    tcp::socket                 socket(io_service);
    boost::system::error_code   error = boost::asio::error::host_not_found;

    // find a connection that works
    while (error && endpoint_iterator != end) {
        socket.close();
        socket.connect(*endpoint_iterator++, error);
    }
    if (error) {
        //throw boost::system::system_error(error);
        return 1;
    }

    std::cout << " <duration>: " << time;
    std::cout << " <direction>: " << direction << std::endl;

    if ( check_client_version(socket) != OK ) {
        std::cout << "failed to recieve current version" << std::endl;
        return 1;
    } else {
        std::cout << "Client version matches server" << std::endl;
    }
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
        std::cerr << "Error: unknown direction: " << direction << std::endl;
        return 1;
    }
    t2=get_pts();
    std::cout << "done" << std::endl;
    status(get_diff(t1,t2), total);

    return OK;
}

int SampleTestClient::run_server_test(tcp::socket& socket) {

    ptime t1,t2;
    std::size_t total;
    uint32_t time=0;
    uint32_t direction=0;

    if ( send_version(socket) != OK ) {
        std::cout  << "failed to send version" << std::endl;
        return 1;
    }
    if ( recv_value(socket, &time) != OK ) {
        std::cout << "error receiving duration" << std::endl;
        return 1;
    }
    if ( recv_value(socket, &direction) != OK ) {
        std::cout << "error receiving direction" << std::endl;
        return 1;
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
        return 1;
    }
    t2 = get_pts();
    std::cout << "done" << std::endl;
    status(get_diff(t1,t2), total);

    return OK;
}
