#include "pch.h"
#include "PlaneIregular.h"
#include <iostream>
#include <cmath>
#include <vector> // For vector operations if needed
#include <utils.h>
#include "Texture.h"
#include "DoomCamera.h"

const Color4f RED{ 1.0f, 0.0f, 0.0f, 1.0f };
const Color4f WHITE{ 1.0f, 1.0f, 1.0f, 1.0f };
const Color4f YELLOW{ 1.0f, 1.0f, 0.0f, 1.0f };
const Color4f BLUE{ 0.0f, 0.0f, 1.0f, 1.0f };
const Color4f GREEN{ 0.0f, 1.0f, .0f, 1.0f };
const Color4f TEAL{ 0.0f, 1.0f, 1.0f, 1.0f };

int cmp_int(const void* va, const void* vb)
{
	int a = *(int*)va, b = *(int*)vb;
	return a < b ? -1 : a > b ? +1 : 0;
}

PlaneIregular::PlaneIregular()
	:width{},
	height{},
	m_colour{},
	m_amoountOfPoints{}
{

}

PlaneIregular::PlaneIregular(std::vector<Vector2f> vertexes, int colour, int height)
	:m_colour{ colour },
	m_amoountOfPoints{ int(vertexes.size()) }
{
	for (int index{}; index < vertexes.size(); index++) {
		m_pPointsArr.emplace_back(vertexes[index].x, vertexes[index].y, height);

	}
}

void PlaneIregular::UpdatePlane(const DoomCamera& eyeObject) {

	//so, when updating, we can get the offseto fo the point. and, we can project the texture along world space then.

	for (int index{ 0 }; index < m_amoountOfPoints; index++) {
		//std::cout << index << std::endl;
		m_pPointsArr[index].point2D = eyeObject.PerspCalc(m_pPointsArr[index], m_pPointsArr[index].point3D);
		if (m_pPointsArr[index].Angle[2] > M_PI * 0.5f || m_pPointsArr[index].Angle[2] < - M_PI * 0.5f) {
			m_pPointsArr[index].amIVisible = false;
		}
		else {
			m_pPointsArr[index].amIVisible = true;
		}
	}

	/*for (int index{ 0 }; index < m_amoountOfPoints; index++)
	{
		int nextIndex{ utils::getArrIndex(index + 1, m_amoountOfPoints) };
		eyeObject.PerspCalc(m_pPointsArr[index], m_pPointsArr[nextIndex]);
	}*/
}

void PlaneIregular::printPoints() const
{
	for (int index{ 0 }; index < m_amoountOfPoints; index++) {
		//std::cout << index << std::endl;
		std::cout << m_pPointsArr[index].point3D.x << " " << m_pPointsArr[index].point3D.y << " " << m_pPointsArr[index].point3D.z << " " << std::endl;
	}

}

point2D_3D PlaneIregular::GetPoint(int index) const
{
	return m_pPointsArr[index];
}

int PlaneIregular::GetAmountPoints() const
{
	return m_amoountOfPoints;
}

void PlaneIregular::insertPoint(int insertPos, point2D_3D& point)
{
	m_pPointsArr.insert(m_pPointsArr.begin() + insertPos, point);
	m_amoountOfPoints+=1;
}

void PlaneIregular::PopPoint(int popIndex)
{
	m_pPointsArr.erase(m_pPointsArr.begin() + popIndex);
	m_amoountOfPoints-=1;
}

void PlaneIregular::DrawPlane(const Texture& texture) {
	bool show{ false };
	for (int index{}; index < m_amoountOfPoints; index++) {
		if (m_pPointsArr[index].amIVisible) {
			show = true;
		}
	}
	if (show) {
		switch (m_colour)
		{
		case 0:
			utils::SetColor(RED);
			break;
		case 1:
			utils::SetColor(WHITE);
			break;
		case 2:
			utils::SetColor(YELLOW);
			break;
		case 3:
			utils::SetColor(BLUE);
			break;
		case 4:
			utils::SetColor(GREEN);
			break;
		case 5:
			utils::SetColor(TEAL);
			break;
		default:
			utils::SetColor(RED);
			break;
		}
		Color4f mycol {( m_colour +100) / 255.f, (m_colour + 100) / 255.f, (m_colour + 100) / 255.f, 1.f };
		utils::SetColor(mycol);
		Vector3f* planesCompArr3D{ new Vector3f[m_amoountOfPoints] };
		Vector2f* planesLayoutArr{ new Vector2f[m_amoountOfPoints] };
		for (int index{}; index < m_amoountOfPoints; index++) {
			planesCompArr3D[index] = Vector3f{ m_pPointsArr[index].point2D.x, m_pPointsArr[index].point2D.y, -m_pPointsArr[index].Dist3D * 0.0001f };
			planesLayoutArr[index] = Vector2f{ m_pPointsArr[index].point3D.x, m_pPointsArr[index].point3D.y };
		}
		
		//utils::FillPolygon3D(planesCompArr3D, m_amoountOfPoints);
		texture.DrawTexturePersp(planesCompArr3D, planesLayoutArr, m_amoountOfPoints, 10);
		delete[] planesCompArr3D;
		delete[] planesLayoutArr;

	}
}
