#pragma once
#pragma once
#include "structs.h"
//#include "Camera.h"
//#include "pVector.h"
#include <utils.h>
#include <vector>

class DoomCamera;
class Texture;

class PlaneIregular
{
public:
	PlaneIregular();
	PlaneIregular(std::vector<Vector2f> vertexes, int colour, int height);
	PlaneIregular(Vector2f* vertexes, int vertexAmount, int colour, int height);
	void UpdatePlane(const DoomCamera& eyeObject);
	void DrawPlane(const Texture& texture);
	//void DrawPlane(const int colour);
	void printPoints() const;
	point2D_3D GetPoint(int index) const;
	int GetAmountPoints() const;
	void insertPoint(int insertPos, point2D_3D& point);
	void PopPoint(int popIndex);
	
	//point3f getLocation();
	//int getClosestPointIndex();
	//float getPoint3dDist(int index);
	//bool PlaneCollision(Camera& eyeObject);
	float planeOfset{ 0.5 };
	int m_colour;

private:

	Vector3f width;
	Vector3f height;
	int* m_ConcaveSequence{};
	bool m_concave{};
	std::vector<point2D_3D> m_pPointsArr;
	int m_amoountOfPoints;
	int m_triangleIndexAmount;
	void MakeConcave(Vector2f* vertexes, int vertexmount);

};

bool isConcave(Vector2f arr[], int arrLen);
bool doIntersect(Vector2f p1, Vector2f q1, Vector2f p2, Vector2f q2,
	Vector2f& res);
bool isTriangulated(Vector2f* vertexes, int vertexIndexes[], int vertexAmount, int basepoint,
	int newVertexAmountA, int* triangulated, int newTriIndex, int recursionDepth);



