#ifndef INCLUDED_INPUT_INPUTSYSTEM_H
#define INCLUDED_INPUT_INPUTSYSTEM_H

#include "InputTypes.h"

namespace WOWGE
{
namespace Input 
{

class InputSystem
{
public:
	static void StaticInitialize(HWND window);
	static void StaticTerminate();
	static InputSystem* Get();

	static void ProcessMessage(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	InputSystem();
	~InputSystem();

	void Initialize(HWND window);
	void Terminate();

	void Update();

	bool IsKeyDown(KeyCode key) const;
	bool IsKeyPressed(KeyCode key) const;

	bool IsMouseDown(MouseButton button) const;
	bool IsMousePressed(MouseButton button) const;

	int GetMouseScreenX() const;
	int GetMouseScreenY() const;

	bool IsMouseLeftEdge() const;
	bool IsMouseRightEdge() const;
	bool IsMouseTopEdge() const;
	bool IsMouseBottomEdge() const;

	int GetMouseMoveX() const;
	int GetMouseMoveY() const;
	int GetMouseMoveZ() const;

	bool IsGamePadButtonDown(GamePadButton button) const;
	bool IsGamePadButtonPressed(GamePadButton button) const;

	bool IsDPadUp() const;
	bool IsDPadDown() const;
	bool IsDPadLeft() const;
	bool IsDPadRight() const;

	float GetLeftAnalogX() const;
	float GetLeftAnalogY() const;
	float GetRightAnalogX() const;
	float GetRightAnalogY() const;

private:
	InputSystem(const InputSystem&) = delete;
	InputSystem& operator=(const InputSystem&) = delete;

	friend BOOL CALLBACK EnumGamePadCallback(const DIDEVICEINSTANCE* pDIDeviceInstance, VOID* pContext);

	void UpdateKeyboard();
	void UpdateMouse();
	void UpdateGamePad();

	IDirectInput8* mDirectInput;
	IDirectInputDevice8* mKeyboardDevice;
	IDirectInputDevice8* mMouseDevice;
	IDirectInputDevice8* mGamePadDevice;

	static const uint32_t kKeyBufferSize = 256;
	uint8_t mCurrKeyBuffer[kKeyBufferSize];
	uint8_t mPrevKeyBuffer[kKeyBufferSize];

	DIMOUSESTATE mCurrMouseState;
	DIMOUSESTATE mPrevMouseState;

	DIJOYSTATE mCurrGamePadState;
	DIJOYSTATE mPrevGamePadState;

	int mMouseX;
	int mMouseY;

	int mScreenWidth;
	int mScreenHeight;

	bool mInitialized;
};

} // namespace Input
} // namespace WOWGE

#endif // #ifndef INCLUDED_INPUT_INPUTSYSTEM_H