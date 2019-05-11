#ifndef INCLUDED_CORE_WINDOW_H
#define INCLUDED_CORE_WINDOW_H

namespace WOWGE
{
namespace Core
{

class Window
{
public:
	Window();
	~Window();

	void Initialize(HINSTANCE instance, LPCSTR appName, uint32_t width, uint32_t height);
	void Terminate();

	bool ProcessMessage();

	HWND GetWindowHandle()const { return mWindow; }

private:
	HINSTANCE mInstance;
	HWND mWindow;

	std::string mAppName;
};

} //namespace Core
} //namespace WOWGE

#endif // !INCLUDED_CORE_WINDOW_H
