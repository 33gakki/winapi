#include "keyboard.h"
bool keyboard::KeyIsPressed(unsigned char keycode) const
{
	//返回某一位是否为真
	return keystates[keycode];
}
//optional表示包含某元素则返回不包含则返回为空
std::optional<keyboard::event> keyboard::ReadKey() noexcept
{
	if (keybuffer.size() > 0u)
	{
		keyboard::event e = keybuffer.front();
		keybuffer.pop();
		return e;
	}
	return {};
}
bool keyboard::KeyIsEmpty() const noexcept
{
	return keybuffer.size();
}

std::optional<char> keyboard::ReadChar() noexcept
{
	if (charbuffer.size() > 0u)
	{
		unsigned char cha = charbuffer.front();
		charbuffer.pop();
		return cha;
	}
	return {};
}
bool keyboard::CharIsEmpty() const noexcept
{
	return charbuffer.empty();
}
void keyboard::FlushKey() noexcept
{
	keybuffer = std::queue<event> ();
}
void keyboard::FlushChar() noexcept
{
	charbuffer = std::queue<char>();
}
void keyboard::Flush() noexcept
{
	FlushKey();
	FlushChar();
}

void keyboard::EnableAutorepeat() noexcept
{
	autorepeatEnabled = true;
}

void keyboard::DisableAutorepeat() noexcept
{
	autorepeatEnabled = false;
}

bool keyboard::AutorepeatIsEnabled() const noexcept
{
	return autorepeatEnabled;
}
void keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
	keystates[keycode] = true;
	keybuffer.push(keyboard::event(keyboard::event::Type::Press, keycode));
}
void keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
	keystates[keycode] = false;
	keybuffer.push(keyboard::event(keyboard::event::Type::Release, keycode));
}
void keyboard::OnChar(char character) noexcept
{
	charbuffer.push(character);
	TrimBuffer(charbuffer);
}
void keyboard::ClearState() noexcept
{
	keystates.reset();
}
template<typename T>
void keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
	if (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}