#include "XFtpRMD.h"
#include "testUtil.h"
#include <dirent.h>  // For directory manipulation
#include <unistd.h>  // For rmdir()
#include <sys/stat.h> // For stat()
#include <iostream>
#include <cstring>   // For strerror()
#include <cerrno>    // For errno

void XFtpRMD::Parse(std::string cmd, std::string param) {
    testout("AT XFtpRMD::Parse");

    if (cmd != "RMD") {
        ResCMD("500 Syntax error, command unrecognized.\r\n");
        return;
    }
    std::string command = "RMD ";
    size_t pos = param.find(command);
    if (pos == 0) {
        param = param.substr(command.length());
    }
    // 清除可能的换行符和回车符   为什么会有奇怪的换行 /r
    param.erase(std::remove(param.begin(), param.end(), '\n'), param.end());
    param.erase(std::remove(param.begin(), param.end(), '\r'), param.end());
    std::cout << "Attempting to remove directory: " << param << std::endl;

    // Attempt to remove the directory
    if (removeDirectory(param)) {
        ResCMD("250 Directory removed successfully.\r\n");
    } else {
        std::cerr << "Error removing directory: " << strerror(errno) << std::endl;
        ResCMD("550 Could not remove directory.\r\n");
    }
}

bool XFtpRMD::removeDirectory(const std::string& path) {
    DIR* dir = opendir(path.c_str());
    if (dir == nullptr) {
        return false;  // Failed to open directory
    }

    dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string entryName = entry->d_name;
        if (entryName == "." || entryName == "..") continue;

        std::string fullPath = path + "/" + entryName;
        struct stat statbuf;
        if (stat(fullPath.c_str(), &statbuf) == 0) {
            if (S_ISDIR(statbuf.st_mode)) {
                if (!removeDirectory(fullPath)) {  // Recursively delete directories
                    closedir(dir);
                    return false;
                }
            } else {
                if (unlink(fullPath.c_str()) != 0) {  // Delete file
                    closedir(dir);
                    return false;
                }
            }
        }
    }
    closedir(dir);
    return rmdir(path.c_str()) == 0;  // Remove the now empty directory
}
