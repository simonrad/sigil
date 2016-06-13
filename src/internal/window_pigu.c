#include "pigu.h"
#include "sl.h"

#include <linux/input.h>

static int sliWindowOpen = 0;
static int sliWindowWidth = 0;
static int sliWindowHeight = 0;

// these are printable keys like 'A', 'B', etc.; this defines a map from the SIGIL keys to the linux keys
static const int NUM_PRINTABLE_KEYS = 61;
static const int SIGIL_RPI_PRINTABLE_KEYS[] = {' ', KEY_SPACE,
											  '!', -1,
											  '\"', -1,
											  '#', -1,
											  '$', -1,
											  '%', -1,
											  '&', -1,
											  '\'', -1,
											  '(', -1,
											  ')', -1,
											  '*', -1,
											  '+', -1,
											  ',', KEY_COMMA,
											  '-', KEY_MINUS,
											  '.', KEY_DOT,
											  '/', KEY_SLASH,
											  '0', KEY_0,
											  '1', KEY_1,
											  '2', KEY_2,
											  '3', KEY_3,
											  '4', KEY_4,
											  '5', KEY_5,
											  '6', KEY_6,
											  '7', KEY_7,
											  '8', KEY_8,
											  '9', KEY_9,
											  ':', -1,
											  ';', KEY_SEMICOLON,
											  '<', -1,
											  '=', KEY_EQUAL,
											  '>', -1,
											  '?', -1,
											  '@', -1,
											  'A', KEY_A,
											  'B', KEY_B,
											  'C', KEY_C,
											  'D', KEY_D,
											  'E', KEY_E,
											  'F', KEY_F,
											  'G', KEY_G,
											  'H', KEY_H,
											  'I', KEY_I,
											  'J', KEY_J,
											  'K', KEY_K,
											  'L', KEY_L,
											  'M', KEY_M,
											  'N', KEY_N,
											  'O', KEY_O,
											  'P', KEY_P,
											  'Q', KEY_Q,
											  'R', KEY_R,
											  'S', KEY_S,
											  'T', KEY_T,
											  'U', KEY_U,
											  'V', KEY_V,
											  'W', KEY_W,
											  'X', KEY_X,
											  'Y', KEY_Y,
											  'Z', KEY_Z,
											  '[', KEY_LEFTBRACE,
											  ']', KEY_RIGHTBRACE};

// these are non-printable keys like arrow keys, etc.; this defines a map from the SIGIL keys to the linux keys
static const int NUM_NON_PRINTABLE_KEYS = 92;
static const int SIGIL_RPI_NON_PRINTABLE_KEYS[] = {SL_KEY_ESCAPE, KEY_ESC,
												 SL_KEY_ENTER, KEY_ENTER,
												 SL_KEY_TAB, KEY_TAB,
												 SL_KEY_BACKSPACE, KEY_BACKSPACE,
												 SL_KEY_INSERT, KEY_INSERT,
												 SL_KEY_DELETE, KEY_DELETE,
												 SL_KEY_RIGHT, KEY_RIGHT,
												 SL_KEY_LEFT, KEY_LEFT,
												 SL_KEY_DOWN, KEY_DOWN,
												 SL_KEY_UP, KEY_UP,
												 SL_KEY_PAGE_UP, KEY_PAGEUP,
												 SL_KEY_PAGE_DOWN, KEY_PAGEDOWN,
												 SL_KEY_HOME, KEY_HOME,
												 SL_KEY_END, KEY_END,
												 -1, -1,
												 -1, -1,
												 -1, -1,
												 -1, -1,
												 -1, -1,
												 -1, -1,
												 -1, -1,
												 -1, -1,
												 -1, -1,
												 -1, -1,
												 SL_KEY_CAPS_LOCK, KEY_CAPSLOCK,
												 SL_KEY_SCROLL_LOCK, KEY_SCROLLLOCK,
												 SL_KEY_NUM_LOCK, KEY_NUMLOCK,
												 SL_KEY_PRINT_SCREEN, KEY_PRINT,
												 SL_KEY_PAUSE, KEY_PAUSE,
												 -1, -1,
												 -1, -1,
												 -1, -1,
												 -1, -1,
												 -1, -1,
												 SL_KEY_F1, KEY_F1,
												 SL_KEY_F2, KEY_F2,
												 SL_KEY_F3, KEY_F3,
												 SL_KEY_F4, KEY_F4,
												 SL_KEY_F5, KEY_F5,
												 SL_KEY_F6, KEY_F6,
												 SL_KEY_F7, KEY_F7,
												 SL_KEY_F8, KEY_F8,
												 SL_KEY_F9, KEY_F9,
												 SL_KEY_F10, KEY_F10,
												 SL_KEY_F11, KEY_F11,
												 SL_KEY_F12, KEY_F12,
												 SL_KEY_F13, KEY_F13,
												 SL_KEY_F14, KEY_F14,
												 SL_KEY_F15, KEY_F15,
												 SL_KEY_F16, KEY_F16,
												 SL_KEY_F17, KEY_F17,
												 SL_KEY_F18, KEY_F18,
												 SL_KEY_F19, KEY_F19,
												 SL_KEY_F20, KEY_F20,
												 SL_KEY_F21, KEY_F21,
												 SL_KEY_F22, KEY_F22,
												 SL_KEY_F23, KEY_F23,
												 SL_KEY_F24, KEY_F24,
												 SL_KEY_F25, -1,
												 -1, -1,
												 -1, -1,
												 -1, -1,
												 -1, -1,
												 -1, -1,
												 SL_KEY_KEYPAD_0, KEY_KP0,
												 SL_KEY_KEYPAD_1, KEY_KP1,
												 SL_KEY_KEYPAD_2, KEY_KP2,
												 SL_KEY_KEYPAD_3, KEY_KP3,
												 SL_KEY_KEYPAD_4, KEY_KP4,
												 SL_KEY_KEYPAD_5, KEY_KP5,
												 SL_KEY_KEYPAD_6, KEY_KP6,
												 SL_KEY_KEYPAD_7, KEY_KP7,
												 SL_KEY_KEYPAD_8, KEY_KP8,
												 SL_KEY_KEYPAD_9, KEY_KP9,
												 SL_KEY_KEYPAD_DECIMAL, KEY_KPDOT,
												 SL_KEY_KEYPAD_DIVIDE, KEY_KPSLASH,
												 SL_KEY_KEYPAD_MULTIPLY, KEY_KPASTERISK,
												 SL_KEY_KEYPAD_SUBTRACT, KEY_KPMINUS,
												 SL_KEY_KEYPAD_ADD, KEY_KPPLUS,
												 SL_KEY_KEYPAD_ENTER, KEY_KPENTER,
												 SL_KEY_KEYPAD_EQUAL, KEY_KPEQUAL,
												 -1, -1,
												 -1, -1,
												 -1, -1,
												 SL_KEY_LEFT_SHIFT, KEY_LEFTSHIFT,
												 SL_KEY_LEFT_CONTROL, KEY_LEFTCTRL,
												 SL_KEY_LEFT_ALT, KEY_LEFTALT,
												 SL_KEY_LEFT_SUPER, KEY_LEFTMETA,
												 SL_KEY_RIGHT_SHIFT, KEY_RIGHTSHIFT,
												 SL_KEY_RIGHT_CONTROL, KEY_RIGHTCTRL,
												 SL_KEY_RIGHT_ALT, KEY_RIGHTALT,
												 SL_KEY_RIGHT_SUPER, KEY_RIGHTMETA};

