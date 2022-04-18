#pragma once
#include<queue>
#include<optional>
#include<windows.h>

class Mouse
{
	friend class window;
	struct RawDelta
	{
		int x, y;
	};
	class event
	{
	public:
		enum class Type {
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheeelUp,
			WheelDown,
			Move,
			Enter,
			Leave
		};
	private:
		Type type;
		bool leftIsPressed;
		bool rightIsPressed;
		int x, y;
	public:
		event(Type type, const Mouse& parent)noexcept :type(type), leftIsPressed(parent.LeftPressed), rightIsPressed(parent.RightPressed),
			x(parent.x), y(parent.y) {}
		Type GetType()const noexcept
		{
			return type;
		}
		std::pair<int, int> GetPos()const noexcept
		{
			return {x,y};
		}
		int GetxPos()const noexcept
		{
			return x;
		}
		int GetyPos()const noexcept
		{
			return y;
		}
		bool LeftIsPressed()const noexcept
		{
			return leftIsPressed;
		}
		bool RightIsPressed()const noexcept
		{
			return rightIsPressed;
		}
	};
public:
	Mouse()=default;
	 Mouse(const Mouse&)=delete;
	 Mouse& operator=(const Mouse&) = delete;

	 std::pair<int,int> GetPos() const noexcept;
	 std::optional<RawDelta> ReadRawDelta() noexcept;
	 int GetPosx()const noexcept;
	 int GetPosy()const noexcept;
	 bool IsInWindow() const noexcept;
	 bool LeftIsPressed() const noexcept;
	 bool RightIsPressed() const noexcept;
	 std::optional<Mouse::event> Read() noexcept;
	 bool IsEmpty() const noexcept
	 {
		 return buffer.empty();
	 }
	 void Flush() noexcept;
	 void EnableRaw() noexcept;
	 void DisableRaw() noexcept;
	 bool RawEnabled() const noexcept;
public:
	void OnMouseMove(int newx, int newy) noexcept;
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void OnRawDelta(int dx, int dy) noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void TrimBuffer() noexcept;
	void TrimRawInputBuffer() noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;
private:
	static constexpr unsigned int bufferSize = 16u;
	bool LeftPressed=false;
	bool RightPressed=false;
	bool isInWindow = false;
	int wheelDeltaCarry = 0;
	bool rawEnabled = false;
	std::queue<event> buffer;
	std::queue<RawDelta> rawDeltaBuffer;
	int x, y;
};

