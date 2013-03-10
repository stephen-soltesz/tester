
double get_ts(void);
int send_data(tcp::socket& socket, int t_length);
int recv_data(tcp::socket& socket, int t_length);
int status(double tdiff, size_t amount);
#define DATASIZE 8192
