#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <vector>
#include <stdlib.h>
#include <string>

using namespace std; //fuk you

const char* windowName = ""; //window name
char gameModule1[] = ""; //client name


class GetInfo {
public:
    void Clear() noexcept
    {
        system("cls || clear");
    }

    DWORD GetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID) //method to get module base addr
    {
        DWORD dwModuleBaseAddress = 0;
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
        MODULEENTRY32 ModuleEntry32 = { 0 };
        ModuleEntry32.dwSize = sizeof(MODULEENTRY32);


        if (Module32First(hSnap, &ModuleEntry32))
        {
            do {
                if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0)
                {
                    dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
                    break;
                }

            } while (Module32Next(hSnap, &ModuleEntry32));
        }
        CloseHandle(hSnap);
        return dwModuleBaseAddress;

    }


    DWORD GetPointerAddress(HWND hwnd, DWORD gameBaseAddr, DWORD address, vector<DWORD> offsets) //snag dat pointer addr
    {
        DWORD pID = NULL;
        GetWindowThreadProcessId(hwnd, &pID);
        HANDLE pHandle = NULL;
        pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);

        if (pHandle == INVALID_HANDLE_VALUE || pHandle == NULL);

        DWORD offset_null = NULL;
        ReadProcessMemory(pHandle, (LPVOID*)(gameBaseAddr + address), &offset_null, sizeof(offset_null), 0);
        DWORD pointerAddress = offset_null;


        for (int i = 0; i < offsets.size() - 1; i++)
        {
            ReadProcessMemory(pHandle, (LPVOID*)(pointerAddress + offsets.at(i)), &pointerAddress, sizeof(pointerAddress), 0);
        }
        return pointerAddress += offsets.at(offsets.size() - 1);

    }
};


int main(void)
{
    GetInfo* hack = new GetInfo();
    HWND hwnd_AC = FindWindowA(NULL, windowName);

    if (hwnd_AC != FALSE);

    DWORD pID = NULL;
    GetWindowThreadProcessId(hwnd_AC, &pID);
    HANDLE pHandle = NULL;

    pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (pHandle == INVALID_HANDLE_VALUE || pHandle == NULL);

    DWORD gamebaseaddress1 = hack->GetModuleBaseAddress(_T(gameModule1), pID);

    //ptr and offset from assault cube
    DWORD healthAddr = 0x00109B74; //replace
    vector<DWORD> healthOffsets{ 0xF8 }; // replace with offsets 

    DWORD healthPtrAddr = hack->GetPointerAddress(hwnd_AC, gamebaseaddress1, healthAddr, healthOffsets);
    
    int health = 1000; //set value you want to write
    WriteProcessMemory(pHandle, (LPVOID*)(healthPtrAddr), &health, 4, 0); //write to memory

    

}
