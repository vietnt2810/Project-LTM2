#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define MAXLINE 100
#define MAX_LISTEN_QUEUE 100

//////////////////////////////////////// Account ////////////////////////////////////////
typedef struct node {
  	char username[20];
	char password[20];
	int status;
	struct node *next;
}node;

node *head = NULL;

void insert(char username[20], char password[20], int status){
	node *temp;
	temp=(node *)malloc(sizeof(node));
	strcpy(temp->username, username);
	strcpy(temp->password, password);
	temp->status = status;
	temp->next = head;
	head = temp;
}

node *find(char name[20]){
   node *current = head;
   if(head == NULL)
	{
      return NULL;
    }
   while(strcmp(current->username, name) != 0){
      if(current->next == NULL){
         return NULL;
      }else {
         current = current->next;
	
      }
   }      
   return current;
}

void printAll()
{
    node *temp;
    temp = head;
    while(temp)
    {
    	printf("%20s %20s %d",temp->username, temp->password, temp->status);
        printf("\n");
        temp=temp->next;
    }
    printf("\n");
}

void openFile(){
	node *acc;
	char *username;
	char *password;
	int status;
	username = (char *)malloc(20);
	password = (char *)malloc(20);
    FILE *fptr;
	if((fptr=fopen("account.txt","r+"))==NULL){
		printf("Not find%s\n","account.txt");
		return;
	}
	while(1){
		fscanf(fptr,"%s",username);
		fscanf(fptr,"%s",password);
		fscanf(fptr,"%d",&status);
		if(feof(fptr)) break;
		insert(username, password, status);
	}
	free(username); free(password);
	fclose(fptr);
}

void writeFile(){
	FILE *fptr;
	node *temp;
    temp = head;
    fptr=fopen("account.txt","w+");
    while(temp){
    	fprintf(fptr, "%s %s %d", temp->username, temp->password, temp->status);
    	fprintf(fptr, "\n");
    	temp=temp->next;
    }
    fclose(fptr);
}

void registerAccount(){
	char username[20];
	char password[20];
	char homepage[200];
	printf("---------Register\n");
	printf("Username: "); scanf("%s", username);
	printf("\n");
	printf("Password: "); scanf("%s", password);
	printf("\n");
	
	if(find(username) != NULL){
	printf("Account existed!");	
	} 
	else{
		insert(username, password, 1);
		writeFile();
		printf("Successful registration. Activation required.\n");
	}
	printf("\n");
}

void activateAccount(){
    char username[20];
	char password[20];
    char temp[20];
    char activationCode[20] = "LTM121216";
    int count = 0;
    printf("---------Activate\n");
    printf("Type username: "); scanf("%s", username);
    printf("\n");
    node *acc = find(username);
    if(acc != NULL){
		if(acc->status == 0){
			printf("Account has been blocked!\n");
            printf("------------------------\n");
		}
        else if(acc->status == 1){
            printf("Account is already activated!\n");
            printf("------------------------\n");
        }
        else if(acc->status == 2){
            printf("Account is idle - need to be activated!\n");
            printf("Type password: "); scanf("%s", password);
            printf("\n");
            if (strcmp(acc->password,password) == 0){
			printf("Success!\n");
            while (count<6) {
            printf("Type activation code: "); scanf("%s", temp);
            printf("\n");
            if (strcmp(temp,activationCode) == 0) {
                acc->status = 1;
                printf("Successful activation.\n");
                printf("------------------------\n");
                writeFile();
                printAll();
                count = 6;
                }
                else { 
                    count++;
                    if (count == 5) {
                        printf("Wrong code! You have failed 5 times. This account is now blocked!\n");
                        printf("------------------------\n");
                        acc->status = 0;
                        writeFile();
                        printAll();
                        count++;
                        }  
                    else {
                        printf("Wrong code! Try again.\n");
                    }
			}   
            }
        }
        else {
                printf("Wrong password!");
                printf("------------------------\n");
            }
        }
    }
    else { 
            printf("Account doesn't exist!\n");
            printf("------------------------\n");
           }
}

