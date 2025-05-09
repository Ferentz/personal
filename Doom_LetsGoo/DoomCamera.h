#pragma once
#include "utils.h"
#include <vector>

//class Window;
class Level;

class DoomCamera final
{
public:
	Vector3f m_Pos;
	Vector3f m_NextPos;
	std::vector<Vector2f> m_Walls;
	float m_projDist;
	void pushPosition();
	int ColideWLevel(const Level* pLevel);
	float m_angleOffset;
	void rayy();
	DoomCamera(const Rectf& window);
	Vector2f PerspCalc(point2D_3D& pointStruc, const Vector3f& point)const;
	Vector2f PerspCalc(point2D_3D& pointStruc)const;
	void PerspCalc(point2D_3D& pointStruc, point2D_3D& pointStruct2)const;
	void intersecCalc(const point2D_3D& visible, point2D_3D& invisible) const;
	float m_GRAVITY{ 9.807 };
	bool m_Standing{ false };
	float m_vericalVelocity{};
	void VelocCalc(float elapsedSec);
	float m_FLoorHeigt{};

	bool IsLookingAtMe(const Vector3f crosspoints[])const;
	void CalculateLookAtVector();
	Vector2f GetLookAtVector() const;
	void ChangeHealth(int damageOheal);
	int GetHealth() const;

private:
	int m_health{ 100 };
	const Rectf& m_Window;
	int m_charDIameter{ 20 };
	int m_charHeight{ 40 };
	
	Vector2f m_lookVector;


};

