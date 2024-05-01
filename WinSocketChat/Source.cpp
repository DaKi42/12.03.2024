//Main.cpp
#include <iostream>
#include "Socket.h"

using namespace std;

HWND hwndEdit;
WSADATA wsaData;
SOCKET _socket;

void StartServer() {
    const int MAXSTRLEN = 255;
    SOCKET acceptSocket;
    sockaddr_in addr;

    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_socket == INVALID_SOCKET) {
        MessageBox(NULL, "Socket creation failed", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);
    addr.sin_port = htons(20000);

    if (bind(_socket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        MessageBox(NULL, "Bind failed", "Error", MB_OK | MB_ICONERROR);
        closesocket(_socket);
        return;
    }

    if (listen(_socket, 1) == SOCKET_ERROR) {
        MessageBox(NULL, "Listen failed", "Error", MB_OK | MB_ICONERROR);
        closesocket(_socket);
        return;
    }

    MessageBox(NULL, "Server is started", "Success", MB_OK | MB_ICONINFORMATION);

    acceptSocket = accept(_socket, NULL, NULL);
    if (acceptSocket == INVALID_SOCKET) {
        MessageBox(NULL, "Accept failed", "Error", MB_OK | MB_ICONERROR);
        closesocket(_socket);
        return;
    }

    char buf[MAXSTRLEN];
    int i = recv(acceptSocket, buf, MAXSTRLEN, 0);
    buf[i] = '\0';
    MessageBox(NULL, buf, "Message from Client", MB_OK | MB_ICONINFORMATION);

    const char* text = "Hello from server!";
    send(acceptSocket, text, strlen(text), 0);

    closesocket(acceptSocket);
    closesocket(_socket);
}

BOOL CALLBACK DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        hwndEdit = GetDlgItem(hwndDlg, IDC_EDIT1);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_BUTTON1:
            StartServer();
            break;
        }
        break;
    case WM_CLOSE:
        EndDialog(hwndDlg, 0);
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow) {
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}
