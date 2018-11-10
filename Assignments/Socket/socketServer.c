//Echo server program

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
  int sock, client_sock;	//server socket, client socket
  //서버 소켓은 받아들이는 일만하고 실제 통신은 클라이언트 소켓이 한다
  char message[BUF_SIZE];	//메세지 수진용 버퍼
  struct sockaddr_in serverAddr;//서버용 소켓(accept용)
  struct sockaddr_in clientAddr;//클라이언트 소켓(서버 소켓이 accept 해주면 생성)

  int recv_len;			//수신한 데이터의 길이
  int client_len = sizeof(clientAddr);
  //클라이언트 주소 구조체의 바이트 사이즈
  int waitSize = 16;

  //Create local (server) socket address
  memset(&serverAddr, 0, sizeof(serverAddr));	//서버 구조체를 0으로 채운다
  serverAddr.sin_family = AF_INET;		//서버 주소 체계를 인터넷 계열로 설정
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);//서버는 주소 필요없음(제자리 동작)
  serverAddr.sin_port = htons(PORT);		//서버가 사용할 포트 번호

  //Create listen socket | TCP 통신용 서버 소켓 생성
  if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("Error: Listen socket failed!");
    exit(1);
  }

  //Bind listen socket to the local socket address | 소켓과 주소를 엮어준다
  if(bind(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
  {
    perror("Error:binding falied!");
    exit(1);
  }

  printf("waiting for client..\n");

  //Listen to connerction requests | 소켓을 서버용으로 사용할 수 있게 한다. (클라이언트가 듣는다?)
  if(listen(sock, waitSize) < 0)
  {
    perror("Error:listening failed!");
    exit(1);
  }


  //Handle the connection
  //서버는 패시브. 계속 기다린다 요청이 올 때까지
  for(;;)
  {
    //Accept connections from client | accept가 성공하면 클라이언트 소켓 client_sock이 만들어진다
    if((client_sock = accept(sock, (struct sockaddr *)&clientAddr, &client_len)) < 0)
    {
      perror("Error:accepting failed!");
      exit(1);
    }

    printf("client ip : %s\n", inet_ntoa(clientAddr.sin_addr));

    //Data transfer section | 클라이언트가 소켓을 닫아버리면 0(NULL)이 돌아온다.
    while((recv_len = recv(client_sock, message, BUF_SIZE, 0)) != 0)
    {
      message[recv_len] = '\0';
      printf("received data : %s", message);
      send(client_sock, message, recv_len, 0);  
    }
    //클라이언트와 접속이 끊겼으면 클라이언트 소켓을 닫는다.
    close(client_sock);
    break;
  }
  //클라이언트가 끊겼으니까 서버 소켓도 닫는다.
  close(sock);
  return 0;
}



