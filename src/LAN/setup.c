#include <WinSock2.h>
#include <stdio.h>
#include "setup.h"

/*
int npcap_init()
{
    // error description buffer
    char errbuff[PCAP_ERRBUF_SIZE];
    // contains info on all adapters
    pcap_if_t* devices;
    int i = 0;

    int error = pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL,
                                    &devices, errbuff);
    if (error == -1)
    {
        printf("Error in function pcap_findalldevs_ex: %s\n", errbuff);
        return 1;
    }
    // print all devices in linked list
    for (pcap_if_t* device = devices; device != NULL; device = device->next, i++)
    {
        printf("device %d: %s (%s)\n", i+1, device->name, device->description);
    }
}
*/
// sets up socket communication for a link
int setup_link_socket(const int PORT_NUMBER, struct sockaddr_in *calling_user, 
                      struct sockaddr_in *connected_user, SOCKET *communication_socket)
{
    // socket initialization
    SOCKET sock;
    // creates socket for user communication
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // error checking socket creation
    if (sock == INVALID_SOCKET)
    {
        printf("could not create socket.\n");
        return 1;
    }

    // address used to bind all local addresses
    long ip_address = 0;
    
    // converts port number to network byte order (big endian)
    u_short port_number = htons(PORT_NUMBER);

    // fill in address info
    calling_user->sin_family = AF_INET;
    calling_user->sin_port = port_number;
    calling_user->sin_addr.s_addr = ip_address;

    // listen for new connections
    int bind_error = bind(sock, (const struct sockaddr*)calling_user, sizeof(*calling_user));
    if (bind_error != 0)
    {
        printf("could not bind socket. error no: %d\n", WSAGetLastError());
        return 1;
    }

    int listen_error = listen(sock, SOMAXCONN);
    if (listen_error != 0)
    {
        printf("could not listen for connections. error no: %d\n", listen_error);
    }
    
    int user_size = (int)sizeof(*connected_user);
    *communication_socket = accept(sock, (struct sockaddr*)connected_user, &user_size);
    if (*communication_socket == INVALID_SOCKET)
    {
        printf("communication socket could not be created. error no: %d\n", WSAGetLastError());
        return 1;
    }
    return 0;

}

void broadcastIP(const int PORT_NUMBER)
{
    SOCKET sock;
    // creates udp socket for broadcasting
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // error checking socket creation
    if (sock == INVALID_SOCKET)
    {
        printf("could not create socket.\n");
        return;
    }
    
    // set socket options to allow broadcast and disable loopback
    int broadcast_opt = 1;
    int opt_error = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *)&broadcast_opt, sizeof(broadcast_opt));
    if (opt_error < 0)
    {
        printf("setsockopt failed, error: %d\n", WSAGetLastError());
        return;
    }

    // broadcast addr struct setup
    struct sockaddr_in broadcast_info;
    broadcast_info.sin_family = AF_INET;
    broadcast_info.sin_addr.s_addr = INADDR_BROADCAST;
    broadcast_info.sin_port = htons(PORT_NUMBER);

    const char* message = "IP ADDRESS BROADCAST";
    int message_len = strlen(message);

    int sent_bytes = sendto(sock, message, message_len, 0, (struct sockaddr *)&broadcast_info, sizeof(broadcast_info));
    if (sent_bytes != message_len)
    {
        printf("sendto error\n");
        return;
    }
}

void notify_IP(long ip_address, const int PORT_NUMBER)
{
    SOCKET sock;
    // creates udp socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // error checking socket creation
    if (sock == INVALID_SOCKET)
    {
        printf("could not create socket.\n");
        return;
    }

    struct sockaddr_in notify_info;
    notify_info.sin_family = AF_INET;
    notify_info.sin_addr.s_addr = ip_address;
    notify_info.sin_port = htons(PORT_NUMBER);
    
    const char* message = "IP ADDRESS NOTIFICATION";
    int message_len = strlen(message);

    while (!stop_notify)
    {
        sendto(sock, message, message_len, 0, (struct sockaddr *)&notify_info, sizeof(notify_info));
    }
    
}

void recieveBroadcast(const int PORT_NUMBER, long* recieved_ip)
{
    SOCKET sock;
    // creates udp socket for broadcasting
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // error checking socket creation
    if (sock == INVALID_SOCKET)
    {
        printf("could not create socket.\n");
        return;
    }
    
    struct sockaddr_in local_info;
    local_info.sin_family = AF_INET;
    local_info.sin_addr.s_addr = 0;
    local_info.sin_port = htons(PORT_NUMBER);

    // set socket options to allow broadcast and disable loopback
    int broadcast_opt = 1;
    int opt_error = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *)&broadcast_opt, sizeof(broadcast_opt));
    if (opt_error < 0)
    {
        printf("setsockopt failed, error: %d\n", WSAGetLastError());
        return;
    }

    int bind_error = bind(sock, (struct sockaddr *)&local_info, sizeof(local_info));
    if (bind_error != 0)
    {
        printf("could not bind socket. error no: %d\n", WSAGetLastError());
        return;
    }

    struct sockaddr_in peer_ip_address;
    int peer_ip_len = sizeof(struct sockaddr);
    char recieved_message[60];
    int recv_error = recvfrom(sock, recieved_message, 60, 0, (struct sockaddr *)&peer_ip_address, &peer_ip_len);
    if (recv_error == SOCKET_ERROR)
    {
        printf("recvfrom error: %d\n", WSAGetLastError());
        return;
    }
    
    *recieved_ip = peer_ip_address.sin_addr.s_addr;
}
void initiateNewUser(void* parameters)
{
    // casting back to original data
    struct broadcast_param *b = (struct broadcast_param*)parameters;
    const int PORT_NUMBER = b->port;
    long* recieved_ip = b->recieved_ip;

    recieveBroadcast(PORT_NUMBER, recieved_ip);
    notify_IP(*recieved_ip, PORT_NUMBER);
}