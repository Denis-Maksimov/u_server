#ifndef U_NETWORK_H
#define U_NETWORK_H


#if defined (_WIN32)|| defined(_WIN64)|| defined(__WIN32__) || defined(__WINDOWS__)
#define OS_WINDOWS
#endif

#if defined(unix) || defined(__unix__) || defined(__unix)
#define PLATFORM_UNIX
#endif

#if !defined (PLATFORM_UNIX) && !defined (OS_WINDOWS)
#error "not support APPLE"
#endif // !1



//-------------------WINDOWS-----------------------
#ifdef OS_WINDOWS
#include <winsock2.h>


typedef  SOCKET u_socket_t;

#define U_INVALID_SOCKET    INVALID_SOCKET


#endif //OS_WINDOWS
//-------------------LINUX-----------------------
#ifdef PLATFORM_UNIX
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <sys/select.h>
#include <sys/types.h>
#include <netdb.h>
// #include <fcntl.h>


#define U_INVALID_SOCKET    0

typedef int u_socket_t;


#endif // PLATFORM_UNIX
//-------------------ALL-----------------------
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

typedef struct 
{
    u_socket_t in;  //for reading
    u_socket_t out; //for writing
}io_sockets;



int start_NW();
void stop_NW();
int u_close_sock(u_socket_t sock);

typedef  struct sockaddr_in u_sockaddr_in_t;
typedef  struct sockaddr u_sockaddr_t;
u_socket_t u_accept(u_socket_t socket,u_sockaddr_t* client_addr, int* size_client_addr);
int u_getsockname(u_socket_t socket, u_sockaddr_t* addr, int* namelen);
#endif // !U_NETWORK_H
