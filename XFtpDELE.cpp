#if __cplusplus != 201703L
#error "This project requires C++17 support"
#endif

#include "XFtpDELE.h"
#include "testUtil.h"
#include <iostream>
#include <filesystem> // Include the filesystem header

namespace fs = std::filesystem; // Create a namespace alias for std::filesystem

void XFtpDELE::Parse(std::string cmd, std::string param) {
    testout("AT XFtpDELE::Parse");
    
    // Checking if the command is indeed a DELE command
    if (cmd != "DELE") {
        ResCMD("500 Syntax error, command unrecognized.\r\n");
        return;
    }
    std::string command = "DELE ";
    size_t pos = param.find(command);
    if (pos == 0) {
        param = param.substr(command.length());
    }
    // 清除可能的换行符和回车符   为什么会有奇怪的换行 /r
    param.erase(std::remove(param.begin(), param.end(), '\n'), param.end());
    param.erase(std::remove(param.begin(), param.end(), '\r'), param.end());
    // Log the attempt to delete the file   std::filesystem::current_path()
    std::cout << std::filesystem::current_path() << std::endl;
    std::cout << "Attempting to delete file: " << param << std::endl;

    // Check if file exists and if it can be deleted using std::filesystem
    if (fs::exists(param)) {
        // Attempt to delete the file using std::filesystem
        try {
            if (fs::remove(param)) {
                // Send success response if the file was deleted
                ResCMD("250 File deleted successfully.\r\n");
            } else {
                // Send error response if the file could not be deleted
                ResCMD("550 Could not delete the file.\r\n");
            }
        } catch (const fs::filesystem_error& e) {
            // If an error occurs during deletion, send an error response
            std::cerr << "Error deleting file: " << e.what() << std::endl;
            ResCMD("550 Could not delete the file.\r\n");
        }
    }else {
        std::cout << "File not found. Checking permissions and links:" << std::endl;
        // Only works on Unix-like systems
        system(("ls -l " + param).c_str());
        cout<< "-------" <<endl;
//        system(("readlink -f " + param).c_str());
        ResCMD("550 File not found.\r\n");
        return;
    }
}
