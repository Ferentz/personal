#pragma once
#include "BaseGame.h"
#include "Player.h"


class UI;
class Level;

class Game final: public BaseGame
{
public:
	explicit Game( const Window& window );
	~Game();

	//rule of 5
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game(Game&& other) = delete;
	Game& operator=(Game&& other) = delete;

	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseWheelEvent(const SDL_MouseWheelEvent& e);

private:

	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
	void EnterLevel();

	Player m_Player;
	UI* m_pUI;
	Level* m_pLevel;
	enum class GameState
	{
		menu,
		alive,
		dead,
		won
	};
	GameState m_State{ GameState::menu };
};