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
    // write_byte_msg(&msg, 0, (uint8_t)wath_duck, 0xff);
    // send_from_udp_to_addr(in.io.in,127,0,0,1, 3000, (char*)(&msg), sizeof(msg));
    printf("%d s\n",wath_duck);
    fflush(stdout);
    return (wath_duck)?(0):(1);
}



void ac_h(io_sockets* io, void* data){
	// HTTP/1.1 %i OK\r\n\r\n\0

	 io->out=u_accept( io->in, 0, 0);
    if(io->out==-1){
        printf(" err accept\n");
        io->out=0;
    }
    char c_data=0;
    int n = recv( io->out, &c_data, 0, MSG_PEEK );
	
    printf("Recv *%d*\n",n);
	((size_t*)data)[0]=malloc(sizeof(size_t));
	((size_t*)data)[0]=0x12345678;

}
void r_h(io_sockets* io, void* data){

	
	
	int bs=1024;
	char* data_from_client=malloc(bs);
	int n = recv( io->out, data_from_client, bs, MSG_PEEK);
	while (n==bs)
	{
		bs+=1024;
		data_from_client=realloc(data_from_client,bs);
		n = recv( io->out, data_from_client, bs, MSG_PEEK);

	}
	data_from_client=realloc(data_from_client,n);
	n = recv( io->out, data_from_client, n, 0);

	// read()

	printf("Received data:\n%.*s\n *%d*\n",n, data_from_client,n);

	//отправка
	if(n){
		char response_to_client[]="HTTP/1.0 200 OK"
		"\r\n\r\n";
		if(((size_t*)data)[0]==0x12345678){

			FILE* f=fopen("./1.html","r");
			fseek(f,0,SEEK_END);
			long size=ftell(f);
			fseek(f,0,SEEK_SET);
			char* buf=malloc(size);
			fread(buf,size,1,f);
			fclose(f);
			
			send( io->out, response_to_client, 19, 0);
			send( io->out, buf, size, 0);
			((size_t*)data)[0]=0x87654321;
			puts("Отправлено");
			free(buf);
		}
		
	}

}

int main(int argc, char const *argv[])
{
	puts(argv[0]);
	if (start_NW()) return -1;

    // struct server_work_in in;


    init_tcp_server(3001, &in);
	
	server_add_accept_handler(&in, ac_h);
	server_add_in_handler(&in, r_h);
    server_add_timeout_handler(&in, timeout_handler);
    
    server_work(&in);
    deinit_server(&in);


    stop_NW();


}

