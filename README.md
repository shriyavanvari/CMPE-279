# CMPE-279 Assignemnts

## Doing the assignments individually 

## Assignment 1

### Requirement:

In assignment 1, you will extend the server code to use privilege separation. You should accomplish this by splitting up the code into two logical parts – the part that sets up the socket and a separate part that processes the data from the client. Once you locate this split, you should fork and have the child process setuid() to drop its privileges to an unprivileged user (maybe the “nobody” user, or a user you create). Do not setuid to a random or made-up UID! The server should wait for the child to exit and the child should process the connection from the client.

### Instructions to run:
Run client code on port 80

gcc -o server server.c && sudo ./server  
gcc -o client client.c && ./client

## Assignment 2

### Requirement:

Starting with assignment 1 as a starting point, add in code to re-exec the server’s child process after forking. You will need to determine how to pass the socket file descriptor between the forked child and the new exec’ed child, so that the new exec’ed child can process the incoming child request.

### Instructions to run:
Run client code on port 80

gcc -o server server.c && sudo ./server  
gcc -o client client.c && ./client
