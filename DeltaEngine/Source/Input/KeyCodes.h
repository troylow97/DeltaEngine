#pragma once

namespace DeltaEngine
{

	//typedef enum class KeyCode : uint16_t
	//{
	//	// From glfw3.h
	//	Space = 32,
	//	Apostrophe = 39, /* ' */
	//	Comma = 44, /* , */
	//	Minus = 45, /* - */
	//	Period = 46, /* . */
	//	Slash = 47, /* / */
	//
	//	D0 = 48, /* 0 */
	//	D1 = 49, /* 1 */
	//	D2 = 50, /* 2 */
	//	D3 = 51, /* 3 */
	//	D4 = 52, /* 4 */
	//	D5 = 53, /* 5 */
	//	D6 = 54, /* 6 */
	//	D7 = 55, /* 7 */
	//	D8 = 56, /* 8 */
	//	D9 = 57, /* 9 */
	//
	//	Semicolon = 59, /* ; */
	//	Equal = 61, /* = */
	//
	//	A = 65,
	//	B = 66,
	//	C = 67,
	//	D = 68,
	//	E = 69,
	//	F = 70,
	//	G = 71,
	//	H = 72,
	//	I = 73,
	//	J = 74,
	//	K = 75,
	//	L = 76,
	//	M = 77,
	//	N = 78,
	//	O = 79,
	//	P = 80,
	//	Q = 81,
	//	R = 82,
	//	S = 83,
	//	T = 84,
	//	U = 85,
	//	V = 86,
	//	W = 87,
	//	X = 88,
	//	Y = 89,
	//	Z = 90,
	//
	//	LeftBracket = 91,  /* [ */
	//	Backslash = 92,  /* \ */
	//	RightBracket = 93,  /* ] */
	//	GraveAccent = 96,  /* ` */
	//
	//	World1 = 161, /* non-US #1 */
	//	World2 = 162, /* non-US #2 */
	//
	//	/* Function keys */
	//	Escape = 256,
	//	Enter = 257,
	//	Tab = 258,
	//	Backspace = 259,
	//	Insert = 260,
	//	Delete = 261,
	//	Right = 262,
	//	Left = 263,
	//	Down = 264,
	//	Up = 265,
	//	PageUp = 266,
	//	PageDown = 267,
	//	Home = 268,
	//	End = 269,
	//	CapsLock = 280,
	//	ScrollLock = 281,
	//	NumLock = 282,
	//	PrintScreen = 283,
	//	Pause = 284,
	//	F1 = 290,
	//	F2 = 291,
	//	F3 = 292,
	//	F4 = 293,
	//	F5 = 294,
	//	F6 = 295,
	//	F7 = 296,
	//	F8 = 297,
	//	F9 = 298,
	//	F10 = 299,
	//	F11 = 300,
	//	F12 = 301,
	//	F13 = 302,
	//	F14 = 303,
	//	F15 = 304,
	//	F16 = 305,
	//	F17 = 306,
	//	F18 = 307,
	//	F19 = 308,
	//	F20 = 309,
	//	F21 = 310,
	//	F22 = 311,
	//	F23 = 312,
	//	F24 = 313,
	//	F25 = 314,
	//
	//	/* Keypad */
	//	KP0 = 320,
	//	KP1 = 321,
	//	KP2 = 322,
	//	KP3 = 323,
	//	KP4 = 324,
	//	KP5 = 325,
	//	KP6 = 326,
	//	KP7 = 327,
	//	KP8 = 328,
	//	KP9 = 329,
	//	KPDecimal = 330,
	//	KPDivide = 331,
	//	KPMultiply = 332,
	//	KPSubtract = 333,
	//	KPAdd = 334,
	//	KPEnter = 335,
	//	KPEqual = 336,
	//
	//	LeftShift = 340,
	//	LeftControl = 341,
	//	LeftAlt = 342,
	//	LeftSuper = 343,
	//	RightShift = 344,
	//	RightControl = 345,
	//	RightAlt = 346,
	//	RightSuper = 347,
	//	Menu = 348
	//} Key;
	//
	//inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	//{
	//	os << static_cast<int32_t>(keyCode);
	//	return os;
	
}

