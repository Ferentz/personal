#pragma once
#include "utils.h"
#include <vector>
#include "HealthManager.h"
#include "WeaponManager.h"
#include "Camera.h"
//class Window;
class Level;
class Thing;
class Plane;
//class HealthManager;
class PLayer final
{
public:
	PLayer(const Rectf& window);

	void pushPosition();
	bool ColideWLevel(const Level* pLevel);
	void VelocCalc(float elapsedSec);

	void Shoot(std::vector<Thing*> thingsToShoot, const std::vector<Plane*>& r_pPlanes, float windowWidth); // clean up ref names
	void Jump();
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void Turn(float offset);

	bool IsLookingAtMe(const Vector3f crosspoints[])const;
	void ResetPos();
	const Vector2f& GetLookAtVector() const;
	const Camera& GetCamera() const;

	HealthManager m_HealthManager;
	WeaponManager m_WeaponManager;
	
	Vector3f m_StartPos{ 1056, -3616 ,10 };
	Vector3f m_Pos{ 1056, -3616 ,10 };
	Vector3f m_NextPos;

private:
	Camera m_Camera;

	int m_charDIameter{ 20 };
	int m_charHeight{ 40 };
	
	Vector2f m_lookVector;

	const float m_GRAVITY{ 9.807 };
	bool m_Standing{ false };
	//name consistency
	float m_vericalVelocity{};
	float m_FLoorHeigt{};
};

