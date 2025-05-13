#pragma once
#include "BaseGame.h"
#include <vector>
#include "Texture.h"
#include "Utils.h"
#include "Level.h"
#include "DoomCamera.h"
class UI;
class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

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

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
	void EnterLevel();
	float leftBound{ 268 };
	float bottomBounf{ 38 };
	float scale{ 2 };
	//Texture* m_UI;
	UI* m_pUI{};
	Level* m_pLevel{};
	DoomCamera m_Camera;
	float m_WindowWidth;
	enum class GameState
	{
		menu,
		alive,
		dead,
		won
	};
	GameState m_State{};
};