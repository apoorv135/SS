#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Structures.c"


void clientOperations(int desc, int try_count, struct AccountDetails user);
void adminOperations(int desc, int try_count);
void initial_setup();

struct flock readLock(struct flock lock, int fd);
struct flock readUnlock(struct flock lock, int fd);
struct flock writeLock(struct flock lock, int fd);
struct flock writeUnlock(struct flock lock, int fd);
void send_message(int desc, char *msg, char *input);
int bidOperation();
int printTrainInfo(int desc);
int checkTrainSeats(struct BookingDetails book);
int updateTrainSeats(struct BookingDetails book);
int print_bookings(int desc, struct AccountDetails user);
void update_ticket_booking(int desc, int try_count, struct AccountDetails user);
void delete_ticket_booking(int desc, int try_count, struct AccountDetails user);
void bookTickets(int desc, int try_count, struct AccountDetails user);
void logout(int desc, struct AccountDetails user);

void client_connection(int desc);
int get_credentials(int desc);
void verify_credentials(int desc, char *username, char *password);
void clientOperations(int desc, int try_count, struct AccountDetails user);

void admin_train_op(int desc, int try_count);
void admin_user_modify(int desc, int try_count, struct AccountDetails temp, int index);
void admin_user_op(int desc, int try_count);
void adminOperations(int desc, int try_count);
void logout(int desc, struct AccountDetails user);