node *login(){
    char username[20];
	char password[20];
	int  count = 0;
    printf("---------Login\n");
    printf("Type username: "); scanf("%s",username);
    printf("\n");
    node *acc = find(username);
    if (acc != NULL) {
        if (acc->status == 0) {
            printf("Account is blocked!");
            printf("------------------------\n");
            return NULL;
        }
        else if (acc->status == 2) {
            printf("Account needs to be activated!");
            printf("------------------------\n");
            return NULL;
        }
        else {
            while (count<4) {
                printf("Type password: "); scanf("%s",password);
                printf("\n");
                if (strcmp(acc->password,password) == 0) {
                    printf("Login succesfully.\n");
                    printf("------------------------\n");
                    return acc;
                }
                else {
                    count++;
                    if (count==3) {
                        printf("Wrong password! You have failed 3 times. This account is now blocked!\n");
                        printf("------------------------\n");
                        acc->status = 0;
                        writeFile();
                        return NULL;
                    }
                    else {
                        printf("Wrong password! Try again.\n");
                    }
                }
            }
        }
    }
    else {
        printf("Account doesn't exist!");
        printf("------------------------\n");
        return NULL;
    }
}

void search(){
	char username[20];
	printf("---------Search\n");
	printf("Type username: "); scanf("%s", username);
	printf("\n");
	node *acc = find(username);
	if(acc != NULL){
		printf("Username: %20s Status: %d\n", acc->username, acc->status);
        printf("------------------------\n");
	}
	else {
        printf("Account does not exist\n");
        printf("------------------------\n");
        }
}

node *signout(){
	char username[20];
	printf("---------Sign Out\n");
	printf("Username: "); scanf("%s", username);
	printf("\n");
	node *acc = find(username);
	if(acc == NULL){
		printf("Account does not exist\n");
		return NULL;
	}
	else return acc;
}
//////////////////////////////////////// Account ////////////////////////////////////////
//////////////////////////////////////// Question ////////////////////////////////////////
typedef struct bank{
    char question[120];
    char choiceA[70];
    char choiceB[70];
    char answer[5];
}Bank;
 
typedef struct question
{
    char word[1000];
    char answer[5];
}Question;

Bank setQuestion[20];
Question questionList[20];
FILE *question;

void readQuestion(){ // Gets and stores questions
    if((question = fopen("question.txt", "r"))==NULL){
        printf("File doesn't exist!\n");
		return;
    }
    else{
        while(!feof(question)){
            for(int i=0; i<20; i++){ // Reads the questions from file
                fgets(setQuestion[i].question, 120, question);
                fgets(setQuestion[i].choiceA, 70, question);
                fgets(setQuestion[i].choiceB, 70, question);
                fgets(setQuestion[i].answer, 70, question);
            }
        }
    }
    fclose(question);
}

void makeQuestion(){
    Question *buff;
    readQuestion();
    for (int i = 0; i < 20; i++)
    {
        buff = (Question *)malloc(sizeof(Question));
        strcpy((*buff).word, setQuestion[i].question);
        strcat((*buff).word, setQuestion[i].choiceA);
        strcat((*buff).word, setQuestion[i].choiceB);
		(*buff).word[strlen((*buff).word) - 1] = '\0';
        strcpy((*buff).answer, setQuestion[i].answer);
		(*buff).answer[strlen((*buff).answer) - 1] = '\0';
        strcpy(questionList[i].word, (*buff).word);
        strcpy(questionList[i].answer, (*buff).answer);
        free(buff);
    }
}

void swap(Question *a, Question *b) { 
    Question temp = *a; 
    *a = *b; 
    *b = temp; 
} 

void randomize(Question arr[], int n) { 
    srand(time(NULL)); 

    for (int i = n-1; i > 0; i--) 
    { 
        int j = rand() % (i+1); 
        swap(&arr[i], &arr[j]); 
    } 
}

// void convertAnswer(char a[5]) {

// }
//////////////////////////////////////// Question ////////////////////////////////////////
void sendMess(char *content, int sockfd, struct sockaddr *servaddr){
	int len, sendBytes;
	  
	len = sizeof(*servaddr);

	sendBytes = send(sockfd, content, strlen(content) +1, 0);

	if(sendBytes < 0){
		perror("Error: \n");
	}
	
}

pid_t fork(void);

