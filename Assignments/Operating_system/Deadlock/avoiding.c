#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define SEMPERM 0600
#define TRUE 1
#define FALSE 0
#define TOTALFORK 3

typedef union   _semun {
             int val;
             struct semid_ds *buf;
             ushort *array;
             } semun;

int initsem (key_t semkey, int n) {
   int status = 0, semid;
   
   //binary sempahore
   if ((semid = semget (semkey, 1, SEMPERM | IPC_CREAT | IPC_EXCL)) == -1)
   {
       if (errno == EEXIST)
                semid = semget (semkey, 1, 0);
   }
   
   //scheduling constraints
   else
   {
       semun arg;
       arg.val = n;
       status = semctl(semid, 0, SETVAL, arg);
   }
   if (semid == -1 || status == -1)
   {
       perror("initsem failed");
       return (-1);
   }
   return (semid);
}

int p (int semid) {
   struct sembuf p_buf;
   p_buf.sem_num = 0;
   p_buf.sem_op = -1;
   p_buf.sem_flg = SEM_UNDO;
   if (semop(semid, &p_buf, 1) == -1)
   {
      printf("p(semid) failed");
      exit(1);
   }
   return (0);
}

int v (int semid) {
   struct sembuf v_buf;
   v_buf.sem_num = 0;
   v_buf.sem_op = 1;
   v_buf.sem_flg = SEM_UNDO;
   if (semop(semid, &v_buf, 1) == -1)
   {
      printf("v(semid) failed");
      exit(1);
   }
   return (0);
}



// Class Lock
typedef struct _lock { 
//세마포 1개로 열림,잠김 및 waiting queue의 상태까지 모두 표현된다
   int semid;
} Lock;

void initLock(Lock *l, key_t semkey) {
   if ((l->semid = initsem(semkey,1)) < 0)
   // 세마포를 연결한다.(없으면 초기값을 1로 주면서 새로 만들어서 연결한다.)
      exit(1);
}

void Acquire(Lock *l) {
  p(l->semid);
}

void Release(Lock *l) {
  v(l->semid);
}



// Shared variable by file
void initVar(char *fileVar) {
// fileVar라는 이름의 텍스트 화일이 없으면 새로 만들고 0값을 기록한다.
  if(fopen(fileVar, "r") == NULL) {  
    FILE *fp = fopen(fileVar, "w");
    fprintf(fp, "%d\n", 0);
    fflush(fp);
    fclose(fp);
  }
}

void initVar_one(char *fileVar) {
// fileVar라는 이름의 텍스트 화일이 없으면 새로 만들고 1값을 기록한다.
  if(fopen(fileVar, "r") == NULL) {  
    FILE *fp = fopen(fileVar, "w");
    fprintf(fp, "%d\n", 1);
    fflush(fp);
    fclose(fp);
  }
}

void Store(char *fileVar,int i) {
// fileVar 화일 끝에 i 값을 append한다.
  FILE *fp = fopen(fileVar, "a");
  fprintf(fp, "%d\n", i);
  fflush(fp);
  fclose(fp);
}

int Load(char *fileVar) {
// fileVar 화일의 마지막 값을 읽어 온다.
  int num = 0;
  FILE *fp = fopen(fileVar, "r");
  while(fscanf(fp, "%d", &num) != EOF);
  fflush(fp);
  fclose(fp);
  
  return num;
}

void add(char *fileVar,int i) {
// fileVar 화일의 마지막 값을 읽어서 i를 더한 후에 이를 끝에 append 한다.
  int num = 0;
  num = Load(fileVar);
  num += i;
  Store(fileVar, num);
}

void sub(char *fileVar,int i) {
// fileVar 화일의 마지막 값을 읽어서 i를 뺀 후에 이를 끝에 append 한다.
  int num = 0;
  num = Load(fileVar);
  num -= i;
  Store(fileVar, num);
}



// Class CondVar
typedef struct _cond {
   int semid;
   char *queueLength;
} CondVar;

void initCondVar(CondVar *c, key_t semkey, char *queueLength) {
   c->queueLength = queueLength;
   initVar(c->queueLength); // queueLength=0
   if ((c->semid = initsem(semkey,0)) < 0)    
   // 세마포를 연결한다.(없으면 초기값을 0로 주면서 새로 만들어서 연결한다.)
      exit(1); 
}

void Wait(CondVar *c, Lock *lock) {
   add(c->queueLength, 1);
   Release(lock);
   p(c->semid);
   Acquire(lock);
}

void Signal(CondVar *c) {
  if(Load(c->queueLength) > 0) {
    v(c->semid);
    sub(c->queueLength, 1);
  }
}

