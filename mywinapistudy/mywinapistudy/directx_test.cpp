#include <Windows.h>
#include "WindowsMessageMap.h"
#include <sstream>
#include "windowst.h"
LRESULT CALLBACK wndproc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	static WindowsMessageMap mm;
	
	OutputDebugString( mm(msg, lparam, wparam).c_str());
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(69);
		break;
	case WM_KEYDOWN:
		if (wparam == 'F');
		{
			SetWindowText(hwnd,"keydown");
		}
		break;
	case WM_KEYUP:
		if (wparam == 'F');
		{
			SetWindowText(hwnd, "keyup");
		}
		break;
	case WM_CHAR:
	{
		static std::string title;
		title.push_back((char)wparam);
		SetWindowText(hwnd, title.c_str());
		break;
	}
		
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		std::ostringstream otsting;
		otsting << "(" << pt.x << "," << pt.y << ")";
		SetWindowText(hwnd, otsting.str().c_str());
		break;
	}
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}
int CALLBACK WinMain( HINSTANCE hinstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	/*
	const auto pclassname = "hw3dbutts";
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = wndproc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pclassname;
	wc.hIconSm = nullptr;

	RegisterClassEx(&wc);

	HWND hd;
	hd = CreateWindowExA(0,pclassname,"windows test",WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU,200,200,640,480,nullptr,nullptr
	,hinstance,nullptr);
	ShowWindow(hd, SW_SHOW);

	
	*/
	try {
		
		
	}
	catch (const ChiliExceptrion& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "std message error", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "no detail about", "unknow Exception", MB_OK | MB_ICONEXCLAMATION);

	}
	window mywin1(720, 640, "yang1");
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	return 0;
}
