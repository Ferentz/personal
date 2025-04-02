#pragma once
#include "utils.h"
#include <vector>

//class Window;
class DoomCamera final
{
public:
	Vector3f m_Pos;
	Vector3f m_NextPos;
	std::vector<Vector2f> m_Walls;
	float m_projDist;
	//const float m_Pi;
	void pushPosition();
	float m_angleOffset;
	void rayy();
	DoomCamera(const Rectf& window);
	Vector2f PerspCalc(point2D_3D& pointStruc, const Vector3f& point)const;
	Vector2f PerspCalc(point2D_3D& pointStruc)const;
	void PerspCalc(point2D_3D& pointStruc, point2D_3D& pointStruct2)const;
	void intersecCalc(const point2D_3D& visible, point2D_3D& invisible) const;


	bool IsLookingAtMe(const Vector3f crosspoints[])const;
	void CalculateLookAtVector();
	Vector2f GetLookAtVector();

private:
	const Rectf& m_Window;
	Vector2f m_lookVector;


};

