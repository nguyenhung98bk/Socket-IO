#include<stdio.h>   //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include<ctype.h>
#define BUFLEN 512  //Max length of buffer

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(int argc, char *argv[])
{
    if(argc!=3){
        printf("Nhap sai\n");
    }
    else{
    struct sockaddr_in address_other;
    int s, s2, i, slen=sizeof(address_other);
    char buf[BUFLEN];
    char SERVER[20];
    char *message;
    char message2[BUFLEN]="connected";
    int PORT = atoi(argv[2]);
    strcpy(SERVER,argv[1]);
    if((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
        die("socket");
    }
    memset((char *) &address_other, 0, sizeof(address_other));
    address_other.sin_family = AF_INET;
    address_other.sin_port = htons(PORT);
    if (inet_aton(SERVER , &address_other.sin_addr) == 0){
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    while(1)
    {   
        printf("Enter message : ");
        __fpurge(stdin);
        gets(message2);
        message=&message2;
        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &address_other, slen)==-1)
        {
            die("sendto()");
        }
        if(strlen(message2)==0){
            printf("Quit!\n");
            exit(0);
        }
    }
    close(s);
    }
    return 0;
}