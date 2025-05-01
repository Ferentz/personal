#include "pch.h"
#include "Level.h"
#include <iostream>
#include "Plane.h"
#include "PlaneIregular.h"
#include "Things.h"
#include "Enemy.h"
#include <vector>
#include <cassert>
#include "DoomCamera.h"

#include <stack>
#include "LevelGenerator.h"
#include "Navigator.h"



Level::Level()
	:m_pWallTexture{}
{
	m_pWallTexture = new Texture("WALL03_1_converted.png");
	m_pFloorTexture = new Texture("FLOOR4_8_converted.png");
	m_pThingTexture = new Texture("COLUA0_converted.png");

	
		


	LevelGenerator gen{ m_BSP, m_BSPleafs,m_pPlanes, m_pFloorsNCeilings, m_tags[0], m_tags[1], r_Things, m_Textures, m_NameMap };// m_pPlanes, m_pFloorsNCeilings};
	/*for (int thingIndex{}; thingIndex < r_Things.size(); thingIndex++)
	{
		Vector2f positionThing{ r_Things[thingIndex]->m_StartPos };
		int leafIndex{ getBSP(positionThing, true) };
		m_BSPleafs[leafIndex].thingIndexes.push_back(thingIndex);
		float zPos = m_BSPleafs[leafIndex].floorheight;
		r_Things[thingIndex]->m_location.point3D.z = zPos;
	}*/
	std::cout << "made";
}

Level::~Level() {
	// some instructions
	for (int i{}; i < m_pPlanes.size(); i++)
	{
		delete m_pPlanes[i];
	}
	delete m_pWallTexture;
	delete m_pFloorTexture;
}


void Level::traverseBSP(int leaf, Vector2f pos)
{
	/*
	we want to receive a childnode
	find the parent
	dive into it's other child untill we find a new child node.
	then go back and find the parent

	parent = startCHild.parent
	for i < 2
		newChild = bsp[parent].child[i]
		if newChild !=startChild
			diveTilChild()
	------------------------------------------
	*/

	// set for constant look up time
	std::set<int> visitedNodes{};

	//current ? leafIndex
	int startLeaf{ leaf };
	if (leaf >= 0)
	{
		startLeaf = utils::InvertInt(leaf); // make sure the leaf is read as negative for correct handeling
	}
	std::stack<int> nodeStack;
	nodeStack.push(startLeaf);

	//while stack is not empty:
	while (nodeStack.size() > 0)
	{
		int current = nodeStack.top();
		nodeStack.pop();
		if (visitedNodes.find(current) != visitedNodes.end())
		{
			continue;
		}
		//std::cout << current << std::endl;
		if (current < 0)
		{

			//visitLeaf(item.index)
			//!!!!!!!! do what i need to do.

			visitedNodes.insert(current);
			int leafIndex{ utils::InvertInt(current) };
			for (int thingNum{}; thingNum < m_BSPleafs[leafIndex].thingIndexes.size(); thingNum++)
			{
				int thingIndex{ m_BSPleafs[leafIndex].thingIndexes[thingNum] };
				r_Things[thingIndex]->DrawThing(m_pThingTexture);
			}
			int floorTexIdx{ m_pFloorsNCeilings[leafIndex * 2].GetTexIdx()};
			int ceilTexIdx{ m_pFloorsNCeilings[leafIndex * 2 + 1].GetTexIdx() };
			m_pFloorsNCeilings[leafIndex * 2].DrawPlane(m_Textures[floorTexIdx]);
			m_pFloorsNCeilings[leafIndex * 2 + 1].DrawPlane(m_Textures[ceilTexIdx]);
			for (int wallNum{}; wallNum < m_BSPleafs[leafIndex].wallIndexes.size(); wallNum++)
			{
				int wallIndex{ m_BSPleafs[leafIndex].wallIndexes[wallNum] };
				int texIDX = m_pPlanes[wallIndex]->GetTexIdx();
				
				m_pPlanes[wallIndex]->DrawPlane(m_Textures[texIDX]);
			}

			

			int parent{ m_BSPleafs[leafIndex].parent };

			if (parent != current) {
				nodeStack.push(parent);
			}
		}
		else
		{
			// we have a node
			visitedNodes.insert(current);
			int parent{ m_BSP[current].parent };
			if (parent != current) {
				nodeStack.push(parent);
			}
			BSPnode node{ m_BSP[current] };
			// perhaps we can check on what side the person is?
			Vector2f end{ node.partitionLine.endPoint };
			Vector2f start{ node.partitionLine.startPoint };
			int orientation{ pos.orientation(start, end) };
			if (orientation < 0)
			{
				nodeStack.push(node.children[1]);
				nodeStack.push(node.children[0]);
			}
			else
			{
				nodeStack.push(node.children[0]);
				nodeStack.push(node.children[1]);
			}


		}
	}
}

