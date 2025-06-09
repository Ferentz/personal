#include "base.h"
#include <cmath>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Vector2f.h"

//-----------------------------------------------------------------
// Vector2f Constructors
//-----------------------------------------------------------------
Vector2f::Vector2f( )
	:Vector2f{ 0.0f, 0.0f }
{
}

Vector2f::Vector2f( float x, float y )
	: x{ x }
	, y{ y }
{
}

Vector2f::Vector2f(int x, int y)
	: x{ float( x ) }
	, y{ float( y)  }
{
}

Vector2f::Vector2f( const Vector2f& fromPoint, const Vector2f& tillPoint )
	: Vector2f{ tillPoint.x - fromPoint.x, tillPoint.y - fromPoint.y }
{
}

// -------------------------
// Methods
// -------------------------
bool Vector2f::Equals(const Vector2f& other, float epsilon) const
{
	return ( abs(x - other.x) < epsilon ) && ( abs(y - other.y) < epsilon );
}

float Vector2f::DotProduct(const Vector2f& other) const
{
	return x * other.x + y * other.y;
}

float Vector2f::CrossProduct(const Vector2f& other) const
{
	return x * other.y - y * other.x;
}

std::string Vector2f::ToString() const
{
	std::stringstream buffer;

	buffer << std::fixed;
	buffer << std::setprecision( 2 );
	buffer << "Vector2f(" <<  x  << ", " <<  y  << ")";
	return buffer.str();
}

float Vector2f::Norm() const
{
	return Length();
}

float Vector2f::Length() const
{
	return sqrt( x * x + y * y );
}

float Vector2f::SquaredLength() const
{
	return x * x + y * y;
}

float Vector2f::AngleWith(const Vector2f& other) const
{
	//https://stackoverflow.com/questions/21483999/using-atan2-to-find-angle-between-two-vectors
	// not calling dot and cross functions to prevent the overhead
	return atan2(x * other.y - other.x * y, x * other.x + y * other.y);
}

Vector2f Vector2f::Normalized(float epsilon) const
{
	float length{ Length( ) };
	if ( length < epsilon )
	{
		return Vector2f{ 0, 0 };
	}
	else
	{
		return Vector2f{ x / length, y / length };
	}
}

Vector2f Vector2f::Orthogonal() const
{
	return Vector2f{ -y, x };
}

Vector2f Vector2f::Reflect( const Vector2f& surfaceNormal ) const
{
	return (*this) - 2 * ( this->DotProduct( surfaceNormal ) * surfaceNormal );
}

bool Vector2f::WithinSegmentBounds(Vector2f segStart, Vector2f segEnd)
{
	const float EPSILON = 0.01f; // Small tolerance for floating-point errors

	return (x >= std::min(segStart.x, segEnd.x) - EPSILON && 
		x <= std::max(segStart.x, segEnd.x) + EPSILON &&
		y >= std::min(segStart.y, segEnd.y) - EPSILON && 
		y <= std::max(segStart.y, segEnd.y) + EPSILON);
}


// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are collinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int Vector2f::orientation(Vector2f p, Vector2f q)
{
	// See https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
	// for details of below formula. 
	Vector2f segment{ p, q };
	Vector2f toPoint{ p, *this };
	float val = segment.CrossProduct(toPoint);

	// Handle floating-point precision errors
	//if (std::abs(val) < 1e-6) return 0;  // Collinear
	if (std::abs(val) < 0.001f) return 0;  // Collinear

	return (val > 0) ? 1 : -1; // 1: Left (Counterclockwise), -1: Right (Clockwise)
}

