#include "pch.h"
#include <iostream>

#include "Plane.h"
#include "Door.h"
#include "PlaneIregular.h"
#include "LevelGenerator.h"
//#include "LevelGenerator.h"

/*
* so, we want to generate the level.
we're gonna load the lmp's and then do shit

from the level, we want to receive the scale

the level just wants the bsp tree, the walls, the floors. and that's it basicaly
*/


LevelGenerator::LevelGenerator(std::vector<BSPnode>& p_BSPtree_p, std::vector<BSPleafeNode>& p_BSPleafs_p,
	std::vector <Plane*>& p_Planes_p, std::vector <PlaneIregular>& p_FloorsNCeilings_p, std::set<int>& tags1, std::set<int>& tags2)
	:vertexesImported{ utils::ReadLMP("VERTEXES.lmp", VERTEXCOLUMS) },
	linesImported{ utils::ReadLMP("LINEDEFS.lmp", LINECOLUMS) },
	sidesImported{ utils::ReadLMP("SIDEDEFS.lmp", SIDECOLUMS) },
	sectorsImported{ utils::ReadLMP("SECTORS.lmp", SECTORCOLUMS) },
	nodesImported{ utils::ReadLMP("NODES.lmp", NODESCULOMS) },
	segsImported{ utils::ReadLMP("SEGS.lmp", SEGSCOLUMS) },
	sectorSegsImported{ utils::ReadLMP("SSECTORS.lmp", SECTORSEGSCOLUMS) },
	r_BSPtree{ p_BSPtree_p },
	r_BSPleafs{ p_BSPleafs_p },
	r_Planes{ p_Planes_p },
	r_FloorsNCeilings{ p_FloorsNCeilings_p },
	r_tags{ tags1 , tags2 }
{
	GenerateLevel();
}
bool WillBeDoor(uint16_t actionBit)
{
	return (actionBit & 0x0001) != 0;
	//m_bBlockMonster = (actionBit & 0x0002) != 0;
	//m_bTwoSided = (actionBit & 0x0004) != 0;
	//m_bUnpeggedUpperTex = (actionBit & 0x0008) != 0;
	//m_bUnpeggedLowerTex = (actionBit & 0x0010) != 0;
	//m_bSecret = (actionBit & 0x0020) != 0;
	//m_bSoundBlock = (actionBit & 0x0040) != 0;
	//m_bNotOnAutoMap = (actionBit & 0x0080) != 0;
	//m_bAlwaysOnAUtoMap = (actionBit & 0x0100) != 0;
}
void LevelGenerator::AddPlane(int line, float floorHeight, float ceilingHeight, const float SCALE)
{
	int vertex1 = int(linesImported.data[line][ReadLines::vertex1]);
	int vertex2 = int(linesImported.data[line][ReadLines::vertex2]);

	//pLines[i].x = linesImported.data[i][0];
	//pLines[i].y = linesImported.data[i][1];

	//int vertex1 = pSides[lineIndex].x;
	//int vertex2 = pSides[lineIndex].y;
	
	
	//const int SCALE{ 10 };
	float wallHeight{ ceilingHeight - floorHeight };

	Vector3f vertex1Loc{ vertexesImported.data[vertex1][ReadVertexes::x] / SCALE, vertexesImported.data[vertex1][ReadVertexes::y] / SCALE };
	Vector3f vertex2Loc{ vertexesImported.data[vertex2][ReadVertexes::x] / SCALE, vertexesImported.data[vertex2][ReadVertexes::y] / SCALE };

	Vector3f width{ vertex2Loc.x - vertex1Loc.x,  vertex2Loc.y - vertex1Loc.y , 0 };
	Vector3f height{ 0,0,wallHeight };
	Vector3f location{ vertex1Loc.x, vertex1Loc.y, floorHeight };
	//Plane plane2{ positionPlanea, widtha, heighta, 2 };
	if (linesImported.data[line][ReadLines::sectorTag] == 1)
	{
		r_tags[0].insert(r_Planes.size());
	}
	if (linesImported.data[line][ReadLines::sectorTag] == 2)
	{
		r_tags[1].insert(r_Planes.size());
	}
	if (WillBeDoor(linesImported.data[line][ReadLines::actionSpecial]))
	{
		r_Planes.emplace_back(new Door{ location, width, height, 2, ceilingHeight, floorHeight });
	}
	else
	{
		r_Planes.emplace_back(new Plane{ location, width, height, 2 });
	}
	r_Planes.back()->SetActionBools(linesImported.data[line][ReadLines::actionSpecial]);
}