//void Level::traverseBSP(int leaf)
//{
//	/*
//	we want to receive a childnode
//	find the parent
//	dive into it's other child untill we find a new child node.
//	then go back and find the parent
//
//	parent = startCHild.parent
//	for i < 2
//		newChild = bsp[parent].child[i]
//		if newChild !=startChild
//			diveTilChild()
//	------------------------------------------
//	*/
//
//	// set for constant look up time
//	std::set<int> visitedNodes{};
//
//	//current ? leafIndex
//	int startLeaf{ leaf };
//	if (leaf >= 0)
//	{
//		startLeaf = utils::InvertInt(leaf); // make sure the leaf is read as negative for correct handeling
//	}
//	std::stack<int> nodeStack;
//	nodeStack.push(startLeaf);
//
//	//while stack is not empty:
//	while (nodeStack.size() > 0)
//	{
//		int current = nodeStack.top();
//		nodeStack.pop();
//		if (visitedNodes.find(current) != visitedNodes.end())
//		{
//			continue;
//		}
//		if (current < 0)
//		{
//			//visitLeaf(item.index)
//			//!!!!!!!! do what i need to do.
//
//			visitedNodes.insert(current);
//			int leafIndex{ utils::InvertInt(current) };
//
//			for (int wallNum{}; wallNum < m_BSPleafs[leafIndex].wallIndexes.size(); wallNum++)
//			{
//				int wallIndex{ m_BSPleafs[leafIndex].wallIndexes[wallNum] };
//				m_pPlanes[wallIndex].DrawPlane(*m_pWallTexture);
//			}
//			m_pFloorsNCeilings[leafIndex].DrawPlane(*m_pFloorTexture);
//			m_pFloorsNCeilings[leafIndex + 1].DrawPlane(*m_pFloorTexture);
//			int parent{ m_BSPleafs[leafIndex].parent };
//
//			if (parent != current) {
//				nodeStack.push(parent);
//			}
//		}
//		else
//		{
//			// we have a node
//			visitedNodes.insert(current);
//			BSPnode node{ m_BSP[current] };
//			nodeStack.push(node.children[0]);
//			nodeStack.push(node.children[1]);
//			int parent{ m_BSP[current].parent };
//			if (parent != current) {
//				nodeStack.push(parent);
//			}
//		}
//	}
//}

//void printNumber(int n) {
//	std::cout << "Number: " << n << "\n";
//}
//
//void callFunction(void (*func)(int), int val) {
//	func(val);
//}
//
//int main() {
//	callFunction(greet(5));  // Pass greet as a function pointer
//	return 0;
//}

//void printNumber(int n) {
//	std::cout << "Number: " << n << "\n";
//}
//
//void callFunction(void (*func)(int), int val) {
//	func(val);
//}


void Level::DrawLevel()
{
	for (int i = 0; i < m_PlaneAmount; ++i) {
		m_pPlanes[i]->DrawPlane(*m_pWallTexture);
		//planes[i].drawPlane(g_Sectors[i]);
	}
	//m_pPlanes[55].DrawPlane(*m_pWallTexture);
	//m_subSectorToDraw
	for (int i = 0; i < m_floorsNCeilingsAmount; ++i) {
		//for (int i = m_subSectorToDraw *2; i < m_subSectorToDraw * 2 + 2; ++i) {
			//planes[i].PlaneCollision(Eye);
		m_pFloorsNCeilings[i].DrawPlane(*m_pFloorTexture);
	}
}

