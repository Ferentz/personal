#include "pch.h"
#include <iostream>
#include "utils.h"
#include "structs.h"

#include "Plane.h"
#include "PlaneIregular.h"
//#include "LevelGenerator.h"

/*
* so, we want to generate the level.
we're gonna load the lmp's and then do shit

from the level, we want to receive the scale

the level just wants the bsp tree, the walls, the floors. and that's it basicaly
*/
//namespace ReadVertexes {
//	const int x{ 0 }, y{ 1 };
//}
//namespace ReadLines {
//	const int vertex1{ 0 }, vertex2{ 1 }, flags{ 2 }, actionSpecial{ 3 },
//		sectorTag{ 4 }, frontSide{ 5 }, backSide{ 6 };
//}
//namespace ReadSides {
//	const int xOffset{0}, yOffset{1}, upperTexture{2}, lowerTexture{6},
//		middleTexture{10}, sector{14};
//}
//namespace ReadSectors {
//	const int floorHeight{ 0}, ceilingHeight{1}, floorTexture{2}, ceilingTexture{6},
//		lightLevel{10}, special{11}, tag{12};
//}
//namespace ReadNodes {
//	const int partitionX{0}, partitionY{1}, pXdiff{2}, pYdiff{3}, rBoxTop{4},
//		rBoxBottom{5}, rBoxLeft{6}, rBoxRight{7}, lBoxLop{8}, lBoxBottom{9},
//		lBoxLeft{10}, lBoxRight{11}, rightChild{12}, leftChild{13};
//}
//namespace ReadSegs {
//	const int vertex1 {0}, vertex2 {1}, angle{2}, line{3}, side{4}, offset{5};
//}
//
//namespace ReadSectorSegs {
//	const int segCount{ 0 }, firstSeg{ 1 };
//}
//void GenerateLevel(std::vector<BSPnode>& p_BSPtree, std::vector<BSPnode>& p_BSPleafs,
//	std::vector <Plane>& p_Planes, std::vector <PlaneIregular>& p_FloorsNCeilings)
//{
//
//	const float SCALE{ 1.f };
//	//void MakeTopWall();
//	//void MakeMiddleWall();
//	//void MakeBottomWall();
//
//
//	// loading the source files
//	// IMPORTANT | unimportant/not implemented ATM
//
//	// vertexes.
//	//		2	   |	2
//	//  X POSITION | Y POSITION
//	const int VERTEXCOLUMS{ 2 }; // 2 * bytepairs = 2
//	const LMP vertexesImported{ utils::ReadLMP("VERTEXES.lmp", VERTEXCOLUMS) };
//
//	// lines. defines the walls/doors
//	//   2		|    2	   |   2   |       2		|     2		 |     2	  |   2
//	// VERTEX 1 | VERTEX 2 | flags | action special | sector tag | FRONT SIDE | BACK SIDE
//	const int LINECOLUMS{ 7 }; // 7 * bytepairs = 7
//	const LMP linesImported{ utils::ReadLMP("LINEDEFS.lmp", LINECOLUMS) };
//
//	// sides. defines what the walls look like + sector 
//	//    2	    |    2	   |    8	   |     8	   |     8		|   2
//	// x offset | y offset | UPPER TEX | LOWER TEX | MIDDLE TEX | SECTORS
//	const int SIDECOLUMS{ 15 }; // (3 * bytepair) + (3 * ( 4 * bytepair))  = 15
//	const LMP sidesImported{ utils::ReadLMP("SIDEDEFS.lmp", SIDECOLUMS) };
//
//	// sectors. sector def and visualization
//	//     2	   |    2		   | 8		   |  8		  |  2			|   2	  | 2
//	// FLOOR HEIGH | CEILING HEIGH | FLOOR TEX | CEIL TEX | LIGHT LEVEL | special | tag
//	//fun fact, properly defining these gave me a breakthrough in an issue i had
//	const int SECTORCOLUMS{ 13 }; // (5 * bytepair) + (2 * ( 4 * bytepair))  = 13
//	const LMP sectorsImported{ utils::ReadLMP("SECTORS.lmp", SECTORCOLUMS) };
//
//	// sectors. sector def and visualization
//	//     2	   |    2		 |		2	|   2	  |  2		 |   2		   |	2	   |	2	   |    2	  |		2		|   2		|	2		 |   2		   | 2 
//	// partition x | partition y | p x diff | p y diff| rBox top | rBox bottom | rBox left | rBox right| lBox top | lBox bottom | lBox left | lBox right | right child | left child
//	const int NODESCULOMS{ 14 }; // 14 bytepairs. | 4 bytepairs + 2 * (4 bytepairs (bounding boxes)) + 2 bytepairs
//	const LMP nodesImported{ utils::ReadLMP("NODES.lmp", NODESCULOMS) };
//
//	// segments. line segments of the roof/floor polygons
//	//    2		|    2	   | 2	   |  2	  |  2	 |   2
//	// VERTEX 1 | VERTEX 2 | angle | LINE | SIDE | offset 
//	const int SEGSCOLUMS{ 6 }; // 6 * bytepair = 6
//	const LMP segsImported{ utils::ReadLMP("SEGS.lmp", SEGSCOLUMS) };
//
//	// segments for each sector
//	//    2		 |    2	  
//	// SEG COUNT | FIRST SEG
//	const int SECTORSEGSCOLUMS{ 2 }; // 2 * bytepair = 2
//	const LMP sectorSegsImported{ utils::ReadLMP("SSECTORS.lmp", SECTORSEGSCOLUMS) };
//
//	
//	p_BSPtree.resize(nodesImported.rows);
//	p_BSPleafs.resize(sectorSegsImported.rows);
//	p_Planes.reserve(segsImported.rows);
//	p_FloorsNCeilings.reserve(sectorSegsImported.rows * 2); // 1 for the floor and 1 for the ceiling
//
//	std::vector<int> linePerWall;
//	linePerWall.reserve(sidesImported.rows);
//
//	//https://learn.microsoft.com/en-us/cpp/cpp/lambda-expressions-in-cpp?view=msvc-170
//	auto AddPlane = [&](int line, float ceilingHeight, float floorHeight)
//		{
//			int vertex1 = int(linesImported.data[ReadLines::vertex1]);
//			int vertex2 = int(linesImported.data[ReadLines::vertex2]);
//			//const int SCALE{ 10 };
//			float wallHeight{ ceilingHeight - floorHeight };
//
//			Vector3f vertex1Loc{ vertexesImported.data[vertex1][ReadVertexes::x] / SCALE, vertexesImported.data[vertex1][ReadVertexes::y] / SCALE };
//			Vector3f vertex2Loc{ vertexesImported.data[vertex2][ReadVertexes::x] / SCALE, vertexesImported.data[vertex2][ReadVertexes::y] / SCALE };
//
//			Vector3f width{ vertex2Loc.x - vertex1Loc.x,  vertex2Loc.y - vertex1Loc.y , 0 };
//			Vector3f height{ 0,0,wallHeight };
//			Vector3f location{ vertex1Loc.x, vertex1Loc.y, floorHeight };
//			//Plane plane2{ positionPlanea, widtha, heighta, 2 };
//			//Plane input{ location, width, height, 2 };
//			p_Planes.emplace_back(Plane{ location, width, height, 2 });
//		};
//	auto GetSectorFromLine = [&](int line, bool backSide) -> int
//		{
//			int sideDef{ linesImported.data[line][ReadLines::frontSide + int(backSide)] };
//			return sidesImported.data[sideDef][ReadSides::sector];
//		};
//
//
//	for (int line = 0; line < linesImported.rows; ++line) {
//
//		bool twoSectored{};
//		if (linesImported.data[line][ReadLines::backSide] != -1) {
//			twoSectored = true;
//		}
//
//		int sideDef{ linesImported.data[line][ReadLines::frontSide] };
//		int sector{ sidesImported.data[sideDef][ReadSides::sector] };
//
//		std::string upperTexture = utils::convertToCharString(&sidesImported.data[sideDef][ReadSides::upperTexture]);
//		std::string lowerTexture = utils::convertToCharString(&sidesImported.data[sideDef][ReadSides::lowerTexture]);
//		std::string middleTexture = utils::convertToCharString(&sidesImported.data[sideDef][ReadSides::middleTexture]);
//		
//		float ceilingHeight = sectorsImported.data[sector][ReadSectors::ceilingHeight] / SCALE;
//		float floorHeight = sectorsImported.data[sector][0] / SCALE;
//
//
//		if (twoSectored)
//		{
//			int sector2{ GetSectorFromLine(line, true) };
//			float ceilingHeight2 = sectorsImported.data[sector2][ReadSectors::ceilingHeight] / SCALE;
//			float floorHeight2 = sectorsImported.data[sector2][ReadSectors::floorHeight] / SCALE;
//
//			if (upperTexture.find("-") == std::string::npos)
//			{
//				linePerWall.push_back(line);
//				AddPlane(line, ceilingHeight2, ceilingHeight);
//				//g_Sectors[wallamount] = sector;
//			}
//			if (middleTexture.find("-") == std::string::npos)
//			{
//				float floorHeight2Use{ floorHeight };
//				if (floorHeight < floorHeight2) {
//					floorHeight2Use = floorHeight2;
//				}
//				float ceilingHeight2Use{ ceilingHeight };
//				if (ceilingHeight > ceilingHeight2) {
//					ceilingHeight2Use = ceilingHeight2;
//				}
//				linePerWall.push_back(line);
//				AddPlane(line, floorHeight2Use, ceilingHeight2Use);
//			}
//			if (lowerTexture.find("-") == std::string::npos)
//			{
//				linePerWall.push_back(line);
//				AddPlane(line, floorHeight2, floorHeight);
//			}
//		}
//		else if (middleTexture.find("-") == std::string::npos) {
//			linePerWall.push_back(line);
//			AddPlane(line, floorHeight, ceilingHeight);
//		}
//		// bottom
//		
//	}
//
//	const int buffer{ 1000 };
//	float top = nodesImported.data[nodesImported.rows - 1][8] + buffer;
//	float bottom = nodesImported.data[nodesImported.rows - 1][5] - buffer;
//	float left = nodesImported.data[nodesImported.rows - 1][6] - buffer;
//	float right = nodesImported.data[nodesImported.rows - 1][11] + buffer;
//
//	auto ConstructSubSectors = [&](int line, float ceilingHeight, float floorHeight)
//		{
//		};
//	std::vector<Vector2f> startingBox{ Vector2f{left, top},Vector2f{right, top},Vector2f{right, bottom },Vector2f{left, bottom} };
//	std::vector<Line> sliceLines;
//	ConstructSubSectors(startingBox, sliceLines,
//		nodesImported, nodesImported.rows - 1, sectorSegsImported, vertexesImported,
//		segsImported, linesImported, sidesImported, sectorsImported,
//		SCALE, sectorPerSubsector);
//
//}
