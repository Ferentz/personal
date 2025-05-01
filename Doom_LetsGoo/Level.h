#pragma once
#include "BaseGame.h"
#include <vector>
#include "Texture.h"
#include "Utils.h"
#include "structs.h"
#include <set>

class DoomCamera;
class Plane;
class PlaneIregular;
class Thing;
class Enemy;
class Level final
{
public:
	Level();
	~Level();
	//void UpdateLevel(float elapsedSec) override;
	//void DrawLevel() const override;
	void traverseBSP(int leaf, Vector2f pos);
	void DrawLevel();
	void UpdateLevel( DoomCamera& eyeObject, float elapsedSec);
	void CollisionCHeck( DoomCamera& player, const int node) ;
	int ColideWLevel(const Vector3f& pos, Vector3f& nextPos, float& floorHeight, const float charDiameter)const;
	bool crossesBSPborder(Vector2f& pos, Vector2f& nextPos, const int node) const;
	int GetLeafWallAmount(const int leaf) const;
	float GetFloorOfLeaf(const int leaf) const;
	Plane* GetWallFromLeaf(int leaf, int wallIdx) const;
	void TriggerDoor(const int leaf, const int wallIdx) const;
	float m_interval{};
	int m_subSectorToDraw{};
	bool IsInSet(int object);
	const std::vector<BSPnode>& GetBSPtree() const;
private:

	//added vars
	Texture* m_pWallTexture{};
	Texture* m_pFloorTexture{};
	Texture* m_pThingTexture{};
	std::vector<Texture> m_Textures{};
	std::map<std::string, int> m_NameMap{};

	std::vector<BSPnode> m_BSP{};
	std::vector<BSPleafeNode> m_BSPleafs{};
	int m_PlaneAmount{};
	std::vector <Plane*> m_pPlanes{};
	// so now, 
	int m_floorsNCeilingsAmount{};
	std::vector <PlaneIregular> m_pFloorsNCeilings{};
	std::vector <Thing*> r_Things{};

	// we need a vector to store what line a wall belongs to
	std::vector<int> m_LinePerWall;
	std::set<int> m_tags[2];
};

