#include "pch.h"
#include "DoomCamera.h"
#include <iostream>
#include "Level.h"
#include "Plane.h"
#include "Things.h"
#include "Navigator.h"
//#include "game.h"

// next we need to work on jump

// get the trigger event in game to shoot.
//then conect to get the level things ad check if smth gets hit

DoomCamera::DoomCamera(const Rectf& window)
	:
	m_NextPos{ m_Pos },
	m_projDist{ 2.0f },
	m_angleOffset{ 0 },
	m_lookVector{ 0,0 },
	m_Window{window},
	m_HealthManager{500},
	m_WeaponManager{}
{
}

void DoomCamera::pushPosition()
{
	m_Pos = m_NextPos;
}

//so, we wanna call a function here that then does the colision.
// perhaps the player can have a reference to the level.
// then the player, in collision calls eveything.
// we want a function that receives the level as a const
bool DoomCamera::ColideWLevel(const Level* pLevel)
{
	bool exit{ false };
	pLevel->ColideWLevel(m_Pos, m_NextPos, m_FLoorHeigt, m_charDIameter, true, exit);
	return exit;
}


//
//void DoomCamera::ColideWLevel(const Level* pLevel)
//{
//	//int charHeight{ 50 };
//	float NextHeight{};
//	Vector2f pos{ m_Pos.x,  m_Pos.y };
//	//Vector2f posNext{ m_NextPos.x,  m_NextPos.y };
//
//	// we get the curent node that the player is in
//	int leaf{ pLevel->getBSP(pos, true) };
//	//int node{ pLevel->getBSP(pos, false) };
//	// we set the floor height to the one of the current leaf.
//	NextHeight = pLevel->GetFloorOfLeaf(leaf);
//
//	//we  check if it crosses the border
//	//pLevel->crossesBSPborder(pos, posNext, node);
//	Vector3f curPos{ m_Pos };
//	Vector3f nextPos{ m_NextPos };
//	
//	
//	bool lookForHit{ true };
//	int whileDepth{};
//	// we loop over the walls of the curent leaf
//	while (lookForHit && whileDepth < 10)
//	{
//		if (whileDepth > 0)
//		{
//			std::cout << whileDepth << std::endl;
//		}
//		
//		Vector3f interPosNorm{ Vector3f{nextPos - curPos}.Normalized() };
//		Vector2f returnPos{};
//		lookForHit = false;
//		for (int i{}; i < pLevel->GetLeafWallAmount(leaf); i++)
//		{
//			// we get each wall
//			const Plane* wall{ pLevel->GetWallFromLeaf(leaf, i) };
//			// we check if the player colides with a wall
//			// if it was to keep it's cuurent direction.
//			if (wall->PlaneCollision(curPos, nextPos + (interPosNorm * m_charDIameter), returnPos))
//			{
//				// if it does collide, we receive the reflected position
//
//				// and ket the floorheight of the new leafnode
//				leaf = pLevel->getBSP(returnPos, true);
//				NextHeight = pLevel->GetFloorOfLeaf(leaf);
//				nextPos.x = returnPos.x;
//				nextPos.y = returnPos.y;
//				nextPos.z = NextHeight;
//				lookForHit = true;
//				break;
//			}
//		}
//		whileDepth++;
//	}
//	
//
//	m_NextPos.x = nextPos.x;
//	m_NextPos.y = nextPos.y;
//	m_FLoorHeigt = NextHeight;
//
//}

// we can check if the 2 posses are on the same orientation to the partition line. if they are, just check curent leaf.
// else check what the next one is and if the collision makes sense.
// the we just safe the node, and if it's negative, we can save it.

