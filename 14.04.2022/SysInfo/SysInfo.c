#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "netapi32.lib")

#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <lm.h>
#include <sddl.h>  
#include <locale.h>
#include <string.h>
#include <time.h>
#include <conio.h>

COORD coord = { 0,0 };

void GoToXY(int x, int y)
{
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void LocalTime(void);
int AllInformationGet(LPUSER_INFO_1* UsersList, DWORD* NameSize, DWORD* TotalenTries, WCHAR* Server);
int PrintfAllInformationsOfUsers(LPUSER_INFO_1 UsersList, DWORD TotalenTries, WCHAR* Server);


int main()
{
	LocalTime();
	LPUSER_INFO_1 UsersList = NULL;
	DWORD TotalenTries = 0;
	DWORD NameSize = MAX_COMPUTERNAME_LENGTH + 1;
	WCHAR* Server = (WCHAR*)malloc(sizeof(WCHAR) * NameSize);
	AllInformationGet(&UsersList, &NameSize, &TotalenTries, Server);
	printf("Computer's name = %ws\n", (WCHAR*)Server);
	PrintfAllInformationsOfUsers(UsersList, TotalenTries, Server);
	WCHAR* EnvpValue[] =
	{
		L"\tNumber of processors: %NUMBER_OF_PROCESSORS%\n"
		L"\tProcessor architecrute: %PROCESSOR_ARCHITECTURE%\n",
		L"\tProcessor identifier: %PROCESSOR_IDENTIFIER%\n",
		L"\tProcessor level: %PROCESSOR_LEVEL%\n",
		L"\tProcessor revision: %PROCESSOR_REVISION%\n",
		L"\tOS : %OS%\n",
		L"\tSYSTEMROOT : %SYSTEMROOT%\n",
		L"\tTEMP : %TEMP%\n",
		L"\tWINDIR : %WINDIR%\n",
		L"\tCOMSPEC : %COMSPEC%\n",
	};
	wchar_t* SysInfo[9][256];
	printf("\nInfo of computer:\n");
	for (int i = 0; i < 9; i++)
	{
		ExpandEnvironmentStringsW(EnvpValue[i], SysInfo[i], 256);
		printf("%ws", SysInfo[i]);
	}
	free(Server);
}


void LocalTime(void)
{
	char dateStr[9];
	char timeStr[9];
	_strdate(dateStr);
	_strtime(timeStr);
	printf("%s", dateStr);
	printf(" %s\n\n", timeStr);
}
int AllInformationGet(LPUSER_INFO_1* UsersList, DWORD* NameSize, DWORD* TotalenTries, WCHAR* Server)
{
	setlocale(LC_ALL, "Rus");
	GetComputerName(Server, NameSize);
	NET_API_STATUS Result;
	DWORD SizeOfInfo = 0;
	PDWORD  resume_handle = 0;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	Result = NetUserEnum(Server, 1, FILTER_NORMAL_ACCOUNT, (LPBYTE*)UsersList, dwPrefMaxLen, &SizeOfInfo, TotalenTries, &resume_handle);
	if (Result != NERR_Success)
		return NULL;
	return 1;
}

int PrintfAllInformationsOfUsers(LPUSER_INFO_1 UsersList, DWORD TotalenTries, WCHAR* Server)
{
	for (int i = 0; i < TotalenTries;i++)
	{
		LPUSER_INFO_1 UserInfo = NULL;
		int Result = NetUserGetInfo(Server, UsersList[i].usri1_name, 1, (LPBYTE)&UserInfo);
		if (Result == NERR_Success)
		{
			wprintf(L"%d)User: %s\n", i+1,UserInfo->usri1_name);
			wprintf(L"\tPassword age: %d\n", UserInfo->usri1_password_age);
			wprintf(L"\tPrivilege level: %d\n", UserInfo->usri1_priv);
			wprintf(L"\tHome directory: %s\n", UserInfo->usri1_home_dir);
			wprintf(L"\tUser comment: %s\n", UserInfo->usri1_comment);
			wprintf(L"\tFlags (in hex): %x\n", UserInfo->usri1_flags);
			wprintf(L"\tScript path: %s\n", UserInfo->usri1_script_path);
		}
		else
			return NULL;
	}
	return 1;
}
