#pragma comment(lib,"Ws2_32.lib")
#include<WinSock2.h>
#include<Windows.h>
#include<iostream>
using namespace std;

SOCKADDR_IN addr;
SOCKET sListen;
SOCKET sConnect;
SOCKET* Connections;
int addrlen=sizeof(addr);
int ConCounter=0;
int ServerThread(int ID)
{	char*Recv=new char[256];
	ZeroMemory(Recv,sizeof(Recv));

	for(;;Sleep(10)){
	if(recv(Connections[ID],Recv,sizeof(Recv),NULL)){
		cout<<Recv<<endl;
	}
}
	return 0;
}
int InitWinSock(){
	int RetVal=0;
	WSAData wsaData;
	WORD DllVersion=MAKEWORD(2,1);
	RetVal=WSAStartup(DllVersion,&wsaData);
	
	return RetVal;
}
int main(){
	int RetVal=0;
	RetVal=InitWinSock();
	if(RetVal!=0){
		MessageBoxA(NULL,"Winsock startup failed","Error",MB_OK |MB_ICONERROR);
		exit(1);
	}
	Connections=(SOCKET*)calloc(64,sizeof(SOCKET));
	
	sListen=socket(AF_INET,SOCK_STREAM,'\0');
	sConnect=socket(AF_INET,SOCK_STREAM,'\0');
	
	addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	addr.sin_port       =htons(1234);
	addr.sin_family     =AF_INET;
	
	bind(sListen,(SOCKADDR*)&addr,sizeof(addr));
	listen(sListen,64);
	for(;;Sleep(50)){
		if(sConnect=accept(sListen,(SOCKADDR*)&addr,&addrlen))
		{
			Connections[ConCounter]=sConnect;
			char* ID=new char[64];
			ZeroMemory(ID,sizeof(ID));
			itoa(ConCounter,ID,10);
			
			send(Connections[ConCounter],ID,sizeof(ID),'\0');
			ConCounter++;
			CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)ServerThread,(LPVOID)(ConCounter-1),NULL,NULL);
		}
	}
	return 0;
}
