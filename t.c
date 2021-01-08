#include "proto.h"
#include "u_server.h"
#include <stdio.h>

struct server_work_in in;

void client_handler(io_sockets* io){

	union{
		char raw_data[32];
		response_msg resp;
	}msg;
    
    int n = recv( io->in, msg.raw_data, 32, 0 );
	if(n != sizeof(response_msg)) printf("size ERROR =(\n");
	else{

	printf("command: *%d*\n", msg.resp.command);
	printf("code err: *%d*\n", msg.resp.errcode);
    printf("Received data: *%d*\n", msg.resp.value);

	}

}

void server_handler(io_sockets* io){
	struct sockaddr from; 
	u_socklen_t fromlen=sizeof(from);

	union{
		char raw_data[32];
		proto_msg request;
	}msg;

    response_msg resp;

	int n = recvfrom( io->in, msg.raw_data, 32, 0, &from, &fromlen);

	if(n != sizeof(proto_msg)) {
		if(!n) return;
		printf("size ERROR =(\n");
		resp.command=0xffff;
		resp.errcode=INVALID_MSG_SIZE;

	}
	else{
        printf("ok!\n");
		response(&(msg.request), &resp);
        uint8_t*reg = get_reg();
        printf("reg[0]=%d\n",reg[0]);
	}
	sendto(io->in,(char*)(&resp),sizeof(resp),0,&from,fromlen);

}


int timeout_handler(void)
{
    static int wath_duck=30;
    wath_duck--;
    
    proto_msg msg;
    write_byte_msg(&msg, 0, (uint8_t)wath_duck, 0xff);
    send_from_udp_to_addr(in.io.in,127,0,0,1, 3000, (char*)(&msg), sizeof(msg));
    printf("%d s\n",wath_duck);
    fflush(stdout);
    return (wath_duck)?(0):(1);
}

int main(int argc, char const *argv[])
{
	
	if (start_NW()) return -1;

    // struct server_work_in in;

    
    // set_default_udp_handlers(&in);
	// server_add_accept_handler(&in, server_handler);
    // init_udp_server(3000, &in);
    // server_work(&in);
    // deinit_server(&in);
//===============================================
    // struct server_work_in in;

    init_udp_server(3001, &in);
	
	server_add_accept_handler(&in, client_handler);
    server_add_timeout_handler(&in, timeout_handler);
    
    server_work(&in);
    deinit_server(&in);


    stop_NW();


}

