#ifndef INCLUDED_GRAPHICS_GUI_H
#define INCLUDED_GRAPHICS_GUI_H

namespace WOWGE
{
namespace Gui
{
	void Initialize(HWND window);
	void Terminate();
	void BeginRender(float deltaTime);
	void EndRender();
} //namespace GUI
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_GUI_H
