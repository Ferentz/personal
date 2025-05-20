#pragma once
#pragma once
#include "structs.h"
//#include "Camera.h"
//#include "pVector.h"
#include <utils.h>
#include <vector>

class PLayer;
class Texture;

class PlaneIregular final
{
public:
	PlaneIregular();
	PlaneIregular(std::vector<Vector2f> vertexes, int texIndx, int height);
	void UpdatePlane(const PLayer& eyeObject);
	void DrawPlane(const Texture& texture) const;
	//void DrawPlane(const int colour);
	void printPoints() const;
	point2D_3D GetPoint(int index) const;
	int GetAmountPoints() const;
	void insertPoint(int insertPos, point2D_3D& point);
	void PopPoint(int popIndex);
	int GetTexIdx();
	
	//point3f getLocation();
	//int getClosestPointIndex();
	//float getPoint3dDist(int index);
	//bool PlaneCollision(Camera& eyeObject);
	float planeOfset{ 0.5 };
	int m_colour;

private:
	int m_texIDX;
	Vector3f width;
	Vector3f height;
	int* m_ConcaveSequence{};
	bool m_concave{};
	std::vector<point2D_3D> m_pPointsArr;
	int m_amoountOfPoints;
};


