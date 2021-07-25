#pragma comment(lib, "Ws2_32.lib")
#include "C:\Users\Nikolina\Desktop\Industrijski komunikacioni protokoli\publisher-subscriber\common\PubSubEngineFunctions.h"

int  main(int argc, char** argv)
{
	int iResult;

	SOCKET publisherListenSocket = INVALID_SOCKET;
	SOCKET subscriberListenSocket = INVALID_SOCKET;

	InitializeOurCriticalSection();

	if (InitializeWindowsSockets() == false)
	{
		return 1;
	}

	publisherListenSocket = *CreatePublisherListenSocket();
	subscriberListenSocket = *CreateSubscriberListenSocket();

	AddSocketToList(&listSockets, &publisherListenSocket);
	AddSocketToList(&listSockets, &subscriberListenSocket);
	DWORD printSubID;
	HANDLE ThreadSub;
	ThreadSub = CreateThread(NULL, 0, &ListenSubscriber, &subscriberListenSocket, 0, &printSubID);
	LitenForPublisher(publisherListenSocket);

	CloseAllSockets();
	Sleep(1500);

	CloseAllThreads();
	CloseHandle(ThreadSub);

	WSACleanup();

	DeleteOurCriticalSection();

	return 0;
}