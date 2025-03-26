#include "pch.h"
#include "Level.h"
#include <iostream>
#include "Plane.h"
#include "PlaneIregular.h"
#include <vector>
#include <cassert>
#include "DoomCamera.h"

Level::Level()
	:m_pWallTexture {}
{

}

Level::~Level () {
	// some instructions
	delete[] m_pPlanes;
	delete[] m_pFloorsNCeilings;
	delete m_pWallTexture;
	delete m_pFloorTexture;
}

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

void Level::UpdateLevel( DoomCamera& eyeObject, float elapsedSec)
{
	m_interval += elapsedSec;
	for (int i = 0; i < m_PlaneAmount; ++i) {
		//planes[i].PlaneCollision(Eye);
		m_pPlanes[i].UpdatePlane(eyeObject);
		m_pPlanes[i].PlaneCollision(eyeObject);
	}
	for (int i = 0; i < m_floorsNCeilingsAmount; ++i) {
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

void Level::InitializeWallls()
{
	const int SCALE{ 1 };
	//TextureFromFile("Resources/WALL03_1.png", m_pwallTexture);
	m_pWallTexture = new Texture("WALL03_1_converted.png");
	m_pFloorTexture = new Texture("FLOOR4_8_converted.png");

	// loading the source files
	// IMPORTANT | unimportant/not implemented ATM

	// vertexes.
	//		2	   |	2
	//  X POSITION | Y POSITION
	const int VERTEXCOLUMS{ 2 }; // 2 * bytepairs = 2
	const LMP vertexesImported{ utils::ReadLMP("VERTEXES.lmp", VERTEXCOLUMS) };

	// lines. defines the walls/doors
	//   2		|    2	   |   2   |       2		|     2		 |     2	  |   2
	// VERTEX 1 | VERTEX 2 | flags | action special | sector tag | FRONT SIDE | BACK SIDE
	const int LINECOLUMS{ 7 }; // 7 * bytepairs = 7
	const LMP linesImported{ utils::ReadLMP("LINEDEFS.lmp", LINECOLUMS) };

	// sides. defines what the walls look like + sector 
	//    2	    |    2	   |    8	   |     8	   |     8		|   2
	// x offset | y offset | UPPER TEX | LOWER TEX | MIDDLE TEX | SECTORS
	const int SIDECOLUMS{ 15 }; // (3 * bytepair) + (3 * ( 4 * bytepair))  = 15
	const LMP sidesImported{ utils::ReadLMP("SIDEDEFS.lmp", SIDECOLUMS) };

	// sectors. sector def and visualization
	//     2	   |    2		   | 8		   |  8		  |  2			|   2	  | 2
	// FLOOR HEIGH | CEILING HEIGH | FLOOR TEX | CEIL TEX | LIGHT LEVEL | special | tag
	//fun fact, properly defining these gave me a breakthrough in an issue i had
	const int SECTORCOLUMS{ 13 }; // (5 * bytepair) + (2 * ( 4 * bytepair))  = 13
	const LMP sectorsImported{ utils::ReadLMP("SECTORS.lmp", SECTORCOLUMS) };
	
	// sectors. sector def and visualization
	//     2	   |    2		 |		2	|   2	  |  2		 |   2		   |	2	   |	2	   |    2	  |		2		|   2		|	2		 |   2		   | 2 
	// partition x | partition y | p x diff | p y diff| rBox top | rBox bottom | rBox left | rBox right| lBox top | lBox bottom | lBox left | lBox right | right child | left child
	const int NODESCULOMS{14}; // 14 bytepairs. | 4 bytepairs + 2 * (4 bytepairs (bounding boxes)) + 2 bytepairs
	const LMP nodesImported{ utils::ReadLMP("NODES.lmp", NODESCULOMS) };
	//m_LevelBSP.reserve(nodesImported.rows);
	m_BSP.resize(nodesImported.rows);
	// from this, i can get an exxtra set of vertexes.
	// so i first will have to check if a sub sector makes a loop. if it does not make a loopp, add the devision line's vertees?
	

	

	// segments. line segments of the roof/floor polygons
	//    2		|    2	   | 2	   |  2	  |  2	 |   2
	// VERTEX 1 | VERTEX 2 | angle | LINE | SIDE | offset 
	const int SEGSCOLUMS{ 6 }; // 6 * bytepair = 6
	const LMP segsImported{ utils::ReadLMP("SEGS.lmp", SEGSCOLUMS) };

	m_PlaneAmount = segsImported.rows;
	m_pPlanes = new Plane[m_PlaneAmount];

	//check if each seg can be matched to a linedef
	for (int seg{}; seg < segsImported.rows; seg++) {
		//segsImported.data
	}
	// it HAS which line it conects to AND if it takes the front side or back side
	// so from seg, we can get line and relativeSide.
	//from lines and relativeSide we can get side
	//from side we can get sector
	//from sector we can get height

	// segments for each sector
	//    2		 |    2	  
	// SEG COUNT | FIRST SEG
	const int SECTORSEGSCOLUMS{ 2 }; // 2 * bytepair = 2
	const LMP sectorSegsImported{ utils::ReadLMP("SSECTORS.lmp", SECTORSEGSCOLUMS) };
	
	m_BSPleafs.resize(sectorSegsImported.rows);

	int* sectorPerSubsector{ new int[sectorSegsImported.rows * 2] };
	//int* pNodePerSector{ new int[sectorSegsImported.rows] };
	// save what node belongs to each sub sector
	for (int i{}; i < nodesImported.rows; i++) {
		for (int j{12}; j < 14; j++) { // for the left and right hand side of the node
			int16_t value{ nodesImported.data[i][j] };
			int16_t sectorVal{ value };
			if (value < 0) {
				sectorVal = utils::ReadNodeField(value);
				//pNodePerSector[sectorVal] = i;
				
			}
		}
	}


	//int** pVertexesPerSector{ new int* [sectorSegsImported.rows]};
	//int* pVertexAmountPerSector{ new int[sectorSegsImported.rows] };
	m_pFloorsNCeilings = new PlaneIregular [sectorSegsImported.rows * 2] ;
	
	Vector2f* pLines{ new Vector2f[linesImported.rows] }; // Allocate rows
	for (int i = 0; i < linesImported.rows; ++i) {
		pLines[i].x = linesImported.data[i][0];
		pLines[i].y = linesImported.data[i][1];
	}


	int wallamount{};
	for (int line = 0; line < linesImported.rows; ++line) {
		// okay, so basicaly, if it has a backside, it means it is part of 2 secorts. 
		// then, depending on which parts get a texture, the middle, upper or lower part need to be made. 
		// th upper part is made by looking at the ceining heights, the loer part by looking at the floor heights. 
		// the middle part is the one in etween.


		bool twoSectored{};
		if (linesImported.data[line][6] != -1) {
			twoSectored = true;
		}

		int sideDef{ linesImported.data[line][5] };
		int sector{ sidesImported.data[sideDef][14] };

		std::string charString1 = utils::convertToCharString(sidesImported.data[sideDef][2],
			sidesImported.data[sideDef][3], sidesImported.data[sideDef][4], sidesImported.data[sideDef][5]);
		std::string charString2 = utils::convertToCharString(sidesImported.data[sideDef][6],
			sidesImported.data[sideDef][7], sidesImported.data[sideDef][8], sidesImported.data[sideDef][9]);
		std::string charString3 = utils::convertToCharString(sidesImported.data[sideDef][10],
			sidesImported.data[sideDef][11], sidesImported.data[sideDef][12], sidesImported.data[sideDef][13]);

		// top
		if (charString1.find("-") == std::string::npos)
		{
			int sideDef2{ linesImported.data[line][5 + 1] };
			int sector2{ sidesImported.data[sideDef2][14] };

			float ceilingHeight{ static_cast<float>(sectorsImported.data[sector][1]) / SCALE };
			float ceilingHeight2{ static_cast<float>(sectorsImported.data[sector2][1]) / SCALE };

			m_LinePerWall.push_back(line);
			AddPlane(pLines, line, wallamount, ceilingHeight2, ceilingHeight, vertexesImported.data, SCALE);
			//g_Sectors[wallamount] = sector;
			wallamount++;
		}
		// middle
		if (charString3.find("-") == std::string::npos && twoSectored)
		{
			int sideDef2{ linesImported.data[line][5 + 1] };
			int sector2{ sidesImported.data[sideDef2][14] };

			float floorHeight{ static_cast<float>(sectorsImported.data[sector][0]) / SCALE };
			float ceilingHeight{ static_cast<float>(sectorsImported.data[sector][1]) / SCALE };
			float floorHeight2{ static_cast<float>(sectorsImported.data[sector2][0]) / SCALE };
			float ceilingHeight2{ static_cast<float>(sectorsImported.data[sector2][1]) / SCALE };

			float floorHeight2Use{ floorHeight };
			if (floorHeight < floorHeight2) {
				floorHeight2Use = floorHeight2;
			}
			float ceilingHeight2Use{ ceilingHeight };
			if (ceilingHeight > ceilingHeight2) {
				ceilingHeight2Use = ceilingHeight2;
			}
			m_LinePerWall.push_back(line);
			AddPlane(pLines, line, wallamount, floorHeight2Use, ceilingHeight2Use, vertexesImported.data, SCALE);
			//g_Sectors[wallamount] = sector;
			wallamount++;
		}
		else if (charString3.find("-") == std::string::npos) {
			float floorHeight{ static_cast<float>(sectorsImported.data[sector][0]) / SCALE };
			float ceilingHeight{ static_cast<float>(sectorsImported.data[sector][1]) / SCALE };
			m_LinePerWall.push_back(line);
			AddPlane(pLines, line, wallamount, floorHeight, ceilingHeight, vertexesImported.data, SCALE);
			//g_Sectors[wallamount] = sector;
			wallamount++;
		}
		// bottom
		if (charString2.find("-") == std::string::npos)
		{
			int sideDef2{ linesImported.data[line][5 + 1] };
			int sector2{ sidesImported.data[sideDef2][14] };

			float floorHeight{ static_cast<float>(sectorsImported.data[sector][0]) / SCALE };
			float floorHeight2{ static_cast<float>(sectorsImported.data[sector2][0]) / SCALE };
			m_LinePerWall.push_back(line);
			AddPlane(pLines, line, wallamount, floorHeight2, floorHeight, vertexesImported.data, SCALE);
			//g_Sectors[wallamount] = sector;
			wallamount++;
		}
	}

	//make the first bounding box

	//     0	   |    1		 |		2	|   3	  |  4		 |   5		   |	6	   |	7	   |    8	  |		9		|   10		|	11		 |   12		   | 13 
	// partition x | partition y | p x diff | p y diff| rBox top | rBox bottom | rBox left | rBox right| lBox top | lBox bottom | lBox left | lBox right | right child | left child

	const int buffer{ 1000 };
	float top = nodesImported.data[nodesImported.rows - 1][8] + buffer;
	float bottom = nodesImported.data[nodesImported.rows - 1][5] - buffer;
	float left = nodesImported.data[nodesImported.rows - 1][6] - buffer;
	float right = nodesImported.data[nodesImported.rows - 1][11] + buffer;


	std::vector<Vector2f> startingBox{ Vector2f{left, top},Vector2f{right, top},Vector2f{right, bottom },Vector2f{left, bottom}};
	std::vector<Line> sliceLines;
	ConstructSubSectors(startingBox, sliceLines,
		nodesImported, nodesImported.rows - 1, sectorSegsImported, vertexesImported,
		segsImported, linesImported, sidesImported, sectorsImported,
		SCALE, sectorPerSubsector);

	SeamFixSubsectors(sectorPerSubsector, sectorSegsImported, vertexesImported,
		segsImported, linesImported, sidesImported);

	for (int wallIndex{}; wallIndex < m_LinePerWall.size(); wallIndex++)
	{
		for (int leafnode{}; leafnode < m_BSPleafs.size(); leafnode++)
		{
			if (m_LinePerWall[wallIndex] == m_BSPleafs[leafnode].lineIndex)
			{
				m_BSPleafs[leafnode].wallIndexes.push_back(wallIndex);
			}
		}
	}

	/*for (int sector{}; sector < sectorSegsImported.rows; sector++) {
		delete[] pVertexesPerSector[sector];
	}
	delete[] pVertexesPerSector;
	delete[] pVertexAmountPerSector;*/

	//delete[] pNodePerSector;

	delete[] sectorPerSubsector;
	delete[] pLines;
	pLines = nullptr;
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
		const int leafnode{ utils::ReadNodeField(child) };
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

void Level::AddPlane(const Vector2f pSides[], const int lineIndex, const int wallIndex,
	const float floorHeight, const float ceilingHeight, const int16_t* const* pVertexesImported, const int scale)
{
	int vertex1 = pSides[lineIndex].x;
	int vertex2 = pSides[lineIndex].y;
	//const int SCALE{ 10 };
	float wallHeight{ ceilingHeight - floorHeight };

	Vector3f vertex1Loc{ static_cast<float>(pVertexesImported[vertex1][0]) / scale, static_cast<float> (pVertexesImported[vertex1][1]) / scale };
	Vector3f vertex2Loc{ static_cast<float>(pVertexesImported[vertex2][0]) / scale, static_cast<float> (pVertexesImported[vertex2][1]) / scale };

	Vector3f width{ vertex2Loc.x - vertex1Loc.x,  vertex2Loc.y - vertex1Loc.y , 0 };
	Vector3f height{ 0,0,wallHeight };
	Vector3f location{ vertex1Loc.x, vertex1Loc.y, floorHeight };
	//Plane plane2{ positionPlanea, widtha, heighta, 2 };
	Plane input{ location, width, height, 2 };
	m_pPlanes[wallIndex] = input;

}


void Level::ConstructSubSectors(const std::vector<Vector2f>& boundingBox, std::vector<Line>& sliceLines,
	const LMP& nodes, int startNode, const LMP& subsectors, const LMP& vertexes,
	const LMP& segs, const LMP& lines, const LMP& sides, const LMP& sectors,
	const int SCALE, int* sectorPerSubsector)
{
	//     0	   |    1		 |	  2		|   3	  |
	// partition x | partition y | p x diff | p y diff|
	//   4		|     5		  |	    6	  |		7	  |
	// rBox top | rBox bottom | rBox left | rBox right|
	//    8		|		9	  |    10	  |		11	   |
	// lBox top | lBox bottom | lBox left | lBox right |
	//		12	   |	13 
	// right child | left child

	// we need for each subsector an array of vector2f's
	// we need a vector of vector2f's along which all the points will be handled

	Vector2f partitionPoint1{ float(nodes.data[startNode][0]), float(nodes.data[startNode][1]) };
	Vector2f partitionPoint2{ float(partitionPoint1.x + nodes.data[startNode][2]), float(partitionPoint1.y + nodes.data[startNode][3]) };

	//we can pass on the slicelines,but what about the sides?
	//

	int childIndex = 12;
	for (int i{}; i < 2; i++)
	{
		int child{ nodes.data[startNode][childIndex + i] };

		if (i == 0)
		{
			sliceLines.push_back(Line{ partitionPoint1, partitionPoint2 });
		}
		else
		{
			sliceLines.push_back(Line{ partitionPoint2, partitionPoint1 });
		}
		m_BSP[startNode].partitionLine = Line{ partitionPoint1, partitionPoint2 };
		m_BSP[startNode].children[i] = child;
		
		if (child < 0)
		{

			// it's a subSector
			const int subSector{ utils::ReadNodeField(child) };

			/*if (subSector == 34)
			{*/

			std::vector<Vector2f> boundingBoxParent{};
			for (int vertex{}; vertex < boundingBox.size(); vertex++)
			{
				boundingBoxParent.push_back(boundingBox[vertex]);
			}
			//std::cout << std::endl << " arived at subsector " << subSector << std::endl;

			for (int line{}; line < sliceLines.size(); line++)
			{
				std::vector<Vector2f> boundingBoxChild{};
				sliceLine(boundingBoxParent, boundingBoxChild, sliceLines[line].startPoint, sliceLines[line].endPoint);
				boundingBoxParent.clear();

				//std::cout << std::endl << line << std::endl;
				//std::cout << sliceLines[line].startPoint << " , " << sliceLines[line].endPoint << std::endl;

				for (int vertex{}; vertex < boundingBoxChild.size(); vertex++)
				{
					boundingBoxParent.push_back(boundingBoxChild[vertex]);
				}
			}
			//now we wanna generate the actual plane.
			std::vector<Vector2f> boundingBoxChild{};
			SliceSegs(boundingBoxParent, boundingBoxChild, subSector, subsectors, segs, vertexes);

			int seg = subsectors.data[subSector][1];
			//if (subSector == 15) {
				//std::cout << " seg " << seg << std::endl;
			//}
			int lineI = segs.data[seg][3];
			
			int orienttion = segs.data[seg][4]; //these are curently magic numbers. they are derived from the file structure, im thinking of putting them in an enumerator
			int sideIndex{ 5 };
			if (orienttion > 0)
			{ // 0 : front side | 1 : backside
				sideIndex = 6;
			}
			int side = lines.data[lineI][sideIndex];
			int sector = sides.data[side][14];

			sectorPerSubsector[m_floorsNCeilingsAmount] = sector;
			sectorPerSubsector[m_floorsNCeilingsAmount + 1] = sector;

			for (int vertex{}; vertex < boundingBoxChild.size(); vertex++)
			{
				boundingBoxChild[vertex].x = boundingBoxChild[vertex].x / SCALE;
				boundingBoxChild[vertex].y = boundingBoxChild[vertex].y / SCALE;
			}
			/*for (int vertex{}; vertex < boundingBox.size(); vertex++)
			{
				boundingBoxParent.push_back(Vector2f{ boundingBoxParent[vertex].x / SCALE, boundingBoxParent[vertex].y / SCALE });
			}*/
			float ceilingHeight = sectors.data[sector][1] / SCALE;
			float floorHeight = sectors.data[sector][0] / SCALE;
			m_BSPleafs[subSector].lineIndex = lineI;
			m_BSPleafs[subSector].floorheight = floorHeight;
			PlaneIregular input{ boundingBoxChild, sector, int(ceilingHeight) };
			PlaneIregular input2{ boundingBoxChild, 3, int(floorHeight) };
			m_pFloorsNCeilings[m_floorsNCeilingsAmount] = input;
			m_pFloorsNCeilings[m_floorsNCeilingsAmount + 1] = input2;

			m_floorsNCeilingsAmount += 2;



				
			//}
		}
		else
		{
			ConstructSubSectors(boundingBox, sliceLines, nodes, child,
				subsectors, vertexes, segs, lines, sides, sectors,
				SCALE, sectorPerSubsector);
		}

		sliceLines.pop_back();
	}
}

void Level::SeamFixSubsectors(const int* sectorPerSubsector, const LMP& subsectors, const LMP& vertexes,
	const LMP& segs, const LMP& lines, const LMP& sides)
{
	for (int subsectorThis{}; subsectorThis < m_floorsNCeilingsAmount; subsectorThis++) //we go over all the subsectors
	{
		PlaneIregular& This{ m_pFloorsNCeilings[subsectorThis] };
		int seg = subsectors.data[subsectorThis/2][1];
		//if (subSector == 15) {
			//std::cout << " seg " << seg << std::endl;
		//}
		int lineI = segs.data[seg][3];

		int orienttion = segs.data[seg][4]; //these are curently magic numbers. they are derived from the file structure, im thinking of putting them in an enumerator
		int sideIndex{ 5 };
		if (orienttion > 0)
		{ // 0 : front side | 1 : backside
			sideIndex = 6;
		}
		int side = lines.data[lineI][sideIndex];
		int sector = sides.data[side][14]; // get the sector that the subsector belongs to
		int sectorThis{ sectorPerSubsector[subsectorThis] };
		for (int subSectorNeighbor{}; subSectorNeighbor < m_floorsNCeilingsAmount; subSectorNeighbor++)
		{
			if (subsectorThis % 1 == subSectorNeighbor % 1)
			{
				int sectorNeighbor = sectorPerSubsector[subSectorNeighbor]; // get the other's subsector
				if (sectorThis == sectorNeighbor) // if they match, we need to start comparing points
				{
					// so, the sector is the same.
					// now we gotta check if the points are on the edges of the subsector
					// if they are on the subsector edges, add them in between

					PlaneIregular Neighbor{ m_pFloorsNCeilings[subSectorNeighbor] };

					int amountOfPointsNeighbor{ Neighbor.GetAmountPoints() };

					for (int indexNeighbor{}; indexNeighbor < amountOfPointsNeighbor; indexNeighbor++)
					{
						point2D_3D curPointNeighbor{ Neighbor.GetPoint(indexNeighbor) };
						Vector2f curPointNeighbor_XZ{ curPointNeighbor.point3D.x, curPointNeighbor.point3D.y };
						int popIndex{ -1 };

						int amountOfPointsThis{ This.GetAmountPoints() };
						for (int indexThis{}; indexThis < amountOfPointsThis; indexThis++)
						{
							point2D_3D curPointThis{ This.GetPoint(indexThis) };
							Vector2f curPointThis_XZ{ curPointThis.point3D.x, curPointThis.point3D.y };


							int nextIndexThis{ utils::getArrIndex(amountOfPointsThis, indexThis + 1) };
							point2D_3D nextPointThis{ This.GetPoint(nextIndexThis) };
							Vector2f nextPointThis_XZ{ nextPointThis.point3D.x, nextPointThis.point3D.y };
							if (curPointThis_XZ == nextPointThis_XZ) {
								popIndex = indexThis;
								continue;
							}
							else
							{
								//assert(nextPointThis_XZ == curPointNeighbor_XZ);

														//if (curPointNeighbor_XZ.OnSegment(curPointThis_XZ, nextPointThis_XZ))
								if (curPointNeighbor_XZ.orientation(curPointThis_XZ, nextPointThis_XZ) == 0 && curPointNeighbor_XZ.OnSegment(curPointThis_XZ, nextPointThis_XZ))
								{
									if (curPointNeighbor_XZ != curPointThis_XZ && curPointNeighbor_XZ != nextPointThis_XZ)
									{
										//std::cout << curPointThis.point3D << "is the curent point" << std::endl;
										//std::cout << nextPointThis.point3D << "is the next point" << std::endl;
										//if so, we want to add the point to the array in between these 2 points;
										curPointNeighbor.point3D.z = curPointThis.point3D.z;
										//std::cout << curPointNeighbor.point3D << "is the newly added point" << std::endl << std::endl;
										This.insertPoint(nextIndexThis, curPointNeighbor);
										break;
									}

								}
							}

						}
						// check it against the edges
						if (popIndex >= 0)
						{
							This.PopPoint(popIndex);
						}
					}
				}
			}
			
		}
	}

	//m_floorsNCeilingsAmount += 2;
}

void Level::sliceLine(const std::vector<Vector2f>& boundingBoxOriginal, std::vector<Vector2f>& boundingBoxNew,
	const Vector2f& partitionPoint1, const Vector2f& partitionPoint2, bool keepEdgePoints) {
	for (int point{}; point < boundingBoxOriginal.size(); point++)
	{
		// we make a new vector to store the child points


		// we take a point from the bounding box
		Vector2f segPoint1 = boundingBoxOriginal[point];
		//std::cout << boundingBoxOriginal[point] << " is first point " << std::endl;

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
			if (partPoint1.OnSegment(segPoint1, segPoint2))
			{
				boundingBoxNew.push_back(partPoint1);
			}
		}
		

		//if they intersect, add the intersectionPoint to the child bounding box.
		//all intersection pointys should always be added.
		if (intersectsWithPartition) {
			if (intersectionPoint.OnSegment(segPoint1, segPoint2))
			{
				boundingBoxNew.push_back(intersectionPoint);
			}
		}

		if (keepEdgePoints) {
			Vector2f partPoint2{ partitionPoint2 };
			if (partPoint2.OnSegment(segPoint1, segPoint2))
			{
				boundingBoxNew.push_back(partPoint2);
			}
		}
	}
}


//for a child, we need to slice it with all the segments
void Level::SliceSegs(std::vector<Vector2f>& boundingBox, std::vector<Vector2f>& boundingBoxSubSector, int subSector, const LMP& subsectors, const LMP& segs, const LMP& vertexes)
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
