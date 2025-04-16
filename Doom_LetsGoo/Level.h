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
	void InitializeWallls();
	void traverseBSP(int leaf, Vector2f pos);
	void DrawLevel();
	void UpdateLevel( DoomCamera& eyeObject, float elapsedSec);
	void CollisionCHeck( DoomCamera& player, const int node);
	int getBSP(Vector2f& pos, const bool returnLeaf) const;
	int getBSP(Vector2f& pos, const int node, const bool returnLeaf) const;
	bool crossesBSPborder(Vector2f& pos, Vector2f& nextPos, const int node) const;
	int GetLeafWallAmount(const int leaf) const;
	float GetFloorOfLeaf(const int leaf) const;
	Plane* GetWallFromLeaf(int leaf, int wallIdx) const;
	float m_interval{};
	int m_subSectorToDraw{};
private:

	//added vars
	Texture* m_pWallTexture;
	Texture* m_pFloorTexture;
	std::vector<BSPnode> m_BSP;
	std::vector<BSPleafeNode> m_BSPleafs;
	int m_PlaneAmount{};
	Plane* m_pPlanes{ nullptr };
	int m_floorsNCeilingsAmount{};
	PlaneIregular* m_pFloorsNCeilings{ nullptr };

	// we need a vector to store what line a wall belongs to
	std::vector<int> m_LinePerWall;

	void AddPlane(const Vector2f pSides[], const int lineIndex, const int wallIndex,
		const float floorHeight, const float ceilingHeight, const int16_t* const * pVertexesImported, const int scale);
	void ConstructSubSectors(const std::vector<Vector2f>& boundingBox, std::vector<Line>& sliceLines,
		const LMP& nodes, int startNode, int prevNode, const LMP& subsectors, const LMP& vertexes,
		const LMP& segs, const LMP& lines, const LMP& sides, const LMP& sectors,
		const int SCALE, int* sectorPerSubsector);
	void SeamFixSubsectors(const int* sectorPerSubsector, const LMP& subsectors, const LMP& vertexes,
		const LMP& segs, const LMP& lines, const LMP& sides);
	void sliceLine(const std::vector<Vector2f>& boundingBoxOriginal, std::vector<Vector2f>& boundingBoxNew,
		const Vector2f& partitionPoint1, const Vector2f& partitionPoint2, bool keepEdgePoints = false);
	void SliceSegs(std::vector<Vector2f>& boundingBox, std::vector<Vector2f>& boundingBoxSubSector, int subSector, const LMP& subsectors, const LMP& segs, const LMP& vertexes);
};