int main(int argc, char* argv[]){

    openFile();

	pid_t pid;
    int listenfd, connfd, n, portNumber;
    pid_t childpid;
    socklen_t clilen;
    char buff[MAXLINE];
    struct sockaddr_in cliaddr, servaddr;

    portNumber = atoi(argv[1]);


    // construct socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // bind socket to ip address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(portNumber);

    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenfd, MAX_LISTEN_QUEUE);
    printf("%s\n", "Server running...waiting for connections.");

    makeQuestion();
    node *acc;
	int loginVar = 0, optionVar = 0, i = 0, check = 0, readyCheck = 0;
    char username[20], password[20], tempPassword[20];
	
    // communicate with client
    while(1){
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
        printf("%s\n", "Received request...");

		if( (pid = fork()) == 0) {           //process in child (forking)
			
			close(listenfd); 	             // child closes listening socket
			
			while(n = recv(connfd, buff, MAXLINE, 0) > 0){
                if (strcmp(buff,"1") == 0 && loginVar == 0) {
                    loginVar = 1;
                    sendMess("--- Nhập tên tài khoản mới ---", connfd, (struct sockaddr*) &cliaddr);
                }
                else if (strcmp(buff,"2") == 0 && loginVar == 0) {
                    sendMess("--- Nhập tên tài khoản của bạn ---", connfd, (struct sockaddr*) &cliaddr);
                    loginVar = 3;
                }
                else {
                    switch (loginVar) {
                        case 1: 
                        if (find(buff) == NULL) {
                            sendMess("--- Nhập mật khẩu ---", connfd, (struct sockaddr*) &cliaddr);
                            strcpy (username, buff);
                            loginVar = 2;
                        }
                        else {
                            sendMess("--- Tài khoản đã tồn tại ---", connfd, (struct sockaddr*) &cliaddr);
                            loginVar = 0;
                        }
                        break;

                        case 2:
                        sendMess("--- Tạo tài khoản thành công ---", connfd, (struct sockaddr*) &cliaddr);
                        strcpy (password, buff);
                        insert (username,password,1);
                        writeFile();
                        loginVar = 5;
                        break;

                        case 3:
                        acc = find(buff);
                        if (acc == NULL) {
                            sendMess("--- Tài khoản không tồn tại ---", connfd, (struct sockaddr*) &cliaddr);
                            loginVar = 0;
                        }
                        else {
                            sendMess("--- Nhập mật khẩu ---", connfd, (struct sockaddr*) &cliaddr);
                            loginVar = 4;
                        }
                        break;

                        case 4:
                        if (strcmp(acc->password,buff) == 0) {
                            sendMess("--- Đăng nhập thành công ---", connfd, (struct sockaddr*) &cliaddr);
                            loginVar = 5;
                        }
                        else {
                            sendMess("--- Mật khẩu không chính xác ---", connfd, (struct sockaddr*) &cliaddr);
                            loginVar = 0;
                        }
                        break;

                        case 5:
                        if (strcmp(buff,"2") == 0 && optionVar == 0) {
                            sendMess("--- Nhập mật khẩu mới ---", connfd, (struct sockaddr*) &cliaddr);
                            optionVar = 2;
                        }
                        else if (strcmp(buff,"3") == 0 && optionVar == 0) {
                            sendMess("--- Đăng xuất thành công ---", connfd, (struct sockaddr*) &cliaddr);
                            loginVar = 0;
                            optionVar = 0;
                        }
                        else if (strcmp(buff,"1") == 0 && optionVar == 0) {
                            sendMess("Nhập READY nếu bạn đã sẵn sàng", connfd, (struct sockaddr*) &cliaddr);
                            optionVar = 1;
                        }
                        else {
                            switch (optionVar) {
                            case 1:
                            if (strcmp(buff,"READY") == 0) {
                                randomize(questionList,10);
                                sendMess(questionList[i].word, connfd, (struct sockaddr*) &cliaddr);
                                readyCheck = 1;
                            }
                            else if ((strcmp(buff,"A") == 0 || strcmp(buff,"a") == 0) && readyCheck == 1) {
                                check = check + 1;
                                i = i + 1;
                                if (i < 10) {
                                    sendMess(questionList[i].word, connfd, (struct sockaddr*) &cliaddr);
                                }
                                else {
                                    sendMess("Bạn đã trả lời xong, vui lòng nhập OK để nhận đáp án từ hệ thống", connfd, (struct sockaddr*) &cliaddr);
                                    i = 0;
                                    optionVar = 4;
                                }
                            }
                            else if ((strcmp(buff,"B") == 0 || strcmp(buff,"b") == 0) && readyCheck == 1) {
                                i = i + 1;
                                if (i < 10) {
                                    sendMess(questionList[i].word, connfd, (struct sockaddr*) &cliaddr);
                                }
                                else {
                                    sendMess("Bạn đã trả lời xong, vui lòng nhập OK để nhận đáp án từ hệ thống", connfd, (struct sockaddr*) &cliaddr); 
                                    i = 0;
                                    optionVar = 4;
                                }
                            }
                            else {
                                sendMess("--- Nhập sai rồi!!!! ---", connfd, (struct sockaddr*) &cliaddr);
                                optionVar = 0;
                                i = 0;
                            }

                            
                            break;

                            case 2:
                            strcpy(tempPassword,buff);
                            sendMess("--- Nhập lại mật khẩu mới ---", connfd, (struct sockaddr*) &cliaddr);
                            optionVar = 3;
                            break;

                            case 3:
                            if (strcmp(tempPassword,buff) == 0) {
                                strcpy(acc->password,tempPassword);
                                writeFile();
                                sendMess("--- Đổi mật khẩu thành công ---", connfd, (struct sockaddr*) &cliaddr);
                                optionVar = 0;
                                }
                            else {
                                sendMess("--- Mật khẩu không khớp ---", connfd, (struct sockaddr*) &cliaddr);
                                optionVar = 0;
                                }
                            break;

                            case 4:
                            if (strcmp(buff,"OK") == 0) {
                                if (check < 3) {
                                    sendMess("Bạn là người hướng có thiên hướng trực giác.\n => Phù hợp với các nghề về kĩ thuật.\nBạn đã chơi xong, vui lòng nhập bất kỳ để trở về menu chính.", connfd, (struct sockaddr*) &cliaddr);
                                    optionVar = 5;
                                }
                                else if (check >=3 && check < 7) {
                                    sendMess("Bạn là người lý trí. phần lí trí là phần được đánh giá cao nhất, nó có vai trò tìm hiểu các thông tin liên quan dựa trên các bộ phân tiêu chí đúng sai, trái hay phải. Sau đó, suy luận một cách logic mới trực tiếp cho đáp án cụ thể nhất, có căn cứ nhất, có khoa học nhất.\n => Phù hợp với các nghề về logic cao.\nBạn đã chơi xong, vui lòng nhập bất kỳ để trở về menu chính.", connfd, (struct sockaddr*) &cliaddr);
                                    optionVar = 5;
                                }
                                else {
                                    sendMess("Bạn là người hướng có thiên hướng cảm xúc,Phần cảm xúc của não bộ sẽ xem xét sự việc trên tổng thế các vấn đề cảm tính, yêu hay ghét, hận hay thu đồng thời các yếu tố đó có sự tác động qua lại lẫn nhau, không có một sự rạch ròi, đó là bản chất của vấn đề cảm xúc do não quyết định.\n => Phù hợp với các nghề về nghệ thuật.\nBạn đã chơi xong, vui lòng nhập bất kỳ để trở về menu chính.", connfd, (struct sockaddr*) &cliaddr);
                                    optionVar = 5;
                                }
                            }
                            else {
                                sendMess("--- Nhập sai rồi!!!! ---", connfd, (struct sockaddr*) &cliaddr);
                                optionVar = 0;
                                readyCheck = 0;
                            }
                            break;

                            case 5:
                            if (strcmp(buff,"OK") == 0){
                                sendMess("--- Cảm ơn bạn đã tham gia chơi ---", connfd, (struct sockaddr*) &cliaddr);
                                optionVar = 0;
                                readyCheck = 0;
                            }
                            else {
                                sendMess("--- Cảm ơn bạn đã tham gia chơi ---", connfd, (struct sockaddr*) &cliaddr);
                                optionVar = 0;
                                readyCheck = 0;
                            }
                            break;

                            default:
                            sendMess("--- Nhập sai rồi!!!! ---", connfd, (struct sockaddr*) &cliaddr);
                            break;
                            }
                            
                        }

                        default:
                        sendMess("--- Vui lòng chọn lại ---", connfd, (struct sockaddr*) &cliaddr);
                        break;
                    }
                }
        	}

			exit(0);			
		
		}

        if(n < 0){
            perror("Read error: ");
            exit(-1);
        }

        close(connfd);
    }

    close(listenfd);

    return 0;
}