// From glfw3.h
#define DE_KEY_SPACE           ::DeltaEngine::Key::Space
#define DE_KEY_APOSTROPHE      ::DeltaEngine::Key::Apostrophe    /* ' */
#define DE_KEY_COMMA           ::DeltaEngine::Key::Comma         /* , */
#define DE_KEY_MINUS           ::DeltaEngine::Key::Minus         /* - */
#define DE_KEY_PERIOD          ::DeltaEngine::Key::Period        /* . */
#define DE_KEY_SLASH           ::DeltaEngine::Key::Slash         /* / */
#define DE_KEY_0               ::DeltaEngine::Key::D0
#define DE_KEY_1               ::DeltaEngine::Key::D1
#define DE_KEY_2               ::DeltaEngine::Key::D2
#define DE_KEY_3               ::DeltaEngine::Key::D3
#define DE_KEY_4               ::DeltaEngine::Key::D4
#define DE_KEY_5               ::DeltaEngine::Key::D5
#define DE_KEY_6               ::DeltaEngine::Key::D6
#define DE_KEY_7               ::DeltaEngine::Key::D7
#define DE_KEY_8               ::DeltaEngine::Key::D8
#define DE_KEY_9               ::DeltaEngine::Key::D9
#define DE_KEY_SEMICOLON       ::DeltaEngine::Key::Semicolon     /* ; */
#define DE_KEY_EQUAL           ::DeltaEngine::Key::Equal         /* = */
#define DE_KEY_A               ::DeltaEngine::Key::A
#define DE_KEY_B               ::DeltaEngine::Key::B
#define DE_KEY_C               ::DeltaEngine::Key::C
#define DE_KEY_D               ::DeltaEngine::Key::D
#define DE_KEY_E               ::DeltaEngine::Key::E
#define DE_KEY_F               ::DeltaEngine::Key::F
#define DE_KEY_G               ::DeltaEngine::Key::G
#define DE_KEY_H               ::DeltaEngine::Key::H
#define DE_KEY_I               ::DeltaEngine::Key::I
#define DE_KEY_J               ::DeltaEngine::Key::J
#define DE_KEY_K               ::DeltaEngine::Key::K
#define DE_KEY_L               ::DeltaEngine::Key::L
#define DE_KEY_M               ::DeltaEngine::Key::M
#define DE_KEY_N               ::DeltaEngine::Key::N
#define DE_KEY_O               ::DeltaEngine::Key::O
#define DE_KEY_P               ::DeltaEngine::Key::P
#define DE_KEY_Q               ::DeltaEngine::Key::Q
#define DE_KEY_R               ::DeltaEngine::Key::R
#define DE_KEY_S               ::DeltaEngine::Key::S
#define DE_KEY_T               ::DeltaEngine::Key::T
#define DE_KEY_U               ::DeltaEngine::Key::U
#define DE_KEY_V               ::DeltaEngine::Key::V
#define DE_KEY_W               ::DeltaEngine::Key::W
#define DE_KEY_X               ::DeltaEngine::Key::X
#define DE_KEY_Y               ::DeltaEngine::Key::Y
#define DE_KEY_Z               ::DeltaEngine::Key::Z
#define DE_KEY_LEFT_BRACKET    ::DeltaEngine::Key::LeftBracket   /* [ */
#define DE_KEY_BACKSLASH       ::DeltaEngine::Key::Backslash     /* \ */
#define DE_KEY_RIGHT_BRACKET   ::DeltaEngine::Key::RightBracket  /* ] */
#define DE_KEY_GRAVE_ACCENT    ::DeltaEngine::Key::GraveAccent   /* ` */
#define DE_KEY_WORLD_1         ::DeltaEngine::Key::World1        /* non-US #1 */
#define DE_KEY_WORLD_2         ::DeltaEngine::Key::World2        /* non-US #2 */