int LevelGenerator::GetSectorFromLine(int line, bool backSide)
{
	int sideDef{ linesImported.data[line][ReadLines::frontSide + int(backSide)] };
	return sidesImported.data[sideDef][ReadSides::sector];
}

void sliceLine(const std::vector<Vector2f>& boundingBoxOriginal, std::vector<Vector2f>& boundingBoxNew,
	const Vector2f& partitionPoint1, const Vector2f& partitionPoint2, bool keepEdgePoints) {
	for (int point{}; point < boundingBoxOriginal.size(); point++)
	{
		// we make a new vector to store the child points


		// we take a point from the bounding box
		Vector2f segPoint1 = boundingBoxOriginal[point];

		//and check if it belongs to the left or right side
		// To find orientation of ordered triplet (p, q, r). 
		// The function returns following values 
		// 0 --> p1, p2 and r are collinear 
		// 1 --> Counterclockwise 
		// -1 --> Clockwise 
		int orientation{ segPoint1.orientation(partitionPoint1, partitionPoint2) };

		//left and right are to be done subsequentialy
		// 0 is right side
		// 1 is left side
		if (orientation < 0)
		{
			//right side
			boundingBoxNew.push_back(segPoint1);
		}
		if (orientation == 0) {
			boundingBoxNew.push_back(segPoint1);
		}
		// get the following point of the bounding box
		int nextPoint = utils::getArrIndex(boundingBoxOriginal.size(), point + 1);
		Vector2f segPoint2 = boundingBoxOriginal[nextPoint];
		Vector2f intersectionPoint{}; // point in which the potential intersection is stored.
		//check if the partition line and the bounding box segment intersect
		//bool intersectsWithPartition = Vector2f::doIntersect(
		bool intersectsWithPartition = Vector2f::intersection(
			partitionPoint1, partitionPoint2, segPoint1, segPoint2, intersectionPoint
		);

		if (keepEdgePoints) {
			Vector2f partPoint1{ partitionPoint1 };
			if (partPoint1.WithinSegmentBounds(segPoint1, segPoint2))
			{
				boundingBoxNew.push_back(partPoint1);
			}
		}


		//if they intersect, add the intersectionPoint to the child bounding box.
		//all intersection pointys should always be added.
		if (intersectsWithPartition) {
			if (intersectionPoint.WithinSegmentBounds(segPoint1, segPoint2))
			{
				boundingBoxNew.push_back(intersectionPoint);
			}
		}

		if (keepEdgePoints) {
			Vector2f partPoint2{ partitionPoint2 };
			if (partPoint2.WithinSegmentBounds(segPoint1, segPoint2))
			{
				boundingBoxNew.push_back(partPoint2);
			}
		}
	}
}

