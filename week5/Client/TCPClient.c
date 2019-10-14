#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
   
int main(int argc, char const *argv[]) 
{ 
  if(argc!=3){
    printf("Nhap sai\n");
  }
  else{
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *message;
    char addr[225];
    int next = 1;
    strcpy(addr,argv[1]);
    int PORT = atoi(argv[2]);
    FILE *fp;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
      printf("\n Loi socket \n"); 
      return -1; 
    } 
    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
    if(inet_pton(AF_INET, addr, &serv_addr.sin_addr) <= 0)  
    { 
        printf("\nDia chi khong hop le \n"); 
        return -1; 
    } 
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    }
    while(1){
      while(1){
        int luachon;
        do{
          printf("Menu:\n1. Gui xau bat ky.\n2. Gui noi dung mot file.\n3. Quit\nNhap lua chon cua ban:");
          scanf("%d",&luachon);
        }while(luachon<1||luachon>3);
        if(luachon==1){
          char mess_from_client[225]="90990800021";
          message=&mess_from_client;
          send(sock , message , strlen(message) , 0 );
          printf("Nhap noi dung tin nhan gui den server\n");
          __fpurge(stdin);
          gets(mess_from_client);
          fflush(stdin);
          message = &mess_from_client;
          send(sock , message , strlen(message) , 0 ); 
          if (strcmp(mess_from_client, "") == 0){
            printf("Quit\n");
            exit(1);
          }
        }
        if(luachon==2){
          char sendfile[50]="90990800022";
          message=&sendfile;
          send(sock , message , strlen(message) , 0 );
          char filename[30];
          printf("Enter the file name:");
          __fpurge(stdin);
          gets(filename);
          fp = fopen(filename,"r");
          if(fp==NULL){
            printf("File khong ton tai!\n");
          }
          else{
            int i=0;
            char c;
            do{
              c = fgetc(fp);
              if(feof(fp)){
                break ;
              }
              message[i]=c;
              i++;
            }while(1);
            send(sock , message , strlen(message) , 0 );
          }
        }
        if(luachon==3){
          printf("Quit!\n");
          exit(1);
        }
      }
    }
    close(sock);  
    }
  return 0; 
} 