Vector2f DoomCamera::PerspCalc(point2D_3D& pointStruc, const Vector3f& point) const {
	//std::cout << m_Window.width << std::endl;
	const float DEVISION = m_Window.width / M_PI; // curently magic number. should get linked up with the window size but havent found how to do that yet in a way that i like
	const float SCALE{ 1.5f };
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

Vector2f DoomCamera::PerspCalc(point2D_3D& pointStruc) const {
	//std::cout << m_Window.width << std::endl;
	const float DEVISION = m_Window.width / M_PI; // curently magic number. should get linked up with the window size but havent found how to do that yet in a way that i like
	const float SCALE{ 1.f };

	const Vector3f point{ pointStruc.point3D };

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

	Vector2f Point2D{};
	Point2D.y = SCALE * ((yAngle)*m_projDist) * DEVISION + m_Window.height / 2;

	//the following code serves to map the points when they are passing behind you, as -pi would be all the wway to the left, and pi all the way to the right.
	// When the point passed behind you, you dont want it to jump from left to right
	// this is only releveant if it's posiblt to move through walls. to be tested if it's still necesary when you can walk through the level.

	Point2D.x = SCALE * ((newXAngle)*m_projDist) * DEVISION + m_Window.width / 2;
	pointStruc.Angle[2] = (newXAngle)*m_projDist;

	if (pointStruc.Angle[2] > M_PI * 0.5f || pointStruc.Angle[2] < -M_PI * 0.5f) {
		pointStruc.amIVisible = false;
	}
	else {
		pointStruc.amIVisible = true;
	}

	return Point2D;

}

void DoomCamera::intersecCalc(const point2D_3D& visible, point2D_3D& invisible) const
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

void DoomCamera::VelocCalc(float elapsedSec)
{
	m_vericalVelocity -= (m_GRAVITY * elapsedSec);
	m_NextPos.z = m_Pos.z + m_vericalVelocity;
	if (m_NextPos.z <= m_FLoorHeigt + m_charHeight)
	{
		m_vericalVelocity = 0;
		m_NextPos.z = m_FLoorHeigt + m_charHeight;
		m_Standing = true;
		return;
	}
	
}

void DoomCamera::PerspCalc(point2D_3D& pointStruct1, point2D_3D& pointStruct2) const {
	//pointStruct1.point2D = PerspCalc(pointStruct1);
	//pointStruct2.point2D = PerspCalc(pointStruct2);

	// if one of them is visible, nd the other isnt
	if (pointStruct1.amIVisible != pointStruct2.amIVisible) {
		// we wanna recalculate the 2d of the invisible one as the intersection point between x and the line
		if (pointStruct1.amIVisible)
		{
			intersecCalc(pointStruct1, pointStruct2);
		}
		else
		{
			// we calculate point 1
			intersecCalc(pointStruct2, pointStruct1);
		}
	}
	// so, they've been calculated.
	//now we need to find the intersectionPoint with 
}

void DoomCamera::Shoot(std::vector<Thing*> thingsToShoot, const std::vector<Plane*>& r_pPlanes, float windowWidth)
{
	for (Thing*& object : thingsToShoot)
	{
		if (object->m_pHealthInteractor != nullptr) // if it has health
		{
			bool lineOfSight{ false };
			if (object->m_pNavigator != nullptr)
			{
				lineOfSight = object->m_pNavigator->m_hasLineOfSight;
			}

			if(lineOfSight || Navigator::HasLineOfSight(r_pPlanes, m_Pos, object->m_location.point3D))
			{
				int shootRange{ 100 };
				if (object->m_location.point2D.x < windowWidth / 2 + shootRange && object->m_location.point2D.x > windowWidth / 2 - shootRange)
				{
					object->m_pHealthInteractor->Harm(m_WeaponManager.GetDamageOutput());
					std::cout << "HIT" << std::endl;
				}
				
			}
		}
	}
}

bool DoomCamera::IsLookingAtMe(const Vector3f crosspoints[])const {
	//Vector3f crosResult{};
	Vector3f crosA{crosspoints[1].CrossProduct(crosspoints[0])}; //crosspoints[1].x - crosspoints[0].x, crosspoints[1].y - crosspoints[0].y, crosspoints[1].z - crosspoints[0].z };
	Vector3f crosb{ crosspoints[2].CrossProduct(crosspoints[0]) };//crosspoints[2].x - crosspoints[0].x, crosspoints[2].y - crosspoints[0].y, crosspoints[2].z - crosspoints[0].z };
	//crosspoints[0]

		//void crossProduct(int vect_A[], int vect_B[], int cross_P[])

	Vector3f crosResult{crosA.CrossProduct(crosb)};
	/*crosResult.x = crosA.y * crosb.z - crosA.z * crosb.y;
	crosResult.y = crosA.z * crosb.x - crosA.x * crosb.z;
	crosResult.z = crosA.x * crosb.y - crosA.y * crosb.x;*/

	//int dotProduct(int vect_A[], int vect_B[])
	Vector3f eyeVec{ m_Pos.CrossProduct(crosspoints[0]) }; //m_eyePos.x - crosspoints[0].x, m_eyePos.y - crosspoints[0].y, m_eyePos.z - crosspoints[0].z };

	float dotResult = crosResult.DotProduct(eyeVec); //crosResult.x * eyeVec.x + crosResult.y * eyeVec.y + crosResult.z * eyeVec.z;

	if (dotResult < 0) {
		return false;
	}
	return true;

}

void DoomCamera::CalculateLookAtVector() { // why is it like this? because with the minecraft boat you also dont want the player to move in the direction you are looking
	m_lookVector.x = cos(m_angleOffset);
	m_lookVector.y = sin(m_angleOffset);
}

void DoomCamera::ResetPos()
{
	m_Pos.x = m_StartPos.x;
	m_Pos.y = m_StartPos.y;
	m_Pos.z = m_StartPos.z;

}

Vector2f DoomCamera::GetLookAtVector() const {
	return m_lookVector;
}

void DoomCamera::rayy()
{
	utils::HitInfo hitInfo{};
	Vector2f pos{ m_Pos.x, m_Pos.y };
	Vector2f nextPos{ m_NextPos.x, m_NextPos.y };
	if (utils::Raycast(m_Walls,
		pos,
		nextPos,
		hitInfo))
	{
		const Vector2f RAY{ pos - nextPos };
		const Vector2f REFLECTION{ RAY.Reflect(hitInfo.normal) };
		// basicaly, we want the colision to happen before the character gets moved.
		// so, if there is hit info, it needs to be checked.
	}
}