#include "Precompiled.h"
#include "InputSystem.h"
#include <ImGui\Inc\imgui.h>

using namespace WOWGE;
using namespace WOWGE::Input;

BOOL CALLBACK Input::EnumGamePadCallback(const DIDEVICEINSTANCE* pDIDeviceInstance, VOID* pContext)
{
	// Obtain an interface to the enumerated joystick
	InputSystem* inputSystem = static_cast<InputSystem*>(pContext);
	IDirectInput8* pDI = inputSystem->mDirectInput;
	IDirectInputDevice8** pGamePad = &(inputSystem->mGamePadDevice);
	if (FAILED(pDI->CreateDevice(pDIDeviceInstance->guidInstance, pGamePad, nullptr))) 
	{
		LOG("[Input::InputSystem] Failed to create game pad device.");
	}

	return DIENUM_STOP;
}

namespace
{
	InputSystem* sInputSystem = nullptr;
}

void InputSystem::StaticInitialize(HWND window)
{
	ASSERT(sInputSystem == nullptr, "[Input::InputSystem] System already initialized!");
	sInputSystem = new InputSystem();
	sInputSystem->Initialize(window);
}

void InputSystem::StaticTerminate()
{
	if (sInputSystem != nullptr)
	{
		sInputSystem->Terminate();
		SafeDelete(sInputSystem);
	}
}

InputSystem* Input::InputSystem::Get()
{
	ASSERT(sInputSystem != nullptr, "[Input::InputSystem] No system registered.");
	return sInputSystem;
}

void Input::InputSystem::ProcessMessage(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (sInputSystem == nullptr)
	{
		return;
	}
	// Capture new state
	ImGuiIO& io = ImGui::GetIO();
	switch (msg)
	{
	case WM_ACTIVATEAPP:
		return;
	case WM_LBUTTONDOWN:
		if (io.WantCaptureMouse)
			io.MouseDown[0] = true;
		return;
	case WM_LBUTTONUP:
		io.MouseDown[0] = false;
		return;
	case WM_RBUTTONDOWN:
		if (io.WantCaptureMouse)
			io.MouseDown[1] = true;
		return;
	case WM_RBUTTONUP:
		io.MouseDown[1] = false;
		return;
	case WM_MBUTTONDOWN:
		if (io.WantCaptureMouse)
			io.MouseDown[2] = true;
		return;
	case WM_MBUTTONUP:
		io.MouseDown[2] = false;
		return;
	case WM_MOUSEWHEEL:
		io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;;
		return;
	case WM_MOUSEMOVE:
	{
		int mouseX = (signed short)(lParam);
		int mouseY = (signed short)(lParam >> 16);

		io.MousePos.x = (float)mouseX;
		io.MousePos.y = (float)mouseY;

		sInputSystem->mMouseX = mouseX;
		sInputSystem->mMouseY = mouseY;

		RECT rect;
		GetClientRect(window, &rect);
	}
	return;
	case WM_KEYDOWN:
		if (wParam < 256)
		{
			io.KeysDown[wParam] = true;
		}
		return;
	case WM_KEYUP:
		if (wParam < 256)
		{
			io.KeysDown[wParam] = false;
		}
		return;
	case WM_CHAR:
		// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
		if (wParam > 0 && wParam < 0x10000)
			io.AddInputCharacter((unsigned short)wParam);
		return;
	}
}

InputSystem::InputSystem()
	: mDirectInput(nullptr)
	, mKeyboardDevice(nullptr)
	, mMouseDevice(nullptr)
	, mGamePadDevice(nullptr)
	, mMouseX(0)
	, mMouseY(0)
	, mScreenWidth(0)
	, mScreenHeight(0)
	, mInitialized(false)
{
	ZeroMemory(mCurrKeyBuffer, kKeyBufferSize);
	ZeroMemory(mPrevKeyBuffer, kKeyBufferSize);
	ZeroMemory(&mCurrMouseState, sizeof(DIMOUSESTATE));
	ZeroMemory(&mPrevMouseState, sizeof(DIMOUSESTATE));
	ZeroMemory(&mCurrGamePadState, sizeof(DIJOYSTATE));
	ZeroMemory(&mPrevGamePadState, sizeof(DIJOYSTATE));
}

