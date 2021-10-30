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
#include "setup.c"
#include "utilityFunctions.c"

#define PORT 55555

void clientOperations(int desc, int try_count, struct AccountDetails user);
void adminOperations(int desc, int try_count);
void initial_setup();


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

void client_connection(int desc){
    char input[1024];
    read(desc, input, sizeof(input));
    get_credentials(desc);
}

int main(int argc, char const *argv[]){
    struct sockaddr_in server, client;
    int socket_desc, size_client, client_desc;
    initial_setup();
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    // server.sin_addr.s_addr = inet_addr("127.0.0.1");

    server.sin_port = htons(PORT);
    if(bind(socket_desc, (void *)(&server), sizeof(server)) < 0) {
	    perror("Error on binding:");
	    exit(EXIT_FAILURE);
	}
    listen(socket_desc, 5);
    while(1){
        size_client = sizeof(client);
	    if((client_desc = accept(socket_desc, (struct sockaddr*)&client, &size_client)) < 0) {
	        printf("Error on accept.\n");
	        exit(EXIT_FAILURE);
	    }
        if(fork() == 0){
            client_connection(client_desc);
            close(client_desc);
            exit(EXIT_SUCCESS);
        }else{
            close(client_desc);
        }
    }
    return 0;
}