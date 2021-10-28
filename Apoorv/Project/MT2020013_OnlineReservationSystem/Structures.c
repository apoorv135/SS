struct AccountDetails
{
    char username[1024];
    char password[1024];
    int accountType;
    char accountStatus[1024];
    int sessionFlag;
};

struct BookingDetails{
    char trainNo[1024];
    char accountNo[1024];
    char bookingStatus[1024];
    int  NumOfSeats;
    int bookingNumber;
};

struct TrainDetails
{
    char number[1024];
    char name[1024];
    int totalSeats;
    int bookedSeats;
    char trainStatus[1024];
};