/* Function keys */
#define DE_KEY_ESCAPE          ::DeltaEngine::Key::Escape
#define DE_KEY_ENTER           ::DeltaEngine::Key::Enter
#define DE_KEY_TAB             ::DeltaEngine::Key::Tab
#define DE_KEY_BACKSPACE       ::DeltaEngine::Key::Backspace
#define DE_KEY_INSERT          ::DeltaEngine::Key::Insert
#define DE_KEY_DELETE          ::DeltaEngine::Key::Delete
#define DE_KEY_RIGHT           ::DeltaEngine::Key::Right
#define DE_KEY_LEFT            ::DeltaEngine::Key::Left
#define DE_KEY_DOWN            ::DeltaEngine::Key::Down
#define DE_KEY_UP              ::DeltaEngine::Key::Up
#define DE_KEY_PAGE_UP         ::DeltaEngine::Key::PageUp
#define DE_KEY_PAGE_DOWN       ::DeltaEngine::Key::PageDown
#define DE_KEY_HOME            ::DeltaEngine::Key::Home
#define DE_KEY_END             ::DeltaEngine::Key::End
#define DE_KEY_CAPS_LOCK       ::DeltaEngine::Key::CapsLock
#define DE_KEY_SCROLL_LOCK     ::DeltaEngine::Key::ScrollLock
#define DE_KEY_NUM_LOCK        ::DeltaEngine::Key::NumLock
#define DE_KEY_PRINT_SCREEN    ::DeltaEngine::Key::PrintScreen
#define DE_KEY_PAUSE           ::DeltaEngine::Key::Pause
#define DE_KEY_F1              ::DeltaEngine::Key::F1
#define DE_KEY_F2              ::DeltaEngine::Key::F2
#define DE_KEY_F3              ::DeltaEngine::Key::F3
#define DE_KEY_F4              ::DeltaEngine::Key::F4
#define DE_KEY_F5              ::DeltaEngine::Key::F5
#define DE_KEY_F6              ::DeltaEngine::Key::F6
#define DE_KEY_F7              ::DeltaEngine::Key::F7
#define DE_KEY_F8              ::DeltaEngine::Key::F8
#define DE_KEY_F9              ::DeltaEngine::Key::F9
#define DE_KEY_F10             ::DeltaEngine::Key::F10
#define DE_KEY_F11             ::DeltaEngine::Key::F11
#define DE_KEY_F12             ::DeltaEngine::Key::F12
#define DE_KEY_F13             ::DeltaEngine::Key::F13
#define DE_KEY_F14             ::DeltaEngine::Key::F14
#define DE_KEY_F15             ::DeltaEngine::Key::F15
#define DE_KEY_F16             ::DeltaEngine::Key::F16
#define DE_KEY_F17             ::DeltaEngine::Key::F17
#define DE_KEY_F18             ::DeltaEngine::Key::F18
#define DE_KEY_F19             ::DeltaEngine::Key::F19
#define DE_KEY_F20             ::DeltaEngine::Key::F20
#define DE_KEY_F21             ::DeltaEngine::Key::F21
#define DE_KEY_F22             ::DeltaEngine::Key::F22
#define DE_KEY_F23             ::DeltaEngine::Key::F23
#define DE_KEY_F24             ::DeltaEngine::Key::F24
#define DE_KEY_F25             ::DeltaEngine::Key::F25

