#include "Mouse.h"
//pair是结构体，因此可以使用{}进行赋值
std::pair<int, int> Mouse::GetPos() const noexcept
{
	return { x,y };
}
std::optional<Mouse::RawDelta> Mouse::ReadRawDelta() noexcept
{
	if (rawDeltaBuffer.empty())
	{
		return  std::nullopt;
	}
	const RawDelta m = rawDeltaBuffer.front();
	rawDeltaBuffer.pop();
	return m;
}
int Mouse::GetPosx()const noexcept
{
	return x;
}
int Mouse::GetPosy()const noexcept
{
	return y;
}
bool Mouse::IsInWindow() const noexcept
{
	return isInWindow;
}
bool Mouse::LeftIsPressed() const noexcept
{
	return LeftPressed;
}
bool Mouse::RightIsPressed() const noexcept
{
	return RightPressed;
}
std::optional<Mouse::event> Mouse::Read() noexcept
{
	if (!buffer.empty())
	{
		Mouse::event y = buffer.front();
		buffer.pop();
		return y;
	}
	return{};
}
void Mouse::Flush() noexcept
{
	buffer=std::queue<event> ();
}
void Mouse::EnableRaw() noexcept
{
	rawEnabled = true;
}
void Mouse::DisableRaw() noexcept
{
	rawEnabled = false;
}
bool Mouse::RawEnabled() const noexcept
{
	return rawEnabled;
}
void Mouse::OnMouseMove(int newx, int newy) noexcept
{
	x = newx;
	y = newy;
	buffer.push(Mouse::event(Mouse::event::Type::Move,*this));
	TrimBuffer();
}
void Mouse::OnMouseLeave() noexcept
{
	isInWindow = false;
	buffer.push(Mouse::event(Mouse::event::Type::Leave, *this));
	TrimBuffer();
}
void Mouse::OnMouseEnter() noexcept
{
	isInWindow = true;
	buffer.push(Mouse::event(Mouse::event::Type::Enter, *this));
	TrimBuffer();
}

void Mouse::OnRawDelta(int dx, int dy) noexcept
{
	rawDeltaBuffer.push({ dx,dy });
	TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y) noexcept
{
	LeftPressed = true;

	buffer.push(Mouse::event(Mouse::event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept
{
	LeftPressed = false;

	buffer.push(Mouse::event(Mouse::event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept
{
	RightPressed = true;

	buffer.push(Mouse::event(Mouse::event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept
{
	RightPressed = false;

	buffer.push(Mouse::event(Mouse::event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept
{
	buffer.push(Mouse::event(Mouse::event::Type::WheeelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
	buffer.push(Mouse::event(Mouse::event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer() noexcept
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}

void Mouse::TrimRawInputBuffer() noexcept
{
	while (rawDeltaBuffer.size() > bufferSize)
	{
		rawDeltaBuffer.pop();
	}
}

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
{
	wheelDeltaCarry += delta;
	// generate events for every 120 
	while (wheelDeltaCarry >= WHEEL_DELTA)
	{
		wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (wheelDeltaCarry <= -WHEEL_DELTA)
	{
		wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}