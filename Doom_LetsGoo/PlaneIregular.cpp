#include "pch.h"
#include "PlaneIregular.h"

#include <iostream>

#include "Texture.h"
#include "Player.h"

PlaneIregular::PlaneIregular()
	:width{},
	height{},
	m_texIDX{},
	m_amoountOfPoints{}
{

}

PlaneIregular::PlaneIregular(std::vector<Vector2f> vertexes, int idx, int height)
	:m_texIDX{ idx },
	m_amoountOfPoints{ int(vertexes.size()) }
{
	for (int index{}; index < vertexes.size(); index++) {
		m_pPointsArr.emplace_back(vertexes[index].x, vertexes[index].y, height);

	}
}

#pragma region basics
void PlaneIregular::UpdatePlane(const Player& player) {

	//so, when updating, we can get the offseto fo the point. and, we can project the texture along world space then.
	for (int index{ 0 }; index < m_amoountOfPoints; index++) {
		//std::cout << index << std::endl;
		m_pPointsArr[index].point2D = player.GetCamera().PerspectiveCalculation(m_pPointsArr[index]);
		//x = 0, y = 1, Used = 2
		if (m_pPointsArr[index].AngleXYUsed[2] > M_PI * 0.5f || m_pPointsArr[index].AngleXYUsed[2] < - M_PI * 0.5f) {
			m_pPointsArr[index].amIVisible = false;
		}
		else {
			m_pPointsArr[index].amIVisible = true;
		}
	}
}

void PlaneIregular::DrawPlane(const Texture& texture) const
{
	bool show{ false };
	for (int index{}; index < m_amoountOfPoints; index++) {
		if (m_pPointsArr[index].amIVisible) {
			show = true;
		}
	}
	if (show) {
		Vector3f* planesCompArr3D{ new Vector3f[m_amoountOfPoints] };
		Vector2f* planesLayoutArr{ new Vector2f[m_amoountOfPoints] };
		for (int index{}; index < m_amoountOfPoints; index++) {
			planesCompArr3D[index] = Vector3f{ m_pPointsArr[index].point2D.x, m_pPointsArr[index].point2D.y, -m_pPointsArr[index].Dist3D * 0.0001f };
			planesLayoutArr[index] = Vector2f{ m_pPointsArr[index].point3D.x, m_pPointsArr[index].point3D.y };
		}

		texture.DrawTexturePersp(planesCompArr3D, planesLayoutArr, m_amoountOfPoints, 10);
		delete[] planesCompArr3D;
		delete[] planesLayoutArr;

	}
}

#pragma endregion basics

#pragma region interactions
int PlaneIregular::GetTexIdx() const
{
	return m_texIDX;
}

void PlaneIregular::printPoints() const
{
	for (int index{ 0 }; index < m_amoountOfPoints; index++) {
		//std::cout << index << std::endl;
		std::cout << m_pPointsArr[index].point3D.x << " " << m_pPointsArr[index].point3D.y << " " << m_pPointsArr[index].point3D.z << " " << std::endl;
	}

}

#pragma endregion interactions

#pragma region editing
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

#pragma endregion editing