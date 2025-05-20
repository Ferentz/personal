#include "base.h"
//#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "utils.h"
#include "structs.h"
#include <string>



#pragma region OpenGLDrawFunctionality

void utils::SetColor( const Color4f& color )
{
	glColor4f( color.r, color.g, color.b, color.a );
}

void utils::SetColor(float R, float G, float B, float A)
{
	Color4f color(R, G, B, A);
	SetColor(color);
}

void utils::DrawPoint( float x, float y, float pointSize )
{
	glPointSize( pointSize );
	glBegin( GL_POINTS );
	{
		glVertex2f( x, y );
	}
	glEnd( );
}

void utils::DrawPoint( const Vector2f& p, float pointSize )
{
	DrawPoint( p.x, p.y, pointSize );
}

void utils::DrawPoints( Vector2f *pVertices, int nrVertices, float pointSize )
{
	glPointSize( pointSize );
	glBegin( GL_POINTS );
	{
		for ( int idx{ 0 }; idx < nrVertices; ++idx )
		{
			glVertex2f( pVertices[idx].x, pVertices[idx].y );
		}
	}
	glEnd( );
}

void utils::DrawLine( float x1, float y1, float x2, float y2, float lineWidth )
{
	glLineWidth( lineWidth );
	glBegin( GL_LINES );
	{
		glVertex2f( x1, y1 );
		glVertex2f( x2, y2 );
	}
	glEnd( );
}

void utils::DrawLine( const Vector2f& p1, const Vector2f& p2, float lineWidth )
{
	DrawLine( p1.x, p1.y, p2.x, p2.y, lineWidth );
}

void utils::DrawTriangle(const Vector2f& p1, const Vector2f& p2, const Vector2f& p3, float lineWidth)
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_LOOP);
	{
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
		glVertex2f(p3.x, p3.y);
	}
	glEnd();
}

void utils::FillTriangle(const Vector2f& p1, const Vector2f& p2, const Vector2f& p3)
{
	glBegin(GL_TRIANGLES);
	{
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
		glVertex2f(p3.x, p3.y);
	}
	glEnd();
}

void utils::DrawRect( float left, float bottom, float width, float height, float lineWidth )
{
	if (width > 0 && height > 0 && lineWidth > 0)
	{

		glLineWidth(lineWidth);
		glBegin(GL_LINE_LOOP);
		{
			glVertex2f(left, bottom);
			glVertex2f(left + width, bottom);
			glVertex2f(left + width, bottom + height);
			glVertex2f(left, bottom + height);
		}
		glEnd();
	}
}

void utils::DrawRect( const Vector2f& bottomLeft, float width, float height, float lineWidth )
{
	DrawRect( bottomLeft.x, bottomLeft.y, width, height, lineWidth );
}

void utils::DrawRect( const Rectf& rect, float lineWidth )
{
	DrawRect( rect.left, rect.bottom, rect.width, rect.height, lineWidth );
}

void utils::FillRect( float left, float bottom, float width, float height )
{
	if (width > 0 && height > 0)
	{

		glBegin(GL_POLYGON);
		{
			glVertex2f(left, bottom);
			glVertex2f(left + width, bottom);
			glVertex2f(left + width, bottom + height);
			glVertex2f(left, bottom + height);
		}
		glEnd();
	}
}

void utils::FillRect( const Vector2f& bottomLeft, float width, float height )
{
	FillRect( bottomLeft.x, bottomLeft.y, width, height );
}

void utils::FillRect( const Rectf& rect )
{
	FillRect( rect.left, rect.bottom, rect.width, rect.height );
}

void utils::DrawEllipse( float centerX, float centerY, float radX, float radY, float lineWidth )
{
	if (radX > 0 && radY > 0 && lineWidth > 0)
	{

		float dAngle{ radX > radY ? float(g_Pi / radX) : float(g_Pi / radY) };

		glLineWidth(lineWidth);
		glBegin(GL_LINE_LOOP);
		{
			for (float angle = 0.0; angle < float(2 * g_Pi); angle += dAngle)
			{
				glVertex2f(centerX + radX * cos(angle), centerY + radY * sin(angle));
			}
		}
		glEnd();
	}
}

void utils::DrawEllipse( const Vector2f& center, float radX, float radY, float lineWidth )
{
	DrawEllipse( center.x, center.y, radX, radY, lineWidth );
}

void utils::DrawEllipse( const Ellipsef& ellipse, float lineWidth  )
{
	DrawEllipse( ellipse.center.x, ellipse.center.y, ellipse.radiusX, ellipse.radiusY, lineWidth );
}

