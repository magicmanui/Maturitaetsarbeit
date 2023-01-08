// Maturarbeit_alpha.cpp : Definiert den Einstiegspunkt für die Anwendung.
//

#include "framework.h"
#include "Maturarbeit_alpha.h"
#include <string>
#include "CPU.h"

#define MAX_LOADSTRING 100

// Globale Variablen:
HINSTANCE hInst;                                // Aktuelle Instanz
WCHAR szTitle[MAX_LOADSTRING];                  // Titelleistentext
WCHAR szWindowClass[MAX_LOADSTRING];            // Der Klassenname des Hauptfensters.

CPU cpu(vector<uint8_t>({ 0x18, 0x01, 00, 0x73, 00, 00 }));

// Vorwärtsdeklarationen der in diesem Codemodul enthaltenen Funktionen:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Hier Code einfügen.

    // Globale Zeichenfolgen initialisieren
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MATURARBEITALPHA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Anwendungsinitialisierung ausführen:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MATURARBEITALPHA));

    MSG msg;

    // Hauptnachrichtenschleife:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNKTION: MyRegisterClass()
//
//  ZWECK: Registriert die Fensterklasse.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MATURARBEITALPHA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MATURARBEITALPHA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNKTION: InitInstance(HINSTANCE, int)
//
//   ZWECK: Speichert das Instanzenhandle und erstellt das Hauptfenster.
//
//   KOMMENTARE:
//
//        In dieser Funktion wird das Instanzenhandle in einer globalen Variablen gespeichert, und das
//        Hauptprogrammfenster wird erstellt und angezeigt.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Instanzenhandle in der globalen Variablen speichern

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNKTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ZWECK: Verarbeitet Meldungen für das Hauptfenster.
//
//  WM_COMMAND  - Verarbeiten des Anwendungsmenüs
//  WM_PAINT    - Darstellen des Hauptfensters
//  WM_DESTROY  - Ausgeben einer Beendenmeldung und zurückkehren
//
//
int x = 0;
int y = 0;
int BlockInterrupt = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (!x) {
        cpu.RAM.assign(65536, 0);
        vector<uint8_t> programm1 = { 0x18, 0x01, 00, 0x73, 00, 00 };
        vector<uint8_t> programm2 = { 0x70, 0x08, 0xf0, 00, 00, 00, 00, 0x03, 0xfc, 0xff, 0x47, 01, 0x77, 0xfd, 0xff };
        for (int i = 0; i < 6; ++i) {
            cpu.RAM[i] = programm1[i];
        }
        for (int i = 0xf000; i < 0xf00f; ++i) {
            cpu.RAM[i] = programm2[i - 0xf000];
        }
    }
    PAINTSTRUCT ps;
    HDC hdc;
    x = 1;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Menüauswahl analysieren:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
        if (!y) {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 0));
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(hbrush));
            EndPaint(hWnd, &ps);
            }
            y = 1;
        }
        break;
    case WM_CHAR:
        if (!BlockInterrupt) {
            cpu.RAM[0xfffc] = cpu.a;
            cpu.RAM[0xfffe] = cpu.counter & 255;
            cpu.RAM[0xffff] = cpu.counter >> 8;
            cpu.counter = 0xf000;
            BlockInterrupt = 1;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        if (y) {
            hdc = GetDC(hWnd);
            vector<int> pixel = cpu.step();
            if ((pixel.size() == 2) && ((pixel[0] | pixel[1]) == 0)) {
                BlockInterrupt = 0;
            }
            if (pixel.size() == 4) {
                int p = (pixel[0] << 16) + (pixel[1]) + (pixel[2] << 8);
                if (p % 1056 < 800 && p / 1056 < 600) {
                    SetPixel(hdc, p % 1056, p / 1056, RGB(((pixel[3] >> 4) & 3) * 88, ((pixel[3] >> 2) & 3) * 88, (pixel[3] & 3) * 88));
                }
            }
            
            SendMessageTimeoutA(hWnd, 0, 0, 0, 0, 0, 0);
        }
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Meldungshandler für Infofeld.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
