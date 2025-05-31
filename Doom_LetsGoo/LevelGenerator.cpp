#include "pch.h"
#include <iostream>
#include "Things.h"
#include "Enemy.h"
#include "Interactor.h"
#include "PowerUp.h"
#include "Plane.h"
#include "Door.h"
#include "PlaneIregular.h"
#include "LevelGenerator.h"
#include "Texture.h"
#include "PLayer.h"
//#include "LevelGenerator.h"

/*
* so, we want to generate the level.
we're gonna load the lmp's and then do shit

from the level, we want to receive the scale

the level just wants the bsp tree, the walls, the floors. and that's it basicaly
*/


LevelGenerator::LevelGenerator(std::vector<BSPnode>& p_BSPtree_p, std::vector<BSPleafeNode>& p_BSPleafs_p,
	std::vector <Plane*>& p_Planes_p, std::vector <PlaneIregular>& p_FloorsNCeilings_p, std::set<int>& tags1, std::set<int>& tags2,
	std::vector <Thing*>& things, std::vector<Texture>& m_Textures, std::map<std::string, int>& map,
	Player& player)
	:vertexesImported{ utils::ReadLMP("VERTEXES.lmp", VERTEXCOLUMS) },
	linesImported{ utils::ReadLMP("LINEDEFS.lmp", LINECOLUMS) },
	sidesImported{ utils::ReadLMP("SIDEDEFS.lmp", SIDECOLUMS) },
	sectorsImported{ utils::ReadLMP("SECTORS.lmp", SECTORCOLUMS) },
	nodesImported{ utils::ReadLMP("NODES.lmp", NODESCULOMS) },
	segsImported{ utils::ReadLMP("SEGS.lmp", SEGSCOLUMS) },
	sectorSegsImported{ utils::ReadLMP("SSECTORS.lmp", SECTORSEGSCOLUMS) },
	thingsImported{ utils::ReadLMP("THINGS.lmp", THINGSSCOLUMS) },
	r_BSPtree{ p_BSPtree_p },
	r_BSPleafs{ p_BSPleafs_p },
	r_Planes{ p_Planes_p },
	r_FloorsNCeilings{ p_FloorsNCeilings_p },
	r_tags{ tags1 , tags2 },
	r_Things{things},
	textures{ m_Textures },
	nameMap{ map },
	m_pPlayer{ player }
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
void LevelGenerator::AddPlane(int line, float floorHeight, float ceilingHeight, const float SCALE, int idx)
{

	bool isExit{ false };
	
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
		r_tags[0].insert(int(r_Planes.size()));
	}
	if (linesImported.data[line][ReadLines::sectorTag] == 2)
	{
		r_tags[1].insert(int(r_Planes.size()));
	}
	if (WillBeDoor(linesImported.data[line][ReadLines::actionSpecial]))
	{
		if (linesImported.data[line][ReadLines::actionSpecial] == 11)
		{
			isExit = true;
		}
		r_Planes.push_back(new Door{ location, width, height, idx, ceilingHeight, floorHeight, isExit });
	}
	else
	{
		r_Planes.push_back(new Plane{ location, width, height, idx });
	}
	r_Planes.back()->SetActionBools(linesImported.data[line][ReadLines::actionSpecial]);
}

int LevelGenerator::GetSectorFromLine(int line, bool backSide) const
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
		int nextPoint = utils::getArrIndex(int(boundingBoxOriginal.size()), point + 1);
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
			
			int lineI = segsImported.data[seg][3]; // it just gets the first line. we need the rest too

			int orienttion = segsImported.data[seg][4]; //these are curently magic numbers. they are derived from the file structure, im thinking of putting them in an enumerator
			int sideIndex{ ReadLines::frontSide };
			if (orienttion > 0)
			{ // 0 : front side | 1 : backside
				sideIndex = ReadLines::backSide;
			}
			int side = linesImported.data[lineI][sideIndex];
			int sector = sidesImported.data[side][ReadSides::sector];

			std::string ceilingTex = utils::convertToCharString(&sectorsImported.data[sector][ReadSectors::ceilingTexture]); //&sidesImported.data[sideDef][ReadSides::upperTexture]);
			std::string floorTex = utils::convertToCharString(&sectorsImported.data[sector][ReadSectors::floorTexture]);
			
			int ceilIdx = nameMap.find(ceilingTex)->second;
			int floorIdx = nameMap.find(floorTex)->second;


			if (sectorsImported.data[sector][ReadSectors::tag] != 0)
			{
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
			PlaneIregular input{ boundingBoxChild, ceilIdx, int(ceilingHeight) };
			PlaneIregular input2{ boundingBoxChild, floorIdx, int(floorHeight) };
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
				int idx = nameMap.find(upperTexture)->second;
				linePerWall.push_back(line);
				AddPlane(line, ceilingHeight2, ceilingHeight, SCALE, idx);
				//g_Sectors[wallamount] = sector;
			}
			if (middleTexture.find("-") == std::string::npos)
			{
				int idx = nameMap.find(middleTexture)->second;
				float floorHeight2Use{ floorHeight };
				if (floorHeight < floorHeight2) {
					floorHeight2Use = floorHeight2;
				}
				float ceilingHeight2Use{ ceilingHeight };
				if (ceilingHeight > ceilingHeight2) {
					ceilingHeight2Use = ceilingHeight2;
				}
				linePerWall.push_back(line);
				AddPlane(line, floorHeight2Use, ceilingHeight2Use, SCALE, idx);
			}
			if (lowerTexture.find("-") == std::string::npos)
			{
				int idx = nameMap.find(lowerTexture)->second;
				linePerWall.push_back(line);
				AddPlane(line, floorHeight2, floorHeight, SCALE, idx);
			}
		}
		else if (middleTexture.find("-") == std::string::npos)
		{
			int idx = nameMap.find(middleTexture)->second;
			linePerWall.push_back(line);
			AddPlane(line, floorHeight, ceilingHeight, SCALE, idx);
		}
		// bottom

	}
}