InputSystem::~InputSystem()
{
	ASSERT(!mInitialized, "[Input::InputSystem] Terminate() must be called to clean up!");
}

void InputSystem::Initialize(HWND window)
{
	// Check if we have already initialized the system
	if (mInitialized)
	{
		LOG("[Input::InputSystem] System already initialized.");
		return;
	}

	LOG("[Input::InputSystem] Initializing...");

	// Obtain an interface to DirectInput
	HRESULT hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDirectInput, nullptr);
	ASSERT(SUCCEEDED(hr), "[Input::InputSystem] Failed to create DirectInput object.");

	//----------------------------------------------------------------------------------------------------
	// Create keyboard device
	hr = mDirectInput->CreateDevice(GUID_SysKeyboard, &mKeyboardDevice, nullptr);
	ASSERT(SUCCEEDED(hr), "[Input::InputSystem] Failed to create keyboard device.");

	// Set the keyboard data format
	hr = mKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	ASSERT(SUCCEEDED(hr), "[Input::InputSystem] Failed to set keyboard data format.");

	// Set the keyboard cooperative level
	hr = mKeyboardDevice->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	ASSERT(SUCCEEDED(hr), "[Input::InputSystem] Failed to set keyboard cooperative level.");

	// Acquire the keyboard device
	hr = mKeyboardDevice->Acquire();
	ASSERT(SUCCEEDED(hr), "[Input::InputSystem] Failed to acquire keyboard device.");
	//----------------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------------
	// Create mouse device
	hr = mDirectInput->CreateDevice(GUID_SysMouse, &mMouseDevice, nullptr);
	ASSERT(SUCCEEDED(hr), "[Input::InputSystem] Failed to create mouse device.");

	// Set the mouse data format
	hr = mMouseDevice->SetDataFormat(&c_dfDIMouse);
	ASSERT(SUCCEEDED(hr), "[Input::InputSystem] Failed to set mouse data format.");

	// Set the mouse cooperative level
	hr = mMouseDevice->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	ASSERT(SUCCEEDED(hr), "[Input::InputSystem] Failed to set mouse cooperative level.");

	// Acquire the mouse device
	hr = mMouseDevice->Acquire();
	ASSERT(SUCCEEDED(hr), "[Input::InputSystem] Failed to acquire mouse device.");

	// Calculate starting mouse position
	RECT clientRect;
	GetClientRect(window, &clientRect);
	mScreenWidth = clientRect.right - clientRect.left;
	mScreenHeight = clientRect.bottom - clientRect.top;
	mMouseX = mScreenWidth >> 1;
	mMouseY = mScreenHeight >> 1;
	//----------------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------------
	// Enumerate for game pad device
	if (FAILED(mDirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumGamePadCallback, this, DIEDFL_ATTACHEDONLY)))
	{
		LOG("[Input::InputSystem] Failed to enumerate for game pad devices.");
	}

	// Check if we have a game pad detected
	if (mGamePadDevice != nullptr)
	{
		// Set the game pad data format
		hr = mGamePadDevice->SetDataFormat(&c_dfDIJoystick);
		ASSERT(SUCCEEDED(hr), "[Input::InputSystem] Failed to set game pad data format.");

		// Set the game pad cooperative level
		hr = mGamePadDevice->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		ASSERT(SUCCEEDED(hr), "[Input::InputSystem] Failed to set game pad cooperative level.");

		// Acquire the game pad device
		hr = mGamePadDevice->Acquire();
		ASSERT(SUCCEEDED(hr), "[Input::InputSystem] Failed to acquire game pad device.");
	}
	else
	{
		LOG("[Input::InputSystem] No game pad attached.");
	}

	// Set flag
	mInitialized = true;

	LOG("[Input::InputSystem] System initialized.");
}

