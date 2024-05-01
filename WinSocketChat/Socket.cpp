#include "Socket.h"

SOCKET _socket;

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_BUTTON1: {
            const int MAXSTRLEN = 255;
            char buf[MAXSTRLEN];
            char str[MAXSTRLEN];

            // получаем текст из IDC_EDIT1
            GetDlgItemText(hwndDlg, IDC_EDIT1, str, MAXSTRLEN);

            // подключаемся к серверу
            WSADATA wsaData;
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                MessageBox(NULL, "WSAStartup failed", "Error", MB_OK | MB_ICONERROR);
                return TRUE;
            }

            _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (_socket == INVALID_SOCKET) {
                MessageBox(NULL, "Socket creation failed", "Error", MB_OK | MB_ICONERROR);
                WSACleanup();
                return TRUE;
            }

            sockaddr_in addr;
            addr.sin_family = AF_INET;
            inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
            addr.sin_port = htons(20000);

            if (connect(_socket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
                MessageBox(NULL, "Connection failed", "Error", MB_OK | MB_ICONERROR);
                closesocket(_socket);
                WSACleanup();
                return TRUE;
            }

            // отправляем текст на сервер
            send(_socket, str, strlen(str), 0);

            // получаем ответ от сервера
            int i = recv(_socket, buf, MAXSTRLEN, 0);
            buf[i] = '\0';

            MessageBox(NULL, buf, "Message from Server", MB_OK | MB_ICONINFORMATION);

            // закрываем соединение и очищаем Winsock
            closesocket(_socket);
            WSACleanup();

            return TRUE;
        }
        }
        break;
    case WM_CLOSE:
        EndDialog(hwndDlg, 0);
        break;
    }
    return FALSE;
}

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow) {
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DialogProc);
}