// dllmain.cpp : Defines the entry point for the DLL application.
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include <iostream>
#include <Windows.h> 
#include <tlhelp32.h>
#include <psapi.h>
#include "sigmatch/sigmatch.hpp"



using namespace sigmatch_literals;

DWORD WINAPI Cons()
{
    /*
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    */

    sigmatch::this_process_target target;
    sigmatch::search_context context = target.in_module("stormworks64.exe");
    sigmatch::search_result result = context.search("48 39 34 D0 74 07"_sig);


    for (const std::byte* address : result.matches()) {
        //std::cout << "matched: " << address << '\n';

        int* addr;
        addr = (int*)address;

        char* buffer;
        //buffer = reinterpret_cast<char*>(addr);
        buffer = (char*)addr;

        
        char myStr[6] = { 0x48 , 0x39, 0x34, 0xD0, 0x75, 0x07 };
        //size_t length = strlen(myStr);
        size_t length = 6;
        
        /*
        const int size = 10;
        char myStr[size] = { 0x73, 0x61 ,0x6D ,0x70 ,0x6C ,0x65 ,0x20 ,0x6E ,0x65 ,0x77 };
        //size_t length = strlen(myStr);
        size_t length = size;
        */
        //std::cout << "matched buffer: " << buffer << '\n';

        
        DWORD oldProtect;
        VirtualProtect(buffer, length, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy(buffer, myStr, length);
        VirtualProtect(buffer, length, oldProtect, nullptr);
        
    }
    
   
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

