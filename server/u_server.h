#ifndef U_SERVER
#define U_SERVER
#include "u_network.h"

struct server_work_in
{
    io_sockets* io;
    void(*handler)(io_sockets* io);

};

int server_work(struct server_work_in* in);
void init_server(uint16_t port, io_sockets* io);
void deinit_server(io_sockets* io);

 int howto_test (void) ;

#endif // !U_SERVER