void utils::FillEllipse( float centerX, float centerY, float radX, float radY )
{
	if (radX > 0 && radY > 0)
	{

		float dAngle{ radX > radY ? float(g_Pi / radX) : float(g_Pi / radY) };

		glBegin(GL_POLYGON);
		{
			for (float angle = 0.0; angle < float(2 * g_Pi); angle += dAngle)
			{
				glVertex2f(centerX + radX * cos(angle), centerY + radY * sin(angle));
			}
		}
		glEnd();
	}
}

void utils::FillEllipse( const Ellipsef& ellipse )
{
	FillEllipse( ellipse.center.x, ellipse.center.y, ellipse.radiusX, ellipse.radiusY );
}

void utils::FillEllipse( const Vector2f& center, float radX, float radY )
{
	FillEllipse( center.x, center.y, radX, radY );
}

void utils::DrawArc( float centerX, float centerY, float radX, float radY, float fromAngle, float tillAngle, float lineWidth )
{
	if ( fromAngle > tillAngle )
	{
		return;
	}

	float dAngle{ radX > radY ? float( g_Pi / radX ) : float( g_Pi / radY ) };

	glLineWidth( lineWidth );
	glBegin( GL_LINE_STRIP );
	{
		for ( float angle = fromAngle; angle < tillAngle; angle += dAngle )
		{
			glVertex2f( centerX + radX * cos( angle ), centerY + radY * sin( angle ) );
		}
		glVertex2f( centerX + radX * cos( tillAngle ), centerY + radY * sin( tillAngle ) );
	}
	glEnd( );

}

void utils::DrawArc( const Vector2f& center, float radX, float radY, float fromAngle, float tillAngle, float lineWidth )
{
	DrawArc( center.x, center.y, radX, radY, fromAngle, tillAngle, lineWidth );
}

void utils::FillArc( float centerX, float centerY, float radX, float radY, float fromAngle, float tillAngle )
{
	if ( fromAngle > tillAngle )
	{
		return;
	}
	float dAngle{ radX > radY ? float( g_Pi / radX ) : float( g_Pi / radY ) };

	glBegin( GL_POLYGON );
	{
		glVertex2f( centerX, centerY );
		for ( float angle = fromAngle; angle < tillAngle; angle += dAngle )
		{
			glVertex2f( centerX + radX * cos( angle ), centerY + radY * sin( angle ) );
		}
		glVertex2f( centerX + radX * cos( tillAngle ), centerY + radY * sin( tillAngle ) );
	}
	glEnd( );
}

void utils::FillArc( const Vector2f& center, float radX, float radY, float fromAngle, float tillAngle )
{
	FillArc( center.x, center.y, radX, radY, fromAngle, tillAngle );
}

void utils::DrawPolygon( const std::vector<Vector2f>& vertices, bool closed, float lineWidth )
{
	DrawPolygon( vertices.data( ), vertices.size( ), closed, lineWidth );
}

void utils::DrawPolygon( const Vector2f* pVertices, size_t nrVertices, bool closed, float lineWidth )
{
	glLineWidth( lineWidth );
	closed ? glBegin( GL_LINE_LOOP ) : glBegin( GL_LINE_STRIP );
	{
		for ( size_t idx{ 0 }; idx < nrVertices; ++idx )
		{
			glVertex2f( pVertices[idx].x, pVertices[idx].y );
		}
	}
	glEnd( );
}

void utils::FillPolygon( const std::vector<Vector2f>& vertices )
{
	FillPolygon( vertices.data( ), vertices.size( ) );
}

void utils::FillPolygon( const Vector2f *pVertices, size_t nrVertices )
{
	glBegin( GL_POLYGON );
	{
		for ( size_t idx{ 0 }; idx < nrVertices; ++idx )
		{
			glVertex2f( pVertices[idx].x, pVertices[idx].y );
		}
	}
	glEnd( );
}

void utils::FillPolygon3D(const Vector3f* pVertices, size_t nrVertices)
{
	glBegin(GL_POLYGON);
	{
		for (size_t idx{ 0 }; idx < nrVertices; ++idx)
		{
			glVertex3f(pVertices[idx].x, pVertices[idx].y, pVertices[idx].z);
		}
	}
	glEnd();
}

