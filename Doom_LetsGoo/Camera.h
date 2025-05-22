#pragma once
class Camera
{
public:
	Camera(const Rectf& window, const Vector3f& pos);
	Vector2f PerspCalc(point2D_3D& pointStruc)const;
	void intersecCalc(const point2D_3D& visible, point2D_3D& invisible) const;
	const Vector2f MakeLookAtVector() const;
	void TurnCamera(float offset);
	float GetDistanceFromMidle(float xScreenPos);
	float GetWindowWidth();
private:
	const Rectf& m_Window;
	const Vector3f& m_Pos;
	const float m_projDist{ 2.0f };
	float m_angleOffset{0};
};

