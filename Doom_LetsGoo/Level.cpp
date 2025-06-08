#include "pch.h"
#include "Level.h"

#include <iostream>
#include <cassert>
#include <stack>
#include <set>

#include "Player.h"
#include "Plane.h"
#include "PlaneIregular.h"
#include "Enemy.h"
#include "PowerUp.h"
#include "Texture.h"

#include "LevelGenerator.h"
#include "Navigator.h"
#include "Interactor.h"

#include "utils.h"



Level::Level(Player& player)
{
	m_pThingTexture = new Texture("COLUA0_converted.png");
	LevelGenerator gen{
		m_BSP, m_BSPleafs,m_pPlanes,
		m_pFloorsNCeilings, m_pThings,
		m_Textures, player
	};
}

Level::~Level() {
	// some instructions
	for (int i{}; i < m_pPlanes.size(); i++)
	{
		delete m_pPlanes[i];
	}
	for (int i{}; i < m_pThings.size(); i++)
	{
		delete m_pThings[i];
	}
	delete m_pThingTexture;
}

#pragma region rule of 5
Level::Level(Level&& toMove) noexcept
{
	MoveLevelMembers(std::move(toMove));
}

Level& Level::operator=(Level&& rhs) noexcept
{
	MoveLevelMembers(std::move(rhs));
	return *this;
}

void Level::MoveLevelMembers(Level&& toMove) noexcept
{
	m_isCleared = std::move(toMove.m_isCleared);
	m_pThingTexture = std::move(toMove.m_pThingTexture);
	toMove.m_pThingTexture = nullptr;
	m_Textures = std::move(toMove.m_Textures);
	m_BSP = std::move(toMove.m_BSP);
	m_BSPleafs = std::move(toMove.m_BSPleafs);
	m_pPlanes = std::move(toMove.m_pPlanes);
	m_pFloorsNCeilings = std::move(toMove.m_pFloorsNCeilings);
	m_pThings = std::move(toMove.m_pThings);
}

#pragma endregion rule of 5

#pragma region basics
void Level::DrawLevel(Vector2f pos)
{
	int leaf{ utils::GetBSP(pos, true, GetBSPtree()) };
	DrawLevelBSP(leaf, pos);
}

void Level::UpdateLevel(Player& eyeObject, float elapsedSec)
{
	for (size_t i = 0; i < m_pPlanes.size(); ++i) {
		m_pPlanes[i]->UpdatePlane(eyeObject, elapsedSec);
	}

	// we wanna implememnt the raycast
	// the ray is from pos to next pos. and we cast to the relevant level geometry


	for (size_t i = 0; i < m_pFloorsNCeilings.size(); ++i) {
		m_pFloorsNCeilings[i].UpdatePlane(eyeObject);
	}

	UpdateThings(eyeObject, elapsedSec);

}

#pragma region basics helpers

