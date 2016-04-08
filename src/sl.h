#pragma once

/*
Simple Input and Graphics Integration Library (SIGIL), for Windows.
Geoff Nagy
*/

/*
todo:
- de-initialization of text/fonts
- de-initialization of textures
*/

// text alignment symbolic constants
#define SL_ALIGN_CENTER 0
#define SL_ALIGN_RIGHT 1
#define SL_ALIGN_LEFT 2

// input key symbolic constants
#define SL_KEY_ESCAPE 256
#define SL_KEY_ENTER 257
#define SL_KEY_TAB 258
#define SL_KEY_BACKSPACE 259
#define SL_KEY_INSERT 260
#define SL_KEY_DELETE 261
#define SL_KEY_RIGHT 262
#define SL_KEY_LEFT 263
#define SL_KEY_DOWN 264
#define SL_KEY_UP 265
#define SL_KEY_PAGE_UP 266
#define SL_KEY_PAGE_DOWN 267
#define SL_KEY_HOME 268
#define SL_KEY_END 269
#define SL_KEY_CAPS_LOCK 280
#define SL_KEY_SCROLL_LOCK 281
#define SL_KEY_NUM_LOCK 282
#define SL_KEY_PRINT_SCREEN 283
#define SL_KEY_PAUSE 284
#define SL_KEY_F1 290
#define SL_KEY_F2 291
#define SL_KEY_F3 292
#define SL_KEY_F4 293
#define SL_KEY_F5 294
#define SL_KEY_F6 295
#define SL_KEY_F7 296
#define SL_KEY_F8 297
#define SL_KEY_F9 298
#define SL_KEY_F10 299
#define SL_KEY_F11 300
#define SL_KEY_F12 301
#define SL_KEY_F13 302
#define SL_KEY_F14 303
#define SL_KEY_F15 304
#define SL_KEY_F16 305
#define SL_KEY_F17 306
#define SL_KEY_F18 307
#define SL_KEY_F19 308
#define SL_KEY_F20 309
#define SL_KEY_F21 310
#define SL_KEY_F22 311
#define SL_KEY_F23 312
#define SL_KEY_F24 313
#define SL_KEY_F25 314
#define SL_KEY_KEYPAD_0 320
#define SL_KEY_KEYPAD_1 321
#define SL_KEY_KEYPAD_2 322
#define SL_KEY_KEYPAD_3 323
#define SL_KEY_KEYPAD_4 324
#define SL_KEY_KEYPAD_5 325
#define SL_KEY_KEYPAD_6 326
#define SL_KEY_KEYPAD_7 327
#define SL_KEY_KEYPAD_8 328
#define SL_KEY_KEYPAD_9 329
#define SL_KEY_KEYPAD_DECIMAL 330
#define SL_KEY_KEYPAD_DIVIDE 331
#define SL_KEY_KEYPAD_MULTIPLY 332
#define SL_KEY_KEYPAD_SUBTRACT 333
#define SL_KEY_KEYPAD_ADD 334
#define SL_KEY_KEYPAD_ENTER 335
#define SL_KEY_KEYPAD_EQUAL 336
#define SL_KEY_LEFT_SHIFT 340
#define SL_KEY_LEFT_CONTROL 341
#define SL_KEY_LEFT_ALT 342
#define SL_KEY_LEFT_SUPER 343
#define SL_KEY_RIGHT_SHIFT 344
#define SL_KEY_RIGHT_CONTROL 345
#define SL_KEY_RIGHT_ALT 346
#define SL_KEY_RIGHT_SUPER 347

// mouse input symbolic constants
#define SL_MOUSE_BUTTON_1 0
#define SL_MOUSE_BUTTON_2 1
#define SL_MOUSE_BUTTON_3 2
#define SL_MOUSE_BUTTON_4 3
#define SL_MOUSE_BUTTON_5 4
#define SL_MOUSE_BUTTON_6 5
#define SL_MOUSE_BUTTON_7 6
#define SL_MOUSE_BUTTON_8 7
#define SL_MOUSE_BUTTON_LEFT SL_MOUSE_BUTTON_1
#define SL_MOUSE_BUTTON_RIGHT SL_MOUSE_BUTTON_2
#define SL_MOUSE_BUTTON_MIDDLE SL_MOUSE_BUTTON_3

#ifdef __cplusplus
extern "C" {
#endif

	// initialization commands

	void slWindow(int width, int height, const char *title);
	void slClose();
	int slShouldClose();

	// simple input

	int slGetKey(int key);

	int slGetMouseButton(int button);
	void slGetMousePos(int *posX, int *posY);

	// simple frame timing

	float slGetDeltaTime();

	// rendering commands

	void slRender();

	// color control

	void slSetBackColor(float red, float green, float blue);
	void slSetForeColor(float red, float green, float blue, float alpha);

	// blending control

	void slAdditiveBlend(int additiveBlend);

	// transformations

	void slPush();
	void slPop();

	void slTranslate(float x, float y);
	void slRotate(float degrees);
	void slScale(float x, float y);

	// texture loading

	int slLoadTexture(const char *filename);

	// simple shape commands

	void slTriangleFill(float x, float y, float width, float height);
	void slTriangleOutline(float x, float y, float width, float height);

	void slRectangleFill(float x, float y, float width, float height);
	void slRectangleOutline(float x, float y, float width, float height);

	void slCircleFill(float x, float y, float radius, int numVertices);
	void slCircleOutline(float x, float y, float radius, int numVertices);

	void slPoint(float x, float y);

	void slLine(float x1, float y1, float x2, float y2);

	void slSetSpriteTiling(float x, float y);
	void slSetSpriteScroll(float x, float y);
	void slSprite(int texture, float x, float y, float width, float height);

	// text commands

	void slSetTextAlign(int fontAlign);
	float slGetTextWidth(const char *text);
	float slGetTextHeight(const char *text);
	void slSetFont(const char *fontFilename, int fontSize);
	void slSetFontSize(int fontSize);
	void slText(float x, float y, const char *text);

#ifdef __cplusplus
} // closing brace for extern "C"
#endif
