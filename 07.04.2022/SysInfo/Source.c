#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib, "netapi32.lib")


#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <lm.h>
#include <assert.h>
#include <LMaccess.h>
#include <locale.h>


int main()
{
	setlocale(LC_ALL, "Rus");
	/*WCHAR Server[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD NameSize = MAX_COMPUTERNAME_LENGTH + 1;
	GetComputerName(Server, &NameSize);*/
	char Server[256];
	unsigned long size = 1024;
	GetComputerName(Server, &size);
	printf("Computersname = %ws\n", (WCHAR*)Server);
	NET_API_STATUS Result;
	LPUSER_INFO_0 UsersList = NULL;
	DWORD SizeOfInfo=0;
	DWORD TotalenTries=0;
	PDWORD  resume_handle=0;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	Result = NetUserEnum(Server, 1,FILTER_NORMAL_ACCOUNT,(LPBYTE*)&UsersList, dwPrefMaxLen,&SizeOfInfo,&TotalenTries,&resume_handle);
	if (Result == NERR_Success)
	{
		for (int i = 0; i < TotalenTries && UsersList[i].usri0_name;i++)
		{
			printf("User %d:",i+1);
			wprintf(L"%s\n", UsersList[i].usri0_name);
		}
	}
}