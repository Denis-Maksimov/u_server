#include "u_server.h"



//!FIXME
int check_flag(){
    return 1;
}



void init_server(uint16_t port, io_sockets* io)
{
    /********************************************
    1. создание серверного сокета*/

    
    io->in=socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(io->in<=0){
        printf("1)   ERROR! Socket is'n created\n");
    }else{ 
        printf("1)   Socket created is succes.\n");
    }
    // int yes=1;
    // setsockopt(io->in,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
    
    /********************************************
    2. Привязка к локальным именам*/

    u_sockaddr_in_t addr;//создаем протатип(addr), структуры sockaddr_in
    addr.sin_family=AF_INET;//при создании Интернет-приложений


    addr.sin_port=htons(port);//порт
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
 
    int i_error=0;
    i_error = bind( io->in, (u_sockaddr_t*)&addr,  sizeof(addr)  );

    if(i_error < 0){ 
        
        printf("2)   ERROR binding!\n");
    }else{ 
        printf("2)   Binding success!\n");
    }
    
    /********************************************
    3. Инициализация процесса «прослушивания» порта*/
    
    
    listen( io->in,3 );
    {//говорим в stdout какой у нас порт
        int tmp=sizeof(u_sockaddr_t);
        u_getsockname(io->in, (u_sockaddr_t*)&addr, &tmp);
        printf("your port = %d\n",ntohs(addr.sin_port));
    }
    io->out=0;
    
}

void deinit_server(io_sockets* io)
{
    
    u_close_sock(io->in);
    if(io->out) u_close_sock(io->out);
}



int server_work(struct server_work_in* args)
{

    int wath_duck=15;

    struct timeval tv;


    fd_set fd_in,fd_out;
    

    u_socket_t largest_sock=args->io->in;
    while(1){  
        FD_ZERO( &fd_in );
        FD_ZERO( &fd_out );
        FD_SET( args->io->in, &fd_in );//добавляем в сет
        if(args->io->out){
            FD_SET( args->io->out, &fd_in );//добавляем в сет
            FD_SET( args->io->out, &fd_out );//добавляем в сет
        }
        tv.tv_sec = 1;
        tv.tv_usec = 0;
  
        (args->io->in < args->io->out)?(largest_sock=args->io->out):(largest_sock=args->io->in);
        
        int ret = select( largest_sock + 1, &fd_in, &fd_out, NULL, &tv );
        // проверяем успешность вызова
        if ( ret == -1 )
        {

            return -1;  

        }else if( ret == 0 )
        {
            //!TODO
            // таймаут, событий не произошло
            // if(check_flag()) return 0;
            if(!(wath_duck--)) return 0;
            printf("%d s\n",wath_duck);
            fflush(stdout);


        }else{
            // обнаружили событие
            if ( FD_ISSET( args->io->in, &fd_in ) && args->io->out==0 ){
   
                args->io->out=u_accept( args->io->in, 0, 0);
                if(args->io->out==-1){printf(" err accept\n");args->io->out=0;}
                printf("new client\n");
            }
            if ( FD_ISSET( args->io->out, &fd_in )&& args->io->out){
  
                    printf("read data\n");

                    args->handler(args->io);
                    

            }
            if ( FD_ISSET( args->io->out, &fd_out )&& args->io->out){
  
                    printf("end writing\n");
                    u_close_sock(args->io->out);
                    args->io->out=0;
            }
                    
            


        }   


    }
}

//.....................................................
//..======..||==../==\...======........................
//....||....||==..\==\.....||..........................
//....||....||==../==/.....||..........................
//.....................................................
void doing(io_sockets* sock);

void doing(io_sockets* io)
{
        char c_data_ot_k[1024]={0};
        int n = recv( io->out, c_data_ot_k, 1024, 0 );

        printf("Received data:\n%.*s\n *%d*\n",n, c_data_ot_k,n);

        //отправка
        if(n){
            char c_data_s_go_k[]="Hello client\r\n";
            send( io->out, c_data_s_go_k, 14, 0);
        }
}

int howto_test (void)
{

    
    /********************************************
    0. настройка библиотеки Ws2_32.dll*/
    if (start_NW()) return -1;

    io_sockets io;
    init_server(3000, &io);

    struct server_work_in in={
        .io=&io,
        .handler=doing  //callback
    };

    server_work(&in);
    stop_NW();
    deinit_server(&io);
    return 0;
}

int main() {
    // return 0;
    return howto_test ();
}