
#include <iostream>
#include <string>
#include <locale>
#include <windows.h>
#include <stdio.h>
using namespace std;

int main()
{
    HANDLE hFile;
    char DataBuffer[] = "This is some test data to write to the file.";
    DWORD dwBytesToWrite = (DWORD)strlen(DataBuffer);
    DWORD dwBytesWritten = 0;
    BOOL bErrorFlag = FALSE;
   
    hFile = CreateFileW(L"Test123",  // name of the write       
        GENERIC_WRITE,          // open for writing
        0,                      // do not share
        NULL,                   // default security
        CREATE_NEW,             // create new file only
        FILE_ATTRIBUTE_NORMAL,  // normal file
        NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("CreateFile failed (%d)\n", GetLastError());
        return 1;
    }
    bErrorFlag = WriteFile(
        hFile,           // open file handle
        DataBuffer,      // start of data to write
        dwBytesToWrite,  // number of bytes to write
        &dwBytesWritten, // number of bytes that were written
        NULL);            // no overlapped structure

    if (FALSE == bErrorFlag)
    {
        printf("Terminal failure: Unable to write to file.\n");
    }
    //WriteFileEx(hFile,DataBuffer,)
    OVERLAPPED sOverlapped;
    sOverlapped.Offset = sizeof(DataBuffer) * 3;
    sOverlapped.OffsetHigh = 0;
    BOOL result = LockFileEx(hFile, LOCKFILE_FAIL_IMMEDIATELY, 0, 0, 0, &sOverlapped);
    if (!result)
    {
        // Handle the error.
        printf("LockFileEx failed (%d)\n", GetLastError());
        
    }
    else printf("LockFileEx succeeded\n");
    Sleep(10000);
}


