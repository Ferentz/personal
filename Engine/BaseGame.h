#pragma once
#include "structs.h"
#include "SDL.h"
#include <map>
// https://BaseGameprogrammingpatterns.com/subclass-sandbox.html


class BaseGame
{
public:
	explicit BaseGame( const Window& window );
	BaseGame( const BaseGame& other ) = delete;
	BaseGame& operator=( const BaseGame& other ) = delete;
	BaseGame(BaseGame&& other) = delete;
	BaseGame& operator=(BaseGame&& other) = delete;
	virtual ~BaseGame( );

	void Run( );

	virtual void Update(float elapsedSec)
	{
		
	}
	virtual void Draw() const
	{
		glDepthFunc(GL_NOTEQUAL);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// Set up a two-dimensional orthographic viewing region.
		glOrtho(0, m_Window.width, 0, m_Window.height, -1, 1); // y from bottom to top

		// Set the viewport to the client window area
		// The viewport is the rectangular region of the window where the image is drawn.
		glViewport(0, 0, int(m_Window.width), int(m_Window.height));

		// Set the Modelview matrix to the identity matrix
		glMatrixMode(GL_MODELVIEW);
	}

	// Event handling
	virtual void ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
	{
		
	}
	virtual void ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
	{
		
	}
	virtual void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
	{
		
	}
	virtual void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
	{
		
	}
	virtual void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
	{
		
	}
	virtual void ProcessMouseWheelEvent(const SDL_MouseWheelEvent& e)
	{

	}

	const Rectf& GetViewPort() const
	{
		return m_Viewport;
	}
	const Window& GetWindow() const
	{
		return m_Window;
	}

private:
	// DATA MEMBERS
	// The window properties
	Window m_Window;
	Rectf m_Viewport;
	// The window we render to
	SDL_Window* m_pWindow;
	// OpenGL context
	SDL_GLContext m_pContext;
	// Init info
	bool m_Initialized;
	// Prevent timing jumps when debugging
	const float m_MaxElapsedSeconds;
	
	// FUNCTIONS
	void InitializeGameEngine( );
	void CleanupGameEngine( );
	// Controller helper functions
	void InitControllerMap();
	bool HandleControllerButton(SDL_Event& e, Uint32 type);
	void HandleControllerAnalogyJoystick(Sint16 value, SDL_Scancode keyNeg, SDL_Scancode keyPos);
	void SendKeyEvent(SDL_Scancode scanCode, Uint32 type);
	const static int AXIS_DEADZONE = 8000;
	// Controller mappings
	std::map<Uint8, SDL_Scancode> m_ControllerMap;
};
