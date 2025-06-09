#include "pch.h"
#include "Camera.h"
#include <iostream>

Camera::Camera(const Rectf& window, const Vector3f& pos)
	:r_Window{ window },
	r_Pos{pos}
{
}

void Camera::TurnCamera(float offset)
{
	m_angleOffset += offset;

	if (m_angleOffset > M_PI) {
		m_angleOffset = -float(M_PI) + (int(m_angleOffset * 100) % int(M_PI * 100)) / 100.f; // /100 and *100 to be able to keep .00 preceision with modulo
	}
	if (m_angleOffset < -M_PI) {
		m_angleOffset = float(M_PI) + (int(m_angleOffset * 100) % int(-M_PI * 100)) / 100.f;
	}
}

#pragma region 3dTo2d

/// <summary>
/// takes a single point in world space and calculates it's 2d position on screen based on the camera position and rotation.
/// Adjusts : AngleXYUsed, prevAngleXY, amIVisible, point2D
/// </summary>
/// <param name="pointStruct" >
/// </param>
/// <returns>
/// Vector2f screen position
/// </returns>
Vector2f Camera::PerspectiveCalculation(point2D_3D& pointStruct) const
{
	const float DEVISION = r_Window.width / float(M_PI);
	const float SCALE{ 1.5f };
	Vector3f point{ pointStruct.point3D };
	float yDist{ point.y - r_Pos.y };
	float hightDist{ point.z - r_Pos.z };
	float xDist{ (point.x - r_Pos.x) };

	float xAngle{ float(atan2(yDist, xDist)) };
	float yAngle{};


	// se we got our originl angle, but we wanna calculate it according to the x axes. so, we rotate it with the offset
	float newXAngle = xAngle + (m_angleOffset);

	if (newXAngle < -M_PI) {
		newXAngle = newXAngle + 2 * float(M_PI);
	}
	if (newXAngle > M_PI) {
		newXAngle = newXAngle - 2 * float(M_PI);
	}
	float diagonalDist{ float(sqrt(xDist * xDist + yDist * yDist)) };
	pointStruct.Dist3D = float(sqrt(diagonalDist * diagonalDist + hightDist * hightDist));
	float newXDist = cos(newXAngle) * diagonalDist;
	float newYDist = sin(newXAngle) * diagonalDist;

	yAngle = float(atan2(hightDist, newXDist));

	pointStruct.AngleXYUsed[0] = xAngle;
	pointStruct.AngleXYUsed[1] = yAngle;
	pointStruct.prevAngleXY[0] = pointStruct.AngleXYUsed[0];
	pointStruct.prevAngleXY[1] = pointStruct.AngleXYUsed[1];
	if (newXDist > 0)
	{
		pointStruct.amIVisible = true;
	}
	else
	{
		pointStruct.amIVisible = false;
	}

	Vector2f Point2D{};
	Point2D.y = SCALE * ((yAngle)*m_projDist) * DEVISION + r_Window.height / 2;

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
		Point2D.x = SCALE * ((newXAngle)*m_projDist) * DEVISION + r_Window.width / 2;
		pointStruct.AngleXYUsed[2] = (newXAngle)*m_projDist;
		pointStruct.point2D = Point2D;
		return Point2D;
	}

	float restAngle = direction * float(M_PI) - newXAngle;
	float entryAngle = atan2(direction * m_projDist, newXDist);
	float entryAngleRest = direction * float(M_PI) - entryAngle;
	float result = restAngle * entryAngle / entryAngleRest;

	Point2D.x = SCALE * (result * m_projDist) * DEVISION + r_Window.width / 2;
	pointStruct.AngleXYUsed[2] = newXAngle;
	pointStruct.point2D = Point2D;
	return Point2D;
}

bool Camera::ClipLineToFOV(point2D_3D& frontPt, point2D_3D& backPt) const
{
	point2D_3D clippedPt{};
	auto wrapAngle = [](float angle) {
		while (angle < -M_PI) angle += float(2 * M_PI);
		while (angle > M_PI) angle -= float(2 * M_PI);
		return angle;
		};

	Vector3f vis3D = frontPt.point3D;
	Vector3f invis3D = backPt.point3D;

	Vector3f vis3D_repos = vis3D - r_Pos;
	Vector3f invis3D_repos = invis3D - r_Pos;

	float angleVis = atan2(vis3D_repos.y, vis3D_repos.x) + m_angleOffset;
	float angleInvis = atan2(invis3D_repos.y, invis3D_repos.x) + m_angleOffset;

	angleVis = wrapAngle(angleVis);
	angleInvis = wrapAngle(angleInvis);

	auto shortestAngleDist = [](float from, float to) {
		float diff = to - from;
		while (diff < -M_PI) diff += float(2 * M_PI);
		while (diff > M_PI) diff -= float (2 * M_PI);
		return diff;
		};

	// Wrap angles to [-?, ?]
	angleVis = wrapAngle(angleVis);
	angleInvis = wrapAngle(angleInvis);
	//--------------------------------
	/*Vector2f relativeVis{ cos(angleVis), sin(angleVis) };
	Vector2f relativeInvis{ cos(angleInvis), sin(angleInvis) };
	Vector2f relativeResult{};
	Vector2f::doIntersect(Vector2f{ -1000.f,0.f }, Vector2f{ 1000.f,0.f }, relativeVis, relativeInvis, relativeResult);
	float dist2D{ sqrt(relativeResult.x* relativeResult.x + relativeResult.y * relativeResult.y) };
	float relativeResultAngle{ atan2(relativeResult.y, relativeResult.x) };
	float resultAngle(relativeResultAngle - m_angleOffset);
	Vector3f result{ cos(resultAngle)* dist2D + m_Pos.x,  sin(resultAngle)* dist2D + m_Pos.y, invis3D.z};
	point2D_3D resultPoint{};
	resultPoint.point3D = result;
	backPt.point2D = PerspCalc(resultPoint);
	return true;*/
	//--------------------------------
	float boundary = (angleVis > 0) ? float(M_PI) / 2.0f : -float(M_PI) / 2.0f;


	if (angleVis / abs(angleVis) != angleInvis / abs(angleInvis))
	{
		if (angleVis > 0)
		{
			angleInvis = angleInvis + 2 * float(M_PI);
		}
		else
		{
			angleInvis = angleInvis - 2 * float(M_PI);
		}
		
	}
	// Use shortest angular distance
	float angleAB = angleInvis - angleVis;
	float angleToBoundary = boundary - angleVis;

	// Avoid divide-by-zero
	if (std::abs(angleAB) < 0.0001f) return false;

	float t = angleToBoundary / angleAB;

	Vector3f clippedPos = invis3D + ((vis3D - invis3D ) * t);
	clippedPt.point3D = clippedPos;
	 // calculate the 2D position
	backPt.point2D = PerspectiveCalculation(clippedPt);

	return true;
}

