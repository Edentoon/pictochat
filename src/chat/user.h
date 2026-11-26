#include "../LAN/setup.h"
#include <WinSock2.h>
#define CHATROOM_1 0
#define CHATROOM_2 1
#define CHATROOM_3 2
#define CHATROOM_4 3
#define MAX_CHATROOM_USERS 16

// simulates a mesh node
typedef struct {
    int user_id;
    char username[12];
    struct sockaddr_in user_info;
    struct list *links;
} user;

// simulates links between each mesh node
typedef struct {
    user *user_2;
    SOCKET communication_socket;
} link;

typedef struct node {
    link *link;
    struct node *next;
} link_node;

// list of links of a given user
typedef struct list{
    link_node *head;
} link_list;

// returns initialized link list, should be called every time a list is desired
void init_list(link_list* list);

// adds user to user list
void list_add_link(link_list *links, link *link);

// removes links that include a given user upon disconnection
void list_remove_user_links(link_list *links, int user_id);

// prints link info (user ids of connected users) for debugging
void print_link(link *l);

// prints each link node for debugging
void print_list(link_list *list);

// prints each link node from user
void print_user_list(user* user);

// assigns new user an id
void assign_user_id(int* counter, user* user);

// frees memory allocated by list
void free_list(link_list *list);