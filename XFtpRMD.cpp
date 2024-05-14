#include "XFtpRMD.h"
#include "testUtil.h"
//#include <dirent.h>  // For directory manipulation
//#include <unistd.h>  // For rmdir()
//#include <sys/stat.h> // For stat()

#include <iostream>
#include <cstring>   // For strerror()
#include <cerrno>    // For errno
#include <filesystem>

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
    trimTrailingNewlines(param);
    std::cout << "Attempting to remove directory: " << param << std::endl;

    // Attempt to remove the directory
    std::error_code ec;
    filesystem::remove_all(filesystem::path(param), ec);
    if (!ec) {
        ResCMD("250 Directory removed successfully.\r\n");
    } else {
        std::cerr << "Error removing directory: " << ec.message() << std::endl;
        ResCMD("550 Could not remove directory.\r\n");
    }
    
//    // Attempt to remove the directory
//    if (removeDirectory(param)) {
//        ResCMD("250 Directory removed successfully.\r\n");
//    } else {
//        std::cerr << "Error removing directory: " << strerror(errno) << std::endl;
//        ResCMD("550 Could not remove directory.\r\n");
//    }
}