void SliceSegs(std::vector<Vector2f>& boundingBox, std::vector<Vector2f>& boundingBoxSubSector, int subSector, const LMP& subsectors, const LMP& segs, const LMP& vertexes)
{
	int amountOfSegs{ subsectors.data[subSector][0] };
	int startSeg{ subsectors.data[subSector][1] };
	std::vector<Vector2f> inBox{};
	std::vector<Vector2f> outbox{};
	for (int i{}; i < boundingBox.size(); i++) {
		inBox.push_back(boundingBox[i]);
	}
	for (int seg{ startSeg }; seg < startSeg + amountOfSegs; seg++) {
		outbox.clear();
		// we basicaly want to do the same as when we sliced the partition lines, so we should make a function.
		Vector2f firstVertex{ float(vertexes.data[segs.data[seg][0]][0]), float(vertexes.data[segs.data[seg][0]][1]) };
		Vector2f secondVertex{ float(vertexes.data[segs.data[seg][1]][0]), float(vertexes.data[segs.data[seg][1]][1]) };
		sliceLine(inBox, outbox, firstVertex, secondVertex, true);
		inBox.clear();
		for (int i{}; i < outbox.size(); i++) {
			inBox.push_back(outbox[i]);
		}
	}
	for (int i{}; i < outbox.size(); i++) {
		boundingBoxSubSector.push_back(outbox[i]);
	}
}


void LevelGenerator::ConstructSubSectors(const std::vector<Vector2f>& boundingBox, std::vector<Line>& sliceLines,
	int currentNode, int prevNode, const float SCALE, int* sectorPerSubsector)
{
	//     0	   |    1		 |	  2		|   3	  |
	// partition x | partition y | p x diff | p y diff|
	//   4		|     5		  |    6	  |		7	  |
	// rBox top | rBox bottom | rBox left | rBox right|
	//    8		|		9	  |    10	  |		11	   |
	// lBox top | lBox bottom | lBox left | lBox right |
	//		12	   |	13 
	// right child | left child

	// we need for each subsector an array of vector2f's
	// we need a vector of vector2f's along which all the points will be handled

	Vector2f partitionPoint1{ float(nodesImported.data[currentNode][ReadNodes::partitionX]),
		float(nodesImported.data[currentNode][ReadNodes::partitionY]) };
	Vector2f partitionPoint2{ float(partitionPoint1.x + nodesImported.data[currentNode][ReadNodes::pXdiff]),
		float(partitionPoint1.y + nodesImported.data[currentNode][ReadNodes::pYdiff]) };

	//we can pass on the slicelines,but what about the sides?
	//

	int childIndex = ReadNodes::rightChild;
	for (int childOffset{}; childOffset < 2; childOffset++) // right child then left child
	{
		int childNodeIndex{ nodesImported.data[currentNode][childIndex + childOffset] };

		if (childOffset == 0)
		{ // right child
			sliceLines.push_back(Line{ partitionPoint1, partitionPoint2 });
		}
		else
		{ // left child
			sliceLines.push_back(Line{ partitionPoint2, partitionPoint1 });
		}
		r_BSPtree[currentNode].partitionLine = Line{ partitionPoint1, partitionPoint2 };
		r_BSPtree[currentNode].children[childOffset] = childNodeIndex;
		r_BSPtree[currentNode].parent = prevNode;

		if (childNodeIndex < 0)
		{
			// it's a subSector
			const int subSector{ utils::InvertInt(childNodeIndex) };

			std::vector<Vector2f> boundingBoxParent{};
			for (int vertex{}; vertex < boundingBox.size(); vertex++)
			{
				boundingBoxParent.push_back(boundingBox[vertex]);
			}
			
			for (int line{}; line < sliceLines.size(); line++)
			{
				std::vector<Vector2f> boundingBoxChild{};
				sliceLine(boundingBoxParent, boundingBoxChild, sliceLines[line].startPoint, sliceLines[line].endPoint, false);
				boundingBoxParent.clear();


				for (int vertex{}; vertex < boundingBoxChild.size(); vertex++)
				{
					boundingBoxParent.push_back(boundingBoxChild[vertex]);
				}
			}
			//now we wanna generate the actual plane.
			std::vector<Vector2f> boundingBoxChild{};
			SliceSegs(boundingBoxParent, boundingBoxChild, subSector, sectorSegsImported,
				segsImported, vertexesImported);

			int seg = sectorSegsImported.data[subSector][1];
			//if (subSector == 15) {
				//std::cout << " seg " << seg << std::endl;
			//}
			int lineI = segsImported.data[seg][3]; // it just gets the first line. we need the rest too

			int orienttion = segsImported.data[seg][4]; //these are curently magic numbers. they are derived from the file structure, im thinking of putting them in an enumerator
			int sideIndex{ ReadLines::frontSide };
			if (orienttion > 0)
			{ // 0 : front side | 1 : backside
				sideIndex = ReadLines::backSide;
			}
			int side = linesImported.data[lineI][sideIndex];
			int sector = sidesImported.data[side][ReadSides::sector];

			if (sectorsImported.data[sector][ReadSectors::tag] != 0)
			{
				std::cout << sectorsImported.data[sector][ReadSectors::tag] << std::endl;
			}
			if (sectorsImported.data[sector][ReadSectors::tag] == 1)
			{
				r_tags[0].insert(-subSector);
			}
			if (sectorsImported.data[sector][ReadSectors::tag] == 2)
			{
				r_tags[1].insert(-subSector);
			}

			sectorPerSubsector[subSector] = sector;
			sectorPerSubsector[subSector + 1] = sector;

			for (int vertex{}; vertex < boundingBoxChild.size(); vertex++)
			{
				boundingBoxChild[vertex].x = boundingBoxChild[vertex].x / SCALE;
				boundingBoxChild[vertex].y = boundingBoxChild[vertex].y / SCALE;
			}
			/*for (int vertex{}; vertex < boundingBox.size(); vertex++)
			{
				boundingBoxParent.push_back(Vector2f{ boundingBoxParent[vertex].x / SCALE, boundingBoxParent[vertex].y / SCALE });
			}*/
			float ceilingHeight = sectorsImported.data[sector][1] / SCALE;
			float floorHeight = sectorsImported.data[sector][0] / SCALE;
			r_BSPleafs[subSector].startSeg = seg;
			r_BSPleafs[subSector].amountOfSegs = sectorSegsImported.data[subSector][0];
			r_BSPleafs[subSector].floorheight = floorHeight;
			r_BSPleafs[subSector].parent = currentNode;
			PlaneIregular input{ boundingBoxChild, sector, int(ceilingHeight) };
			PlaneIregular input2{ boundingBoxChild, 3, int(floorHeight) };
			r_FloorsNCeilings[subSector*2] = input;
			r_FloorsNCeilings[subSector*2 + 1] = input2;





			//}
		}
		else
		{
			ConstructSubSectors(boundingBox, sliceLines, 
				childNodeIndex, currentNode, SCALE, sectorPerSubsector);
		}

		sliceLines.pop_back();
	}
}

