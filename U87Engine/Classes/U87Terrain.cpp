#include <fstream>
#include <cmath>
#include "U87Math.h"
#include "U87Terrain.h"

namespace u87
{

	Tile::Tile() {
		innerSgmtListEntry = NULL;
	}

	void Tile::addBorderSegment(BorderNode* pSgmt) {
		BorderNode* newpSgmt = new BorderNode;
		newpSgmt->segment = pSgmt->segment;
		if (innerSgmtListEntry == NULL) {
			newpSgmt->next = NULL;
			innerSgmtListEntry = newpSgmt;
		}
		else {
			newpSgmt->next = innerSgmtListEntry->next;
			innerSgmtListEntry->next = newpSgmt;
		}
	}

	Terrain::Terrain() {
		for (int i = 0; i < MAX_MAP_HEIGHT; i++) 
			for (int j = 0; j < MAX_MAP_WIDTH; j++)
				map[i][j].setType(TT_OUT_OF_SG);
	}

	void Terrain::loadBordersFromFile(const char* fileName) {
		std::ifstream fs(fileName);
		if (!fs.is_open()) {
			exit(-1);
		}
		char line[256];
		char* context;
		fs.getline(line, sizeof(line));
		char* item = strtok_s(line, " ", &context);  height = atoi(item);
		item = strtok_s(NULL, " ", &context);  width = atoi(item);
		item = strtok_s(NULL, " ", &context);  tileSize = atoi(item);
		int row0, col0;
		fs.getline(line, sizeof(line));
		BorderNode* lastNode;
		BorderNode* newNode = new BorderNode;
		item = strtok_s(line, " ", &context);  newNode->segment.start.row = atoi(item);
		item = strtok_s(NULL, " ", &context);  newNode->segment.start.col = atoi(item);
		fs.getline(line, sizeof(line));
		item = strtok_s(line, " ", &context);  row0 = newNode->segment.end.row = atoi(item);
		item = strtok_s(NULL, " ", &context);  col0 = newNode->segment.end.col = atoi(item);
		borderEntry = lastNode = newNode;
		fs.getline(line, sizeof(line));
		while (!fs.eof()) {
			newNode = new BorderNode;
			newNode->segment.start.row = row0;
			newNode->segment.start.col = col0;
			item = strtok_s(line, " ", &context);  row0 = newNode->segment.end.row = atoi(item);
			item = strtok_s(NULL, " ", &context);  col0 = newNode->segment.end.col = atoi(item);
			lastNode->next = newNode;
			lastNode = newNode;
			fs.getline(line, sizeof(line));
		}
		newNode = new BorderNode;
		newNode->segment.start.row = row0;
		newNode->segment.start.col = col0;
		newNode->segment.end.row = borderEntry->segment.start.row;
		newNode->segment.end.col = borderEntry->segment.start.col;
		lastNode->next = newNode;
		newNode->next = borderEntry;
		calcTileDataFromBorders();
		GridCross seed = findFloodSeed();
		sgFloodfill(seed.row, seed.col);
	}

	void Terrain::calcTileDataFromBorders() {
		BorderNode* border = borderEntry; 
		do {
			int row1 = border->segment.start.row, row2 = border->segment.end.row;
			int col1 = border->segment.start.col, col2 = border->segment.end.col;
			int drow = row2 - row1, dcol = col2 - col1;
			if (dcol == 0) {
				if (drow > 0) {
					for (int i = row1; i < row2; i++)
						if (map[i][col1 - 1].getType() != TT_BORDER)
							map[i][col1 - 1].setType(TT_IN_SG);
				}
				else if (drow < 0) {
					for (int i = row2; i < row1; i++)
						if (map[i][col1].getType() != TT_BORDER)
							map[i][col1].setType(TT_IN_SG);
				}
				border = border->next;  continue;
			}
			if (drow == 0) {
				if (dcol > 0) {
					for (int i = col1; i < col2; i++)
						if (map[row1][i].getType() != TT_BORDER)
							map[row1][i].setType(TT_IN_SG);
				}
				else if (drow < 0) {
					for (int i = col2; i < col1; i++)
						if (map[row1 - 1][i].getType() != TT_BORDER)
							map[row1 - 1][i].setType(TT_IN_SG);
				}
				border = border->next;  continue;
			}
			if (dcol < 0) {
				int t;
				t = row1;  row1 = row2;  row2 = t;
				t = col1;  col1 = col2;  col2 = t;
				dcol = -dcol;  drow = -drow;
			}
			int loop = gcd(dcol, abs(drow));
			int rloop = drow / loop, cloop = dcol / loop;
			double step = 1.0 * drow / dcol;
			double stepLeft = 0.0, stepRight = 0.0;
			for (int i = col1; i < col1 + cloop; i++) {
				stepRight += step;
				for (int j = (int)floor(stepLeft); j != (int)floor(stepRight) + sign(drow); j += sign(drow)) {
					if (i == col1 + cloop - 1 && drow > 0 && j == (int)floor(stepRight)
						|| i == col1 && drow < 0 && j == (int)floor(stepLeft)) {
						continue;
					}
					for (int k = 0; k < loop; k++) {
						map[row1 + j + k * rloop][i + k * cloop].setType(TT_BORDER);
						map[row1 + j + k * rloop][i + k * cloop].addBorderSegment(border);
					}
				}
				stepLeft = stepRight;
			}
			border = border->next;
		} while (border != borderEntry);
	}

