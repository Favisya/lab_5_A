#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>

int main()
{
	int sum = 0;
	char* strReceived;
	HANDLE hReceivedEvent, hAnswerEvent, hdopEvent, hMapping;

	// Открытие объекта "отображение файла" и создание представления strReceived
	hMapping = OpenFileMapping(FILE_MAP_WRITE, FALSE, L"Lab5_Mapping");
	strReceived = (char*)MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 0, 250);

	// Открытие событий для синхронизации обмена с процессом A
	hReceivedEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Sent1");
	hAnswerEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"Sent2");
	hdopEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"dop");

	while (true)
	{
		WaitForSingleObject(hAnswerEvent, INFINITE);
		//приема/посылки данных
		printf("sum = %d\n", atoi(strReceived) + sum);
		sum += atoi(strReceived);
	}

	return 0;
}