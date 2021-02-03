//
// Created by yzqlwt on 2021/1/22.
//
#ifndef DIRWATCHER_HPP_
#define DIRWATCHER_HPP_

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#include <string>

typedef std::function<void(std::string type, std::string filename)> CallBack;

class DirWatcher {

private:
    std::string _file;
    bool _recursive = false;
    CallBack _callback;
public:
    void setDir(const std::string& file){
        _file = file;
    }
    void setRecursive(bool recursive){
        _recursive = recursive;
    }
    void setCallback(CallBack func){
        this->_callback = func;
    }
    void start()
    {
        int		nBufferSize			= 1024;
        char*	buffer				= new char[nBufferSize];
        HANDLE  hDirectoryHandle	= NULL;


        hDirectoryHandle = ::CreateFileA(
                this->_file.c_str(),
                FILE_LIST_DIRECTORY,
                FILE_SHARE_READ
                | FILE_SHARE_WRITE
                | FILE_SHARE_DELETE,
                NULL,
                OPEN_EXISTING,
                FILE_FLAG_BACKUP_SEMANTICS
                | FILE_FLAG_OVERLAPPED,
                NULL);

        if(hDirectoryHandle == INVALID_HANDLE_VALUE)
            return;

        while(1)
        {
            DWORD dwBytes = 0;

            memset(buffer, 0, nBufferSize);

            if(!::ReadDirectoryChangesW(
                    hDirectoryHandle,
                    buffer,
                    nBufferSize,
                    this->_recursive,
                    FILE_NOTIFY_CHANGE_LAST_WRITE,
                    &dwBytes,
                    NULL,
                    NULL) || GetLastError() == ERROR_INVALID_HANDLE)
            {
                break;
            }

            if(!dwBytes)
            {
                printf("Buffer overflow~~rn");
            }

            PFILE_NOTIFY_INFORMATION record = (PFILE_NOTIFY_INFORMATION)buffer;
            DWORD cbOffset = 0;

            do
            {
                switch (record->Action)
                {
                    case FILE_ACTION_ADDED:
                        printf("FILE_ACTION_ADDED:");
                        break;
                    case FILE_ACTION_REMOVED:
                        printf("FILE_ACTION_REMOVED:");
                        break;
                    case FILE_ACTION_MODIFIED:
                        printf("FILE_ACTION_MODIFIED:");
                        break;
                    case FILE_ACTION_RENAMED_OLD_NAME:
                        printf("FILE_ACTION_RENAMED_OLD_NAME:");
                        break;

                    case FILE_ACTION_RENAMED_NEW_NAME:
                        printf("FILE_ACTION_RENAMED_NEW_NAME:");
                        break;

                    default:
                        break;
                }

                char fileBuffer[512];
                WideCharToMultiByte(CP_ACP, 0, record->FileName, record->FileNameLength, fileBuffer, record->FileNameLength, NULL, NULL);
                if (record->Action == FILE_ACTION_MODIFIED){
                    this->_callback("modify", fileBuffer);
                }
                std::cout << fileBuffer << std::endl;
                cbOffset = record->NextEntryOffset;
                record = (PFILE_NOTIFY_INFORMATION)((LPBYTE) record + cbOffset);
            }while(cbOffset);
        }

        delete buffer;

        if(hDirectoryHandle)
            CloseHandle(hDirectoryHandle);
    }
};

#endif

#endif //DIRWATCHER_HPP_
