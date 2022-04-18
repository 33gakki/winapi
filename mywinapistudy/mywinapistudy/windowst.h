#pragma once
#include <string>
#include <sstream>
#include <windows.h>
#include "ChiliExceptrion.h"
#include "keyboard.h"
#include "Mouse.h"

#define CHWND_EXCEPT( hr ) window::HrException( __LINE__,__FILE__,(hr) )
#define CHWND_LAST_EXCEPT() window::HrException( __LINE__,__FILE__,GetLastError() )
class window
{
public:
	class Exception :public ChiliExceptrion
	{
		//�������������û����Ա
		using ChiliExceptrion::ChiliExceptrion;
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept;

	};
	//�����̳���Exception����
	class HrException :public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override ;
		const char* GetType() const noexcept override;

		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription()const noexcept;
	private:
		HRESULT hr;
		
	};
	class NoGfxException :public Exception
	{
		using Exception::Exception;
		const char* GetType() const noexcept override;
	};
private:
	//����ģʽ����ʵ����ʵ��
	class windowst
	{
	public:
		static const char* getname() noexcept;
		static HINSTANCE getinstance() noexcept;
		static void setname();
	private:
		windowst() noexcept;
		~windowst();
		//delete��ֹ�����������
		windowst(const windowst&) = delete;
		windowst operator=(const windowst&) = delete;
		static constexpr const char* wndClassname="gfdgfd";
		static windowst sinwindows;
		HINSTANCE hinst;
	};
public:
	window(int width, int height, const char* name) noexcept;
	~window();
	window(const window&) = delete;
	window& operator=(const window&) = delete;
	
	
private:
	static LRESULT WINAPI HandleMsgSetup(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	keyboard kbd;
	Mouse mou;
private:
	int height;
	int width;
	
	HWND hwnd;	
};

