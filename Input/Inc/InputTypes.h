#ifndef INCLUDED_INPUT_INPUTTYPES_H
#define INCLUDED_INPUT_INPUTTYPES_H

namespace WOWGE
{
namespace Input 
{

enum class KeyCode : uint32_t
{
	// Keyboard roll 1
	ESCAPE = 0x01,
	F1 = 0x3B,
	F2 = 0x3C,
	F3 = 0x3D,
	F4 = 0x3E,
	F5 = 0x3F,
	F6 = 0x40,
	F7 = 0x41,
	F8 = 0x42,
	F9 = 0x43,
	F10 = 0x44,
	F11 = 0x57,
	F12 = 0x58,

	// Keyboard roll 2
	GRAVE = 0x29,
	ONE = 0x02,
	TWO = 0x03,
	THREE = 0x04,
	FOUR = 0x05,
	FIVE = 0x06,
	SIX = 0x07,
	SEVEN = 0x08,
	EIGHT = 0x09,
	NINE = 0x0A,
	ZERO = 0x0B,
	MINUS = 0x0C,
	EQUALS = 0x0D,
	BACKSPACE = 0x0E,

	// Keyboard roll 3
	TAB = 0x0F,
	Q = 0x10,
	W = 0x11,
	E = 0x12,
	R = 0x13,
	T = 0x14,
	Y = 0x15,
	U = 0x16,
	I = 0x17,
	O = 0x18,
	P = 0x19,
	LBRACKET = 0x1A,
	RBRACKET = 0x1B,
	BACKSLASH = 0x2B,

	// Keyboard roll 4
	A = 0x1E,
	S = 0x1F,
	D = 0x20,
	F = 0x21,
	G = 0x22,
	H = 0x23,
	J = 0x24,
	K = 0x25,
	L = 0x26,
	SEMICOLON = 0x27,
	APOSTROPHE = 0x28,
	ENTER = 0x1C,

	// Keyboard roll 5
	Z = 0x2C,
	X = 0x2D,
	C = 0x2E,
	V = 0x2F,
	B = 0x30,
	N = 0x31,
	M = 0x32,
	COMMA = 0x33,
	PERIOD = 0x34,
	SLASH = 0x35,

	// Lock keys
	CAPSLOCK = 0x3A,
	NUMLOCK = 0x45,
	SCROLLLOCK = 0x46,

	// Numpad
	NUMPAD1 = 0x4F,
	NUMPAD2 = 0x50,
	NUMPAD3 = 0x51,
	NUMPAD4 = 0x4B,
	NUMPAD5 = 0x4C,
	NUMPAD6 = 0x4D,
	NUMPAD7 = 0x47,
	NUMPAD8 = 0x48,
	NUMPAD9 = 0x49,
	NUMPAD0 = 0x52,
	NUM_ADD = 0x4E,
	NUM_SUB = 0x4A,
	NUM_MUL = 0x37,
	NUM_DIV = 0xB5,
	NUM_ENTER = 0x9C,
	NUM_DECIMAL = 0x53,

	// Navigation keys
	INS = 0xD2,
	DEL = 0xD3,
	HOME = 0xC7,
	END = 0xCF,
	PGUP = 0xC9,
	PGDN = 0xD1,

	// Support keys
	LSHIFT = 0x2A,
	RSHIFT = 0x36,
	LCONTROL = 0x1D,
	RCONTROL = 0x9D,
	LALT = 0x38,
	RALT = 0xB8,
	LWIN = 0xDB,
	RWIN = 0xDC,
	SPACE = 0x39,

	// Arrow keys
	UP = 0xC8,
	DOWN = 0xD0,
	LEFT = 0xCB,
	RIGHT = 0xCD,
};

enum class MouseButton : uint32_t
{
	LBUTTON = 0,
	RBUTTON = 1,
	MBUTTON = 2,
};

enum class GamePadButton : uint32_t
{
	BUTTON1 = 0,
	BUTTON2 = 1,
	BUTTON3 = 2,
	BUTTON4 = 3,
	BUTTON5 = 4,
	BUTTON6 = 5,
	BUTTON7 = 6,
	BUTTON8 = 7,
	BUTTON9 = 8,
	BUTTON10 = 9,
};

} //namespace Input
} //namespace WOWGE

#endif // #ifndef INCLUDED_INPUT_INPUTTYPES_H