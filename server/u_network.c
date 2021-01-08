#include <u_network.h>

//-------------------WINDOWS-----------------------
#ifdef OS_WINDOWS


/**********************************
*  настройка библиотеки Ws2_32.dll 
***********************************/
int start_NW()
{
    int i_error=0;//будет хранить код ошибки
    WSADATA wsaData;//определяем переменную
    i_error = WSAStartup(MAKEWORD(2,2),&wsaData);//настраиваем
    if (i_error)//проверка на успешность настройки
    {
        printf("0)   ERROR! %d; _error: %d\n", WSAGetLastError(), i_error);

    }else{ 
        printf("0   library Ws2_32.dll - connected.\n");
    }
    return i_error;
}

void stop_NW()
{
    WSACleanup();// выгружаем WinSock
}

int u_close_sock(u_socket_t sock)
{
     return closesocket (sock);//закрытие
}


u_socket_t u_accept(u_socket_t socket,u_sockaddr_t* client_addr, int* size_client_addr){
    return accept(socket, client_addr, size_client_addr);
}

int u_getsockname(u_socket_t socket, u_sockaddr_t* addr, int* namelen){
    return getsockname(socket, addr, namelen);
}




#else
//-------------------LINUX-----------------------


int start_NW(){return 0;} //затычка
void stop_NW(){ return; } //затычка

int u_close_sock(u_socket_t sock)
{
    return close(sock);//закрытие
}

u_socket_t u_accept(u_socket_t socket,u_sockaddr_t* client_addr, int* size_client_addr)
{
    return accept(socket, client_addr, (unsigned int*)size_client_addr);
}
int u_getsockname(u_socket_t socket, u_sockaddr_t* addr, int* namelen)
{
    return getsockname(socket, addr, (unsigned int*)namelen);
}

#endif


