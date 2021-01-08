#include <u_server.h>



//!FIXME
int check_flag(){
    return 1;
}
static void set_default_tcp_handlers(struct server_work_in* srv);
static void set_default_udp_handlers(struct server_work_in* srv);

//TCP
void init_tcp_server(uint16_t port, struct server_work_in* swin)
{
    /********************************************
    1. создание серверного сокета*/
    swin->io.in=socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // swin->io.in=socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(swin->io.in<=0){
        printf("1)   ERROR! Socket is'n created\n");
    }else{ 
        printf("1)   Socket created is succes.\n");
    }
    // int yes=1;
    // setsockopt(io->in,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
    
    /********************************************
    2. Привязка к локальным именам*/
    u_sockaddr_in_t addr;//создаем прототип(addr), структуры sockaddr_in
    addr.sin_family=AF_INET;//при создании Интернет-приложений


    addr.sin_port=htons(port);//порт
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
 
    int i_error=0;
    i_error = bind( swin->io.in, (u_sockaddr_t*)&addr,  sizeof(addr)  );

    if(i_error < 0){ 
        
        printf("2)   ERROR binding!\n");
    }else{ 
        printf("2)   Binding success!\n");
    }
    
    /********************************************
    // 3. Инициализация процесса «прослушивания» порта*/    
    listen( swin->io.in,3 );
    {//говорим в stdout какой у нас порт
        int tmp=sizeof(u_sockaddr_t);
        u_getsockname(swin->io.in, (u_sockaddr_t*)&addr, &tmp);
        printf("your port = %d\n",ntohs(addr.sin_port));
    }
    swin->io.out=0;
    set_round_timeout(swin, 1, 0);
    set_default_tcp_handlers(swin);
    
}

//--------------------------------------------------------


void init_udp_server(uint16_t port, struct server_work_in* swin)
{
    memset(&swin->handlers,0,sizeof(swin->handlers));
    /********************************************
    1. создание серверного сокета*/
    // swin->io.in=socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
    swin->io.in=socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(swin->io.in<=0){
        printf("1)   ERROR! Socket is'n created\n");
    }else{ 
        printf("1)   Socket created is succes.\n");
    }
    // int yes=1;
    // setsockopt(io->in,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
    
    /********************************************
    2. Привязка к локальным именам*/
    u_sockaddr_in_t addr;//создаем прототип(addr), структуры sockaddr_in
    addr.sin_family=AF_INET;//при создании Интернет-приложений


    addr.sin_port=htons(port);//порт
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
 
    int i_error=0;
    i_error = bind( swin->io.in, (u_sockaddr_t*)&addr,  sizeof(addr)  );

    if(i_error < 0){ 
        
        printf("2)   ERROR binding!\n");
    }else{ 
        printf("2)   Binding success!\n");
    }
    
    /********************************************/
    swin->io.out=0;
    set_round_timeout(swin, 1, 0);
    set_default_udp_handlers(swin);
}
//--------------------------------------------------------

// ini обработчик ввода
static void _add_in_handler(struct server_work_in* srv_h,void(*in_handler)(io_sockets* io))
{
    srv_h->handlers.in_n=1;
    srv_h->handlers.in_handlers=malloc(srv_h->handlers.in_n*sizeof(_handler));
    srv_h->handlers.in_handlers[0]=in_handler;
    srv_h->handlers.flags &= ~(uint8_t) in_handler_init;
}
//--------------------------------------------------------

// Прицепить обработчик ввода
void server_add_in_handler(struct server_work_in* srv_h,void(*in_handler)(io_sockets* io))
{
    if(!(srv_h->handlers.flags&in_handler_init))
    {
        srv_h->handlers.in_n=0;
    }
        srv_h->handlers.in_handlers=realloc(srv_h->handlers.in_handlers,
                                            (srv_h->handlers.in_n+1)*sizeof(_handler));
        srv_h->handlers.in_handlers[srv_h->handlers.in_n]=in_handler;
        srv_h->handlers.in_n++;
    // }else{
    //     _add_in_handler(srv_h, in_handler);
    // }
}
//--------------------------------------------------------

// init обработчик вывода
static void _add_out_handler(struct server_work_in* srv_h,void(*out_handler)(io_sockets* io))
{
    srv_h->handlers.out_n=1;
    srv_h->handlers.out_handlers=malloc(srv_h->handlers.out_n*sizeof(_handler));
    srv_h->handlers.out_handlers[0]=out_handler;
    srv_h->handlers.flags &= ~(uint8_t) out_handler_init;
}
//--------------------------------------------------------

// Прицепить обработчик вывода
void server_add_out_handler(struct server_work_in* srv_h,void(*out_handler)(io_sockets* io))
{
    if(!(srv_h->handlers.flags&out_handler_init))
    {
        srv_h->handlers.out_n=0;
    }
        
        srv_h->handlers.out_handlers=realloc(srv_h->handlers.out_handlers,
                                            (srv_h->handlers.out_n+1)*sizeof(_handler));
        srv_h->handlers.out_handlers[srv_h->handlers.out_n]=out_handler;
        srv_h->handlers.out_n++;
    // }else{
    //     _add_out_handler(srv_h, out_handler);
    // }
}
//--------------------------------------------------------

