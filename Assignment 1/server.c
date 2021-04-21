// Server side C/C++ program to demonstrate Socket programming

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>

#define PORT 80

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[102] = {0};
    char *hello = "Hello from server";
    int setuid_return_val;
    pid_t current;
    struct passwd* nobodystruct;

    printf("execve=0x%p\n", execve);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) //Domain=ipv4,type=tcp,protocol value for IP is 0
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attaching socket to port 80
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //ip address
    address.sin_port = htons( PORT ); 

    // Forcefully attaching socket to the port 80
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }


    //split between creating the socket and processing the data 
    
    current=fork(); // fork will return pid of child to parent and status to child (-1-unsuccesful/0-successfull)
    
    if(current==0) //child process is exceuting since child has status
    {
        printf("Child process is running\n");
        printf("Current child id is: %d \n", getuid());
        //drop privilege
        
        if((nobodystruct=getpwnam("nobody"))==NULL){
            printf("Cannot find id of nobody user\n");
        } // trying to get the id of nobody user for current OS
        
        setuid_return_val=setuid(nobodystruct->pw_uid); //Setting the process id of child to nobody's id so that they dont have any sudo privileges
        printf("Child id after privilege drop is: %d \n", getuid());
        
        if (setuid_return_val==0)
        {
            //Privilege drop successful 
            printf("Child privilege dropped\n");
            printf("Child id is: %d \n", getuid());
            valread = read( new_socket , buffer, 1024);
            printf("%s\n",buffer );
            send(new_socket , hello , strlen(hello) , 0 );
            printf("Hello message sent\n");
        }

        else{
            //privilege drop not succesful and we exit  to avoid any communication with client
            printf("Privilege drop failed. Sudo privileges required. Exiting\n");
            return 0;
        }
        

    
    }
    else if(current>0){
        //in parent process fork return child's pid which is greater than 0
        wait(NULL);
        printf("Parent is running\n");
    }

    else{
        printf("Error while forking\n");
    }

    

    return 0;
}