void Level::UpdateLevel(DoomCamera& eyeObject, float elapsedSec)
{
	m_interval += elapsedSec;
	for (int i = 0; i < m_pPlanes.size(); ++i) {
		//planes[i].PlaneCollision(Eye);
		m_pPlanes[i]->UpdatePlane(eyeObject, elapsedSec);
		//m_pPlanes[i].PlaneCollision(eyeObject);
	}

	// we wanna implememnt the raycast
	// the ray is from pos to next pos. and we cast to the relevant level geometry


	for (int i = 0; i < m_pFloorsNCeilings.size(); ++i) {
		//for (int i = m_subSectorToDraw * 2; i < m_subSectorToDraw * 2 + 2; ++i) {
			//planes[i].PlaneCollision(Eye);
		m_pFloorsNCeilings[i].UpdatePlane(eyeObject);
	}

	for (int i{ 0 }; i < r_Things.size(); i++)
	{
		r_Things[i]->UpdateThing(eyeObject, elapsedSec);
		if (r_Things[i]->m_pNavigator != nullptr)
		{
			Vector3f nextPos{ r_Things[i]->m_pNavigator->GetNextPos(eyeObject.m_Pos, m_BSP, elapsedSec) };
			ColideWLevel(r_Things[i]->m_pNavigator->m_Pos, nextPos, r_Things[i]->m_pNavigator->m_Pos.z, 20);
			r_Things[i]->m_pNavigator->PushPosition(nextPos);
		}
	}

	//CollisionCHeck(eyeObject, m_BSP.size() - 1);
	//if (m_interval >= 5) {
	//	for (int i = m_subSectorToDraw * 2; i < m_subSectorToDraw * 2 + 2; ++i) {
	//		//planes[i].PlaneCollision(Eye);
	//		m_pFloorsNCeilings[i].getPoints();
	//	}
	//	m_interval = 0;
	//}
}

float Level::GetFloorOfLeaf(const int leaf) const
{
	return m_BSPleafs[leaf].floorheight;
}

void Level::CollisionCHeck(DoomCamera& player, const int node)
{
	//int node{ 3 };
	// we want to run over the BSP with the camera and check where it is;
	Vector2f pos = Vector2f{ player.m_Pos.x, player.m_Pos.y };
	int orient = pos.orientation(m_BSP[node].partitionLine.startPoint, m_BSP[node].partitionLine.endPoint);
	int child{ 0 };
	if (orient > 0)
	{
		// go to left child
		child = m_BSP[node].children[1];
	}
	else if (orient < 0)
	{
		// go to right child
		child = m_BSP[node].children[0];
	}

	if (child < 0)
	{
		const int leafnode{ utils::InvertInt(child) };
		const int wallsToCHeck{ int(m_BSPleafs[leafnode].wallIndexes.size()) };
		for (int wall{}; wall < wallsToCHeck; wall++)
		{
			int wallIndex{ m_BSPleafs[leafnode].wallIndexes[wall] };
			m_pPlanes[wallIndex]->PlaneCollision(player);
		}
		// we have reached a leaf node
		// now we must 
	}
	else
	{
		//go to the next node
		CollisionCHeck(player, child);
	}
}

