#pragma once
#include "utils.h"
#include <vector>
#include "HealthManager.h"
#include "WeaponManager.h"
#include "Camera.h"
#include "SoundEffect.h"
//class Window;
class Level;
class Thing;
class Plane;
//class HealthManager;
class Player final
{
public:
	explicit Player(const Rectf& window);

	//basics
	bool UpdatePlayer(float elapsedsecs, Level* pLevel);

	//input
	void Shoot(std::vector<Thing*> thingsToShoot, const std::vector<Plane*>& r_pPlanes, float windowWidth); // clean up ref names
	void Jump();
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void Turn(float offset);
	void SwitchWeapon(int weaponIndex);

	//other
	void ResetPos();
	const Vector2f& GetLookAtVector(bool invert = false) const;
	const Camera& GetCamera() const;
	const Vector3f& GetPos()const;

	//public since the classes themselves maintain what can or cannot be done
	HealthManager m_HealthManager;
	WeaponManager m_WeaponManager;

private:
	//update helpers
	void pushPosition();
	bool ColideWLevel(const Level* pLevel);
	void VelocCalc(float elapsedSec);
	int m_PrevHealth{};
	SoundEffect m_HurtSound{"sounds/DSOOF.wav"};
	SoundEffect m_NearDeath{ "sounds/DSPLPAIN.wav" };

	//move helper
	void Move(float x, float y);


	Vector3f m_StartPos{ 1056, -3616 ,10 };
	Vector3f m_Pos{ m_StartPos };
	Vector3f m_NextPos{ m_StartPos };

	Camera m_Camera;

	const float m_charDIameter{ 20.f };
	const int m_CharHeight{ 40 };
	
	//movement
	const float m_Speed{ 5.f };
	const float m_Gravity{ 9.807f };
	const float m_JumpnFallSpeed{7.f}; // implemented after elapsedsecs got aplied. allows you to keep the ratio between jump strength and gravity
	const float m_JumpVelocity{ 20.f };
	bool m_Standing{ false };

	//function handled
	float m_VericalVelocity{};
	float m_FLoorHeigt{};
	Vector2f m_lookVector{};
	Vector2f m_InvertedLookVector{};
};

