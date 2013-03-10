
ptime       get_pts(void);
double      get_diff(ptime t1, ptime t2);
void        status(double tdiff, size_t amount);
std::size_t send_data(tcp::socket& socket, int t_length);
std::size_t recv_data(tcp::socket& socket, int t_length);
int         send_value(tcp::socket& socket, uint32_t  value);
int         recv_value(tcp::socket& socket, uint32_t *value);
#define OK                          0
#define DATASIZE                    8192
#define DIRECTION_CLIENT_UPLOAD     1
#define DIRECTION_CLIENT_DOWNLOAD   2
#define CLIENT_VERSION              "0001"
#define min(x,y)                    ( (x) < (y) ? (x) : (y) )
