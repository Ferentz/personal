#pragma once
#include <map>
#include <vector>

class Plane;
class PlaneIregular;
class Thing;
class Texture;
class Player;


class LevelGenerator final
{
public:
	explicit LevelGenerator(std::vector<BSPnode>& p_BSPtree, std::vector<BSPleafeNode>& p_BSPleafs,
		std::vector <Plane*>& p_Planes, std::vector <PlaneIregular>& p_FloorsNCeilings,
		std::vector <Thing*>& things, std::vector<Texture>& m_Textures, Player& player);
	
private:
	void GenerateLevel();
	
	//generate level helpers
	void MakeWalls(const float SCALE);
	void ConstructSubSectors(const std::vector<Vector2f>& boundingBox, std::vector<Line>& sliceLines,
			int startNode, int prevNode, const float SCALE, int* sectorPerSubsector);
	void SeamFixSubsectors(const int* sectorPerSubsector);
	void ReadThings();
	void AdjustThings();
	//hardcoded data
	void MapNames();
	void AddFnCTextures();
	void GetTextureStructs();
	
	// MakeWalls helpers
	void AddPlane(int line, float floorHeight, float ceilingHeight, const float SCALE, int idx);
	bool WillBeDoor(uint16_t actionBit);
	int GetSectorFromLine(int line, bool backSide) const;

	// ConstructSubSectors helpers
	void SliceLine(const std::vector<Vector2f>& boundingBoxOriginal, std::vector<Vector2f>& boundingBoxNew, const Vector2f& partitionPoint1, const Vector2f& partitionPoint2, bool keepEdgePoints);
	void SliceSegs(std::vector<Vector2f>& boundingBox, std::vector<Vector2f>& boundingBoxSubSector, int subSector, const LMP& subsectors, const LMP& segs, const LMP& vertexes);

	

	// vertexes.
	//		2	   |	2
	//  X POSITION | Y POSITION
	const int VERTEXCOLUMS{ 2 };
	const LMP vertexesImported;

	// lines. defines the walls/doors
	//   2		|    2	   |   2   |       2		|     2		 |     2	  |   2
	// VERTEX 1 | VERTEX 2 | flags | action special | sector tag | FRONT SIDE | BACK SIDE
	const int LINECOLUMS{ 7 };
	const LMP linesImported;

	// sides. defines what the walls look like + sector 
	//    2	    |    2	   |    8	   |     8	   |     8		|   2
	// x offset | y offset | UPPER TEX | LOWER TEX | MIDDLE TEX | SECTORS
	const int SIDECOLUMS{ 15 };
	const LMP sidesImported;

	// sectors. sector def and visualization
	//     2	   |    2		   | 8		   |  8		  |  2			|   2	  | 2
	// FLOOR HEIGH | CEILING HEIGH | FLOOR TEX | CEIL TEX | LIGHT LEVEL | special | tag
	//fun fact, properly defining these gave me a breakthrough in an issue i had
	const int SECTORCOLUMS{ 13 };
	const LMP sectorsImported;

	// sectors. sector def and visualization
	//     2	   |    2		 |		2	|   2	  |  2		 |   2		   |	2	   |	2	   |    2	  |		2		|   2		|	2		 |   2		   | 2 
	// partition x | partition y | p x diff | p y diff| rBox top | rBox bottom | rBox left | rBox right| lBox top | lBox bottom | lBox left | lBox right | right child | left child
	const int NODESCULOMS{ 14 };
	const LMP nodesImported;

	// segments. line segments of the roof/floor polygons
	//    2		|    2	   | 2	   |  2	  |  2	 |   2
	// VERTEX 1 | VERTEX 2 | angle | LINE | SIDE | offset 
	const int SEGSCOLUMS{ 6 };
	const LMP segsImported;

	// segments for each sector
	//    2		 |    2	  
	// SEG COUNT | FIRST SEG
	const int SECTORSEGSCOLUMS{ 2 };
	const LMP sectorSegsImported;

	const int THINGSSCOLUMS{ 5 };
	const LMP thingsImported;

	//const int TEXNAMESCOLUMS{ 6 };
	//const LMP texNamesImported;

	std::vector<BSPnode>& r_BSPtree;
	std::vector<BSPleafeNode>& r_BSPleafs;
	std::vector <Plane*>& r_pPlanes;
	std::vector <PlaneIregular>& r_FloorsNCeilings;
	std::vector <Thing*>& r_pThings;
	std::vector<Texture>& r_Textures;
	Player& r_Player;

	std::map<std::string, int> m_NameMap{};
	std::vector<int> m_LinePerWall{};
	std::vector<WallTexture> m_Walltextures{};
};

#pragma region LMP reading namespaces
namespace ReadVertexes {
	const int x{ 0 }, y{ 1 };
}
namespace ReadLines {
	const int vertex1{ 0 }, vertex2{ 1 }, flags{ 2 },
		actionSpecial{ 3 },
		sectorTag{ 4 }, frontSide{ 5 }, backSide{ 6 };
}
namespace ReadSides {
	const int xOffset{ 0 }, yOffset{ 1 }, upperTexture{ 2 },
		lowerTexture{ 6 },
		middleTexture{ 10 }, sector{ 14 };
}
namespace ReadSectors {
	const int floorHeight{ 0 }, ceilingHeight{ 1 }, floorTexture{ 2 },
		ceilingTexture{ 6 },
		lightLevel{ 10 }, special{ 11 }, tag{ 12 };
}
namespace ReadNodes {
	const int partitionX{ 0 }, partitionY{ 1 }, pXdiff{ 2 },
		pYdiff{ 3 }, rBoxTop{ 4 },
		rBoxBottom{ 5 }, rBoxLeft{ 6 }, rBoxRight{ 7 }, lBoxLop{ 8 },
		lBoxBottom{ 9 },
		lBoxLeft{ 10 }, lBoxRight{ 11 }, rightChild{ 12 },
		leftChild{ 13 };
}
namespace ReadSegs {
	const int vertex1{ 0 }, vertex2{ 1 }, angle{ 2 }, line{ 3 },
		side{ 4 }, offset{ 5 };
}

namespace ReadSectorSegs {
	const int segCount{ 0 }, firstSeg{ 1 };
}

namespace ReadThings {
	const int xPos{ 0 }, yPos{ 1 }, direction{ 2 }, type{ 3 }, flags{ 4 };
}

namespace ReadTexNames {
	const int nameNum{ 0 }, nameIndex{ 4 };
}

#pragma endregion LMP reading namespaces