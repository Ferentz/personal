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
	//std::cout << "new Plane!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
	//m_pPointsArr = new point2D_3D[m_amoountOfPoints];
	//for (int index{}; index < vertexes.size(); index++) {
	//	//std::cout << vertexes[index].x << " " << vertexes[index].y << " " << height << std::endl;
	//	m_pPointsArr[index] = {};
	//	m_pPointsArr[index].point3D.x = vertexes[index].x;
	//	m_pPointsArr[index].point3D.y = vertexes[index].y;
	//	m_pPointsArr[index].point3D.z = height;


	//}
	//m_pPointsArr = new point2D_3D[m_amoountOfPoints];
	for (int index{}; index < vertexes.size(); index++) {
		//std::cout << vertexes[index].x << " " << vertexes[index].y << " " << height << std::endl;
		m_pPointsArr.emplace_back(vertexes[index].x, vertexes[index].y, height);
		//m_pPointsArr[index] = {};
		/*m_pPointsArr.
		m_pPointsArr[index].point3D.x = vertexes[index].x;
		m_pPointsArr[index].point3D.y = vertexes[index].y;
		m_pPointsArr[index].point3D.z = height;*/
	}
}

PlaneIregular::PlaneIregular(Vector2f* vertexes, int vertexAmount, int colour, int height)
	:m_colour{ colour },
	m_amoountOfPoints{ vertexAmount }
{

	if (isConcave(vertexes, vertexAmount)) {
		
		MakeConcave(vertexes, vertexAmount); // reorder vertexes so it describes the hull;
	}

	for (int index{}; index < vertexAmount; index++) {
		//std::cout << vertexes[index].x << " " << vertexes[index].y << " " << height << std::endl;
		m_pPointsArr.emplace_back(vertexes[index].x, vertexes[index].y, height);
		//m_pPointsArr[index] = {};
		/*m_pPointsArr.
		m_pPointsArr[index].point3D.x = vertexes[index].x;
		m_pPointsArr[index].point3D.y = vertexes[index].y;
		m_pPointsArr[index].point3D.z = height;*/
	}

	// need to check if the verte pairs intersect with one another.
	//if they do, start from the first 2, and find a path that doesnt intersect any other line.

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


//bool PlaneIregular::PlaneCollision(Camera& eyeObject) {
//	PVector normalWidth{ width.normalize() };
//	PVector normalHeight{ height.normalize() };
//	PVector normal{ width.cross(height).normalize() };
//
//	PVector relativeEyeLocation_moved{ eyeObject.m_eyePos.x - location.x,
//		eyeObject.m_eyePos.y - location.y,
//		eyeObject.m_eyePos.z - location.z };
//
//
//
//	float newXpos = float(pow(relativeEyeLocation_moved.dot(normalWidth), 2)); // all power 2 to avoid using the square root later
//	float newYpos = float(pow(relativeEyeLocation_moved.dot(normalHeight), 2));
//	float newZpos = float(pow(relativeEyeLocation_moved.dot(normal), 2));
//
//	float planeWidth = width.dot(width);  // Squared length of width vector
//	float planeHeight = height.dot(height);  // Squared length of height vector
//
//	while (newXpos >= 0 - planeOfset && newXpos <= planeWidth + planeOfset &&
//		newYpos >= 0 - planeOfset && newYpos <= planeHeight + planeOfset &&
//		newZpos >= -planeOfset && newZpos <= planeOfset) //planeOfset is the power 2 of the actual planeoffset
//	{
//		float sideCheck = relativeEyeLocation_moved.dot(normal);
//		int mult{ -1 };
//		if (sideCheck > 0) {
//			mult = 1;
//		}
//		eyeObject.m_eyePos.x += normal.x * mult;
//		eyeObject.m_eyePos.y += normal.y * mult;
//		eyeObject.m_eyePos.z += normal.z * mult;
//		return true;
//	}
//
//	return false;
//}


bool isTriangulated(Vector2f* vertexes, int vertexIndexes[], int vertexAmount,
	int basepoint, int newVertexAmountA, int* triangulated, int newTriIndex, int recursionDepth)
{
	if (recursionDepth > vertexAmount * 2) {
		return false;
	}
	if (newVertexAmountA <= 3) {
		for (int i{}; i < newVertexAmountA; i++) {
			triangulated[newTriIndex + i] = vertexIndexes[i];
		}
		return true;
	}
	int offset{ 0 };
	bool clockise{ bool(recursionDepth % 2) };
	if (clockise) {
		offset = -2;
	}
	for (int i{}; i < 3; i++) {
		int midIndex{ utils::getArrIndex(newVertexAmountA, basepoint + offset + 1) };
		int indexA{ vertexIndexes[utils::getArrIndex(newVertexAmountA, basepoint + offset)] };
		int indexB{ vertexIndexes[midIndex] };
		int indexC{ vertexIndexes[utils::getArrIndex(newVertexAmountA, basepoint + offset + 2)] };
		Vector2f pointA{ vertexes[indexA] };
		Vector2f pointB{ vertexes[indexB] };
		Vector2f pointC{ vertexes[indexC] };

		Vector3f A{ pointA.x - pointB.x, pointA.y - pointB.y, 0 };
		Vector3f C{ pointC.x - pointB.x, pointC.y - pointB.y, 0 };

		bool intersect{ false };
		for (int check{ 0 }; check < vertexAmount; check++)
		{
			if (!(check == indexA || check == indexC || check + 1 == indexC || check + 1 == indexA)) {
				Vector2f intersection;
				bool foundIntersection{};
				foundIntersection = doIntersect(pointA, pointC, vertexes[check], vertexes[check + 1], intersection);
				//std::cout << "triangle intersects" << std::endl;
				if (foundIntersection) {
					intersect = true;
					break;
				}
			}
		}


		float cross{ A.CrossProduct(C).z };
		if (cross > 0 && !intersect) {
			// viable to make a triangle
			triangulated[newTriIndex] = indexA;
			triangulated[newTriIndex + 1] = indexB;
			triangulated[newTriIndex + 2] = indexC;
			int* newIndexes{ new int[newVertexAmountA - 1] };
			int indexCounter{ 0 };
			for (int j{}; j < newVertexAmountA; j++) {
				if (j != midIndex) {
					newIndexes[indexCounter] = vertexIndexes[j];
					indexCounter++;
				}
			}
			//std::cout << vertexAmount << ", " << newVertexAmountA - 1 << ", " << newTriIndex + 3 << std::endl;
			bool toReturn{};
			toReturn = isTriangulated(vertexes, newIndexes, vertexAmount, basepoint, newVertexAmountA - 1, triangulated, newTriIndex + 3, recursionDepth + 1);

			delete[] newIndexes;
			return toReturn;
		}
		else if (offset == 0 && clockise) {
			offset = -2;
		}
		else if (offset == 0 && !clockise) {
			offset = -1;
		}
		else if (offset == -2 && clockise) {
			offset = -1;
		}
		else if (offset == -2 && !clockise) {
			offset = 0;
		}
	}

	// so, if we get here, we need to take another sample point
	return isTriangulated(vertexes, vertexIndexes, vertexAmount, basepoint + 1, newVertexAmountA, triangulated, newTriIndex, recursionDepth + 1);
}



// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are collinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int orientation(Vector2f p, Vector2f q, Vector2f r)
{
	// See https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
	// for details of below formula. 
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // collinear 

	return (val > 0) ? 1 : 2; // clock or counterclock wise 
}

// The main function that returns true if line segment 'p1q1' 
// and 'p2q2' intersect. 
bool doIntersect(Vector2f p1, Vector2f q1, Vector2f p2, Vector2f q2,
	Vector2f& res) {

	// Find the four orientations needed for general and
	// special cases
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case: lines intersect if they have different
	// orientations
	if (o1 != o2 && o3 != o4) {

		// Compute intersection point
		double a1 = q1.y - p1.y;
		double b1 = p1.x - q1.x;
		double c1 = a1 * p1.x + b1 * p1.y;

		double a2 = q2.y - p2.y;
		double b2 = p2.x - q2.x;
		double c2 = a2 * p2.x + b2 * p2.y;

		double determinant = a1 * b2 - a2 * b1;

		if (determinant != 0) {
			res.x = (c1 * b2 - c2 * b1) / determinant;
			res.y = (a1 * c2 - a2 * c1) / determinant;
			return true;
		}
	}

	// Special Cases: check if the lines are collinear and
	// overlap
	if (o1 == 0 && p2.OnSegment(p1, q1))
	{
		res = p2;
		return true;
	}
	if (o2 == 0 && q2.OnSegment(p1, q1))
	{
		res = q2;
		return true;
	}
	if (o3 == 0 && p1.OnSegment(p2, q2))
	{
		res = p1;
		return true;
	}
	if (o4 == 0 && q1.OnSegment(p2, q2))
	{
		res = q1;
		return true;
	}

	// Lines do not intersect in any case
	return false;
}

bool isConcave(Vector2f arr[], int arrLen)
{
	// if 0 to n crosses k - k+1
	for (int line{ 2 }; line < arrLen - 1; line++)
	{
		for (int check{ 1 }; check < arrLen; check++)
		{
			if (!(check == line || check + 1 == line)) {
				Vector2f intersection;
				if (doIntersect(arr[0], arr[line], arr[check], arr[check + 1], intersection))
				{
					return true;
				}
			}
		}
	}
	return false;
}
//
//void PlaneIregular::MakeConcave(Vector2f* vertexes, int vertexAmount)
//{
//	// we prime the first 2 indexes
//	m_ConcaveSequence[0] = 0;
//	m_ConcaveSequence[1] = 1;
//
//	int indexA = 0;
//	int indexB = 1;
//
//	int fillIndex{ 2 };
//	int indexNew[2]{};
//	while(fillIndex < vertexAmount){
//		for (int check{ 0 }; check < vertexAmount; check++)
//		{
//			bool foundIntersection{};
//			// we check if the line segment intersects with any of the other sides.
//			if (!(check == indexA || check == indexB || check + 1 == indexB || check + 1 == indexA)) {
//				Vector2f intersection;
//				
//				Vector2f pointA{ vertexes[indexA] };
//				Vector2f pointB{ vertexes[indexB] };
//				foundIntersection = doIntersect(pointA, pointB, vertexes[check], vertexes[check + 1], intersection);
//				//std::cout << "triangle intersects" << std::endl;
//				if (foundIntersection) {
//					indexNew[0] = check;
//					indexNew[1] = check + 1;
//				}
//			}
//			bool intersectionCheck2[2]{};
//			for (int i{}; i < 2; i++) {
//				for (int check{ 0 }; check < vertexAmount; check++)
//				{
//					if (!(check == indexNew[i] || check == indexB || check + 1 == indexB || check + 1 == indexNew[i])) {
//						Vector2f intersection;
//						bool foundIntersection{};
//						Vector2f pointA{ vertexes[indexNew[i]] };
//						Vector2f pointB{ vertexes[indexB] };
//						intersectionCheck2[i] = doIntersect(pointB, pointA, vertexes[check], vertexes[check + 1], intersection);
//						if (intersectionCheck2[i]) {
//							break;
//						}
//						//std::cout << "triangle intersects" << std::endl;
//					}
//				}
//			}
//			if (intersectionCheck2[0] && intersectionCheck2[1])
//			{
//				continue;
//			}
//			else if (intersectionCheck2[0])
//			{
//				//then 1 is the net vertex in the sequence
//				m_ConcaveSequence[fillIndex] = indexNew[1];
//				indexA = indexB;
//				indexB = indexNew[1];
//				fillIndex++;
//				break;
//				
//			}
//			else
//			{
//				m_ConcaveSequence[fillIndex] = indexNew[0];
//				indexA = indexB;
//				indexB = indexNew[1];
//				fillIndex++;
//				break;
//			}
//		}
//	}
//}

void PlaneIregular::MakeConcave(Vector2f* vertexes, int vertexAmount) {
	if (vertexAmount < 3) return; // Not enough vertices to form a shape

	// Step 1: Find the leftmost (or bottommost) point
	int leftmost = 0;
	for (int i = 1; i < vertexAmount; i++) {
		if (vertexes[i].x < vertexes[leftmost].x ||
			(vertexes[i].x == vertexes[leftmost].x && vertexes[i].y < vertexes[leftmost].y)) {
			leftmost = i;
		}
	}

	// Step 2: Swap the leftmost point to the first position
	std::swap(vertexes[0], vertexes[leftmost]);
	Vector2f pivot = vertexes[0];

	// Step 3: Manual sorting (Selection Sort) using your CrossProduct function
	for (int i = 1; i < vertexAmount - 1; i++) {
		int bestIndex = i;
		for (int j = i + 1; j < vertexAmount; j++) {
			// Compute cross product using vector subtraction
			Vector2f vecA = vertexes[bestIndex] - pivot;
			Vector2f vecB = vertexes[j] - pivot;
			float cross = vecA.CrossProduct(vecB);

			// If vecB is more counterclockwise OR if collinear and closer, update bestIndex
			if (cross < 0 || (cross == 0 && vertexes[j].SquaredLength() < vertexes[bestIndex].SquaredLength())) {
				bestIndex = j;
			}
		}
		std::swap(vertexes[i], vertexes[bestIndex]);
	}
}