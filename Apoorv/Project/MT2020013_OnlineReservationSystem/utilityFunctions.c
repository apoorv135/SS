#include <declarationHeader.h>

struct flock readLock(struct flock lock, int fd){
    lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
	
	fcntl(fd, F_SETLKW, &lock);
    
    return lock;
}

struct flock readUnlock(struct flock lock, int fd){
    lock.l_type = F_UNLCK;
	
	fcntl(fd, F_SETLK, &lock);
    return lock;
}

struct flock writeLock(struct flock lock, int fd){
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
	
	fcntl(fd, F_SETLKW, &lock);
    
    return lock;
}

struct flock writeUnlock(struct flock lock, int fd){
    lock.l_type = F_UNLCK;
	
	fcntl(fd, F_SETLK, &lock);
    return lock;
}

void send_message(int desc, char *msg, char *input){
    for (size_t i = 0; i < 10000; i++);    
    write(desc, msg, 1024);
    read(desc, input, 1024);
}

int bidOperation(){
    int bid, temp;
    struct flock lock;
    int fd = open("bidInfo.dat", O_RDWR);
    writeLock(lock, fd);
	read(fd, &bid, sizeof(bid));
    temp = bid;
	++bid;
	lseek(fd, 0L, SEEK_SET);
	write(fd, &bid, sizeof(bid));
    writeUnlock(lock, fd);
    close(fd);
    return temp;
}


int printTrainInfo(int desc){
    struct TrainDetails train;
    struct flock lock;
    char buff[1024];
    int fd = open("trainInfo.dat", O_RDONLY);
    readLock(lock, fd);
    int flag = 0;
    char message[1024] = "\nTrain Details are:";
    send_message(desc, "R", buff);
    send_message(desc, message, buff);
    while(read(fd, &train, sizeof(train)) > 0){
        if(strcmp(train.trainStatus, "ACTIVE")==0){
            strcpy(message, train.number);
            strcat(message, "(TrainNumber)\t");
            strcat(message, train.name);
            strcat(message, "(TrainName)\t");
            snprintf(buff, sizeof(buff), "%d", train.totalSeats);
            strcat(message, buff);
            strcat(message, "(TotalSeats)\t");
            snprintf(buff, sizeof(buff), "%d", train.bookedSeats);
            strcat(message, buff);
            strcat(message, "(BookedSeats)\t");
            send_message(desc, "R", buff);
            send_message(desc, message, buff);
            flag = 1;
        }
    }
    readUnlock(lock, fd);
    close(fd);
    if(!flag){
        strcpy(message, "Trains not found. Kindly check with admin.\n");
        send_message(desc, "R", buff);
        send_message(desc, message, buff);
        return 0;
    }
    return 1;
}

int checkTrainSeats(struct BookingDetails book){
    struct TrainDetails train;
    struct flock lock;
    int index = 0, seatsRemaining;
    int fd = open("trainInfo.dat", O_RDWR);
    writeLock(lock, fd);
    while(read(fd, &train, sizeof(train))>0){
        if(strcmp(train.trainStatus, "ACTIVE")==0 && strcmp(train.number, book.trainNo)==0){
            seatsRemaining = train.totalSeats - train.bookedSeats;
            if(book.NumOfSeats<=seatsRemaining){
                train.bookedSeats += book.NumOfSeats;
                lseek(fd, index*sizeof(train), SEEK_SET);
                write(fd, &train, sizeof(train));
                writeUnlock(lock, fd);
                close(fd);
                return 1;
            }
            else{
                writeUnlock(lock, fd);
                close(fd);
                return 0;
            }
        }
        ++index;
    }
    writeUnlock(lock, fd);
    close(fd);
    return 2;
}

int updateTrainSeats(struct BookingDetails book){
    struct TrainDetails train;
    struct flock lock;
    int index = 0;
    int fd = open("trainInfo.dat", O_RDWR);
    writeLock(lock, fd);
    while(read(fd, &train, sizeof(train))>0){
        if(strcmp(train.trainStatus, "ACTIVE")==0 && strcmp(train.number, book.trainNo)==0){
            train.bookedSeats = train.bookedSeats + book.NumOfSeats;
            lseek(fd, index*sizeof(train), SEEK_SET);
            write(fd, &train, sizeof(train));
            writeUnlock(lock, fd);
            close(fd);
            return 1;
        }
        ++index;
    }
    writeUnlock(lock, fd);
    close(fd);
    printf("Train Not found for updating the seats.\n");
    return 0;
}


int print_bookings(int desc, struct AccountDetails user){
    struct BookingDetails book;
    struct flock lock;
    int fd = open("bookingInfo.dat", O_RDONLY); 
    readLock(lock, fd);
    int flag = 0;
    char input[1024];
    char msg[1024] = "\nView Previous Bookings - User ";
    strcat(msg, user.username);
    send_message(desc, "R", input);
    send_message(desc, msg, input);
    while(read(fd, &book, sizeof(book)) > 0){
        if(strcmp(book.accountNo, user.username)==0){
            snprintf(input, sizeof(input), "%d", book.bookingNumber);
            strcpy(msg, input);
            strcat(msg, "(booking ID)\t");
            strcat(msg, book.trainNo);
            strcat(msg, "(train)\t");
            strcat(msg, book.accountNo);
            strcat(msg, "(username)\t");
            snprintf(input, sizeof(input), "%d", book.NumOfSeats);
            strcat(msg, input);
            strcat(msg, "(seats booked)\t");
            strcat(msg, book.bookingStatus);
            strcat(msg, "(status)\t");
            send_message(desc, "R", input);
            send_message(desc, msg, input);
            flag = 1;
        }
    }
    readUnlock(lock, fd);
    close(fd);
    if(!flag){
        strcpy(msg, "**No Booking found. Please book first.\n");
        send_message(desc, "R", input);
        send_message(desc, msg, input);
        return 0;
    }
    strcpy(msg, "\n");
    send_message(desc, "R", input);
    send_message(desc, msg, input);
    return 1;

}