// init обработчик по приёму
static void _add_accept_handler(struct server_work_in* srv_h,void(*accept_handler)(io_sockets* io))
{
    srv_h->handlers.accept_n=1;
    srv_h->handlers.accept_handlers=malloc(srv_h->handlers.accept_n*sizeof(_handler));
    srv_h->handlers.accept_handlers[0]=accept_handler;
    srv_h->handlers.flags &= ~(uint8_t) accept_handler_init;
}
//--------------------------------------------------------
// Прицепить обработчик по приёму
void server_add_accept_handler(struct server_work_in* srv_h,void(*accept_handler)(io_sockets*))
{
    if(!(srv_h->handlers.flags&accept_handler_init))
    {
        srv_h->handlers.accept_n=0;
    }
        srv_h->handlers.accept_handlers=realloc(srv_h->handlers.accept_handlers,
                                            (srv_h->handlers.accept_n+1)*sizeof(_handler));
        srv_h->handlers.accept_handlers[srv_h->handlers.accept_n]=accept_handler;
    srv_h->handlers.accept_n++;
}
//--------------------------------------------------------
// init обработчик таймаута
static void _add_timeout_handler(struct server_work_in* srv_h, int(*timeout_handler)(void))
{
    srv_h->handlers.to_n=1;
    srv_h->handlers.timeout_handlers=malloc(sizeof(_to_handler));
    srv_h->handlers.timeout_handlers[0]=timeout_handler;
    srv_h->handlers.flags &= ~(uint8_t)timeout_handler_init;
}
//--------------------------------------------------------
// Прицепить обработчик таймаута
void server_add_timeout_handler(struct server_work_in* srv_h, int(*timeout_handler)(void))
{
    if(!(srv_h->handlers.flags&timeout_handler_init))
    {
        srv_h->handlers.to_n=0;
    }
        srv_h->handlers.timeout_handlers=realloc(srv_h->handlers.timeout_handlers,
                                            (srv_h->handlers.to_n+1)*sizeof(_handler));
        srv_h->handlers.timeout_handlers[srv_h->handlers.to_n]=timeout_handler;
        srv_h->handlers.to_n++;
    // }else{
    //     _add_accept_handler(srv_h, timeout_handler);
    // }
}

