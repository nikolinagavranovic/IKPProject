#pragma comment(lib, "Ws2_32.lib")
#include "C:\Users\Nikolina\Desktop\Industrijski komunikacioni protokoli\publisher-subscriber\common\PublisherFunctions.h"
#include "C:\Users\Nikolina\Desktop\Industrijski komunikacioni protokoli\publisher-subscriber\common\Communication.h"

int __cdecl main(int argc, char** argv)
{
	srand(time(NULL));		// potrebno za generisanje random broja

	SOCKET connectSocket = INVALID_SOCKET;
	const char* messageToSend = "this is a test";
	const char* initialMessage = "New publisher connection.";
	Topic topic = GenerateRandomTopic();
	TypeTopic type = GenerateRandomType(topic);

	PrintPublisherInfo(topic, type);

	if (argc != 2)
	{
		printf("usage: %s server-name\n", argv[0]);
		return 1;
	}

	if (InitializeWindowsSockets() == false)
	{
		return 1;
	}

	connectSocket = socket(AF_INET,
		SOCK_STREAM,
		IPPROTO_TCP);

	if (connectSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
	serverAddress.sin_port = htons(DEFAULT_PORT);

	if (connect(connectSocket, (SOCKADDR*)& serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		printf("Unable to connect to server.\n");
		closesocket(connectSocket);
		WSACleanup();
	}

	if (Connect(connectSocket, initialMessage))
		while (true) {
			if (!Publish((void*)topic, (void*)type, messageToSend, connectSocket))
				break;
			Sleep(2500);
		}

	CloseSocket(&connectSocket);
	WSACleanup();

	return 0;
}