// so, we have the flag on linedefs
// it's an 8 way bool (each bit sets a flag)
/*
0	0x0001	blocks players and monsters
1	0x0002	blocks monsters
2	0x0004	two sided
3	0x0008	upper texture is unpegged
4	0x0010	lower texture is unpegged
5	0x0020	secret (shows as one-sided on automap), and monsters cannot open if it is a door (type 1)
6	0x0040	blocks sound
7	0x0080	never shows on automap
8	0x0100	always shows on automap
*/

void LevelGenerator::MakeWalls(const float SCALE)
{

	for (int line = 0; line < linesImported.rows; ++line) {

		bool twoSectored{};
		if (linesImported.data[line][ReadLines::backSide] != -1) {
			twoSectored = true;
		}

		int sideDef{ linesImported.data[line][ReadLines::frontSide] };
		if (linesImported.data[line][ReadLines::sectorTag] != 0)
		{
			std::cout << linesImported.data[line][ReadLines::sectorTag] << std::endl;
		}
		
		int sector{ sidesImported.data[sideDef][ReadSides::sector] };

		std::string upperTexture = utils::convertToCharString(&sidesImported.data[sideDef][ReadSides::upperTexture]);
		std::string lowerTexture = utils::convertToCharString(&sidesImported.data[sideDef][ReadSides::lowerTexture]);
		std::string middleTexture = utils::convertToCharString(&sidesImported.data[sideDef][ReadSides::middleTexture]);

		float ceilingHeight = sectorsImported.data[sector][ReadSectors::ceilingHeight] / SCALE;
		float floorHeight = sectorsImported.data[sector][0] / SCALE;


		if (twoSectored)
		{
			int sector2{ GetSectorFromLine(line, true) };
			float ceilingHeight2 = sectorsImported.data[sector2][ReadSectors::ceilingHeight] / SCALE;
			float floorHeight2 = sectorsImported.data[sector2][ReadSectors::floorHeight] / SCALE;

			if (upperTexture.find("-") == std::string::npos)
			{
				linePerWall.push_back(line);
				AddPlane(line, ceilingHeight2, ceilingHeight, SCALE);
				//g_Sectors[wallamount] = sector;
			}
			if (middleTexture.find("-") == std::string::npos)
			{
				float floorHeight2Use{ floorHeight };
				if (floorHeight < floorHeight2) {
					floorHeight2Use = floorHeight2;
				}
				float ceilingHeight2Use{ ceilingHeight };
				if (ceilingHeight > ceilingHeight2) {
					ceilingHeight2Use = ceilingHeight2;
				}
				linePerWall.push_back(line);
				AddPlane(line, floorHeight2Use, ceilingHeight2Use, SCALE);
			}
			if (lowerTexture.find("-") == std::string::npos)
			{
				linePerWall.push_back(line);
				AddPlane(line, floorHeight2, floorHeight, SCALE);
			}
		}
		else if (middleTexture.find("-") == std::string::npos) {
			linePerWall.push_back(line);
			AddPlane(line, floorHeight, ceilingHeight, SCALE);
		}
		// bottom

	}
}