//--------------------------------------------------------
void server_free_handlers(struct server_work_in* srv_h)
{
    free(srv_h->handlers.timeout_handlers);
    free(srv_h->handlers.accept_handlers);
    free(srv_h->handlers.out_handlers);
    free(srv_h->handlers.in_handlers);
}
//--------------------------------------------------------
void deinit_server(struct server_work_in* srv)
{
    if(srv->io.in) u_close_sock(srv->io.in);
    if(srv->io.out) u_close_sock(srv->io.out);
    srv->io.in=0;
    srv->io.out=0;
    server_free_handlers(srv);
}
//--------------------------------------------------------
void change_tcp_port(struct server_work_in* swin, uint16_t port)
{
    deinit_server(swin);
    init_tcp_server(port,swin);
}
//--------------------------------------------------------
void change_udp_port(struct server_work_in* swin, uint16_t port)
{
    deinit_server(swin);
    init_udp_server(port,swin);
}
//--------------------------------------------------------
void set_round_timeout(struct server_work_in* srv, int second, int us)
{
    srv->tv.tv_sec = second;
    srv->tv.tv_usec = us;
}
//--------------------------------------------------------
int server_work(struct server_work_in* args)
{

    // int wath_duck=15;

    fd_set fd_in,fd_out;
    struct timeval tv;

    u_socket_t largest_sock=args->io.in;
    while(1){  
        FD_ZERO( &fd_in );
        FD_ZERO( &fd_out );
        FD_SET( args->io.in, &fd_in );//добавляем в сет
        if(args->io.out){
            FD_SET( args->io.out, &fd_in );//добавляем в сет
            FD_SET( args->io.out, &fd_out );//добавляем в сет
        }
        tv.tv_sec = args->tv.tv_sec;
        tv.tv_usec = args->tv.tv_usec;
  
        (args->io.in < args->io.out)?(largest_sock=args->io.out):(largest_sock=args->io.in);
        
        int ret = select( largest_sock + 1, &fd_in, &fd_out, NULL, &tv );
        // проверяем успешность вызова
        if ( ret == -1 )
        {
            fprintf(stderr,"SELECT_ERROR!!\n");
            return -1;  

        }else if( ret == 0 )
        {
            // таймаут, событий не произошло
            for (size_t i = 0; i < args->handlers.to_n; i++)
            {
                if(args->handlers.timeout_handlers[i]()) return 0;
            }
            
            

        }else{
            // обнаружили событие

            if (FD_ISSET(args->io.in, &fd_in) && args->io.out == 0)
            {
                // TCP - новый клиент
                printf("new client\n");
                for (size_t i = 0; i < args->handlers.accept_n; i++)
                {
                    args->handlers.accept_handlers[i](&args->io);
                }
            }
            if (FD_ISSET(args->io.out, &fd_in) && args->io.out)
            {
                // Событие входа
                printf("read data\n");
                for (size_t i = 0; i < args->handlers.in_n; i++)
                {
                    args->handlers.in_handlers[i](&args->io);
                }
            }
            if (FD_ISSET(args->io.out, &fd_out) && args->io.out)
            {
                // Событие выхода
                printf("end writing\n");
                for (size_t i = 0; i < args->handlers.out_n; i++)
                {
                    args->handlers.out_handlers[i](&args->io);
                }
                // u_close_sock(args->io.out);
                // args->io.out=0;
            }
        }   


    }
}
//--------------------------------------------------------
/*На входяещее сообщение всегда "Hello client" отвечаем*/
static void default_in_handler(io_sockets* io)
{
        char data_from_client[1024]={0};
        int n = recv( io->out, data_from_client, 1024, 0 );

        printf("Received data:\n%.*s\n *%d*\n",n, data_from_client,n);

        //отправка
        if(n){
            char response_to_client[]="Hello client\r\n";
            send( io->out, response_to_client, 14, 0);
        }
}
//--------------------------------------------------------
/*когда отправлено сообщение закрываем сокет*/
static void default_out_handler(io_sockets* io){

    u_close_sock(io->out);
    io->out=0;
}
//--------------------------------------------------------
static void default_udp_handler(io_sockets* io){

    char c_data_ot_k[1024]={0};
    int n = recv( io->in, c_data_ot_k, 1024, 0 );
    printf("Received data:\n%.*s\n *%d*\n",n, c_data_ot_k,n);
}
//--------------------------------------------------------
static void default_accept_handler(io_sockets* io)
{
    io->out=u_accept( io->in, 0, 0);
    if(io->out==-1){
        printf(" err accept\n");
        io->out=0;
    }
    char c_data=0;
    int n = recv( io->out, &c_data, 0, 0 );
    printf("Recv *%d*\n",n);
}
//--------------------------------------------------------
static int default_timeout_handler(void)
{
    static int wath_duck=300;
    wath_duck--;
    printf("%d s\n",wath_duck);
    fflush(stdout);
    return (wath_duck)?(0):(1);
}
//--------------------------------------------------------
void send_udp_to_addr(uint8_t b1,uint8_t b2,uint8_t b3,uint8_t b4, uint16_t port, char* msg, size_t len)
{
    //send_tcp_to_addr(192,168,8,1, msg,1024);
    
    /* 1. создание серверного сокета*/
    u_socket_t s=socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if( s <= 0 ){
        printf("SEND: ERROR! Socket is'n created\n");
    }else{ 
        printf("SEND: Socket created is succes.\n");
    }

    u_sockaddr_in_t addr;
    addr.sin_port=htons(port);//порт
    uint32_t a_a=0;
    a_a |= ((uint32_t)b1)<<0;
    a_a |= ((uint32_t)b2)<<8;
    a_a |= ((uint32_t)b3)<<16;
    a_a |= ((uint32_t)b4)<<24;//адрес

    addr.sin_addr.s_addr=a_a;//адрес
    addr.sin_family=AF_INET;

    sendto(s, msg, len, 0, (u_sockaddr_t*)&addr,  sizeof(addr));
    u_close_sock(s);

}
//--------------------------------------------------------
void send_from_udp_to_addr(u_socket_t s,uint8_t b1,uint8_t b2,uint8_t b3,uint8_t b4, uint16_t port, char* msg, size_t len)
{

    u_sockaddr_in_t addr;
    addr.sin_port=htons(port);//порт
    uint32_t a_a=0;
    a_a |= ((uint32_t)b1)<<0;
    a_a |= ((uint32_t)b2)<<8;
    a_a |= ((uint32_t)b3)<<16;
    a_a |= ((uint32_t)b4)<<24;//адрес

    addr.sin_addr.s_addr=a_a;//адрес
    addr.sin_family=AF_INET;

    sendto(s, msg, len, 0, (u_sockaddr_t*)&addr,  sizeof(addr));

}
//--------------------------------------------------------
void set_default_tcp_handlers(struct server_work_in* srv)
{
    _add_accept_handler(srv, default_accept_handler);
    _add_in_handler(srv, default_in_handler);
    _add_out_handler(srv, default_out_handler);
    _add_timeout_handler(srv, default_timeout_handler);
}
//--------------------------------------------------------
void set_default_udp_handlers(struct server_work_in* srv)
{
    _add_accept_handler(srv, default_udp_handler);
    _add_in_handler(srv, default_udp_handler);
    _add_out_handler(srv, default_out_handler);
    _add_timeout_handler(srv, default_timeout_handler);
}

//.....................................................
//..======..||==../==\...======........................
//....||....||==..\==\.....||..........................
//....||....||==../==/.....||..........................
//.....................................................


int howto_test (void)
{

    
    /********************************************
    0. настройка библиотеки Ws2_32.dll*/
    if (start_NW()) return -1;


    struct server_work_in in;
    init_tcp_server(3000, &in);

    server_work(&in);
    deinit_server(&in);

    stop_NW();
    return 0;
}

// int main() {
//     // return 0;
//     return howto_test ();
// }
