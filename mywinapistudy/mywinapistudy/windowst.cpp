#include "windowst.h"
#include "windows.h"
#include "resource.h"

window::windowst window::windowst::sinwindows;

window::windowst::windowst() noexcept:hinst(GetModuleHandle(nullptr))
{
	//wndClassname = window::classname;
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = getinstance();
	wc.hIcon = LoadIcon(getinstance(), MAKEINTRESOURCE(IDI_ICON1)); 
	wc.hCursor = LoadIcon(getinstance(), MAKEINTRESOURCE(IDI_ICON1));
	wc.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = getname();
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}
window::windowst::~windowst()
{
	UnregisterClass(wndClassname, getinstance());
}
void window::windowst::setname()
{
	//wndClassname=window::transmsg();
}
const char* window::windowst::getname() noexcept
{
	return wndClassname;
}
HINSTANCE window::windowst::getinstance() noexcept
{
	return sinwindows.hinst;
}
window::window(int width, int height, const char* name)  noexcept
{
	//window::classname = claame;
	//windowst::setname();
	//throw std::runtime_error("fdsfdsgd");
	//throw CHWND_EXCEPT(ERROR_ARENA_TRASHED);
	RECT rect;
	rect.left = 100;
	rect.right = width + rect.left;
	rect.top = 100;
	rect.bottom = height + rect.top;
	AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	hwnd = CreateWindow(windowst::getname(), name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,CW_USEDEFAULT, CW_USEDEFAULT,
		width,height, nullptr, nullptr, windowst::getinstance(), this);
	ShowWindow(hwnd, SW_SHOWDEFAULT);
}
window::~window()
{
		DestroyWindow(hwnd);
}

LRESULT CALLBACK window::HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wparem, LPARAM lparam) noexcept
{
	//�ж��Ƿ��ڷ���Ϣ���򴴽������򷵻�Ĭ�ϴ���ʽ
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCT* const pcreat = reinterpret_cast<CREATESTRUCT*>(lparam);
		//lpCreateParams����֮ǰ������this��������ʵ����winapi��������
		window* const pWnd = static_cast<window*>(pcreat->lpCreateParams);
		//�Ѵ���ʵ��������winapi������д洢
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&window::HandleMsgThunk));
		return pWnd->HandleMsg(hwnd, msg, wparem, lparam);

	}
	return DefWindowProc(hwnd, msg, wparem, lparam);

}
LRESULT CALLBACK window::HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wparem, LPARAM lparam) noexcept
{
	window* const pWnd = reinterpret_cast<window*>(GetWindowLongPtr(hwnd,GWLP_USERDATA));
	return pWnd->HandleMsg(hwnd, msg, wparem, lparam);
}
//����ͨ��winapi�������ó�Ա��������˴����˾�̬������������winapi�˵�userdata��ȡ�õ�
LRESULT window::HandleMsg(HWND hwnd, UINT msg, WPARAM wparem, LPARAM lparam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		kbd.OnKeyPressed(static_cast<unsigned char> (wparem));
		MessageBox(nullptr, "WM_KEYDOWN", "OnKeyPressed", MB_OK | MB_ICONEXCLAMATION);
		break;
	case WM_KEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char> (wparem));
		MessageBox(nullptr, "WM_KEYUP", "OnKeyReleased", MB_OK | MB_ICONEXCLAMATION);
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char> (wparem));
		MessageBox(nullptr, "WM_KEYUP", "OnChar", MB_OK | MB_ICONEXCLAMATION);
		break;
	}
	

	return DefWindowProc(hwnd, msg, wparem, lparam);
}

//�����쳣����Ĵ���
std::string window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	const DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,nullptr,hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf),0,nullptr
	);
	if (nMsgLen == 0)
	{
		return "no match error message!!!";
	}
	std::string errorMsg = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorMsg;
}
//ֱ�ӳ�ʼ�������ж�������ݣ�����ʱҲ����ֱ�ӷ���
window::HrException::HrException(int line, const char* file, HRESULT hr)noexcept :Exception(line,file),hr(hr)
{
}

const char* window::HrException::what()const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	//chiliException �ж�����protected���ڴ棬���Թ�����һ�����
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}
const char* window::HrException::GetType() const noexcept
{
	return "window exception";
}
HRESULT window::HrException::GetErrorCode() const noexcept
{
	return hr;
}
std::string window::HrException::GetErrorDescription()const noexcept
{
	return Exception::TranslateErrorCode(hr);
}
const char* window::NoGfxException::GetType() const noexcept
{
	return "Chili Window Exception [No Graphics]";
}