void InputSystem::Terminate()
{
	// Check if we have already terminated the system
	if (!mInitialized)
	{
		LOG("[Input::InputSystem] System already terminated.");
		return;
	}

	LOG("[Input::InputSystem] Terminating...");

	// Release devices
	if (mGamePadDevice != nullptr)
	{
		mGamePadDevice->Unacquire();
		mGamePadDevice->Release();
		mGamePadDevice = nullptr;
	}
	if (mMouseDevice != nullptr)
	{
		mMouseDevice->Unacquire();
		mMouseDevice->Release();
		mMouseDevice = nullptr;
	}
	if (mKeyboardDevice != nullptr)
	{
		mKeyboardDevice->Unacquire();
		mKeyboardDevice->Release();
		mKeyboardDevice = nullptr;
	}

	SafeRelease(mDirectInput);

	// Set flag
	mInitialized = false;

	LOG("[Input::InputSystem] System terminated.");
}

void InputSystem::Update()
{
	ASSERT(mInitialized, "[Input::InputSystem] System not initialized.");

	// Update keyboard
	if (mKeyboardDevice != nullptr)
	{
		UpdateKeyboard();
	}

	// Update mouse
	if (mMouseDevice != nullptr)
	{
		UpdateMouse();
	}

	// Update game pad
	if (mGamePadDevice != nullptr)
	{
		UpdateGamePad();
	}
}

bool InputSystem::IsKeyDown(KeyCode key) const
{
	return (mCurrKeyBuffer[(uint32_t)key] & 0x80) != 0;
}

bool InputSystem::IsKeyPressed(KeyCode key) const
{
	const bool currState = (mCurrKeyBuffer[(uint32_t)key] & 0x80) != 0;
	const bool prevState = (mPrevKeyBuffer[(uint32_t)key] & 0x80) != 0;
	return !prevState && currState;
}

bool InputSystem::IsMouseDown(MouseButton button) const
{
	return (mCurrMouseState.rgbButtons[(uint32_t)button] & 0x80) != 0;
}

bool InputSystem::IsMousePressed(MouseButton button) const
{
	const bool currState = (mCurrMouseState.rgbButtons[(uint32_t)button] & 0x80) != 0;
	const bool prevState = (mPrevMouseState.rgbButtons[(uint32_t)button] & 0x80) != 0;
	return !prevState && currState;
}

int InputSystem::GetMouseScreenX() const
{
	return mMouseX;
}

int InputSystem::GetMouseScreenY() const
{
	return mMouseY;
}

bool InputSystem::IsMouseLeftEdge() const
{
	return (mMouseX == 0);
}

bool InputSystem::IsMouseRightEdge() const
{
	return (mMouseX == mScreenWidth - 1);
}

bool InputSystem::IsMouseTopEdge() const
{
	return (mMouseY == 0);
}

bool InputSystem::IsMouseBottomEdge() const
{
	return (mMouseY == mScreenHeight - 1);
}

int InputSystem::GetMouseMoveX() const
{
	return mCurrMouseState.lX;
}

int InputSystem::GetMouseMoveY() const
{
	return mCurrMouseState.lY;
}

int InputSystem::GetMouseMoveZ() const
{
	return mCurrMouseState.lZ;
}

bool InputSystem::IsGamePadButtonDown(GamePadButton button) const
{
	return (mCurrGamePadState.rgbButtons[(uint32_t)button] & 0x80) != 0;
}

bool InputSystem::IsGamePadButtonPressed(GamePadButton button) const
{
	const bool currState = (mCurrGamePadState.rgbButtons[(uint32_t)button] & 0x80) != 0;
	const bool prevState = (mPrevGamePadState.rgbButtons[(uint32_t)button] & 0x80) != 0;
	return !prevState && currState;
}

bool InputSystem::IsDPadUp() const
{
	const bool hasGamePad = (mGamePadDevice != nullptr);
	return hasGamePad && (mCurrGamePadState.rgdwPOV[0] == 0);
}

bool InputSystem::IsDPadDown() const
{
	return (mCurrGamePadState.rgdwPOV[0] == 18000);
}

bool InputSystem::IsDPadLeft() const
{
	return (mCurrGamePadState.rgdwPOV[0] == 27000);
}

bool InputSystem::IsDPadRight() const
{
	return (mCurrGamePadState.rgdwPOV[0] == 9000);
}

float InputSystem::GetLeftAnalogX() const
{
	return (mCurrGamePadState.lX / 32767.5f) - 1.0f;
}

float InputSystem::GetLeftAnalogY() const
{
	return -(mCurrGamePadState.lY / 32767.5f) + 1.0f;
}