void LevelGenerator::GenerateLevel()
{
	GetTextureStructs();
	mapNames();
	AddFnCTextures();

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
	int top = nodesImported.data[nodesImported.rows - 1][8] + buffer;
	int bottom = nodesImported.data[nodesImported.rows - 1][5] - buffer;
	int left = nodesImported.data[nodesImported.rows - 1][6] - buffer;
	int right = nodesImported.data[nodesImported.rows - 1][11] + buffer;

	std::vector<Vector2f> startingBox{ Vector2f{left, top},Vector2f{right, top},Vector2f{right, bottom },Vector2f{left, bottom} };
	std::vector<Line> sliceLines;
	int* sectorPerSubsector{ new int[sectorSegsImported.rows * 2] };
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

	ReadThings();
	AdjustThings();
	//std::vector<PatchData> patches{};
	//utils::ReadTexLMP("TEXTURE1.lmp", patches);
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

void LevelGenerator::ReadThings()
{
	r_Things.reserve(thingsImported.rows);
	for (int thing{}; thing < thingsImported.rows; thing++)
	{
		/*if (thingsImported.data[thing][ReadThings::type] == 5 ||
			thingsImported.data[thing][ReadThings::type] == 40 ||
			thingsImported.data[thing][ReadThings::type] == 13 ||
			thingsImported.data[thing][ReadThings::type] == 38 ||
			thingsImported.data[thing][ReadThings::type] == 6 ||
			thingsImported.data[thing][ReadThings::type] == 39)
		{
		}*/
		Vector2f position{ float(thingsImported.data[thing][ReadThings::xPos]),
							float(thingsImported.data[thing][ReadThings::yPos]) };
		utils::ThingsEnum thingType{ utils::ThingsEnum(thingsImported.data[thing][ReadThings::type]) };

		bool isSet{ false };
		std::string sprite{};
		std::string sequence{};
		std::string properties{};
		utils::ThingType type{};

		int radius{1};
		int height{1};
		int hp{1};
		float speedScale{1.};
		float speed{1};

		

		/*sprite = "";
		sequence = "";
		properties = "";
		type = utils::ThingType::ammunition;*/

		switch (thingType)
		{
		case utils::ThingsEnum::playerStart:
			//break;
		case utils::ThingsEnum::player2Start:
			//break;
		case utils::ThingsEnum::player3Start:
			//break;
		case utils::ThingsEnum::player4Start:
			//break;
		case utils::ThingsEnum::DeathmatchStart:
			type = utils::ThingType::other;
			continue;
			break;
		}
		//will need a position
		//will need a texture
		//will need collision
		//will need size

		switch (thingType)
		{
		case utils::ThingsEnum::ShotgunGuy:
			// we wanna set it's health
			//it's damage
			//it's movement speed
			radius = 20 ;
			height = 56;
			hp = 30;
			speed = 8 / speedScale;

			sprite = "SPOS";
			sequence = "AB+";
			properties = "MO*";
			type = utils::ThingType::monster;
			break;
		case utils::ThingsEnum::Imp:
			radius = 20;
			height = 56;
			hp = 60;
			speed = 8 / speedScale;

			sprite = "TROO";
			sequence = "AB+";
			properties = "MO*";
			type = utils::ThingType::monster;
			break;
		case utils::ThingsEnum::Zombieman:
			radius = 20;
			height = 56;
			hp = 20;
			speed = 8 / speedScale;

			sprite = "POSS";
			sequence = "AB+";
			properties = "MO*";
			type = utils::ThingType::monster;
			break;
			//--------------------------------------------- MONSTER
		case utils::ThingsEnum::BloodyMess:
			radius = 20;
			height = 16;
			sprite = "PLAY";
			sequence = "W";
			type = utils::ThingType::deco;
			break;
		case utils::ThingsEnum::BloodyMess2:
			radius = 20;
			height = 16;
			sprite = "PLAY";
			sequence = "W";
			type = utils::ThingType::deco;
			break;
		case utils::ThingsEnum::DeadPlayer:
			radius = 20;
			height = 16;
			sprite = "PLAY";
			sequence = "N";
			type = utils::ThingType::deco;
			break;
		case utils::ThingsEnum::PoolOfBloodAndFlesh:
			radius = 20;
			height = 16;
			sprite = "POL5";
			sequence = "A";
			type = utils::ThingType::deco;
			break;
			//--------------------------------------------- DECORATION
		case utils::ThingsEnum::Candelabra:
			radius = 16;
			height = 16;
			sprite = "CLBR";
			sequence = "A";
			properties = "O";
			type = utils::ThingType::obstacle;
			break;
		case utils::ThingsEnum::TallTechnoColumn:
			radius = 16;
			height = 16;
			sprite = "ELEC";
			sequence = "A";
			properties = "O";
			type = utils::ThingType::obstacle;
			break;
		case utils::ThingsEnum::FloorLamp:
			radius = 16;
			height = 16;
			sprite = "COLU";
			sequence = "A";
			properties = "O";
			type = utils::ThingType::obstacle;
			break;
		case utils::ThingsEnum::ExplodingBarrel: // THIS ONE EXPLODES
			radius = 10;
			height = 42;
			sprite = "BAR1";
			sequence = "AB";
			properties = "O*";
			type = utils::ThingType::obstacle;
			break;
			//--------------------------------------------- WEAPONS
		case utils::ThingsEnum::Shotgun:
			radius = 20;
			height = 16;
			sprite = "SHOT";
			sequence = "A";
			properties = "WP";
			type = utils::ThingType::weapon;
			break;
		case utils::ThingsEnum::Chaingun:
			radius = 20;
			height = 16;
			sprite = "MGUN";
			sequence = "A";
			properties = "WP";
			type = utils::ThingType::weapon;
			break;
		case utils::ThingsEnum::RocketLauncher:
			radius = 20;
			height = 16;
			sprite = "LAUN";
			sequence = "A";
			properties = "WP";
			type = utils::ThingType::weapon;
			break;
			//--------------------------------------------- PICKUP
		case utils::ThingsEnum::Clip:
			radius = 20;
			height = 16;
			sprite = "CLIP";
			sequence = "A";
			properties = "P";
			type = utils::ThingType::ammunition;
			break;
		case utils::ThingsEnum::shotgunShells4:
			radius = 20;
			height = 16;
			sprite = "SHEL";
			sequence = "A";
			properties = "P";
			type = utils::ThingType::ammunition;
			break;
		case utils::ThingsEnum::BoxOfRockets:
			radius = 20;
			height = 16;
			sprite = "BROK";
			sequence = "A";
			properties = "P";
			type = utils::ThingType::ammunition;
			break;
		case utils::ThingsEnum::BoxOfBullets:
			radius = 20;
			height = 16;
			sprite = "AMMO";
			sequence = "A";
			properties = "P";
			type = utils::ThingType::ammunition;
			break;
		case utils::ThingsEnum::BoxOfShotgunShells:
			radius = 20;
			height = 16;
			sprite = "SBOX";
			sequence = "A";
			properties = "P";
			type = utils::ThingType::ammunition;
			break;
			//---------------------------------------------
		case utils::ThingsEnum::Medikit:
			radius = 20;
			height = 16;
			sprite = "MEDI";
			sequence = "A";
			properties = "P";
			type = utils::ThingType::powerUp;
			break;
		case utils::ThingsEnum::Armor:
			radius = 20;
			height = 16;
			sprite = "ARM1";
			sequence = "AB";
			properties = "P";
			type = utils::ThingType::powerUp;
			break;
		case utils::ThingsEnum::Megaarmor:
			radius = 20;
			height = 16;
			sprite = "ARM2";
			sequence = "AB";
			properties = "P";
			type = utils::ThingType::powerUp;
			break;
			//---------------------------------------------
		case utils::ThingsEnum::HealthBonus:
			radius = 20;
			height = 16;
			sprite = "BON1";
			sequence = "ABCDCB";
			properties = "AP";
			type = utils::ThingType::artifact;
			break;
		case utils::ThingsEnum::ArmorBonus:
			radius = 20;
			height = 16;
			sprite = "BON2";
			sequence = "ABCDCB";
			properties = "AP";
			type = utils::ThingType::artifact;
			break;
		}
		//utils::ThingType::ammunition
		//utils::ThingType::artifact
		//utils::ThingType::powerUp
		switch (type)
		{
		case utils::ThingType::ammunition:
			
		case utils::ThingType::artifact:
			
		case utils::ThingType::powerUp:
			r_Things.emplace_back(new PowerUp{ position, float(height), thingType, sprite });
			break;
		case utils::ThingType::monster:
			r_Things.emplace_back(new Enemy{ position, float(height), sprite });
			break;
		default:
			r_Things.emplace_back(new Thing{ position, float(height) , sprite });
			break;
		}
	}
	Enemy::AssignPlayer(m_pPlayer.m_HealthManager);
	Interactor::AssignPlayer(m_pPlayer);
}

void LevelGenerator::AdjustThings()
{
	for (int thingIndex{}; thingIndex < r_Things.size(); thingIndex++)
	{
		Vector2f positionThing{ r_Things[thingIndex]->GetStartPos()};
		int leafIndex{ utils::GetBSP(positionThing, true, r_BSPtree) };
		r_BSPleafs[leafIndex].thingIndexes.push_back(thingIndex);
		float zPos = r_BSPleafs[leafIndex].floorheight;
		r_Things[thingIndex]->SetPosHeight( zPos );
		r_Things[thingIndex]->SetSubsectorIndex( leafIndex);
	}
}

void LevelGenerator::mapNames() {
	
	//std::string name{ utils::convertToCharString(texNamesImported.data[ReadTexNames::nameIndex]) };
	for (int texID{}; texID < walltextures.size(); texID ++)
	{

		nameMap.insert(std::pair<std::string, int >(walltextures[texID].name, texID));
		textures.emplace_back(walltextures[texID]);

	}
	//givenTexPointer = new Texture( walltextures[1] );
	//Texture test{ walltextures[2] };
}

void LevelGenerator::GetTextureStructs() {
	

	walltextures.reserve(300);

	walltextures.emplace_back("AASTINKY", 24, 72);

	walltextures.back().patches.emplace_back("WALL00_3", 0, 0);
	walltextures.back().patches.emplace_back("WALL00_3", 12, -6);


	walltextures.emplace_back("BIGDOOR1", 128, 96);

	walltextures.back().patches.emplace_back("W13_1", 0, 0);
	walltextures.back().patches.emplace_back("W13_1", 0, 24);
	walltextures.back().patches.emplace_back("DOOR2_1", 17, 0);
	walltextures.back().patches.emplace_back("W13_1", 113, 0);
	walltextures.back().patches.emplace_back("W13_1", 113, 25);


	walltextures.emplace_back("BIGDOOR2", 128, 128);

	walltextures.back().patches.emplace_back("DOOR2_4", 0, 0);


	walltextures.emplace_back("BIGDOOR4", 128, 128);

	walltextures.back().patches.emplace_back("DOOR9_1", 0, 0);


	walltextures.emplace_back("BRNBIGC", 128, 128);

	walltextures.back().patches.emplace_back("W113_1", 0, 0);


	walltextures.emplace_back("BRNBIGL", 32, 128);

	walltextures.back().patches.emplace_back("W113_2", 0, 0);


	walltextures.emplace_back("BRNBIGR", 32, 128);

	walltextures.back().patches.emplace_back("W113_3", 0, 0);


	walltextures.emplace_back("BRNPOIS", 64, 128);

	walltextures.back().patches.emplace_back("WALL62_2", 0, 0);
	walltextures.back().patches.emplace_back("PS20A0", 1, 60);


	walltextures.emplace_back("BRNPOIS2", 64, 128);

	walltextures.back().patches.emplace_back("WALL62_1", 0, 0);
	walltextures.back().patches.emplace_back("PS20A0", 0, 57);


	walltextures.emplace_back("BRNSMAL1", 64, 64);

	walltextures.back().patches.emplace_back("W111_2", 0, 0);


	walltextures.emplace_back("BRNSMAL2", 64, 64);

	walltextures.back().patches.emplace_back("W111_3", 0, 0);


	walltextures.emplace_back("BRNSMALC", 64, 64);

	walltextures.back().patches.emplace_back("W112_1", 0, 0);


	walltextures.emplace_back("BRNSMALL", 32, 64);

	walltextures.back().patches.emplace_back("W112_3", 0, 0);


	walltextures.emplace_back("BRNSMALR", 32, 64);

	walltextures.back().patches.emplace_back("W112_2", 0, 0);


	walltextures.emplace_back("BROWN1", 128, 128);

	walltextures.back().patches.emplace_back("WALL02_2", 0, 56);
	walltextures.back().patches.emplace_back("WALL02_2", 0, 0);
	walltextures.back().patches.emplace_back("WALL02_3", 64, 56);
	walltextures.back().patches.emplace_back("WALL02_1", 88, 56);
	walltextures.back().patches.emplace_back("WALL02_3", 64, 0);
	walltextures.back().patches.emplace_back("WALL02_1", 88, 0);


	walltextures.emplace_back("BROWN144", 128, 128);

	walltextures.back().patches.emplace_back("WALL00_5", 0, 0);
	walltextures.back().patches.emplace_back("WALL00_6", 16, 0);
	walltextures.back().patches.emplace_back("WALL00_7", 32, 0);
	walltextures.back().patches.emplace_back("WALL00_8", 48, 0);
	walltextures.back().patches.emplace_back("WALL00_6", 64, 0);
	walltextures.back().patches.emplace_back("WALL00_6", 112, -16);
	walltextures.back().patches.emplace_back("WALL00_7", 96, -1);
	walltextures.back().patches.emplace_back("WALL00_6", 80, 0);


	walltextures.emplace_back("BROWN96", 128, 128);

	walltextures.back().patches.emplace_back("WALL62_1", 0, 0);


	walltextures.emplace_back("BROWNGRN", 64, 128);

	walltextures.back().patches.emplace_back("WALL62_2", 0, 0);


	walltextures.emplace_back("BROWNHUG", 64, 128);

	walltextures.back().patches.emplace_back("WALL03_4", 0, 0);
	walltextures.back().patches.emplace_back("WALL03_4", 0, 72);


	walltextures.emplace_back("BROWNPIP", 128, 128);

	walltextures.back().patches.emplace_back("TP2_1", 0, 0);
	walltextures.back().patches.emplace_back("STEP07", 0, 64);
	walltextures.back().patches.emplace_back("STEP07", 32, 64);
	walltextures.back().patches.emplace_back("STEP07", 64, 64);
	walltextures.back().patches.emplace_back("STEP07", 96, 64);
	walltextures.back().patches.emplace_back("WALL05_2", 0, 72);
	walltextures.back().patches.emplace_back("WALL05_2", 64, 72);
	walltextures.back().patches.emplace_back("STEP07", 0, 120);
	walltextures.back().patches.emplace_back("STEP07", 32, 120);
	walltextures.back().patches.emplace_back("STEP07", 64, 120);
	walltextures.back().patches.emplace_back("STEP07", 96, 120);


	walltextures.emplace_back("COMP2", 256, 128);

	walltextures.back().patches.emplace_back("COMP02_1", 0, 0);
	walltextures.back().patches.emplace_back("COMP02_2", 64, 0);
	walltextures.back().patches.emplace_back("COMP02_4", 128, 0);
	walltextures.back().patches.emplace_back("COMP02_3", 192, 0);
	walltextures.back().patches.emplace_back("COMP02_6", 0, 64);
	walltextures.back().patches.emplace_back("COMP02_8", 64, 64);
	walltextures.back().patches.emplace_back("COMP02_7", 128, 64);
	walltextures.back().patches.emplace_back("COMP02_5", 192, 64);
	walltextures.back().patches.emplace_back("STEP08", 0, 56);
	walltextures.back().patches.emplace_back("STEP08", 32, 56);
	walltextures.back().patches.emplace_back("STEP08", 64, 56);
	walltextures.back().patches.emplace_back("STEP08", 96, 56);
	walltextures.back().patches.emplace_back("STEP08", 128, 56);
	walltextures.back().patches.emplace_back("STEP08", 160, 56);
	walltextures.back().patches.emplace_back("STEP08", 192, 56);
	walltextures.back().patches.emplace_back("STEP08", 224, 56);
	walltextures.back().patches.emplace_back("STEP08", 0, 120);
	walltextures.back().patches.emplace_back("STEP08", 32, 120);
	walltextures.back().patches.emplace_back("STEP08", 64, 120);
	walltextures.back().patches.emplace_back("STEP08", 96, 120);
	walltextures.back().patches.emplace_back("STEP08", 128, 120);
	walltextures.back().patches.emplace_back("STEP08", 160, 120);
	walltextures.back().patches.emplace_back("STEP08", 192, 120);
	walltextures.back().patches.emplace_back("STEP08", 224, 120);


	walltextures.emplace_back("COMPSPAN", 32, 128);

	walltextures.back().patches.emplace_back("COMP03_4", 0, 0);
	walltextures.back().patches.emplace_back("COMP03_4", 0, 64);


	walltextures.emplace_back("COMPSTA1", 128, 128);

	walltextures.back().patches.emplace_back("TOMW2_1", 0, 0);
	walltextures.back().patches.emplace_back("AG128_2", 0, 72);
	walltextures.back().patches.emplace_back("AG128_2", 64, 72);


	walltextures.emplace_back("COMPSTA2", 128, 128);

	walltextures.back().patches.emplace_back("TOMW2_2", 0, 0);
	walltextures.back().patches.emplace_back("AG128_2", 0, 72);
	walltextures.back().patches.emplace_back("AG128_2", 64, 72);


	walltextures.emplace_back("COMPTALL", 256, 128);

	walltextures.back().patches.emplace_back("COMP04_5", 32, 0);
	walltextures.back().patches.emplace_back("COMP04_8", 96, 0);
	walltextures.back().patches.emplace_back("COMP03_8", 160, 0);
	walltextures.back().patches.emplace_back("COMP04_6", 0, 64);
	walltextures.back().patches.emplace_back("COMP04_7", 64, 64);
	walltextures.back().patches.emplace_back("COMP04_2", 128, 64);
	walltextures.back().patches.emplace_back("COMP03_5", 192, 0);
	walltextures.back().patches.emplace_back("COMP04_1", 192, 64);
	walltextures.back().patches.emplace_back("COMP03_8", 0, 0);


	walltextures.emplace_back("COMPTILE", 128, 128);

	walltextures.back().patches.emplace_back("COMP03_7", 0, 0);
	walltextures.back().patches.emplace_back("COMP03_1", 32, 0);
	walltextures.back().patches.emplace_back("COMP03_6", 96, 0);
	walltextures.back().patches.emplace_back("COMP03_7", 0, 64);
	walltextures.back().patches.emplace_back("COMP03_1", 32, 64);
	walltextures.back().patches.emplace_back("COMP03_6", 96, 64);


	walltextures.emplace_back("COMPUTE1", 128, 128);

	walltextures.back().patches.emplace_back("COMP01_5", 26, 64);
	walltextures.back().patches.emplace_back("COMP01_1", 0, 64);
	walltextures.back().patches.emplace_back("W33_8", 0, 56);
	walltextures.back().patches.emplace_back("W33_8", 64, 56);
	walltextures.back().patches.emplace_back("W33_8", 0, 120);
	walltextures.back().patches.emplace_back("W33_8", 64, 120);
	walltextures.back().patches.emplace_back("COMP01_5", 28, 0);
	walltextures.back().patches.emplace_back("COMP1C_6", 72, 64);
	walltextures.back().patches.emplace_back("COMP01_6", 72, 0);
	walltextures.back().patches.emplace_back("COMP1B_4", 0, 0);


	walltextures.emplace_back("COMPUTE2", 256, 56);

	walltextures.back().patches.emplace_back("COMP02_1", 0, 0);
	walltextures.back().patches.emplace_back("COMP02_2", 64, 0);
	walltextures.back().patches.emplace_back("COMP02_3", 128, 0);
	walltextures.back().patches.emplace_back("COMP02_7", 192, 0);


	walltextures.emplace_back("COMPUTE3", 128, 128);

	walltextures.back().patches.emplace_back("SW11_4", 0, 4);
	walltextures.back().patches.emplace_back("SW11_5", 32, 4);
	walltextures.back().patches.emplace_back("SW11_4", 64, 4);
	walltextures.back().patches.emplace_back("COMP02_1", 0, 0);
	walltextures.back().patches.emplace_back("SW11_5", 96, 4);
	walltextures.back().patches.emplace_back("COMP02_2", 64, 0);
	walltextures.back().patches.emplace_back("STEP08", 0, 56);
	walltextures.back().patches.emplace_back("STEP08", 32, 56);
	walltextures.back().patches.emplace_back("STEP08", 64, 56);
	walltextures.back().patches.emplace_back("STEP08", 96, 56);


	walltextures.emplace_back("DOOR1", 64, 72);

	walltextures.back().patches.emplace_back("WALL03_1", 0, 0);


	walltextures.emplace_back("DOOR3", 64, 72);

	walltextures.back().patches.emplace_back("DOOR2_5", 0, 0);


	walltextures.emplace_back("DOORBLU", 8, 128);

	walltextures.back().patches.emplace_back("W46_37", 0, 112);
	walltextures.back().patches.emplace_back("W46_37", 0, 0);
	walltextures.back().patches.emplace_back("W46_37", 0, 96);
	walltextures.back().patches.emplace_back("W46_37", 0, 80);
	walltextures.back().patches.emplace_back("W46_37", 0, 16);
	walltextures.back().patches.emplace_back("W46_37", 0, 32);
	walltextures.back().patches.emplace_back("W46_37", 0, 48);
	walltextures.back().patches.emplace_back("W46_37", 0, 64);


	walltextures.emplace_back("DOORRED", 8, 128);

	walltextures.back().patches.emplace_back("W46_38", 0, 0);
	walltextures.back().patches.emplace_back("W46_38", 0, 16);
	walltextures.back().patches.emplace_back("W46_38", 0, 32);
	walltextures.back().patches.emplace_back("W46_38", 0, 48);
	walltextures.back().patches.emplace_back("W46_38", 0, 64);
	walltextures.back().patches.emplace_back("W46_38", 0, 80);
	walltextures.back().patches.emplace_back("W46_38", 0, 96);
	walltextures.back().patches.emplace_back("W46_38", 0, 112);


	walltextures.emplace_back("DOORSTOP", 8, 128);

	walltextures.back().patches.emplace_back("TTALL1_2", 0, 0);


	walltextures.emplace_back("DOORTRAK", 8, 128);

	walltextures.back().patches.emplace_back("DOORTRAK", 0, 0);


	walltextures.emplace_back("DOORYEL", 8, 128);

	walltextures.back().patches.emplace_back("W46_39", 0, 112);
	walltextures.back().patches.emplace_back("W46_39", 0, 80);
	walltextures.back().patches.emplace_back("W46_39", 0, 64);
	walltextures.back().patches.emplace_back("W46_39", 0, 0);
	walltextures.back().patches.emplace_back("W46_39", 0, 16);
	walltextures.back().patches.emplace_back("W46_39", 0, 48);
	walltextures.back().patches.emplace_back("W46_39", 0, 32);
	walltextures.back().patches.emplace_back("W46_39", 0, 96);


	walltextures.emplace_back("EXITDOOR", 128, 72);

	walltextures.back().patches.emplace_back("DOOR3_6", 0, 0);
	walltextures.back().patches.emplace_back("DOOR3_4", 64, 0);
	walltextures.back().patches.emplace_back("DOOR3_5", 88, 0);
	walltextures.back().patches.emplace_back("T14_5", 112, 0);


	walltextures.emplace_back("EXITSIGN", 64, 16);

	walltextures.back().patches.emplace_back("EXIT1", 0, 0);
	walltextures.back().patches.emplace_back("EXIT2", 32, 0);
	walltextures.back().patches.emplace_back("EXIT2", 40, 0);
	walltextures.back().patches.emplace_back("EXIT2", 48, 0);
	walltextures.back().patches.emplace_back("EXIT2", 56, 0);


	walltextures.emplace_back("GRAY4", 64, 128);

	walltextures.back().patches.emplace_back("W33_5", 0, 0);
	walltextures.back().patches.emplace_back("W33_5", 0, 64);


	walltextures.emplace_back("GRAY5", 64, 128);

	walltextures.back().patches.emplace_back("W33_7", 0, 72);
	walltextures.back().patches.emplace_back("W33_7", 0, 0);
	walltextures.back().patches.emplace_back("W33_8", 0, 64);


	walltextures.emplace_back("GRAY7", 256, 128);

	walltextures.back().patches.emplace_back("W32_4", 0, 0);
	walltextures.back().patches.emplace_back("W32_1", 64, 0);
	walltextures.back().patches.emplace_back("W32_4", 128, 0);
	walltextures.back().patches.emplace_back("W33_8", 0, 120);
	walltextures.back().patches.emplace_back("W33_8", 64, 120);
	walltextures.back().patches.emplace_back("W33_8", 128, 120);
	walltextures.back().patches.emplace_back("W32_4", 192, 0);
	walltextures.back().patches.emplace_back("W33_8", 192, 120);
	walltextures.back().patches.emplace_back("W32_4", 64, 56);
	walltextures.back().patches.emplace_back("W32_1", 128, 56);
	walltextures.back().patches.emplace_back("W32_4", 192, 56);
	walltextures.back().patches.emplace_back("W32_4", 0, 56);


	walltextures.emplace_back("GRAYTALL", 128, 128);

	walltextures.back().patches.emplace_back("WALL00_1", 16, 0);
	walltextures.back().patches.emplace_back("WALL00_3", 80, 0);
	walltextures.back().patches.emplace_back("WALL00_2", 0, 0);
	walltextures.back().patches.emplace_back("WALL00_1", 96, 0);


	walltextures.emplace_back("LITE2", 64, 128);

	walltextures.back().patches.emplace_back("WALL02_2", 0, 56);
	walltextures.back().patches.emplace_back("WALL02_2", 0, 0);
	walltextures.back().patches.emplace_back("FLAMP", 5, 0);
	walltextures.back().patches.emplace_back("FLAMP", 35, 0);


	walltextures.emplace_back("LITE3", 32, 128);

	walltextures.back().patches.emplace_back("WLITA0", 0, 0);
	walltextures.back().patches.emplace_back("WLITB0", 8, 0);
	walltextures.back().patches.emplace_back("WLITB0", 16, 0);
	walltextures.back().patches.emplace_back("WLITC0", 24, 0);
	walltextures.back().patches.emplace_back("WLITA0", 0, 8);
	walltextures.back().patches.emplace_back("WLITB0", 8, 8);
	walltextures.back().patches.emplace_back("WLITB0", 16, 8);
	walltextures.back().patches.emplace_back("WLITC0", 24, 8);
	walltextures.back().patches.emplace_back("WLITA0", 0, 16);
	walltextures.back().patches.emplace_back("WLITB0", 8, 16);
	walltextures.back().patches.emplace_back("WLITB0", 16, 16);
	walltextures.back().patches.emplace_back("WLITC0", 24, 16);
	walltextures.back().patches.emplace_back("WLITA0", 0, 24);
	walltextures.back().patches.emplace_back("WLITB0", 8, 24);
	walltextures.back().patches.emplace_back("WLITB0", 16, 24);
	walltextures.back().patches.emplace_back("WLITC0", 24, 24);
	walltextures.back().patches.emplace_back("WLITA0", 0, 32);
	walltextures.back().patches.emplace_back("WLITB0", 8, 32);
	walltextures.back().patches.emplace_back("WLITB0", 16, 32);
	walltextures.back().patches.emplace_back("WLITC0", 24, 32);
	walltextures.back().patches.emplace_back("WLITA0", 0, 40);
	walltextures.back().patches.emplace_back("WLITB0", 8, 40);
	walltextures.back().patches.emplace_back("WLITB0", 16, 40);
	walltextures.back().patches.emplace_back("WLITC0", 24, 40);
	walltextures.back().patches.emplace_back("WLITA0", 0, 48);
	walltextures.back().patches.emplace_back("WLITB0", 8, 48);
	walltextures.back().patches.emplace_back("WLITB0", 16, 48);
	walltextures.back().patches.emplace_back("WLITC0", 24, 48);
	walltextures.back().patches.emplace_back("WLITA0", 0, 56);
	walltextures.back().patches.emplace_back("WLITB0", 8, 56);
	walltextures.back().patches.emplace_back("WLITB0", 16, 56);
	walltextures.back().patches.emplace_back("WLITC0", 24, 56);
	walltextures.back().patches.emplace_back("WLITA0", 0, 64);
	walltextures.back().patches.emplace_back("WLITB0", 8, 64);
	walltextures.back().patches.emplace_back("WLITB0", 16, 64);
	walltextures.back().patches.emplace_back("WLITC0", 24, 64);
	walltextures.back().patches.emplace_back("WLITA0", 0, 72);
	walltextures.back().patches.emplace_back("WLITB0", 8, 72);
	walltextures.back().patches.emplace_back("WLITB0", 16, 72);
	walltextures.back().patches.emplace_back("WLITC0", 24, 72);
	walltextures.back().patches.emplace_back("WLITA0", 0, 80);
	walltextures.back().patches.emplace_back("WLITB0", 8, 80);
	walltextures.back().patches.emplace_back("WLITB0", 16, 80);
	walltextures.back().patches.emplace_back("WLITC0", 24, 80);
	walltextures.back().patches.emplace_back("WLITA0", 0, 88);
	walltextures.back().patches.emplace_back("WLITB0", 8, 88);
	walltextures.back().patches.emplace_back("WLITB0", 16, 88);
	walltextures.back().patches.emplace_back("WLITC0", 24, 88);
	walltextures.back().patches.emplace_back("WLITA0", 0, 96);
	walltextures.back().patches.emplace_back("WLITB0", 8, 96);
	walltextures.back().patches.emplace_back("WLITB0", 16, 96);
	walltextures.back().patches.emplace_back("WLITC0", 24, 96);
	walltextures.back().patches.emplace_back("WLITA0", 0, 104);
	walltextures.back().patches.emplace_back("WLITB0", 8, 104);
	walltextures.back().patches.emplace_back("WLITB0", 16, 104);
	walltextures.back().patches.emplace_back("WLITC0", 24, 104);
	walltextures.back().patches.emplace_back("WLITA0", 0, 112);
	walltextures.back().patches.emplace_back("WLITB0", 8, 112);
	walltextures.back().patches.emplace_back("WLITB0", 16, 112);
	walltextures.back().patches.emplace_back("WLITC0", 24, 112);
	walltextures.back().patches.emplace_back("WLITA0", 0, 120);
	walltextures.back().patches.emplace_back("WLITB0", 8, 120);
	walltextures.back().patches.emplace_back("WLITB0", 16, 120);
	walltextures.back().patches.emplace_back("WLITC0", 24, 120);


	walltextures.emplace_back("LITE4", 16, 128);

	walltextures.back().patches.emplace_back("T14_5", 0, 56);
	walltextures.back().patches.emplace_back("WLITA0", 0, 0);
	walltextures.back().patches.emplace_back("WLITC0", 8, 0);
	walltextures.back().patches.emplace_back("WLITA0", 0, 48);
	walltextures.back().patches.emplace_back("WLITC0", 8, 48);
	walltextures.back().patches.emplace_back("WLITA0", 0, 40);
	walltextures.back().patches.emplace_back("WLITC0", 8, 40);
	walltextures.back().patches.emplace_back("WLITA0", 0, 32);
	walltextures.back().patches.emplace_back("WLITC0", 8, 32);
	walltextures.back().patches.emplace_back("WLITA0", 0, 24);
	walltextures.back().patches.emplace_back("WLITC0", 8, 24);
	walltextures.back().patches.emplace_back("WLITA0", 0, 16);
	walltextures.back().patches.emplace_back("WLITC0", 8, 16);
	walltextures.back().patches.emplace_back("WLITA0", 0, 8);
	walltextures.back().patches.emplace_back("WLITC0", 8, 8);


	walltextures.emplace_back("LITE5", 16, 128);

	walltextures.back().patches.emplace_back("WLITA0", 0, 0);
	walltextures.back().patches.emplace_back("WLITC0", 8, 0);
	walltextures.back().patches.emplace_back("WLITA0", 0, 8);
	walltextures.back().patches.emplace_back("WLITC0", 8, 8);
	walltextures.back().patches.emplace_back("WLITA0", 0, 16);
	walltextures.back().patches.emplace_back("WLITC0", 8, 16);
	walltextures.back().patches.emplace_back("WLITA0", 0, 24);
	walltextures.back().patches.emplace_back("WLITC0", 8, 24);
	walltextures.back().patches.emplace_back("WLITA0", 0, 32);
	walltextures.back().patches.emplace_back("WLITC0", 8, 32);
	walltextures.back().patches.emplace_back("WLITA0", 0, 40);
	walltextures.back().patches.emplace_back("WLITC0", 8, 40);
	walltextures.back().patches.emplace_back("WLITA0", 0, 48);
	walltextures.back().patches.emplace_back("WLITC0", 8, 48);
	walltextures.back().patches.emplace_back("WLITA0", 0, 56);
	walltextures.back().patches.emplace_back("WLITC0", 8, 56);
	walltextures.back().patches.emplace_back("WLITA0", 0, 64);
	walltextures.back().patches.emplace_back("WLITC0", 8, 64);
	walltextures.back().patches.emplace_back("WLITA0", 0, 72);
	walltextures.back().patches.emplace_back("WLITC0", 8, 72);
	walltextures.back().patches.emplace_back("WLITA0", 0, 80);
	walltextures.back().patches.emplace_back("WLITC0", 8, 80);
	walltextures.back().patches.emplace_back("WLITA0", 0, 88);
	walltextures.back().patches.emplace_back("WLITC0", 8, 88);
	walltextures.back().patches.emplace_back("WLITA0", 0, 96);
	walltextures.back().patches.emplace_back("WLITC0", 8, 96);
	walltextures.back().patches.emplace_back("WLITA0", 0, 104);
	walltextures.back().patches.emplace_back("WLITC0", 8, 104);
	walltextures.back().patches.emplace_back("WLITA0", 0, 112);
	walltextures.back().patches.emplace_back("WLITC0", 8, 112);
	walltextures.back().patches.emplace_back("WLITA0", 0, 120);
	walltextures.back().patches.emplace_back("WLITC0", 8, 120);


	walltextures.emplace_back("LITEBLU1", 8, 128);

	walltextures.back().patches.emplace_back("AGB128_1", 0, 0);


	walltextures.emplace_back("LITEBLU2", 16, 72);

	walltextures.back().patches.emplace_back("W13_A", 0, 0);


	walltextures.emplace_back("LITEBLU3", 16, 128);

	walltextures.back().patches.emplace_back("W13_8", 0, 0);
	walltextures.back().patches.emplace_back("W13_8", 0, 56);


	walltextures.emplace_back("LITEBLU4", 16, 128);

	walltextures.back().patches.emplace_back("BLITA0", 0, 0);
	walltextures.back().patches.emplace_back("BLITB0", 8, 0);
	walltextures.back().patches.emplace_back("BLITC0", 8, 0);
	walltextures.back().patches.emplace_back("BLITA0", 0, 8);
	walltextures.back().patches.emplace_back("BLITC0", 8, 8);
	walltextures.back().patches.emplace_back("BLITA0", 0, 16);
	walltextures.back().patches.emplace_back("BLITC0", 8, 16);
	walltextures.back().patches.emplace_back("BLITA0", 0, 24);
	walltextures.back().patches.emplace_back("BLITC0", 8, 24);
	walltextures.back().patches.emplace_back("BLITA0", 0, 32);
	walltextures.back().patches.emplace_back("BLITC0", 8, 32);
	walltextures.back().patches.emplace_back("BLITA0", 0, 40);
	walltextures.back().patches.emplace_back("BLITC0", 8, 40);
	walltextures.back().patches.emplace_back("BLITA0", 0, 48);
	walltextures.back().patches.emplace_back("BLITC0", 8, 48);
	walltextures.back().patches.emplace_back("BLITA0", 0, 56);
	walltextures.back().patches.emplace_back("BLITC0", 8, 56);
	walltextures.back().patches.emplace_back("BLITA0", 0, 64);
	walltextures.back().patches.emplace_back("BLITC0", 8, 64);
	walltextures.back().patches.emplace_back("BLITA0", 0, 72);
	walltextures.back().patches.emplace_back("BLITC0", 8, 72);
	walltextures.back().patches.emplace_back("BLITA0", 0, 80);
	walltextures.back().patches.emplace_back("BLITC0", 8, 80);
	walltextures.back().patches.emplace_back("BLITA0", 0, 88);
	walltextures.back().patches.emplace_back("BLITC0", 8, 88);
	walltextures.back().patches.emplace_back("BLITA0", 0, 96);
	walltextures.back().patches.emplace_back("BLITC0", 8, 96);
	walltextures.back().patches.emplace_back("BLITA0", 0, 104);
	walltextures.back().patches.emplace_back("BLITC0", 8, 104);
	walltextures.back().patches.emplace_back("BLITA0", 0, 112);
	walltextures.back().patches.emplace_back("BLITC0", 8, 112);
	walltextures.back().patches.emplace_back("BLITA0", 0, 120);
	walltextures.back().patches.emplace_back("BLITC0", 8, 120);


	walltextures.emplace_back("METAL1", 64, 128);

	walltextures.back().patches.emplace_back("WALL03_7", 0, 0);
	walltextures.back().patches.emplace_back("WALL03_7", 0, 64);


	walltextures.emplace_back("NUKE24", 64, 24);

	walltextures.back().patches.emplace_back("NUKEDGE", 0, 0);


	walltextures.emplace_back("NUKEDGE1", 128, 128);

	walltextures.back().patches.emplace_back("WALL04_3", 0, 0);
	walltextures.back().patches.emplace_back("WALL04_4", 16, 0);
	walltextures.back().patches.emplace_back("WALL04_5", 32, 0);
	walltextures.back().patches.emplace_back("WALL04_2", 48, 0);
	walltextures.back().patches.emplace_back("WALL04_2", 64, 0);
	walltextures.back().patches.emplace_back("WALL04_5", 96, 32);
	walltextures.back().patches.emplace_back("WALL04_3", 80, 0);
	walltextures.back().patches.emplace_back("WALL04_5", 112, 0);
	walltextures.back().patches.emplace_back("WALL04_7", 96, 0);
	walltextures.back().patches.emplace_back("NUKEDGE", 0, 104);
	walltextures.back().patches.emplace_back("NUKEDGE", 64, 104);
	walltextures.back().patches.emplace_back("WALL04_5", 112, 32);
	walltextures.back().patches.emplace_back("WALL04_3", 0, 33);
	walltextures.back().patches.emplace_back("WALL04_4", 16, 33);
	walltextures.back().patches.emplace_back("WALL04_5", 32, 33);
	walltextures.back().patches.emplace_back("WALL04_2", 48, 33);
	walltextures.back().patches.emplace_back("WALL04_2", 64, 33);
	walltextures.back().patches.emplace_back("WALL04_3", 80, 33);


	walltextures.emplace_back("NUKESLAD", 64, 128);

	walltextures.back().patches.emplace_back("WALL57_1", 0, 0);


	walltextures.emplace_back("PIPE2", 256, 128);

	walltextures.back().patches.emplace_back("TP2_1", 0, 0);
	walltextures.back().patches.emplace_back("TP2_2", 128, 0);


	walltextures.emplace_back("PLANET1", 256, 128);

	walltextures.back().patches.emplace_back("AG128_1", 0, 0);
	walltextures.back().patches.emplace_back("TSCRN2", 4, 69);
	walltextures.back().patches.emplace_back("AG128_1", 64, 0);
	walltextures.back().patches.emplace_back("TSCRN3", 68, 69);
	walltextures.back().patches.emplace_back("AG128_1", 128, 0);
	walltextures.back().patches.emplace_back("TSCRN4", 132, 69);
	walltextures.back().patches.emplace_back("TSCRN5", 68, 7);
	walltextures.back().patches.emplace_back("TSCRN6", 4, 7);
	walltextures.back().patches.emplace_back("TSCRN8", 132, 7);
	walltextures.back().patches.emplace_back("AG128_1", 192, 0);


	walltextures.emplace_back("PLAT1", 128, 128);

	walltextures.back().patches.emplace_back("PLAT2_1", 0, 0);


	walltextures.emplace_back("REDWALL1", 128, 128);

	walltextures.back().patches.emplace_back("W15_4", 0, 56);
	walltextures.back().patches.emplace_back("W15_5", 64, 56);
	walltextures.back().patches.emplace_back("W15_4", 0, 0);
	walltextures.back().patches.emplace_back("W15_5", 64, 0);


	walltextures.emplace_back("SHAWN2", 64, 128);

	walltextures.back().patches.emplace_back("AG128_1", 0, 0);


	walltextures.emplace_back("SKY1", 256, 128);

	walltextures.back().patches.emplace_back("SKY1", 0, -8);


	walltextures.emplace_back("SLADPOIS", 64, 128);

	walltextures.back().patches.emplace_back("WLA128_1", 0, 0);
	walltextures.back().patches.emplace_back("PS20A0", 1, 49);


	walltextures.emplace_back("SLADRIP1", 64, 128);

	walltextures.back().patches.emplace_back("WALL57_2", 0, 0);


	walltextures.emplace_back("SLADRIP2", 64, 128);

	walltextures.back().patches.emplace_back("WALL57_3", 0, 0);


	walltextures.emplace_back("SLADRIP3", 64, 128);

	walltextures.back().patches.emplace_back("WALL57_4", 0, 0);


	walltextures.emplace_back("SLADWALL", 64, 128);

	walltextures.back().patches.emplace_back("WLA128_1", 0, 0);


	walltextures.emplace_back("STARG1", 64, 128);

	walltextures.back().patches.emplace_back("SW12_1", 0, 0);
	walltextures.back().patches.emplace_back("SW12_2", 32, 0);


	walltextures.emplace_back("STARG3", 128, 128);

	walltextures.back().patches.emplace_back("SW19_3", 64, 0);
	walltextures.back().patches.emplace_back("SW19_4", 0, 0);


	walltextures.emplace_back("STARGR1", 64, 128);

	walltextures.back().patches.emplace_back("SW11_1", 0, 0);
	walltextures.back().patches.emplace_back("SW11_2", 32, 0);


	walltextures.emplace_back("STARTAN1", 64, 128);

	walltextures.back().patches.emplace_back("SW12_4", 0, 0);
	walltextures.back().patches.emplace_back("SW12_5", 32, 0);


	walltextures.emplace_back("STARTAN2", 128, 128);

	walltextures.back().patches.emplace_back("SW17_4", 0, 0);
	walltextures.back().patches.emplace_back("SW17_5", 32, 0);
	walltextures.back().patches.emplace_back("SW17_6", 64, 0);
	walltextures.back().patches.emplace_back("SW18_7", 96, 0);


	walltextures.emplace_back("STARTAN3", 128, 128);

	walltextures.back().patches.emplace_back("SW19_1", 64, 0);
	walltextures.back().patches.emplace_back("SW19_2", 0, 0);


	walltextures.emplace_back("STEP1", 32, 8);

	walltextures.back().patches.emplace_back("STEP04", 0, 0);


	walltextures.emplace_back("STEP2", 32, 8);

	walltextures.back().patches.emplace_back("STEP03", 0, 0);


	walltextures.emplace_back("STEP3", 32, 8);

	walltextures.back().patches.emplace_back("STEP05", 0, 0);


	walltextures.emplace_back("STEP4", 32, 16);

	walltextures.back().patches.emplace_back("STEP06", 0, 0);
	walltextures.back().patches.emplace_back("STEP06", 0, 8);


	walltextures.emplace_back("STEP5", 32, 16);

	walltextures.back().patches.emplace_back("STEP09", 0, 0);
	walltextures.back().patches.emplace_back("STEP09", 0, 8);


	walltextures.emplace_back("STEP6", 32, 16);

	walltextures.back().patches.emplace_back("STEP10", 0, 8);
	walltextures.back().patches.emplace_back("STEP07", 0, 0);


	walltextures.emplace_back("STONE", 256, 128);

	walltextures.back().patches.emplace_back("WALL01_1", 0, 0);
	walltextures.back().patches.emplace_back("WALL01_2", 16, 0);
	walltextures.back().patches.emplace_back("WALL01_3", 32, 0);
	walltextures.back().patches.emplace_back("WALL01_4", 48, 0);
	walltextures.back().patches.emplace_back("WALL01_5", 64, 0);
	walltextures.back().patches.emplace_back("WALL01_6", 80, 0);
	walltextures.back().patches.emplace_back("WALL01_7", 96, 0);
	walltextures.back().patches.emplace_back("WALL01_8", 112, 0);
	walltextures.back().patches.emplace_back("WALL01_9", 128, 0);
	walltextures.back().patches.emplace_back("WALL01_A", 144, 0);
	walltextures.back().patches.emplace_back("WALL01_B", 160, 0);
	walltextures.back().patches.emplace_back("WALL01_C", 176, 0);
	walltextures.back().patches.emplace_back("WALL01_3", 192, 0);
	walltextures.back().patches.emplace_back("WALL01_1", 208, 0);
	walltextures.back().patches.emplace_back("WALL01_6", 224, 0);
	walltextures.back().patches.emplace_back("WALL01_A", 240, 0);
	walltextures.back().patches.emplace_back("WALL01_1", 0, 72);
	walltextures.back().patches.emplace_back("WALL01_2", 16, 72);
	walltextures.back().patches.emplace_back("WALL01_3", 32, 72);
	walltextures.back().patches.emplace_back("WALL01_4", 48, 72);
	walltextures.back().patches.emplace_back("WALL01_5", 64, 72);
	walltextures.back().patches.emplace_back("WALL01_6", 80, 72);
	walltextures.back().patches.emplace_back("WALL01_7", 96, 72);
	walltextures.back().patches.emplace_back("WALL01_8", 112, 72);
	walltextures.back().patches.emplace_back("WALL01_9", 128, 72);
	walltextures.back().patches.emplace_back("WALL01_A", 144, 72);
	walltextures.back().patches.emplace_back("WALL01_B", 160, 72);
	walltextures.back().patches.emplace_back("WALL01_C", 176, 72);
	walltextures.back().patches.emplace_back("WALL01_3", 192, 72);
	walltextures.back().patches.emplace_back("WALL01_1", 208, 72);
	walltextures.back().patches.emplace_back("WALL01_6", 224, 72);
	walltextures.back().patches.emplace_back("WALL01_A", 240, 72);
	walltextures.back().patches.emplace_back("W33_8", 0, 64);
	walltextures.back().patches.emplace_back("W33_8", 64, 64);
	walltextures.back().patches.emplace_back("W33_8", 128, 64);
	walltextures.back().patches.emplace_back("W33_8", 192, 64);
	walltextures.back().patches.emplace_back("W33_8", 0, 120);
	walltextures.back().patches.emplace_back("W33_8", 64, 120);
	walltextures.back().patches.emplace_back("W33_8", 128, 120);
	walltextures.back().patches.emplace_back("W33_8", 192, 120);


	walltextures.emplace_back("STONE2", 128, 128);

	walltextures.back().patches.emplace_back("W28_8", 0, 64);
	walltextures.back().patches.emplace_back("W28_8", 64, 0);
	walltextures.back().patches.emplace_back("W28_5", 0, 0);
	walltextures.back().patches.emplace_back("W28_5", 64, 64);


	walltextures.emplace_back("STONE3", 128, 128);

	walltextures.back().patches.emplace_back("W28_7", 0, 0);
	walltextures.back().patches.emplace_back("W28_6", 0, 64);
	walltextures.back().patches.emplace_back("W28_7", 64, 64);
	walltextures.back().patches.emplace_back("W28_6", 64, 0);


	walltextures.emplace_back("STONPOIS", 64, 128);

	walltextures.back().patches.emplace_back("WALL01_1", 0, 0);
	walltextures.back().patches.emplace_back("WALL01_2", 16, 0);
	walltextures.back().patches.emplace_back("WALL01_3", 32, 0);
	walltextures.back().patches.emplace_back("WALL01_4", 48, 0);
	walltextures.back().patches.emplace_back("WALL01_1", 0, 72);
	walltextures.back().patches.emplace_back("WALL01_2", 16, 72);
	walltextures.back().patches.emplace_back("WALL01_3", 32, 72);
	walltextures.back().patches.emplace_back("WALL01_4", 48, 72);
	walltextures.back().patches.emplace_back("W33_8", 0, 64);
	walltextures.back().patches.emplace_back("W33_8", 0, 120);
	walltextures.back().patches.emplace_back("PS20A0", 1, 43);


	walltextures.emplace_back("SUPPORT2", 64, 128);

	walltextures.back().patches.emplace_back("SUPPORT2", 19, 72);
	walltextures.back().patches.emplace_back("SUPPORT2", 19, 0);
	walltextures.back().patches.emplace_back("SUPPORT2", 0, 0);
	walltextures.back().patches.emplace_back("SUPPORT2", 0, 72);
	walltextures.back().patches.emplace_back("SUPPORT2", 40, 0);
	walltextures.back().patches.emplace_back("SUPPORT2", 40, 72);


	walltextures.emplace_back("SW1BRCOM", 128, 128);

	walltextures.back().patches.emplace_back("WALL62_1", 0, 0);
	walltextures.back().patches.emplace_back("SW1S0", 48, 72);


	walltextures.emplace_back("SW1BRN1", 64, 128);

	walltextures.back().patches.emplace_back("WALL02_2", 0, 56);
	walltextures.back().patches.emplace_back("WALL02_2", 0, 0);
	walltextures.back().patches.emplace_back("SW3S0", 16, 75);


	walltextures.emplace_back("SW1BRN2", 64, 128);

	walltextures.back().patches.emplace_back("WALL02_2", 0, 56);
	walltextures.back().patches.emplace_back("WALL02_2", 0, 0);
	walltextures.back().patches.emplace_back("SW4S0", 20, 79);


	walltextures.emplace_back("SW1BRNGN", 64, 128);

	walltextures.back().patches.emplace_back("WALL62_2", 0, 0);
	walltextures.back().patches.emplace_back("SW4S0", 20, 80);


	walltextures.emplace_back("SW1BROWN", 128, 128);

	walltextures.back().patches.emplace_back("WALL62_1", 0, 0);
	walltextures.back().patches.emplace_back("SW3S1", 48, 72);


	walltextures.emplace_back("SW1COMM", 64, 72);

	walltextures.back().patches.emplace_back("W13_1", 0, 0);
	walltextures.back().patches.emplace_back("SW1S0", 15, 18);


	walltextures.emplace_back("SW1COMP", 64, 128);

	walltextures.back().patches.emplace_back("COMP03_4", 0, 64);
	walltextures.back().patches.emplace_back("COMP04_5", 0, 0);
	walltextures.back().patches.emplace_back("COMP03_4", 32, 64);
	walltextures.back().patches.emplace_back("SW2S0", 16, 72);


	walltextures.emplace_back("SW1DIRT", 64, 128);

	walltextures.back().patches.emplace_back("WALL00_7", 32, 0);
	walltextures.back().patches.emplace_back("WALL00_6", 16, 0);
	walltextures.back().patches.emplace_back("SW1S0", 16, 20);
	walltextures.back().patches.emplace_back("WALL00_6", 0, -16);
	walltextures.back().patches.emplace_back("WALL00_7", 48, 0);


	walltextures.emplace_back("SW1EXIT", 32, 72);

	walltextures.back().patches.emplace_back("W32_4", 0, 0);
	walltextures.back().patches.emplace_back("SW2S0", 0, 16);
	walltextures.back().patches.emplace_back("W33_8", 0, 64);


	walltextures.emplace_back("SW1GRAY", 64, 128);

	walltextures.back().patches.emplace_back("W31_1", 0, 0);
	walltextures.back().patches.emplace_back("W31_1", 0, 64);
	walltextures.back().patches.emplace_back("SW2S0", 16, 70);


	walltextures.emplace_back("SW1GRAY1", 64, 128);

	walltextures.back().patches.emplace_back("W31_1", 0, 64);
	walltextures.back().patches.emplace_back("W31_1", 0, 0);
	walltextures.back().patches.emplace_back("SW4S0", 19, 79);


	walltextures.emplace_back("SW1METAL", 64, 128);

	walltextures.back().patches.emplace_back("WALL03_7", 0, 0);
	walltextures.back().patches.emplace_back("WALL03_7", 0, 64);
	walltextures.back().patches.emplace_back("SW4S0", 20, 68);


	walltextures.emplace_back("SW1PIPE", 128, 128);

	walltextures.back().patches.emplace_back("TP2_2", 0, 0);
	walltextures.back().patches.emplace_back("SW3S0", 48, 76);


	walltextures.emplace_back("SW1SLAD", 64, 128);

	walltextures.back().patches.emplace_back("WLA128_1", 0, 0);
	walltextures.back().patches.emplace_back("WARNB0", 24, 73);


	walltextures.emplace_back("SW1STARG", 128, 128);

	walltextures.back().patches.emplace_back("SW19_3", 64, 0);
	walltextures.back().patches.emplace_back("SW19_4", 0, 0);
	walltextures.back().patches.emplace_back("SW3S0", 48, 76);


	walltextures.emplace_back("SW1STON1", 64, 128);

	walltextures.back().patches.emplace_back("W28_8", 0, 64);
	walltextures.back().patches.emplace_back("W28_8", 0, 0);
	walltextures.back().patches.emplace_back("SW1S0", 16, 78);


	walltextures.emplace_back("SW1STON2", 64, 128);

	walltextures.back().patches.emplace_back("W28_5", 0, 0);
	walltextures.back().patches.emplace_back("W28_8", 0, 64);
	walltextures.back().patches.emplace_back("WARNB0", 24, 73);


	walltextures.emplace_back("SW1STONE", 64, 128);

	walltextures.back().patches.emplace_back("WALL01_1", 0, 0);
	walltextures.back().patches.emplace_back("WALL01_2", 16, 0);
	walltextures.back().patches.emplace_back("WALL01_3", 32, 0);
	walltextures.back().patches.emplace_back("WALL01_4", 48, 0);
	walltextures.back().patches.emplace_back("WALL01_1", 0, 72);
	walltextures.back().patches.emplace_back("WALL01_2", 16, 72);
	walltextures.back().patches.emplace_back("WALL01_3", 32, 72);
	walltextures.back().patches.emplace_back("WALL01_4", 48, 72);
	walltextures.back().patches.emplace_back("W33_8", 0, 64);
	walltextures.back().patches.emplace_back("W33_8", 0, 120);
	walltextures.back().patches.emplace_back("SW1S0", 16, 72);


	walltextures.emplace_back("SW1STRTN", 64, 128);

	walltextures.back().patches.emplace_back("SW12_4", 0, 0);
	walltextures.back().patches.emplace_back("SW12_5", 32, 0);
	walltextures.back().patches.emplace_back("SW1S0", 16, 72);


	walltextures.emplace_back("SW2BRCOM", 128, 128);

	walltextures.back().patches.emplace_back("WALL62_1", 0, 0);
	walltextures.back().patches.emplace_back("SW1S1", 48, 72);


	walltextures.emplace_back("SW2BRN1", 64, 128);

	walltextures.back().patches.emplace_back("STEP07", 32, 72);
	walltextures.back().patches.emplace_back("STEP07", 0, 72);
	walltextures.back().patches.emplace_back("WALL02_2", 0, 56);
	walltextures.back().patches.emplace_back("WALL02_2", 0, 0);
	walltextures.back().patches.emplace_back("SW3S1", 16, 75);


	walltextures.emplace_back("SW2BRN2", 64, 128);

	walltextures.back().patches.emplace_back("WALL02_2", 0, 56);
	walltextures.back().patches.emplace_back("WALL02_2", 0, 0);
	walltextures.back().patches.emplace_back("SW4S1", 20, 79);


	walltextures.emplace_back("SW2BRNGN", 64, 128);

	walltextures.back().patches.emplace_back("WALL62_2", 0, 0);
	walltextures.back().patches.emplace_back("SW4S1", 20, 80);


	walltextures.emplace_back("SW2BROWN", 128, 128);

	walltextures.back().patches.emplace_back("WALL62_1", 0, 0);
	walltextures.back().patches.emplace_back("SW3S0", 48, 72);


	walltextures.emplace_back("SW2COMM", 64, 72);

	walltextures.back().patches.emplace_back("W13_1", 0, 0);
	walltextures.back().patches.emplace_back("SW1S1", 15, 18);


	walltextures.emplace_back("SW2COMP", 64, 128);

	walltextures.back().patches.emplace_back("COMP03_4", 0, 64);
	walltextures.back().patches.emplace_back("COMP04_5", 0, 0);
	walltextures.back().patches.emplace_back("COMP03_4", 32, 64);
	walltextures.back().patches.emplace_back("SW2S1", 16, 72);


	walltextures.emplace_back("SW2DIRT", 64, 128);

	walltextures.back().patches.emplace_back("WALL00_8", 48, 0);
	walltextures.back().patches.emplace_back("WALL00_7", 32, 0);
	walltextures.back().patches.emplace_back("WALL00_6", 16, 0);
	walltextures.back().patches.emplace_back("WALL00_5", 0, -1);
	walltextures.back().patches.emplace_back("SW1S1", 16, 20);


	walltextures.emplace_back("SW2EXIT", 32, 72);

	walltextures.back().patches.emplace_back("W32_4", 0, 0);
	walltextures.back().patches.emplace_back("SW2S1", 0, 16);
	walltextures.back().patches.emplace_back("W33_8", 0, 64);


	walltextures.emplace_back("SW2GRAY", 64, 128);

	walltextures.back().patches.emplace_back("W31_1", 0, 0);
	walltextures.back().patches.emplace_back("W31_1", 0, 64);
	walltextures.back().patches.emplace_back("SW2S1", 16, 70);


	walltextures.emplace_back("SW2GRAY1", 64, 128);

	walltextures.back().patches.emplace_back("W31_1", 0, 64);
	walltextures.back().patches.emplace_back("W31_1", 0, 0);
	walltextures.back().patches.emplace_back("SW4S1", 19, 79);


	walltextures.emplace_back("SW2METAL", 64, 128);

	walltextures.back().patches.emplace_back("WALL03_7", 0, 0);
	walltextures.back().patches.emplace_back("WALL03_7", 0, 64);
	walltextures.back().patches.emplace_back("SW4S1", 20, 68);


	walltextures.emplace_back("SW2PIPE", 128, 128);

	walltextures.back().patches.emplace_back("TP2_2", 0, 0);
	walltextures.back().patches.emplace_back("SW3S1", 48, 76);


	walltextures.emplace_back("SW2SLAD", 64, 128);

	walltextures.back().patches.emplace_back("WLA128_1", 0, 0);
	walltextures.back().patches.emplace_back("WARNA0", 24, 73);


	walltextures.emplace_back("SW2STARG", 128, 128);

	walltextures.back().patches.emplace_back("SW19_3", 64, 0);
	walltextures.back().patches.emplace_back("SW19_4", 0, 0);
	walltextures.back().patches.emplace_back("SW3S1", 48, 76);


	walltextures.emplace_back("SW2STON1", 64, 128);

	walltextures.back().patches.emplace_back("W28_8", 0, 64);
	walltextures.back().patches.emplace_back("W28_8", 0, 0);
	walltextures.back().patches.emplace_back("SW1S1", 16, 78);


	walltextures.emplace_back("SW2STON2", 64, 128);

	walltextures.back().patches.emplace_back("W28_8", 0, 64);
	walltextures.back().patches.emplace_back("WARNA0", 24, 73);
	walltextures.back().patches.emplace_back("W28_5", 0, 0);


	walltextures.emplace_back("SW2STONE", 64, 128);

	walltextures.back().patches.emplace_back("WALL01_1", 0, 0);
	walltextures.back().patches.emplace_back("WALL01_2", 16, 0);
	walltextures.back().patches.emplace_back("WALL01_3", 32, 0);
	walltextures.back().patches.emplace_back("WALL01_4", 48, 0);
	walltextures.back().patches.emplace_back("WALL01_1", 0, 72);
	walltextures.back().patches.emplace_back("WALL01_2", 16, 72);
	walltextures.back().patches.emplace_back("WALL01_3", 32, 72);
	walltextures.back().patches.emplace_back("WALL01_4", 48, 72);
	walltextures.back().patches.emplace_back("W33_8", 0, 64);
	walltextures.back().patches.emplace_back("W33_8", 0, 120);
	walltextures.back().patches.emplace_back("SW1S1", 16, 72);


	walltextures.emplace_back("SW2STRTN", 64, 128);

	walltextures.back().patches.emplace_back("SW12_4", 0, 0);
	walltextures.back().patches.emplace_back("SW12_5", 32, 0);
	walltextures.back().patches.emplace_back("SW1S1", 16, 72);


	walltextures.emplace_back("TEKWALL1", 128, 128);

	walltextures.back().patches.emplace_back("W17_1", 0, -27);
	walltextures.back().patches.emplace_back("W17_1", 0, 16);


	walltextures.emplace_back("TEKWALL2", 128, 128);

	walltextures.back().patches.emplace_back("WALL24_1", 0, 0);


	walltextures.emplace_back("TEKWALL3", 128, 112);

	walltextures.back().patches.emplace_back("W17_1", -104, 0);


	walltextures.emplace_back("TEKWALL4", 128, 128);

	walltextures.back().patches.emplace_back("W94_1", 0, 0);


	walltextures.emplace_back("TEKWALL5", 128, 128);

	walltextures.back().patches.emplace_back("WALL24_1", -120, -8);


	walltextures.emplace_back("ASHWALL", 64, 128);

	walltextures.back().patches.emplace_back("W104_1", 0, 0);


	walltextures.emplace_back("BIGDOOR3", 128, 128);

	walltextures.back().patches.emplace_back("DOOR9_2", 0, 0);


	walltextures.emplace_back("BIGDOOR5", 128, 128);

	walltextures.back().patches.emplace_back("WALL40_1", 0, 0);
	walltextures.back().patches.emplace_back("WALL42_3", 51, 0);
	walltextures.back().patches.emplace_back("WALL42_3", 0, 0);
	walltextures.back().patches.emplace_back("WALL42_3", 104, 0);


	walltextures.emplace_back("BIGDOOR6", 128, 112);

	walltextures.back().patches.emplace_back("DOOR11_1", 4, 0);
	walltextures.back().patches.emplace_back("DOOR11_1", 124, 0);
	walltextures.back().patches.emplace_back("DOOR11_1", -116, 0);


	walltextures.emplace_back("BIGDOOR7", 128, 128);

	walltextures.back().patches.emplace_back("W105_1", -4, -4);
	walltextures.back().patches.emplace_back("W105_1", 124, -4);


	walltextures.emplace_back("BLODGR1", 32, 128);

	walltextures.back().patches.emplace_back("TP5_1", 0, 0);


	walltextures.emplace_back("BLODGR2", 32, 128);

	walltextures.back().patches.emplace_back("TP5_2", 0, 0);


	walltextures.emplace_back("BLODGR3", 32, 128);

	walltextures.back().patches.emplace_back("TP5_3", 0, 0);


	walltextures.emplace_back("BLODGR4", 32, 128);

	walltextures.back().patches.emplace_back("TP5_4", 0, 0);


	walltextures.emplace_back("BLODRIP1", 32, 128);

	walltextures.back().patches.emplace_back("RP2_1", 0, 0);


	walltextures.emplace_back("BLODRIP2", 32, 128);

	walltextures.back().patches.emplace_back("RP2_2", 0, 0);


	walltextures.emplace_back("BLODRIP3", 32, 128);

	walltextures.back().patches.emplace_back("RP2_3", 0, 0);


	walltextures.emplace_back("BLODRIP4", 32, 128);

	walltextures.back().patches.emplace_back("RP2_4", 0, 0);


	walltextures.emplace_back("BROVINE", 256, 128);

	walltextures.back().patches.emplace_back("WALL02_2", 0, 56);
	walltextures.back().patches.emplace_back("WALL02_2", 0, 0);
	walltextures.back().patches.emplace_back("WALL02_1", 88, 56);
	walltextures.back().patches.emplace_back("WALL02_1", 88, 0);
	walltextures.back().patches.emplace_back("WALL02_3", 64, 56);
	walltextures.back().patches.emplace_back("WALL02_3", 64, 0);
	walltextures.back().patches.emplace_back("WALL02_3", 152, 56);
	walltextures.back().patches.emplace_back("WALL02_3", 152, 0);
	walltextures.back().patches.emplace_back("WALL02_3", 232, 56);
	walltextures.back().patches.emplace_back("WALL02_2", 176, 56);
	walltextures.back().patches.emplace_back("WALL02_3", 232, 0);
	walltextures.back().patches.emplace_back("WALL02_2", 176, 0);
	walltextures.back().patches.emplace_back("W107_1", 0, 0);


	walltextures.emplace_back("BROVINE2", 256, 128);

	walltextures.back().patches.emplace_back("WALL62_2", 0, 0);
	walltextures.back().patches.emplace_back("WALL62_2", 64, 0);
	walltextures.back().patches.emplace_back("WALL62_2", 128, 0);
	walltextures.back().patches.emplace_back("WALL62_2", 192, 0);
	walltextures.back().patches.emplace_back("W106_1", 0, 0);


	walltextures.emplace_back("BROWNWEL", 256, 72);

	walltextures.back().patches.emplace_back("WALL03_4", 0, 0);
	walltextures.back().patches.emplace_back("WALL03_3", 128, 0);
	walltextures.back().patches.emplace_back("WALL03_4", 64, 0);
	walltextures.back().patches.emplace_back("WALL03_4", 192, 0);


	walltextures.emplace_back("CEMENT1", 128, 128);

	walltextures.back().patches.emplace_back("WALL52_1", 0, 0);


	walltextures.emplace_back("CEMENT2", 128, 128);

	walltextures.back().patches.emplace_back("WALL53_1", 0, 0);


	walltextures.emplace_back("CEMENT3", 128, 128);

	walltextures.back().patches.emplace_back("WALL54_1", 0, 0);


	walltextures.emplace_back("CEMENT4", 128, 128);

	walltextures.back().patches.emplace_back("WALL55_1", 0, 0);


	walltextures.emplace_back("CEMENT5", 128, 128);

	walltextures.back().patches.emplace_back("WALL52_2", 0, 0);


	walltextures.emplace_back("CEMENT6", 128, 128);

	walltextures.back().patches.emplace_back("WALL54_2", 0, 0);


	walltextures.emplace_back("CEMPOIS", 128, 128);

	walltextures.back().patches.emplace_back("WALL52_1", 0, 0);
	walltextures.back().patches.emplace_back("PS20A0", 0, 28);


	walltextures.emplace_back("COMPBLUE", 64, 128);

	walltextures.back().patches.emplace_back("COMP03_1", 0, 0);
	walltextures.back().patches.emplace_back("COMP03_2", 0, 64);


	walltextures.emplace_back("COMPOHSO", 128, 128);

	walltextures.back().patches.emplace_back("WALL21_1", 64, 70);
	walltextures.back().patches.emplace_back("WALL21_5", 0, 70);
	walltextures.back().patches.emplace_back("WALL21_3", 0, -2);
	walltextures.back().patches.emplace_back("WALL21_1", 64, -2);


	walltextures.emplace_back("COMPWERD", 64, 128);

	walltextures.back().patches.emplace_back("COMP04_6", 0, 0);
	walltextures.back().patches.emplace_back("COMP04_6", 0, 64);


	walltextures.emplace_back("CRATE1", 64, 128);

	walltextures.back().patches.emplace_back("BCRATEL1", 0, 0);
	walltextures.back().patches.emplace_back("BCRATER1", 32, 0);
	walltextures.back().patches.emplace_back("BCRATEL1", 0, 64);
	walltextures.back().patches.emplace_back("BCRATER1", 32, 64);


	walltextures.emplace_back("CRATE2", 64, 128);

	walltextures.back().patches.emplace_back("GCRATEL1", 0, 0);
	walltextures.back().patches.emplace_back("GCRATER1", 32, 0);
	walltextures.back().patches.emplace_back("GCRATEL1", 0, 64);
	walltextures.back().patches.emplace_back("GCRATER1", 32, 64);


	walltextures.emplace_back("CRATELIT", 64, 128);

	walltextures.back().patches.emplace_back("SGCRATE2", 0, 0);
	walltextures.back().patches.emplace_back("SGCRATE2", 32, 0);
	walltextures.back().patches.emplace_back("BCRATEL1", 0, 64);
	walltextures.back().patches.emplace_back("BCRATER1", 32, 64);


	walltextures.emplace_back("CRATINY", 64, 16);

	walltextures.back().patches.emplace_back("VGCRATE1", 0, 0);
	walltextures.back().patches.emplace_back("VGCRATE1", 16, 0);
	walltextures.back().patches.emplace_back("VGCRATE1", 32, 0);
	walltextures.back().patches.emplace_back("VGCRATE1", 48, 0);


	walltextures.emplace_back("CRATWIDE", 128, 128);

	walltextures.back().patches.emplace_back("BCRATEL1", 0, 64);
	walltextures.back().patches.emplace_back("BCRATEM1", 40, 64);
	walltextures.back().patches.emplace_back("BCRATEM1", 32, 64);
	walltextures.back().patches.emplace_back("BCRATEM1", 48, 64);
	walltextures.back().patches.emplace_back("BCRATEM1", 56, 64);
	walltextures.back().patches.emplace_back("BCRATER1", 96, 64);
	walltextures.back().patches.emplace_back("BCRATEM1", 88, 64);
	walltextures.back().patches.emplace_back("BCRATEM1", 80, 64);
	walltextures.back().patches.emplace_back("BCRATEM1", 72, 64);
	walltextures.back().patches.emplace_back("BCRATEM1", 64, 64);
	walltextures.back().patches.emplace_back("GCRATEL1", 0, 0);
	walltextures.back().patches.emplace_back("GCRATER1", 96, 0);
	walltextures.back().patches.emplace_back("GCRATEM1", 48, 0);
	walltextures.back().patches.emplace_back("GCRATEM1", 40, 0);
	walltextures.back().patches.emplace_back("GCRATEM1", 32, 0);
	walltextures.back().patches.emplace_back("GCRATEM1", 72, 0);
	walltextures.back().patches.emplace_back("GCRATEM1", 64, 0);
	walltextures.back().patches.emplace_back("GCRATEM1", 56, 0);
	walltextures.back().patches.emplace_back("GCRATEM1", 80, 0);
	walltextures.back().patches.emplace_back("GCRATEM1", 88, 0);


	walltextures.emplace_back("DOORBLU2", 16, 128);

	walltextures.back().patches.emplace_back("W108_2", 0, 0);
	walltextures.back().patches.emplace_back("W108_2", 0, 24);
	walltextures.back().patches.emplace_back("W108_2", 0, 48);
	walltextures.back().patches.emplace_back("W108_2", 0, 72);
	walltextures.back().patches.emplace_back("W108_2", 0, 96);
	walltextures.back().patches.emplace_back("STEP07", 0, 120);


	walltextures.emplace_back("DOORHI", 64, 128);

	walltextures.back().patches.emplace_back("DOOR2_3", 0, 0);
	walltextures.back().patches.emplace_back("DOOR2_3", 0, 32);
	walltextures.back().patches.emplace_back("DOOR2_3", 0, 96);
	walltextures.back().patches.emplace_back("DOOR2_3", 0, 64);


	walltextures.emplace_back("DOORRED2", 16, 128);

	walltextures.back().patches.emplace_back("W108_3", 0, 0);
	walltextures.back().patches.emplace_back("W108_3", 0, 24);
	walltextures.back().patches.emplace_back("W108_3", 0, 48);
	walltextures.back().patches.emplace_back("W108_3", 0, 72);
	walltextures.back().patches.emplace_back("W108_3", 0, 96);
	walltextures.back().patches.emplace_back("STEP07", 0, 120);


	walltextures.emplace_back("DOORYEL2", 16, 128);

	walltextures.back().patches.emplace_back("W108_4", 0, 0);
	walltextures.back().patches.emplace_back("W108_4", 0, 24);
	walltextures.back().patches.emplace_back("W108_4", 0, 48);
	walltextures.back().patches.emplace_back("W108_4", 0, 72);
	walltextures.back().patches.emplace_back("W108_4", 0, 96);
	walltextures.back().patches.emplace_back("STEP07", 0, 120);


	walltextures.emplace_back("EXITSTON", 64, 128);

	walltextures.back().patches.emplace_back("W28_8", 0, 64);
	walltextures.back().patches.emplace_back("W28_8", 0, 0);
	walltextures.back().patches.emplace_back("EXIT1", 17, 22);


	walltextures.emplace_back("FIREBLU1", 128, 128);

	walltextures.back().patches.emplace_back("W65B_1", 0, 0);


	walltextures.emplace_back("FIREBLU2", 128, 128);

	walltextures.back().patches.emplace_back("W65B_2", 0, 0);


	walltextures.emplace_back("FIRELAV2", 128, 128);

	walltextures.back().patches.emplace_back("W73A_2", 0, 0);


	walltextures.emplace_back("FIRELAV3", 128, 128);

	walltextures.back().patches.emplace_back("W73B_1", 0, 0);


	walltextures.emplace_back("FIRELAVA", 128, 128);

	walltextures.back().patches.emplace_back("W73A_1", 0, 0);


	walltextures.emplace_back("FIREMAG1", 128, 128);

	walltextures.back().patches.emplace_back("W74A_1", 0, 0);


	walltextures.emplace_back("FIREMAG2", 128, 128);

	walltextures.back().patches.emplace_back("W74A_2", 0, 0);


	walltextures.emplace_back("FIREMAG3", 128, 128);

	walltextures.back().patches.emplace_back("W74B_1", 0, 0);


	walltextures.emplace_back("FIREWALA", 128, 112);

	walltextures.back().patches.emplace_back("WALL23_1", 0, 0);


	walltextures.emplace_back("FIREWALB", 128, 112);

	walltextures.back().patches.emplace_back("WALL23_2", 0, 0);


	walltextures.emplace_back("FIREWALL", 128, 112);

	walltextures.back().patches.emplace_back("WALL22_1", 0, 0);


	walltextures.emplace_back("GRAY1", 64, 128);

	walltextures.back().patches.emplace_back("W31_1", 0, 64);
	walltextures.back().patches.emplace_back("W31_1", 0, 0);


	walltextures.emplace_back("GRAY2", 64, 72);

	walltextures.back().patches.emplace_back("W33_7", 0, -8);
	walltextures.back().patches.emplace_back("W31_1", 0, 56);
	walltextures.back().patches.emplace_back("DUCT1", 20, 16);


	walltextures.emplace_back("GRAYBIG", 128, 128);

	walltextures.back().patches.emplace_back("WALL00_1", 0, 0);
	walltextures.back().patches.emplace_back("WALL00_1", 64, 0);
	walltextures.back().patches.emplace_back("WALL00_2", 83, 0);


	walltextures.emplace_back("GRAYDANG", 64, 128);

	walltextures.back().patches.emplace_back("W33_7", 0, 0);
	walltextures.back().patches.emplace_back("W33_7", 0, 64);
	walltextures.back().patches.emplace_back("PS15A0", 14, 74);


	walltextures.emplace_back("GRAYPOIS", 64, 72);

	walltextures.back().patches.emplace_back("WALL04_9", 0, 0);
	walltextures.back().patches.emplace_back("WALL04_A", 16, 0);
	walltextures.back().patches.emplace_back("WALL04_B", 32, 0);
	walltextures.back().patches.emplace_back("WALL04_C", 48, 0);
	walltextures.back().patches.emplace_back("PS18A0", 1, 19);


	walltextures.emplace_back("GRAYVINE", 256, 128);

	walltextures.back().patches.emplace_back("WALL00_1", 0, 0);
	walltextures.back().patches.emplace_back("WALL00_1", 64, 0);
	walltextures.back().patches.emplace_back("WALL00_1", 128, -16);
	walltextures.back().patches.emplace_back("WALL00_1", 192, -16);
	walltextures.back().patches.emplace_back("W106_1", 0, 0);


	walltextures.emplace_back("GSTFONT1", 64, 128);

	walltextures.back().patches.emplace_back("WALL58_1", 0, 0);


	walltextures.emplace_back("GSTFONT2", 64, 128);

	walltextures.back().patches.emplace_back("WALL58_2", 0, 0);


	walltextures.emplace_back("GSTFONT3", 64, 128);

	walltextures.back().patches.emplace_back("WALL58_3", 0, 0);


	walltextures.emplace_back("GSTGARG", 64, 128);

	walltextures.back().patches.emplace_back("WALL30_2", 0, 0);


	walltextures.emplace_back("GSTLION", 64, 128);

	walltextures.back().patches.emplace_back("WALL30_4", 0, 0);


	walltextures.emplace_back("GSTONE1", 256, 128);

	walltextures.back().patches.emplace_back("WALL48_1", 0, 0);
	walltextures.back().patches.emplace_back("WALL48_2", 64, 0);
	walltextures.back().patches.emplace_back("WALL48_3", 128, 0);
	walltextures.back().patches.emplace_back("WALL48_4", 192, 0);


	walltextures.emplace_back("GSTONE2", 256, 128);

	walltextures.back().patches.emplace_back("WALL59_1", 0, 0);
	walltextures.back().patches.emplace_back("WALL59_2", 64, 0);
	walltextures.back().patches.emplace_back("WALL59_3", 128, 0);
	walltextures.back().patches.emplace_back("WALL59_4", 192, 0);


	walltextures.emplace_back("GSTSATYR", 64, 128);

	walltextures.back().patches.emplace_back("WALL30_3", 0, 0);


	walltextures.emplace_back("GSTVINE1", 256, 128);

	walltextures.back().patches.emplace_back("WALL48_1", 0, 0);
	walltextures.back().patches.emplace_back("WALL48_2", 64, 0);
	walltextures.back().patches.emplace_back("WALL48_3", 128, 0);
	walltextures.back().patches.emplace_back("WALL48_4", 192, 0);
	walltextures.back().patches.emplace_back("W106_1", 0, 0);


	walltextures.emplace_back("GSTVINE2", 256, 128);

	walltextures.back().patches.emplace_back("WALL48_1", 0, 0);
	walltextures.back().patches.emplace_back("WALL48_2", 64, 0);
	walltextures.back().patches.emplace_back("WALL48_3", 128, 0);
	walltextures.back().patches.emplace_back("WALL48_4", 192, 0);
	walltextures.back().patches.emplace_back("W107_1", 0, 0);


	walltextures.emplace_back("ICKDOOR1", 64, 72);

	walltextures.back().patches.emplace_back("WALL69_4", 0, 9);
	walltextures.back().patches.emplace_back("WALL70_3", 0, 0);


	walltextures.emplace_back("ICKWALL1", 64, 128);

	walltextures.back().patches.emplace_back("WALL69_4", 0, 0);
	walltextures.back().patches.emplace_back("W32_4", 0, 64);


	walltextures.emplace_back("ICKWALL2", 64, 128);

	walltextures.back().patches.emplace_back("WALL71_5", 0, 0);
	walltextures.back().patches.emplace_back("W33_5", 0, 64);


	walltextures.emplace_back("ICKWALL3", 64, 128);

	walltextures.back().patches.emplace_back("WALL72_7", 0, 0);
	walltextures.back().patches.emplace_back("WALL69_9", 0, 64);
	walltextures.back().patches.emplace_back("W67_2", 32, 64);


	walltextures.emplace_back("ICKWALL4", 64, 128);

	walltextures.back().patches.emplace_back("WALL72_5", 0, 0);
	walltextures.back().patches.emplace_back("WALL72_3", 0, 64);


	walltextures.emplace_back("ICKWALL5", 64, 128);

	walltextures.back().patches.emplace_back("WALL70_4", 0, 0);
	walltextures.back().patches.emplace_back("W32_4", 0, 64);


	walltextures.emplace_back("ICKWALL6", 64, 128);

	walltextures.back().patches.emplace_back("WALL69_4", 0, 0);
	walltextures.back().patches.emplace_back("WALL70_2", 4, 0);
	walltextures.back().patches.emplace_back("W32_4", 0, 64);


	walltextures.emplace_back("ICKWALL7", 64, 128);

	walltextures.back().patches.emplace_back("W67_1", 0, 0);
	walltextures.back().patches.emplace_back("W67_2", 63, 64);
	walltextures.back().patches.emplace_back("WALL70_9", 28, 64);
	walltextures.back().patches.emplace_back("W67_2", -1, 64);


	walltextures.emplace_back("LITE96", 64, 128);

	walltextures.back().patches.emplace_back("WALL62_1", -1, 0);
	walltextures.back().patches.emplace_back("FLAMP", 4, 18);
	walltextures.back().patches.emplace_back("FLAMP", 38, 18);


	walltextures.emplace_back("LITEMET", 128, 128);

	walltextures.back().patches.emplace_back("WALL03_7", 0, 0);
	walltextures.back().patches.emplace_back("WALL03_7", 64, 0);
	walltextures.back().patches.emplace_back("WALL03_7", 0, 64);
	walltextures.back().patches.emplace_back("WALL03_7", 64, 64);
	walltextures.back().patches.emplace_back("FLAMP", 81, 34);
	walltextures.back().patches.emplace_back("FLAMP", 19, 34);


	walltextures.emplace_back("LITERED", 16, 128);

	walltextures.back().patches.emplace_back("W15_6", -4, 56);
	walltextures.back().patches.emplace_back("W15_6", -4, 0);


	walltextures.emplace_back("LITESTON", 64, 128);

	walltextures.back().patches.emplace_back("W28_5", 0, 0);
	walltextures.back().patches.emplace_back("W28_8", 0, 64);
	walltextures.back().patches.emplace_back("FLAMP", 7, 27);
	walltextures.back().patches.emplace_back("FLAMP", 37, 27);


	walltextures.emplace_back("MARBFAC2", 128, 128);

	walltextures.back().patches.emplace_back("MWALL4_2", 0, 0);


	walltextures.emplace_back("MARBFAC3", 128, 128);

	walltextures.back().patches.emplace_back("MWALL5_1", 0, 0);


	walltextures.emplace_back("MARBFACE", 128, 128);

	walltextures.back().patches.emplace_back("MWALL4_1", 0, 0);


	walltextures.emplace_back("MARBLE1", 128, 128);

	walltextures.back().patches.emplace_back("MWALL1_1", 0, 0);


	walltextures.emplace_back("MARBLE2", 128, 128);

	walltextures.back().patches.emplace_back("MWALL2_1", 0, 0);


	walltextures.emplace_back("MARBLE3", 128, 128);

	walltextures.back().patches.emplace_back("MWALL3_1", 0, 0);


	walltextures.emplace_back("MARBLOD1", 128, 128);

	walltextures.back().patches.emplace_back("MWALL1_2", 0, 0);


	walltextures.emplace_back("METAL", 64, 128);

	walltextures.back().patches.emplace_back("WALL47_1", 0, 0);


	walltextures.emplace_back("MIDBRN1", 64, 112);

	walltextures.back().patches.emplace_back("DOOR12_1", 0, 0);


	walltextures.emplace_back("MIDGRATE", 128, 128);

	walltextures.back().patches.emplace_back("M1_1", 0, 0);


	walltextures.emplace_back("MIDVINE1", 256, 128);

	walltextures.back().patches.emplace_back("W106_1", 0, 0);


	walltextures.emplace_back("MIDVINE2", 256, 128);

	walltextures.back().patches.emplace_back("W107_1", 0, 0);


	walltextures.emplace_back("NUKEPOIS", 64, 128);

	walltextures.back().patches.emplace_back("WALL04_3", 0, 0);
	walltextures.back().patches.emplace_back("WALL04_4", 16, 0);
	walltextures.back().patches.emplace_back("WALL04_5", 32, 0);
	walltextures.back().patches.emplace_back("WALL04_2", 48, 0);
	walltextures.back().patches.emplace_back("NUKEDGE", 0, 104);
	walltextures.back().patches.emplace_back("WALL04_3", 0, 33);
	walltextures.back().patches.emplace_back("WALL04_4", 16, 33);
	walltextures.back().patches.emplace_back("WALL04_5", 32, 33);
	walltextures.back().patches.emplace_back("WALL04_2", 48, 33);
	walltextures.back().patches.emplace_back("PS20A0", -1, 68);


	walltextures.emplace_back("PIPE1", 256, 128);

	walltextures.back().patches.emplace_back("RP1_1", 0, 0);
	walltextures.back().patches.emplace_back("RP1_2", 128, 0);


	walltextures.emplace_back("PIPE4", 256, 128);

	walltextures.back().patches.emplace_back("TP7_1", 0, 0);
	walltextures.back().patches.emplace_back("TP7_2", 128, 0);


	walltextures.emplace_back("PIPE6", 256, 128);

	walltextures.back().patches.emplace_back("TP3_1", 0, 0);
	walltextures.back().patches.emplace_back("TP3_2", 128, 0);


	walltextures.emplace_back("REDWALL", 128, 128);

	walltextures.back().patches.emplace_back("WALL78_1", 0, 0);


	walltextures.emplace_back("ROCKRED1", 128, 128);

	walltextures.back().patches.emplace_back("WALL64_2", 0, 0);


	walltextures.emplace_back("ROCKRED2", 128, 128);

	walltextures.back().patches.emplace_back("W64B_1", 0, 0);


	walltextures.emplace_back("ROCKRED3", 128, 128);

	walltextures.back().patches.emplace_back("W64B_2", 0, 0);


	walltextures.emplace_back("SHAWN1", 128, 128);

	walltextures.back().patches.emplace_back("W13_1", 64, 56);
	walltextures.back().patches.emplace_back("W13_1", 0, 56);
	walltextures.back().patches.emplace_back("CYL1_1", 0, 0);
	walltextures.back().patches.emplace_back("WALL01_1", 118, 0);
	walltextures.back().patches.emplace_back("WALL01_1", 117, 56);


	walltextures.emplace_back("SHAWN3", 64, 72);

	walltextures.back().patches.emplace_back("T14_3", 0, 0);


	walltextures.emplace_back("SKIN2", 128, 128);

	walltextures.back().patches.emplace_back("HELL8_2", 64, 0);
	walltextures.back().patches.emplace_back("HELL8_4", 0, 0);


	walltextures.emplace_back("SKINBORD", 64, 128);

	walltextures.back().patches.emplace_back("HELL6_1", 0, 0);
	walltextures.back().patches.emplace_back("HELL8_3", 32, 0);


	walltextures.emplace_back("SKINCUT", 256, 128);

	walltextures.back().patches.emplace_back("W102_1", 0, 0);
	walltextures.back().patches.emplace_back("W102_2", 128, 0);


	walltextures.emplace_back("SKINEDGE", 128, 128);

	walltextures.back().patches.emplace_back("HELL6_2", 0, 0);
	walltextures.back().patches.emplace_back("HELL8_1", 64, 0);


	walltextures.emplace_back("SKINFACE", 256, 128);

	walltextures.back().patches.emplace_back("HELL5_1", 0, 0);
	walltextures.back().patches.emplace_back("HELL5_2", 128, 0);


	walltextures.emplace_back("SKINLOW", 256, 104);

	walltextures.back().patches.emplace_back("W92_1", 0, 0);
	walltextures.back().patches.emplace_back("W92_2", 128, 0);


	walltextures.emplace_back("SKINMET1", 256, 128);

	walltextures.back().patches.emplace_back("W98_1", 0, 0);
	walltextures.back().patches.emplace_back("W98_2", 128, 0);


	walltextures.emplace_back("SKINMET2", 256, 128);

	walltextures.back().patches.emplace_back("W99_1", 0, 0);
	walltextures.back().patches.emplace_back("W99_2", 128, 0);


	walltextures.emplace_back("SKINSCAB", 256, 128);

	walltextures.back().patches.emplace_back("W101_1", 0, 0);
	walltextures.back().patches.emplace_back("W101_2", 128, 0);


	walltextures.emplace_back("SKINSYMB", 256, 128);

	walltextures.back().patches.emplace_back("W103_1", 0, 0);
	walltextures.back().patches.emplace_back("W103_2", 128, 0);


	walltextures.emplace_back("SKINTEK1", 256, 128);

	walltextures.back().patches.emplace_back("W109_1", 0, 0);
	walltextures.back().patches.emplace_back("W109_2", 128, 0);


	walltextures.emplace_back("SKINTEK2", 256, 128);

	walltextures.back().patches.emplace_back("W110_1", 0, 0);


	walltextures.emplace_back("SKSNAKE1", 64, 128);

	walltextures.back().patches.emplace_back("SNAK7_1", 0, 0);


	walltextures.emplace_back("SKSNAKE2", 64, 128);

	walltextures.back().patches.emplace_back("SNAK8_1", 0, 0);


	walltextures.emplace_back("SKSPINE1", 128, 128);

	walltextures.back().patches.emplace_back("SPINE4_1", 0, 0);


	walltextures.emplace_back("SKSPINE2", 256, 96);

	walltextures.back().patches.emplace_back("SPINE3_1", 0, 0);
	walltextures.back().patches.emplace_back("SPINE3_2", 128, 0);


	walltextures.emplace_back("SKULWAL3", 128, 128);

	walltextures.back().patches.emplace_back("WALL76_1", -12, 0);
	walltextures.back().patches.emplace_back("WALL76_1", 116, 0);


	walltextures.emplace_back("SKULWALL", 128, 128);

	walltextures.back().patches.emplace_back("WALL79_1", -4, 0);
	walltextures.back().patches.emplace_back("WALL79_1", 124, 0);


	walltextures.emplace_back("SKY2", 256, 128);

	walltextures.back().patches.emplace_back("SKY2", 0, 0);


	walltextures.emplace_back("SKY3", 256, 128);

	walltextures.back().patches.emplace_back("SKY3", 0, 0);


	walltextures.emplace_back("SLADSKUL", 64, 128);

	walltextures.back().patches.emplace_back("WLA128_1", 0, 0);
	walltextures.back().patches.emplace_back("SW2_2", 21, 65);


	walltextures.emplace_back("SP_DUDE1", 128, 128);

	walltextures.back().patches.emplace_back("WALL50_1", 0, 0);


	walltextures.emplace_back("SP_DUDE2", 128, 128);

	walltextures.back().patches.emplace_back("WALL50_2", 0, 0);


	walltextures.emplace_back("SP_DUDE3", 64, 128);

	walltextures.back().patches.emplace_back("WALL51_1", 0, 0);


	walltextures.emplace_back("SP_DUDE4", 64, 128);

	walltextures.back().patches.emplace_back("WALL51_2", 0, 0);


	walltextures.emplace_back("SP_DUDE5", 64, 128);

	walltextures.back().patches.emplace_back("WALL51_3", 0, 0);


	walltextures.emplace_back("SP_DUDE6", 64, 128);

	walltextures.back().patches.emplace_back("W108_1", 0, 0);


	walltextures.emplace_back("SP_FACE1", 128, 96);

	walltextures.back().patches.emplace_back("WALL25_1", 0, 0);


	walltextures.emplace_back("SP_HOT1", 256, 128);

	walltextures.back().patches.emplace_back("WALL49_1", 0, 0);
	walltextures.back().patches.emplace_back("WALL49_2", 64, 0);
	walltextures.back().patches.emplace_back("WALL49_3", 128, 0);
	walltextures.back().patches.emplace_back("WALL49_4", 192, 0);


	walltextures.emplace_back("SP_ROCK1", 128, 128);

	walltextures.back().patches.emplace_back("WALL63_1", 0, 0);


	walltextures.emplace_back("SP_ROCK2", 128, 128);

	walltextures.back().patches.emplace_back("WALL63_2", 0, 0);


	walltextures.emplace_back("STARBR2", 128, 128);

	walltextures.back().patches.emplace_back("SW15_4", 0, 0);
	walltextures.back().patches.emplace_back("SW16_4", 32, 0);
	walltextures.back().patches.emplace_back("SW15_6", 64, 0);
	walltextures.back().patches.emplace_back("SW16_6", 96, 0);


	walltextures.emplace_back("STARG2", 128, 128);

	walltextures.back().patches.emplace_back("SW17_1", 0, 0);
	walltextures.back().patches.emplace_back("SW17_2", 32, 0);
	walltextures.back().patches.emplace_back("SW17_3", 64, 0);
	walltextures.back().patches.emplace_back("SW18_5", 96, 0);


	walltextures.emplace_back("STARGR2", 128, 128);

	walltextures.back().patches.emplace_back("SW15_1", 0, 0);
	walltextures.back().patches.emplace_back("SW15_3", 64, 0);
	walltextures.back().patches.emplace_back("SW16_1", 96, 0);
	walltextures.back().patches.emplace_back("SW16_2", 32, 0);


	walltextures.emplace_back("STEPLAD1", 64, 16);

	walltextures.back().patches.emplace_back("LADDER16", 0, 0);


	walltextures.emplace_back("STEPTOP", 128, 16);

	walltextures.back().patches.emplace_back("RIPW15", 0, 0);


	walltextures.emplace_back("STONGARG", 128, 128);

	walltextures.back().patches.emplace_back("W28_6", 64, 64);
	walltextures.back().patches.emplace_back("W28_7", 64, 0);
	walltextures.back().patches.emplace_back("W28_6", 0, 0);
	walltextures.back().patches.emplace_back("W28_7", 0, 64);
	walltextures.back().patches.emplace_back("SW2_3", 0, 79);
	walltextures.back().patches.emplace_back("SW2_3", 64, 15);


	walltextures.emplace_back("SUPPORT3", 64, 128);

	walltextures.back().patches.emplace_back("WALL42_3", 20, 0);
	walltextures.back().patches.emplace_back("WALL42_3", 0, 0);
	walltextures.back().patches.emplace_back("WALL42_3", 40, 0);


	walltextures.emplace_back("SW1BLUE", 64, 128);

	walltextures.back().patches.emplace_back("COMP03_1", 0, 0);
	walltextures.back().patches.emplace_back("COMP03_2", 0, 64);
	walltextures.back().patches.emplace_back("SW2_7", 14, 66);


	walltextures.emplace_back("SW1CMT", 64, 128);

	walltextures.back().patches.emplace_back("WALL54_1", -42, 0);
	walltextures.back().patches.emplace_back("SW3S1", 16, 72);


	walltextures.emplace_back("SW1GARG", 64, 128);

	walltextures.back().patches.emplace_back("WALL47_2", 0, 0);
	walltextures.back().patches.emplace_back("WALL42_6", 12, 62);


	walltextures.emplace_back("SW1GSTON", 64, 128);

	walltextures.back().patches.emplace_back("WALL48_2", 0, 0);
	walltextures.back().patches.emplace_back("SW2_7", 13, 67);


	walltextures.emplace_back("SW1HOT", 64, 128);

	walltextures.back().patches.emplace_back("WALL49_1", 0, 0);
	walltextures.back().patches.emplace_back("SW2_7", 12, 66);


	walltextures.emplace_back("SW1LION", 64, 128);

	walltextures.back().patches.emplace_back("WALL47_2", 0, 0);
	walltextures.back().patches.emplace_back("WALL42_5", 11, 62);


	walltextures.emplace_back("SW1SATYR", 64, 128);

	walltextures.back().patches.emplace_back("WALL47_2", 0, 0);
	walltextures.back().patches.emplace_back("WALL42_1", 12, 62);


	walltextures.emplace_back("SW1SKIN", 64, 128);

	walltextures.back().patches.emplace_back("HELL6_3", 0, 0);
	walltextures.back().patches.emplace_back("SW2_5", 0, 59);


	walltextures.emplace_back("SW1VINE", 64, 128);

	walltextures.back().patches.emplace_back("WALL00_1", 0, -16);
	walltextures.back().patches.emplace_back("W106_1", 0, 0);
	walltextures.back().patches.emplace_back("SW4S0", 20, 84);


	walltextures.emplace_back("SW1WOOD", 64, 128);

	walltextures.back().patches.emplace_back("WALL40_2", -64, 0);
	walltextures.back().patches.emplace_back("SW2_7", 14, 66);


	walltextures.emplace_back("SW2BLUE", 64, 128);

	walltextures.back().patches.emplace_back("COMP03_1", 0, 0);
	walltextures.back().patches.emplace_back("COMP03_2", 0, 64);
	walltextures.back().patches.emplace_back("SW2_8", 14, 66);


	walltextures.emplace_back("SW2CMT", 64, 128);

	walltextures.back().patches.emplace_back("WALL54_1", -42, 0);
	walltextures.back().patches.emplace_back("SW3S0", 16, 72);


	walltextures.emplace_back("SW2GARG", 64, 128);

	walltextures.back().patches.emplace_back("WALL47_2", 0, 0);
	walltextures.back().patches.emplace_back("WALL47_5", 12, 62);


	walltextures.emplace_back("SW2GSTON", 64, 128);

	walltextures.back().patches.emplace_back("WALL48_2", 0, 0);
	walltextures.back().patches.emplace_back("SW2_8", 13, 67);


	walltextures.emplace_back("SW2HOT", 64, 128);

	walltextures.back().patches.emplace_back("WALL49_1", 0, 0);
	walltextures.back().patches.emplace_back("SW2_8", 12, 66);


	walltextures.emplace_back("SW2LION", 64, 128);

	walltextures.back().patches.emplace_back("WALL47_2", 0, 0);
	walltextures.back().patches.emplace_back("WALL47_4", 11, 62);


	walltextures.emplace_back("SW2SATYR", 64, 128);

	walltextures.back().patches.emplace_back("WALL47_2", 0, 0);
	walltextures.back().patches.emplace_back("WALL47_3", 12, 62);


	walltextures.emplace_back("SW2SKIN", 64, 128);

	walltextures.back().patches.emplace_back("HELL6_3", 0, 0);
	walltextures.back().patches.emplace_back("SW2_6", 0, 59);


	walltextures.emplace_back("SW2VINE", 64, 128);

	walltextures.back().patches.emplace_back("WALL00_1", 0, -16);
	walltextures.back().patches.emplace_back("W106_1", 0, 0);
	walltextures.back().patches.emplace_back("SW4S1", 20, 84);


	walltextures.emplace_back("SW2WOOD", 64, 128);

	walltextures.back().patches.emplace_back("WALL40_2", -64, 0);
	walltextures.back().patches.emplace_back("SW2_8", 14, 66);


	walltextures.emplace_back("WOOD1", 256, 128);

	walltextures.back().patches.emplace_back("WALL40_1", 128, 0);
	walltextures.back().patches.emplace_back("WALL40_2", 0, 0);


	walltextures.emplace_back("WOOD3", 256, 128);

	walltextures.back().patches.emplace_back("WALL97_1", -4, 0);
	walltextures.back().patches.emplace_back("WALL97_2", 60, 0);
	walltextures.back().patches.emplace_back("WALL97_3", 124, 0);
	walltextures.back().patches.emplace_back("WALL97_2", 188, 0);
	walltextures.back().patches.emplace_back("WALL97_1", 252, 0);


	walltextures.emplace_back("WOOD4", 64, 128);

	walltextures.back().patches.emplace_back("WALL97_2", -4, 0);
	walltextures.back().patches.emplace_back("SW2_2", 22, 18);
	walltextures.back().patches.emplace_back("SW2_1", 22, 76);
	walltextures.back().patches.emplace_back("WALL97_2", 60, 0);


	walltextures.emplace_back("WOOD5", 256, 128);

	walltextures.back().patches.emplace_back("W96_1", -4, 0);
	walltextures.back().patches.emplace_back("W96_2", 124, 0);
	walltextures.back().patches.emplace_back("W96_1", 252, 0);


	walltextures.emplace_back("WOODGARG", 64, 128);

	walltextures.back().patches.emplace_back("WALL40_1", 0, 0);
	walltextures.back().patches.emplace_back("SW2_4", 6, 63);


	walltextures.emplace_back("WOODSKUL", 64, 128);

	walltextures.back().patches.emplace_back("WALL40_1", 0, 0);
	walltextures.back().patches.emplace_back("SW2_2", 24, 70);


	// End of texture definitions);



}

void LevelGenerator::AddFnCTextures()
{
	int startSize{ int(textures.size()) };
	Texture test{ "patches/FLAT20" };
	textures.emplace_back("patches/FLAT20.png");
	nameMap.insert(std::pair<std::string, int >("FLAT20", startSize));
	textures.emplace_back("patches/FLAT5_5.png");
	nameMap.insert(std::pair<std::string, int >("FLAT5_5", startSize+1));
	textures.emplace_back("patches/FLAT14.png");
	nameMap.insert(std::pair<std::string, int >("FLAT14", startSize + 2));
	textures.emplace_back("patches/FLAT18.png");
	nameMap.insert(std::pair<std::string, int >("FLAT18", startSize + 3));

	textures.emplace_back( "patches/FLOOR4_8.png" );
	nameMap.insert(std::pair<std::string, int >("FLOOR4_8", startSize + 4));
	textures.emplace_back("patches/FLOOR7_1.png");
	nameMap.insert(std::pair<std::string, int >("FLOOR7_1", startSize + 5));
	textures.emplace_back("patches/FLOOR5_1.png");
	nameMap.insert(std::pair<std::string, int >("FLOOR5_1", startSize + 6));
	textures.emplace_back("patches/FLOOR5_2.png");
	nameMap.insert(std::pair<std::string, int >("FLOOR5_2", startSize + 7));
	textures.emplace_back("patches/FLOOR1_1.png");
	nameMap.insert(std::pair<std::string, int >("FLOOR1_1", startSize + 8));

	textures.emplace_back("patches/NUKAGE3.png");
	nameMap.insert(std::pair<std::string, int >("NUKAGE3", startSize + 9));
	textures.emplace_back("patches/STEP2.png");
	nameMap.insert(std::pair<std::string, int >("STEP2", startSize + 10));


	textures.emplace_back("patches/CEIL3_5.png");
	nameMap.insert(std::pair<std::string, int >("CEIL3_5", startSize + 11));
	textures.emplace_back("patches/TLITE6_4.png");
	nameMap.insert(std::pair<std::string, int >("TLITE6_4", startSize + 12));
	textures.emplace_back("patches/F_SKY1.png");
	nameMap.insert(std::pair<std::string, int >("F_SKY1", startSize + 13));
	textures.emplace_back("patches/CEIL5_1.png");
	nameMap.insert(std::pair<std::string, int >("CEIL5_1", startSize + 14));
	textures.emplace_back("patches/TLITE6_5.png");
	nameMap.insert(std::pair<std::string, int >("TLITE6_5", startSize + 15));
	textures.emplace_back("patches/CEIL5_2.png");
	nameMap.insert(std::pair<std::string, int >("CEIL5_2", startSize + 16));
	textures.emplace_back("patches/FLOOR7_2.png");
	nameMap.insert(std::pair<std::string, int >("FLOOR7_2", startSize + 17));
	textures.emplace_back("patches/TLITE6_1.png");
	nameMap.insert(std::pair<std::string, int >("TLITE6_1", startSize + 18));
	textures.emplace_back("patches/FLOOR6_2.png");
	nameMap.insert(std::pair<std::string, int >("FLOOR6_2", startSize + 19));
	textures.emplace_back("patches/TLITE6_6.png");
	nameMap.insert(std::pair<std::string, int >("TLITE6_6", startSize + 20));
	textures.emplace_back("patches/FLAT23.png");
	nameMap.insert(std::pair<std::string, int >("FLAT23", startSize + 21));

}