void sliOpenWindow(int width, int height, const char *title, int fullScreen)
{
	// start up PIGU
	piguInit();

	// create our OpenGL window; fullScreen parameter is ignored
	piguCreateWindow(width,					// window width
					 height,				// window height
					 8,						// red bits
					 8,						// green bits
					 8,						// blue bits
					 8,						// alpha bits
					 24,					// depth bits
					 8,						// stencil bits
					 0);					// no multi-sampling

	// record window size
	sliWindowWidth = width;
	sliWindowHeight = height;

	// record the window as open
	sliWindowOpen = 1;
}

void sliShowCursor(int showCursor) { }

void sliCloseWindow()
{
	piguCloseWindow();
	sliWindowOpen = 0;
}

int sliIsWindowOpen()
{
	return sliWindowOpen;
}

int sliShouldClose()
{
	return 0;
}

int sliGetKey(int key)
{
	int index;
	int code;
	int linuxKey;
	int result = 0;

	// if this is a non-printable char
	if(key >= SIGIL_RPI_NON_PRINTABLE_KEYS[0] && key <= SIGIL_RPI_NON_PRINTABLE_KEYS[(NUM_NON_PRINTABLE_KEYS - 1) * 2])
	{
		// determine if we actually have an entry for this key code
		index = key - SIGIL_RPI_NON_PRINTABLE_KEYS[0];
		code = SIGIL_RPI_NON_PRINTABLE_KEYS[index * 2];
		if(code != -1)
		{
			// look up the linux key associated with the input key we received
			linuxKey = SIGIL_RPI_NON_PRINTABLE_KEYS[index * 2 + 1];
			if(linuxKey != -1)
			{
				result = piguIsKeyDown(linuxKey);
			}
		}
	}
	else if(key >= SIGIL_RPI_PRINTABLE_KEYS[0] && key <= SIGIL_RPI_PRINTABLE_KEYS[(NUM_PRINTABLE_KEYS - 1) * 2])
	{
		// determine if this printable character has a key associated with it
		index = key - SIGIL_RPI_PRINTABLE_KEYS[0];
		code = SIGIL_RPI_PRINTABLE_KEYS[index * 2];
		if(code != -1)
		{
			// look up the linux key associated with the input key we received
			linuxKey = SIGIL_RPI_PRINTABLE_KEYS[index * 2 + 1];
			if(linuxKey != -1)
			{
				result = piguIsKeyDown(linuxKey);
			}
		}
	}

	return result;
}

int sliGetMouseButton(int button)
{
	return piguIsMouseButtonDown(button);
}

void sliGetMousePos(int *posX, int *posY)
{
	piguGetMousePosition(posX, posY);
	*posY = sliWindowHeight - *posY;
}

double sliGetTime()
{
	return piguGetTime();
}

void sliPollAndSwap()
{
	piguPollEvents();
	piguSwapBuffers();
}
