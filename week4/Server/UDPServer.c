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
int main(int argc,char *argv[]){
    if(argc!=2){
        printf("Nhap sai\n");
    }
    else{
        struct sockaddr_in address_me, address_other, address_other2,address_other3;
        int new_socket,s, i, slen = sizeof(address_other),slen2 = sizeof(address_other2),slen3 = sizeof(address_other3) , recv_len;
        char buf[BUFLEN],buf2[BUFLEN];
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
        printf("Please wait all client connect.....\n");
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &address_other, &slen)) == -1){
            die("recvfrom()");
        }
        buf[recv_len]='\0';
        printf("Client 1 : %s\n", buf);
        if ((recv_len = recvfrom(s, buf2, BUFLEN, 0, (struct sockaddr *) &address_other2, &slen2)) == -1){
            die("recvfrom()");
        }
        buf2[recv_len]='\0';
        printf("Client 2 : %s\n", buf2);
        strcpy(buf,"All client has connected");
        if (sendto(s, buf, strlen(buf), 0, (struct sockaddr*) &address_other, slen) == -1)
        {
            die("sendto()");
        }
        if (sendto(s, buf, strlen(buf), 0, (struct sockaddr*) &address_other2, slen2) == -1)
        {
            die("sendto()");
        }
        while(1){
            strcpy(buf,"");
            strcpy(buf2,"");
            printf("Waiting for data...\n");
            if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &address_other3, &slen)) == -1){
                die("recvfrom()");
            }
            printf("%d\n",ntohs(address_other3.sin_port));
            buf[recv_len]='\0';
            if(ntohs(address_other.sin_port)!=ntohs(address_other3.sin_port)){
                if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &address_other, &slen) == -1)
                {
                    die("sendto()");
                }
            }
            else{
                if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &address_other2, slen2) == -1)
                {
                    die("sendto()");
                }
            }
            if(recv_len==0){
                printf("Client stopped\n");
                exit(0);
            }
        }
        close(s);
    }
    return 0;
}