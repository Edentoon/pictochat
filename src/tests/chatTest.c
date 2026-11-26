#include "../chat/user.h"
#include <stdio.h>

void add_list_test(void);
void remove_list_test(void);

user user_1, user_2, user_3;
link link_1, link_2, link_3;

int main(void)
{ 
    // setup links
    user_1.user_id = 1;
    user_2.user_id = 2;
    user_3.user_id = 3;

    link_1.user_2 = &user_1;
    
    link_2.user_2 = &user_2;

    link_3.user_2 = &user_3;

    remove_list_test();
    return 0;
}

void add_list_test(void)
{
    printf("TEST: list_add_link()\n\n");
    link_list *list = malloc(sizeof(link_list));
    init_list(list);
    
    list_add_link(list, &link_1);
    list_add_link(list, &link_2);
    list_add_link(list, &link_3);

    print_link(list->head->link);
    print_list(list);

    // free memory used by list
    free_list(list);
}

void remove_list_test(void)
{
    printf("TEST: list_remove_user()\n\n");

    link_list *list = malloc(sizeof(link_list));
    init_list(list);

    list_add_link(list, &link_1);
    list_add_link(list, &link_2);
    list_add_link(list, &link_3);

    print_list(list);
    printf("\nremoving user 3\n\n");
    list_remove_user_links(list, 3);
    print_list(list);
    user_1.links = list;
    print_user_list(&user_1);
    free_list(list);
}
