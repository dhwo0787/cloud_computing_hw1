// C lib
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// Unix lib
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>

#define Buf_size 1024
#define Max_Client 100

void* signal_handler(void* arg);
void send_data(FILE* fp,char* file_name, int* client_socket);
void send_error(FILE* fp, int* client_socket);

int client_cnt = 0;

int main() {
    
    char buf[Buf_size] = {0,}; 

    // TCP 소켓 생성
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    // 서버 주소 / PORT(5050) 설정
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5050);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr *)&server_address,sizeof(server_address));
    listen(server_socket,50);
    
    int client_socket[Max_Client];
    struct sockaddr_in client_address;
    pthread_t t_id[Max_Client];

    while(1) {

        // 클라이언트 소켓 처리
        int client_address_size = sizeof(client_address);
	    client_socket[client_cnt] = accept(server_socket,(struct sockaddr*)&client_address,&client_address_size);
	    printf("Connection Request : %s in Port Num.%d\r\n",inet_ntoa(client_address.sin_addr),ntohs(client_address.sin_port));

        // 클라이언트 임계값 설정
        if(client_cnt == Max_Client) {
            printf("client accept full(max client count : %d)\n", Max_Client);
            close(client_socket[client_cnt]);
            continue;
        }

        // Multi Thread로 클라이언트 요청 처리
        if(pthread_create(&t_id[client_cnt],NULL,signal_handler,&client_socket[client_cnt])) {
            printf("Thread create error\n");
            close(client_socket[client_cnt]);
            continue;
        }
        pthread_detach(t_id[client_cnt]);
        
        // 클라이언트 수 조정
        client_cnt++;
    }

    close(server_socket);
    return 0;
}

void* signal_handler(void* arg) {
	
    printf("Successfully made Thread\r\n");
    int client_socket = *((int*)arg);
    char req_buf[200] = {0,};  // 요청 string
    char file_name[50] = {0,}; // 필요한 파일 이름
    char method[20] = {0,};    // 요청 방식
	
    FILE* client_read = fdopen(client_socket,"r");
    FILE* client_write = fdopen(dup(client_socket),"w");
    fgets(req_buf,200,client_read);

    printf("request : %s \r\n",req_buf);
    if(strstr(req_buf,"HTTP/") == NULL) {
        send_error(client_write,&client_socket);
        fclose(client_read);
        fclose(client_write);
        return NULL;
    }

    strcpy(method,strtok(req_buf," /"));
    if(strncmp(method,"GET",3)==0) {
        printf("complete GET command\r\n");
        strcpy(file_name,strtok(NULL," /"));
        fclose(client_read);
        send_data(client_write,file_name,&client_socket);
    }
    else if (strncmp(method,"POST",4)==0){
        printf("complete POST command\r\n");
        strcpy(file_name,strtok(NULL," /"));
        fclose(client_read);
        send_data(client_write,file_name,&client_socket);
    }
    else {
        send_error(client_write,&client_socket);
        fclose(client_read);
        fclose(client_write);
        return NULL;
    }

}
void send_data(FILE* fp,char* file_name,int* client_socket) {
    char status[] = "HTTP/1.1 200 Ok\r\n";
    char server[] = "Server: Linux server\r\n\r\n";

    char buf[Buf_size] = {0,};
    FILE* send_file;

    send_file = fopen(file_name,"r");
    if(send_file == NULL) {
        send_error(fp,client_socket);
        fclose(fp);
        return;
    }
    fputs(status,fp);
    fputs(server,fp);

    while(fgets(buf,Buf_size,send_file) != NULL) {
        fputs(buf,fp);
        fflush(fp);
    }
    send(*client_socket,buf,Buf_size,0);
    printf("succesfully sending file\n");
    fflush(fp);
    fclose(fp);
}
void send_error(FILE* fp,int* client_socket) {
    char status[] = "HTTP/3.0 400 Bad Request\r\n";
    char server[] = "Server: Linux server\r\n\r\n";
    char content[] = "<html><head></head><body><a href='localhost:5050'>return home</body></html>";
    printf("error sending\n");

    fputs(status,fp);
    fputs(server,fp);
    fputs(content,fp);
    send(*client_socket,content,Buf_size,0);
    fflush(fp);
}
