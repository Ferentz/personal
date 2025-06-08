#include "pch.h"
#include "Game.h"
#include <ios>
#include <iostream>
#include "UI.h"
#include "Level.h"
#include "utils.h"

Game::Game(const Window& window)
	:BaseGame{ window },
	m_Player{ GetViewPort() },
	m_pUI{ new UI{GetViewPort(), &m_Player} },
	m_pLevel{ new Level(m_Player) }
{
	//m_pUI->AssignPlayer(&m_Player);
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
}

void Game::Cleanup( )
{
	delete m_pLevel;
}

void Game::Update( float elapsedSec )
{
	switch (m_State)
	{
	case Game::GameState::menu:
		break;
	case Game::GameState::alive:
	{
		m_pUI->UpdateUI(elapsedSec);

		

		m_pLevel->UpdateLevel(m_Player, elapsedSec);

		if (m_Player.UpdatePlayer(elapsedSec, m_pLevel))
		{
			SDL_ShowCursor(SDL_ENABLE);  // shows the cursor
			m_State = GameState::won;
		}
		if (m_Player.m_HealthManager.GetHealth() <= 0)
		{
			SDL_ShowCursor(SDL_ENABLE);  // shows the cursor
			m_State = GameState::dead;
		}

		Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);
		if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			m_Player.Shoot(m_pLevel->GetThings(), m_pLevel->GetWalls(), GetWindow().width);

		}

		// Check keyboard state
		const Uint8* pStates = SDL_GetKeyboardState(nullptr);
		if (pStates[SDL_SCANCODE_A])
		{
			m_Player.MoveLeft();
		}
		if (pStates[SDL_SCANCODE_D])
		{
			m_Player.MoveRight();
		}
		if (pStates[SDL_SCANCODE_W])
		{
			m_Player.MoveForward();
		}
		if (pStates[SDL_SCANCODE_S])
		{
			m_Player.MoveBackward();
		}

		break;
	}
	case Game::GameState::dead:
		break;
	default:
		break;
	}
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
		
		m_pUI->DrawUI(GetWindow().width,
			m_Player.m_HealthManager.GetHealth(), m_Player.m_HealthManager.GetArmourHealth(),
			m_Player.m_WeaponManager.GetAmo());
		Vector2f pos{ m_Player.GetPos().x, m_Player.GetPos().y};
		m_pLevel->DrawLevel( pos);

		m_pUI->DrawOverlay();

		}
		break;
	case Game::GameState::dead:
		break;
	default:
		break;
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	SDL_Keycode key = e.keysym.sym;
	switch (key)
	{
	case SDLK_1:
		m_Player.SwitchWeapon(1);
		break;
	case SDLK_2:
		m_Player.SwitchWeapon(2);
		break;
	case SDLK_LEFT:
		break;
	case SDLK_RIGHT:
		break;
	case SDLK_UP:

		break;
	case SDLK_DOWN:
		break;
	//case SDLK_a:
	//	m_Player.MoveLeft();
	//	break;
	//case SDLK_d:
	//	m_Player.MoveRight();
	//	break;
	//case SDLK_w:
	//	m_Player.MoveForward();
	//	break;
	//case SDLK_s:
	//	m_Player.MoveBackward();
	//	break;
	case SDLK_q:
		break;
	case SDLK_e:
		break;
	case SDLK_SPACE:
		m_Player.Jump();
		break;
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
	const int mouseX{ int(e.xrel) };
	const int mouseY{ int(e.y) };
	const int mousPosX{ int(e.x) };
	if (mousPosX <= 1)
	{
		SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), int(GetViewPort().width) - 2, mouseY);
	}
	if (mousPosX >= GetViewPort().width - 1)
	{
		SDL_WarpMouseInWindow(SDL_GL_GetCurrentWindow(), 2, mouseY);
	}
	
	
	m_Player.Turn(-mouseX / 100.f);
	
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
		switch (m_State)
		{
		case Game::GameState::menu:
			m_State = GameState::alive;

			utils::PlaySoundwPriority(m_pLevel->m_LevelMusic, 2, true);
				//SOUND m_pLevel->m_LevelMusic
			
			SDL_ShowCursor(SDL_DISABLE);  // Hides the cursor
			EnterLevel();
			break;
		case Game::GameState::alive:
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
	/*float increment{ 10.f };
	m_Player.m_NextPos.x += m_Player.GetLookAtVector().x * e.y * increment;
	m_Player.m_NextPos.y -= m_Player.GetLookAtVector().y * e.y * increment;*/
}

void Game::ClearBackground( ) const
{
	switch (m_State)
	{
	case Game::GameState::menu:
		glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
		break;
	case Game::GameState::alive:
		glClearColor(0.f, 0.f, 0.f, 0.f);
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
	m_Player.ResetPos();
}
