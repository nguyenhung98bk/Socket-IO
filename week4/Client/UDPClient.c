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
    message=&message2;
    if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &address_other, slen)==-1){
        die("sendto()");
    }
    printf("Please wait all client connect.....\n");
    memset(buf,'\0', BUFLEN);
    if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &address_other, &slen) == -1){
        die("recvfrom()");
    }
    printf("%s\n",buf );
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
        memset(buf,'\0', BUFLEN);
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &address_other, &slen) == -1){
            die("recvfrom()");
        }
        int i=0;
        while(buf[i]!='\0'){
            i++;
        }
        if(i==0){
            printf("The opponent has stopped the conversation!\n");
            exit(0);
        }
        printf_message(buf);
    }
    close(s);
    }
    return 0;
}