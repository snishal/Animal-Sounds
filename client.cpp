#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <iostream>

#define PORT 8888
#define buffer_size 1024

int main(int argc, char const *argv[]) 
{ 
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	// char *hello = "Hello from client"; 
	char read_buffer[buffer_size] = {0}, write_buffer[buffer_size];

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	}
    
    while(1){
        // send(sock , hello , strlen(hello) , 0 ); 
        // printf("Hello message sent\n"); 
        // valread = read(sock , buffer, buffer_size); 
        // printf("%s\n", buffer);
        bzero(write_buffer, sizeof(buffer_size));
        std::cin >> write_buffer;

        if(strcasecmp("SOUND", write_buffer) == 0){
            send(sock , write_buffer , strlen(write_buffer) , 0); 
            printf("Waiting for Reply...\n"); 
            valread = read(sock , read_buffer, buffer_size); 
            printf("%s\n", read_buffer);

            printf("Animal Name: ");
            bzero(write_buffer, sizeof(buffer_size));
            std::cin >> write_buffer;
            
            send(sock , write_buffer , strlen(write_buffer) , 0); 
            printf("Waiting for Reply...\n"); 
            valread = read(sock , read_buffer, buffer_size); 
            printf("%s\n", read_buffer);
        }else if(strcasecmp("STORE", write_buffer) == 0){
            send(sock , write_buffer , strlen(write_buffer) , 0); 
            printf("Waiting for Reply...\n"); 
            valread = read(sock , read_buffer, buffer_size); 
            printf("%s\n", read_buffer);

            printf("Animal Name: ");
            bzero(write_buffer, sizeof(buffer_size));
            std::cin >> write_buffer;
            
            send(sock , write_buffer , strlen(write_buffer) , 0); 
            printf("Waiting for Reply...\n"); 
            valread = read(sock , read_buffer, buffer_size); 
            printf("%s\n", read_buffer);

            printf("Animal Sound: ");
            bzero(write_buffer, sizeof(buffer_size));
            std::cin >> write_buffer;
            
            send(sock , write_buffer , strlen(write_buffer) , 0);
            printf("Waiting for Reply...\n"); 
            valread = read(sock , read_buffer, buffer_size); 
            printf("%s\n", read_buffer);
        }else if(strcasecmp("QUERY", write_buffer) == 0){
            send(sock , write_buffer , strlen(write_buffer) , 0); 
            printf("Waiting for Reply...\n"); 
            valread = read(sock , read_buffer, buffer_size); 
            printf("%s\n", read_buffer);
        }else if(strcasecmp("BYE", write_buffer) == 0){
            // End Session
        }else if(strcasecmp("END", write_buffer) == 0){
            // Terminate Connection
        }else{
            //UNKOWN REQUEST
        }
    }
	return 0; 
} 
