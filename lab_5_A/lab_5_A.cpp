#define _CRT_SECURE_NO_WARNINGS

#include "process.h"
#include <iostream>
#include <windows.h>
#include <ctime>
#include <stdlib.h>
#include <io.h>
#include <Fcntl.h>
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>

int main()
{
	char* strToSend;
	HANDLE hDataSentEvent, hAnswerEvent, hdopEvent, hMapping;

	//Структура STARTUPINFO используется с функциями CreateProcess, CreateProcessAsUser, CreateProcessWithLogonW,
	//чтобы определить оконный терминал, рабочий стол, стандартный дескриптор и внешний вид основного окна для нового процесса.
	STARTUPINFO startInfo = { sizeof(startInfo) }; 

	//Структура PROCESS_INFORMATION заполняется функцией CreateProcess с информацией о недавно созданном процессе и его первичном потоке.
	PROCESS_INFORMATION procInfo1, procInfo2;

	//Структура SECURITY_ATTRIBUTES определяет уровень и тип защиты объекта.
		//Размер структуры в байтах
		//Указатель на описатель объекта. Для использования описателя. по умолчанию нуль
		//Определяет, должен ли описатель объекта быть унаследован новыми процессами или программами
	SECURITY_ATTRIBUTES pipeAttributes = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

	// Создание событий для синхронизации обмена с процессами
	// CreateEvent:
		// атрибут защиты
		// тип сброса TRUE - ручной
		// начальное состояние TRUE - сигнальное
		// имя обьекта
	hDataSentEvent = CreateEvent(NULL, FALSE, FALSE, L"Sent1");
	hAnswerEvent = CreateEvent(NULL, FALSE, FALSE, L"Sent2");
	hdopEvent = CreateEvent(NULL, FALSE, FALSE, L"dop");

	// Функция CreateFileMapping создает или открывает именованный или безымянный объект
	//  отображенного в памяти (проецируемого) файла для заданного файла.
		// дескриптор файла
		// защита 
		// атрибуты защиты
		// старшее слово размера
		// младшее слово размера
		// имя объекта
	hMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 4096, L"Lab5_Mapping");

	//Функция MapViewOfFile отображает представление проецируемого файла в адресное пространство вызывающего процесса.
		// дескр. объекта проецируемый файл
		// режим доступа
		// старшее DWORD смещения
		// младшее DWORD смещения
		// число отображаемых байтов
	strToSend = (char*)MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 0, 250);

	//CreateProcess
			// имя исполняемого модуля
			// командная строка
			// SD (дескриптор безопасности)
			// содержит сведения о безопасности, связанные с защищаемым объектом.
			// Дескриптор безопасности состоит из структуры _ дескрипторов безопасности и связанных
			// с ней сведений о безопасности. Дескриптор безопасности может включать следующие сведения о безопасности
			// SD
			// дескриптор параметра наследования
			// флажки создания
			// новый блок конфигурации
			// имя текущего каталога
			// информация предустановки
			// информация о процессе
	printf("1|0 = process is created or not.\n");
	printf("First process: ");
	printf("%d\n", CreateProcess(L"lab_5_B.exe", NULL, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &startInfo, &procInfo1));
	printf("Second process: ");
	printf("%d\n", CreateProcess(L"lab_5_C.exe", NULL, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &startInfo, &procInfo2));

	int proc;
	// Основной цикл посылки/приема данных
	do {
		printf("Enter the num: ");
		scanf("%s", strToSend);
		printf("\n");
		printf("Enter num of proc: ");
		scanf("%d", &proc);
		printf("\n");

		switch (proc)
		{
		case 1:
			SetEvent(hDataSentEvent);
			break;
		case 2:
			SetEvent(hAnswerEvent);
			break;
		case 0:
			SetEvent(hdopEvent);
			return 0;
		}
	} while (true);
}