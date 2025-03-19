#pragma once
#include <string>
#include "Vector2f.h"
#include "Vector3f.h"

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
	LMP(int16_t** Arr2D, int rows, int colums);
	~LMP();
	const int16_t* const* data;
	const int rows;
	const int colums;
};

struct Line {
	Vector2f startPoint;
	Vector2f endPoint;
};

struct point2D_3D {
	// see if this can be turned into a function rather than a vriable
	point2D_3D();
	point2D_3D(float x, float y, float z);
	Vector2f point2D{};
	Vector3f point3D;
	float Angle[3]{}; // rename this
	bool amIVisible{ true };
	float Dist3D{};
	float prevAngle[2]{};
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



