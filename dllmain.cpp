// dllmain.cpp : Defines the entry point for the DLL application.
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h> 
#include "Pattern16.h"
#include <tlhelp32.h>
#include <psapi.h>

bool GetPid(const wchar_t* targetProcess, DWORD* procID)
{
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap && snap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(pe);
        if (Process32First(snap, &pe))
        {
            do
            {
                if (!wcscmp(pe.szExeFile, targetProcess))
                {
                    CloseHandle(snap);
                    *procID = pe.th32ProcessID;
                    return true;
                }
            } while (Process32Next(snap, &pe));
        }
    }
    return false;
}

char* GetModuleBase(const wchar_t* ModuleName, DWORD procID)
{
    MODULEENTRY32 ModuleEntry = { 0 };
    HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);

    if (!SnapShot) return NULL;

    ModuleEntry.dwSize = sizeof(ModuleEntry);

    if (!Module32First(SnapShot, &ModuleEntry)) return NULL;

    do
    {
        if (!wcscmp(ModuleEntry.szModule, ModuleName))
        {
            CloseHandle(SnapShot);
            return (char*)ModuleEntry.modBaseAddr;
        }
    } while (Module32Next(SnapShot, &ModuleEntry));

    CloseHandle(SnapShot);
    return NULL;
}

uintptr_t FindDmaAddy(int PointerLevel, HANDLE hProcHandle, uintptr_t Offsets[], uintptr_t BaseAddress)
{
    uintptr_t pointer = BaseAddress;
    uintptr_t pTemp = 0;

    uintptr_t pointerAddr = 0;
    for (int i = 0; i < PointerLevel; i++)
    {
        if (i == 0)
        {
            ReadProcessMemory(hProcHandle, (LPCVOID)pointer, &pTemp, sizeof(pTemp), NULL);
        }
        pointerAddr = pTemp + Offsets[i];

        ReadProcessMemory(hProcHandle, (LPCVOID)pointerAddr, &pTemp, sizeof(pTemp), NULL);
    }
    return pointerAddr;
}

DWORD WINAPI Cons()
{
    /*
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    */
    /*
    DWORD ProcId;
    GetPid(L"stormworks64.exe", &ProcId);
    char* ExeBaseAddress = GetModuleBase(L"stormworks64.exe", ProcId);
    */

    HANDLE ExeHandle = GetModuleHandleA("stormworks64.exe");

    void* startAdress = (char*)ExeHandle;
    void* endAdress = (char*)ExeHandle + (0xCD9000);

    void* regionStart = (void*)startAdress;
    size_t regionSize = (DWORD)endAdress;
    std::string signature = "74 20 FF C3 E8 A5 FE FF FF";
    void* address = Pattern16::find(regionStart, regionSize, signature);


    //DWORD add = FindDmaAddy(0, ExeBaseAddress,0,(DWORD)ExeBaseAddress);
    /*
    std::cout << startAdress << std::endl;
    std::cout << endAdress << std::endl;
    */
    std::cout << address << std::endl;
    char* name = (char*)((char*)address);
    std::cout << name << std::endl;

    char* buffer = reinterpret_cast<char*>(address);


    char myStr[9] = { 0x75 , 0x20, 0xFF, 0xC3, 0xE8, 0xA5, 0xFE, 0xFF, 0xFF };
    //size_t length = strlen(myStr);
    size_t length = 9;

    DWORD oldProtect;
    VirtualProtect(buffer, length, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(buffer, myStr, length);
    VirtualProtect(buffer, length, oldProtect, nullptr);

    //strcpy((char*)name, "#version460");






    /*
    while (1)
    {
        Sleep(100);
        if (GetAsyncKeyState(VK_NUMPAD0))
            break;
    }
    fclose(fp);
    FreeConsole();
    */

    return 0;

}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Cons, NULL, 0, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

