#pragma once
class Camera
{
public:
	explicit Camera(const Rectf& window, const Vector3f& pos);

	void TurnCamera(float offset);

	Vector2f PerspectiveCalculation(point2D_3D& pointStruct)const;
	bool ClipLineToFOV(point2D_3D& frontPt, point2D_3D& backPt) const;
	void intersecCalc(const point2D_3D& visible, point2D_3D& invisible) const;

	const Vector2f MakeLookAtVector( bool ivert = false) const;
	float GetDistanceFromMidle(float xScreenPos) const;
	float GetWindowWidth() const;

private:
	const float m_projDist{ 2.0f };
	float m_angleOffset{ 0 };

	const Rectf& r_Window;
	const Vector3f& r_Pos;
};