void Broadcast(CondVar *c) {
  while(Load(c->queueLength) > 0) {
    v(c->semid);
    sub(c->queueLength, 1);
  }
}

void Take_R1(Lock *L1, char *R1, CondVar *C1, 
	     char *who, char *p1, char *p2) 
{
  Acquire(L1);
  // R1이 1이면 젓가락이 있고 0이면 젓가락이 없어서 기다려야 함
  while (Load(R1) == 0) 
  {
    // print message: getpid()가 R1을 기다림
    printf("%d waiting for R1\n", getpid());
    Wait(C1, L1);
    // print message: getpid()가 R1을 기다리다가 깨어남
    printf("%d was waiting for R1 and get up now\n", getpid());
  }

  while (Load(R1) == 1)
  {
    if(Load(p1) == 1 && Load(p2) == 1)
    {
      // print message: getpid()가 R1을 기다림
      printf("%d waiting for someone\n", getpid());    
      Wait(C1, L1);
      // print message: getpid()가 R1을 기다리다가 깨어남
      printf("%d was waiting for someone and get up now\n", getpid());
    }
    else 
      break;
  }

  Store(R1,0);
  add(who, 1);

  // print message: getpid()가  R1을 가져옴
  printf("%d get R1\n", getpid());

  Release(L1);
}

void Take_R2(Lock *L2, char *R2, CondVar *C2, 
	     char *who, char *p1, char *p2) 
{
  Acquire(L2);
  // R2이 1이면 젓가락이 있고 0이면 젓가락이 없어서 기다려야 함
  while (Load(R2) == 0) 
  {
    // print message: getpid()가 R2을 기다림
    printf("%d waiting for R2\n", getpid());
    Wait(C2, L2);
    // print message: getpid()가 R2을 기다리다가 깨어남
    printf("%d was waiting for R2 and get up now\n", getpid());
  }

  while (Load(R2) == 1)
  {
    if(Load(p1) == 1 && Load(p2) == 1)
    {
      // print message: getpid()가 R2을 기다림
      printf("%d waiting for someone\n", getpid());    
      Wait(C2, L2);
      // print message: getpid()가 R2을 기다리다가 깨어남
      printf("%d was waiting for someone and get up now\n", getpid());
    }
    else 
      break;
  }

  Store(R2,0);
  add(who, 1);

  // print message: getpid()가  R2을 가져옴
  printf("%d get R2\n", getpid());

  Release(L2);
}

void Take_R3(Lock *L3, char *R3, CondVar *C3, 
	     char *who, char *p1, char *p2) 
{
  Acquire(L3);
  // R3이 1이면 젓가락이 있고 0이면 젓가락이 없어서 기다려야 함
  while (Load(R3) == 0) 
  {
    // print message: getpid()가 R3을 기다림
    printf("%d waiting for R3\n", getpid());
    Wait(C3, L3);
    // print message: getpid()가 R3을 기다리다가 깨어남
    printf("%d was waiting for R3 and get up now\n", getpid());
  }

  while (Load(R3) == 1)
  {
    if(Load(p1) == 1 && Load(p2) == 1)
    {
      // print message: getpid()가 R3을 기다림
      printf("%d waiting for someone\n", getpid());    
      Wait(C3, L3);
      // print message: getpid()가 R3을 기다리다가 깨어남
      printf("%d was waiting for someone and get up now\n", getpid());
    }
    else 
      break;
  }

  Store(R3,0);
  add(who, 1);

  // print message: getpid()가  R3을 가져옴
  printf("%d get R3\n", getpid());

  Release(L3);
}

void Put_R1(Lock *L1, char *R1, CondVar *C1, char *who) {
  Acquire(L1);
  Store(R1,1);
  sub(who, 1);
  Signal(C1);
  // print message: getpid()가 R1을 기다림
  //printf("%d was waiting for R1\n", getpid());
  printf("%d put R1\n", getpid());
  Release(L1);
}

void Put_R2(Lock *L2, char *R2, CondVar *C2, char *who) {
  Acquire(L2);
  Store(R2,1);
  sub(who, 1);
  Signal(C2);
  // print message: getpid()가 R2을 기다림
  //printf("%d was waiting for R2\n", getpid());
  printf("%d put R2\n", getpid());
  Release(L2);
}

void Put_R3(Lock *L3, char *R3, CondVar *C3, char *who) {
  Acquire(L3);
  Store(R3,1);
  sub(who, 1);
  Signal(C3);
  // print message: getpid()가 R3을 기다림
  //printf("%d was waiting R3\n", getpid());
  printf("%d put R3\n", getpid());
  Release(L3);
}

