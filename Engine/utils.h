#pragma once
#include <vector>
#include <cmath>
#include "Vector2f.h"
#include "Vector3f.h"

class SoundStream;
class SoundEffect;

namespace utils
{
	const float g_Pi{ 3.1415926535f };

#pragma region OpenGLDrawFunctionality

	void SetColor( const Color4f& color );
	void SetColor(float R, float G, float B, float A = 1.f);
	
	void DrawPoint( float x, float y, float pointSize = 1.0f );
	void DrawPoint( const Vector2f& p, float pointSize = 1.0f );
	void DrawPoints( Vector2f *pVertices, int nrVertices, float pointSize = 1.0f );

	void DrawLine( float x1, float y1, float x2, float y2, float lineWidth = 1.0f );
	void DrawLine( const Vector2f& p1, const Vector2f& p2, float lineWidth = 1.0f );

	void DrawTriangle(const Vector2f& p1, const Vector2f& p2, const Vector2f& p3, float lineWidth = 1);
	void FillTriangle(const Vector2f& p1, const Vector2f& p2, const Vector2f& p3);

	void DrawRect(float left, float bottom, float width, float height, float lineWidth = 1.0f);
	void DrawRect(const Vector2f& bottomLeft, float width, float height, float lineWidth = 1.0f);
	void DrawRect(const Rectf& rect, float lineWidth = 1.0f);
	void FillRect(float left, float bottom, float width, float height);
	void FillRect(const Vector2f& bottomLeft, float width, float height);
	void FillRect(const Rectf& rect);

	void DrawEllipse(float centerX, float centerY, float radX, float radY, float lineWidth = 1.0f);
	void DrawEllipse(const Vector2f& center, float radX, float radY, float lineWidth = 1.0f);
	void DrawEllipse(const Ellipsef& ellipse , float lineWidth = 1.0f );
	void FillEllipse( float centerX, float centerY, float radX, float radY );
	void FillEllipse(const Ellipsef& ellipse );
	void FillEllipse(const Vector2f& center, float radX, float radY);

	// Draws an arc. The angle parameters are in radians, not in degrees.
	void DrawArc(float centerX, float centerY, float radX, float radY, float fromAngle, float tillAngle, float lineWidth = 1.0f);
	// Draws an arc. The angle parameters are in radians, not in degrees.
	void DrawArc(const Vector2f& center, float radX, float radY, float fromAngle, float tillAngle, float lineWidth = 1.0f);
	// Fills an arc. The angle parameters are in radians, not in degrees.
	void FillArc(float centerX, float centerY, float radX, float radY, float fromAngle, float tillAngle);
	// Fills an arc. The angle parameters are in radians, not in degrees.
	void FillArc(const Vector2f& center, float radX, float radY, float fromAngle, float tillAngle);

	void DrawPolygon( const std::vector<Vector2f>& vertices, bool closed = true, float lineWidth = 1.0f );
	void DrawPolygon( const Vector2f* pVertices, size_t nrVertices, bool closed = true, float lineWidth = 1.0f );
	void FillPolygon( const std::vector<Vector2f>& vertices);
	void FillPolygon( const Vector2f* pVertices, size_t nrVertices);

	void FillPolygon3D(const Vector3f* pVertices, size_t nrVertices);
	void FillPolygon3DStrip(const Vector3f* pVertices, size_t nrVertices);
	void FillPolygon3DTris(const Vector3f* pVertices, size_t nrVertices);
#pragma endregion OpenGLDrawFunctionality

#pragma region CollisionFunctionality
	struct HitInfo
	{
		float lambda;
		Vector2f intersectPoint;
		Vector2f normal;
	};

	float GetDistance(float x1, float y1, float x2, float y2);
	float GetDistance(const Vector2f& p1, const Vector2f& p2);

	bool IsPointInRect(const Vector2f& p, const Rectf& r);
	bool IsPointInCircle(const Vector2f& p, const Circlef& c);
	bool IsPointInPolygon( const Vector2f& p, const std::vector<Vector2f>& vertices );
	bool IsPointInPolygon( const Vector2f& p, const Vector2f* vertices, size_t nrVertices );

