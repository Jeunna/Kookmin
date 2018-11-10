//TCP echo client program

#include <stdio.h>	//표준입출력
#include <stdlib.h>	//표준 라이브러리
#include <string.h>	//문자열 처리 함수
#include <unistd.h>	//유닉스 표준
#include <arpa/inet.h>	//인터넷 프로토콜
#include <sys/socket.h>	//소켓 통신 함

#define PORT 9999
#define BUF_SIZE 256

int main(void)
{
  //Declare and define
  int sock; 			//서버에 접속할 소켓
  char message[BUF_SIZE];	//서버에 보낼 메세지를 저장할 문자열 버퍼
  struct sockaddr_in serverAddr;//접속할 서버의 주소
  int recv_len;			//송수신 메세지의 문자열 길이


  //Create remote(server) socket address
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serverAddr.sin_port = htons(PORT);

  //Create socket | TCP(Stream) 프로토콜 생성
  if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("Error:socket creation failed!");
    exit(1);
  }

  //connect to the server | 클라이언트 함수이고 서버는 accept 상태여야 한다.
  if(connect (sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
  {
    perror("Error:connection failed!");
    exit(1);
  }

  //연결 성공
  printf("Connected!\n");


  //Data transfer section
  while(1)
  {
    printf("(bye to quit) ");
    printf("<< ");
    fgets(message, BUF_SIZE, stdin);

    if(!strcmp(message, "bye\n") || !strcmp(message, "BYE\n"))
    {
      send(sock, message, strlen(message), 0);
      recv_len = recv(sock, message, BUF_SIZE-1, 0);
      break;
    }
    send(sock, message, strlen(message), 0);		//소켓을 이용해 메세지 전송
    recv_len = recv(sock, message, BUF_SIZE-1, 0);	//에코되어 돌아오는 메세지 수신
    message[recv_len] = '\0';				//보낼 때 NULL문자 안보내서 받았을 때 넣어줘야함.
    //Print and verify the echoed string
    printf("Echoed string received: %s\n", message);
  }
  
  //Close socket
  close(sock);
  //Stop program
  exit(0);
}

