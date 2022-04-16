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
	//判断是否在非消息区域创建，否则返回默认处理方式
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCT* const pcreat = reinterpret_cast<CREATESTRUCT*>(lparam);
		//lpCreateParams就是之前创建的this，将窗口实例与winapi关联起来
		window* const pWnd = static_cast<window*>(pcreat->lpCreateParams);
		//把窗口实例数据在winapi方面进行存储
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
//不能通过winapi函数调用成员函数，因此创建了静态函数，数据是winapi端的userdata中取得的
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

//关于异常处理的代码
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
//直接初始化基类中定义的数据，访问时也可以直接访问
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
	//chiliException 中定义了protected的内存，可以供子类一起访问
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