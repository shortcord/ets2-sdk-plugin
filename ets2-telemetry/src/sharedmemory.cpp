#include "sharedmemory.hpp"

#ifndef _WIN32
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

void SharedMemory::LogError(const char *logPtr)
{
#ifdef SHAREDMEM_LOGGING
	if (this->logFilePtr == NULL)
	{
		this->logFilePtr = fopen(SHAREDMEM_FILENAME, "a");
	}
	if (this->logFilePtr != NULL)
	{
		fprintf(this->logFilePtr, "%s\r\n", logPtr);
#ifdef _WIN32
		fprintf(this->logFilePtr, "Windows Error code: %d\r\n\r\n", GetLastError());
#else
		fprintf(this->logFilePtr, "POSIX Error code: %d (%s)\r\n\r\n", errno, strerror(errno));
#endif
	}
#endif
}

SharedMemory::SharedMemory(SharedMemoryName namePtr, unsigned int size)
{
	this->mapsize = size;
	this->namePtr = namePtr;
	this->isSharedMemoryHooked = false;
	this->pBufferPtr = NULL;
#ifdef _WIN32
	this->hMapFile = NULL;
#else
	this->hMapFile = -1;
#endif
#ifdef SHAREDMEM_LOGGING
	this->logFilePtr = NULL;
#endif

#ifdef _WIN32
    hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE, // use paging file
            NULL, // default security
            PAGE_READWRITE, // read/write access
            0, // maximum object size (high-order DWORD)
            size, // maximum object size (low-order DWORD)
            namePtr); // name of mapping object
	LogError("Created file map");
    if (hMapFile == NULL)
    {
		LogError("but it's NULL!");
        if(GetLastError() == (DWORD)183) // already exists
        {
                hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, namePtr);
                if (hMapFile == NULL)
                {
                        LogError("Could not open existing file mapping");
                        return;
                }
        }
        else
        {
                LogError("Could not create file mapping object");
                return;
        }
    }
	else
	{
		LogError("and it's not NULL!");
	}

    this->pBufferPtr = (void*) MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, size);
    if (this->pBufferPtr == NULL)
    {
        LogError("Could not reserve buffer for shared memory");
        CloseHandle(hMapFile);
    }
	else
	{
		memset(this->pBufferPtr, 0, size);
		this->isSharedMemoryHooked = true;
		LogError("Opened MMF");
	}
#else
    hMapFile = shm_open(namePtr, O_CREAT | O_RDWR, 0666);
    if (hMapFile == -1)
    {
        LogError("Could not create shared memory object");
        return;
    }

    if (ftruncate(hMapFile, size) == -1)
    {
        LogError("Could not size shared memory object");
        close(hMapFile);
        hMapFile = -1;
        return;
    }

    this->pBufferPtr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, hMapFile, 0);
    if (this->pBufferPtr == MAP_FAILED)
    {
        this->pBufferPtr = NULL;
        LogError("Could not map shared memory object");
        close(hMapFile);
        hMapFile = -1;
        return;
    }

    memset(this->pBufferPtr, 0, size);
    this->isSharedMemoryHooked = true;
    LogError("Opened POSIX shared memory");
#endif
		
}


void SharedMemory::Close(void)
{
#ifdef SHAREDMEM_LOGGING
		if (logFilePtr != NULL)
		{
			fclose(logFilePtr);
			logFilePtr = NULL;
		}
#endif
        if (isSharedMemoryHooked)
        {
#ifdef _WIN32
                if (pBufferPtr != NULL) UnmapViewOfFile(pBufferPtr);
                if (hMapFile != NULL) CloseHandle(hMapFile);
                hMapFile = NULL;
#else
                if (pBufferPtr != NULL) munmap(pBufferPtr, mapsize);
                if (hMapFile != -1) close(hMapFile);
                if (namePtr != NULL) shm_unlink(namePtr);
                hMapFile = -1;
#endif
        }

        pBufferPtr = NULL;
        isSharedMemoryHooked = false;

}
