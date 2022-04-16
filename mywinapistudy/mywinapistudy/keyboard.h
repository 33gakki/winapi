#pragma once
#include <queue>
#include <bitset>
#include <optional>
class keyboard
{
	//声明window类可以任意读取本类中的数据
	friend class window;
public:
	class event
	{
	public:
		enum class Type
		{
			Press,
		    Release,
		};
	private:
		Type type;
		unsigned char code;
	public:
		event(Type type, unsigned char code) noexcept: type(type), code(code) {}
		bool isPressed()const noexcept
		{
			return type == Type::Press;
		}
		bool IsRelease()const noexcept
		{
			return type == Type::Release;
		}
		unsigned char GetCode() const noexcept
		{
			return code;
		}
	};
	//当无参时可以调用默认构造，防止自定构造时，默认构造被删除
	keyboard() = default;
	//= delete显示禁用此函数
	keyboard(const keyboard&) = delete;

	keyboard& operator=(keyboard&) noexcept =delete;

	bool KeyIsPressed(unsigned char keycode) const;
	std::optional<event> ReadKey() noexcept;
	bool KeyIsEmpty() const noexcept;
	

	std::optional<char> ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void FlushKey() noexcept;
	void FlushChar() noexcept;
	void Flush() noexcept;

	void EnableAutorepeat() noexcept;
	void DisableAutorepeat() noexcept;
	bool AutorepeatIsEnabled()const noexcept;

private:
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;
private:
	//256u表示第九位
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool autorepeatEnabled = false;
	std::bitset<nKeys> keystates;
	std::queue<event> keybuffer;
	std::queue<char> charbuffer;

};

