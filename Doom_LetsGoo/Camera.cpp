#include "pch.h"
#include "Camera.h"

Camera::Camera(const Rectf& window, const Vector3f& pos)
	:m_Window{ window },
	m_Pos{pos}
{
}

Vector2f Camera::PerspCalc(point2D_3D& pointStruc) const
{
	const float DEVISION = m_Window.width / M_PI; // curently magic number. should get linked up with the window size but havent found how to do that yet in a way that i like
	const float SCALE{ 1.5f };
	Vector3f point{ pointStruc.point3D };
	float yDist{ point.y - m_Pos.y };
	float hightDist{ point.z - m_Pos.z };
	float xDist{ (point.x - m_Pos.x) };
	float floorDist{};
	floorDist = sqrt(xDist * xDist + yDist * yDist);

	float xAngle{ float(atan2(yDist, xDist)) };
	float yAngle{};


	// se we got our originl angle, but we wanna calculate it according to the x axes. so, we rotate it with the negative offset
	float newXAngle = xAngle + (m_angleOffset);

	if (newXAngle < -M_PI) {
		newXAngle = newXAngle + 2 * M_PI;
	}
	if (newXAngle > M_PI) {
		newXAngle = newXAngle - 2 * M_PI;
	}
	float diagonalDist{ float(sqrt(xDist * xDist + yDist * yDist)) };
	pointStruc.Dist3D = float(sqrt(diagonalDist * diagonalDist + hightDist * hightDist));
	float newXDist = cos(newXAngle) * diagonalDist; // should be good. cos times hypotheneus
	float newYDist = sin(newXAngle) * diagonalDist;

	yAngle = float(atan2(hightDist, newXDist));

	pointStruc.Angle[0] = xAngle;
	pointStruc.Angle[1] = yAngle;
	pointStruc.prevAngle[0] = pointStruc.Angle[0];
	pointStruc.prevAngle[1] = pointStruc.Angle[1];
	if (newXDist > 0)
	{
		pointStruc.amIVisible = true;
	}
	else
	{
		pointStruc.amIVisible = false;
	}

	Vector2f Point2D{};
	Point2D.y = SCALE * ((yAngle)*m_projDist) * DEVISION + m_Window.height / 2;

	//the following code serves to map the points when they are passing behind you, as -pi would be all the wway to the left, and pi all the way to the right.
	// When the point passed behind you, you dont want it to jump from left to right
	// this is only releveant if it's posiblt to move through walls. to be tested if it's still necesary when you can walk through the level.
	int direction{ 1 };

	if (newYDist < m_projDist && newXAngle > M_PI / 2) {
		direction = 1;
	}
	else if (newYDist > -m_projDist && newXAngle < -M_PI / 2) {
		direction = -1;
	}
	else {
		Point2D.x = SCALE * ((newXAngle)*m_projDist) * DEVISION + m_Window.width / 2;
		pointStruc.Angle[2] = (newXAngle)*m_projDist;
		pointStruc.point2D = Point2D;
		return Point2D;
	}

	float restAngle = direction * M_PI - newXAngle;
	float entryAngle = atan2(direction * m_projDist, newXDist);
	float entryAngleRest = direction * M_PI - entryAngle;
	float result = restAngle * entryAngle / entryAngleRest;

	Point2D.x = SCALE * (result * m_projDist) * DEVISION + m_Window.width / 2;
	pointStruc.Angle[2] = newXAngle;
	pointStruc.point2D = Point2D;
	return Point2D;
}

void Camera::intersecCalc(const point2D_3D& visible, point2D_3D& invisible) const
{
	// we will need, the visible point and the invisible point

	const float DEVISION = 900 / M_PI; // curently magic number. should get linked up with the window size but havent found how to do that yet in a way that i like
	const float SCALE{ 1.f };

	const Vector3f point_invis{ invisible.point3D };

	float yDist_invis{ point_invis.y - m_Pos.y };
	float xDist_invis{ (point_invis.x - m_Pos.x) };
	float xAngle_invis{ float(atan2(yDist_invis, xDist_invis)) };

	// se we got our originl angle, but we wanna calculate it according to the x axes. so, we rotate it with the negative offset
	float newXAngle_invis = xAngle_invis + (m_angleOffset);

	if (newXAngle_invis < -M_PI) {
		newXAngle_invis = newXAngle_invis + 2 * M_PI;
	}
	if (newXAngle_invis > M_PI) {
		newXAngle_invis = newXAngle_invis - 2 * M_PI;
	}

	float diagonalDist_invis{ float(sqrt(xDist_invis * xDist_invis + yDist_invis * yDist_invis)) };
	float newXDist_invis = cos(newXAngle_invis) * diagonalDist_invis; // should be good. cos times hypotheneus
	float newYDist_invis = sin(newXAngle_invis) * diagonalDist_invis;
	//---------------------------------------------------------------------------------------------------------------------------
	const Vector3f point_vis{ visible.point3D };

	float yDist_vis{ point_vis.y - m_Pos.y };
	float xDist_vis{ (point_vis.x - m_Pos.x) };
	float xAngle_vis{ float(atan2(yDist_vis, xDist_vis)) };

	// se we got our originl angle, but we wanna calculate it according to the x axes. so, we rotate it with the negative offset
	float newXAngle_vis = xAngle_vis + (m_angleOffset);

	if (newXAngle_vis < -M_PI) {
		newXAngle_vis = newXAngle_vis + 2 * M_PI;
	}
	if (newXAngle_vis > M_PI) {
		newXAngle_vis = newXAngle_vis - 2 * M_PI;
	}
	float diagonalDist_vis{ float(sqrt(xDist_vis * xDist_vis + yDist_vis * yDist_vis)) };
	float newXDist_vis = cos(newXAngle_vis) * diagonalDist_vis; // should be good. cos times hypotheneus
	float newYDist_vis = sin(newXAngle_vis) * diagonalDist_vis;


	Vector2f visible2D{ newXDist_vis , newYDist_vis };
	Vector2f invisible2D{ newXDist_invis , newYDist_invis };
	Vector2f result{};
	if (Vector2f::doIntersect(Vector2f{ -1000.f,0.f }, Vector2f{ 1000.f,0.f }, visible2D, invisible2D, result))
	{
		atan2(result.x, result.y);
		float hightDist_invis{ invisible.point3D.z - m_Pos.z };
		float yAngle_invis = float(atan2(hightDist_invis, newXDist_invis));

		Vector2f Point2D{};
		Point2D.y = SCALE * ((yAngle_invis)*m_projDist) * DEVISION + 900 / 2;
		Point2D.x = SCALE * ((newXAngle_invis)*m_projDist) * DEVISION + 900 / 2;

		invisible.point2D = Point2D;
	}
	//utils::Raycast(LINE_SEGMENT, CENTER, m_MousePos, hitInfo)
}

const Vector2f Camera::MakeLookAtVector() const
{
	return Vector2f{ cos(m_angleOffset), sin(m_angleOffset) };
}

void Camera::TurnCamera(float offset)
{
	m_angleOffset += offset;

	if (m_angleOffset > M_PI) {
		m_angleOffset = -M_PI + (int(m_angleOffset * 100) % int(M_PI * 100)) / 100.f;
	}
	if (m_angleOffset < -M_PI) {
		m_angleOffset = M_PI + (int(m_angleOffset * 100) % int(-M_PI * 100)) / 100.f;
	}
}
