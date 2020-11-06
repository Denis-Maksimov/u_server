#ifndef U_SERVER
#define U_SERVER
#include <u_network.h>

struct server_work_in
{
    io_sockets io;
    void(*accept_handler)(io_sockets* io);  //обработчик событий ввода
    void(*in_handler)(io_sockets* io);      //обработчик событий ввода
    void(*out_handler)(io_sockets* io);     //обработчик событий вывода
    int(*timeout_handler)(void);           //обработчик таймаута раунда (if not 0 завершает сервер)

    struct timeval tv;                      //таймаут
};

int server_work(struct server_work_in* in);
void init_server(uint16_t port, struct server_work_in* swin);
void init_udp_server(uint16_t port, struct server_work_in* swin);
void deinit_server(struct server_work_in* srv);
void change_port(struct server_work_in* swin, uint16_t port);
void change_udp_port(struct server_work_in* swin, uint16_t port);

void server_add_in_handler(struct server_work_in* srv_h,void(*in_handler)(io_sockets* io));
void server_add_out_handler(struct server_work_in* srv_h,void(*out_handler)(io_sockets* io));
void server_add_accept_handler(struct server_work_in* srv_h,void(*accept_handler)(io_sockets* io));
void server_add_timeout_handler(struct server_work_in* srv_h, int(*timeout_handler)(void));
void set_default_handlers(struct server_work_in* srv);
void set_default_udp_handlers(struct server_work_in* srv);

void send_udp_to_addr(uint8_t b1,uint8_t b2,uint8_t b3,uint8_t b4, uint16_t port, char* msg, size_t len);
 int howto_test (void) ;

void set_round_timeout(struct server_work_in* srv, int second, int us);

int howto_test (void);
#endif // !U_SERVER
