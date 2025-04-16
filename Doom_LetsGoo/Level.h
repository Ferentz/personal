#pragma once
#include "BaseGame.h"
#include <vector>
#include "Texture.h"
#include "Utils.h"
#include "structs.h"

class DoomCamera;
class Plane;
class PlaneIregular;
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
	void CollisionCHeck( DoomCamera& player, const int node);
	int getBSP(Vector2f& pos, const bool returnLeaf) const;
	int getBSP(Vector2f& pos, const int node, const bool returnLeaf) const;
	bool crossesBSPborder(Vector2f& pos, Vector2f& nextPos, const int node) const;
	int GetLeafWallAmount(const int leaf) const;
	float GetFloorOfLeaf(const int leaf) const;
	const Plane& GetWallFromLeaf(int leaf, int wallIdx) const;
	float m_interval{};
	int m_subSectorToDraw{};
private:

	//added vars
	Texture* m_pWallTexture{};
	Texture* m_pFloorTexture{};
	std::vector<BSPnode> m_BSP{};
	std::vector<BSPleafeNode> m_BSPleafs{};
	int m_PlaneAmount{};
	std::vector <Plane> m_pPlanes{};
	// so now, 
	int m_floorsNCeilingsAmount{};
	std::vector <PlaneIregular> m_pFloorsNCeilings{};

	// we need a vector to store what line a wall belongs to
	std::vector<int> m_LinePerWall;
};