void Phil_A(Lock *L3, char *R3, CondVar *C3, 
	    Lock *L1, char *R1, CondVar *C1, 
	    char *A, char *B, char *C) 
{
  Take_R3(L3, R3, C3, A, B, C);

  // print message: getpid()가  생각을 시작함
  printf("%d start to thinking\n", getpid());
  //think(); // sleep()으로 구현
  sleep(1);

  // print message: getpid()가  생각을 멈춤
  printf("%d stop to thinking\n", getpid());
  Take_R1(L1, R1, C1, A, B, C);

  // print message: getpid()가  먹기 시작함
  printf("%d start to eating\n", getpid());
  //eating(); // sleep()으로 구현
  sleep(1);

  // print message: getpid()가  먹기를 멈춤
  printf("%d stop to eating\n", getpid());
  Put_R3(L3, R3, C3, A);
  Put_R1(L1, R1, C1, A);
  sleep(1);
}

void Phil_B(Lock *L1, char *R1, CondVar *C1, 
	    Lock *L2, char *R2, CondVar *C2, 
	    char *A, char *B, char *C) 
{
  Take_R1(L1, R1, C1, B, A, C);

  // print message: getpid()가  생각을 시작함
  printf("%d start to thinking\n", getpid());
  //think(); // sleep()으로 구현
  sleep(1);

  // print message: getpid()가  생각을 멈춤
  printf("%d stop to thinking\n", getpid());
  Take_R2(L2, R2, C2, B, A, C);

  // print message: getpid()가  먹기 시작함
  printf("%d start to eating\n", getpid());
  //eating(); // sleep()으로 구현
  sleep(1);

  // print message: getpid()가  먹기를 멈춤
  printf("%d stop to eating\n", getpid());
  Put_R1(L1, R1, C1, B);
  Put_R2(L2, R2, C2, B);
  sleep(1);
}

void Phil_C(Lock *L2, char *R2, CondVar *C2, 
	    Lock *L3, char *R3, CondVar *C3, 
	    char *A, char *B, char *C) 
{
  Take_R2(L2, R2, C2, C, A, B);

  // print message: getpid()가  생각을 시작함
  printf("%d start to thinking\n", getpid());
  //think(); // sleep()으로 구현
  sleep(1);

  // print message: getpid()가  생각을 멈춤
  printf("%d stop to thinking\n", getpid());
  Take_R3(L3, R3, C3, C, A, B);

  // print message: getpid()가  먹기 시작함
  printf("%d start to eating\n", getpid());
  //eating(); // sleep()으로 구현
  sleep(1);

  // print message: getpid()가  먹기를 멈춤
  printf("%d stop to eating\n", getpid());
  Put_R2(L2, R2, C2, C);
  Put_R3(L3, R3, C3, C);
  sleep(1);
}

int main(int argc, char* argv[]) 
{
  key_t semkey_L1 = 0x200; 
  key_t semkey_L2 = 0x201;
  key_t semkey_L3 = 0x202;

  key_t semkey_C1 = 0x203;
  key_t semkey_C2 = 0x204;
  key_t semkey_C3 = 0x205;

  Lock L1;
  initLock(&L1, semkey_L1);
  Lock L2;
  initLock(&L2, semkey_L2);
  Lock L3;
  initLock(&L3, semkey_L3);

  CondVar C1;
  initCondVar(&C1, semkey_C1, "C1.txt");  
  CondVar C2;
  initCondVar(&C2, semkey_C2, "C2.txt");
  CondVar C3;
  initCondVar(&C3, semkey_C3, "C3.txt");
  
  char *R1 = "R1.txt";  
  char *R2 = "R2.txt";
  char *R3 = "R3.txt";

  char *A = "A.txt";
  char *B = "B.txt";
  char *C = "C.txt";

  initVar(A);
  initVar(B);
  initVar(C);

  initVar_one(R1);
  initVar_one(R2);
  initVar_one(R3);

  //void Take_R1(Lock *L1, char *R1, CondVar *C1)
  //void Put_R1(Lock *L1, char *R1, CondVar *C1)

  if(!strcmp(argv[1], "A"))
  {
    for(int i=0; i<100; i++)
    {
      Phil_A(&L3, R3, &C3, &L1, R1, &C1, A, B, C);
    }
  }

  if(!strcmp(argv[1], "B"))
  {
    for(int j=0; j<100; j++)
    {
      Phil_B(&L1, R1, &C1, &L2, R2, &C2, A, B, C);
    }
  }

  if(!strcmp(argv[1], "C"))
  {
    for(int k=0; k<100; k++)
    {
      Phil_C(&L2, R2, &C2, &L3, R3, &C3, A, B, C);
    }
  }


// Phil_A, Phil_B, Phil_C 는 별도의 프로세스로 실행되어야 함
 
  return 0;
}

