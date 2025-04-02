#include "pch.h"
#include "Game.h"
#include <ios>
#include <iostream>


Game::Game( const Window& window ) 
	:BaseGame{ window },
	m_Camera{ GetViewPort()},
	m_WindowWidth{window.width}
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	glEnable(GL_DEPTH_TEST);
	m_pLevel = new Level();
	m_pLevel->InitializeWallls();
}

void Game::Cleanup( )
{
	delete m_pLevel;
}

void Game::Update( float elapsedSec )
{
	// Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}
	m_pLevel->UpdateLevel(m_Camera, elapsedSec);
	m_Camera.m_Pos = m_Camera.m_NextPos;

}

void Game::Draw( ) const
{
	BaseGame::Draw();
	ClearBackground( );
	m_pLevel->DrawLevel();;

}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	SDL_Keycode key = e.keysym.sym;
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
	switch (key)
	{
	case SDLK_LEFT:
		//g_eyePos.x = g_eyePos.x + increment;
		m_Camera.m_NextPos.x += -m_Camera.GetLookAtVector().y;//cos(Eye.m_offset) * e.y;
		m_Camera.m_NextPos.y -= m_Camera.GetLookAtVector().x;//sin(Eye.m_offset) * e.y;

		break;
	case SDLK_RIGHT:
		//g_eyePos.x = g_eyePos.x - increment;
		m_Camera.m_NextPos.x += m_Camera.GetLookAtVector().y;//cos(Eye.m_offset) * e.y;
		m_Camera.m_NextPos.y -= -m_Camera.GetLookAtVector().x;//sin(Eye.m_offset) * e.y;
		break;
	case SDLK_UP:
		//g_eyePos.z = g_eyePos.z + increment;
		//Eye.m_eyePos.x = Eye.m_eyePos.x + Eye.getLookkVector().x;
		//Eye.m_eyePos.y = Eye.m_eyePos.y + Eye.getLookkVector().y;
		m_pLevel->m_subSectorToDraw++;
		break;
	case SDLK_DOWN:
		//g_eyePos.z = g_eyePos.z - increment;
		m_pLevel->m_subSectorToDraw--;
		break;
	case SDLK_a:
		m_Camera.m_NextPos.x += -m_Camera.GetLookAtVector().y;//cos(Eye.m_offset) * e.y;
		m_Camera.m_NextPos.y -= m_Camera.GetLookAtVector().x;//sin(Eye.m_offset) * e.y;
		break;
	case SDLK_d:
		m_Camera.m_NextPos.x += m_Camera.GetLookAtVector().y;//cos(Eye.m_offset) * e.y;
		m_Camera.m_NextPos.y -= -m_Camera.GetLookAtVector().x;//sin(Eye.m_offset) * e.y;
		break;
	case SDLK_w:
		m_Camera.m_NextPos.x += m_Camera.GetLookAtVector().x;
		m_Camera.m_NextPos.y -= m_Camera.GetLookAtVector().y;
		break;
	case SDLK_s:
		m_Camera.m_NextPos.x -= m_Camera.GetLookAtVector().x;
		m_Camera.m_NextPos.y += m_Camera.GetLookAtVector().y;
		break;
	case SDLK_q:
		m_Camera.m_NextPos.z += 1;
		break;
	case SDLK_e:
		m_Camera.m_NextPos.z -= 1;
		break;
		//case SDLK_1:
		//case SDLK_KP_1:

	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
	// SAMPLE CODE: print mouse position
	//const float mouseX{ float(e.x) }; //.xrel
	const float mouseX{ float(e.xrel) }; //.xrel
	const float mouseY{ float(e.y) };
	const float mousPosX{ float(e.x) };
	if (mousPosX <= 1)
	{
		//SDL_WarpMouseGlobal(mousPosX + GetViewPort().width - 2, mouseY);
		SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), GetViewPort().width - 2, mouseY);
		//SDL_WarpMouseInWindow
	}
	if (mousPosX >= GetViewPort().width - 1)
	{
		//SDL_WarpMouseGlobal(mousPosX + GetViewPort().width - 2, mouseY);
		SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), 2, mouseY);
		//SDL_WarpMouseInWindow
	}
	
	const float mult = 50 * M_PI / GetViewPort().width;// g_WindowWidth;
	//g_offset = ((mouseX - g_WindowWidth / 2) * 1)/100;
	m_Camera.m_angleOffset += mouseX / 100;

	if (m_Camera.m_angleOffset > M_PI) {
		m_Camera.m_angleOffset = -M_PI + (int(m_Camera.m_angleOffset * 100) % int(M_PI * 100)) / 100.f;
	}
	if (m_Camera.m_angleOffset < -M_PI) {
		m_Camera.m_angleOffset = M_PI + (int(m_Camera.m_angleOffset * 100) % int(-M_PI * 100)) / 100.f;
	}
	m_Camera.CalculateLookAtVector();
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	std::cout << "MOUSEBUTTONUP event: ";
	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		std::cout << " left button " << std::endl;
		break;
	case SDL_BUTTON_RIGHT:
		std::cout << " right button " << std::endl;
		break;
	case SDL_BUTTON_MIDDLE:
		std::cout << " middle button " << std::endl;
		break;
	}
}

void Game::ProcessMouseWheelEvent( const SDL_MouseWheelEvent& e)
{

	float increment{ 10.f };
	//Eye.m_eyePos.y = Eye.m_eyePos.y + increment * e.y;
	m_Camera.m_NextPos.x += cos(m_Camera.m_angleOffset) * e.y * increment;
	m_Camera.m_NextPos.y -= sin(m_Camera.m_angleOffset) * e.y * increment;


}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	//glClear( GL_COLOR_BUFFER_BIT );
	//glClearDepthf(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


