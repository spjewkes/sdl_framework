#ifndef __SDL_FRAMEWORK_HPP__
#define __SDL_FRAMEWORK_HPP__

#include <iostream>
#include <exception>
#include <chrono>
#include <SDL2/SDL.h>

using namespace std;

struct KeyState
{
	bool bPressed;
	bool bReleased;
	bool bHeld;
};

class SDLException : public std::exception
{
public:
	SDLException(const char *m) : mesg(m) {}

	virtual const char* what() const throw()
		{
			return mesg;
		}

private:
	const char *mesg;
};

class SDLFramework
{
public:
	SDLFramework(int width, int height);
	virtual ~SDLFramework();

	virtual bool OnCreate() = 0;
	virtual bool OnUpdate(float elapsed_time) = 0;
	virtual void OnDestroy() = 0;

	void Start();

protected:
	virtual const char* GetName() { return "SDLFramework"; }
	
	SDL_Window* GetWindow() { return window; }
	SDL_Renderer* GetRenderer() { return renderer; }
	int ScreenWidth() const { return w; }
	int ScreenHeight() const { return h; }

	void* LockScreen();
	void UnlockScreen();

	int GetMouseX() const { return m_mousePosX; }
	int GetMouseY() const { return m_mousePosY; }
	KeyState GetMouse(int buttonID) const { return m_mouse[buttonID]; }

	void DrawLine(int x0, int y0, int x1, int y1);
	void FillCircle(int xc, int yc, int r);

	void FillBox(int x0, int y0, int x1, int y1);
	void FillPolygon(int num_points, int *points);
	void FillTrapezoid(int y_top, int y_bottom, int x0_top, int x1_top, int x0_bottom, int x1_bottom);
	void FillTriangle(int x0, int y0, int x1, int y1, int x2, int y2);

	void SetDrawColor(int r, int g, int b, int a);
	void ClearScreen(int r, int g, int b, int a);

private:
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	int w = 0;
	int h = 0;

	KeyState m_mouse[2];

	int m_mousePosX;
	int m_mousePosY;
};

#endif
