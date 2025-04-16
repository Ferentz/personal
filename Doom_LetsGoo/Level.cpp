#include "pch.h"
#include "Level.h"
#include <iostream>
#include "Plane.h"
#include "PlaneIregular.h"
#include <vector>
#include <cassert>
#include "DoomCamera.h"
#include <set>
#include <stack>
#include "LevelGenerator.h"

Level::Level()
	:m_pWallTexture{}
{
	m_pWallTexture = new Texture("WALL03_1_converted.png");
	m_pFloorTexture = new Texture("FLOOR4_8_converted.png");
	LevelGenerator gen{ m_BSP, m_BSPleafs,m_pPlanes, m_pFloorsNCeilings };// m_pPlanes, m_pFloorsNCeilings};
}

Level::~Level() {
	// some instructions
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
			m_pFloorsNCeilings[leafIndex * 2].DrawPlane(*m_pFloorTexture);
			m_pFloorsNCeilings[leafIndex * 2 + 1].DrawPlane(*m_pFloorTexture);
			for (int wallNum{}; wallNum < m_BSPleafs[leafIndex].wallIndexes.size(); wallNum++)
			{
				int wallIndex{ m_BSPleafs[leafIndex].wallIndexes[wallNum] };
				m_pPlanes[wallIndex].DrawPlane(*m_pWallTexture);
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
		m_pPlanes[i].DrawPlane(*m_pWallTexture);
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
		m_pPlanes[i].UpdatePlane(eyeObject);
		//m_pPlanes[i].PlaneCollision(eyeObject);
	}

	// we wanna implememnt the raycast
	// the ray is from pos to next pos. and we cast to the relevant level geometry


	for (int i = 0; i < m_pFloorsNCeilings.size(); ++i) {
		//for (int i = m_subSectorToDraw * 2; i < m_subSectorToDraw * 2 + 2; ++i) {
			//planes[i].PlaneCollision(Eye);
		m_pFloorsNCeilings[i].UpdatePlane(eyeObject);
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
			m_pPlanes[wallIndex].PlaneCollision(player);
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

int Level::getBSP(Vector2f& pos, const bool returnLeaf) const
{
	return getBSP(pos, m_BSP.size() - 1, returnLeaf);
}

int Level::getBSP(Vector2f& pos, const int node, const bool returnLeaf) const
{
	//int node{ 3 };
	// we want to run over the BSP with the camera and check where it is;
	//Vector2f pos = Vector2f{ player.m_Pos.x, player.m_Pos.y };
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
		return getBSP(pos, child, returnLeaf);
	}
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

const Plane& Level::GetWallFromLeaf(const int leaf, const int wallIdx) const
{
	const int wallIndex{ m_BSPleafs[leaf].wallIndexes[wallIdx] };
	return m_pPlanes.at(wallIndex);
}
