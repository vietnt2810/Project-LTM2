#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 1000
#define MAX_LISTEN_QUEUE 1000

//////////////////////////////////////// Menu Function ////////////////////////////////////////
void loginMenu() {
    printf("1. Đăng ký tài khoản.\n");
    printf("2. Đăng nhập tài khoản.\n");
}
void mainMenu() {
    printf("1. Chơi.\n");
    printf("2. Đổi mật khẩu.\n");
    printf("3. Đăng xuất.\n");
}
//////////////////////////////////////// Menu Function ////////////////////////////////////////

int main(int argc, char *argv[]){
    int sockfd, serverPortNum, sendBytes, rcvBytes;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], recvline[MAXLINE], server_response[256], string1[100];

    // create a socket for theo client
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Error: ");
        exit(-1);
    }

    serverPortNum = atoi(argv[2]);

    // create the remote server socket information structure
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);

    // inet_aton("127.0.0.1", &servaddr.sin_addr); 
    servaddr.sin_port = htons(serverPortNum);

    // connection theo client to the server socket
    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        perror("Problem in connecting to the server");
        exit(-1);
    }

    loginMenu();

    while(1){
        fflush(stdin);
        fgets(sendline, MAXLINE,stdin);
        sendline[strlen(sendline) -1] = '\0';
        sendBytes = send(sockfd, sendline , strlen(sendline) +1, 0);
        if(sendBytes < 0){
            perror("Error: ");
            return 0;
        }
        
        rcvBytes = recv(sockfd, &server_response, MAXLINE, 0);
        if(rcvBytes <0){
            perror("Error: ");
            return 0;
        }
        server_response[rcvBytes] = '\0';
        printf("%s\n", server_response);  

        if(strcmp(server_response, "--- Vui lòng chọn lại ---") == 0){
            loginMenu();
        } 

        if(strcmp(server_response, "--- Tài khoản đã tồn tại ---") == 0){
            loginMenu();
        } 

        if(strcmp(server_response, "--- Tài khoản không tồn tại ---") == 0){
            loginMenu();
        }

        if(strcmp(server_response, "--- Mật khẩu không chính xác ---") == 0){
            loginMenu();
        }

        if(strcmp(server_response, "--- Tạo tài khoản thành công ---") == 0){
            mainMenu();
        } 

        if(strcmp(server_response, "--- Đăng nhập thành công ---") == 0){
            mainMenu();
        } 

        if(strcmp(server_response, "--- Đổi mật khẩu thành công ---") == 0){
            mainMenu();
        } 

        if(strcmp(server_response, "--- Mật khẩu không khớp ---") == 0){
            mainMenu();
        } 

        if(strcmp(server_response, "--- Đăng xuất thành công ---") == 0){
            loginMenu();
        }  

        if(strcmp(server_response, "--- Nhập sai rồi!!!! ---") == 0){
            mainMenu();
        } 

        if(strcmp(server_response, "--- Cảm ơn bạn đã tham gia chơi ---") == 0){
            mainMenu();
        } 
    }
}