int Level::ColideWLevel(const Vector3f& pos, Vector3f& r_nextPos, float& floorHeight, const float charDiameter) const
{
	const int MaxIterations = 10;
	const float Epsilon = 0.01f;

	Vector3f curPos = pos;
	Vector3f nextPos = r_nextPos;

	int iterations = 0;
	bool hitSomething = true;
	int hitWal{ -1 };
	float NextHeight{};
	while (hitSomething && iterations < MaxIterations)
	{
		hitSomething = false;

		Vector2f cur2D{ curPos.x, curPos.y };
		int leaf = utils::GetBSP(cur2D, true, m_BSP);
		float floorHeight = GetFloorOfLeaf(leaf);
		NextHeight = floorHeight;
		//nextPos.z = floorHeight;

		Vector3f moveVec = nextPos - curPos;
		if (moveVec.Length() < Epsilon)
			break;

		Vector3f moveDirNorm = moveVec.Normalized();

		for (int i = 0; i < GetLeafWallAmount(leaf); ++i)
		{
			Plane* wall{ GetWallFromLeaf(leaf, i) };

			Vector2f impactPoint2D, wallNormal2D;
			if (wall->PlaneCollision(curPos, nextPos + moveDirNorm * charDiameter, impactPoint2D, wallNormal2D))
			{
				if (wall->IsDoor())
				{
					TriggerDoor(leaf, i);
				}
				if (hitWal == -1)
				{
					hitWal = i;
				}

				Vector3f wallNormal3D(wallNormal2D.x, wallNormal2D.y, 0.0f);

				Vector3f fullMove = nextPos - curPos;
				Vector3f slideVec = fullMove - wallNormal3D * (fullMove.DotProduct(wallNormal3D));

				if (slideVec.Length() < Epsilon)
				{
					nextPos = curPos;
				}
				else
				{
					nextPos = curPos + slideVec;
				}

				// Update next floor height after wall slide
				Vector2f next2D{ nextPos.x, nextPos.y };
				int newLeaf = utils::GetBSP(next2D, true, m_BSP);
				NextHeight = GetFloorOfLeaf(newLeaf);

				hitSomething = true;
				//break;
			}
		}

		++iterations;
	}

	if (iterations >= MaxIterations)
	{
		r_nextPos = pos;
	}
	else {
		r_nextPos.x = nextPos.x;
		r_nextPos.y = nextPos.y;
		floorHeight = NextHeight;
	}
	// Final corrected position after all slides


	return hitWal;

}

bool Level::crossesBSPborder(Vector2f& pos, Vector2f& nextPos, const int node) const
{
	int orient = pos.orientation(m_BSP[node].partitionLine.startPoint, m_BSP[node].partitionLine.endPoint);
	int orientNext = nextPos.orientation(m_BSP[node].partitionLine.startPoint, m_BSP[node].partitionLine.endPoint);
	if (orient == orientNext)
	{
		return false;
	}
	return true;
}

int Level::GetLeafWallAmount(const int leaf) const
{
	return m_BSPleafs[leaf].wallIndexes.size();
}

Plane* Level::GetWallFromLeaf(const int leaf, const int wallIdx) const
{
	const int wallIndex{ m_BSPleafs[leaf].wallIndexes[wallIdx] };
	return m_pPlanes.at(wallIndex);
}

void Level::TriggerDoor(const int leaf, const int wallIdx) const
{
	const int wallIndex{ m_BSPleafs[leaf].wallIndexes[wallIdx] };
	if (m_pPlanes.at(wallIndex + 1)->IsDoor())
	{
		m_pPlanes.at(wallIndex + 1)->Triggered();
	}
	if (m_pPlanes.at(wallIndex - 1)->IsDoor())
	{
		m_pPlanes.at(wallIndex - 1)->Triggered();
	}
}

bool Level::IsInSet(int object)
{
	if (m_tags[0].find(object) != m_tags[0].end())
	{
		std::set<int>::reverse_iterator object;
		for (object = m_tags[0].rbegin();
			object != m_tags[0].rend(); object++)
		{
			if(*object < 0)
			{
				// it's a subsector
			}
			else
			{
				//it's a line
				//m_pPlanes[*object]->triggered();
			}
		}
	}
	if (m_tags[1].find(object) != m_tags[1].end())
	{

	}
	return false;
}

const std::vector<BSPnode>& Level::GetBSPtree() const
{
	return m_BSP;
	// TODO: insert return statement here
}