bool Vector2f::doIntersect(Vector2f p1, Vector2f p2, Vector2f s1, Vector2f s2,
	Vector2f& res) {

	// Find the four orientations needed for general and
	// special cases
	int o1 = s1.orientation(p1, p2);
	int o2 = s2.orientation(p1, p2);
	int o3 = p1.orientation(s1, s2);
	int o4 = p2.orientation(s1, s2);

	// General case: lines intersect if they have different
	// orientations
	if (o1 != o2 && o3 != o4) {

		// Compute intersection point
		float a1 = p2.y - p1.y;
		float b1 = p1.x - p2.x;
		float c1 = a1 * p1.x + b1 * p1.y;

		float a2 = s2.y - s1.y;
		float b2 = s1.x - s2.x;
		float c2 = a2 * s1.x + b2 * s1.y;

		float determinant = a1 * b2 - a2 * b1;

		if (determinant != 0) {
			res.x = (c1 * b2 - c2 * b1) / determinant;
			res.y = (a1 * c2 - a2 * c1) / determinant;
			return true;
		}
	}

	// Special Cases: check if the lines are collinear and
	// overlap
	if (o1 == 0 && s1.WithinSegmentBounds(p1, p2))
	{
		res = s1;
		return true;
	}
	if (o2 == 0 && s2.WithinSegmentBounds(p1, p2))
	{
		res = s2;
		return true;
	}
	if (o3 == 0 && p1.WithinSegmentBounds(s1, s2))
	{
		res = p1;
		return true;
	}
	if (o4 == 0 && p2.WithinSegmentBounds(s1, s2))
	{
		res = p2;
		return true;
	}

	// Lines do not intersect in any case
	return false;
}


bool Vector2f::intersection(Vector2f p1, Vector2f p2, Vector2f s1, Vector2f s2, Vector2f& res) {
	// Line AB represented as a1x + b1y = c1
	float a = p2.y - p1.y;
	float b = p1.x - p2.x;
	float c = a * (p1.x) + b * (p1.y);
	// Line CD represented as a2x + b2y = c2
	float a1 = s2.y - s1.y;
	float b1 = s1.x - s2.x;
	float c1 = a1 * (s1.x) + b1 * (s1.y);
	float det = a * b1 - a1 * b;
	if (det== 0)
	{
		return false;
	}
	else {
		float x = (b1 * c - b * c1) / det;
		float y = (a * c1 - a1 * c) / det;
		res = Vector2f{ x, y };
		return true;
	}
}

void Vector2f::Set(float newX, float newY)
{
	x = newX;
	y = newY;
}

// -------------------------
// Member operators
// -------------------------
Vector2f Vector2f::operator-( ) const
{
	return Vector2f{ -x, -y };
}
Vector2f Vector2f::operator+ ( ) const
{
	return Vector2f{ x, y };
}

Vector2f& Vector2f::operator*=(float rhs)
{
	x *= rhs;
	y *= rhs;
	return *this;
}

Vector2f& Vector2f::operator/=(float rhs)
{
	*this *= 1 / rhs;
	return *this;
}

Vector2f& Vector2f::operator+=(const Vector2f& rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

Vector2f& Vector2f::operator-=(const Vector2f& rhs)
{
	*this += -rhs;
	return *this;
}

Vector2f::operator Vector2f()
{
	return Vector2f{ x,y };
}

// -------------------------
// Non-member operators
// -------------------------
Vector2f operator*( float lhs, Vector2f rhs )
{
	return rhs *= lhs;
}

Vector2f operator*( Vector2f lhs, float rhs )
{
	return lhs *= rhs;
}

Vector2f operator/( Vector2f lhs, float rhs )
{
	return lhs *= (1 / rhs);
}

Vector2f operator+( Vector2f lhs, const Vector2f& rhs )
{
	return lhs += rhs;
}

Vector2f operator-( Vector2f lhs, const Vector2f& rhs )
{
	return lhs += -rhs;
}

bool operator==(Vector2f lhs, const Vector2f& rhs)
{
	if (lhs.x == rhs.x && lhs.y == rhs.y)
	{
		return true;
	}
	return false;
}

bool operator!=(Vector2f lhs, const Vector2f& rhs)
{
	float e{ 0.00001f };
	//if (lhs.x != rhs.x || lhs.y != rhs.y)
	if ((lhs.x - rhs.x) * (lhs.x - rhs.x) > e || (lhs.y - rhs.y) * (lhs.y - rhs.y) > e)
	{
		return true;
	}
	return false;
}

std::ostream& operator<< ( std::ostream& lhs, const Vector2f& rhs )
{
	lhs << rhs.ToString( );
	return lhs;
}