	bool IsOverlapping( const Vector2f& a, const Vector2f& b, const Circlef& c );
	bool IsOverlapping( const Vector2f& a, const Vector2f& b, const Rectf& r );

	bool IsOverlapping(const Rectf & r1, const Rectf & r2);
	bool IsOverlapping( const Rectf& r, const Circlef& c );
	bool IsOverlapping( const Circlef& c1, const Circlef& c2 );
	bool IsOverlapping( const std::vector<Vector2f>& vertices, const Circlef& c );
	bool IsOverlapping( const Vector2f* vertices, size_t nrVertices, const Circlef& c );
	bool Raycast( const Vector2f* vertices, const size_t nrVertices, const Vector2f& rayP1, const Vector2f& rayP2, HitInfo& hitInfo );
	bool Raycast( const std::vector<Vector2f>& vertices, const Vector2f& rayP1, const Vector2f& rayP2, HitInfo& hitInfo );

	bool IntersectLineSegments(const Vector2f& p1, const Vector2f& p2, const Vector2f& q1, const Vector2f& q2, float& outLambda1, float& outLambda2, float epsilon = 1e-6);
	float DistPointLineSegment(const Vector2f& p, const Vector2f& a, const Vector2f& b);
	bool IsPointOnLineSegment(const Vector2f& p, const Vector2f& a, const Vector2f& b);
	bool IntersectRectLine(const Rectf& r, const Vector2f& p1, const Vector2f& p2, float& intersectMin, float& intersectMax);

#pragma endregion CollisionFunctionality
#pragma region FileHandelingFunctionality
	//LMP ReadLMP(const std::string& filename, int16_t**& outArray, int elementsPerRow);
	LMP ReadLMP(const std::string& filename, int elementsPerRow);
	void ReadTexLMP(const std::string& filename, std::vector<PatchData>& patches);
	void ReadTexData(std::vector<int8_t> data, int baseTextureDataIndex, std::vector<int32_t>& offsets, std::vector<PatchData>& patches);
	void ReadPatchData(std::vector<int8_t> data, int basePatchDataIndex, int16_t patchCount, std::vector<PatchData>& patches);
	std::string convertToCharString(const int16_t* stringStart);
	short InvertInt(uint16_t field);
#pragma endregion FileHandelingFunctionality
	
	int getArrIndex(int arrLen, int index);

	int GetBSP(Vector2f& pos, const bool returnLeaf, const std::vector<BSPnode>& nodes);

	int GetBSP(Vector2f& pos, const int node, const bool returnLeaf, const std::vector<BSPnode>& nodes);

	enum class ThingType {
		other,
		monster,
		deco,
		obstacle,
		weapon,
		ammunition,
		powerUp,
		artifact
	};

	enum class ThingsEnum {
		defaultThing = 0,
		playerStart = 1,
		player2Start = 2,
		player3Start = 3,
		player4Start = 4,
		ShotgunGuy = 9,
		BloodyMess = 10,
		DeathmatchStart = 11,
		BloodyMess2 = 12,
		DeadPlayer = 15,
		PoolOfBloodAndFlesh = 24,
		Candelabra = 35,
		TallTechnoColumn = 48,
		Shotgun = 2001,
		Chaingun = 2002,
		RocketLauncher = 2003,
		Clip = 2007,
		shotgunShells4 = 2008,
		Stimpack = 2011,
		Medikit = 2012,
		HealthBonus = 2014,
		ArmorBonus = 2015,
		Armor = 2018,
		Megaarmor = 2019,
		FloorLamp = 2028,
		ExplodingBarrel = 2035,
		BoxOfRockets = 2046,
		BoxOfBullets = 2048,
		BoxOfShotgunShells = 2049,
		Imp = 3001,
		Zombieman = 3004,

	};
	
	/// <summary>
	/// 4 priority levels
	/// 1 = smallest -> most quiet
	/// 4 = largest -> loudest
	/// </summary>
	/// <param name="sound"></param>
	/// <param name="priorityLevel"></param>
	//void PlaySoundwPriority(SoundStream& sound, int priorityLevel, bool loop);
	//void PlaySoundwPriority(SoundEffect& sound, int priorityLevel, bool loop);
}