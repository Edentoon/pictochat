#include "../LAN/setup.h"
#include <process.h>
#define PORT_NUMBER 14000


void handleNewUser(long *ip_address)
{
    struct broadcast_param b;
    b.port = PORT_NUMBER;
    b.recieved_ip = ip_address;
    _beginthread(initiateNewUser, 0, (void *)&b);
}

long checkIfFirstUser()
{
    broadcast_IP(PORT_NUMBER);
    SOCKET sock;
    // creates udp socket for recieving response
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // error checking socket creation
    if (sock == INVALID_SOCKET)
    {
        printf("could not create socket.\n");
        return -1;
    }

    struct sockaddr_in local_info;
    local_info.sin_family = AF_INET;
    local_info.sin_addr.s_addr = 0;
    local_info.sin_port = htons(PORT_NUMBER);

    u_long enable_non_blocking = 1;
    ioctlsocket(sock, FIONBIO, &enable_non_blocking);

    int bind_error = bind(sock, (struct sockaddr *)&local_info, sizeof(local_info));
    if (bind_error != 0)
    {
        printf("could not bind socket. error no: %d\n", WSAGetLastError());
        return -1;
    }

    struct sockaddr_in peer_ip_address;
    int peer_ip_len = sizeof(struct sockaddr);
    char recieved_message[60];
    int recv_error = recvfrom(sock, recieved_message, 60, 0, (struct sockaddr *)&peer_ip_address, &peer_ip_len);
    if (recv_error == SOCKET_ERROR)
    {
        recv_error = WSAGetLastError();
        if (recv_error == WSAEWOULDBLOCK)
            return 1;
        else 
        {
            printf("recvfrom error: %d\n", recv_error);
            return -1;
        }
    }
    stop_notify = 1;
    return peer_ip_address.sin_addr.s_addr;
}
