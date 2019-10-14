#include<stdio.h> 
#include<string.h>
#include<stdlib.h> 
#include<arpa/inet.h>
#include<sys/socket.h>
#include<ctype.h>
#include <unistd.h> 
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
int main(int argc,char *argv[]){
    if(argc!=2){
        printf("Nhap sai\n");
    }
    else{
        struct sockaddr_in address_me, address_other;
        int new_socket,s, i, slen = sizeof(address_other), recv_len;
        char buf[BUFLEN];
        int PORT;
        PORT = atoi(argv[1]);
        if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        {
            die("socket");
        }
        memset((char *) &address_me, 0, sizeof(address_me));
        address_me.sin_family = AF_INET;
        address_me.sin_port = htons(PORT);
        address_me.sin_addr.s_addr = htonl(INADDR_ANY);
        function_bind(s,address_me);
        while(1){
            printf("Waiting for data...\n");
            if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &address_other, &slen)) == -1){
                die("recvfrom()");
            }
            buf[recv_len]='\0';
            if(recv_len==0){
                printf("Client stopped\n");
                exit(0);
            }
            printf_message(buf);
        }
        close(s);
    }
    return 0;
}