void LevelGenerator::GenerateLevel()
{

	const float SCALE{ 1.f };
	
	
	r_BSPtree.resize(nodesImported.rows);
	r_BSPleafs.resize(sectorSegsImported.rows);
	r_Planes.reserve(segsImported.rows);
	//p_FloorsNCeilings.reserve(sectorSegsImported.rows * 2); // 1 for the floor and 1 for the ceiling
	r_FloorsNCeilings.resize(sectorSegsImported.rows * 2);
	
	linePerWall.reserve(sidesImported.rows);

	// the walls have a sector tag
	MakeWalls(SCALE);

	const int buffer{ 1000 };
	float top = nodesImported.data[nodesImported.rows - 1][8] + buffer;
	float bottom = nodesImported.data[nodesImported.rows - 1][5] - buffer;
	float left = nodesImported.data[nodesImported.rows - 1][6] - buffer;
	float right = nodesImported.data[nodesImported.rows - 1][11] + buffer;

	std::vector<Vector2f> startingBox{ Vector2f{left, top},Vector2f{right, top},Vector2f{right, bottom },Vector2f{left, bottom} };
	std::vector<Line> sliceLines;
	int* sectorPerSubsector{ new int[sectorSegsImported.rows * 2] };
	std::cout << "--" << std::endl;
	//the sectors have a tag
	ConstructSubSectors(startingBox, sliceLines, nodesImported.rows - 1, nodesImported.rows - 1,
		SCALE, sectorPerSubsector);

	SeamFixSubsectors(sectorPerSubsector);

	for (int wallIndex{}; wallIndex < linePerWall.size(); wallIndex++)
	{
		for (int leafnode{}; leafnode < r_BSPleafs.size(); leafnode++)
		{
			for (int segOffset{}; segOffset < r_BSPleafs[leafnode].amountOfSegs; segOffset++)
			{
				int line = segsImported.data[r_BSPleafs[leafnode].startSeg + segOffset][3];
				if (linePerWall[wallIndex] == line)
				{
					r_BSPleafs[leafnode].wallIndexes.push_back(wallIndex);
				}
			}

			// so we gotta get the first line, and then all lines after.

		}
	}

	delete[] sectorPerSubsector;
}

