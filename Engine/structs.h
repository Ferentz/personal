#pragma once
#include <string>
#include "Vector2f.h"
#include "Vector3f.h"
#include <vector>

struct Window
{
	explicit Window( const std::string& title = "Title", float width = 320.0f, 
		float height = 180.0f, bool isVSyncOn = true );

	std::string title;
	float width;
	float height;
	bool isVSyncOn;
};

struct LMP {
	LMP(int16_t** Arr2D, int rows, unsigned int colums);
	~LMP();
	LMP(const LMP& toCopy) = delete;
	LMP(LMP&& rhs) noexcept;
	LMP& operator=(const LMP& rhs) = delete;
	LMP& operator=(LMP&& rhs) = delete;
	const int16_t* const* data; // 2D array, only the outer pointer can change for move semantics
	const int rows;
	const unsigned int colums;
};

struct Line {
	Vector2f startPoint;
	Vector2f endPoint;
};

struct BSPnode {
	Line partitionLine{};
	int children[2]{};
	int parent{};
};

struct BSPleafeNode
{
	int parent{};
	int startSeg{};
	int amountOfSegs{};
	float floorheight{};
	std::vector<int> wallIndexes;
	std::vector<int> thingIndexes;
};

struct PatchData
{
	uint16_t xOffset{};
	uint16_t yOffset{};
	uint16_t patchNum{};
};

struct Int2 {
	Int2(int x, int y) :u_offset{ x }, v_offset{ y } {}
	int u_offset;
	int v_offset;
};

struct Patch {
	Patch(std::string nameIn, int xIn, int yIn)
		:name{nameIn},
		x{xIn},
		y{yIn}
	{};
	std::string name;
	int x;
	int y;
};
struct WallTexture {
	WallTexture(std::string nameIn, int xIn, int yIn) : name{ nameIn }, x{ xIn }, y{ yIn } {};
	std::string name;
	int x;
	int y;
	std::vector<Patch> patches{};
};

struct point2D_3D {
	// see if this can be turned into a function rather than a vriable
	point2D_3D();
	point2D_3D(float x, float y, float z);
	Vector2f point2D{};
	Vector3f point3D;
	float AngleXYUsed[3]{};
	bool amIVisible{ true };
	float Dist3D{};
	float prevAngleXY[2]{};
};

struct Rectf
{
	Rectf( );
	explicit Rectf( float left, float bottom, float width, float height );
	
	float left;
	float bottom;
	float width;
	float height;

};


struct Color4f
{
	Color4f( );
	explicit Color4f( float r, float g, float b, float a );
	
	float r;
	float g;
	float b;
	float a;
};

struct Circlef
{
	Circlef( );
	explicit Circlef( const Vector2f& center, float radius );
	explicit Circlef( float centerX, float centerY, float radius );

	Vector2f center;
	float radius;
};


struct Ellipsef
{
	Ellipsef( );
	explicit Ellipsef( const Vector2f& center, float radiusX, float radiusY );
	explicit Ellipsef( float centerX, float centerY, float radiusX, float radiusY );

	Vector2f center;
	float radiusX;
	float radiusY;
};



