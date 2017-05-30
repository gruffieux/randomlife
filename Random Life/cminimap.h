#ifndef CMINIMAP
#define CMINIMAP

class MiniMap : public Panel
{
private:
	Axe TileSize;
	TileEngine *Map;
public:
	MiniMap();
	MiniMap(int TileSizeX, int TileSizeY, TileEngine *MM_Map, int Panel_BorderSpace);
	~MiniMap();
	void Init();
	void InitAni();
	void RestoreAni();
	void DestroyAni();
	bool Refresh();
	bool Generate(bool waited=false, short click=0, Item *Cursor=NULL);
	bool InitTiles(char *filename, int layer, int ColorKey, void (CALLBACK *CallBackProc)()=NULL);
	Axe* GetTileSize() {return &TileSize;}
};

#endif