float InputSystem::GetRightAnalogX() const
{
	return (mCurrGamePadState.lZ / 32767.5f) - 1.0f;
}

float InputSystem::GetRightAnalogY() const
{
	return -(mCurrGamePadState.lRz / 32767.5f) + 1.0f;
}

void InputSystem::UpdateKeyboard()
{
	// Store the previous keyboard state
	memcpy(mPrevKeyBuffer, mCurrKeyBuffer, kKeyBufferSize);

	// Get keyboard state
	static bool sWriteToLog = true;
	HRESULT hr = mKeyboardDevice->GetDeviceState(kKeyBufferSize, (void*)mCurrKeyBuffer);
	if (FAILED(hr))
	{   
		// Check if the device is lost
		if (DIERR_INPUTLOST == hr || DIERR_NOTACQUIRED == hr)
		{
			if (sWriteToLog)
			{
				LOG("[Input::InputSystem] Keyboard device is lost.");
				sWriteToLog = false;
			}

			// Try to acquire keyboard device again
			mKeyboardDevice->Acquire();
		}
		else
		{
			LOG("[Input::InputSystem] Failed to get keyboard state.");
			return;
		}
	}
	else
	{
		// Reset flag
		sWriteToLog = true;
	}
}

void InputSystem::UpdateMouse()
{
	// Store the previous mouse state
	memcpy(&mPrevMouseState, &mCurrMouseState, sizeof(DIMOUSESTATE));

	// Get mouse state
	static bool sWriteToLog = true;
	HRESULT hr = mMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (void*)&mCurrMouseState);
	if (FAILED(hr))
	{   
		// Check if the device is lost
		if (DIERR_INPUTLOST == hr || DIERR_NOTACQUIRED == hr)
		{
			if (sWriteToLog)
			{
				LOG("[Input::InputSystem] Mouse device is lost.");
				sWriteToLog = false;
			}

			// Try to acquire mouse device again
			mMouseDevice->Acquire();
		}
		else
		{
			LOG("[Input::InputSystem] Failed to get mouse state.");
			return;
		}
	}
	else
	{
		// Reset flag
		sWriteToLog = true;

		// Update mouse position
		mMouseX += mCurrMouseState.lX;
		mMouseY += mCurrMouseState.lY;

		// Check bound
		if (mMouseX < 0)
			mMouseX = 0;
		if (mMouseX>= mScreenWidth)
			mMouseX = mScreenWidth - 1;
		if (mMouseY < 0)
			mMouseY = 0;
		if (mMouseY>= mScreenHeight)
			mMouseY = mScreenHeight - 1;
	}
}

void InputSystem::UpdateGamePad()
{
	// Store the previous game pad state
	memcpy(&mPrevGamePadState, &mCurrGamePadState, sizeof(DIJOYSTATE));

	// Poll the game pad device
	static bool sWriteToLog = true;
	HRESULT hr = mGamePadDevice->Poll();
	if (FAILED(hr))
	{   
		// Check if the device is lost
		if (DIERR_INPUTLOST == hr || DIERR_NOTACQUIRED == hr)
		{
			if (sWriteToLog)
			{
				LOG("[Input::InputSystem] Game pad device is lost.");
				sWriteToLog = false;
			}

			// Try to acquire game pad device again
			mGamePadDevice->Acquire();
		}
		else
		{
			LOG("[Input::InputSystem] Failed to get game pad state.");
			return;
		}
	}
	else
	{
		// Reset flag
		sWriteToLog = true;
	}

	// Get game pad state
	hr = mGamePadDevice->GetDeviceState(sizeof(DIJOYSTATE), (void*)&mCurrGamePadState);
	if (FAILED(hr))
	{   
		// Check if the device is lost
		if (DIERR_INPUTLOST == hr || DIERR_NOTACQUIRED == hr)
		{
			if (sWriteToLog)
			{
				LOG("[Input::InputSystem] Game pad device is lost.");
				sWriteToLog = false;
			}

			// Try to acquire game pad device again
			mGamePadDevice->Acquire();
		}
		else
		{
			LOG("[Input::InputSystem] Failed to get game pad state.");
			return;
		}
	}
	else
	{
		// Reset flag
		sWriteToLog = true;
	}
}