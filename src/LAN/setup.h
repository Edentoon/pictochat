#include <WinSock2.h>
// flag that is set when notify function should cease
static int stop_notify = 0;

// initiates a connection between users, provides calling user with socket and connected user info
int setup_link_socket(const int PORT_NUMBER, struct sockaddr_in *calling_user, 
                      struct sockaddr_in *connected_user, SOCKET *communication_socket);
// broadcasts user ip address to all devices on local network
void broadcastIP(const int PORT_NUMBER);

// struct for passing in two parameters into thread function
struct broadcast_param {
    int port;
    long recieved_ip;
};
// thread function that recieves broadcast IP address and stores the value, 
// and responds with its own ip
void initiateNewUser(void* parameters);
// helper function to listen for udp broadcast from new user
void recieveBroadcast(const int PORT_NUMBER, long* recieved_ip);
// helper function to notify specified ip address that there is a user already in the chat
void notify_IP(long ip_address, const int PORT_NUMBER);