void LevelGenerator::SeamFixSubsectors(const int* sectorPerSubsector)
{
	for (int subsectorThis{}; subsectorThis < r_FloorsNCeilings.size(); subsectorThis++) //we go over all the subsectors
	{
		PlaneIregular& thisPlane{ r_FloorsNCeilings[subsectorThis] };
		int seg = sectorSegsImported.data[subsectorThis / 2][ReadSectorSegs::firstSeg];
		int lineI = segsImported.data[seg][ReadSegs::line];

		int orienttion = segsImported.data[seg][ReadSegs::side]; //these are curently magic numbers. they are derived from the file structure, im thinking of putting them in an enumerator
		int sideIndex{ ReadLines::frontSide };
		if (orienttion > 0)
		{ // 0 : front side | 1 : backside
			sideIndex = ReadLines::backSide;
		}
		int side = linesImported.data[lineI][sideIndex];
		int sector = sidesImported.data[side][ReadSides::sector]; // get the sector that the subsector belongs to
		int sectorThis{ sectorPerSubsector[subsectorThis] };
		for (int subSectorNeighbor{}; subSectorNeighbor < r_FloorsNCeilings.size(); subSectorNeighbor++)
		{
			if (subsectorThis % 1 == subSectorNeighbor % 1) // dont compare ceilings and floors
			{
				int sectorNeighbor = sectorPerSubsector[subSectorNeighbor]; // get the other's sector
				if (sectorThis == sectorNeighbor) // if they match, we need to start comparing points
				{
					// so, the sector is the same.
					// now we gotta check if the points are on the edges of the subsector
					// if they are on the subsector edges, add them in between

					PlaneIregular neighborPlane{ r_FloorsNCeilings[subSectorNeighbor] };
					int amountOfPointsNeighbor{ neighborPlane.GetAmountPoints() };

					for (int indexNeighbor{}; indexNeighbor < amountOfPointsNeighbor; indexNeighbor++)
					{
						point2D_3D curPointNeighbor{ neighborPlane.GetPoint(indexNeighbor) };
						Vector2f curPointNeighbor_XY{ curPointNeighbor.point3D.x, curPointNeighbor.point3D.y };
						int popIndex{ -1 };

						int amountOfPointsThis{ thisPlane.GetAmountPoints() };
						for (int indexThis{}; indexThis < amountOfPointsThis; indexThis++)
						{
							point2D_3D curPointThis{ thisPlane.GetPoint(indexThis) };
							Vector2f curPointThis_XY{ curPointThis.point3D.x, curPointThis.point3D.y };


							int nextIndexThis{ utils::getArrIndex(amountOfPointsThis, indexThis + 1) };
							point2D_3D nextPointThis{ thisPlane.GetPoint(nextIndexThis) };
							Vector2f nextPointThis_XY{ nextPointThis.point3D.x, nextPointThis.point3D.y };
							if (curPointThis_XY == nextPointThis_XY) {
								popIndex = indexThis;
								continue;
							}
							else
							{
								if (curPointNeighbor_XY.orientation(curPointThis_XY, nextPointThis_XY) == 0
									&& curPointNeighbor_XY.WithinSegmentBounds(curPointThis_XY, nextPointThis_XY))
								{
									if (curPointNeighbor_XY != curPointThis_XY && curPointNeighbor_XY != nextPointThis_XY)
									{
										//if so, we want to add the point to the array in between these 2 points;
										curPointNeighbor.point3D.z = curPointThis.point3D.z;
										thisPlane.insertPoint(nextIndexThis, curPointNeighbor);
										break;
									}

								}
							}

						}
						// pop duplicate points
						if (popIndex >= 0)
						{
							thisPlane.PopPoint(popIndex);
						}
					}
				}
			}

		}
	}

	//m_floorsNCeilingsAmount += 2;
}