void Camera::intersecCalc(const point2D_3D& visible, point2D_3D& invisible) const
{
	// we will need, the visible point and the invisible point

	const Vector3f point_invis{ invisible.point3D };

	float yDist_invis{ point_invis.y - r_Pos.y };
	float xDist_invis{ (point_invis.x - r_Pos.x) };
	float xAngle_invis{ float(atan2(yDist_invis, xDist_invis)) };

	// se we got our originl angle, but we wanna calculate it according to the x axes. so, we rotate it with the negative offset
	float newXAngle_invis = xAngle_invis + (m_angleOffset);

	if (newXAngle_invis < -M_PI) {
		newXAngle_invis = newXAngle_invis + 2 * float(M_PI);
	}
	if (newXAngle_invis > M_PI) {
		newXAngle_invis = newXAngle_invis - 2 * float(M_PI);
	}

	float diagonalDist_invis{ float(sqrt(xDist_invis * xDist_invis + yDist_invis * yDist_invis)) };
	float newXDist_invis = cos(newXAngle_invis) * diagonalDist_invis; // should be good. cos times hypotheneus
	float newYDist_invis = sin(newXAngle_invis) * diagonalDist_invis;
	//---------------------------------------------------------------------------------------------------------------------------
	const Vector3f point_vis{ visible.point3D };

	float yDist_vis{ point_vis.y - r_Pos.y };
	float xDist_vis{ (point_vis.x - r_Pos.x) };
	float xAngle_vis{ float(atan2(yDist_vis, xDist_vis)) };

	// se we got our originl angle, but we wanna calculate it according to the x axes. so, we rotate it with the negative offset
	float newXAngle_vis = xAngle_vis + (m_angleOffset);

	if (newXAngle_vis < -M_PI) {
		newXAngle_vis = newXAngle_vis + 2 * float(M_PI);
	}
	if (newXAngle_vis > M_PI) {
		newXAngle_vis = newXAngle_vis - 2 * float(M_PI);
	}
	float diagonalDist_vis{ float(sqrt(xDist_vis * xDist_vis + yDist_vis * yDist_vis)) };
	float newXDist_vis = cos(newXAngle_vis) * diagonalDist_vis; // should be good. cos times hypotheneus
	float newYDist_vis = sin(newXAngle_vis) * diagonalDist_vis;


	Vector2f visible2D{ newXDist_vis , newYDist_vis };
	Vector2f invisible2D{ newXDist_invis , newYDist_invis };
	Vector2f result{};
	if (Vector2f::doIntersect(Vector2f{ -1000.f,0.f }, Vector2f{ 1000.f,0.f }, visible2D, invisible2D, result))
	{
		// we got the result of the thing interjecting with the camera shoulder line.
		// we then need to get it's x which...doesnt matter. itgets projected stragthe on -90 0r +90 degrees
		// so... what?
		const float DEVISION = r_Window.width / float(M_PI); // curently magic number. should get linked up with the window size but havent found how to do that yet in a way that i like
		const float SCALE{ 1.f };
		Vector2f Point2D{};

		float newXDist = 0;
		float newYDist = result.y;
		//

		float hightDist{ point_invis.z - r_Pos.z };
		float yAngle = float(atan2(hightDist, newXDist));
		Point2D.y = SCALE * ((yAngle)*m_projDist) * DEVISION + r_Window.height / 2;
		Point2D.x = SCALE * float(atan2(result.y, 0) * m_projDist) * DEVISION + r_Window.width / 2;
	
		invisible.point2D = Point2D;
	}
	//utils::Raycast(LINE_SEGMENT, CENTER, m_MousePos, hitInfo)
}

#pragma endregion 3dTo2d

#pragma region camProperties

const Vector2f Camera::MakeLookAtVector(bool invert) const
{
	if(invert)
	{
		return Vector2f{ float(cos(-m_angleOffset)), float(sin(-m_angleOffset)) };

	}
	return Vector2f{ float(cos(m_angleOffset)), float(sin(m_angleOffset)) };
}

float Camera::GetDistanceFromMidle(float xScreenPos) const
{
	return xScreenPos - r_Window.width / 2;
}

float Camera::GetWindowWidth() const
{
	return r_Window.width;
}

#pragma endregion camProperties