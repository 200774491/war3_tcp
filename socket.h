#ifndef SOCKET_HANDLER_H
#define SOCKET_HANDLER_H

int init_socket(const char* ip, int port);
int send_data(const char* data);
const char* get_message();
void close_socket();

#endif
