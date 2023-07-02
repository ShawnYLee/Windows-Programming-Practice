#include <iostream>
#include <Windows.h>
#include <string>
using namespace std;

#define PIPE_NAME L"\\\\.\\pipe\\PlayerRegistrationPipe"

bool StartServer()
{
    HANDLE hPipe = CreateNamedPipeW(
        PIPE_NAME,                // Pipe name
        PIPE_ACCESS_DUPLEX,       // Pipe open mode
        PIPE_TYPE_MESSAGE |       // Pipe mode
        PIPE_READMODE_MESSAGE |   // Pipe mode
        PIPE_WAIT,                // Pipe mode
        PIPE_UNLIMITED_INSTANCES, // Max instances
        0,                        // Output buffer size
        0,                        // Input buffer size
        0,                        // Timeout
        nullptr                   // Security attributes
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create named pipe. Error code: " << GetLastError() << std::endl;
        return false;
    }

    std::cout << "Waiting for client connection..." << std::endl;

    if (!ConnectNamedPipe(hPipe, nullptr)) {
        std::cout << "Failed to connect to client. Error code: " << GetLastError() << std::endl;
        CloseHandle(hPipe);
        return false;
    }

    std::cout << "Client connected. Waiting for registration data..." << std::endl;

    std::string playerName;
    char buffer[256];
    DWORD bytesRead;

    // Read the player name from the client
    if (ReadFile(hPipe, buffer, sizeof(buffer), &bytesRead, nullptr)) {
        playerName = std::string(buffer, bytesRead);

        // Perform registration logic here...
        std::cout << "Registering player: " << playerName << std::endl;

        // Send registration complete message to the client
        std::string message = "Registration complete.";
        if (!WriteFile(hPipe, message.c_str(), message.size() + 1, &bytesRead, nullptr)) {
            std::cout << "Failed to send registration complete message. Error code: " << GetLastError() << std::endl;
        }
    }
    else {
        std::cout << "Failed to read player name. Error code: " << GetLastError() << std::endl;
    }

    CloseHandle(hPipe);
    return true;
}

bool StartClient(const std::string& playerName)
{
    HANDLE hPipe = CreateFileW(
        PIPE_NAME,                  // Pipe name
        GENERIC_READ | GENERIC_WRITE,  // Access mode
        0,                          // Share mode
        nullptr,                    // Security attributes
        OPEN_EXISTING,              // Creation disposition
        0,                          // Flags and attributes
        nullptr                     // Template file handle
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to connect to server. Error code: " << GetLastError() << std::endl;
        return false;
    }

    std::cout << "Connected to server. Sending registration data..." << std::endl;

    DWORD bytesWritten;

    // Send player name to the server
    if (!WriteFile(hPipe, playerName.c_str(), playerName.size() + 1, &bytesWritten, nullptr)) {
        std::cout << "Failed to send player name. Error code: " << GetLastError() << std::endl;
        CloseHandle(hPipe);
        return false;
    }

    char buffer[256];
    DWORD bytesRead;

    // Read the registration complete message from the server
    if (ReadFile(hPipe, buffer, sizeof(buffer), &bytesRead, nullptr)) {
        std::string message(buffer, bytesRead);
        std::cout << "Registration complete: " << message << std::endl;
    }
    else {
        std::cout << "Failed to read registration complete message. Error code: " << GetLastError() << std::endl;
    }

    CloseHandle(hPipe);
    return true;
}

int main(int argc, char* argv[])
{
    
    std::string mode = "";
    std::string playerName = "";
    std::cout << "input mode(server/client): ";
    std::cin >> mode;
    

    if (mode == "server") {
        if (!StartServer()) {
            cout << "StartServer failed" << endl;
        }
    }
    else if (mode == "client") {
        cout << "input player's name: ";
        cin >> playerName;
        if (!StartClient(playerName)) {
            cout << "StartClient failed" << endl;
        }
    }
    else {
        std::cout << "Invalid mode. Use 'server' or 'client'." << std::endl;
        return 1;
    }

    return 0;
}
