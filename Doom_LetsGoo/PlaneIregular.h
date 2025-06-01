#pragma once
#include "structs.h"
#include <vector>

class Player;
class Texture;

class PlaneIregular final
{
public:
	PlaneIregular();
	explicit PlaneIregular(std::vector<Vector2f> vertexes, int texIndx, int height);
	
	//basics
	void UpdatePlane(const Player& eyeObject);
	void DrawPlane(const Texture& texture) const;

	//interactions
	int GetTexIdx() const;
	void printPoints() const;
	

	//editing
	point2D_3D GetPoint(int index) const; //not a const or ref as it might be inserted in another PlaneIregular
	int GetAmountPoints() const;
	void insertPoint(int insertPos, point2D_3D& point);
	void PopPoint(int popIndex);

private:
	int m_texIDX;
	Vector3f width;
	Vector3f height;
	std::vector<point2D_3D> m_pPointsArr;
	int m_amoountOfPoints;
};


