#ifndef U_SERVER
#define U_SERVER
#include <u_network.h>
#include <stdlib.h>
#include <string.h>

typedef void(*_handler)(io_sockets*,void*);
typedef int(*_to_handler)(void);

enum _handlers_flags
{
    accept_handler_init =   0x1<<0,
    in_handler_init =       0x1<<1,
    out_handler_init =      0x1<<2,
    timeout_handler_init =  0x1<<3,
};

struct _handlers_t
{
    _handler* accept_handlers;
    _handler* in_handlers;
    _handler* out_handlers;
    _to_handler* timeout_handlers;
    size_t accept_n, in_n, out_n, to_n;
    uint8_t flags;
};

struct server_work_in
{
    io_sockets io;
    struct _handlers_t handlers;
    struct timeval tv;                      //таймаут
    void* _reserved;     // TODO: тут будет структура данных, общая для хэндлеров
};




int server_work(struct server_work_in* in);
void init_tcp_server(uint16_t port, struct server_work_in* swin);
void init_udp_server(uint16_t port, struct server_work_in* swin);
void deinit_server(struct server_work_in* srv);
void change_tcp_port(struct server_work_in* swin, uint16_t port);
void change_udp_port(struct server_work_in* swin, uint16_t port);

void server_add_in_handler(struct server_work_in* srv_h,_handler);
void server_add_out_handler(struct server_work_in* srv_h,_handler);
void server_add_accept_handler(struct server_work_in* srv_h,_handler);
void server_add_timeout_handler(struct server_work_in* srv_h, int(*timeout_handler)(void));


void send_from_udp_to_addr(u_socket_t s,uint8_t b1,uint8_t b2,uint8_t b3,uint8_t b4, uint16_t port, char* msg, size_t len);
void send_udp_to_addr(uint8_t b1,uint8_t b2,uint8_t b3,uint8_t b4, uint16_t port, char* msg, size_t len);
 int howto_test (void) ;

void set_round_timeout(struct server_work_in* srv, int second, int us);

int howto_test (void);
#endif // !U_SERVER
