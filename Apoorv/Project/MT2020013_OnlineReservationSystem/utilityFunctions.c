#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


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