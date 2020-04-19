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
    bool down = false, loggedIn = false;
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	// char *hello = "Hello from client"; 
	char read_buffer[buffer_size], write_buffer[buffer_size];

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
        bzero(write_buffer, buffer_size);
		bzero(read_buffer, buffer_size);

        valread = read(sock , read_buffer, buffer_size); 
        std::cout << read_buffer << std::endl;
        
        if(strcasecmp("Logged In \r\n", read_buffer) == 0){
            loggedIn = true;
        }

        if(down){
            exit(0);
        }

        if(!loggedIn){
            char user[1024], pass[1024];
            std::cout << "Username: ";
            std::cin >> user;
            std::cout << "Password: ";
            std::cin >> pass;

            strcat(write_buffer, user);
            strcat(write_buffer, "\n");
            strcat(write_buffer, pass);

            write(sock, write_buffer, strlen(write_buffer));
        }else{
            std::cin >> write_buffer;

            if(strcasecmp("SOUND", write_buffer) == 0){
                write(sock , write_buffer , strlen(write_buffer)); 
            }else if(strcasecmp("STORE", write_buffer) == 0){
                char animal_name[1024], animal_sound[1024];
                std::cin >> animal_name;
                std::cin >> animal_sound;

                strcat(write_buffer, "\n");
                strcat(write_buffer, animal_name);
                strcat(write_buffer, "\n");
                strcat(write_buffer, animal_sound);

                std::cout << write_buffer;

                write(sock , write_buffer , strlen(write_buffer)); 
            }else if(strcasecmp("QUERY", write_buffer) == 0){
                write(sock , write_buffer , strlen(write_buffer));
            }else if(strcasecmp("BYE", write_buffer) == 0){
                write(sock, write_buffer, strlen(write_buffer));
                shutdown(sock, SHUT_WR);
                down = true;
            }else if(strcasecmp("END", write_buffer) == 0){
                write(sock, write_buffer, strlen(write_buffer));
                shutdown(sock, SHUT_WR);
                down = true;
            }else{
                write(sock , write_buffer , strlen(write_buffer)); 
            }
        }
    }
	return 0; 
} 