void Level::DrawLevelBSP(int leaf, Vector2f posInLevel)
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

	//current leafIndex
	int startLeaf{ leaf };
	if (startLeaf >= 0)
	{
		startLeaf = utils::InvertInt(startLeaf); // make sure the leaf is read as negative for correct handeling
	}
	std::stack<int> nodeStack;
	nodeStack.push(startLeaf);

	//while stack is not empty:
	while (nodeStack.size() > 0)
	{
		int current = nodeStack.top();
		nodeStack.pop();
		if (visitedNodes.find(current) != visitedNodes.end()) // if it's already been visited
		{
			continue;
		}

		visitedNodes.insert(current);

		if (current < 0) // if it's a leaf
		{
			int leafIndex{ utils::InvertInt(current) };

			DrawLeaf(leafIndex);
			
			int parent{ m_BSPleafs[leafIndex].parent };
			if (parent != current) {
				nodeStack.push(parent);
			}
		}
		else // if it's a node
		{
			int parent{ m_BSP[current].parent };
			if (parent != current) {
				nodeStack.push(parent);
			}
			BSPnode node{ m_BSP[current] };
			// perhaps we can check on what side the person is?
			Vector2f end{ node.partitionLine.endPoint };
			Vector2f start{ node.partitionLine.startPoint };
			int orientation{ posInLevel.orientation(start, end) };
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

void Level::DrawLeaf(int leafIndex)
{
	for (int thingNum{}; thingNum < m_BSPleafs[leafIndex].thingIndexes.size(); thingNum++)
	{
		int thingIndex{ m_BSPleafs[leafIndex].thingIndexes[thingNum] };
		/*if (thingIndex == 93 || thingIndex == 94) // to check specific ones
		{*/
		assert(m_pThingTexture != nullptr);
		m_pThings[thingIndex]->DrawThing(m_pThingTexture);
		//}
	}
	int floorTexIdx{ m_pFloorsNCeilings[leafIndex * 2].GetTexIdx() };
	int ceilTexIdx{ m_pFloorsNCeilings[leafIndex * 2 + 1].GetTexIdx() };
	m_pFloorsNCeilings[leafIndex * 2].DrawPlane(m_Textures[floorTexIdx]);
	m_pFloorsNCeilings[leafIndex * 2 + 1].DrawPlane(m_Textures[ceilTexIdx]);
	for (int wallNum{}; wallNum < m_BSPleafs[leafIndex].wallIndexes.size(); wallNum++)
	{
		int wallIndex{ m_BSPleafs[leafIndex].wallIndexes[wallNum] };
		int texIDX = m_pPlanes[wallIndex]->GetTexIdx();

		m_pPlanes[wallIndex]->DrawPlane(m_Textures[texIDX]);
	}
}


void Level::UpdateThings(Player& eyeObject, float elapsedSec)
{
	for (int ThingLevelIndex{ 0 }; ThingLevelIndex < m_pThings.size(); ThingLevelIndex++)
	{
		
		m_pThings[ThingLevelIndex]->UpdateThing(eyeObject, elapsedSec);

		if (m_pThings[ThingLevelIndex]->GetSubClass() == ThingSubclass::enemy)
		{
			Enemy* enemyPtr = static_cast<Enemy*>(m_pThings[ThingLevelIndex]);
			if (enemyPtr->Navigate(this, eyeObject, elapsedSec)) // if he is not shooting and alive
			{
				const Vector3f nextPos{ enemyPtr->GetpNavigator()->GetNextPos()};
				Vector2f next2D{ nextPos.x, nextPos.y };
				const int subSector2 = utils::GetBSP(next2D, true, m_BSP);
				const int originalThingSubSector{ enemyPtr->GetSubsectorIndex()};
				if (subSector2 != originalThingSubSector)
				{
					SwapThingSubsector(originalThingSubSector, subSector2, ThingLevelIndex);
				}
			}
		}
	}
}

void Level::SwapThingSubsector(const int originalThingSubSector, const int subSector2, const int ThingLevelIndex)
{
	for (int localIndex{}; localIndex < m_BSPleafs[originalThingSubSector].thingIndexes.size(); localIndex++)
	{
		if (m_BSPleafs[originalThingSubSector].thingIndexes[localIndex] == ThingLevelIndex)
		{
			int temp = m_BSPleafs[originalThingSubSector].thingIndexes[localIndex];
			m_BSPleafs[originalThingSubSector].thingIndexes[localIndex] =
				m_BSPleafs[originalThingSubSector].thingIndexes.back();
			m_BSPleafs[originalThingSubSector].thingIndexes.pop_back();
			m_BSPleafs[subSector2].thingIndexes.push_back(ThingLevelIndex);
			m_pThings[ThingLevelIndex]->SetSubsectorIndex( subSector2 );
			break;
		}
	}

}

const std::vector<BSPnode>& Level::GetBSPtree() const
{
	return m_BSP;
}

#pragma endregion basics helpers

#pragma endregion basics

#pragma region collision
/// <summary>
/// collides anything with a 3d position and a diameter with the level
/// the position will be adjusted for a max of 10 times
/// </summary>
/// <param name="pos"></param>
/// <param name="r_nextPos"> will be mutated for later use </param>
/// <param name="floorHeight"> will corespond to the new next position </param>
/// <param name="charDiameter"></param>
/// <param name="ignoreLedges"> to ignore ledges or not</param>
/// <param name="foundExit"> if the exit was found during this collision </param>
/// <returns></returns>
void Level::ColideWLevel(const Vector3f& pos, Vector3f& r_nextPos, float& floorHeight,
	const float charDiameter, const bool ignoreLedges, bool& foundExit) const
{
	const int MaxIterations = 10;
	const float Epsilon = 0.01f;

	Vector3f curPos = pos;
	Vector3f nextPos = r_nextPos;

	int iterations = 0;
	bool hitSomething = true;
	float NextFloorHeight{};
	Vector2f cur2Dpos{ curPos.x, curPos.y };
	int leaf{ utils::GetBSP(cur2Dpos, true, m_BSP) };
	int newLeaf{ leaf };
	while (hitSomething && iterations < MaxIterations)
	{
		hitSomething = false;

		cur2Dpos = Vector2f{curPos.x, curPos.y }; // we get the curent 2d pos
		leaf = utils::GetBSP(cur2Dpos, true, m_BSP); // we get the subsector in which that position is
		NextFloorHeight = GetFloorOfLeaf(leaf); // we get the floorehight of that leaf

		Vector3f moveVec = nextPos - curPos; // we get the direction in which the character is moving
		if (moveVec.Length() < Epsilon) // if the amount he's trying to move is too small, dont move
		{
			r_nextPos = pos;
			break;
		}
			
		Vector3f moveDirNorm = moveVec.Normalized(); // normalized direction
		
		for (int i = 0; i < GetLeafWallAmount(leaf); ++i)
		{
			Plane* wall{ GetWallFromLeaf(leaf, i) };
			
			Vector2f reflectionPoint, wallNormal2D;
			if (wall->PlaneCollision(curPos, nextPos + moveDirNorm * charDiameter, reflectionPoint, wallNormal2D, ignoreLedges))
			{
				if (wall->IsDoor() && ignoreLedges)
				{
					if (TriggerDoor(leaf, i) && ignoreLedges)
					{
						foundExit = true;
					}
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
				newLeaf = utils::GetBSP(next2D, true, m_BSP);
				NextFloorHeight = GetFloorOfLeaf(newLeaf);

				hitSomething = true;
				//break;
			}
		}
		++iterations;
	}

	if (iterations >= MaxIterations)
	{
		r_nextPos = pos;
		//return leaf;
	}
	else {
		r_nextPos.x = nextPos.x;
		r_nextPos.y = nextPos.y;
		floorHeight = NextFloorHeight;
		//return newLeaf;
	}
	// Final corrected position after all slides
}

float Level::GetFloorOfLeaf(const int leaf) const
{
	return m_BSPleafs[leaf].floorheight;
}

int Level::GetLeafWallAmount(const int leaf) const
{
	return int(m_BSPleafs[leaf].wallIndexes.size());
}

Plane* Level::GetWallFromLeaf(const int leaf, const int wallIdx) const
{
	const int wallIndex{ m_BSPleafs[leaf].wallIndexes[wallIdx] };
	return m_pPlanes.at(wallIndex);
}

/// <summary>
/// returns true if the triggered door is the exit, false if it's any other door
/// </summary>
/// <param name="leaf"></param>
/// <param name="wallIdx"></param>
/// <returns>
/// </returns>
bool Level::TriggerDoor(const int leaf, const int wallIdx) const
{
	
	const int wallIndex{ m_BSPleafs[leaf].wallIndexes[wallIdx] };
	if (m_pPlanes.at(wallIndex)->Trigger())
	{
		return true;
	}
	int upIndex{ utils::getArrIndex(m_pPlanes.size(), wallIndex + 1) };
	if (m_pPlanes.at(upIndex)->IsDoor())
	{
		m_pPlanes.at(upIndex)->Trigger();
	}
	int downIndex{ utils::getArrIndex(m_pPlanes.size(), wallIndex - 1) };
	if (m_pPlanes.at(downIndex)->IsDoor())
	{
		m_pPlanes.at(downIndex)->Trigger();
	}
	return false;
}

#pragma endregion collision

#pragma region interactions
const std::vector<Thing*>& Level::GetThings() const 
{
	return m_pThings;
}

const std::vector<Plane*>& Level::GetWalls() const
{
	return m_pPlanes;
}

bool Level::GetCleared() const
{
	return m_isCleared;
}

void Level::SpawnWeapon(point2D_3D Location)
{
	m_pThings.emplace_back(new PowerUp{ Location });
	Vector2f positionThing{ m_pThings.back()->GetStartPos() };
	int leafIndex{ utils::GetBSP(positionThing, true, m_BSP) };
	m_BSPleafs[leafIndex].thingIndexes.push_back(int(m_pThings.size()) - 1);
	float zPos = m_BSPleafs[leafIndex].floorheight;
	m_pThings.back()->SetPosHeight(zPos);
	m_pThings.back()->SetSubsectorIndex(leafIndex);
}
#pragma endregion interactions