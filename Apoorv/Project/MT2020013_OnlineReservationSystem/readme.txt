How to Run:

1. Enter following command : 
$ ./compile.sh 
This will compile all the files for you. 

2. To run the server : 
$ ./server 
This will run the server and it will keep running until its killed manually by linux admin. 

3. To run the client : 
#./ client 
This is a dummy client and it is just used to send and retrieve the messages back and forth with server. 
All the operations are done on server. 

*****Additional Details******
ADMIN Credentials for login first time : 
Username : admin
Password : admin 

There are two primary operations to be done by admin for first time the program is run. 
Which is adding a train and adding a user or agent so that tickets can be booked. 

In case there is some port issue, modify the port number defined on top of client.c and server.c to some other port
and recompile. 

In case you want to reset the program and recompile, you can simply run below command : 
$./reset.sh 
This will delete existing files if any and will leave the folder with default code files. 