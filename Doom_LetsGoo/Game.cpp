#include "pch.h"
#include "Game.h"
#include <ios>
#include <iostream>
#include "UI.h"

// need to start thinking on ui

Game::Game(const Window& window)
	:BaseGame{ window },
	m_Camera{ GetViewPort() },
	m_WindowWidth{ window.width },
	m_pUI{ new UI{GetViewPort()} }
	//m_UI{ new Texture{"STBAR_converted.png"} }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
	delete m_pUI;
}

void Game::Initialize( )
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	
	m_pLevel = new Level(m_Camera);
	m_pUI->AssignPlayer(&m_Camera);
}

void Game::Cleanup( )
{
	if (m_pLevel != nullptr)
	{
		delete m_pLevel;
	}
	
}

void Game::Update( float elapsedSec )
{
	switch (m_State)
	{
	case Game::GameState::menu:
		break;
	case Game::GameState::alive:
		if (!m_pLevel->m_LevelMusic.IsPlaying())
		{
			m_pLevel->m_LevelMusic.Play(true);
		}
		m_Camera.UpdatePlayer(elapsedSec);
		m_pLevel->UpdateLevel(m_Camera, elapsedSec);
		if(m_Camera.ColideWLevel(m_pLevel))
		{
			m_State = GameState::won;
		}
		m_Camera.VelocCalc(elapsedSec);
		m_Camera.pushPosition();
		if (m_Camera.m_HealthManager.GetHeath() <= 0)
		{
			m_State = GameState::dead;
		}
		break;
	case Game::GameState::dead:
		break;
	default:
		break;
	}
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
	
	
}

void Game::Draw( ) const
{
	BaseGame::Draw();
	ClearBackground();
	glDepthFunc(GL_LESS);
	switch (m_State)
	{
	case Game::GameState::menu:
		break;
	case Game::GameState::alive:
		{
		//------UI propper
		//UI myUI{};
		m_pUI->DrawUI(GetWindow().width, bottomBounf, leftBound, scale,
			m_Camera.m_HealthManager.GetHeath(), m_Camera.m_HealthManager.GetArmour(),
			m_Camera.m_WeaponManager.GetAmo());

		//------gameplay
		//m_pLevel->DrawLevelUnordered();
		Vector2f pos{ m_Camera.m_Pos.x, m_Camera.m_Pos.y };
		int leaf{ utils::GetBSP(pos, true, m_pLevel->GetBSPtree()) };
		m_pLevel->DrawLevelBSP(leaf, pos);

		//------UI old
		//glDepthFunc(GL_ALWAYS);
		//m_Camera.GetHealth();
		//float width{ GetWindow().width };
		}
		break;
	case Game::GameState::dead:
		break;
	default:
		break;
	}

	//float uiWidth{ m_UI->GetWidth() };
	//float ratio{ uiWidth / width };

	//Rectf hi{ 0, 0, width, m_UI->GetHeight()/ratio};
	//m_UI->Draw(hi);

}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	SDL_Keycode key = e.keysym.sym;
	switch (key)
	{
	case SDLK_1:
		m_Camera.SwitchWeapon(1);
		break;
	case SDLK_2:
		m_Camera.SwitchWeapon(2);
		break;
	case SDLK_LEFT:
		leftBound--;
		std::cout << leftBound << " leftOffset" << std::endl;
		//g_eyePos.x = g_eyePos.x + increment;
		//m_Camera.m_NextPos.x += -m_Camera.GetLookAtVector().y;//cos(Eye.m_offset) * e.y;
		//m_Camera.m_NextPos.y -= m_Camera.GetLookAtVector().x;//sin(Eye.m_offset) * e.y;

		break;
	case SDLK_RIGHT:
		leftBound++;
		std::cout << leftBound << " leftOffset" << std::endl;
		//g_eyePos.x = g_eyePos.x - increment;
		//m_Camera.m_NextPos.x += m_Camera.GetLookAtVector().y;//cos(Eye.m_offset) * e.y;
		//m_Camera.m_NextPos.y -= -m_Camera.GetLookAtVector().x;//sin(Eye.m_offset) * e.y;
		break;
	case SDLK_UP:
		//bottomBounf++;
		//std::cout << bottomBounf << " bottomOffset" << std::endl;
		//g_eyePos.z = g_eyePos.z + increment;
		//Eye.m_eyePos.x = Eye.m_eyePos.x + Eye.getLookkVector().x;
		//Eye.m_eyePos.y = Eye.m_eyePos.y + Eye.getLookkVector().y;
		//m_pLevel->m_subSectorToDraw++;
		break;
	case SDLK_DOWN:
		//bottomBounf--;
		//std::cout << bottomBounf << " bottomOffset" << std::endl;
		//g_eyePos.z = g_eyePos.z - increment;
		//m_pLevel->m_subSectorToDraw--;
		break;
	case SDLK_a:
		//scale--;
		//std::cout << scale << " scale" << std::endl;
		m_Camera.MoveLeft();
		break;
	case SDLK_d:
		//scale++;
		m_Camera.MoveRight();
		//std::cout << scale << " scale" << std::endl;
		
		break;
	case SDLK_w:
		m_Camera.MoveForward();
		
		break;
	case SDLK_s:
		m_Camera.MoveBackward();
		
		break;
	case SDLK_q:
		break;
	case SDLK_e:
		break;
	case SDLK_SPACE:
		m_Camera.Jump();
		
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
	
	
	m_Camera.Turn(mouseX / 100);
	
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		switch (m_State)
		{
		case Game::GameState::menu:
			m_State = GameState::alive;
			EnterLevel();
			break;
		case Game::GameState::alive:
			m_Camera.Shoot(m_pLevel->GetThings(), m_pLevel->GetWalls(), m_WindowWidth);
			break;
		case GameState::won:

		case Game::GameState::dead:
			m_State = GameState::menu;
			break;
		default:
			break;
		}
		
		break;
	case SDL_BUTTON_RIGHT:
		break;
	case SDL_BUTTON_MIDDLE:
		break;
	}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	/*std::cout << "MOUSEBUTTONUP event: ";
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
	}*/
}

void Game::ProcessMouseWheelEvent( const SDL_MouseWheelEvent& e)
{

	float increment{ 10.f };
	//Eye.m_eyePos.y = Eye.m_eyePos.y + increment * e.y;
	m_Camera.m_NextPos.x += m_Camera.GetLookAtVector().x * e.y * increment;
	m_Camera.m_NextPos.y -= m_Camera.GetLookAtVector().y * e.y * increment;


}

void Game::ClearBackground( ) const
{
	switch (m_State)
	{
	case Game::GameState::menu:
		glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
		break;
	case Game::GameState::alive:
		glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	break;
	case GameState::won:
		glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
		break;
	case Game::GameState::dead:
		glClearColor(0.3f, 0.0f, 0.0f, 1.0f);
		break;
	default:
		glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
		break;
	}
	
	//glClear( GL_COLOR_BUFFER_BIT );
	//glClearDepthf(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::EnterLevel()
{
	//m_Camera.m_HealthManager.FullHealth();
	m_Camera.ResetPos();
}