void update_ticket_booking(int desc, int try_count, struct AccountDetails user){
    char buff[1024];
    struct flock lock;
    int flag = 0, index = 0, diff = 0;
    struct BookingDetails book, temp;
    if(try_count<=0){
        char message[1024] = "\n**Maximum invalid input limit reached. Shutting down.";
        send_message(desc, "R", buff);
        send_message(desc, message, buff);
        logout(desc, user);
        return;
    }

    printTrainInfo(desc);
    if(print_bookings(desc, user)){
        char message[1024] = "Update A Ticket Booking(Seats)- Add Booking Details";
        send_message(desc, "R", buff);
        send_message(desc, message, buff);
        strcpy(message, "Enter Booking Number: ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        sscanf(buff, "%d", &book.bookingNumber);
        strcpy(message, "Enter to be booked number of seats in the train(>0)(subject to availability)(new): ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        sscanf(buff, "%d", &book.NumOfSeats);
        if(book.NumOfSeats<=0){
            strcpy(message, "\n**Invalid Number of seats entered, Please try again.");
            send_message(desc, "R", buff);
            send_message(desc, message, buff);
            update_ticket_booking(desc, --try_count, user);
        }
        else{
            int fd = open("bookingInfo.dat", O_RDONLY);
            readLock(lock, fd);
            while(read(fd, &temp, sizeof(temp))>0){
                if(strcmp(temp.bookingStatus, "CONFIRMED")==0 && temp.bookingNumber == book.bookingNumber){
                    flag = 1;
                    temp.NumOfSeats = book.NumOfSeats - temp.NumOfSeats;
                    break;
                }
                ++index;
            }
            readUnlock(lock, fd);
            close(fd);        
            if(!flag){
                strcpy(message, "\n**Invalid Booking Number entered or Booking is Cancelled, Please Enter valid Booking Number");
                send_message(desc, "R", buff);
                send_message(desc, message, buff);
                clientOperations(desc, --try_count, user);
            }
            else{
                switch (checkTrainSeats(temp)){
                    case 0:
                        strcpy(message, "\n**");
                        snprintf(buff, sizeof(buff), "%d", temp.NumOfSeats);
                        strcat(message, buff);
                        strcat(message, " Seats not available on Train Number ");
                        strcat(message, book.trainNo);
                        strcat(message, ", Please try again.");
                        send_message(desc, "R", buff);
                        send_message(desc, message, buff);
                        clientOperations(desc, --try_count, user);
                        return;
                    case 1:
                        fd = open("bookingInfo.dat", O_RDWR);
                        writeLock(lock, fd);
                        temp.NumOfSeats = book.NumOfSeats;
                        lseek(fd, index*sizeof(temp), SEEK_SET);
                        write(fd, &temp, sizeof(temp));
                        writeUnlock(lock, fd);
                        close(fd);
                        strcpy(message, "\n*Booking with Booking ID - ");
                        snprintf(buff, sizeof(buff), "%d", temp.bookingNumber);
                        strcat(message, buff);
                        strcat(message, ", successfully updated, Now Number of seats booked are ");
                        snprintf(buff, sizeof(buff), "%d", temp.NumOfSeats);
                        strcat(message, buff);
                        strcat(message, " on train ");
                        strcat(message, temp.trainNo);
                        strcat(message, "\n");
                        send_message(desc, "R", buff);
                        send_message(desc, message, buff);
                        clientOperations(desc, 2, user);
                        return;
                    case 2:
                        strcpy(message, "\n**Train Number ");
                        strcat(message, temp.trainNo);
                        strcat(message, " NOT FOUND. Please try again.\n");
                        send_message(desc, "R", buff);
                        send_message(desc, message, buff);
                        return;
                }


            }
        }
    }else{
        logout(desc, user);
        return;
    }
}

void delete_ticket_booking(int desc, int try_count, struct AccountDetails user){
    char buff[1024];
    struct flock lock;
    int flag = 0, index = 0;
    struct BookingDetails book, temp;
    if(try_count<=0){
        char message[1024] = "\n**Maximum invalid input limit reached. Shutting down.";
        send_message(desc, "R", buff);
        send_message(desc, message, buff);
        logout(desc, user);
        return;
    }
    if(print_bookings(desc, user)){
        char message[1024] = "Delete A Ticket Booking- Add Booking Details";
        send_message(desc, "R", buff);
        send_message(desc, message, buff);
        strcpy(message, "Enter Booking Number: ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        sscanf(buff, "%d", &book.bookingNumber);
        int fd = open("bookingInfo.dat", O_RDONLY);
        readLock(lock, fd);
        while(read(fd, &temp, sizeof(temp))>0){
            if(strcmp(temp.bookingStatus, "CONFIRMED")==0 && temp.bookingNumber == book.bookingNumber){
                temp.NumOfSeats = -temp.NumOfSeats;
                if(updateTrainSeats(temp)){
                    strcpy(temp.bookingStatus, "CANCELLED");
                    temp.NumOfSeats = 0;
                    flag = 1;
                }
                break;
            }
            ++index;
        }
        readUnlock(lock, fd);
        close(fd);
        if(!flag){
            strcpy(message, "\n**Invalid Booking Number entered or Booking is already Cancelled, Please Enter valid Booking Number");
            send_message(desc, "R", buff);
            send_message(desc, message, buff);
            clientOperations(desc, --try_count, user);
        }
        else{
            int fd = open("bookingInfo.dat", O_RDWR);
            writeLock(lock, fd);
            lseek(fd, index*sizeof(temp), SEEK_SET);
            write(fd, &temp, sizeof(temp));
            writeUnlock(lock, fd);
            close(fd);
            strcpy(message, "\n*Booking with Booking ID - ");
            snprintf(buff, sizeof(buff), "%d", temp.bookingNumber);
            strcat(message, buff);
            strcat(message, ", successfully deleted, On train ");
            strcat(message, temp.trainNo);
            strcat(message, "\n");
            send_message(desc, "R", buff);
            send_message(desc, message, buff);
            clientOperations(desc, 2, user);
            return;
        }
    }else{
        logout(desc, user);
        return;
    }
}

void bookTickets(int desc, int try_count, struct AccountDetails user){
    char buff[1024];
    struct flock lock;
    struct BookingDetails book, temp;
    int fd;
    if(try_count<=0){
        char message[1024] = "\nInvalid input limit reached. Try logging after some time.";
        send_message(desc, "R", buff);
        send_message(desc, message, buff);
        logout(desc, user);
        return;
    }
    if(printTrainInfo(desc)){
        char message[1024] = "\nLets book a ticket, fill required info :";
        send_message(desc, "R", buff);
        send_message(desc, message, buff);
        strcpy(message, "Enter Train Number: ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        strcpy(book.trainNo, buff);
        strcpy(message, "(seats will be booked only if available)\nEnter number of seats to be booked : ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        sscanf(buff, "%d", &book.NumOfSeats);
        book.bookingNumber = bidOperation();
        strcpy(book.accountNo, user.username);
        strcpy(book.bookingStatus, "TICKETS CONFIRMED");
        if(book.NumOfSeats<=0){
            strcpy(message, "\n**Incorrect number of seats entered, Kindly try again.");
            send_message(desc, "R", buff);
            send_message(desc, message, buff);
            clientOperations(desc, --try_count, user);
        }
        else{
            switch (checkTrainSeats(book)){
            case 0:
                strcpy(message, "\n**");
                snprintf(buff, sizeof(buff), "%d", book.NumOfSeats);
                strcat(message, buff);
                strcat(message, " Seats not available on Train Number ");
                strcat(message, book.trainNo);
                strcat(message, ", Please try again.");
                send_message(desc, "R", buff);
                send_message(desc, message, buff);
                clientOperations(desc, --try_count, user);
                return;
            case 1:
                fd = open("bookingInfo.dat", O_RDWR);
                writeLock(lock, fd);
                while(read(fd, &temp, sizeof(temp))>0);
                write(fd, &book, sizeof(book));
                writeUnlock(lock, fd);
                close(fd);
                strcpy(message, "\nBooking ID - ");
                snprintf(buff, sizeof(buff), "%d", book.bookingNumber);
                strcat(message, buff);
                strcat(message, ", Successfully booked number of seats :  ");
                snprintf(buff, sizeof(buff), "%d", book.NumOfSeats);
                strcat(message, buff);
                strcat(message, " on train ");
                strcat(message, book.trainNo);
                strcat(message, "\n");
                send_message(desc, "R", buff);
                send_message(desc, message, buff);
                clientOperations(desc, 2, user);
                return;
            case 2:
                strcpy(message, "\nTrain Number ");
                strcat(message, book.trainNo);
                strcat(message, " NOT FOUND. Kindly try again.");
                send_message(desc, "R", buff);
                send_message(desc, message, buff);
                clientOperations(desc, --try_count, user);
                return;
            }
        }
    }else{
        logout(desc, user);
        return;
    }
}

void client_connection(int desc){
    char input[1024];
    read(desc, input, sizeof(input));
    get_credentials(desc);
}

int get_credentials(int desc){
    char username_msg[1024] = "Enter Username: ";
    char password_msg[1024] = "Enter Password: ";
    char username[1024], password[1024];
    char welcome_msg[1024] = "------------Welcome to Ticketing System------------";
    char login_msg[1024] = "Kindly Enter your credentials";
    char input[1024];
    send_message(desc, "R", input);
    send_message(desc, welcome_msg, input);
    send_message(desc, "R", input);
    send_message(desc, login_msg, input);
    send_message(desc, "RW", input);
    send_message(desc, username_msg, username);
    send_message(desc, "RW", input);
    send_message(desc, password_msg, password);
    verify_credentials(desc, username, password);
    send_message(desc, "CLOSE", input);
    return 0;  
}

void verify_credentials(int desc, char *username, char *password){
    struct AccountDetails account;
    struct flock lock;
    int fd = open("loginInfo.dat", O_RDWR);
    writeLock(lock, fd);
    char input[1024];
    int index = 0;
    if(fd == -1){
        char file_not_found[] = "Incorrect UserName or Password. Try Again.\n"; 
        send_message(desc, "R", input);
        send_message(desc, file_not_found, input);
        send_message(desc, "CLOSE", input);
    }
    while(read(fd, &account, sizeof(account)) > 0){
        if( strcmp(account.username, username)==0 && strcmp(account.password, password)==0 && strcmp(account.accountStatus, "ACTIVE") == 0){
            if(account.accountType==1 && account.sessionFlag==1){
                char welcome_msg[1024] = "\nWelcome ";
                strcat(welcome_msg, account.username);
                strcat(welcome_msg, "\n**You are already logged in. You can't login again.\n");
                send_message(desc, "R", input);
                send_message(desc, welcome_msg, input);
                writeUnlock(lock, fd);
                close(fd);
                send_message(desc, "CLOSE", input);
                return;
            }
            account.sessionFlag = 1;
            lseek(fd, index*sizeof(account), SEEK_SET);
            write(fd, &account, sizeof(account));
            writeUnlock(lock, fd);
            close(fd);
            char welcome_msg[1024] = "\nWelcome ";
            strcat(welcome_msg, account.username);
            send_message(desc, "R", input);
            send_message(desc, welcome_msg, input);
            switch(account.accountType){
                case 1:
                    clientOperations(desc, 2, account);
                    logout(desc, account);
                    return;
                case 2:
                    clientOperations(desc, 2, account);
                    logout(desc, account);
                    return;
                case 3:
                    adminOperations(desc, 2);
                    logout(desc, account);
                    return;
            }
            break;
        }
        ++index;
    }
    char user_not_found[] = "Incorrect UserName or Password. Try Again.\n"; 
    send_message(desc, "R", input);
    send_message(desc, user_not_found, input);
    send_message(desc, "CLOSE", input);
}

void clientOperations(int desc, int try_count, struct AccountDetails user){
    char input[1024];
    int flag = 0;
    if(try_count<=0){
        char message[1024] = "\n**Maximum invalid input limit reached. Shutting down.";
        send_message(desc, "R", input);
        send_message(desc, message, input);
        return;
    }
    char message[1024] = "Press 1 to Book Ticket\nPress 2 to View Previous Bookings\nPress 3 to Update Booking\nPress 4 to Cancel Booking\nPress any other key to Exit\nEnter your choice: ";
    send_message(desc, "RW", input);
    send_message(desc, message, input);
    if(strcmp("1", input)==0){
        bookTickets(desc, 2, user);
        clientOperations(desc, 2, user);
    }else if(strcmp("2", input) == 0){
        if(!print_bookings(desc, user)){
            clientOperations(desc, --try_count, user);
        }
        clientOperations(desc, 2, user);
    }else if(strcmp("3", input) == 0){
        update_ticket_booking(desc, 2, user);
        clientOperations(desc, 2, user);
    }else if(strcmp("4", input) == 0){
        delete_ticket_booking(desc, 2, user);
        clientOperations(desc, 2, user);        
    }else{
        logout(desc, user);
        return;
    }
}



void client_connection(int desc){
    char input[1024];
    read(desc, input, sizeof(input));
    get_credentials(desc);
}



void admin_train_modify(int desc, int try_count, struct TrainDetails temp, int index){
    char buff[1024];
    struct flock lock;
    int flag = 0;
    if(try_count<=0){
        char message[1024] = "\n**Maximum invalid input limit reached. Shutting down.";
        send_message(desc, "R", buff);
        send_message(desc, message, buff);
        return;
    }
    char message[1024]= "\nTrain Attributes Modification - ";
    strcat(message, temp.number);
    strcat(message, "\nPress 1 to Modify Train Name\nPress 2 to Modify Train Total Seats\nPress 3 to Modify Train Booked Seats\nPress any other key to Exit\nEnter your choice: ");
    send_message(desc, "RW", buff);
    send_message(desc, message, buff);
    if(strcmp("1", buff) == 0){
        strcpy(message, "Enter Train Name (new): ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        strcpy(temp.name, buff);
        strcpy(message, "*Train Name details modified successfully\n");
        flag = 1;
    }
    else if(strcmp("2", buff) == 0){
        int total_seats;
        strcpy(message, "Enter total seats in the train(>0) (new): ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        sscanf(buff, "%d", &total_seats);
        if(total_seats<=0){
            strcpy(message, "**Invalid Number of seats entered, Please try again.");
            send_message(desc, "R", buff);
            send_message(desc, message, buff);
            admin_train_modify(desc, --try_count, temp, index);
        }
        else{
            temp.totalSeats = total_seats;
            strcpy(message, "*Train total seats details modified successfully.\n");
            flag = 1;
        }
        
    }else if(strcmp("3", buff) == 0){
        int booked_seats;
        strcpy(message, "Enter booked seats in the train(>0) (new): ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        sscanf(buff, "%d", &booked_seats);
        if(booked_seats<0 && booked_seats>temp.totalSeats){
            strcpy(message, "**Invalid Number of seats entered, Please try again.");
            send_message(desc, "R", buff);
            send_message(desc, message, buff);
            admin_train_modify(desc, --try_count, temp, index);
        }
        else{
            temp.bookedSeats = booked_seats;
            flag = 1;
            strcpy(message, "*Train booked seats details modified successfully.\n");
        }
    }else{
        return ;
    }
    if(flag){
        char msg[1024] = "\nTrain Number ";
        char value[100];
        strcat(msg, temp.number);
        strcat(msg, " (");
        strcat(msg, temp.name);
        strcat(msg, ") modified details are:");
        send_message(desc, "R", buff);
        send_message(desc, msg, buff);
        strcpy(msg, temp.number);
        strcat(msg, "\t");
        strcat(msg, temp.name);
        strcat(msg, "\t");
        snprintf(value, sizeof(value), "%d", temp.totalSeats);
        strcat(msg, value);
        strcat(msg, "(total seats)\t");
        snprintf(value, sizeof(value), "%d", temp.bookedSeats);
        strcat(msg, value);
        strcat(msg, "(booked seats)\t");
        strcat(msg, temp.trainStatus);
        send_message(desc, "R", buff);
        send_message(desc, msg, buff);
        int fd = open("trainInfo.dat", O_RDWR);
        writeLock(lock, fd);
        lseek(fd, index*sizeof(temp), SEEK_SET);
        write(fd, &temp, sizeof(temp));
        writeUnlock(lock, fd);
        close(fd);
        send_message(desc, "R", buff);
        send_message(desc, message, buff);
        adminOperations(desc, 2);
        return;
    }
}

void admin_train_op(int desc, int try_count){
    char buff[1024];
    int index = 0;
    struct flock lock;
    if(try_count<=0){
        char message[1024] = "\n**Maximum invalid input limit reached. Shutting down.";
        send_message(desc, "R", buff);
        send_message(desc, message, buff);
        return;
    }
    char message[1024]= "\nTrain Operations\nPress 1 to Add a Train\nPress 2 to Delete a Train\nPress 3 to Update a Train\nPress 4 to Retrieve Train Details\nPress any other key to Exit\nEnter your choice: ";
    struct TrainDetails train, temp;
    int flag = 0;
    send_message(desc, "RW", buff);
    send_message(desc, message, buff);
    if(strcmp("1", buff) == 0){
        char message[1024] = "\nOperation Add a Train- Add Train Details";
        send_message(desc, "R", buff);
        send_message(desc, message, buff);
        strcpy(message, "Enter Train Number: ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        strcpy(train.number, buff);
        strcpy(message, "Enter Train Name: ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        strcpy(train.name, buff);
        strcpy(message, "Enter total seats in the train(>0): ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        sscanf(buff, "%d", &train.totalSeats);
        if(train.totalSeats<=0){
            strcpy(message, "**Invalid Number of seats entered, Please try again.");
            send_message(desc, "R", buff);
            send_message(desc, message, buff);
            admin_train_op(desc, --try_count);
        }
        train.bookedSeats = 0;
        strcpy(train.trainStatus, "ACTIVE");
        int fd = open("trainInfo.dat", O_RDWR);
        writeLock(lock, fd);
        while(read(fd, &temp, sizeof(temp)) > 0){
            if(strcmp(train.number, temp.number)==0 && strcmp(temp.trainStatus, "ACTIVE")==0){
                char msg[1024] = "**Train Number ";
                strcat(msg, train.number);
                strcat(msg, " is already present. Cannot add Train with same train number, Kindly try again.");
                send_message(desc, "R", buff);
                send_message(desc, msg, buff);
                flag = 1;
                break;
            }
            else if(strcmp(train.number, temp.number)==0 && strcmp(temp.trainStatus, "INACTIVE")==0){                        
                lseek(fd, index*sizeof(temp), SEEK_SET);
                write(fd, &train, sizeof(train));
                writeUnlock(lock, fd);
                close(fd);
                char msg[1024] = "*Train Number ";
                strcat(msg, train.number);
                strcat(msg, " (");
                strcat(msg, train.name);
                strcat(msg, ") is successfully added in the train list.\n");
                send_message(desc, "R", buff);
                send_message(desc, msg, buff);
                adminOperations(desc, 2);
                return;
            }
            ++index;
        } 
        if(flag){
            writeUnlock(lock, fd);
            close(fd);
            admin_train_op(desc, --try_count);
        }
        else{
            write(fd, &train, sizeof(train));
            writeUnlock(lock, fd);
            close(fd);
            char msg[1024] = "*Train Number ";
            strcat(msg, train.number);
            strcat(msg, " (");
            strcat(msg, train.name);
            strcat(msg, ") is successfully added in the train list.\n");
            send_message(desc, "R", buff);
            send_message(desc, msg, buff);
            adminOperations(desc, 2);
        }
    }else if(strcmp("2", buff) == 0){
        char message[1024] = "\nOperation Delete a Train";
        int index = 0;
        send_message(desc, "R", buff);
        send_message(desc, message, buff);
        strcpy(message, "Enter Train Number(to be deleted): ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        strcpy(train.number, buff);
        int fd = open("trainInfo.dat", O_RDWR);
        writeLock(lock, fd);
        while(read(fd, &temp, sizeof(temp)) > 0){
            if(strcmp(train.number, temp.number)==0 && strcmp(temp.trainStatus, "ACTIVE")==0 && temp.bookedSeats==0){
                strcpy(temp.trainStatus, "INACTIVE");
                flag = 1;
                break;
            }
            ++index;
        }
        if(flag){
            lseek(fd, index*sizeof(temp), SEEK_SET);
            write(fd, &temp, sizeof(temp));
            writeUnlock(lock, fd);
            close(fd);
            char msg[1024] = "*Train Number ";
            strcat(msg, temp.number);
            strcat(msg, " (");
            strcat(msg, temp.name);
            strcat(msg, ") is successfully deleted from the train list.\n");
            send_message(desc, "R", buff);
            send_message(desc, msg, buff);
            adminOperations(desc, 2);
        }else{
            writeUnlock(lock, fd);
            close(fd);
            char msg[1024] = "**Train Number ";
            strcat(msg, train.number);
            strcat(msg, " not found in the train list OR Train has Bookings registered to it. Kindly try again.");
            send_message(desc, "R", buff);
            send_message(desc, msg, buff);
            admin_train_op(desc, --try_count);
        }        
    }else if(strcmp("3", buff) == 0){
        char message[1024] = "\nOperation Modify a Train- Update Train Details";
        int index = 0;
        send_message(desc, "R", buff);
        send_message(desc, message, buff);
        strcpy(message, "Enter Train Number(to be modified): ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        strcpy(train.number, buff);
        int fd = open("trainInfo.dat", O_RDONLY);
        readLock(lock, fd);
        while(read(fd, &temp, sizeof(temp)) > 0){
            if(strcmp(train.number, temp.number)==0 && strcmp(temp.trainStatus, "ACTIVE")==0){
                flag = 1;
                break;
            }
            ++index;
        }
        readUnlock(lock, fd);
        close(fd);
            
        if(flag){
            admin_train_modify(desc, 2, temp, index);
            adminOperations(desc, 2);
        }else{
            char msg[1024] = "**Train Number ";
            strcat(msg, train.number);
            strcat(msg, " not found in the train list. Kindly try again.");
            send_message(desc, "R", buff);
            send_message(desc, msg, buff);
            admin_train_op(desc, --try_count);
        }
        
    }else if(strcmp("4", buff) == 0){
        char message[1024] = "\nOperation Retrieve a Train- Retrieve Train Details";
        send_message(desc, "R", buff);
        send_message(desc, message, buff);
        strcpy(message, "Enter Train Number(to be retrieved): ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        strcpy(train.number, buff);
        int fd = open("trainInfo.dat", O_RDONLY);
        readLock(lock, fd);
        while(read(fd, &temp, sizeof(temp)) > 0){
            if(strcmp(train.number, temp.number)==0){
                flag = 1;
                break;
            }
        }
        readUnlock(lock, fd);
        close(fd);
        
        if(flag){
            char value[100];
            char msg[1024] = "\nTrain Number ";
            strcat(msg, temp.number);
            strcat(msg, " (");
            strcat(msg, temp.name);
            strcat(msg, ") details are:");
            send_message(desc, "R", buff);
            send_message(desc, msg, buff);
            strcpy(msg, temp.number);
            strcat(msg, "\t");
            strcat(msg, temp.name);
            strcat(msg, "\t");
            snprintf(value, sizeof(value), "%d", temp.totalSeats);
            strcat(msg, value);
            strcat(msg, "(total seats)\t");
            snprintf(value, sizeof(value), "%d", temp.bookedSeats);
            strcat(msg, value);
            strcat(msg, "(booked seats)\t");
            strcat(msg, temp.trainStatus);
            strcat(msg, "\n");
            send_message(desc, "R", buff);
            send_message(desc, msg, buff);            
            adminOperations(desc, 2);
        }else{
            char msg[1024] = "\n**Train Number ";
            strcat(msg, train.number);
            strcat(msg, " not found in the train list. Kindly try again.");
            send_message(desc, "R", buff);
            send_message(desc, msg, buff);
            admin_train_op(desc, --try_count);
        }

    }else{
        return ;
    }
}

void admin_user_modify(int desc, int try_count, struct AccountDetails temp, int index){
    char buff[1024];
    int flag = 0;
    struct flock lock;
    if(try_count<=0){
        char message[1024] = "\n**Maximum invalid input limit reached. Shutting down.";
        send_message(desc, "R", buff);
        send_message(desc, message, buff);
        return;
    }
    char message[1024]= "\nAccount Attributes Modification - ";
    strcat(message, temp.username);
    strcat(message, "\nPress 1 to change Account Password\nPress 2 to Modify Account Type\nPress any other key to Exit\nEnter your choice: ");
    send_message(desc, "RW", buff);
    send_message(desc, message, buff);
    if(strcmp("1", buff) == 0){
        strcpy(message, "Enter Account Password(new): ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        strcpy(temp.password, buff);
        strcpy(message, "*Account password changed successfully\n");
        flag = 1;
    }
    else if(strcmp("2", buff) == 0){
        int type;
        strcpy(message, "Enter Account type - 1 for User or 2 for Agent or 3 for Admin: ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        sscanf(buff, "%d", &type);
        if(type<=0 && type>3){
            strcpy(message, "**Invalid Account type, Please try again.");
            send_message(desc, "R", buff);
            send_message(desc, message, buff);
            admin_user_modify(desc, --try_count, temp, index);
        }
        else{
            temp.accountType = type;
            strcpy(message, "*Account type modified successfully\n");
            flag = 1;
        }
    }else{
        return ;
    }

    if(flag){
        char msg[1024] = "\nUsername ";
        char value[100];
        strcat(msg, temp.username);
        strcat(msg, " (");
        switch (temp.accountType){
        case 1:
            strcat(msg, "(User) ");
            strcpy(value, "User(account type)\t");
            break;
        case 2:
            strcat(msg, "(Agent) ");
            strcpy(value, "Agent(account type)\t");
            break;
        case 3:
            strcat(msg, "(Admin) ");
            strcpy(value, "Admin(account type)\t");
            break;
        default:
            strcat(msg, "(Not Possible) ");
            strcpy(value, "NA(account type)\t");
            break;
        }
        strcat(msg, "modified details are:");
        send_message(desc, "R", buff);
        send_message(desc, msg, buff);
        strcpy(msg, temp.username);
        strcat(msg, "\t");
        strcat(msg, temp.password);
        strcat(msg, "(password)\t");
        strcat(msg, value);
        strcat(msg, temp.accountStatus);
        send_message(desc, "R", buff);
        send_message(desc, msg, buff);            
        int fd = open("loginInfo.dat", O_RDWR);
        writeLock(lock, fd);
        lseek(fd, index*sizeof(temp), SEEK_SET);
        write(fd, &temp, sizeof(temp));
        writeUnlock(lock, fd);
        close(fd);
        send_message(desc, "R", buff);
        send_message(desc, message, buff);
        adminOperations(desc, 2);
        return;
    }
}

void admin_user_op(int desc, int try_count){
    char buff[1024];
    struct flock lock;
    int index = 0;
    if(try_count<=0){
        char message[1024] = "\n**Maximum invalid input limit reached. Shutting down.";
        send_message(desc, "R", buff);
        send_message(desc, message, buff);
        return;
    }
    char message[1024]= "\nUser/Agent/Admin Account Operations\nPress 1 to Add an Account\nPress 2 to Delete an Account\nPress 3 to Update an Account\nPress 4 to Retrieve an Account Details\nPress any other key to Exit\nEnter your choice: ";
    struct AccountDetails user, temp;
    int flag = 0;
    send_message(desc, "RW", buff);
    send_message(desc, message, buff);
    if(strcmp("1", buff) == 0){
        char message[1024] = "\nOperation Add an Account- Add Account Details";
        send_message(desc, "R", buff);
        send_message(desc, message, buff);
        strcpy(message, "Enter Account Name: ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        strcpy(user.username, buff);
        strcpy(message, "Enter Account Password: ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        strcpy(user.password, buff);
        strcpy(message, "Enter Account type - 1 for User or 2 for Agent or 3 for Admin: ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        sscanf(buff, "%d", &user.accountType);
        user.sessionFlag = 0;
        if(user.accountType<=0 || user.accountType>3){
            strcpy(message, "**Invalid Account type, Please try again.");
            send_message(desc, "R", buff);
            send_message(desc, message, buff);
            admin_user_op(desc, --try_count);
        }
        else{
            strcpy(user.accountStatus, "ACTIVE");
            int fd = open("loginInfo.dat", O_RDWR);
            writeLock(lock, fd);
            while(read(fd, &temp, sizeof(temp)) > 0){
                if(strcmp(user.username, temp.username)==0 && strcmp(temp.accountStatus, "ACTIVE")==0){
                    char msg[1024] = "**Account ";
                    strcat(msg, user.username);
                    strcat(msg, " is already present. Cannot add another account with same username, Kindly try again.");
                    send_message(desc, "R", buff);
                    send_message(desc, msg, buff);
                    flag = 1;
                    break;
                }
                else if(strcmp(user.username, temp.username)==0 && strcmp(temp.accountStatus, "INACTIVE")==0){
                    lseek(fd, index*sizeof(temp), SEEK_SET);
                    write(fd, &user, sizeof(user));
                    writeUnlock(lock, fd);
                    close(fd);
                    char msg[1024] = "*Account ";
                    strcat(msg, user.username);
                    strcat(msg, "(");
                    switch (user.accountType)
                    {
                    case 1:
                        strcat(msg, "(User) ");
                        break;
                    case 2:
                        strcat(msg, "(Agent) ");
                        break;
                    case 3:
                        strcat(msg, "(Admin) ");
                        break;
                    default:
                        strcat(msg, "(Not Possible) ");
                        break;
                    }
                    strcat(msg, "is successfully added in the accounts list.\n");
                    send_message(desc, "R", buff);
                    send_message(desc, msg, buff);
                    adminOperations(desc, 2);
                    return;
                }
                ++index;
            }
            if(flag){
                writeUnlock(lock, fd);
                close(fd);
                admin_user_op(desc, --try_count);
            }
            else{
                write(fd, &user, sizeof(user));
                writeUnlock(lock, fd);
                close(fd);
                char msg[1024] = "*Account ";
                strcat(msg, user.username);
                strcat(msg, "(");
                switch (user.accountType)
                {
                case 1:
                    strcat(msg, "(User) ");
                    break;
                case 2:
                    strcat(msg, "(Agent) ");
                    break;
                case 3:
                    strcat(msg, "(Admin) ");
                    break;
                default:
                    strcat(msg, "(Not Possible) ");
                    break;
                }
                strcat(msg, "is successfully added in the accounts list.\n");
                send_message(desc, "R", buff);
                send_message(desc, msg, buff);
                adminOperations(desc, 2);
            }
        }        
    }else if(strcmp("2", buff) == 0){
        char message[1024] = "\nOperation Delete an Account";
        int index = 0;
        send_message(desc, "R", buff);
        send_message(desc, message, buff);
        strcpy(message, "Enter username(to be deleted): ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        strcpy(user.username, buff);
        int fd = open("loginInfo.dat", O_RDWR);
        writeLock(lock, fd);
        int fd2 = open("bookingInfo.dat", O_RDONLY);
        struct flock lock2;
        lock2 = readLock(lock2, fd2);
        struct BookingDetails book;
        while(read(fd, &temp, sizeof(temp)) > 0){
            if(strcmp(user.username, temp.username)==0 && strcmp(temp.accountStatus, "ACTIVE")==0){
                while(read(fd2, &book, sizeof(book))>0){
                    if(strcmp(user.username, book.accountNo)==0 && strcmp(book.bookingStatus, "CONFIRMED")==0){
                        flag = 0;
                        break;
                    }
                }
                strcpy(temp.accountStatus, "INACTIVE");
                flag = 1;
                break;
            }
            ++index;
        }
        lock2 = readUnlock(lock2, fd2);
        close(fd2);                
        if(flag){
            lseek(fd, index*sizeof(temp), SEEK_SET);
            write(fd, &temp, sizeof(temp));
            writeUnlock(lock, fd);
            close(fd);
            char msg[1024] = "*Account ";
            strcat(msg, temp.username);
            switch (temp.accountType){
            case 1:
                strcat(msg, "(User) ");
                break;
            case 2:
                strcat(msg, "(Agent) ");
                break;
            case 3:
                strcat(msg, "(Admin) ");
                break;
            default:
                strcat(msg, "(Not Possible) ");
                break;
            }
            strcat(msg, "is successfully deleted from the accounts list.\n");
            send_message(desc, "R", buff);
            send_message(desc, msg, buff);
            adminOperations(desc, 2);
        }else{
            writeUnlock(lock, fd);
            close(fd);
            char msg[1024] = "**Account (";
            strcat(msg, user.username);
            strcat(msg, ") not found in the accounts list OR Bookings done for given user account. Kindly try again.");
            send_message(desc, "R", buff);
            send_message(desc, msg, buff);
            admin_user_op(desc, --try_count);
        }        
    }else if(strcmp("3", buff) == 0){
        char message[1024] = "\nOperation Modify an Account- Update Account Details";
        int index = 0;
        send_message(desc, "R", buff);
        send_message(desc, message, buff);
        strcpy(message, "Enter username(to be modified): ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        strcpy(user.username, buff);
        int fd = open("loginInfo.dat", O_RDONLY);
        readLock(lock, fd);
        while(read(fd, &temp, sizeof(temp)) > 0){
            if(strcmp(user.username, temp.username)==0 && strcmp(temp.accountStatus, "ACTIVE")==0){
                flag = 1;
                break;
            }
            ++index;
        }
        readUnlock(lock, fd);
        close(fd);
            
        if(flag){
            admin_user_modify(desc, 2, temp, index);
            adminOperations(desc, 2);
        }else{
            char msg[1024] = "**Account (";
            strcat(msg, user.username);
            strcat(msg, ") not found in the accounts list. Kindly try again.");
            send_message(desc, "R", buff);
            send_message(desc, msg, buff);
            admin_user_op(desc, --try_count);
        }
        
    }else if(strcmp("4", buff) == 0){
        char message[1024] = "\nOperation Retrieve an Account- Retrieve Account Details";
        send_message(desc, "R", buff);
        send_message(desc, message, buff);
        strcpy(message, "Enter username(to be retrieved): ");
        send_message(desc, "RW", buff);
        send_message(desc, message, buff);
        strcpy(user.username, buff);
        int fd = open("loginInfo.dat", O_RDWR);
        writeLock(lock, fd);
        while(read(fd, &temp, sizeof(temp)) > 0){
            if(strcmp(user.username, temp.username)==0){
                flag = 1;
                break;
            }
        }
        writeUnlock(lock, fd);
        close(fd);
        char msg[1024] = "\nUsername ";
        if(flag){
            char value[100];
            strcat(msg, temp.username);
            strcat(msg, " (username)");
            switch (temp.accountType){
            case 1:
                strcat(msg, "(User) ");
                strcpy(value, "User(account type)\t");
                break;
            case 2:
                strcat(msg, "(Agent) ");
                strcpy(value, "Agent(account type)\t");
                break;
            case 3:
                strcat(msg, "(Admin) ");
                strcpy(value, "Admin(account type)\t");
                break;
            default:
                strcat(msg, "(Not Possible) ");
                strcpy(value, "NA(account type)\t");
                break;
            }
            strcat(msg, "details are:");
            send_message(desc, "R", buff);
            send_message(desc, msg, buff);
            strcpy(msg, temp.username);
            strcat(msg, "\t");
            strcat(msg, temp.password);
            strcat(msg, "(password)\t");
            strcat(msg, value);
            strcat(msg, temp.accountStatus);
            strcat(msg, "\n");
            send_message(desc, "R", buff);
            send_message(desc, msg, buff);            
            adminOperations(desc, 2);
        }else{
            char msg[1024] = "**Account (";
            strcat(msg, user.username);
            strcat(msg, ") not found in the accounts list. Kindly try again.");
            send_message(desc, "R", buff);
            send_message(desc, msg, buff);
            admin_user_op(desc, --try_count);
        }

    }else{
        return;
    }
    return;
}

void adminOperations(int desc, int try_count){
    char input[1024];
    if(try_count<=0){
        char message[1024] = "\n**Maximum invalid input limit reached. Shutting down.";
        send_message(desc, "R", input);
        send_message(desc, message, input);
        return;
    }
    char message[1024] = "Press 1 for Operation on Trains\nPress 2 for Operations on User Accounts\nPress any other key to Exit\nEnter your choice: ";
    send_message(desc, "RW", input);
    send_message(desc, message, input);
    if(strcmp("1", input)==0){
        admin_train_op(desc, 2);
    }
    else if(strcmp("2", input) == 0){
        admin_user_op(desc, 2);
    }
    else{
        return;
    }
}


void logout(int desc, struct AccountDetails user){
    struct AccountDetails tempAccount;
    int index = 0;
    char input[1024];
    struct flock lock;
    int fd = open("loginInfo.dat", O_RDWR);
    writeLock(lock, fd);
    while(read(fd, &tempAccount, sizeof(tempAccount)) > 0){
        if(strcmp(tempAccount.username, user.username)==0){
            user.sessionFlag = 0;
            lseek(fd, index*sizeof(user), SEEK_SET);
            write(fd, &user, sizeof(user));
            break;            
        }
        ++index;
    }
    writeUnlock(lock, fd);
    close(fd);
    send_message(desc, "CLOSE", input);
}