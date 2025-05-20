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
	Vector3f m_StartPos{ 1056, -3616 ,10 };
	Vector3f m_Pos{ 1056, -3616 ,10 };
	Vector3f m_NextPos;
	void pushPosition();
	bool ColideWLevel(const Level* pLevel);
	float m_angleOffset;
	PLayer(const Rectf& window);
	Vector2f PerspCalc(point2D_3D& pointStruc)const;
	void intersecCalc(const point2D_3D& visible, point2D_3D& invisible) const;
	
	
	void VelocCalc(float elapsedSec);

	void Shoot(std::vector<Thing*> thingsToShoot, const std::vector<Plane*>& r_pPlanes, float windowWidth);
	void Jump();
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void Turn(float offset);

	bool IsLookingAtMe(const Vector3f crosspoints[])const;
	void CalculateLookAtVector();

	void ResetPos();
	Vector2f GetLookAtVector() const;
	HealthManager m_HealthManager;
	WeaponManager m_WeaponManager;
	

private:
	Camera m_Camera;
	const Rectf& m_Window;

	int m_charDIameter{ 20 };
	int m_charHeight{ 40 };
	
	Vector2f m_lookVector;

	const float m_GRAVITY{ 9.807 };
	bool m_Standing{ false };

	float m_vericalVelocity{};
	float m_projDist;
	float m_FLoorHeigt{};
};

