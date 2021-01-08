#include "proto.h"
#include "u_server.h"
#include <stdio.h>

proto_msg msg;
response_msg resp;


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
	socklen_t fromlen=sizeof(from);

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
		response(&msg, &resp);
	}
	sendto(io->in,&resp,sizeof(resp),0,&from,fromlen);

}



int main(){
	
	
	if (start_NW()) return -1;

    struct server_work_in in;

    init_tcp_server(3000, &in);
    set_default_handlers(&in);
	server_add_accept_handler(&in, server_handler);
    server_work(&in);
    deinit_server(&in);

    stop_NW();


}

