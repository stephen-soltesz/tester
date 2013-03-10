
double get_ts(void);
std::size_t send_data(tcp::socket& socket, int t_length);
std::size_t recv_data(tcp::socket& socket, int t_length);
void status(double tdiff, size_t amount);
#define DATASIZE 8192
