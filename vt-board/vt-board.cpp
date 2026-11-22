// vt-board.cpp : アプリケーションのエントリポイントを定義します。

#include "framework.h"
#include "vt-board.h"
#include "app.h"

#define MAX_LOADSTRING 100
#define WIDTH 1280
#define HEIGHT 720

// グローバル変数:
HINSTANCE hInst;  // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];  // タイトルバーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];  // メインウィンドウクラス名
// ウィンドウのリサイズ
BOOL scheduleResize;
UINT scheduleResizeWidth;
UINT scheduleResizeHeight;

// このコードモジュールに含まれる関数の宣言を転送します:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(
  _In_ HINSTANCE hInstance,
  _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPWSTR lpCmdLine,
  _In_ int nCmdShow)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // グローバル文字列を初期化する
  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_VTBOARD, szWindowClass, MAX_LOADSTRING);

  // ウィンドウクラスを登録します。
  WNDCLASSEXW wcex;
  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VTBOARD));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_VTBOARD);
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
  RegisterClassExW(&wcex);

  // アプリケーション初期化の実行
  // 目的: インスタンスハンドルを保存して、メインウィンドウを作成します
  // コメント:
  //   この関数で、グローバル変数でインスタンスハンドルを保存し、
  //   メインプログラムウィンドウを作成および表示します。
  hInst = hInstance;  // グローバル変数にインスタンスハンドルを格納する
  RECT windowRect = { 0, 0, static_cast<LONG>(WIDTH), static_cast<LONG>(HEIGHT) };
  AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
  LONG width = windowRect.right - windowRect.left;
  LONG height = windowRect.bottom - windowRect.top;
  HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, hInstance, nullptr);
  if (!hWnd)
  {
    return FALSE;
  }
  ShowWindow(hWnd, nCmdShow);

  App app = {};
  app.OnInit(hWnd, static_cast<UINT>(width), static_cast<UINT>(height));

  HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VTBOARD));

  MSG msg{};

  // メインイベントループ:
  while (msg.message != WM_QUIT)
  {
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
      if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
    else
    {
      if (scheduleResize)
      {
        app.OnResize(scheduleResizeWidth, scheduleResizeHeight);
        scheduleResize = FALSE;
      }

      app.OnUpdate();
      app.OnRender();
    }
  }

  app.OnDestroy();

  return static_cast<int>(msg.wParam);
}

//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//  目的: メインウィンドウのメッセージを処理します。
//  WM_COMMAND  - アプリケーションメニューの処理
//  WM_DESTROY  - 中止メッセージを表示して戻る
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
    case WM_COMMAND:
    {
      int wmId = LOWORD(wParam);

      // 選択されたメニューの解析:
      switch (wmId)
      {
        case IDM_ABOUT:
        {
          DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
          break;
        }

        case IDM_EXIT:
        {
          DestroyWindow(hWnd);
          break;
        }

        default:
        {
          return DefWindowProc(hWnd, message, wParam, lParam);
        }
      }
      break;
    }

    case WM_SIZE:
    {
      scheduleResizeWidth = LOWORD(lParam);
      scheduleResizeHeight = HIWORD(lParam);
      scheduleResize = TRUE;
      break;
    }

    case WM_DESTROY:
    {
      PostQuitMessage(0);
      break;
    }

    default:
    {
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
  }

  return 0;
}

// バージョン情報ボックスのメッセージハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  UNREFERENCED_PARAMETER(lParam);

  switch (message)
  {
    case WM_INITDIALOG:
    {
      return static_cast<INT_PTR>(TRUE);
    }

    case WM_COMMAND:
    {
      if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
      {
        EndDialog(hDlg, LOWORD(wParam));
        return static_cast<INT_PTR>(TRUE);
      }
      break;
    }
  }

  return static_cast<INT_PTR>(FALSE);
}
