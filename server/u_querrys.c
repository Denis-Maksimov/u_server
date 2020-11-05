#include "u_server.h"


void handle_recv(io_sockets* io)
{
        char c_data_ot_k[1024] ={0};
        int n = recv( io->out, c_data_ot_k, 1024, 0 );

        printf("Received data:\n%.*s\n *%d*\n",n, c_data_ot_k,n);

        //отправка
        if(n){
            char c_data_s_go_k[]="Hello client\r\n";
            send( io->out, c_data_s_go_k, 14, 0);
        }
}

void send_file(u_socket_t dest, const char* filename)
{

    FILE* fd = fopen(filename,"r");
    

}