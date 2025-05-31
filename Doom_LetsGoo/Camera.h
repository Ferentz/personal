#pragma once
class Camera
{
public:
	explicit Camera(const Rectf& window, const Vector3f& pos);

	Vector2f PerspCalc(point2D_3D& pointStruc)const;
	bool ClipLineToFOV(point2D_3D& frontPt, point2D_3D& backPt) const;
	void intersecCalc(const point2D_3D& visible, point2D_3D& invisible) const;
	const Vector2f MakeLookAtVector() const;
	void TurnCamera(float offset);
	float GetDistanceFromMidle(float xScreenPos) const;
	float GetWindowWidth() const;

private:
	const Rectf& m_Window;
	const Vector3f& m_Pos;
	const float m_projDist{ 2.0f };
	float m_angleOffset{0};
};

//
//Vector2f Camera::PerspCalc(point2D_3D& pointStruc) const
//{
//	const float DEVISION = m_Window.width / M_PI; // curently magic number. should get linked up with the window size but havent found how to do that yet in a way that i like
//	const float SCALE{ 1.5f };
//	Vector3f point{ pointStruc.point3D };
//	float yDist{ point.y - m_Pos.y };
//	float hightDist{ point.z - m_Pos.z };
//	float xDist{ (point.x - m_Pos.x) };
//	float floorDist{};
//	floorDist = sqrt(xDist * xDist + yDist * yDist);
//
//	float xAngle{ float(atan2(yDist, xDist)) };
//	float yAngle{};
//
//
//	// se we got our originl angle, but we wanna calculate it according to the x axes. so, we rotate it with the negative offset
//	float newXAngle = xAngle + (m_angleOffset);
//
//	if (newXAngle < -M_PI) {
//		newXAngle = newXAngle + 2 * M_PI;
//	}
//	if (newXAngle > M_PI) {
//		newXAngle = newXAngle - 2 * M_PI;
//	}
//	float diagonalDist{ float(sqrt(xDist * xDist + yDist * yDist)) };
//	pointStruc.Dist3D = float(sqrt(diagonalDist * diagonalDist + hightDist * hightDist));
//	float newXDist = cos(newXAngle) * diagonalDist; // should be good. cos times hypotheneus
//	float newYDist = sin(newXAngle) * diagonalDist;
//
//	yAngle = float(atan2(hightDist, newXDist));
//
//	pointStruc.AngleXYUsed[0] = xAngle;
//	pointStruc.AngleXYUsed[1] = yAngle;
//	pointStruc.prevAngleXY[0] = pointStruc.AngleXYUsed[0];
//	pointStruc.prevAngleXY[1] = pointStruc.AngleXYUsed[1];
//	if (newXDist > 0)
//	{
//		pointStruc.amIVisible = true;
//	}
//	else
//	{
//		pointStruc.amIVisible = false;
//	}
//
//	Vector2f Point2D{};
//	Point2D.y = SCALE * ((yAngle)*m_projDist) * DEVISION + m_Window.height / 2;
//
//	//the following code serves to map the points when they are passing behind you, as -pi would be all the wway to the left, and pi all the way to the right.
//	// When the point passed behind you, you dont want it to jump from left to right
//	// this is only releveant if it's posiblt to move through walls. to be tested if it's still necesary when you can walk through the level.
//	int direction{ 1 };
//
//	if (newYDist < m_projDist && newXAngle > M_PI / 2) {
//		direction = 1;
//	}
//	else if (newYDist > -m_projDist && newXAngle < -M_PI / 2) {
//		direction = -1;
//	}
//	else {
//		Point2D.x = SCALE * ((newXAngle)*m_projDist) * DEVISION + m_Window.width / 2;
//		pointStruc.AngleXYUsed[2] = (newXAngle)*m_projDist;
//		pointStruc.point2D = Point2D;
//		return Point2D;
//	}
//
//	float restAngle = direction * M_PI - newXAngle;
//	float entryAngle = atan2(direction * m_projDist, newXDist);
//	float entryAngleRest = direction * M_PI - entryAngle;
//	float result = restAngle * entryAngle / entryAngleRest;
//
//	Point2D.x = SCALE * (result * m_projDist) * DEVISION + m_Window.width / 2;
//	pointStruc.AngleXYUsed[2] = newXAngle;
//	pointStruc.point2D = Point2D;
//	return Point2D;
//}