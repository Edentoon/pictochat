// checks if user is first to join chat room, returns 1 if so, returns ip address of message if not
long checkIfFirstUser();
// starts thread to initiate communication with new user and
// retrieve their ip address, storing this in the argument
void handleNewUser(long *ip_address);