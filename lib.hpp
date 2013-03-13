
#define OK                          0
#define DATASIZE                    8192
#define DIRECTION_CLIENT_UPLOAD     1
#define DIRECTION_CLIENT_DOWNLOAD   2
#define CLIENT_VERSION              "VERSION:0001"
#define min(x,y)                    ( (x) < (y) ? (x) : (y) )
class SampleTestClient
{

public:
    SampleTestClient(const std::string& hostname) : 
        m_hostname(hostname) {};
    virtual ~SampleTestClient() {};

    // perform all operations needed to run a test
    int         run_client_test(const std::string& hostname, int time, int direction);
    int         run_server_test(tcp::socket& socket);

    // used by client to check server's version
    int         check_client_version(tcp::socket& socket);

    // used by server to report version
    int         send_version(tcp::socket& socket);

    // report the status in time, size, and rate.
    void        status(double tdiff, size_t amount);

private:
    // send/recv for t_length seconds, then stop.
    std::size_t send_data(tcp::socket& socket, int t_length);
    std::size_t recv_data(tcp::socket& socket, int t_length);

    // send/recv a single 32-bit value
    int         send_value(tcp::socket& socket, uint32_t  value);
    int         recv_value(tcp::socket& socket, uint32_t *value);

    // send/recv the given data and return success or failure.
    int         sure_send(tcp::socket& socket, const void *data, std::size_t size);
    int         sure_recv(tcp::socket& socket, void *data, std::size_t size);

    ptime       get_pts(void);
    double      get_diff(ptime t1, ptime t2);

    std::string m_hostname;
};
