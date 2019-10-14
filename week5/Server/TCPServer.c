#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <ctype.h>
#define BUFLEN 512
void die(char *s)
{
    perror(s);
    exit(1);
}
void function_bind(int server_fd,struct sockaddr_in address){
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) { 
        die("bind");
    }
}
void printf_message(char buffer[]){
    char messageSo[BUFLEN];
    char messageChu[BUFLEN];
    int j=0,k=0,i=0,check = 0;
    while(buffer[i]!='\0'){
        i++;
    }
    int n=i;
    for(int i=0;i<n;i++){
        if(48<=buffer[i]&&buffer[i]<=57){
            messageSo[j++]= buffer[i];
        }
        else if((65<=buffer[i]&&buffer[i]<=90)||(97<=buffer[i]&&buffer[i]<=122)){
            messageChu[k++]= buffer[i];
        }
        else{
            printf("Error\n");
            check = 1;
        }
    }
    messageChu[k]='\0';
    messageSo[j]='\0';
    if(check ==0 ){
        printf("%s\n", messageChu);
        printf("%s\n", messageSo);
    }
}
int main(int argc, char const *argv[]) 
{ 
    if(argc!=2){
		printf("Nhap sai\n");
	}
	else{
        int server_fd, new_socket,recv_len; 
        struct sockaddr_in address; 
        int opt = 1; 
        int addrlen = sizeof(address); 
        char mess_from_client[225];
        char buffer[BUFLEN] = {0}; 
        int PORT;
        PORT = atoi(argv[1]);
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
        { 
            perror("socket failed"); 
            exit(EXIT_FAILURE); 
        }  
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
        { 
            perror("setsockopt"); 
            exit(EXIT_FAILURE); 
        } 
        address.sin_family = AF_INET; 
        address.sin_addr.s_addr = INADDR_ANY; 
        address.sin_port = htons( PORT );
        function_bind(server_fd,address);
        if (listen(server_fd, 3) < 0) 
        { 
            perror("listen"); 
            exit(EXIT_FAILURE); 
        } 
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) 
        { 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        } 
        while(1){
            char str_cli_ip[INET_ADDRSTRLEN];
            struct sockaddr_in* ip_client = (struct sockaddr_in*)&address;
            if ((recv_len = recv(new_socket, buffer, BUFLEN, 0)) == -1){
                die("recv()");
            }
            buffer[recv_len]='\0';
            if(strcmp(buffer,"90990800022")==0){
                if ((recv_len = recv(new_socket, buffer, BUFLEN, 0)) == -1){
                die("recv()");
                }
                buffer[recv_len]='\0';
                printf("Noi dung file da gui: %s\n",buffer);
            }
            else{    
                if ((recv_len = recv(new_socket, buffer, BUFLEN, 0)) == -1){
                    die("recv()");
                }
                buffer[recv_len]='\0';
                if(recv_len==0){
                    printf("Client stopped\n");
                    exit(0);
                }
                printf_message(buffer);
	       }
       }
       close(new_socket);
    }
    return 0; 
} 