/* Keypad */
#define DE_KEY_KP_0            ::DeltaEngine::Key::KP0
#define DE_KEY_KP_1            ::DeltaEngine::Key::KP1
#define DE_KEY_KP_2            ::DeltaEngine::Key::KP2
#define DE_KEY_KP_3            ::DeltaEngine::Key::KP3
#define DE_KEY_KP_4            ::DeltaEngine::Key::KP4
#define DE_KEY_KP_5            ::DeltaEngine::Key::KP5
#define DE_KEY_KP_6            ::DeltaEngine::Key::KP6
#define DE_KEY_KP_7            ::DeltaEngine::Key::KP7
#define DE_KEY_KP_8            ::DeltaEngine::Key::KP8
#define DE_KEY_KP_9            ::DeltaEngine::Key::KP9
#define DE_KEY_KP_DECIMAL      ::DeltaEngine::Key::KPDecimal
#define DE_KEY_KP_DIVIDE       ::DeltaEngine::Key::KPDivide
#define DE_KEY_KP_MULTIPLY     ::DeltaEngine::Key::KPMultiply
#define DE_KEY_KP_SUBTRACT     ::DeltaEngine::Key::KPSubtract
#define DE_KEY_KP_ADD          ::DeltaEngine::Key::KPAdd
#define DE_KEY_KP_ENTER        ::DeltaEngine::Key::KPEnter
#define DE_KEY_KP_EQUAL        ::DeltaEngine::Key::KPEqual

#define DE_KEY_LEFT_SHIFT      ::DeltaEngine::Key::LeftShift
#define DE_KEY_LEFT_CONTROL    ::DeltaEngine::Key::LeftControl
#define DE_KEY_LEFT_ALT        ::DeltaEngine::Key::LeftAlt
#define DE_KEY_LEFT_SUPER      ::DeltaEngine::Key::LeftSuper
#define DE_KEY_RIGHT_SHIFT     ::DeltaEngine::Key::RightShift
#define DE_KEY_RIGHT_CONTROL   ::DeltaEngine::Key::RightControl
#define DE_KEY_RIGHT_ALT       ::DeltaEngine::Key::RightAlt
#define DE_KEY_RIGHT_SUPER     ::DeltaEngine::Key::RightSuper
#define DE_KEY_MENU            ::DeltaEngine::Key::Menu

//MouseCodes

namespace DeltaEngine
{
	//typedef enum class MouseCode : uint16_t
	//{
	//	// From glfw3.h
	//	Button0 = 0,
	//	Button1 = 1,
	//	Button2 = 2,
	//	Button3 = 3,
	//	Button4 = 4,
	//	Button5 = 5,
	//	Button6 = 6,
	//	Button7 = 7,
	//
	//	ButtonLast = Button7,
	//	ButtonLeft = Button0,
	//	ButtonRight = Button1,
	//	ButtonMiddle = Button2
	//} Mouse;
	//
	//inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	//{
	//	os << static_cast<int32_t>(mouseCode);
	//	return os;
	//}
}

#define DE_MOUSE_BUTTON_0      ::DeltaEngine::Mouse::Button0
#define DE_MOUSE_BUTTON_1      ::DeltaEngine::Mouse::Button1
#define DE_MOUSE_BUTTON_2      ::DeltaEngine::Mouse::Button2
#define DE_MOUSE_BUTTON_3      ::DeltaEngine::Mouse::Button3
#define DE_MOUSE_BUTTON_4      ::DeltaEngine::Mouse::Button4
#define DE_MOUSE_BUTTON_5      ::DeltaEngine::Mouse::Button5
#define DE_MOUSE_BUTTON_6      ::DeltaEngine::Mouse::Button6
#define DE_MOUSE_BUTTON_7      ::DeltaEngine::Mouse::Button7
#define DE_MOUSE_BUTTON_LAST   ::DeltaEngine::Mouse::ButtonLast
#define DE_MOUSE_BUTTON_LEFT   ::DeltaEngine::Mouse::ButtonLeft
#define DE_MOUSE_BUTTON_RIGHT  ::DeltaEngine::Mouse::ButtonRight
#define DE_MOUSE_BUTTON_MIDDLE ::DeltaEngine::Mouse::ButtonMiddle

