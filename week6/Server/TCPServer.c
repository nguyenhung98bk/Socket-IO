#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include "dslkd.h"

#define BUFLEN 512

void die(char *s);
void function_bind(int server_fd,struct sockaddr_in address);
void scanlist(FILE *fin);
void updatefile(node *p);
elementtype findInforAccount(node *p,char username[]);
void logout(int connfd,struct sockaddr_in address);
void checkPassword(int connfd,struct sockaddr_in address,node *p,elementtype tmp);
int PlusTimeFalse(node *p,elementtype tmp);
void ResetTimeFalse(node *p,elementtype tmp);
void changeStatus(node *p,elementtype tmp);

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
void scanlist(FILE *fin){
	elementtype tmp;
	while(!feof(fin))
    {
        fscanf(fin,"%s %s %d",tmp.username,tmp.password,&tmp.status);     
        insert_first(tmp);												
    }
}
void scanTimeFalse(node *p,FILE *fin){
    elementtype tmp;
    while (!feof(fin))
    {
        fscanf(fin,"%s %d",tmp.username,&tmp.TimeFalse);
        p->element.TimeFalse = tmp.TimeFalse;
        p = p->next;
    }
}
void updatelist1(node *p){
    FILE *f;
    f = fopen("account.txt","r");
    elementtype tmp;
    while(!feof(f)){
        fscanf(f,"%s %s %d",tmp.username,tmp.password,&tmp.status);
        if(p->element.status!=tmp.status){
            p->element.status=tmp.status;
        }
        p=p->next;
    }
    fclose(f);
}
void updatelist2(node *p){
    FILE *f;
    f = fopen("TimeFalse.txt","r");
    elementtype tmp;
    while(!feof(f)){
        fscanf(f,"%s %d",tmp.username,&tmp.TimeFalse);
        if(p->element.TimeFalse!=tmp.TimeFalse){
            p->element.TimeFalse=tmp.TimeFalse;
        }
        p=p->next;
    }
    fclose(f);
}
void updatefile1(node *p){
	FILE * fp;
   	fp = fopen ("account.txt", "w");
	while(p->next!=NULL){
		elementtype tmp=p->element;
		fprintf(fp, "%s %s %d\n",tmp.username,tmp.password,tmp.status );    
		p=p->next;
	}
	elementtype tmp=p->element;
	fprintf(fp, "%s %s %d",tmp.username,tmp.password,tmp.status );			
	fclose(fp);														
}
void updatefile2(node *p){
	FILE * fp;
   	fp = fopen ("TimeFalse.txt", "w");
	while(p->next!=NULL){
		elementtype tmp=p->element;
		fprintf(fp, "%s %d\n",tmp.username,tmp.TimeFalse );    
		p=p->next;
	}
	elementtype tmp=p->element;
	fprintf(fp, "%s %d",tmp.username,tmp.TimeFalse );			
	fclose(fp);														
}
elementtype findInforAccount(node *p,char username[]){
    elementtype tmp;
	while(p!=NULL){
    	tmp=p->element;
        if(strcmp(tmp.username,username)==0){
            return tmp;
        }
  		p=p->next;
  	}
    tmp.status = -1;
    return tmp;
}
void sendMessage(int connfd,char status[],struct sockaddr_in address){
    int slen = sizeof(address);
    sendto(connfd , status , 20 , 0 ,(struct sockaddr *)&address,slen);
}
void checkAccount(int connfd,struct sockaddr_in address,node *p)
{     
    char username[BUFLEN];
    elementtype tmp;
    int recv_len;
    int slen = sizeof(address);
    while(1){
        recv_len = recvfrom(connfd, username, BUFLEN, 0,(struct sockaddr *)&address,&slen);
        username[recv_len] = '\0';
        updatelist1(p);
        tmp = findInforAccount(root,username);
        if(tmp.status == -1){
            sendMessage(connfd,"-1",address);
        }
        else if(tmp.status == 0){
            sendMessage(connfd,"0",address);
        }
        else{
            sendMessage(connfd,"1",address);
            checkPassword(connfd,address,p,tmp);
        }
    }
}
void checkPassword(int connfd,struct sockaddr_in address,node *p,elementtype tmp){
    int recv_len;
    char password[BUFLEN];
    int slen = sizeof(address);
    int a;
    while(1){
        recv_len = recvfrom(connfd, password, BUFLEN, 0,(struct sockaddr *)&address,&slen);
        password[recv_len]='\0';
        updatelist2(p);
        if(strcmp(password,tmp.password)==0){
            sendMessage(connfd,"1",address);
            logout(connfd,address);
            ResetTimeFalse(root,tmp);
            updatefile2(p);
            break;
        }
        else{
            a = PlusTimeFalse(p,tmp);
            updatefile2(p);
            if(a>=3){
                changeStatus(p,tmp);
                updatefile1(p);
                sendMessage(connfd,"-1",address);
                break;
            }  
            sendMessage(connfd,"0",address);
        }
    }
}
void changeStatus(node *p,elementtype tmp){
    while(p!=NULL){
        if(strcmp(p->element.username,tmp.username)==0){
            p->element.status = 0;
            break;
        }
        p = p->next;
    }
}
void ResetTimeFalse(node *p,elementtype tmp){
    while(p!=NULL){
        if(strcmp(p->element.username,tmp.username)==0){
            p->element.TimeFalse = 0;
            break;
        }
        p = p->next;
    }
}
int PlusTimeFalse(node *p,elementtype tmp){
    while(p!=NULL){
        if(strcmp(p->element.username,tmp.username)==0){
            p->element.TimeFalse++;
            break;
        }
        p = p->next;
    }
    return (p->element.TimeFalse);
}
void logout(int connfd,struct sockaddr_in address){
    char selection[30];
    int slen = sizeof(address);
    recvfrom(connfd, selection, 30, 0,(struct sockaddr *)&address,&slen);
    printf("Client was logout!\n");
}
int main(int argc, char const *argv[]) 
{ 
    if(argc!=2){
		printf("Nhap sai\n");
	}
	else{
        FILE *f1;
        f1 = fopen("account.txt","r");
        scanlist(f1);
        fclose(f1);
        root=reverse(root);
        updatefile2(root);
        int server_fd, connfd,recv_len,PORT; 
        struct sockaddr_in address; 
        int opt = 1; 
        int addrlen = sizeof(address);
        pid_t pid;
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
        while(1){
            fork();
            fork();
            fork();
            if ((connfd = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) { 
                perror("accept"); 
                exit(EXIT_FAILURE); 
            }
            checkAccount(connfd,address,root);
            close(connfd);
        }
    }
    return 0; 
} 