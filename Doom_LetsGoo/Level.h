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
	void DrawLevel();
	void UpdateLevel(const DoomCamera& eyeObject, float elapsedSec);
	float m_interval{};
	int m_subSectorToDraw{};
private:

	//added vars
	Texture* m_pWallTexture;
	Texture* m_pFloorTexture;
	int m_PlaneAmount{};
	Plane* m_pPlanes{ nullptr };
	int m_floorsNCeilingsAmount{};
	PlaneIregular* m_pFloorsNCeilings{ nullptr };
	void AddPlane(const Vector2f pSides[], const int lineIndex, const int wallIndex,
		const float floorHeight, const float ceilingHeight, const int16_t* const * pVertexesImported, const int scale);
	void ConstructFloor(const LMP& subSectorData, const LMP& segData, const LMP& lineData,
		const LMP& sideData, const LMP& sectordata, const LMP& vertexArr, const LMP& nodesArr, const int* nodePerSubSector,
		const float scale, int* const pVertexAmountPerSubSector, int* const * const pVertexesPerSubSector);
	void ConstructSubSectors(const std::vector<Vector2f>& boundingBox, std::vector<Line>& sliceLines,
		const LMP& nodes, int startNode, const LMP& subsectors, const LMP& vertexes,
		const LMP& segs, const LMP& lines, const LMP& sides, const LMP& sectors,
		const int SCALE, int* sectorPerSubsector);
	void SeamFixSubsectors(const int* sectorPerSubsector, const LMP& subsectors, const LMP& vertexes,
		const LMP& segs, const LMP& lines, const LMP& sides);
	void sliceLine(const std::vector<Vector2f>& boundingBoxOriginal, std::vector<Vector2f>& boundingBoxNew,
		const Vector2f& partitionPoint1, const Vector2f& partitionPoint2, bool keepEdgePoints = false);
	void SliceSegs(std::vector<Vector2f>& boundingBox, std::vector<Vector2f>& boundingBoxSubSector, int subSector, const LMP& subsectors, const LMP& segs, const LMP& vertexes);
};

