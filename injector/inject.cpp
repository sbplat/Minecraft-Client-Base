#include <windows.h>

#include <filesystem>
#include <iostream>
#include <string>

#include "instr.h"

std::string window_class = "LWJGL", window_name = "Minecraft 1.8.9";

int getProcessID() {
    OBF_BEGIN
    HWND hWnd = FindWindowExA(NULL, NULL, window_class.c_str(), window_name.c_str());
    DWORD processID;
    GetWindowThreadProcessId(hWnd, &processID);
    std::cout << "Process ID: " << processID << std::endl;
    RETURN(processID)
    OBF_END
}

bool inject(DWORD processID, std::string dllPath) {
    OBF_BEGIN
    HANDLE handle = OpenProcess(N(PROCESS_ALL_ACCESS), false, processID);

    IF (handle == NULL) {
        std::cout << "OpenProcess failed" << std::endl;
        RETURN(false)
    } ELSE {
        std::cout << "Opened process" << std::endl;
    }
    ENDIF

    LPVOID alloc = VirtualAllocEx(handle, NULL, dllPath.size(), N((MEM_COMMIT | MEM_RESERVE)), N(PAGE_EXECUTE_READWRITE));

    IF (alloc == NULL) {
        std::cout << "VirtualAllocEx failed" << std::endl;
        RETURN(false)
    } ELSE {
        std::cout << "Allocated memory" << std::endl;
    }
    ENDIF

    bool writeMemory = WriteProcessMemory(handle, alloc, dllPath.c_str(), dllPath.size(), NULL);

    IF (V(writeMemory) == false) {
        std::cout << "WriteProcessMemory failed" << std::endl;
        RETURN(false)
    } ELSE {
        std::cout << "Wrote to process memory" << std::endl;
    }
    ENDIF

    LPTHREAD_START_ROUTINE loadLibraryAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");

    IF (loadLibraryAddress == NULL) {
        std::cout << "loadLibraryAddress not found" << std::endl;
        RETURN(false)
    } ELSE {
        std::cout << "Found loadLibraryAddress" << std::endl;
    }
    ENDIF

    HANDLE threadHandle = CreateRemoteThread(handle, NULL, N(0), loadLibraryAddress, alloc, N(0), NULL);

    IF (threadHandle == NULL) {
        std::cout << "CreateRemoteThread failed" << std::endl;
        RETURN(false)
    } ELSE {
        std::cout << "Created remote thread" << std::endl;
    }
    ENDIF

    std::cout << "DLL injected" << std::endl;

    RETURN(true)
    OBF_END
}

int main(int argc, char const *argv[]) {
    OBF_BEGIN
    int processID = getProcessID();

    IF (std::filesystem::exists("./loader.dll")) {
        std::cout << "Found loader.dll" << std::endl;
        inject(processID, std::filesystem::absolute("./loader.dll").string());

    } ELSE {
        std::cout << "Could not find loader.dll" << std::endl;
    }
    ENDIF

    system("pause");
    RETURN(0)
    OBF_END
}
