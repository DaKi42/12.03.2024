#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "WinSock2.h" 
#include <ws2tcpip.h> 
#pragma comment(lib, "Ws2_32.lib") 
#include "Windows.h"
#include "tchar.h"
#include "resource.h"
using namespace std;

const int MAXSTRLEN = 255;

class Socket
{
protected:
	WSADATA wsaData;//структура для хранения информации о инициализации сокетов
	SOCKET _socket; //дескриптор слушающего сокета
	SOCKET acceptSocket;//дескриптор сокета, который связан с клиентом 
	sockaddr_in addr; //локальный адрес и порт
public:
	Socket();
	~Socket();
	bool SendData(char*); //метод для отправки данных в сеть
	bool ReceiveData(char*, int);//метод для получения данных
	void CloseConnection(); //метод для закрытия соединения
	void SendDataMessage();//метод для отправки сообщения пользователя
};

class ServerSocket : public Socket
{
public:
	void Listen(); //метод для активации "слушающего" сокета
	void Bind(int port); //метод для привязки сокета к порту
	void StartHosting(int port); //объединяет вызов двух предыдущих методов
};

class ClientSocket : public Socket
{
public:
	//метод для подключения к серверу
	void ConnectToServer(const char * ip, int port);
};