//GL_TRIANGLE_STRIP
void utils::FillPolygon3DStrip(const Vector3f* pVertices, size_t nrVertices)
{
	//GL_TRIANGLE_STRIP
	glBegin(GL_TRIANGLE_STRIP);
	{
		for (size_t idx{ 0 }; idx < nrVertices; ++idx)
		{
			glVertex3f(pVertices[idx].x, pVertices[idx].y, pVertices[idx].z);
		}
	}
	glEnd();
}

void utils::FillPolygon3DTris(const Vector3f* pVertices, size_t nrVertices)
{
	//GL_TRIANGLE_STRIP
	glBegin(GL_TRIANGLES);
	{
		for (size_t idx{ 0 }; idx < nrVertices; ++idx)
		{
			glVertex3f(pVertices[idx].x, pVertices[idx].y, pVertices[idx].z);
		}
	}
	glEnd();
}


#pragma endregion OpenGLDrawFunctionality

#pragma region CollisionFunctionality
float utils::GetDistance(float x1, float y1, float x2, float y2)
{
	return (sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

float utils::GetDistance(const Vector2f& p1, const Vector2f& p2)
{
	return GetDistance(p1.x, p1.y, p2.x, p2.y);
}

bool utils::IsPointInRect( const Vector2f& p, const Rectf& r )
{
	return ( p.x >= r.left && 
		p.x <= r.left + r.width &&
		p.y >= r.bottom &&
		p.y <= r.bottom + r.height );
}

bool utils::IsPointInCircle( const Vector2f& p, const Circlef& c )
{
	float squaredDist{ (p.x - c.center.x) * (p.x - c.center.x) + (p.y - c.center.y) * (p.y - c.center.y) };
	float squaredRadius{ c.radius * c.radius };
	return ( squaredRadius >= squaredDist );
}

bool utils::IsOverlapping( const Vector2f& a, const Vector2f& b, const Rectf& r )
{
	// if one of the line segment end points is in the rect
	if ( utils::IsPointInRect( a, r ) || utils::IsPointInRect( b, r ) )
	{
		return true;
	}

	HitInfo hitInfo{};
	Vector2f vertices[]{ Vector2f {r.left, r.bottom},
		Vector2f{ r.left + r.width, r.bottom },
		Vector2f{ r.left + r.width, r.bottom + r.height },
		Vector2f{ r.left, r.bottom + r.height } };

	return Raycast( vertices, 4, a, b, hitInfo );
}

bool utils::IsOverlapping( const Rectf& r1, const Rectf& r2 )
{
	// If one rectangle is on left side of the other
	if ( ( r1.left + r1.width ) < r2.left || ( r2.left + r2.width ) < r1.left )
	{
		return false;
	}

	// If one rectangle is under the other
	if ( r1.bottom > ( r2.bottom + r2.height ) || r2.bottom > ( r1.bottom + r1.height ) )
	{
		return false;
	}

	return true;
}

bool utils::IsOverlapping( const Rectf& r, const Circlef& c )
{
	// Is center of circle in the rectangle?
	if (IsPointInRect(c.center, r))
	{
		return true;
	}
	// Check line segments
	if (utils::DistPointLineSegment(c.center, Vector2f{ r.left, r.bottom }, Vector2f{ r.left, r.bottom + r.height }) <= c.radius)
	{
		return true;
	}
	if ( utils::DistPointLineSegment( c.center, Vector2f{ r.left, r.bottom }, Vector2f{ r.left + r.width, r.bottom } ) <= c.radius )
	{
		return true;
	}
	if (utils::DistPointLineSegment(c.center, Vector2f{ r.left + r.width, r.bottom + r.height }, Vector2f{ r.left, r.bottom + r.height }) <= c.radius)
	{
		return true;
	}
	if (utils::DistPointLineSegment(c.center, Vector2f{ r.left + r.width, r.bottom + r.height }, Vector2f{ r.left + r.width, r.bottom }) <= c.radius)
	{
		return true;
	}
	return false;
}

bool utils::IsOverlapping( const Circlef& c1, const Circlef& c2 )
{
	// squared distance between centers
	float xDistance{ c1.center.x - c2.center.x };
	float yDistance{ c1.center.y - c2.center.y };
	float squaredDistance{ xDistance * xDistance + yDistance * yDistance };

	float squaredTouchingDistance{ (c1.radius + c2.radius) * (c1.radius + c2.radius) };
	return (squaredDistance < squaredTouchingDistance);
}

bool utils::IsOverlapping( const Vector2f& a, const Vector2f& b, const Circlef& c )
{
	return utils::DistPointLineSegment( c.center, a, b ) <= c.radius;
}

bool utils::IsOverlapping( const std::vector<Vector2f>& vertices, const Circlef& c )
{
	return IsOverlapping( vertices.data( ), vertices.size( ), c );
}

bool utils::IsOverlapping( const Vector2f* vertices, size_t nrVertices, const Circlef& c )
{
	// Verify whether one of vertices is in circle
	for ( size_t i{ 0 }; i < nrVertices; ++i )
	{
		if ( IsPointInCircle( vertices[i], c ) )
		{
			return true;
		}
	}

	// Verify whether one of the polygon edges overlaps with circle
	for ( size_t i{ 0 }; i < nrVertices; ++i )
	{
		if ( DistPointLineSegment( c.center, vertices[i], vertices[( i + 1 ) % nrVertices] ) <= c.radius )
		{
			return true;
		}
	}

	// No overlapping with edges, verify whether circle is completely inside the polygon
	if ( IsPointInPolygon( c.center, vertices, nrVertices ) )
	{
		return true;
	}
	return false;
}

bool utils::IsPointInPolygon( const Vector2f& p, const std::vector<Vector2f>& vertices )
{
	return IsPointInPolygon( p, vertices.data( ), vertices.size( ) );
}

bool utils::IsPointInPolygon( const Vector2f& p, const Vector2f* vertices, size_t nrVertices )
{
	if ( nrVertices < 2 )
	{
		return false;
	}
	// 1. First do a simple test with axis aligned bounding box around the polygon
	float xMin{ vertices[0].x };
	float xMax{ vertices[0].x };
	float yMin{ vertices[0].y };
	float yMax{ vertices[0].y };
	for ( size_t idx{ 1 }; idx < nrVertices; ++idx )
	{
		if (xMin > vertices[idx].x)
		{
			xMin = vertices[idx].x;
		}
		if (xMax < vertices[idx].x)
		{
			xMax = vertices[idx].x;
		}
		if (yMin > vertices[idx].y)
		{
			yMin = vertices[idx].y;
		}
		if (yMax < vertices[idx].y)
		{
			yMax = vertices[idx].y;
		}
	}
	if (p.x < xMin || p.x > xMax || p.y < yMin || p.y > yMax)
	{
		return false;
	}

	// 2. Draw a virtual ray from anywhere outside the polygon to the point 
	//    and count how often it hits any side of the polygon. 
	//    If the number of hits is even, it's outside of the polygon, if it's odd, it's inside.
	int numberOfIntersectionPoints{0};
	Vector2f p2{ xMax + 10.0f, p.y }; // Horizontal line from point to point outside polygon (p2)

	// Count the number of intersection points
	float lambda1{}, lambda2{};
	for ( size_t i{ 0 }; i < nrVertices; ++i )
	{
		if ( IntersectLineSegments( vertices[i], vertices[( i + 1 ) % nrVertices], p, p2, lambda1, lambda2 ) )
		{
			if ( lambda1 > 0 && lambda1 <= 1 && lambda2 > 0 && lambda2 <= 1 )
			{
				++numberOfIntersectionPoints;
			}
		}
	}
	if (numberOfIntersectionPoints % 2 == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool utils::IntersectLineSegments( const Vector2f& p1, const Vector2f& p2, const Vector2f& q1, const Vector2f& q2, float& outLambda1, float& outLambda2, float epsilon )
{
	bool intersecting{ false };

	Vector2f p1p2{ p1, p2 };
	Vector2f q1q2{ q1, q2 };

	// Cross product to determine if parallel
	float denom = p1p2.CrossProduct( q1q2 );

	// Don't divide by zero
	if ( std::abs( denom ) > epsilon )
	{
		intersecting = true;

		Vector2f p1q1{ p1, q1 };

		float num1 = p1q1.CrossProduct( q1q2 );
		float num2 = p1q1.CrossProduct( p1p2 );
		outLambda1 = num1 / denom;
		outLambda2 = num2 / denom;
	}
	else // are parallel
	{
		// Connect start points
		Vector2f p1q1{ p1, q1 };

		// Cross product to determine if segments and the line connecting their start points are parallel, 
		// if so, than they are on a line
		// if not, then there is no intersection
		if (std::abs( p1q1.CrossProduct(q1q2) ) > epsilon)
		{
			return false;
		}

		// Check the 4 conditions
		outLambda1 = 0;
		outLambda2 = 0;
		if (utils::IsPointOnLineSegment(p1, q1, q2) ||
			utils::IsPointOnLineSegment(p2, q1, q2) ||
			utils::IsPointOnLineSegment(q1, p1, p2) ||
			utils::IsPointOnLineSegment(q2, p1, p2))
		{
			intersecting = true;
		}
	}
	return intersecting;
}

bool utils::Raycast( const std::vector<Vector2f>& vertices, const Vector2f& rayP1, const Vector2f& rayP2, HitInfo& hitInfo )
{
	return Raycast( vertices.data( ), vertices.size( ), rayP1, rayP2, hitInfo );
}

bool utils::Raycast( const Vector2f* vertices, const size_t nrVertices, const Vector2f& rayP1, const Vector2f& rayP2, HitInfo& hitInfo )
{
	if ( nrVertices == 0 )
	{
		return false;
	}

	std::vector<HitInfo> hits;

	Rectf r1, r2;
	// r1: minimal AABB rect enclosing the ray
	r1.left = std::min( rayP1.x, rayP2.x );
	r1.bottom = std::min( rayP1.y, rayP2.y );
	r1.width = std::max( rayP1.x, rayP2.x ) - r1.left;
	r1.height = std::max( rayP1.y, rayP2.y ) - r1.bottom;

	// Line-line intersections.
	for ( size_t idx{ 0 }; idx <= nrVertices; ++idx )
	{
		// Consider line segment between 2 consecutive vertices
		// (modulo to allow closed polygon, last - first vertice)
		Vector2f q1 = vertices[( idx + 0 ) % nrVertices];
		Vector2f q2 = vertices[( idx + 1 ) % nrVertices];

		// r2: minimal AABB rect enclosing the 2 vertices
		r2.left = std::min( q1.x, q2.x );
		r2.bottom = std::min( q1.y, q2.y );
		r2.width = std::max( q1.x, q2.x ) - r2.left;
		r2.height = std::max( q1.y, q2.y ) - r2.bottom;

		if ( IsOverlapping( r1, r2 ) )
		{
			float lambda1{};
			float lambda2{};
			if ( IntersectLineSegments( rayP1, rayP2, q1, q2, lambda1, lambda2 ) )
			{
				if ( lambda1 > 0 && lambda1 <= 1 && lambda2 > 0 && lambda2 <= 1 )
				{
					HitInfo linesHitInfo{};
					linesHitInfo.lambda = lambda1;
					linesHitInfo.intersectPoint = Vector2f{ rayP1.x + ( ( rayP2.x - rayP1.x ) * lambda1 ), rayP1.y + ( ( rayP2.y - rayP1.y ) * lambda1 ) };
					linesHitInfo.normal = Vector2f{ q2 - q1 }.Orthogonal( ).Normalized( );
					hits.push_back(linesHitInfo);
				}
			}
		}
	}

	if ( hits.size( ) == 0 )
	{
		return false;
	}

	// Get closest intersection point and copy it into the hitInfo parameter
	hitInfo = *std::min_element
	( 
		hits.begin( ), hits.end( ),
		[]( const HitInfo& first, const HitInfo& last ) 
		{
			return first.lambda < last.lambda; 
		} 
	);
	return true;
}

bool  utils::IsPointOnLineSegment( const Vector2f& p, const Vector2f& a, const Vector2f& b )
{
	Vector2f ap{ a, p }, bp{ b, p };
	// If not on same line, return false
	if ( abs( ap.CrossProduct( bp ) ) > 0.001f )
	{
		return false;
	}

	// Both vectors must point in opposite directions if p is between a and b
	if ( ap.DotProduct( bp ) > 0 )
	{
		return false;
	}

	return true;
}

float  utils::DistPointLineSegment( const Vector2f& p, const Vector2f& a, const Vector2f& b )
{
	Vector2f ab{ a, b };
	Vector2f ap{ a, p };
	Vector2f abNorm{ ab.Normalized() };
	float distToA{ abNorm.DotProduct(ap) };

	// If distToA is negative, then the closest point is A
	// return the distance a, p
	if ( distToA < 0 )
	{
		return ap.Length( );
	}
	// If distToA is > than dist(a,b) then the closest point is B
	// return the distance b, p
	float distAB{ ab.Length() };
	if ( distToA > distAB )
	{
		return Vector2f{ b, p }.Length( );
	}

	// Closest point is between A and B, calc intersection point
	Vector2f intersection{ abNorm.DotProduct(ap) * abNorm + Vector2f{ a } };
	return Vector2f{ p - intersection }.Length( );
}

bool utils::IntersectRectLine(const Rectf& r, const Vector2f& p1, const Vector2f& p2, float& intersectMin, float& intersectMax)
{
	// Parameters
	// input: 
	//   r: axis aligned bounding box, start and end points of line segment.
	//   p1, p2: line
	// output: 
	//   intersectMin and intersectMax: in the interval [0,1] if intersection point is on the line segment. 
	// return
	//   true if there is an intersection

	// Example of how to use
	//float min{};
	//float max{};
	//if (utils::IntersectRectLine(rect, p1, p2, min, max))
	//{
	//	Vector2f intersectP1{ p1 + (Vector2f(p2) - Vector2f(p1)) * min };
	//	Vector2f intersectP2{ p1 + (Vector2f(p2) - Vector2f(p1)) * max };
	//}

	// 4 floats to convert rect space to line space
	// x1: value between 0 and 1 where 0 is on p1 and 1 is on p2, <0 and >1 means intersection is not on line segment
	float x1{ (r.left - p1.x) / (p2.x - p1.x) };
	float x2{ (r.left + r.width - p1.x) / (p2.x - p1.x) };
	float y1{ (r.bottom - p1.y) / (p2.y - p1.y) };
	float y2{ (r.bottom + r.height - p1.y) / (p2.y - p1.y) };

	using std::max; using std::min;
	float tMin{ max(min(x1,x2),min(y1,y2)) };
	float tMax{ min(max(x1,x2), max(y1,y2)) };
	if (tMin > tMax)
	{
		return false;
	}
	intersectMin = tMin;
	intersectMax = tMax;
	return true;
}

#pragma endregion CollisionFunctionality

#pragma region FileHandelingFunctionality
//int utils::ReadLMP(const std::string& filename, int16_t**& outArray, int elementsPerRow) {
//	std::string fullPath = ("..\\" + filename);
//
//	
//
//	// Open the file in binary mode
//	std::ifstream file(filename, std::ios::binary);
//	if (!file) {
//		std::cerr << "Error: Could not open the file." << std::endl; // cerr is typicaly used for errors
//		return -1; // Return error code
//	}
//
//	// Vector to store the signed 16-bit integers
//	std::vector<int16_t> integers;
//
//	// Read the file 2 bytes at a time
//	int16_t value;
//	while (file.read(reinterpret_cast<char*>(&value), sizeof(int16_t))) {
//		integers.push_back(value);
//	}
//	file.close();
//
//	// Calculate the number of rows
//	int totalElements = int(integers.size());
//	if (totalElements % elementsPerRow != 0) {
//		std::cerr << "Error: File data size is not divisible by the expected row size." << std::endl;
//		return -1; // Return -1 as an error code
//	}
//	int rows = totalElements / elementsPerRow;
//
//	// Dynamically allocate the 2D array
//	outArray = new int16_t * [rows]; // Allocate rows
//	for (int i = 0; i < rows; ++i) {
//		outArray[i] = new int16_t[elementsPerRow]; // Allocate columns
//	}
//
//	// **& outarr means it's a reference to a pointer of a pointer
//
//	// Fill the 2D array with integers
//	int index = 0;
//	for (int i = 0; i < rows; ++i) {
//		for (int j = 0; j < elementsPerRow; ++j) {
//			outArray[i][j] = integers[index++];
//		}
//	}
//
//	// Return the number of rows
//	return rows;
//}

LMP utils::ReadLMP(const std::string& filename, int elementsPerRow) {
	std::string fullPath = ("..\\Resources/" + filename);

	int16_t** outArray{};

	// Open the file in binary mode
	std::ifstream file(filename, std::ios::binary);
	if (!file) {
		std::cerr << "Error: Could not open the file." << std::endl; // cerr is typicaly used for errors
		return LMP{ nullptr, -1, elementsPerRow };  // Return error code
		//return -1; // Return error code
	}

	// Vector to store the signed 16-bit integers
	std::vector<int16_t> integers;

	// Read the file 2 bytes at a time
	int16_t value;
	while (file.read(reinterpret_cast<char*>(&value), sizeof(int16_t))) {
		integers.push_back(value);
	}
	file.close();

	// Calculate the number of rows
	int totalElements = int(integers.size());
	if (totalElements % elementsPerRow != 0) {
		std::cerr << "Error: File data size is not divisible by the expected row size." << std::endl;
		return LMP{ nullptr, -1, elementsPerRow };  // Return error code
	}
	int rows = totalElements / elementsPerRow;

	// Dynamically allocate the 2D array
	outArray = new int16_t * [rows]; // Allocate rows
	for (int i = 0; i < rows; ++i) {
		outArray[i] = new int16_t[elementsPerRow]; // Allocate columns
	}

	// **& outarr means it's a reference to a pointer of a pointer

	// Fill the 2D array with integers
	int index = 0;
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < elementsPerRow; ++j) {
			outArray[i][j] = integers[index++];
		}
	}

	// Return the number of rows
	//return rows;
	return LMP{ outArray, rows, elementsPerRow };  // Return error code
}

void utils::ReadTexLMP(const std::string& filename, std::vector<PatchData>& patches)
{
	//https://doomwiki.org/wiki/TEXTURE1_and_TEXTURE2
	std::string fullPath = ("..\\Resources/" + filename);

	//int16_t** outArray{};

	// Open the file in binary mode
	std::ifstream file(filename, std::ios::binary);
	if (!file) {
		std::cerr << "Error: Could not open the file." << std::endl; // cerr is typicaly used for errors
		//return -1; // Return error code
	}

	// Vector to store the signed 16-bit integers
	//std::vector<int16_t> integers;
	std::vector<int8_t> integers;
	// Read the file 2 bytes at a time
	int8_t value;
	while (file.read(reinterpret_cast<char*>(&value), sizeof(int8_t))) {
		integers.push_back(value);
	}
	file.close();

	int8_t var1{ integers[0]};
	int8_t var2{ integers[1] };
	int8_t var3{ integers[2] };
	int8_t var4{ integers[3] };

	int32_t numTextures{ (int32_t(var4) << 24) + (int32_t(var3) << 16) + (int32_t(var2) << 8) + int32_t(var1)};

	int baseOffsetIndex{ 4 };
	int baseTextureDataIndex{ baseOffsetIndex + numTextures * 4 };
	std::vector<int32_t> offsets{};
	for (int offsetIndex{}; offsetIndex < numTextures; offsetIndex++)
	{
		int index = baseOffsetIndex + offsetIndex * 4;
		int8_t var1_ofset{ integers[index + 0] };
		int8_t var2_ofset{ integers[index + 1] };
		int8_t var3_ofset{ integers[index + 2] };
		int8_t var4_ofset{ integers[index + 3] };
		offsets.push_back((var1_ofset << 24) + (var2_ofset << 16) + (var3_ofset << 8) + var4_ofset); // << does bitshifting
	}
	ReadTexData(integers, baseTextureDataIndex, offsets, patches);
}

void utils::ReadTexData(std::vector<int8_t> data, int baseTextureDataIndex, std::vector<int32_t>& offsets, std::vector<PatchData>& patches)
{
	int16_t stringArr[4]{ };
	for (int i{}; i < 4; i++)
	{
		data[baseTextureDataIndex];
		int16_t number{ (data[baseTextureDataIndex + i * 2] << 8)
			+ data[baseTextureDataIndex + i * 2 + 1] };
		stringArr[i] = number;
	}
	std::string name{ convertToCharString(stringArr) };
	int maskedBaseIndex{ baseTextureDataIndex + 8 };
	int8_t boolVar1{ data[maskedBaseIndex + 0] };
	int8_t boolVar2{ data[maskedBaseIndex + 1] };
	int8_t boolVar3{ data[maskedBaseIndex + 2] };
	int8_t boolVar4{ data[maskedBaseIndex + 3] };

	int32_t masked{ (boolVar1 << 24) + (boolVar2 << 16) + (boolVar3 << 8) + boolVar4 };

	int widthBaseIndex{ maskedBaseIndex + 4};
	int8_t widthVar1{ data[widthBaseIndex + 0] };
	int8_t widthVar2{ data[widthBaseIndex + 1] };
	int16_t width{ (widthVar1 << 8) + widthVar2 };

	int heightBaseIndex{ widthBaseIndex + 2 };
	int8_t heightVar1{ data[heightBaseIndex + 0] };
	int8_t heightVar2{ data[heightBaseIndex + 1] };
	int16_t height{ (heightVar1 << 8) + heightVar2 };

	int ignoredBaseIndex{ heightBaseIndex + 2 };
	int patchCountBaseIndex{ ignoredBaseIndex + 4 };
	int8_t patchCVar1{ data[patchCountBaseIndex + 0] };
	int8_t patchCVar2{ data[patchCountBaseIndex + 1] };
	int16_t patchCount{ (patchCVar1 << 8) + patchCVar2 };
	int patchBaseIndex{ patchCountBaseIndex + 2 };
	//std::vector<PatchData> patches{};
	ReadPatchData(data, patchBaseIndex, patchCount, patches);
}

void utils::ReadPatchData(std::vector<int8_t> data, int basePatchDataIndex,
	int16_t patchCount, std::vector<PatchData>& patches)
{
	for (int patchIndex{}; patchIndex < patchCount; patchIndex++)
	{
		int curPatchIndex{ basePatchDataIndex + patchIndex * 10 };
		PatchData patch{};
		patch.xOffset = (data[curPatchIndex] << 8) + data[curPatchIndex + 1]; //(boolVar3 << 8) + boolVar4
		patch.yOffset = (data[curPatchIndex + 2] << 8) + data[curPatchIndex + 2 + 1];
		patch.patchNum = (data[curPatchIndex + 4] << 8) + data[curPatchIndex + 4 + 1];
		patches.push_back(patch);
	}
}

std::string utils::convertToCharString(const int16_t* stringStart)
{
	// Create a string large enough to hold the binary representation of four 16-bit integers
	std::string result(sizeof(uint16_t) * 4, '\0');

	const int16_t a{ stringStart[0] };
	const int16_t b{ stringStart[1] };
	const int16_t c{ stringStart[2] };
	const int16_t d{ stringStart[3] };
	// Pack the integers into the string
	result[0] = a & 0xFF;        // Lower 8 bits of A
	result[1] = (a >> 8) & 0xFF; // Upper 8 bits of A
	result[2] = b & 0xFF;        // Lower 8 bits of B
	result[3] = (b >> 8) & 0xFF; // Upper 8 bits of B
	result[4] = c & 0xFF;        // Lower 8 bits of C
	result[5] = (c >> 8) & 0xFF; // Upper 8 bits of C
	result[6] = d & 0xFF;        // Lower 8 bits of D
	result[7] = (d >> 8) & 0xFF; // Upper 8 bits of D

	for (int i{7}; i >=0; i--)
	{
		if (result[i] == 0)
		{
			result.erase(i, 1);
		}
	}



	return result;
}

short utils::InvertInt(uint16_t field) {
	// Bit 15 determines if it's a subnode (0) or a subsector (1)
	bool isNegative = ((field & 0x8000) != 0); // Check if bit 15 is set
	// reads field as binary, as well as 0x8000 and compares where both bits are true.
	// since in 0x8000 only bit 15 is true, the result will either be 0x8000 or 0x0000.

	// Bits 0-14 store the node or subsector number
	short number = field & 0x7FFF; // Mask out bit 15
	// same as the previous, only all bits EXEPT for the final bit are true, so it will coppy field in it's entirety exept for the final bit.

	if (isNegative) {
		// return positive number
		return number;
	}
	else {
		//return negative number
		number = number | 0x8000;
		return number;
		//with 0x8000 only having 1 in the sign bit, and since we're comparing with OR 
		// this means that regardless of if number has the sign bit on or not,
		// the result will be 1 for the signbit, always turning the number negative
	}
	
}

#pragma endregion FileHandelingFunctionality

int utils::getArrIndex(int arrLen, int index) {
	int modIndex = index % arrLen;
	return (modIndex >= 0) ? modIndex : modIndex + arrLen;
}
int utils::GetBSP(Vector2f& pos, const bool returnLeaf, const std::vector<BSPnode>& nodes)
{
	return utils::GetBSP(pos, nodes.size() - 1, returnLeaf, nodes);
}
int utils::GetBSP(Vector2f& pos, const int node, const bool returnLeaf, const std::vector<BSPnode>& nodes)
{
	//int node{ 3 };
	// we want to run over the BSP with the camera and check where it is;
	//Vector2f pos = Vector2f{ player.m_Pos.x, player.m_Pos.y };
	int orient = pos.orientation(nodes[node].partitionLine.startPoint, nodes[node].partitionLine.endPoint);
	int child{ 0 };
	if (orient > 0)
	{
		// go to left child
		child = nodes[node].children[1];
	}
	else if (orient < 0)
	{
		// go to right child
		child = nodes[node].children[0];
	}

	if (child < 0)
	{
		if (returnLeaf)
		{
			const int leafnode{ utils::InvertInt(child) };
			return leafnode;
		}
		else
		{
			return node;
		}
	}
	else
	{
		//go to the next node
		return utils::GetBSP(pos, child, returnLeaf, nodes);
	}
}