	GridCross Terrain::findFloodSeed() {
		int col, row;
		GridCross cross;
		for (row = 0; row < height; row++) {
			col = 0;
			while (col < width && map[row][col].getType() == TT_OUT_OF_SG) col++;
			if (col == width) continue;
			while (col < width && map[row][col].getType() != TT_OUT_OF_SG) col++;
			if (col == width) continue;
			while (col < width && map[row][col].getType() == TT_OUT_OF_SG) col++;
			if (col == width) continue;
			if (map[row][col].getType() != TT_OUT_OF_SG) {
				cross.row = row;  cross.col = col - 1;
				return cross;
			}
		}
		cross.row = cross.col = -1;
		return cross;
	}

	void Terrain::sgFloodfill(int seedrow, int seedcol) {
		if (seedrow < 0 || seedrow >= height || seedcol < 0 || seedcol >= width)
			return;
		if (map[seedrow][seedcol].getType() == TT_BORDER || map[seedrow][seedcol].getType() == TT_IN_SG)
			return;
		map[seedrow][seedcol].setType(TT_IN_SG);
		sgFloodfill(seedrow - 1, seedcol);
		sgFloodfill(seedrow + 1, seedcol);
		sgFloodfill(seedrow, seedcol - 1);
		sgFloodfill(seedrow, seedcol + 1);
	}

	void Terrain::print() {
		for (int i = height - 1; i >= 0; i--) {
			for (int j = 0; j < width; j++) {
				switch(map[i][j].getType()) {
				case TT_BORDER:
					printf("#");  break;
				case TT_IN_SG:
					printf("+");  break;
				case TT_OUT_OF_SG:
					printf("-");  break;
				default:
					printf(".");  break;
				}
			}
			printf("\n");
		}
	}

	bool Terrain::isInStandGround(const Point& point) {
		int col = (int)(point.x / tileSize), row = (int)(point.y / tileSize);
		if (map[row][col].getType() == TT_IN_SG)
			return true;
		if (map[row][col].getType() == TT_OUT_OF_SG)
			return false;
		BorderNode* pSgmt = map[row][col].getSgmtListEntry();
		Point sgmtStart, sgmtEnd;
		while (pSgmt != NULL) {
			sgmtStart = ccp(tileSize * pSgmt->segment.start.col, tileSize * pSgmt->segment.start.row);
			sgmtEnd = ccp(tileSize * pSgmt->segment.end.col, tileSize * pSgmt->segment.end.row);
			if (pointOnLineSide(point, sgmtStart, sgmtEnd) == POS_ON_THE_LEFT)
				return false;
			pSgmt = pSgmt->next;
		}
		return true;
	}

	Point Terrain::movingRevision(const Point& last, const Point& updated) {
		CC_UNUSED_PARAM(last);
		if (isInStandGround(updated)) {
			return ccp(0.0f, 0.0f);
		}
		BorderNode* pSgmt = borderEntry;
		Point wallVect, nextWallVect, sgmtStart, sgmtEnd, nextEnd, dividerVect;
		Point revised;
		do {
			sgmtStart = ccp(tileSize * pSgmt->segment.start.col, tileSize * pSgmt->segment.start.row);
			sgmtEnd = ccp(tileSize * pSgmt->segment.end.col, tileSize * pSgmt->segment.end.row);
			nextEnd = ccp(tileSize * pSgmt->next->segment.end.col, tileSize * pSgmt->next->segment.end.row);
			wallVect = ccpRotateByAngle(sgmtEnd - sgmtStart, ccp(0, 0), CC_DEGREES_TO_RADIANS(-90));
			nextWallVect = ccpRotateByAngle(nextEnd - sgmtEnd, ccp(0, 0), CC_DEGREES_TO_RADIANS(-90));
			if (ccpCross(sgmtEnd - sgmtStart, nextEnd - sgmtEnd) > 0) // 凸角
			{
				if (pointOnLineSide(updated, sgmtStart, sgmtEnd) == POS_ON_THE_RIGHT
					&& pointOnLineSide(updated, sgmtStart, sgmtStart + wallVect) == POS_ON_THE_LEFT
					&& pointOnLineSide(updated, sgmtEnd, sgmtEnd + wallVect) == POS_ON_THE_RIGHT)
				{
					// 作个垂线修正..
					revised = sgmtStart + ccpProject(updated - sgmtStart, sgmtEnd - sgmtStart);
					break;
				}
				else if (pointOnLineSide(updated, sgmtEnd, sgmtEnd + wallVect) == POS_ON_THE_LEFT
					&& pointOnLineSide(updated, sgmtEnd, sgmtEnd + nextWallVect) == POS_ON_THE_RIGHT)
				{
					// 修正到当前边末端的凸角点..
					revised = sgmtEnd;
					break;
				}
			}
			else // 凹角
			{
				dividerVect = ccpNormalize(nextEnd - sgmtEnd) - ccpNormalize(sgmtEnd - sgmtStart);
				if (pointOnLineSide(updated, sgmtStart, sgmtEnd) == POS_ON_THE_RIGHT
					&& pointOnLineSide(updated, sgmtStart, sgmtStart + wallVect) == POS_ON_THE_LEFT
					&& pointOnLineSide(updated, sgmtEnd, sgmtEnd + dividerVect) == POS_ON_THE_RIGHT)
				{
					// 作个垂线修正..
					revised = sgmtStart + ccpProject(updated - sgmtStart, sgmtEnd - sgmtStart);
					break;
				}
			}
			pSgmt = pSgmt->next;
		} while (pSgmt != borderEntry);
		return (revised - updated);
	}

}