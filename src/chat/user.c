#include "user.h"
#include <stdio.h>

int link_contains_user(link_node* link, int target_user_id);

void init_list(link_list* list)
{
    list->head = NULL;
}

void init_user(user* user)
{
    user->links = NULL;

}
void list_add_link(link_list *links, link *add_link)
{
    link_node* add_node = malloc(sizeof(link_node));
    
    if (links->head == NULL)
    {
        add_node->link = add_link;
        links->head = add_node;
        links->head->next = NULL;
    }
    else 
    {
        link_node* current_link = links->head;
        while (current_link->next != NULL)
        {
            current_link = current_link->next;
        }
        
        add_node->link = add_link;
        current_link->next = add_node;
        current_link->next->next = NULL;
    }
}
void list_remove_user_links(link_list *links, int remove_user_id)
{
    link_node* current_link = links->head;
    if (current_link == NULL)
        return;
        
    // edge case where head is removed
    if (link_contains_user(current_link, remove_user_id))
        {
            links->head = current_link->next;
            current_link = links->head;
            return;
        }
    link_node* previous_link;
    link_node* next_link = current_link->next;
    // traverse through each list node
    while (next_link != NULL)
    {
        // determine if link is connected to target user
        if (link_contains_user(next_link, remove_user_id))
        {
            current_link->next = next_link->next;
            return;
        }
        previous_link = current_link;
        current_link = current_link->next;
        if (current_link == NULL)
            break;
        next_link = current_link->next;
    }
    // edge case where last link needs removed
    if (current_link != NULL && link_contains_user(current_link, remove_user_id))
    {
        previous_link->next = NULL;
    }
}
// helper function to check if link contains user to remove
int link_contains_user(link_node* link, int target_user_id)
{
    if (link->link->user_2->user_id == target_user_id)
        return 1;
    else 
        return 0;
}

void print_link(link *l)
{
    printf("outward link to user %d\n", l->user_2->user_id);
}

void print_list(link_list* links)
{
    link_node *current_link = links->head;
    int i = 0;
    while (current_link != NULL)
    {
        printf("Node %d: ", i);
        print_link(current_link->link);
        i++;
        current_link = current_link->next;
    }
}

void print_user_list(user* user)
{
    printf("user %d:\n", user->user_id);
    print_list(user->links);
}

void free_list(link_list *list)
{
    link_node* current_link = list->head;
    link_node* next_link;
    while (current_link != NULL)
    {
        next_link = current_link->next;
        free(current_link);
        current_link = next_link;
    }
    free(list);
}

void assign_user_id(int* counter, user* user)
{
    *counter++;
    user->user_id = *counter;
}