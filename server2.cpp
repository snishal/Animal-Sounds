#include <stdio.h> 
#include <string.h> //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> //close 
#include <arpa/inet.h> //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 

#include <bits/stdc++.h>

#include "LRU.h"
#include "BST.h"

#define TRUE 1 
#define FALSE 0 
#define PORT 8888
#define buffer_size 1024
	
int main(int argc , char *argv[]) 
{ 

	std::unordered_map<std::string, std::string> users;
	users["user1"] = "pass1";
	users["user2"] = "pass2";
	users["user3"] = "pass3";

	bool client_logged_in[30];

	BST tree;

	int action = -1;

	int opt = TRUE; 
	int master_socket , addrlen , new_socket , client_socket[30], 
		max_clients = 30 , activity, i , valread , sd; 
	int max_sd; 
	struct sockaddr_in address; 
		
	char read_buffer[buffer_size], write_buffer[buffer_size]; //data buffer of 1K 
		
	//set of socket descriptors 
	fd_set readfds; 
		
	//initialise all client_socket[] to 0 so not checked 
	// and logged_in false
	for (i = 0; i < max_clients; i++) 
	{ 
		client_socket[i] = 0; 
		client_logged_in[i] = false;
	} 
		
	//create a master socket 
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	//set master socket to allow multiple connections , 
	//this is just a good habit, it will work without this 
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
		sizeof(opt)) < 0 ) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	
	//type of socket created 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
		
	//bind the socket to localhost port 8888 
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	printf("Listener on port %d \n", PORT); 
		
	//try to specify maximum of 3 pending connections for the master socket 
	if (listen(master_socket, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
		
	//accept the incoming connection 
	addrlen = sizeof(address); 
	puts("Waiting for connections ..."); 
		
	while(TRUE) 
	{ 
		bzero(write_buffer, buffer_size);
		bzero(read_buffer, buffer_size);
		
		//clear the socket set 
		FD_ZERO(&readfds); 
	
		//add master socket to set 
		FD_SET(master_socket, &readfds); 
		max_sd = master_socket; 
			
		//add child sockets to set 
		for ( i = 0 ; i < max_clients ; i++) 
		{ 
			//socket descriptor 
			sd = client_socket[i]; 
				
			//if valid socket descriptor then add to read list 
			if(sd > 0) 
				FD_SET( sd , &readfds); 
				
			//highest file descriptor number, need it for the select function 
			if(sd > max_sd) 
				max_sd = sd; 
		} 
	
		//wait for an activity on one of the sockets , timeout is NULL , 
		//so wait indefinitely 
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL); 
	
		if ((activity < 0) && (errno!=EINTR)) 
		{ 
			printf("select error"); 
		} 
			
		//If something happened on the master socket , 
		//then its an incoming connection 
		if (FD_ISSET(master_socket, &readfds)) 
		{ 
			if ((new_socket = accept(master_socket, 
					(struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
			{ 
				perror("accept"); 
				exit(EXIT_FAILURE); 
			} 
			
			//inform user of socket number - used in send and receive commands 
			printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs 
				(address.sin_port)); 
		
			//send new connection greeting message
			strcpy(write_buffer, "Welcome to Sound Server \r\n");
			if( send(new_socket, write_buffer, strlen(write_buffer), 0) != strlen(write_buffer) ) 
			{ 
				perror("send"); 
			} 
				
			puts("Welcome message sent successfully"); 
				
			//add new socket to array of sockets 
			for (i = 0; i < max_clients; i++) 
			{ 
				//if position is empty 
				if( client_socket[i] == 0 ) 
				{ 
					client_socket[i] = new_socket; 
					printf("Adding to list of sockets as %d\n" , i); 
						
					break; 
				} 
			} 
		}

		//else its some IO operation on some other socket 
		for (i = 0; i < max_clients; i++) 
		{
			sd = client_socket[i]; 
				
			if (FD_ISSET( sd , &readfds)) 
			{ 
				//Check if it was for closing , and also read the 
				//incoming message 
				if ((valread = read( sd , read_buffer, buffer_size)) == 0) 
				{ 
					//Somebody disconnected , get his details and print 
					getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen); 
					printf("Host disconnected , ip %s , port %d \n" , 
						inet_ntoa(address.sin_addr) , ntohs(address.sin_port)); 
						
					//Close the socket and mark as 0 in list for reuse 
					close( sd ); 
					client_socket[i] = 0; 
					client_logged_in[i] = false;
				}else if(!client_logged_in[i]){
					std::string user(strtok(read_buffer, "\n"));
					std::string pass(strtok(NULL, "\n"));

					if(users.count(user) > 0){
						if(users[user] == pass){
							strcpy(write_buffer, "Logged In \r\n");
							write(sd , write_buffer , strlen(write_buffer)); 
							client_logged_in[i] = true;
						}else{
							strcpy(write_buffer, "Invalid Password \r\n");
							write(sd , write_buffer , strlen(write_buffer)); 
						}
					}else{
						users[user] = pass;
						strcpy(write_buffer, "Successfully Registered \r\n");
						write(sd , write_buffer , strlen(write_buffer)); 
					}
				} 
				else if(strcasecmp("SOUND", read_buffer) == 0){
					strcpy(write_buffer, "SOUND : OK \r\n");
					write(sd , write_buffer , strlen(write_buffer)); 
					action = 1;
				}else if(strcasecmp("STORE", strtok(read_buffer, "\n")) == 0){
					std::string animal_name(strtok(NULL, "\n"));
					std::string animal_sound(strtok(NULL, "\n"));

					std::transform(animal_name.begin(), animal_name.end(), animal_name.begin(), ::toupper);
					std::transform(animal_sound.begin(), animal_sound.end(), animal_sound.begin(), ::toupper);

					tree.insert(animal_name, animal_sound);

					strcpy(write_buffer, "STORE : OK \r\n");
					write(sd , write_buffer , strlen(write_buffer)); 
				}else if(strcasecmp("QUERY", read_buffer) == 0){
					for(const auto& animal : tree.inorder()){
						strcpy(write_buffer, animal.c_str());
						strcat(write_buffer, "\n");
						write(sd, write_buffer, strlen(write_buffer));
					}
					strcpy(write_buffer, "QUERY : OK \r\n");
					write(sd , write_buffer , strlen(write_buffer));
				}else if(strcasecmp("BYE", read_buffer) == 0){
					strcpy(write_buffer, "BYE : OK \r\n");
					write(sd , write_buffer , strlen(write_buffer));

					printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port)); 
					close(sd); 
					client_socket[i] = 0;
					client_logged_in[i] = false;
				}else if(strcasecmp("END", read_buffer) == 0){
					for(i = 0; i < max_clients; i++){
						strcpy(write_buffer, "END : OK \r\n");
						write(sd , write_buffer , strlen(write_buffer));

						printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port)); 
						close(sd); 
						client_socket[i] = 0;
						client_logged_in[i] = false;
					}
					exit(0);
				}
				else
				{ 
					if(action == 1){
						std::string animal(strtok(read_buffer, "\n"));
						std::transform(animal.begin(), animal.end(), animal.begin(), ::toupper);

						strcpy(write_buffer, tree.search(animal).c_str());
						write(sd , write_buffer , strlen(write_buffer));
					}else{
						strcpy(write_buffer, "Invalid Sequence");
						write(sd , write_buffer , strlen(write_buffer));
					}
				} 
			} 
		} 
	} 
		
	return 0; 
} 
