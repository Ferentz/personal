#include "pch.h"
#include "Level.h"
#include <iostream>
#include "Plane.h"
#include "PlaneIregular.h"
#include <vector>
#include <cassert>

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
	//m_subSectorToDraw
	for (int i = 0; i < m_floorsNCeilingsAmount; ++i) {
	//for (int i = m_subSectorToDraw *2; i < m_subSectorToDraw * 2 + 2; ++i) {
		//planes[i].PlaneCollision(Eye);
		m_pFloorsNCeilings[i].DrawPlane(*m_pFloorTexture);
	}
}

void Level::UpdateLevel(const DoomCamera& eyeObject, float elapsedSec)
{
	//std::cout << m_interval << std::endl;
	m_interval += elapsedSec;
	for (int i = 0; i < m_PlaneAmount; ++i) {
		//planes[i].PlaneCollision(Eye);
		m_pPlanes[i].UpdatePlane(eyeObject);
	}
	for (int i = 0; i < m_floorsNCeilingsAmount; ++i) {
	//for (int i = m_subSectorToDraw * 2; i < m_subSectorToDraw * 2 + 2; ++i) {
		//planes[i].PlaneCollision(Eye);
		m_pFloorsNCeilings[i].UpdatePlane(eyeObject);
	}

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
	const int SCALE{ 10 };
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
	//for (int subSector{}; subSector < sectorSegsImported.rows; subSector++)
	//{
	//	int potentialVertexAmount{ sectorSegsImported.data[subSector][0] * 2 };
	//	int* pVertexes{ new int [potentialVertexAmount] };
	//	int curIndex{};
	//	for (int index{}; index < potentialVertexAmount; index++)
	//	{
	//		pVertexes[index] = -1;
	//	}
	//	int segCount = sectorSegsImported.data[subSector][0];
	//	int startSeg = sectorSegsImported.data[subSector][1];
	//	//int curSeg{ startSeg };
	//	for (int seg{}; seg < segCount; seg++)
	//	{
	//		int vertexes[2]{};
	//		bool isFound[2]{};
	//		int vertexPos[]{-1, -1};
	//		vertexes[0] = segsImported.data[startSeg + seg][0];
	//		vertexes[1] = segsImported.data[startSeg + seg][1];

	//		for (int i = 0; i < 2; i++)
	//		{ // we check if either vertex already exists, and if so, where in the array it exists
	//			int vertexToCheck = vertexes[i];
	//			for (int index{}; index < potentialVertexAmount; index++)
	//			{
	//				if (pVertexes[index] == vertexToCheck)
	//				{
	//					isFound[i] = true;
	//					vertexPos[i] = index;
	//				}
	//			}
	//			
	//		}
	//		

	//		if (isFound[0] != isFound[1]) // if only one is true
	//		{
	//			//we want to check which one,
	//			int insertPos{ 0 };
	//			int vertexToInsert{};
	//			if (isFound[0])
	//			{
	//				// and after if it's th efirst one
	//				insertPos = vertexPos[0] + 1;
	//				vertexToInsert = vertexes[1];
	//			}
	//			else
	//			{
	//				//insert it before if it's the second one,
	//				insertPos = vertexPos[1];
	//				vertexToInsert = vertexes[0];
	//			}

	//			for (int index{ potentialVertexAmount - 1 }; index >= insertPos; index--)
	//			{
	//				pVertexes[index] = pVertexes[index - 1]; // starting from the back, we insert the value in front
	//			}
	//			pVertexes[insertPos] = vertexToInsert;
	//			curIndex++;

	//		}
	//		else if (isFound[0] == false) //if not only one is true, and one is false, meaning both are false
	//		{
	//			for (int i = 0; i < 2; i++)
	//			{
	//				pVertexes[curIndex] = vertexes[i];
	//				curIndex++;
	//			}

	//		}
	//	}

	//	pVertexesPerSector[subSector] = pVertexes;
	//	pVertexAmountPerSector[subSector] = curIndex;
	//}

	/*ConstructFloor(sectorSegsImported, segsImported, linesImported, sidesImported, sectorsImported, vertexesImported, 
		nodesImported, pNodePerSector,
		SCALE, pVertexAmountPerSector, pVertexesPerSector);*/

	// so each line says for that sector ho many segs it has and at hat seg it starts
	// for each sector
		// loop for seg count
			//first vec of the segs added
	

	Vector2f* pLines{ new Vector2f[linesImported.rows] }; // Allocate rows
	for (int i = 0; i < linesImported.rows; ++i) {
		pLines[i].x = linesImported.data[i][0];
		pLines[i].y = linesImported.data[i][1];
	}


	int wallamount{};
	for (int i = 0; i < linesImported.rows; ++i) {
		// okay, so basicaly, if it has a backside, it means it is part of 2 secorts. 
		// then, depending on which parts get a texture, the middle, upper or lower part need to be made. 
		// th upper part is made by looking at the ceining heights, the loer part by looking at the floor heights. 
		// the middle part is the one in etween.


		bool twoSectored{};
		if (linesImported.data[i][6] != -1) {
			twoSectored = true;
		}

		int sideDef{ linesImported.data[i][5] };
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
			int sideDef2{ linesImported.data[i][5 + 1] };
			int sector2{ sidesImported.data[sideDef2][14] };

			float ceilingHeight{ static_cast<float>(sectorsImported.data[sector][1]) / SCALE };
			float ceilingHeight2{ static_cast<float>(sectorsImported.data[sector2][1]) / SCALE };

			AddPlane(pLines, i, wallamount, ceilingHeight2, ceilingHeight, vertexesImported.data, SCALE);
			//g_Sectors[wallamount] = sector;
			wallamount++;
		}
		// middle
		if (charString3.find("-") == std::string::npos && twoSectored)
		{
			int sideDef2{ linesImported.data[i][5 + 1] };
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

			AddPlane(pLines, i, wallamount, floorHeight2Use, ceilingHeight2Use, vertexesImported.data, SCALE);
			//g_Sectors[wallamount] = sector;
			wallamount++;
		}
		else if (charString3.find("-") == std::string::npos) {
			float floorHeight{ static_cast<float>(sectorsImported.data[sector][0]) / SCALE };
			float ceilingHeight{ static_cast<float>(sectorsImported.data[sector][1]) / SCALE };
			AddPlane(pLines, i, wallamount, floorHeight, ceilingHeight, vertexesImported.data, SCALE);
			//g_Sectors[wallamount] = sector;
			wallamount++;
		}
		// bottom
		if (charString2.find("-") == std::string::npos)
		{
			int sideDef2{ linesImported.data[i][5 + 1] };
			int sector2{ sidesImported.data[sideDef2][14] };

			float floorHeight{ static_cast<float>(sectorsImported.data[sector][0]) / SCALE };
			float floorHeight2{ static_cast<float>(sectorsImported.data[sector2][0]) / SCALE };

			AddPlane(pLines, i, wallamount, floorHeight2, floorHeight, vertexesImported.data, SCALE);
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


void Level::ConstructFloor(const LMP& subSectorData , const LMP& segData, const LMP& lineData,
	const LMP& sideData, const LMP& sectordata, const LMP& vertexData, const LMP& nodesArr, const int* nodePerSubSector,
	const float scale, int* const pVertexAmountPerSubSector, int* const * const pVertexesPerSubSector) {

	// it HAS which line it conects to AND if it takes the front side or back side
	// so from seg, we can get line and relativeSide.
	//from lines and relativeSide we can get side
	//from side we can get sector
	//from sector we can get height

	for (int subSector{}; subSector < subSectorData.rows; subSector++)
	{
		//if (subSector == 15) {
			//std::cout << "sub sector " << subSector << std::endl;
		//}
		int seg = subSectorData.data[subSector][1];
		//if (subSector == 15) {
			//std::cout << " seg " << seg << std::endl;
		//}
		int line = segData.data[seg][3];
		int orienttion = segData.data[seg][4]; //these are curently magic numbers. they are derived from the file structure, im thinking of putting them in an enumerator
		int sideIndex{ 5 };
		if (orienttion > 0) { // 0 : front side | 1 : backside
			sideIndex = 6;
		}
		int side = lineData.data[line][sideIndex];
		int sector = sideData.data[side][14];

		int ceilingHeight = sectordata.data[sector][1] / scale;
		int floorHeight = sectordata.data[sector][0] / scale;

		const int amount{ pVertexAmountPerSubSector[subSector] + 2 };
		Vector2f* vertexes{ new Vector2f[amount] };

		for (int vertex{}; vertex < amount - 2; vertex++) {
			int vertexIndex{ pVertexesPerSubSector[subSector][vertex] };
			vertexes[vertex] = Vector2f{ vertexData.data[vertexIndex][0] / scale,static_cast<float> (vertexData.data[vertexIndex][1]) / scale };
			/*if (subSector == 6) {
				std::cout << vertexes[vertex].x << " " << vertexes[vertex].y << std::endl;
			}*/
		}

		int node{ nodePerSubSector[subSector] };
		//std::cout << node << " : sS " << subSector << std::endl;
		float startX{ float(nodesArr.data[node][0]) };
		float startY{ float(nodesArr.data[node][1]) };
		int xOffset{ nodesArr.data[node][2] };
		int yOffset{ nodesArr.data[node][2] };
		vertexes[amount - 2] = Vector2f{ startX / scale, startY / scale };
		vertexes[amount - 1] = Vector2f{ (startX + xOffset) / scale, (startY + yOffset) / scale };
		/*if (subSector == 6) {
			std::cout << vertexes[amount - 2].x << " " << vertexes[amount - 2].y << std::endl;
			std::cout << vertexes[amount - 1].x << " " << vertexes[amount - 1].y << std::endl;
		}*/

		PlaneIregular input{ vertexes, pVertexAmountPerSubSector[subSector], 2, int(ceilingHeight) };
		PlaneIregular input2{ vertexes, pVertexAmountPerSubSector[subSector], 3, int(floorHeight) };
		m_pFloorsNCeilings[2 * subSector] = input;
		m_pFloorsNCeilings[2 * subSector + 1] = input2;
		m_floorsNCeilingsAmount += 2;
		delete[] vertexes;
		vertexes = nullptr;

	}
}

void Level::ConstructSubSectors(const std::vector<Vector2f>& boundingBox, std::vector<Line>& sliceLines,
	const LMP& nodes, int startNode, const LMP& subsectors, const LMP& vertexes,
	const LMP& segs, const LMP& lines, const LMP& sides, const LMP& sectors,
	const int SCALE, int* sectorPerSubsector)
{
	//std::cout << std::endl;
	//std::cout << " going into node " << startNode << std::endl;
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
		if (i == 0)
		{
			sliceLines.push_back(Line{ partitionPoint1, partitionPoint2 });
		}
		else
		{
			sliceLines.push_back(Line{ partitionPoint2, partitionPoint1 });
		}
		int child{ nodes.data[startNode][childIndex + i] };
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
			int ceilingHeight = sectors.data[sector][1] / SCALE;
			int floorHeight = sectors.data[sector][0] / SCALE;
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

//int Level::ConstructSubSectors(const std::vector<Vector2f>& boundingBox, std::vector<Line>& sliceLines,
//	const LMP& nodes, int startNode, const LMP& subsectors, const LMP& vertexes,
//	const LMP& segs, const LMP& lines, const LMP& sides, const LMP& sectors,
//	const int SCALE)
//{
//	//std::cout << std::endl;
//	//std::cout << " going into node " << startNode << std::endl;
//	//     0	   |    1		 |	  2		|   3	  |
//	// partition x | partition y | p x diff | p y diff|
//	//   4		|     5		  |	    6	  |		7	  |
//	// rBox top | rBox bottom | rBox left | rBox right|
//	//    8		|		9	  |    10	  |		11	   |
//	// lBox top | lBox bottom | lBox left | lBox right |
//	//		12	   |	13 
//	// right child | left child
//
//	// we need for each subsector an array of vector2f's
//	// we need a vector of vector2f's along which all the points will be handled
//
//	Vector2f partitionPoint1{ float(nodes.data[startNode][0]), float(nodes.data[startNode][1]) };
//	Vector2f partitionPoint2{ float(partitionPoint1.x + nodes.data[startNode][2]), float(partitionPoint1.y + nodes.data[startNode][3]) };
//	
//	//we can pass on the slicelines,but what about the sides?
//	//
//
//	int childIndex = 12;
//	for (int i{}; i < 2; i++)
//	{
//		if (i == 0)
//		{
//			sliceLines.push_back(Line{ partitionPoint1, partitionPoint2 });
//		}
//		else
//		{
//			sliceLines.push_back(Line{ partitionPoint2, partitionPoint1 });
//		}
//		int child{ nodes.data[startNode][childIndex + i] };
//		if (child < 0)
//		{
//			
//			// it's a subSector
//			int subSector = utils::ReadNodeField(child);
//
//			if (subSector == 15)
//			{
//
//				std::vector<Vector2f> boundingBoxParent{};
//				for (int vertex{}; vertex < boundingBox.size(); vertex++)
//				{
//					boundingBoxParent.push_back(boundingBox[vertex]);
//				}
//				std::cout << std::endl << " arived at subsector " << subSector << std::endl;
//
//				for (int line{}; line < sliceLines.size(); line++)
//				{
//					std::cout << std::endl << line << std::endl;
//					std::cout << sliceLines[line].startPoint << " , " << sliceLines[line].endPoint << std::endl;
//					std::vector<Vector2f> boundingBoxChild{};
//					sliceLine(boundingBoxParent, boundingBoxChild, sliceLines[line].startPoint, sliceLines[line].endPoint);
//					boundingBoxParent.clear();
//
//					
//
//					for (int vertex{}; vertex < boundingBoxChild.size(); vertex++)
//					{
//						boundingBoxParent.push_back(boundingBoxChild[vertex]);
//					}
//
//					//now we wanna generate the actual plane.
//
//					int seg = subsectors.data[subSector][1];
//					//if (subSector == 15) {
//						//std::cout << " seg " << seg << std::endl;
//					//}
//					int lineI = segs.data[seg][3];
//					int orienttion = segs.data[seg][4]; //these are curently magic numbers. they are derived from the file structure, im thinking of putting them in an enumerator
//					int sideIndex{ 5 };
//					if (orienttion > 0)
//					{ // 0 : front side | 1 : backside
//						sideIndex = 6;
//					}
//					int side = lines.data[lineI][sideIndex];
//					int sector = sides.data[side][14];
//
//					for (int vertex{}; vertex < boundingBoxChild.size(); vertex++)
//					{
//						boundingBoxChild[vertex].x = boundingBoxChild[vertex].x / SCALE;
//						boundingBoxChild[vertex].y = boundingBoxChild[vertex].y / SCALE;
//					}
//					/*for (int vertex{}; vertex < boundingBox.size(); vertex++)
//					{
//						boundingBoxParent.push_back(Vector2f{ boundingBoxParent[vertex].x / SCALE, boundingBoxParent[vertex].y / SCALE });
//					}*/
//					int ceilingHeight = sectors.data[sector][1] / SCALE;
//					int floorHeight = sectors.data[sector][0] / SCALE;
//					PlaneIregular input{ boundingBoxChild, 2, int(ceilingHeight) };
//					PlaneIregular input2{ boundingBoxChild, 3, int(floorHeight) };
//					m_pFloorsNCeilings[m_floorsNCeilingsAmount] = input;
//					m_pFloorsNCeilings[m_floorsNCeilingsAmount + 1] = input2;
//
//					m_floorsNCeilingsAmount += 2;
//
//
//
//				}
//			}
//		}
//		else
//		{
//			ConstructSubSectors(boundingBox, sliceLines, nodes, child,
//				subsectors, vertexes, segs, lines, sides, sectors,
//				SCALE);
//		}
//
//		sliceLines.pop_back();
//
//	}
//	return 0;
//}

//int Level::ConstructSubSectors(const std::vector<Vector2f>& boundingBox, 
//	const LMP& nodes, int startNode, const LMP& subsectors, const LMP& vertexes,
//	const LMP& segs, const LMP& lines, const LMP& sides, const LMP& sectors,
//	const int SCALE)
//{
//	std::cout << std::endl;
//	std::cout << " going into node " << startNode << std::endl;
//	//     0	   |    1		 |	  2		|   3	  |
//	// partition x | partition y | p x diff | p y diff|
//	//   4		|     5		  |	    6	  |		7	  |
//	// rBox top | rBox bottom | rBox left | rBox right|
//	//    8		|		9	  |    10	  |		11	   |
//	// lBox top | lBox bottom | lBox left | lBox right |
//	//		12	   |	13 
//	// right child | left child
//
//	// we need for each subsector an array of vector2f's
//	// we need a vector of vector2f's along which all the points will be handled
//
//	std::vector<Vector2f> boundingBoxRight{};
//
//	Vector2f partitionPoint1{ float(nodes.data[startNode][0]), float(nodes.data[startNode][1]) };
//	Vector2f partitionPoint2{ float(partitionPoint1.x + nodes.data[startNode][2]), float(partitionPoint1.y + nodes.data[startNode][3]) };
//	int childIndex = 12;
//	for (int i{}; i < 2; i++)
//	{
//		int child{ nodes.data[startNode][childIndex + i] };
//		if (child < 0)
//		{
//			std::vector<Vector2f> boundingBoxChild{};
//			// it's a subSector
//			int subSector = utils::ReadNodeField(child);
//
//			std::cout << "from " << startNode << " arived at subsector " << subSector << std::endl;
//			std::cout << "using partition points " << partitionPoint1 << " and " << partitionPoint2 << std::endl;
//			//sliceLine(boundingBox, boundingBoxChild, partitionPoint1, partitionPoint2, i);
//			// we want to clip the curent plane against each seg of the subsector
//			//std::vector<Vector2f> boundingBoxSubSector{};
//			//boundingBoxSubSector.reserve(boundingBox.size());
//			//SliceSegs(boundingBoxChild, boundingBoxSubSector, subSector, subsectors, segs, vertexes);
//
//			//now we wanna generate the actual plane.
//
//			int seg = subsectors.data[subSector][1];
//			//if (subSector == 15) {
//				//std::cout << " seg " << seg << std::endl;
//			//}
//			int line = segs.data[seg][3];
//			int orienttion = segs.data[seg][4]; //these are curently magic numbers. they are derived from the file structure, im thinking of putting them in an enumerator
//			int sideIndex{ 5 };
//			if (orienttion > 0)
//			{ // 0 : front side | 1 : backside
//				sideIndex = 6;
//			}
//			int side = lines.data[line][sideIndex];
//			int sector = sides.data[side][14];
//			
//			/*for (int vertex{}; vertex < boundingBoxChild.size(); vertex++)
//			{
//				boundingBoxChild[vertex].x = boundingBoxChild[vertex].x / SCALE;
//				boundingBoxChild[vertex].y = boundingBoxChild[vertex].y / SCALE;
//			}*/
//			for (int vertex{}; vertex < boundingBox.size(); vertex++)
//			{
//				boundingBoxChild.push_back(Vector2f{ boundingBox[vertex].x / SCALE, boundingBox[vertex].y / SCALE });
//			}
//			int ceilingHeight = sectors.data[sector][1] / SCALE;
//			int floorHeight = sectors.data[sector][0] / SCALE;
//			PlaneIregular input{ boundingBoxChild, 2, int(ceilingHeight) };
//			PlaneIregular input2{ boundingBoxChild, 3, int(floorHeight) };
//			m_pFloorsNCeilings[m_floorsNCeilingsAmount] = input;
//			m_pFloorsNCeilings[m_floorsNCeilingsAmount + 1] = input2;
//
//			m_floorsNCeilingsAmount += 2;
//		}
//		else
//		{
//			std::cout << "from " << startNode << " to child node " << child << std::endl;
//			std::cout << "using partition points " << partitionPoint1 << " and " << partitionPoint2 << std::endl;
//			//first child right, then child left
//			//it's another node
//			// we want to clip the curent plane against the partition line
//			std::vector<Vector2f> boundingBoxChild{};
//			sliceLine(boundingBox, boundingBoxChild, partitionPoint1, partitionPoint2, i);
//			
//			int subsector{
//				ConstructSubSectors(boundingBoxChild, nodes, child,
//				subsectors, vertexes, segs, lines, sides, sectors,
//				SCALE)
//			};
//			
//		}
//	}
//	return 0;
//}

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


//void Level::sliceLine(const std::vector<Vector2f>& boundingBoxOriginal, std::vector<Vector2f>& boundingBoxNew,
//	 const Vector2f& partitionPoint1, const Vector2f& partitionPoint2, const int sideToSlice) {
//	for (int point{}; point < boundingBoxOriginal.size(); point++)
//	{
//		// we make a new vector to store the child points
//
//
//		// we take a point from the bounding box
//		Vector2f segPoint1 = boundingBoxOriginal[point];
//		
//		//and check if it belongs to the left or right side
//		// To find orientation of ordered triplet (p, q, r). 
//		// The function returns following values 
//		// 0 --> p1, p2 and r are collinear 
//		// 1 --> Counterclockwise 
//		// -1 --> Clockwise 
//		int orientation{ segPoint1.orientation(partitionPoint1, partitionPoint2) };
//		
//		//left and right are to be done subsequentialy
//		// 0 is right side
//		// 1 is left side
//		if (sideToSlice == 0 && orientation < 0)
//		{
//			//right side
//			boundingBoxNew.push_back(segPoint1);
//		}
//		if (sideToSlice == 1 && orientation > 0)
//		{
//			//left side
//			boundingBoxNew.push_back(segPoint1);
//		}
//		if (orientation == 0) {
//			boundingBoxNew.push_back(segPoint1);
//		}
//		// get the following point of the bounding box
//		int nextPoint = utils::getArrIndex(boundingBoxOriginal.size(), point + 1);
//		Vector2f segPoint2 = boundingBoxOriginal[nextPoint];
//		
//		Vector2f intersectionPoint{}; // point in which the potential intersection is stored.
//		//check if the partition line and the bounding box segment intersect
//		//bool intersectsWithPartition = Vector2f::doIntersect(
//		bool intersectsWithPartition = Vector2f::intersection(
//			partitionPoint1, partitionPoint2, segPoint1, segPoint2, intersectionPoint
//		);
//
//
//
//		//if they intersect, add the intersectionPoint to the child bounding box.
//		//all intersection pointys should always be added.
//		if (intersectsWithPartition) {
//			if (intersectionPoint.OnSegment(segPoint1, segPoint2))
//			{
//				boundingBoxNew.push_back(intersectionPoint);
//			}
//		}
//	}
//}

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
