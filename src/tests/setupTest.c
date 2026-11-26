#include "../LAN/setup.h"
#include <WinSock2.h>
#include <stdio.h>
#define PORT_NUMBER 14000

void socketSetupTest(void)
{
    SOCKET comm_sock;
    struct sockaddr_in user_info, other_user_info;
    setup_link_socket(PORT_NUMBER, &user_info, &other_user_info, &comm_sock);
}

void broadcastTest(void)
{
    int choice;
    long broadcast_ip = 0;
    printf("send broadcast (0) or receive broadcast (1) ?\n");
    scanf("%d", &choice);
    if (choice)
    {
        receiveBroadcast(PORT_NUMBER, &broadcast_ip);
        if (broadcast_ip != 0)
            printf("received ip address: %d\n", broadcast_ip);
    }
    else 
        broadcastIP(PORT_NUMBER);
}


int main(void)
{
    WSADATA wsadata;
    int error = WSAStartup(MAKEWORD(2,2),&wsadata);
    if (error)
    {
        printf("WSA startup failed. error no: %d\n", error);
        return 1;
    }

    broadcastTest();
}
