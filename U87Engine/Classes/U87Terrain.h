#pragma once

#include "U87Math.h"

namespace u87
{

	#define MAX_MAP_WIDTH				100
	#define MAX_MAP_HEIGHT				100

	typedef enum _TileType {
		TT_OUT_OF_SG, TT_IN_SG, TT_BORDER
	} TileType;

	typedef struct _GridCross {
		int row, col;
	} GridCross;

	typedef struct _BorderSegment {
		GridCross start, end;
	} BorderSegment;

	typedef struct _BorderNode {
		BorderSegment segment;
		struct _BorderNode* next;
	} BorderNode;

	class Tile {
	private:
		TileType type;
		BorderNode* innerSgmtListEntry;
	public:
		Tile();
		TileType getType() { return type; }
		BorderNode* getSgmtListEntry() { return innerSgmtListEntry; }
		void setType(TileType _type) { type = _type; }
		void addBorderSegment(BorderNode* pSgmt);
	};

	class Terrain {
	private:
		int tileSize;
		Tile map[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];
		int width, height;
		BorderNode* borderEntry;
		void calcTileDataFromBorders();
		GridCross findFloodSeed();
		void sgFloodfill(int seedrow, int seedcol);
	public:
		Terrain();
		void loadBordersFromFile(const char* fileName);
		void print();
		bool isInStandGround(const Point& point);
		BorderNode* getBorderEntry() { return borderEntry; }
		int getTileSize() { return tileSize; }
		Point movingRevision(const Point& last, const Point& updated);
	};

}