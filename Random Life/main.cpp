/*
******************************* Random Life **********************************

Notes:
-Ne pas créer plusieurs instances sonores (Performance, AudioPath), ça ralentit beaucoup!
-L'affichage des textes ralentit les fps

TODO:

Bugs:
-Certain personnages de la zone ne peuvent pas être ciblé, la collision avec le curseur n'est pas détectée
-Parfois un acteur se bloque après avoir tué sa cible

Exploits:
*/

#include <dxerror.h>
#include <cinput.h>
#include <cconfigfile.h>
#include <ckeyboardlistener.h>
#include <cvisuallist.h>
#include <canimation.h>
#include <ccursor.h>
#include <cworldmap.h>
#include <citemmap.h>
#include <cmsg.h>
#include "cpotion.h"
#include "cingredient.h"
#include "cinventory.h"
#include "cplayer.h"
#include "canimal.h"
#include "ccreature.h"
#include "ccombatsystemlist.h"
#include "cquestcomment.h"
#include "cevents.h"
#include "cquestbook.h"
#include "cdialogue.h"
#include "ccharinfo.h"
#include "ctargetgauge.h"
#include "clookedpickup.h"
#include "clookedspell.h"
#include "cspelllist.h"
#include "cmagiclearner.h"
#include "cchemicallab.h"
#include "cworldtime.h"
#include "cdataprogress.h"
#include "cmenu.h"
#include "caskforsave.h"
#include "cshortcut.h"
#include "cbattleinfo.h"
#include "resource.h"
#include "../RLRes/RLRes/resource.h"

//////////////////////////////
// Prototypes des fonctions //
//////////////////////////////

// Fonction principales
bool AddRezPoint(int ZoneX, int ZoneY);
bool AddRezPoint(ItemMap *pItemMap);
bool AddZoneChar(Character *pCharacter, Zone *pZone, int *AnTotal, int *HuTotal, int *AnCounter, int *HuCounter, int *ClassCounter);
bool AddZoneChar(Character *pCharacter, Zone *pZone, int *AnTotal, int *HuTotal, int *WkTotal, int *MdTotal, int *StTotal, int *AnCounter, int *HuCounter, int *WkCounter, int *MdCounter, int *StCounter);
bool AddZoneChar(Character *pCharacter, Zone *pZone, int *RaceCounter=NULL, int *ClassCounter=NULL);
void ApplyConfig();
bool CanLoadData();
void ChangeZone(Player *pPlayer, int x, int y, int Char_posx, int Char_posy, int Camera_posx, int Camera_posy, bool ForceLoadZone, bool ResetPos);
void CheckUpdate();
void DestroyAll();
void DestroyDirectDraw();
void DestroyDirectInput();
void DestroyDirectMusic();
void DispatchCreature(Zone *pZone);
void DispatchPickupsOnChar();
void DispatchPickupsOnZone();
void DispatchTraderPickups(Zone *pZone);
void DisplayPanel(SHORT_PANEL ShortKey);
void CALLBACK GenerateDataProgress(int CB_IndexCount);
Zone * GetCurrentZone();
void GiveMagicSpellToChar(Humanized *pHumanized);
void GivePickupToChar();
void InitCharacter(Character *pCharacter, int BasicStatScore, int BasicSkillLevel, int MinStatBonus, int MaxStatBonus, int MinFavoriteSkillLevel, int MaxFavoriteSkillLevel, int MinMagicSkillLevel, int MaxMagicSkillLevel);
bool InitCombat();
void InitCreatureOnZone();
void InitDirectDraw();
void InitDirectInput();
void InitDirectMusic();
void InitIngredients();
bool InitMagicSpellPack();
void InitMessages();
void InitObjects();
bool InitORC();
void InitPanels();
void InitPickup(Pickup *pPickup);
HWND InitWindow(HINSTANCE hInst, int nCmdShow);
void InitZone(Zone *pZone, bool ResetPos);
bool LoadConfig();
DWORD WINAPI LoadData(LPVOID lpParam);
void NewGame();
void PlayMusic();
bool PlayScene(Zone *pZone);
bool ProcessAI(Character *pCharacter);
bool ProcessKbInput();
bool ProcessMouseInput();
void RestoreDirectDraw();
DWORD WINAPI SaveData(LPVOID lpParam);
void SetZoneTile(int layer);
void UpdateCurse(int x, int y, short value, Zone *pZone);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

////////////////////////
// Variables globales //
////////////////////////

// Objets de base nécessaire à l'application
Display *Screen;
Input *Keyboard, *Mouse;
extern MsgReporter EventsReporter;

// Objets divers du jeu
ConfigFile Config;
KeyboardListener keybListener;
DataFileList Rules("rules.kdat", DataFileList::KEY_INDEX);
Clock *TicTac;
TileEngine *Map;
Cursor *cursor;
Panel *PTest;
Menu *PMenu;
AskForSave *PSave;
ShortCut *PShortCut;
BattleInfo *PBattle;
SpellList *PSpellList;
CharInfo *PCharInfo;
Gauge *PGauge;
TargetGauge *PTargetGauge;
LookedPickup *PPickup;
LookedSpell *PMagicSpell;
ChemicalLab * PChemicalLab;
WorldTime *PWorldTime;
Dialogue *PDialogue;
QuestBook *PBook;
Events *PEvents;
Inventory *PInventory, *PTargetInventory;
MagicLearner *PGreatMago;
WorldMap *LandMap;
DataProgress *PData;
List MusicPack[TOTAL_MUSIC_CATEGORY], MusicRandomPack[TOTAL_MUSIC_CATEGORY], SoundPack;
VisualList PanelPack(VisualList::ORDER_BY_PLAN);
MagicSpellList MagicSpellPack;
CombatSystemList BattlePack;
MiniEvents ActionReporter;

// Les personnages
Player *Hero;
Animal *Spider[TOTAL_CHAR_ANIMAL_SPIDER], *FlatSnake[TOTAL_CHAR_ANIMAL_FLATSNAKE], *PinkSnake[TOTAL_CHAR_ANIMAL_PINKSNAKE], *BlackBear[TOTAL_CHAR_ANIMAL_BLACKBEAR], *MaleficDog[TOTAL_CHAR_ANIMAL_MELEFIC_DOG];
Creature *Robert[TOTAL_CHAR_HUMANIZED_ROBERT], *Redford[TOTAL_CHAR_HUMANIZED_REDFORD], *MiniPoux[TOTAL_CHAR_HUMANIZED_MINI_POUX], *Poux[TOTAL_CHAR_HUMANIZED_POUX], *Troll[TOTAL_CHAR_HUMANIZED_TROLL], *BadLogger[TOTAL_CHAR_HUMANIZED_BAD_LOGGER], *MadLogger[TOTAL_CHAR_HUMANIZED_MAD_LOGGER], *MarshPlant[TOTAL_CHAR_HUMANIZED_MARSHPLANT];
Creature *YungAcolyt[TOTAL_CHAR_HUMANIZED_YUNG_ACOLYT], *OldAcolyt[TOTAL_CHAR_HUMANIZED_OLD_ACOLYT], *LinkAcolyt[TOTAL_CHAR_HUMANIZED_LINK_ACOLYT], *Balrogue, *Wizard;
Pnj *Trader, *GreatMago, *Captain, *Wise, *Leader, *Guard, *Citizen[TOTAL_CHAR_PNJ_CITIZEN];
List CharPack, LinkAcolytPack;
VisualList ZoneCharPack(VisualList::ORDER_BY_PLAN);

// Les pickups
Pickup *MoneyDefaultBag, *MoneyPoorBag[TOTAL_PICKUP_MONEY_POORBAG], *MoneyMediumBag[TOTAL_PICKUP_MONEY_MEDIUMBAG], *MoneyRichBag[TOTAL_PICKUP_MONEY_RICHBAG], *MoneyPNJ1Bag[TOTAL_CHAR_PNJ], *MoneyPNJ2Bag[TOTAL_CHAR_PNJ_CITIZEN];
Pickup *Authorization, *MindProtector, *MagicCircle;
Ingredient *WhiteFlower[TOTAL_PICKUP_FLOWER_WHITE], *RedFlower[TOTAL_PICKUP_FLOWER_RED], *BlueFlower[TOTAL_PICKUP_FLOWER_BLUE], *RedFruit[TOTAL_PICKUP_FRUIT_RED], *PinkFruit[TOTAL_PICKUP_FRUIT_PINK], *BlueFruit[TOTAL_PICKUP_FRUIT_BLUE], *BlackFruit[TOTAL_PICKUP_FRUIT_BLACK], *OrangeFruit[TOTAL_PICKUP_FRUIT_ORANGE], *WaterBottle[TOTAL_PICKUP_DRINK_WATER], *WineBottle[TOTAL_PICKUP_DRINK_WINE], *TrollFluid[TOTAL_CHAR_HUMANIZED_TROLL];
Potion *LifePotion[TOTAL_PICKUP_POTION_LIFE], *ManaPotion[TOTAL_PICKUP_POTION_MANA], *CheapPotion[TOTAL_PICKUP_POTION_CHEAP], *DiscretionFlow[TOTAL_PICKUP_POTION_DF];
Weapon *WkDagger[TOTAL_PICKUP_WEAPON_WK_DAGGER], *StDagger[TOTAL_PICKUP_WEAPON_ST_DAGGER], *MdSword[TOTAL_PICKUP_WEAPON_MD_SWORD], *StSword[TOTAL_PICKUP_WEAPON_ST_SWORD], *PnjSword[TOTAL_PICKUP_WEAPON_PNJ_SWORD], *WkMace[TOTAL_PICKUP_WEAPON_WK_MACE], *MdMace[TOTAL_PICKUP_WEAPON_MD_MACE], *StMace[TOTAL_PICKUP_WEAPON_ST_MACE], *MdAxe[TOTAL_PICKUP_WEAPON_MD_AXE], *StAxe[TOTAL_PICKUP_WEAPON_ST_AXE], *Daggy[TOTAL_PICKUP_WEAPON_DAGGY], *Axel, *WizardStaff;
Armor *WkLightArmor[TOTAL_PICKUP_ARMOR_WK_LIGHT], *MdLightArmor[TOTAL_PICKUP_ARMOR_MD_LIGHT], *StLightArmor[TOTAL_PICKUP_ARMOR_ST_LIGHT], *Pnj1LightArmor[TOTAL_PICKUP_ARMOR_PNJ1_LIGHT], *Pnj2LightArmor[TOTAL_CHAR_PNJ_CITIZEN], *WkHeavyArmor[TOTAL_PICKUP_ARMOR_WK_HEAVY], *MdHeavyArmor[TOTAL_PICKUP_ARMOR_MD_HEAVY], *StHeavyArmor[TOTAL_PICKUP_ARMOR_ST_HEAVY], *PnjHeavyArmor[TOTAL_PICKUP_ARMOR_PNJ_HEAVY], *MagicianDress[TOTAL_PICKUP_ARMOR_MAGICIAN_DRESS], *Sylvanel;
List PickupPack, ZonePickupPack, FlowerPack, FruitPack, DrinkPack;

// Variables et structures
int ZoneTile[TOTAL_LIVING_TILE_ID], ZoneTileCount, RezPointCount, RezPoint[TOTAL_ZONE_Y][TOTAL_ZONE_X], CurrentMusicCategory, CurrentMusic;
bool running, reinit, EraseFile;
Axe ZoneCoord;
ORC ORCData[TOTAL_CHAR_SKILL_RESULT];
CharCombat CombatData[TOTAL_COMBAT_RECORD];
Zone *CurrentZone;

// Tableau des zones
Zone ZoneData[TOTAL_ZONE_Y][TOTAL_ZONE_X] =
{
	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, MC_OUTLAND, false, false, false, false, false, 0, NULL, NULL, NULL, NULL},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, MC_OUTLAND, false, false, false, false, false, 0, NULL, NULL, NULL, NULL},
		{2, 0, 5, 10, 0, 100, 0, 0, 100, MC_WIZARD_FORTRESS, true, true, true, true, false, 0, "ground_0_2.dat", "decoration_0_2.dat", "items_0_2.dat", "bridges_0_2.dat"},
		{3, 0, 15, 20, 10, 90, 0, 10, 90, MC_SNOW, true, true, true, false, false, 0, "ground_0_3.dat", "decoration_0_3.dat", "items_0_3.dat", NULL},
		{4, 0, 15, 20, 10, 90, 0, 20, 80, MC_SNOW, true, true, true, false, false, 0, "ground_0_4.dat", "decoration_0_4.dat", "items_0_4.dat", NULL},
		{5, 0, 15, 20, 10, 90, 0, 40, 60, MC_SNOW, true, true, true, false, false, 0, "ground_0_5.dat", "decoration_0_5.dat", "items_0_5.dat", NULL},
		{6, 0, 0, 0, 0, 0, 0, 0, 0, MC_OUTLAND, false, false, false, false, false, 0, NULL, NULL, NULL, NULL}
	},
	{
		{0, 1, 0, 0, 0, 0, 0, 0, 0, MC_OUTLAND, false, false, false, false, false, 0, NULL, NULL, NULL, NULL},
		{1, 1, 0, 0, 0, 0, 0, 0, 0, MC_OUTLAND, false, false, false, false, false, 0, NULL, NULL, NULL, NULL},
		{2, 1, 10, 15, 90, 10, 40, 60, 0, MC_SNOW, true, true, true, false, false, 0, "ground_1_2.dat", "decoration_1_2.dat", "items_1_2.dat", NULL},
		{3, 1, 10, 15, 80, 20, 20, 70, 10, MC_SNOW, true, true, true, false, false, 0, "ground_1_3.dat", "decoration_1_3.dat", "items_1_3.dat", NULL},
		{4, 1, 10, 15, 50, 50, 0, 70, 30, MC_SNOW, true, true, true, false, false, 0, "ground_1_4.dat", "decoration_1_4.dat", "items_1_4.dat", NULL},
		{5, 1, 10, 15, 20, 80, 0, 50, 50, MC_SNOW, true, true, true, true, false, 0, "ground_1_5.dat", "decoration_1_5.dat", "items_1_5.dat", "bridges_1_5.dat"},
		{6, 1, 0, 0, 0, 0, 0, 0, 0, MC_OUTLAND, false, false, false, false, false, 0, NULL, NULL, NULL, NULL}
	},
	{
		{0, 2, 0, 0, 0, 0, 0, 0, 0, MC_OUTLAND, false, false, false, false, false, 0, NULL, NULL, NULL, NULL},
		{1, 2, 10, 15, 80, 20, 40, 60, 0, MC_OUTLAND, true, true, true, false, false, 0, "ground_2_1.dat", "decoration_2_1.dat", "items_2_1.dat", NULL},
		{2, 2, 10, 15, 80, 20, 80, 20, 0, MC_OUTLAND, true, true, true, false, false, 0, "ground_2_2.dat", "decoration_2_2.dat", "items_2_2.dat", NULL},
		{3, 2, 13, 16, 50, 50, 100, 0, 0, MC_VILLAGE, true, true, true, true, false, 0, "ground_2_3.dat", "decoration_2_3.dat", "items_2_3.dat", "bridges_2_3.dat"},
		{4, 2, 0, 0, 0, 0, 0, 0, 0, MC_OUTLAND, false, false, false, false, false, 0, NULL, NULL, NULL, NULL},
		{5, 2, 0, 0, 0, 0, 0, 0, 0, MC_OUTLAND, false, false, false, false, false, 0, NULL, NULL, NULL, NULL},
		{6, 2, 0, 0, 0, 0, 0, 0, 0, MC_OUTLAND, false, false, false, false, false, 0, NULL, NULL, NULL, NULL}
	},
	{
		{0, 3, 10, 15, 60, 40, 20, 70, 10, MC_OUTLAND, true, true, true, false, false, 0, "ground_3_0.dat", "decoration_3_0.dat", "items_3_0.dat", NULL},
		{1, 3, 10, 15, 70, 30, 30, 70, 0, MC_OUTLAND, true, true, true, false, false, 0, "ground_3_1.dat", "decoration_3_1.dat", "items_3_1.dat", NULL},
		{2, 3, 10, 15, 70, 30, 40, 60, 0, MC_OUTLAND, true, true, true, false, false, 0, "ground_3_2.dat", "decoration_3_2.dat", "items_3_2.dat", NULL},
		{3, 3, 10, 15, 70, 30, 80, 20, 0, MC_OUTLAND, true, true, true, true, false, 0, "ground_3_3.dat", "decoration_3_3.dat", "items_3_3.dat", "bridges_3_3.dat"},
		{4, 3, 10, 15, 70, 30, 40, 60, 0, MC_OUTLAND, true, true, true, true, false, 0, "ground_3_4.dat", "decoration_3_4.dat", "items_3_4.dat", "bridges_3_4.dat"},
		{5, 3, 10, 15, 20, 80, 20, 70, 10, MC_OUTLAND, true, true, true, false, false, 0, "ground_3_5.dat", "decoration_3_5.dat", "items_3_5.dat", NULL},
		{6, 3, 10, 15, 20, 80, 0, 80, 20, MC_MONTAIN, true, true, true, false, false, 0, "ground_3_6.dat", "decoration_3_6.dat", "items_3_6.dat", NULL}
	},
	{
		{0, 4, 15, 20, 40, 60, 0, 60, 40, MC_FOREST, true, true, true, true, false, 0, "ground_4_0.dat", "decoration_4_0.dat", "items_4_0.dat", "bridges_4_0.dat"},
		{1, 4, 15, 20, 30, 70, 0, 40, 60, MC_FOREST, true, true, true, true, false, 0, "ground_4_1.dat", "decoration_4_1.dat", "items_4_1.dat", "bridges_4_1.dat"},
		{2, 4, 15, 20, 40, 60, 20, 60, 20, MC_OUTLAND, true, true, true, true, false, 0, "ground_4_2.dat", "decoration_4_2.dat", "items_4_2.dat", "bridges_4_2.dat"},
		{3, 4, 0, 0, 0, 0, 0, 0, 0, MC_OUTLAND, false, false, false, false, false, 0, NULL, NULL, NULL, NULL},
		{4, 4, 0, 0, 0, 0, 0, 0, 0, MC_OUTLAND, false, false, false, false, false, 0, NULL, NULL, NULL, NULL},
		{5, 4, 15, 20, 0, 100, 0, 90, 10, MC_MONTAIN, true, true, true, false, false, 0, "ground_4_5.dat", "decoration_4_5.dat", "items_4_5.dat", NULL},
		{6, 4, 15, 20, 10, 90, 0, 50, 50, MC_MONTAIN, true, true, true, true, false, 0, "ground_4_6.dat", "decoration_4_6.dat", "items_4_6.dat", "bridges_4_6.dat"}
	},
	{
		{0, 5, 15, 20, 50, 50, 0, 30, 70, MC_OUTLAND, true, true, true, true, false, 0, "ground_5_0.dat", "decoration_5_0.dat", "items_5_0.dat", "bridges_5_0.dat"},
		{1, 5, 15, 20, 10, 90, 0, 20, 80, MC_FOREST, true, true, true, false, false, 0, "ground_5_1.dat", "decoration_5_1.dat", "items_5_1.dat", NULL},
		{2, 5, 0, 0, 0, 0, 0, 0, 0, MC_OUTLAND, false, false, false, false, false, 0, NULL, NULL, NULL, NULL},
		{3, 5, 0, 0, 0, 0, 0, 0, 0, MC_OUTLAND, false, false, false, false, false, 0, NULL, NULL, NULL, NULL},
		{4, 5, 0, 0, 0, 0, 0, 0, 0, MC_OUTLAND, false, false, false, false, false, 0, NULL, NULL, NULL, NULL},
		{5, 5, 0, 0, 0, 0, 0, 0, 0, MC_OUTLAND, false, false, false, false, false, 0, NULL, NULL, NULL, NULL},
		{6, 5, 15, 20, 0, 100, 0, 20, 80, MC_MONTAIN, true, true, true, true, false, 0, "ground_5_6.dat", "decoration_5_6.dat", "items_5_6.dat", "bridges_5_6.dat"}
	}
};

///////////////////////////////
// Définitions des fonctions //
///////////////////////////////

bool AddRezPoint(int ZoneX, int ZoneY)
{
	int x, y;
	ItemMap *NewRezPoint;

	if (RezPoint[ZoneY][ZoneX])
		return false;

	for (x = 0; x < Map->GetMapData()->TileCount.x; x++)
		for (y = 0; y < Map->GetMapData()->TileCount.y; y++)
			if (Map->GetMapData()->TileID[y][x][ITEMS] == 1)
			{
				NewRezPoint = new ItemMap(1, 16, 16);
				if (LandMap->AddItemMap(NewRezPoint))
				{
					RezPoint[ZoneY][ZoneX] = 1;
					LandMap->SetItemOnMap(NewRezPoint, x*Map->GetTileData()->getDimension()->x, y*Map->GetTileData()->getDimension()->y, x, y, ZoneX, ZoneY, true);
					RezPointCount++;
					return true;
				}
				else
				{
					delete NewRezPoint;
					return false;
				}
			}

	return false;
}

bool AddRezPoint(ItemMap *pItemMap)
{
	int x, y;
	ItemMap *NewRezPoint;

	if (!pItemMap) return false;

	x = pItemMap->GetZoneID()->x;
	y = pItemMap->GetZoneID()->y;

	if (RezPoint[y][x]) return false;

	NewRezPoint = new ItemMap(*pItemMap);

	if (LandMap->AddItemMap(NewRezPoint))
	{
		RezPoint[y][x] = 1;
		LandMap->SetItemOnMap(NewRezPoint, NewRezPoint->GetWorldPosition()->x, NewRezPoint->GetWorldPosition()->y, NewRezPoint->GetCoord()->x, NewRezPoint->GetCoord()->y, NewRezPoint->GetZoneID()->x, NewRezPoint->GetZoneID()->y, true);
		RezPointCount++;
		return true;
	}

	delete NewRezPoint;

	return false;
}

bool AddZoneChar(Character *pCharacter, Zone *pZone, int *AnTotal, int *HuTotal, int *AnCounter, int *HuCounter, int *ClassCounter)
{
	switch (pCharacter->GetRace())
	{
	case ANIMAL:
		if (*AnCounter < *AnTotal) return AddZoneChar(pCharacter, pZone, AnCounter, ClassCounter);
		return false;
	case HUMANIZED:
		if (*HuCounter < *HuTotal) return AddZoneChar(pCharacter, pZone, HuCounter, ClassCounter);
		return false;
	default:
		return false;
	}
}

bool AddZoneChar(Character *pCharacter, Zone *pZone, int *AnTotal, int *HuTotal, int *WkTotal, int *MdTotal, int *StTotal, int *AnCounter, int *HuCounter, int *WkCounter, int *MdCounter, int *StCounter)
{
	switch (pCharacter->GetClass())
	{
	case WEAK:
		if (*WkCounter < *WkTotal) return AddZoneChar(pCharacter, pZone, AnTotal, HuTotal, AnCounter, HuCounter, WkCounter);
		return false;
	case MEDIUM:
		if (*MdCounter < *MdTotal) return AddZoneChar(pCharacter, pZone, AnTotal, HuTotal, AnCounter, HuCounter, MdCounter);
		return false;
	case STRONG:
		if (*StCounter < *StTotal) return AddZoneChar(pCharacter, pZone, AnTotal, HuTotal, AnCounter, HuCounter, StCounter);
		return false;
	case PNJ2:
		if (*WkCounter < *WkTotal) return AddZoneChar(pCharacter, pZone, AnTotal, HuTotal, AnCounter, HuCounter, WkCounter);
		return false;
	default:
		return false;
	}
}

bool AddZoneChar(Character *pCharacter, Zone *pZone, int *RaceCounter, int *ClassCounter)
{
	if (pZone->cursed)
		pCharacter->Curse(true, PWorldTime->GetChao());
	else
		pCharacter->Curse(false, PWorldTime->GetChao());

	if (ZoneCharPack.AddElement(pCharacter))
	{
		if (RaceCounter) *RaceCounter += 1;
		if (ClassCounter) *ClassCounter += 1;
		return true;
	}

	return false;
}

void ApplyConfig()
{
	int i;

	// dimension du menu de départ
	PMenu->getDimension()->x = Screen->GetCurrentMode()->GetWidth();
	PMenu->getDimension()->y = Screen->GetCurrentMode()->GetHeight();

	// dimension de la carte
	Map->GetScreenData()->dimension.x = Screen->GetCurrentMode()->GetWidth();
	Map->GetScreenData()->dimension.y = Screen->GetCurrentMode()->GetHeight();
	Map->InitScreenData();

	// position de la mini-carte
	LandMap->getPosition()->x = Screen->GetCurrentMode()->GetWidth()/2 - 560/2;
	LandMap->getPosition()->y = Screen->GetCurrentMode()->GetHeight()/2 - 480/2;

	// Replace les rezpoints
	for (i = 0; i < LandMap->GetElementCount(); i++)
		if (LandMap->GetItemMapElement(i)->GetType() == 1)
			LandMap->SetItemOnMap(LandMap->GetItemMapElement(i), LandMap->GetItemMapElement(i)->GetWorldPosition()->x, LandMap->GetItemMapElement(i)->GetWorldPosition()->y, LandMap->GetItemMapElement(i)->GetCoord()->x, LandMap->GetItemMapElement(i)->GetCoord()->y, LandMap->GetItemMapElement(i)->GetZoneID()->x, LandMap->GetItemMapElement(i)->GetZoneID()->y, true);

	// position et dimension du panneau pour les testes
	if (PTest)
	{
		PTest->getPosition()->y = Screen->GetCurrentMode()->GetHeight() - PTest->getDimension()->y;
		PTest->getDimension()->x = Screen->GetCurrentMode()->GetWidth();
		PTest->getDimension()->y = 16;
	}

	// position et dimension du panneau de progression des données
	PData->getDimension()->x = Screen->GetCurrentMode()->GetWidth() - Screen->GetCurrentMode()->GetWidth() / 5;
	PData->getDimension()->y = 48;
	PData->getPosition()->x = Screen->GetCurrentMode()->GetWidth() / 2 - PData->getDimension()->x / 2;
	PData->getPosition()->y = Screen->GetCurrentMode()->GetHeight() - 16 * 4;
	PData->GetDataBar()->SetMaxSize(PData->getDimension()->x - 16);
}

bool CanLoadData()
{
	HANDLE hFind;
	WIN32_FIND_DATA FileData;

	hFind = FindFirstFile("savegame.dat", &FileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		return true;
	}

	return false;
}

void ChangeZone(Player *pPlayer, int x, int y, int Char_posx, int Char_posy, int Camera_posx, int Camera_posy, bool ForceLoadZone, bool ResetPos)
{
	int i;
	bool LoadZone;

	if ((x != ZoneCoord.x) | (y != ZoneCoord.y) | (ForceLoadZone == true))
	{
		LoadZone = true;
		PostMessage(Screen->GetDxWnd(), WM_NULL, ResetPos, 0);
	}
	else
	{
		LoadZone = false;
		for (i = 0; i < ZoneCharPack.GetElementCount(); i++)
			if (TileIA::getTileIAElement(i, &ZoneCharPack))
				*TileIA::getTileIAElement(i, &ZoneCharPack)->GetCompass() = PathFinder(Map);
	}

	ZoneCoord.x = x;
	ZoneCoord.y = y;
	pPlayer->getPosition()->x = Char_posx;
	pPlayer->getPosition()->y = Char_posy;
	Map->GetScreenData()->Camera.getPosition()->x = Camera_posx;
	Map->GetScreenData()->Camera.getPosition()->y = Camera_posy;

	for (i = 0; i < 8; i++)
		*pPlayer->GetAdjNode(i) = ActorNode();

	if (!LoadZone)
		for (i = 0; i < ZoneCharPack.GetElementCount(); i++)
			if (!Player::getPlayerElement(i, &ZoneCharPack))
				TileActor::getTileActorElement(i, &ZoneCharPack)->GenerateMapPosition();

	pPlayer->SetTarget(pPlayer);
	pPlayer->GetIgnoredBy()->RemoveAllElement();
	pPlayer->Undo();
}

void CheckUpdate()
{
	DWORD exitCode;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

	CreateProcess(NULL, "UpdateClient.exe CHECK", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	WaitForSingleObject(pi.hProcess, 15000);
	GetExitCodeProcess(pi.hProcess, &exitCode);

	switch (exitCode)
	{
	case 0:
		break;
	case 1:
		if (MessageBox(NULL, "Le programme doit fermer, voulez vous continuer? Relancez-le après la mise à jour.", "Mise à jour disponible!", MB_YESNO) == IDYES)
		{
			ShellExecute(NULL, "open", "UpdateClient.exe", "UPDATE", NULL, SW_SHOW);
			exit(0);
		}
		break;
	default:
		TerminateProcess(pi.hProcess, 0);
		MessageBox(NULL, "Impossible de vérifier la mise à jour, si le problème persiste, lancez le programme de mis à jour par vous-même.", "Erreur inconnue", MB_ICONERROR);
		break;
	}
}

void DestroyAll()
{
	int i;

	DestroyDirectMusic();
	DestroyDirectDraw();
	DestroyDirectInput();

	// Les panels
	if (PTest) 
		delete PTest;
	delete PMenu;
	delete PData;
	PanelPack.RemoveAllElement(0, true);

	// Les sons
	for (i = 0; i < TOTAL_MUSIC_CATEGORY; i++)
		MusicPack[i].RemoveAllElement(0, true);
	SoundPack.RemoveAllElement(0, true);

	// Les personnages
	CharPack.RemoveAllElement(0, true);

	// Les pickups
	PickupPack.RemoveAllElement(0, true);

	// Les sorts magiques
	MagicSpellPack.RemoveAllElement(0, true);

	// Le reste...
	delete LandMap;
	delete Map;
	delete cursor;
	delete TicTac;
	delete Screen;
	delete Keyboard;
	delete Mouse;
}

void DestroyDirectDraw()
{
	int i;

	//La map
	Map->GetTileData()->destroyAni();

	//Le curseur
	cursor->destroyAni();

	// Panel pour les testes
	if (PTest) PTest->DestroyText();

	//Panel PMenu
	PMenu->DestroyText();
	PMenu->destroyAni();

	//Panel de progression des données
	PData->DestroyText();
	PData->destroyAni();

	// Les autres panels
	for (i = 0; i < PanelPack.GetElementCount(); i++)
	{
		Panel::getPanelElement(i, &PanelPack)->DestroyText();
		Panel::getPanelElement(i, &PanelPack)->destroyAni();
	}

	//La carte
	LandMap->destroyAni();

	//Les personnages
	for (i = 0; i < CharPack.GetElementCount(); i++)
		Visual::getVisualElement(i, &CharPack)->destroyAni();

	//Les pickups
	for (i = 0; i < PickupPack.GetElementCount(); i++)
		Visual::getVisualElement(i, &PickupPack)->destroyAni();
	
	Screen->Destroy();
}

void DestroyDirectInput()
{
	Keyboard->Destroy();
	Mouse->Destroy();
}

void DestroyDirectMusic()
{
	int i, j;

	for (i = 0; i < TOTAL_MUSIC_CATEGORY; i++)
		for (j = 0; j < MusicPack[i].GetElementCount(); j++)
			Sound::getSoundElement(j, &MusicPack[i])->destroy();

	Sound::destroyPlayer();
}

void DispatchCreature(Zone *pZone)
{
	int i, j, CreatureTotal, AnCreatureTotal, HuCreatureTotal, WkCreatureTotal, MdCreatureTotal, StCreatureTotal, AnCreatureCount, HuCreatureCount, WkCreatureCount, MdCreatureCount, StCreatureCount;
	Axe *MapPosition = new Axe[CharPack.GetElementCount()];
	List RandomCharPack[TOTAL_LIVING_TILE_ID];

	AnCreatureCount = HuCreatureCount = WkCreatureCount = MdCreatureCount = StCreatureCount = 0;

	//Récupère les infos de la zone et définit les critères de race et de classe
	CreatureTotal = Clock::random(pZone->MinCreature, pZone->MaxCreature);
	AnCreatureTotal = CreatureTotal * pZone->AnimalCreatureRate / 100;
	HuCreatureTotal = CreatureTotal * pZone->HumanizedCreatureRate / 100;
	WkCreatureTotal = CreatureTotal * pZone->WeakCreatureRate / 100;
	MdCreatureTotal = CreatureTotal * pZone->MediumCreatureRate / 100;
	StCreatureTotal = CreatureTotal * pZone->StrongCreatureRate / 100;

	//Définit les créatures pouvant figurer dans la zone pour chaque type de terrain
	for (i = 0; i < CharPack.GetElementCount(); i++)
		if (Character::getCharacterElement(i, &CharPack)->GetBehavior()->dispatch)
		{
			for (j = 0; j < ZoneTileCount; j++)
				if (Character::getCharacterElement(i, &CharPack)->CheckLivingTile(ZoneTile[j]))
					if (Mob::getMobElement(i, &CharPack))
					{
						if (!Mob::getMobElement(i, &CharPack)->GetZoneStatus(pZone->CoordX, pZone->CoordY)->dead)
							RandomCharPack[j].AddElement(Mob::getMobElement(i, &CharPack));
					}
					else
						RandomCharPack[j].AddElement(Character::getCharacterElement(i, &CharPack));
		}
		else
			if ((TileActor::getTileActorElement(i, &CharPack)->GetTileCoord()->x != -1) && (TileActor::getTileActorElement(i, &CharPack)->GetTileCoord()->y != -1))
				MapPosition[i] = *TileActor::getTileActorElement(i, &CharPack)->GetTileCoord();

	//Créer aléatoirement le pack de personnage figurant dans la zone pour chaque type de terrain
	while (ZoneCharPack.GetElementCount() < CreatureTotal)
	{
		int FailureCount = 0;
		for (i = 0; i < ZoneTileCount; i++)
			if (RandomCharPack[i].GetElementCount())
			{
				j = Clock::random(0, RandomCharPack[i].GetElementCount() - 1);
				Character *pCharacter = Character::getCharacterElement(j, &RandomCharPack[i]);
				AddZoneChar(pCharacter, pZone, &AnCreatureTotal, &HuCreatureTotal, &WkCreatureTotal, &MdCreatureTotal, &StCreatureTotal, &AnCreatureCount, &HuCreatureCount, &WkCreatureCount, &MdCreatureCount, &StCreatureCount);
				//Enlève le personnage courant des listes de créatures pouvant figurer dans la zone pour chaque type de terrain
				for (j = 0; j < ZoneTileCount; j++)
					RandomCharPack[j].RemoveElement(pCharacter);
			}
			else
				FailureCount++;
		if (FailureCount >= ZoneTileCount)
			break;
	}

	i = 0;

	//Place les personnages de la zone sur la carte
	while (i < ZoneCharPack.GetElementCount())
		if (Character::getCharacterElement(i, &ZoneCharPack)->GetBehavior()->dispatch)
		{
			MapPosition[i] = TileActor::getTileActorElement(i, &ZoneCharPack)->GenerateZonePosition(ZoneTile, ZoneTileCount, MapPosition, i, MAX_LIVING_TILE, GROUND, false, true, false);
			if ((MapPosition[i].x != -1) && (MapPosition[i].y != -1))
			{
				TileActor::getTileActorElement(i, &ZoneCharPack)->SetMapPositionByCoord(MapPosition[i].x, MapPosition[i].y, false);
				i++;
			}
			else
				ZoneCharPack.RemoveElement(i, 1); //Enlève les personnages n'ayant pas reçut de nouvelle position
		}
		else
			i++;

	delete [] MapPosition;
}

void DispatchPickupsOnChar()
{
	int i, j, k, DispatchCount;
	bool money, drink, weapon, armor, NextPickup;
	CharSkill *BestCombatSkill, *BestArmorSkill;
	List DispatchPack;

	k = -1;
	j = DispatchCount = 0;

	for (i = 0; i < PickupPack.GetElementCount(); i++)
		DispatchPack.AddElement(PickupPack.GetElement(i));

	while (DispatchCount < PickupPack.GetElementCount())
	{
		NextPickup = true;
		if ((j >= PickupPack.GetElementCount() - DispatchCount) | (k == -1))
		{
			money = drink = weapon = armor = false;
			j = 0;
			k++;
			if (k >= ZoneCharPack.GetElementCount())
				break;
			if (Humanized::getHumanizedElement(k, &ZoneCharPack))
			{
				BestCombatSkill = Humanized::getHumanizedElement(k, &ZoneCharPack)->GetCombatSkillPack()->GetBestCombatSkill();
				BestArmorSkill = Humanized::getHumanizedElement(k, &ZoneCharPack)->GetSkillPack()->GetBestArmorSkill();
			}
		}
		if (Humanized::getHumanizedElement(k, &ZoneCharPack))
		{
			if ((Humanized::getHumanizedElement(k, &ZoneCharPack)->GetClass() == Pickup::getPickupElement(j, &DispatchPack)->GetClass()) && (Pickup::getPickupElement(j, &DispatchPack)->GetOwnerID() == -1) && (Humanized::getHumanizedElement(k, &ZoneCharPack)->GetRace() != ANIMAL) && (Humanized::getHumanizedElement(k, &ZoneCharPack)->GetBehavior()->GetPickup == true))
			{
				switch (Pickup::getPickupElement(j, &DispatchPack)->GetType())
				{
				case MONEY:
					if (!money)
					{
						money = true;
						NextPickup = false;
						Humanized::getHumanizedElement(k, &ZoneCharPack)->AddToEquipment(Pickup::getPickupElement(j, &DispatchPack));
						DispatchPack.RemoveElement(j, 1);
						DispatchCount++;
					}
					break;
				case DRINK:
					if (Ingredient::getIngredientElement(j, &DispatchPack))
						if ((drink == false) && (Ingredient::getIngredientElement(j, &DispatchPack)->GetType() == DRINK))
						{
							drink = true;
							NextPickup = false;
							Humanized::getHumanizedElement(k, &ZoneCharPack)->AddToEquipment(Pickup::getPickupElement(j, &DispatchPack));
							DispatchPack.RemoveElement(j, 1);
							DispatchCount++;
						}
					break;
				case WEAPON:
					if (Weapon::getWeaponElement(j, &DispatchPack))
						if ((weapon == false) && (Weapon::getWeaponElement(j, &DispatchPack)->GetCombatSkill() == BestCombatSkill->GetSkillId()))
						{
							weapon = true;
							NextPickup = false;
							Humanized::getHumanizedElement(k, &ZoneCharPack)->AddToEquipment(Weapon::getWeaponElement(j, &DispatchPack));
							DispatchPack.RemoveElement(j, 1);
							DispatchCount++;
						}
					break;
				case ARMOR:
					if (Armor::getArmorElement(j, &DispatchPack))
						if ((armor == false) && (Armor::getArmorElement(j, &DispatchPack)->GetArmorSkill() == BestArmorSkill->GetSkillId()))
						{
							armor = true;
							NextPickup = false;
							Humanized::getHumanizedElement(k, &ZoneCharPack)->AddToEquipment(Armor::getArmorElement(j, &DispatchPack));
							DispatchPack.RemoveElement(j, 1);
							DispatchCount++;
						}
					break;
				}
			}
		}
		if (((money == true) && (drink == true) && (weapon == true) && (armor == true)) | (Character::getCharacterElement(k, &ZoneCharPack)->GetRace() == ANIMAL) | (Character::getCharacterElement(k, &ZoneCharPack)->GetBehavior()->GetPickup == false))
			j = PickupPack.GetElementCount();
		if (NextPickup)
			j++;
	}
}

void DispatchPickupsOnZone()
{
	int i, j, x, y, *RandomTileNodeID;
	Axe *MapPosition;
	List DispatchPack, TrollPack, TrollFluidPack;

	// On définit les objets pouvants figurer dans la zone
	for (i = 0; i < PickupPack.GetElementCount(); i++)
		if ((Pickup::getPickupElement(i, &PickupPack)->GetType() == FLOWER) | (Pickup::getPickupElement(i, &PickupPack)->GetType() == FRUIT) | ((Ingredient::getIngredientElement(i, &PickupPack) != NULL) && (Ingredient::getIngredientElement(i, &PickupPack)->GetIngredientType() == DRINK_TROLLFLUID)))
			if ((Pickup::getPickupElement(i, &PickupPack)->CheckLivingTile(ZoneTileCount, ZoneTile) > 0) && (Pickup::getPickupElement(i, &PickupPack)->GetStatus()->picked == false))
			{
				DispatchPack.AddElement(Pickup::getPickupElement(i, &PickupPack));
				ZonePickupPack.AddElement(Pickup::getPickupElement(i, &PickupPack));
			}

	// On définit les trolls présents sur la carte
	for (i = 0; i < TOTAL_CHAR_HUMANIZED_TROLL; i++)
		if (ZoneCharPack.SearchElement(Troll[i]) != -1)
			TrollPack.AddElement(Troll[i]);

	// On définit les "pipis" présents sur la carte
	for (i = 0; i < TOTAL_CHAR_HUMANIZED_TROLL; i++)
		if (ZonePickupPack.SearchElement(TrollFluid[i]) != -1)
			TrollFluidPack.AddElement(TrollFluid[i]);

	MapPosition = new Axe[TOTAL_CHAR_HUMANIZED_TROLL];
	i = 0;

	// On place les "pipis" à proximités des trolls
	while (i < TrollPack.GetElementCount())
	{
		TileIA::getTileIAElement(i, &TrollPack)->GetCompass()->SetCheckObstacle(true);
		TileIA::getTileIAElement(i, &TrollPack)->GetCompass()->StartSearch(TileActor::getTileActorElement(i, &TrollPack)->GetTileNodeID(), -1);
		TileIA::getTileIAElement(i, &TrollPack)->GetCompass()->SearchByCycle(3);
		RandomTileNodeID = new int[TileIA::getTileIAElement(i, &TrollPack)->GetCompass()->GetOpens()->GetElementCount()];
		for (j = 0; j < TileIA::getTileIAElement(i, &TrollPack)->GetCompass()->GetOpens()->GetElementCount(); j++)
		{
			x = PathNode::getPathNodeElement(j, TileIA::getTileIAElement(i, &TrollPack)->GetCompass()->GetOpens())->GetZone()->x;
			y = PathNode::getPathNodeElement(j, TileIA::getTileIAElement(i, &TrollPack)->GetCompass()->GetOpens())->GetZone()->y;
			RandomTileNodeID[j] = Map->GetMapData()->TileNodeID[y][x];
		}
		if (TrollFluidPack.GetElementCount())
		{
			MapPosition[i] = TileActor::getTileActorElement(0, &TrollFluidPack)->GenerateZonePosition(RandomTileNodeID, TileIA::getTileIAElement(i, &TrollPack)->GetCompass()->GetOpens()->GetElementCount(), MapPosition, i, MAX_LIVING_TILE, GROUND, true, true, true);
			if ((MapPosition[i].x != -1) && (MapPosition[i].y != -1))
			{
				TileActor::getTileActorElement(0, &TrollFluidPack)->SetMapPositionByCoord(MapPosition[i].x, MapPosition[i].y, true);
				i++;
			}
			else
			{
				TrollPack.RemoveElement(i, 1);
				ZonePickupPack.RemoveElement(TrollFluidPack.GetElement(0));
			}
			DispatchPack.RemoveElement(TrollFluidPack.GetElement(0));
			TrollFluidPack.RemoveElement(0, 1);
		}
		else
			i++;
		delete [] RandomTileNodeID;
	}

	delete [] MapPosition;
	MapPosition = new Axe[DispatchPack.GetElementCount()];
	i = 0;

	// On place les autre objets de la zone sur la carte
	while (i < DispatchPack.GetElementCount())
	{
		MapPosition[i] = TileActor::getTileActorElement(i, &DispatchPack)->GenerateZonePosition(ZoneTile, ZoneTileCount, MapPosition, i, MAX_LIVING_TILE, GROUND, false, true, true);
		if ((MapPosition[i].x != -1) && (MapPosition[i].y != -1))
		{
			TileActor::getTileActorElement(i, &DispatchPack)->SetMapPositionByCoord(MapPosition[i].x, MapPosition[i].y, true);
			i++;
		}
		else
			ZonePickupPack.RemoveElement(i, 1); // On enlève les objets n'ayant pas reçut de nouvelle position
		DispatchPack.RemoveElement(i, 1);
	}

	delete [] MapPosition;
}

void DispatchTraderPickups(Zone *pZone)
{
	int i, j, DispatchCount;
	bool NextPickup;
	CHAR_CLASS PickupClass = WEAK;
	List DispatchPack;

	j = DispatchCount = 0;

	if (pZone->MediumCreatureRate > PickupClass)
		PickupClass = MEDIUM;
	if (pZone->StrongCreatureRate > PickupClass)
		PickupClass = STRONG;

	for (i = 0; i < PickupPack.GetElementCount(); i++)
		DispatchPack.AddElement(PickupPack.GetElement(i));

	while (DispatchCount < PickupPack.GetElementCount())
	{
		NextPickup = true;
		if (j >= PickupPack.GetElementCount() - DispatchCount)
			break;
		if (Trader->GetChargePoints()->GetAmount() < Trader->GetChargePoints()->GetMaxAmount() - (Trader->GetChargePoints()->GetMaxAmount() / 5))
			if ((Pickup::getPickupElement(j, &DispatchPack)->GetClass() != PNJ1) && (Pickup::getPickupElement(j, &DispatchPack)->GetClass() != NONE_CHAR_CLASS) && ((Pickup::getPickupElement(j, &DispatchPack)->GetClass() == PickupClass) | (Pickup::getPickupElement(j, &DispatchPack)->GetGroup() == OTHERS) | (Pickup::getPickupElement(j, &DispatchPack)->GetGroup() == INGREDIENTS)) && (Pickup::getPickupElement(j, &DispatchPack)->GetOwnerID() == -1))
			{
				if (Potion::getPotionElement(j, &DispatchPack))
				{
					if (Potion::getPotionElement(j, &DispatchPack)->GetEffect())
					{
						NextPickup = false;
						Trader->AddToEquipment(Potion::getPotionElement(j, &DispatchPack));
						DispatchPack.RemoveElement(j, 1);
						DispatchCount++;
					}
				}
				else
					if ((Pickup::getPickupElement(j, &DispatchPack)->GetType() == DRINK) | (Pickup::getPickupElement(j, &DispatchPack)->GetType() == WEAPON) | (Pickup::getPickupElement(j, &DispatchPack)->GetType() == ARMOR))
					{
						NextPickup = false;
						Trader->AddToEquipment(Pickup::getPickupElement(j, &DispatchPack));
						DispatchPack.RemoveElement(j, 1);
						DispatchCount++;
					}
			}
		if (NextPickup)
			j++;
	}
}

void DisplayPanel(SHORT_PANEL ShortKey)
{
	switch (ShortKey)
	{
	case SHORT_INVENTORY:
		if (!PInventory->hide())
		{
			PInventory->show();
			PInventory->Refresh();
		}
		break;
	case SHORT_CHARINFO:
		if (!PCharInfo->hide())
		{
			PCharInfo->show();
			PCharInfo->Refresh();
		}
		break;
	case SHORT_SPELLLIST:
		if (!PSpellList->hide())
		{
			PSpellList->show();
			PSpellList->Refresh();
		}
		break;
	case SHORT_CHEMICALLAB:
		if (!PChemicalLab->hide())
		{
			PChemicalLab->show();
			PChemicalLab->Refresh();
		}
		break;
	case SHORT_LANDMAP:
		if (!LandMap->hide())
			LandMap->show();
		break;
	case SHORT_QUESTBOOK:
		if (!PBook->hide())
		{
			PBook->show();
			PBook->Refresh();
		}
		break;
	case SHORT_EVENTS:
		if (!PEvents->hide())
		{
			PEvents->show();
			PEvents->Refresh();
		}
		break;
	case SHORT_MENU:
		if (PMenu->getVisible())
		{
			if (Hero->GetStatPack()->getCharStatElement(0)->GetScore())
				PMenu->hide();
		}
		else
		{
			PMenu->show();
			PMenu->Refresh();
			Hero->GetStatus()->access = false;
		}
		break;
	}
}

void CALLBACK GenerateDataProgress(int CB_IndexCount)
{
	PData->GetDataBar()->SetAmount((float)CB_IndexCount);
	PData->GetDataBar()->GenerateBar();
}

Zone * GetCurrentZone()
{
	int x, y;

	for (x = 0; x < TOTAL_ZONE_X; x++)
		for (y = 0; y < TOTAL_ZONE_Y; y++)
			if ((ZoneData[y][x].CoordX == ZoneCoord.x) && (ZoneData[y][x].CoordY == ZoneCoord.y))
				return &ZoneData[y][x];

	return NULL;
}

void GiveMagicSpellToChar(Humanized *pHumanized)
{
	int i, j, SchoolIndex;
	CharStat *BestSchool;
	List *GiftPack;

	GiftPack = new List[pHumanized->GetMagicSchoolPack()->GetElementCount()];

	for (i = 0; i < MagicSpellPack.GetElementCount(); i++)
	{
		SchoolIndex = MagicSpellPack.getMagicSpellElement(i)->GetSchool()-1;
		GiftPack[SchoolIndex].AddElement(MagicSpellPack.getMagicSpellElement(i));
	}

	i = 0;
	BestSchool = pHumanized->GetMagicSchoolPack()->GetBestStat();
	SchoolIndex = BestSchool->GetStatId() - 1;

	while (pHumanized->GetMagicPoints())
	{
		if (GiftPack[SchoolIndex].GetElementCount())
		{
			MagicSpell *pMagicSpell = MagicSpell::getMagicSpellElement(i, &GiftPack[SchoolIndex]);
			if ((pMagicSpell != NULL) && (pHumanized->GetMagicSkill()->GetLevel() >= pMagicSpell->GetLevel()))
			{
				pHumanized->GetMagicSpellPack(SchoolIndex)->AddElement(new MagicSpell(*pMagicSpell));
				pHumanized->SetMagicPoints(pHumanized->GetMagicPoints() - SPELL_PRICE);
			}
			GiftPack[SchoolIndex].RemoveElement(i, 1);
		}
		if (!GiftPack[SchoolIndex].GetElementCount())
		{
			SchoolIndex++;
			if (SchoolIndex == pHumanized->GetMagicSchoolPack()->GetElementCount())
				SchoolIndex = 0;
			i = 0;
		}
		for (j = 0; j < pHumanized->GetMagicSchoolPack()->GetElementCount(); j++)
			if (GiftPack[j].GetElementCount())
				break;
		if (j == pHumanized->GetMagicSchoolPack()->GetElementCount())
			if (pHumanized->GetMagicPoints())
			{
				int n = pHumanized->GetMagicPoints() / 10;
				int max = MAX_CHAR_STAT_SCORE - pHumanized->GetMagicSchoolPack()->GetStat(BestSchool->GetStatId())->GetScore();
				if (n > max)
					n = max;
				pHumanized->SetMagicPoints(0);
				pHumanized->GetMagicSchoolPack()->GetStat(BestSchool->GetStatId())->SetScore(pHumanized->GetMagicSchoolPack()->GetStat(BestSchool->GetStatId())->GetScore() + n);
			}
	}

	delete [] GiftPack;
}

void GivePickupToChar()
{
	int i;

	if (Daggy[2]->GetOwnerID() == -1)			GreatMago->AddToEquipment(Daggy[2]);
	if (MagicianDress[2]->GetOwnerID() == -1)	GreatMago->AddToEquipment(MagicianDress[2]);
	if (MagicCircle->GetOwnerID() == -1)		Captain->AddToEquipment(MagicCircle);
	if (MindProtector->GetOwnerID() == -1)		Captain->AddToEquipment(MindProtector);
	if (Daggy[0]->GetOwnerID() == -1)			Wise->AddToEquipment(Daggy[0]);
	if (MagicianDress[0]->GetOwnerID() == -1)	Wise->AddToEquipment(MagicianDress[0]);
	if (Axel->GetOwnerID() == -1)				Balrogue->AddToEquipment(Axel);
	if (Sylvanel->GetOwnerID() == -1)			Balrogue->AddToEquipment(Sylvanel);
	if (Daggy[1]->GetOwnerID() == -1)			Wizard->AddToEquipment(Daggy[1]);
	if (WizardStaff->GetOwnerID() == -1)		Wizard->AddToEquipment(WizardStaff);
	if (MagicianDress[1]->GetOwnerID() == -1)	Wizard->AddToEquipment(MagicianDress[1]);

	for (i = 0; i < TOTAL_PICKUP_POTION_DF; i++)
		if (DiscretionFlow[i]->GetOwnerID() == -1)
			Leader->AddToEquipment(DiscretionFlow[i]);
}

void InitCharacter(Character *pCharacter, int BasicStatScore, int BasicSkillLevel, int MinStatBonus, int MaxStatBonus, int MinFavoriteSkillLevel, int MaxFavoriteSkillLevel, int MinMagicSkillLevel, int MaxMagicSkillLevel)
{
	int i, j, k[3];
	Humanized *pHumanized = dynamic_cast<Humanized*>(pCharacter);
	Pnj *pPnj = dynamic_cast<Pnj*>(pCharacter);

	for (i = 0; i < pCharacter->GetStatPack()->GetElementCount(); i++)
		pCharacter->GetStatPack()->getCharStatElement(i)->SetScore(BasicStatScore);

	k[0] = pCharacter->GetCombatSkillPack()->SearchElementBySkillId(pCharacter->GetBehavior()->FavoriteCombatSkill);

	//Définit la compétence de combat favorite et ajoute un bonus à ses stats associées
	for (i = 0; i < pCharacter->GetCombatSkillPack()->GetElementCount(); i++)
		if (i == k[0])
		{
			pCharacter->GetCombatSkillPack()->getCharSkillElement(i)->SetLevel(Clock::random(MinFavoriteSkillLevel, MaxFavoriteSkillLevel));
			for (j = 0; j < pCharacter->GetCombatSkillPack()->getCharSkillElement(i)->GetStatPack()->GetElementCount(); j++)
			{
				k[1] = pCharacter->GetCombatSkillPack()->getCharSkillElement(i)->GetStatPack()->getCharStatElement(j)->GetStatId();
				pCharacter->GetStatPack()->GetStat(k[1])->SetScore(pCharacter->GetStatPack()->GetStat(k[1])->GetScore() + Clock::random(MinStatBonus, MaxStatBonus));
			}
		}
		else
			pCharacter->GetCombatSkillPack()->getCharSkillElement(i)->SetLevel(BasicSkillLevel);

	if (pHumanized)
	{
		for (i = 0; i < Hero->GetMagicSchoolPack()->GetElementCount(); i++)
			pHumanized->GetMagicSchoolPack()->getCharStatElement(i)->SetScore(BasicStatScore);
		k[0] = pHumanized->GetSkillPack()->SearchElementBySkillId(pHumanized->GetBehavior()->FavoriteSkill1);
		k[1] = pHumanized->GetSkillPack()->SearchElementBySkillId(pHumanized->GetBehavior()->FavoriteSkill2);
		pHumanized->GetMagicSkill()->SetLevel(BasicSkillLevel);
		pHumanized->GetMagicResistSkill()->SetLevel(BasicSkillLevel);
		for (i = 0; i < pHumanized->GetSkillPack()->GetElementCount(); i++)
		{
			pHumanized->GetSkillPack()->getCharSkillElement(i)->SetLevel(BasicSkillLevel);
			//Définit les compétences favorites et ajoute un bonus à leurs stats associées
			if ((i == k[0]) | (i == k[1]))
			{
				pHumanized->GetSkillPack()->getCharSkillElement(i)->SetLevel(Clock::random(MinFavoriteSkillLevel, MaxFavoriteSkillLevel));
				for (j = 0; j < pHumanized->GetSkillPack()->getCharSkillElement(i)->GetStatPack()->GetElementCount(); j++)
				{
					k[2] = pHumanized->GetSkillPack()->getCharSkillElement(i)->GetStatPack()->getCharStatElement(j)->GetStatId();
					pHumanized->GetStatPack()->GetStat(k[2])->SetScore(pHumanized->GetStatPack()->GetStat(k[2])->GetScore() + Clock::random(MinStatBonus, MaxStatBonus));
				}
				if (pHumanized->GetSkillPack()->getCharSkillElement(i)->GetSkillId() == MEDITATION)
				{
					int level = Clock::random(MinMagicSkillLevel, MaxMagicSkillLevel);
					pHumanized->GetMagicSkill()->SetLevel(level);
					pHumanized->GetMagicResistSkill()->SetLevel(level);
					if (pHumanized->IsAPlant())
						pHumanized->GetMagicSchoolPack()->GetStat(DESTRUCTION)->SetScore(pHumanized->GetMagicSchoolPack()->GetStat(DESTRUCTION)->GetScore() + Clock::random(MinStatBonus, MaxStatBonus));
					else
						for (j = 0; j < pHumanized->GetMagicSchoolPack()->GetElementCount(); j++)
							pHumanized->GetMagicSchoolPack()->getCharStatElement(j)->SetScore(pHumanized->GetMagicSchoolPack()->getCharStatElement(j)->GetScore() + Clock::random(MinStatBonus, MaxStatBonus));
				}
			}
		}
		pHumanized->SetMagicPoints(pHumanized->GetMagicSkill()->GetLevel() * MAGIC_POINTS / 2);
	}

	if (pPnj)
		if (pPnj->GetPNJJob() == MAGIC_LEARNER)
			pPnj->SetMagicPoints(MagicSpellPack.GetElementCount() * SPELL_PRICE);

	pCharacter->GenerateStat();
	pCharacter->GenerateStat2();

	if (pCharacter->GetClass() == PNJ1)
		pCharacter->GetHitPoints()->SetAmount((float)pCharacter->GetStat2Pack()->GetStat(HIT_POINTS)->GetScore());
	else
		pCharacter->GetHitPoints()->SetAmount((float)Clock::random(pCharacter->GetStat2Pack()->GetStat(HIT_POINTS)->GetScore() / 2, pCharacter->GetStat2Pack()->GetStat(HIT_POINTS)->GetScore()));

	pCharacter->GetHitPoints()->GenerateBar();

	if (pHumanized)
	{
		pHumanized->GetManaPoints()->SetAmount((float)pHumanized->GetStat2Pack()->GetStat(MANA_POINTS)->GetScore());
		pHumanized->GetManaPoints()->GenerateBar();
	}

	pCharacter->InitOrientation();
}

bool InitCombat()
{
	int i, j;
	DataGroup *Element;
	DataGroupList CombatTable;

	if (Rules.GetStatus() != DataFileList::OPENED_READONLY)
		return false;

	if (!Rules.CreateDataGroupList("CharCombat", &CombatTable))
		return false;

	for (i = 0; i < CombatTable.GetElementCount(); i++)
	{
		Element = DataGroup::getDataGroupElement(i, &CombatTable);
		CombatData[i].MinCS = Str::ToInt(Element->GetValue("MinCS"));
		CombatData[i].MaxCS = Str::ToInt(Element->GetValue("MaxCS"));
		CombatData[i].MaxDistance = Str::ToInt(Element->GetValue("MaxDistance"));
		for (j = 0; j < TOTAL_COMBAT_PHASE; j++)
			CombatData[i].action[j] = Str::ToInt(Element->GetValue(BuildString("phase%i", j+1)));
	}

	return true;
}

void InitCreatureOnZone()
{
	int i;

	for (i = 0; i < ZoneCharPack.GetElementCount(); i++)
		if (Mob::getMobElement(i, &ZoneCharPack))
		{
			switch (Character::getCharacterElement(i, &ZoneCharPack)->GetClass())
			{
			case WEAK:
				InitCharacter(Character::getCharacterElement(i, &ZoneCharPack), 1, 1, 1, 1, 2, 3, 3, 3);
				break;
			case MEDIUM:
				InitCharacter(Character::getCharacterElement(i, &ZoneCharPack), 4, 3, 1, 2, 4, 8, 6, 9);
				break;
			case STRONG:
				InitCharacter(Character::getCharacterElement(i, &ZoneCharPack), 6, 5, 2, 4, 9, MAX_CHAR_SKILL_LEVEL, MAX_CHAR_MAGIC_LEVEL, MAX_CHAR_MAGIC_LEVEL);
				break;
			case PNJ1:
				InitCharacter(Character::getCharacterElement(i, &ZoneCharPack), 7, 6, 3, 3, 10, MAX_CHAR_SKILL_LEVEL, MAX_CHAR_MAGIC_LEVEL, MAX_CHAR_MAGIC_LEVEL);
				break;
			case PNJ2:
				InitCharacter(Character::getCharacterElement(i, &ZoneCharPack), 1, 1, 1, 1, 2, 3, 2, 3);
				break;
			}
			Character::getCharacterElement(i, &ZoneCharPack)->UpdateCombat(CombatData);
			if (Humanized::getHumanizedElement(i, &ZoneCharPack))
			{
				GiveMagicSpellToChar(Humanized::getHumanizedElement(i, &ZoneCharPack));
				Humanized::getHumanizedElement(i, &ZoneCharPack)->UpdateMagicSpell();
				Humanized::getHumanizedElement(i, &ZoneCharPack)->GenerateTitle();
			}
		}
}

void InitDirectDraw()
{
	int i;

	// La map
	Map->GetTileData()->addAni(IDB_MAP_GROUND, "", 0, 0, 0, RGB(255, 255, 255), 90);
	Map->GetTileData()->addAni(IDB_MAP_DECORATION, "", 0, 0, 0, RGB(255, 255, 255), 90);
	Map->GetTileData()->addAni(IDB_MAP_ITEMS, "", 0, 0, 0, RGB(255, 255, 255), 90);
	Map->GetTileData()->addAni(IDB_MAP_BRIDGES, "", 0, 0, 0, RGB(255, 255, 255), 90);
#ifdef _DEBUG
	Map->GetTileData()->addAni(IDB_MAP_PATHNODES, "", 0, 0, 0, RGB(255, 255, 255), 90);
#endif

	// Le curseur
	cursor->addAni(IDB_CURSOR, "", 1, 20, 24, RGB(255, 255, 255), 90);

	// Panel pour les testes
	if (PTest) PTest->Initialize();

	// Panel Menu
	PMenu->initAni();
	PMenu->Initialize();

	// Panel de progression des données
	PData->initAni();
	PData->Initialize();

	// Panels liste de sorts
	PSpellList->InitCloseButton(IDB_PANEL_CLOSE, "", 16, 16);

	// Les autres panels
	for (i = 0; i < PanelPack.GetElementCount(); i++)
	{
		Panel::getPanelElement(i, &PanelPack)->InitBackground(IDB_PANEL_BKG, "", 16, 16);
		Panel::getPanelElement(i, &PanelPack)->initBorders(IDB_PANEL_BORDER, "", 61, 6);
		Panel::getPanelElement(i, &PanelPack)->initAni();
		Panel::getPanelElement(i, &PanelPack)->Initialize();
	}

	//La carte
	LandMap->addAni(IDB_LANDMAP, "", 1, LandMap->getDimension()->x, LandMap->getDimension()->y, -1, 90);
	LandMap->AddPicMap(IDB_ELEVATOR);
	LandMap->AddPicMap(IDB_SELECT_BUTTONS);
}

void InitDirectInput()
{
	Keyboard->CreateDevice(GUID_SysKeyboard, &c_dfDIKeyboard, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	Mouse->CreateDevice(GUID_SysMouse, &c_dfDIMouse, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
}

void InitDirectMusic()
{
	int i;
	HANDLE hFind;
	WIN32_FIND_DATA FileData;
	Str CategoryPath, FilePath;
	Sound *pSound;

	Sound::initPlayer(Screen->GetDxWnd(), DMUS_APATH_DYNAMIC_STEREO, 64, DMUS_AUDIOF_ALL);

	// Pour chaque catégorie de musique, on charge tous les fichiers contenus dans le répertoire de cette catégorie
	for (i = 0; i < TOTAL_MUSIC_CATEGORY; i++)
	{
		switch ((MUSIC_CATEGORY)i)
		{
		case MC_OUTLAND:
			CategoryPath = "Musics/Outland/";
			break;
		case MC_VILLAGE:
			CategoryPath = "Musics/Village/";
			break;
		case MC_FOREST:
			CategoryPath = "Musics/Forest/";
			break;
		case MC_MONTAIN:
			CategoryPath = "Musics/Montain/";
			break;
		case MC_SNOW:
			CategoryPath = "Musics/Snow/";
			break;
		case MC_WIZARD_FORTRESS:
			CategoryPath = "Musics/Wizard Fortress/";
			break;
		default:
			CategoryPath = "Musics/";
			break;
		}
		hFind = FindFirstFile(CategoryPath + "*.mid", &FileData);
		if (hFind != INVALID_HANDLE_VALUE)
			do
			{
				FilePath = CategoryPath + FileData.cFileName;
				pSound = new Sound();
				pSound->load(FilePath, true);
				MusicPack[i].AddElement(pSound);
			}
			while (FindNextFile(hFind, &FileData));
		FindClose(hFind);
	}

	CategoryPath = "Sounds/";
	hFind = FindFirstFile(CategoryPath + "*.wav", &FileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			FilePath = CategoryPath + FileData.cFileName;
			pSound = new Sound();
			pSound->load(FilePath);
			SoundPack.AddElement(pSound);
		}
		while (FindNextFile(hFind, &FileData));
	}

	SoundPack.OrderBy(List::ORDER_BY_NAME);

	FindClose(hFind);
}

void InitIngredients()
{
	int i, j, CID[TOTAL_PICKUP_POTION_EFFECT-1], RandomCID[TOTAL_PICKUP_INGREDIENT_TYPE];
	List RandomPack;

	for (i = 0; i < TOTAL_PICKUP_POTION_EFFECT - 1; i++)
		CID[i] = CHEMICAL_SCALE * i;

	for (i = 0; i < TOTAL_PICKUP_INGREDIENT_TYPE; i++)
		RandomCID[i] = Clock::random(0, 20);

	RandomPack.AddElement(WhiteFlower[0]);
	RandomPack.AddElement(RedFlower[0]);
	RandomPack.AddElement(BlueFlower[0]);

	j = 0;

	while (j++ < TOTAL_PICKUP_POTION_EFFECT - 2)
	{
		i = Clock::random(0, RandomPack.GetElementCount() - 1);
		Ingredient::getIngredientElement(i, &RandomPack)->SetChemicalID(CID[j]);
		RandomPack.RemoveElement(i, 1);
	}

	for (i = 0; i < FlowerPack.GetElementCount(); i++)
	{
		switch (Ingredient::getIngredientElement(i, &FlowerPack)->GetIngredientType())
		{
		case FLOWER_WHITE:
			Ingredient::getIngredientElement(i, &FlowerPack)->SetChemicalID(WhiteFlower[0]->GetChemicalID());
			break;
		case FLOWER_RED:
			Ingredient::getIngredientElement(i, &FlowerPack)->SetChemicalID(RedFlower[0]->GetChemicalID());
			break;
		case FLOWER_BLUE:
			Ingredient::getIngredientElement(i, &FlowerPack)->SetChemicalID(BlueFlower[0]->GetChemicalID());
			break;
		}
		Ingredient::getIngredientElement(i, &FlowerPack)->GenerateEffectFactor();
		Ingredient::getIngredientElement(i, &FlowerPack)->GenerateLivingTile();
	}

	for (i = 0; i < FruitPack.GetElementCount(); i++)
	{
		Ingredient::getIngredientElement(i, &FruitPack)->SetChemicalID(RandomCID[Ingredient::getIngredientElement(i, &FruitPack)->GetIngredientType()]);
		Ingredient::getIngredientElement(i, &FruitPack)->GenerateEffectFactor();
		Ingredient::getIngredientElement(i, &FruitPack)->AddLivingTile(1);
		Ingredient::getIngredientElement(i, &FruitPack)->AddLivingTile(7);
	}

	for (i = 0; i < DrinkPack.GetElementCount(); i++)
	{
		Ingredient::getIngredientElement(i, &DrinkPack)->SetChemicalID(RandomCID[Ingredient::getIngredientElement(i, &DrinkPack)->GetIngredientType()]);
		Ingredient::getIngredientElement(i, &DrinkPack)->GenerateEffectFactor();
	}
}

bool InitMagicSpellPack()
{
	int i;
	DataGroup *Element;
	MagicSpell *Spell;
	DataGroupList SpellTable;

	if (Rules.GetStatus() != DataFileList::OPENED_READONLY)
		return false;

	if (!Rules.CreateDataGroupList("CharMagicSpell", &SpellTable))
		return false;

	MagicSpellPack.RemoveAllElement(0, true);

	for (i = 0; i < SpellTable.GetElementCount(); i++)
	{
		Element = DataGroup::getDataGroupElement(i, &SpellTable);
		Spell = new MagicSpell();
		Spell->SetSchool(Str::ToInt(Element->GetValue("ex_school")));
		Spell->SetLevel(Str::ToInt(Element->GetValue("level")));
		Spell->SetDifficulty(Str::ToInt(Element->GetValue("difficulty")));
		Spell->SetMinDifficulty(Str::ToInt(Element->GetValue("MinDifficulty")));
		Spell->SetCastTime(Str::ToInt(Element->GetValue("CastTime")));
		Spell->GetDuration()->data = Str::ToInt(Element->GetValue("duration"));
		Spell->GetRange()->data = Str::ToInt(Element->GetValue("range"));
		Spell->GetMinEffect()->data = Str::ToInt(Element->GetValue("MinEffect"));
		Spell->GetMaxEffect()->data = Str::ToInt(Element->GetValue("MaxEffect"));
		Spell->SetEffect((CHAR_MAGIC_SPELL_EFFECT)Str::ToInt(Element->GetValue("effect")));
		Spell->SetCheckTurn(Str::ToInt(Element->GetValue("CheckTurn")));
		Spell->SetDescription(Element->GetValue("Description"));
		if (Spell->GetMinEffect()->data)
			Spell->GetMinEffect()->flag = true;
		else
			Spell->GetDuration()->flag = true;
		switch (Spell->GetEffect())
		{
		case SELF_HEAL:
			Spell->SetHitSound((Sound*)SoundPack.GetElement(SPELL_EFFECT_HEAL));
			break;
		case LIFE_DAMAGE:
			Spell->SetHitSound((Sound*)SoundPack.GetElement(SPELL_EFFECT_DAMAGE));
			break;
		case CHOC_WAVE:
			Spell->SetHitSound((Sound*)SoundPack.GetElement(SPELL_EFFECT_WAVE));
			break;
		case ANTIMAGIC:
			Spell->SetHitSound((Sound*)SoundPack.GetElement(SPELL_EFFECT_ANTIMAGIC));
			break;
		case ABSORBING:
			Spell->SetHitSound((Sound*)SoundPack.GetElement(SPELL_EFFECT_PROTECTION));
			break;
		case REFLECTING:
			Spell->SetHitSound((Sound*)SoundPack.GetElement(SPELL_EFFECT_PROTECTION));
			break;
		}
		MagicSpellPack.AddElement(Spell);
	}

	return true;
}

void InitMessages()
{
	InitDXErrorMsg();

	EventsReporter.GetMessagePack(BUY_PICKUP_DENIED)->AddElement(new Msg(TRADING_MSG_COLOR, "Cet objet ne peut pas être échangé"));
	EventsReporter.GetMessagePack(BUY_PICKUP_DENIED)->AddElement(new Msg(TRADING_MSG_COLOR, "Cet objet ne peut pas être échangé car son propriétaire le porte actuellement"));

	EventsReporter.GetMessagePack(LEARN_SPELL_DENIED)->AddElement(new Msg(LEARN_SPELL_MSG_COLOR, "Vous ne pouvez pas apprendre ce sort car vous le savez déjà"));
	EventsReporter.GetMessagePack(LEARN_SPELL_DENIED)->AddElement(new Msg(LEARN_SPELL_MSG_COLOR, "Vous ne pouvez pas apprendre ce sort car votre niveau de magie est trop bas"));
	EventsReporter.GetMessagePack(LEARN_SPELL_DENIED)->AddElement(new Msg(LEARN_SPELL_MSG_COLOR, "Vous ne pouvez pas apprendre ce sort car vous n'avez pas assez de points magiques"));
	EventsReporter.GetMessagePack(LEARN_SPELL_DENIED)->AddElement(new Msg(LEARN_SPELL_MSG_COLOR, "Vous n'avez pas le sort nécessaire pour déjà acheter celui-là"));

	EventsReporter.GetMessagePack(CAST_SPELL_DENIED)->AddElement(new Msg(CAST_SPELL_MSG_COLOR, "Vous n'avez pas assez de points de mana pour jeter ce sort"));
	EventsReporter.GetMessagePack(CAST_SPELL_DENIED)->AddElement(new Msg(CAST_SPELL_MSG_COLOR, "Vous n'avez pas le niveau requis pour jeter ce sort"));
	EventsReporter.GetMessagePack(CAST_SPELL_DENIED)->AddElement(new Msg(CAST_SPELL_MSG_COLOR, "Vous n'avez aucun sort mémorisé"));
	EventsReporter.GetMessagePack(CAST_SPELL_DENIED)->AddElement(new Msg(CAST_SPELL_MSG_COLOR, "Cible invalide"));
	EventsReporter.GetMessagePack(CAST_SPELL_DENIED)->AddElement(new Msg(CAST_SPELL_MSG_COLOR, "Votre cible est trop loin pour lui jeter ce sort"));
	EventsReporter.GetMessagePack(CAST_SPELL_DENIED)->AddElement(new Msg(CAST_SPELL_MSG_COLOR, "Vous ne pouvez pas vous concentrer"));

	EventsReporter.GetMessagePack(XP_GAIN_DENIED)->AddElement(new Msg(XP_MSG_COLOR, "Vous avez déjà espionné cette créature, vous ne recevez donc pas d'expérience"));
	EventsReporter.GetMessagePack(XP_GAIN_DENIED)->AddElement(new Msg(XP_MSG_COLOR, "Vous ne pouvez progresser qu'une fois par jour en Méditation"));

	EventsReporter.GetMessagePack(ACTION_DENIED)->AddElement(new Msg(ACTION_MSG_COLOR, "Vous ne pouvez pas jeter cet objet"));
	EventsReporter.GetMessagePack(ACTION_DENIED)->AddElement(new Msg(ACTION_MSG_COLOR, "Vous ne pouvez pas créer de potion sans tous les ingédients nécessaires"));
	EventsReporter.GetMessagePack(ACTION_DENIED)->AddElement(new Msg(ACTION_MSG_COLOR, "Vous devez avoir du mana pour consommer cet objet"));
	EventsReporter.GetMessagePack(ACTION_DENIED)->AddElement(new Msg(ACTION_MSG_COLOR, "Vous n'êtes pas fatigué..."));
	EventsReporter.GetMessagePack(ACTION_DENIED)->AddElement(new Msg(ACTION_MSG_COLOR, "Vous êtes trop loin de ce personnage, approchez-vous"));
	EventsReporter.GetMessagePack(ACTION_DENIED)->AddElement(new Msg(ACTION_MSG_COLOR, "Vous ne pouvez pas faire ça avec un animal"));
	EventsReporter.GetMessagePack(ACTION_DENIED)->AddElement(new Msg(ACTION_MSG_COLOR, "Vous ne pouvez pas faire ça pendant un combat"));
	EventsReporter.GetMessagePack(ACTION_DENIED)->AddElement(new Msg(ACTION_MSG_COLOR, "L'échange à àchouer"));
	EventsReporter.GetMessagePack(ACTION_DENIED)->AddElement(new Msg(ACTION_MSG_COLOR, "Vous ne pouvez pas vous équiper de cet objet"));
	EventsReporter.GetMessagePack(ACTION_DENIED)->AddElement(new Msg(ACTION_MSG_COLOR, "Cette action est interdite"));
	EventsReporter.GetMessagePack(ACTION_DENIED)->AddElement(new Msg(ACTION_MSG_COLOR, "Ce personnage est occupé poue le moment"));

	EventsReporter.GetMessagePack(SAVE_DATA)->AddElement(new Msg(ACTION_MSG_COLOR, "Les données ont été sauvées"));
	EventsReporter.GetMessagePack(SAVE_DATA)->AddElement(new Msg(ACTION_MSG_COLOR, "Les données ont été restaurées"));
	EventsReporter.GetMessagePack(SAVE_DATA)->AddElement(new Msg(ACTION_MSG_COLOR, "Des erreurs se sont produites lors de la sauvegarde des données"));
	EventsReporter.GetMessagePack(SAVE_DATA)->AddElement(new Msg(ACTION_MSG_COLOR, "Des erreurs se sont produites lors de la restauration des données"));
}

void InitObjects()
{
	int i;

	EventsReporter.SetLastToFirst(true);

	// Mapping des touches configurables
	Config.GetKeyMap()->MapName("Aller à gauche", KEY_LEFT);
	Config.GetKeyMap()->MapName("Aller à droite", KEY_RIGHT);
	Config.GetKeyMap()->MapName("Aller en haut", KEY_UP);
	Config.GetKeyMap()->MapName("Aller en bas", KEY_DOWN);
	Config.GetKeyMap()->MapName("Toujours marcher", KEY_ALWAYSWALK);
	Config.GetKeyMap()->MapName("Attaquer", KEY_ATTACK);
	Config.GetKeyMap()->MapName("Lancer un sort", KEY_CASTSPELL);
	Config.GetKeyMap()->MapName("Méditer", KEY_MEDITATE);
	Config.GetKeyMap()->MapName("Prendre un objet au sol", KEY_PICKUP);
	Config.GetKeyMap()->MapName("Passer un tour", KEY_NEXTTURN);
	Config.GetKeyMap()->MapName("Fenêtre des raccourcis", KEY_SHORTCUT);
	Config.GetKeyMap()->MapName("Fenêtre inventaire", KEY_SHORT_INVENTORY);
	Config.GetKeyMap()->MapName("Fenêtre des compétences", KEY_SHORT_CHARINFO);
	Config.GetKeyMap()->MapName("Fenêtre des sorts", KEY_SHORT_SPELLLIST);
	Config.GetKeyMap()->MapName("Fenêtre de chimie", KEY_SHORT_CHEMICALLAB);
	Config.GetKeyMap()->MapName("Fenêtre de la carte", KEY_SHORT_LANDMAP);
	Config.GetKeyMap()->MapName("Fenêtre du journal", KEY_SHORT_QUESTBOOK);
	Config.GetKeyMap()->MapName("Fenêtre des événements", KEY_SHORT_EVENTS);
	Config.GetKeyMap()->OrderBy(List::ORDER_BY_NUMINDEX);

#ifdef NDEBUG
	//Active le filtre pour les messages de debug
	EventsReporter.FilterMessage(DEBUG_ACTION);
#endif

	//Les personnages
	for (i = 0; i < CharPack.GetElementCount(); i++)
	{
		if (TileIA::getTileIAElement(i, &CharPack))
			Item::getItemElement(i, &CharPack)->setRemoved(true);
		*Item::getItemElement(i, &CharPack)->getDimension() = Axe(23, 23);
		TileActor::getTileActorElement(i, &CharPack)->SetMap(Map);
		Character::getCharacterElement(i, &CharPack)->Init();
		Character::getCharacterElement(i, &CharPack)->initAni();
		Character::getCharacterElement(i, &CharPack)->InitSound(&SoundPack);
	}

	for (i = 0; i < GreatMago->GetMagicSchoolPack()->GetElementCount(); i++)
		GreatMago->GetMagicSpellPack(i)->SetOrder(MagicSpellList::ORDER_BY_LEVEL);

	//Les pickups
	for (i = 0; i < PickupPack.GetElementCount(); i++)
	{
		Item::getItemElement(i, &PickupPack)->setRemoved(true);
		TileActor::getTileActorElement(i, &PickupPack)->SetMap(Map);
		InitPickup(Pickup::getPickupElement(i, &PickupPack));
		Pickup::getPickupElement(i, &PickupPack)->initAni();
	}

	InitIngredients();

	// Le curseur
	*cursor->getDimension() = Axe(1, 1);
	*cursor->getMover()->getDimension() = Axe(10, 10);
}

bool InitORC()
{
	int i, j;
	DataGroup *Element;
	DataGroupList ORCTable[TOTAL_CHAR_SKILL_RESULT];

	if (Rules.GetStatus() != DataFileList::OPENED_READONLY)
		return false;

	Rules.CreateDataGroupList("ORC_CRITICAL_SUCCESS", &ORCTable[CRITICAL_SUCCESS]);
	Rules.CreateDataGroupList("ORC_SUCCESS", &ORCTable[SUCCESS]);
	Rules.CreateDataGroupList("ORC_FAILURE", &ORCTable[FAILURE]);
	Rules.CreateDataGroupList("ORC_CRITICAL_FAILURE", &ORCTable[CRITICAL_FAILURE]);

	//Initialise la structure ORCData avec toutes les valeurs de la ORC depuis la base de règles
	//Ref: "The Freestyle RPG Basic Rules", page 16.
	for (i = 1; i < TOTAL_CHAR_SKILL_RESULT; i++)
		for (j = 0; j < ORCTable[i].GetElementCount(); j++)
		{
			Element = DataGroup::getDataGroupElement(j, &ORCTable[i]);
			ORCData[i].SRS[j+1] = Str::ToInt(Element->GetValue("SRS"));
			ORCData[i].min[j+1] = Str::ToInt(Element->GetValue("min"));
			ORCData[i].max[j+1] = Str::ToInt(Element->GetValue("max"));
		}

	return true;
}

void InitPanels()
{
	if (PTest) PTest->Init();

	// Menu
	PMenu->Init();

	// Progression des données
	PData->Init();

	// Panels personnages
	PCharInfo->SetFocus(Hero);

	// Chimie
	PChemicalLab->SetChemist(Hero);

	// Grand mage
	PGreatMago->SetFocus(GreatMago);

	// Objet
	PPickup->SetObserver(Hero);

	// Sort
	PMagicSpell->SetObserver(Hero);

	// Liste des sorts
	PSpellList->SetMagician(Hero);

	// Journal des quêtes
	PBook->SetOrder(QuestBook::ORDER_BY_DAY);
	PBook->SetLastToFirst(true);

	LoadPanels(&PanelPack);

	// Les gauges
	PGauge->SetFocus(Hero);
	PTargetGauge->SetFocus(Hero->GetTarget());
	PTargetGauge->SetObserver(Hero);

	// Inventaires
	PInventory->SetFocus(Hero);
	PTargetInventory->SetFocus(Hero->GetHumanizedTarget());
}

void InitPickup(Pickup *pPickup)
{
	int WeaponMinDamage, WeaponMaxDamage, LightArmorRating, HeavyArmorRating;
	Weapon *pWeapon = dynamic_cast<Weapon*>(pPickup);
	Armor *pArmor = dynamic_cast<Armor*>(pPickup);

	switch (pPickup->GetClass())
	{
	case WEAK:
		WeaponMinDamage = Clock::random(2, 3);
		WeaponMaxDamage = Clock::random(3, 4);
		LightArmorRating = 1;
		HeavyArmorRating = 2;
		break;
	case MEDIUM:
		WeaponMinDamage = Clock::random(3, 4);
		WeaponMaxDamage = Clock::random(5, 6);
		LightArmorRating = Clock::random(2, 3);
		HeavyArmorRating = Clock::random(3, 5);
		break;
	case STRONG:
		WeaponMinDamage = Clock::random(5, 7);
		WeaponMaxDamage = Clock::random(8, 10);
		LightArmorRating = Clock::random(4, 6);
		HeavyArmorRating = Clock::random(6, 8);
		break;
	case PNJ1:
		WeaponMinDamage = 6;
		WeaponMaxDamage = 10;
		LightArmorRating = 6;
		HeavyArmorRating = 8;
		break;
	case PNJ2:
		WeaponMinDamage = 2;
		WeaponMaxDamage = 4;
		LightArmorRating = 2;
		HeavyArmorRating = 3;
		break;
	}

	if (pWeapon)
	{
		if (!pWeapon->GetMinDamage())
			pWeapon->SetMinDamage(WeaponMinDamage);
		if (!pWeapon->GetMaxDamage())
			pWeapon->SetMaxDamage(WeaponMaxDamage);
	}

	if (pArmor)
		switch (pArmor->GetArmorSkill())
		{
		case LIGHT_ARMOR:
			if (!pArmor->GetRating())
				pArmor->SetRating(LightArmorRating);
			break;
		case HEAVY_ARMOR:
			if (!pArmor->GetRating())
				pArmor->SetRating(HeavyArmorRating);
			break;
		}
}

HWND InitWindow(HINSTANCE hInst, int nCmdShow)
{
	HWND hWnd;

	WNDCLASS wc; //Structure contenant les informations de la fenêtre
	
	//----------Enregistrement d'une classe window----------

	//Assignation de valeure pour chaques champs de la structure wc
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor((HINSTANCE) NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "MainWndClass";

	//Enregistrement
	if (!RegisterClass(&wc))
		ReportDxError(S_FALSE, WIN32_, ERR_REGISTERCLASS);
	
	//----------Création de la fenêtre----------

	hWnd = CreateWindow(wc.lpszClassName, "Random Life", WS_OVERLAPPEDWINDOW, 0, 0, 1024, 768, (HWND) NULL, (HMENU) NULL, hInst, (LPVOID) NULL);

	if (!hWnd)
		ReportDxError(S_FALSE, WIN32_, ERR_CREATEWIN);

	ShowWindow(hWnd, nCmdShow); //Afficher la fenêtre
	UpdateWindow(hWnd);

	return hWnd;
}

void InitZone(Zone *pZone, bool ResetPos)
{
	int i, x, y, MaleficDogCount, LinkAcolytCount, TestCreatureCount;

	MaleficDogCount = LinkAcolytCount = TestCreatureCount = 0;

	for (i = 0; i < CharPack.GetElementCount(); i++)
		if (TileActor::getTileActorElement(i, &CharPack))
		{
			TileActor::getTileActorElement(i, &CharPack)->SetTileNodeID(-1);
			if (Mob::getMobElement(i, &CharPack))
			{
				Mob::getMobElement(i, &CharPack)->remove();
				Mob::getMobElement(i, &CharPack)->GetDests()->RemoveAllElement();
				*Mob::getMobElement(i, &CharPack)->GetCompass() = PathFinder(Map);
				if (Humanized::getHumanizedElement(i, &CharPack))
					Humanized::getHumanizedElement(i, &CharPack)->EmptyBag();
				Character::getCharacterElement(i, &CharPack)->Reborn(true, NULL);
			}
		}

	for (i = 0; i < PickupPack.GetElementCount(); i++)
		if (TileActor::getTileActorElement(i, &PickupPack))
		{
			TileActor::getTileActorElement(i, &PickupPack)->SetTileNodeID(-1);
			TileActor::getTileActorElement(i, &PickupPack)->remove();
		}

	if (pZone->GroundFileName)
	{
		Map->GetMapFile()->SetFileName((Str)"zones/" + (Str)pZone->GroundFileName);
		Map->LoadMap(GROUND);
	}

	if (pZone->DecoFileName)
	{
		Map->GetMapFile()->SetFileName((Str)"zones/" + (Str)pZone->DecoFileName);
		Map->LoadMap(DECORATION);
	}

	if (pZone->ItemsFileName)
	{
		Map->GetMapFile()->SetFileName((Str)"zones/" + (Str)pZone->ItemsFileName);
		Map->LoadMap(ITEMS);
	}

	if (pZone->BridgesFileName)
	{
		Map->GetMapFile()->SetFileName((Str)"zones/" + (Str)pZone->BridgesFileName);
		Map->LoadMap(BRIDGES);
	}

	if (pZone->PathNodesLayer)
		Map->InitMap(80, 80, 11, PATHNODES);

	for (x = 0; x < Map->GetMapData()->TileCount.x; x++)
		for (y = 0; y < Map->GetMapData()->TileCount.y; y++)
		{
			Map->GetMapData()->obstacle[y][x] = 0;
			if ((Map->GetMapData()->TileID[y][x][GROUND] == WATER) | ((Map->GetMapData()->TileID[y][x][DECORATION] >= 8) && (Map->GetMapData()->TileID[y][x][DECORATION] <= 10)) | ((Map->GetMapData()->TileID[y][x][DECORATION] >= 21) && (Map->GetMapData()->TileID[y][x][DECORATION] <= 27)) | (Map->GetMapData()->TileID[y][x][DECORATION] == 45) | (Map->GetMapData()->TileID[y][x][DECORATION] == 46) | (Map->GetMapData()->TileID[y][x][DECORATION] >= 49))
				Map->GetMapData()->obstacle[y][x] = 1;
			if (((Map->GetMapData()->TileID[y][x][DECORATION] >= 1) && (Map->GetMapData()->TileID[y][x][DECORATION] <= 7)) | (Map->GetMapData()->TileID[y][x][DECORATION] == 19) | (Map->GetMapData()->TileID[y][x][DECORATION] == 20) | (Map->GetMapData()->TileID[y][x][DECORATION] == 28))
				Map->GetMapData()->obstacle[y][x] = 2;
			if ((pZone->BridgesLayer == true) && (Map->GetMapData()->TileID[y][x][BRIDGES] != 0))
				Map->GetMapData()->obstacle[y][x] = 0;
			//Place si il y'en a les objets et personnages à position prédéfinie sur la map.
			if (pZone->ItemsLayer)
				switch (Map->GetMapData()->TileID[y][x][ITEMS])
				{
				case 1:
					if (ResetPos)
					{
						Hero->GetTileCoord()->x = x;
						Hero->GetTileCoord()->y = y;
					}
					break;
				case 2:
					if (!Trader->GetZoneStatus(pZone->CoordX, pZone->CoordY)->dead)
					{
						Trader->SetMapPositionByCoord(x, y, false);
						AddZoneChar(Trader, pZone);
					}
					break;
				case 3:
					if (!GreatMago->GetZoneStatus(pZone->CoordX, pZone->CoordY)->dead)
					{
						GreatMago->SetMapPositionByCoord(x, y, false);
						AddZoneChar(GreatMago, pZone);
					}
					break;
				case 4:
					if (!Captain->GetZoneStatus(pZone->CoordX, pZone->CoordY)->dead)
					{
						Captain->SetMapPositionByCoord(x, y, false);
						AddZoneChar(Captain, pZone);
					}
					break;
				case 5:
					if (!Wise->GetZoneStatus(pZone->CoordX, pZone->CoordY)->dead)
					{
						Wise->SetMapPositionByCoord(x, y, false);
						AddZoneChar(Wise, pZone);
					}
					break;
				case 6:
					if (!Guard->GetZoneStatus(pZone->CoordX, pZone->CoordY)->dead)
					{
						Guard->SetMapPositionByCoord(x, y, false);
						AddZoneChar(Guard, pZone);
					}
					break;
				case 7:
					if (!Leader->GetZoneStatus(pZone->CoordX, pZone->CoordY)->dead)
					{
						Leader->SetMapPositionByCoord(x, y, false);
						AddZoneChar(Leader, pZone);
					}
					break;
				case 8:
					if (!LinkAcolyt[LinkAcolytCount]->GetZoneStatus(pZone->CoordX, pZone->CoordY)->dead)
					{
						LinkAcolyt[LinkAcolytCount]->SetMapPositionByCoord(x, y, false);
						AddZoneChar(LinkAcolyt[LinkAcolytCount], pZone);
					}
					else
						LinkAcolytPack.RemoveElement(LinkAcolyt[LinkAcolytCount]);
					LinkAcolytCount++;
					break;
				case 9:
					if (!MaleficDog[MaleficDogCount]->GetZoneStatus(pZone->CoordX, pZone->CoordY)->dead)
					{
						MaleficDog[MaleficDogCount]->SetMapPositionByCoord(x, y, false);
						AddZoneChar(MaleficDog[MaleficDogCount], pZone);
					}
					MaleficDogCount++;
					break;
				case 10:
					if (!Balrogue->GetZoneStatus(pZone->CoordX, pZone->CoordY)->dead)
					{
						Balrogue->SetMapPositionByCoord(x, y, false);
						AddZoneChar(Balrogue, pZone);
					}
					break;
				case 11:
					if (!Wizard->GetZoneStatus(pZone->CoordX, pZone->CoordY)->dead)
					{
						Wizard->SetMapPositionByCoord(x, y, false);
						AddZoneChar(Wizard, pZone);
					}
					break;
				case 12:
					PinkSnake[TestCreatureCount]->SetMapPositionByCoord(x, y, false);
					AddZoneChar(PinkSnake[TestCreatureCount], pZone);
					TestCreatureCount++;
					break;
				}
		}
}

bool LoadConfig()
{
	int i, j;
	bool b;
	Sound *pSound;

	if (!Config.Open(true, false, false))
		return false;

	b = Config.PlaySounds();

	for (i = 0; i < SoundPack.GetElementCount(); i++)
		if (b)
			Sound::getSoundElement(i, &SoundPack)->setMuted(0);
		else
		{
			Sound::getSoundElement(i, &SoundPack)->setMuted(2);
			Sound::getSoundElement(i, &SoundPack)->mute();
		}

	b = Config.PlayMusic();

	for (i = 0; i < TOTAL_MUSIC_CATEGORY; i++)
		for (j = 0; j < MusicPack[i].GetElementCount(); j++)
			if (b)
				Sound::getSoundElement(j, &MusicPack[i])->setMuted(0);
			else
			{
				Sound::getSoundElement(j, &MusicPack[i])->setMuted(2);
				Sound::getSoundElement(j, &MusicPack[i])->mute();
			}

	if (MusicRandomPack[CurrentMusicCategory].ElementExist(CurrentMusic))
	{
		pSound = Sound::getSoundElement(CurrentMusic, &MusicRandomPack[CurrentMusicCategory]);
		if (pSound && !pSound->getMuted() && !pSound->isPlaying())
			PlayMusic();
	}
	else
		if (!MusicRandomPack[CurrentMusicCategory].GetElementCount())
			PlayMusic();

	Config.ReadGraphic(Screen->GetCurrentMode());
	Config.ReadKeys();
	Config.Close();

	return true;
}

DWORD WINAPI LoadData(LPVOID lpParam)
{
	int i, j, x, y, total[3], NewRezPointCount, *CommentDay;
	EVENT_TYPE result;
	Str SubIndex[3];
	Pickup *pPickup;
	Ingredient *pIngredient;
	Potion *pPotion;
	Weapon *pWeapon;
	Armor *pArmor;
	ItemMap *ItemMapData;
	MagicSpell *pMagicSpell;
	List InPocket, OutPocket, *SpellGift;
	DataFileList LoadFile = DataFileList("savegame.dat");

	PGreatMago->hide();
	PDialogue->hide();
	PTargetInventory->hide();
	LandMap->hide();

	for (i = 0; i < PickupPack.GetElementCount(); i++) OutPocket.AddElement(PickupPack.GetElement(i));
	PBook->Start();

	PData->GetDataBar()->SetAmount(0);
	PData->GetDataBar()->GenerateBar();
	PData->setName("PREPARATION DES DONNEES");
	PData->show();
	PData->Refresh();

	if (!LoadFile.Open(true, false, false)) ExitThread(-2);

	PData->setName("CHARGEMENT DES DONNEES");
	PData->GetDataBar()->SetMaxAmount((float)LoadFile.GetIndexTotal());
	PData->Refresh();

	TicTac->setCounter(LoadFile.Read("TicTac_counter", false, "", false, GenerateDataProgress));
	PWorldTime->SetDay(LoadFile.Read("RanOutTime_day", false, "", false, GenerateDataProgress));
	PWorldTime->SetMeditateProgress(LoadFile.Read("RanOutTime_meditate", false, "", false, GenerateDataProgress));
	PWorldTime->SetChao(LoadFile.Read("RanOutTime_chao", false, "", false, GenerateDataProgress));
	PWorldTime->GetTodayBar()->SetAmount((float)LoadFile.Read("Today_amount", false, "", false, GenerateDataProgress));
	PWorldTime->GetChaoBar()->SetAmount((float)LoadFile.Read("Chao_amount", false, "", false, GenerateDataProgress));
	Hero->SetNameID(LoadFile.Read("Hero_NameID", false, "", false, GenerateDataProgress));
	Hero->SetSkillPoints(LoadFile.Read("Hero_SkillPoints", false, "", false, GenerateDataProgress));
	Hero->SetMagicPoints(LoadFile.Read("Hero_MagicPoints", false, "", false, GenerateDataProgress));

	total[0] = LoadFile.Read("TOTAL_CHAR_STAT", false, "", false, GenerateDataProgress);
	for (i = 0; i < total[0]; i++)
	{
		SubIndex[0].Cpy(i+1);
		Hero->GetStatPack()->getCharStatElement(i)->SetScore(LoadFile.Read(SubIndex[0], true, "Hero_Stat_score", false, GenerateDataProgress));
		Hero->GetStatPack()->getCharStatElement(i)->SetTempScore(LoadFile.Read(SubIndex[0], true, "Hero_TempStat_score", false, GenerateDataProgress));
	}

	total[0] = LoadFile.Read("TOTAL_CHAR_MAGIC_SCHOOL", false, "", false, GenerateDataProgress);
	for (i = 0; i < total[0]; i++)
	{
		SubIndex[0].Cpy(i+1);
		Hero->GetMagicSchoolPack()->getCharStatElement(i)->SetScore(LoadFile.Read(SubIndex[0], true, "Hero_MagicSchool_score", false, GenerateDataProgress));
		Hero->GetMagicSchoolPack()->getCharStatElement(i)->SetTempScore(LoadFile.Read(SubIndex[0], true, "Hero_TempMagicSchool_score", false, GenerateDataProgress));
	}

	total[0] = LoadFile.Read("TOTAL_CHAR_SKILL", false, "", false, GenerateDataProgress);
	for (i = 0; i < total[0]; i++)
	{
		SubIndex[0].Cpy(i+1);
		Hero->GetSkillPack()->getCharSkillElement(i)->SetLevel(LoadFile.Read(SubIndex[0], true, "Hero_Skill_level", false, GenerateDataProgress));
		Hero->GetSkillPack()->getCharSkillElement(i)->GetBPData()->gain = LoadFile.Read(SubIndex[0], true, "Hero_Skill_BPData_gain", false, GenerateDataProgress);
		Hero->GetSkillPack()->getCharSkillElement(i)->GetBPData()->total = LoadFile.Read(SubIndex[0], true, "Hero_Skill_BPData_total", false, GenerateDataProgress);
	}

	total[0] = LoadFile.Read("TOTAL_CHAR_COMBAT_SKILL", false, "", false, GenerateDataProgress);
	for (i = 0; i < total[0]; i++)
	{
		SubIndex[0].Cpy(i+1);
		Hero->GetCombatSkillPack()->getCharSkillElement(i)->SetLevel(LoadFile.Read(SubIndex[0], true, "Hero_CombatSkill_level", false, GenerateDataProgress));
		Hero->GetCombatSkillPack()->getCharSkillElement(i)->GetBPData()->gain = LoadFile.Read(SubIndex[0], true, "Hero_CombatSkill_BPData_gain", false, GenerateDataProgress);
		Hero->GetCombatSkillPack()->getCharSkillElement(i)->GetBPData()->total = LoadFile.Read(SubIndex[0], true, "Hero_CombatSkill_BPData_total", false, GenerateDataProgress);
	}

	Hero->GetMagicSkill()->SetLevel(LoadFile.Read("Hero_MagicSkill_level", false, "", false, GenerateDataProgress));
	Hero->GetMagicSkill()->GetBPData()->gain = LoadFile.Read("Hero_MagicSkill_BPData_gain", false, "", false, GenerateDataProgress);
	Hero->GetMagicSkill()->GetBPData()->total = LoadFile.Read("Hero_MagicSkill_BPData_total", false, "", false, GenerateDataProgress);
	Hero->GetHitPoints()->SetAmount((float)LoadFile.Read("Hero_HitPoints_amount", false, "", false, GenerateDataProgress));
	Hero->GetManaPoints()->SetAmount((float)LoadFile.Read("Hero_ManaPoints_amount", false, "", false, GenerateDataProgress));

	total[0] = LoadFile.Read("TOTAL_PICKUP_GROUP", false, "", false, GenerateDataProgress);
	for (i = 0; i < total[0]; i++)
	{
		SubIndex[0].Cpy(i+1);
		total[1] = LoadFile.Read(SubIndex[0], true, "Hero_Pocket_ElementCount", false, GenerateDataProgress);
		for (j = 0; j < total[1]; j++)
		{
			SubIndex[1] = SubIndex[0];
			SubIndex[1].Cat(j+1);
			pPickup = new Pickup(LoadFile.Read(SubIndex[1], true, "Hero_Pocket_Pickup_price", false, GenerateDataProgress), NULL, false, (PICKUP_UNIQUE_ID)LoadFile.Read(SubIndex[1], true, "Hero_Pocket_Pickup_UniqueID", false, GenerateDataProgress), (PICKUP_TYPE)LoadFile.Read(SubIndex[1], true, "Hero_Pocket_Pickup_type", false, GenerateDataProgress), OTHERS, (CHAR_CLASS)LoadFile.Read(SubIndex[1], true, "Hero_Pocket_Pickup_Class", false, GenerateDataProgress), PickupMalus(), Enchant()); 
			if (LoadFile.Read(SubIndex[1], true, "Hero_Pocket_Pickup_Status_picked", false, GenerateDataProgress))
				pPickup->GetStatus()->picked = true;
			else
				pPickup->GetStatus()->picked = false;
			if (LoadFile.Read(SubIndex[1], true, "Hero_Pocket_Pickup_Status_equiped", false, GenerateDataProgress))
				pPickup->GetStatus()->equiped = true;
			else
				pPickup->GetStatus()->equiped = false;
			pIngredient = new Ingredient((PICKUP_INGREDIENT_TYPE)LoadFile.Read(SubIndex[1], true, "Hero_Pocket_Ingredient_IngredientType", false, GenerateDataProgress), pPickup->GetPrice(), NULL, pPickup->GetType(), pPickup->GetClass(), NULL, NULL, Str());
			*pIngredient->GetStatus() = *pPickup->GetStatus();
			pPotion = new Potion((float)LoadFile.Read(SubIndex[1], true, "Hero_Pocket_Potion_power", false, GenerateDataProgress), (PICKUP_POTION_EFFECT)LoadFile.Read(SubIndex[1], true, "Hero_Pocket_Potion_effect", false, GenerateDataProgress), pPickup->GetPrice(), NULL, pPickup->GetUniqueID(), pPickup->GetClass(), NULL, NULL, Str());
			*pPotion->GetStatus() = *pPickup->GetStatus();
			pWeapon = new Weapon(LoadFile.Read(SubIndex[1], true, "Hero_Pocket_Weapon_MinDamage", false, GenerateDataProgress), LoadFile.Read(SubIndex[1], true, "Hero_Pocket_Weapon_MaxDamage", false, GenerateDataProgress), (CHAR_COMBAT_SKILL)LoadFile.Read(SubIndex[1], true, "Hero_Pocket_Weapon_CombatSkill", false, GenerateDataProgress), pPickup->GetPrice(), NULL, false, pPickup->GetUniqueID(), pPickup->GetClass(), PickupMalus(), Enchant(), NULL, NULL, Str());
			*pWeapon->GetStatus() = *pPickup->GetStatus();
			pArmor = new Armor(LoadFile.Read(SubIndex[1], true, "Hero_Pocket_Armor_rating", false, GenerateDataProgress), NULL, (CHAR_SKILL)LoadFile.Read(SubIndex[1], true, "Hero_Pocket_Armor_ArmorSkill", false, GenerateDataProgress), pPickup->GetPrice(), NULL, false, pPickup->GetUniqueID(), pPickup->GetClass(), PickupMalus(), Enchant(), NULL, NULL, Str());
			*pArmor->GetStatus() = *pPickup->GetStatus();
			if (pIngredient->GetIngredientType() != -1)
				InPocket.AddElement(pIngredient);
			else
				if (pPotion->GetEffect() != -1)
					InPocket.AddElement(pPotion);
				else
					if (pWeapon->GetCombatSkill() != -1)
						InPocket.AddElement(pWeapon);
					else
						if (pArmor->GetArmorSkill() != -1)
							InPocket.AddElement(pArmor);
						else
							InPocket.AddElement(pPickup);
		}
	}

	SpellGift = new List[Hero->GetMagicSchoolPack()->GetElementCount()];
	for (i = 0; i < Hero->GetMagicSchoolPack()->GetElementCount(); i++)
	{
		SubIndex[0].Cpy(i+1);
		total[0] = LoadFile.Read(SubIndex[0], true, "TOTAL_CHAR_MAGIC_SPELL", false, GenerateDataProgress);
		for (j = 0; j < total[0]; j++)
		{
			SubIndex[1] = SubIndex[0];
			SubIndex[1].Cat(j+1);
			pMagicSpell = new MagicSpell();
			pMagicSpell->SetSchool(LoadFile.Read(SubIndex[1], true, "Hero_MagicSpell_school", false, GenerateDataProgress));
			pMagicSpell->SetLevel(LoadFile.Read(SubIndex[1], true, "Hero_MagicSpell_level", false, GenerateDataProgress));
			pMagicSpell->SetEffect((CHAR_MAGIC_SPELL_EFFECT)LoadFile.Read(SubIndex[1], true, "Hero_MagicSpell_effect", false, GenerateDataProgress));
			SpellGift[i].AddElement(pMagicSpell);
		}
	}

	Hero->getPosition()->x = LoadFile.Read("Hero_Position_x", false, "", false, GenerateDataProgress);
	Hero->getPosition()->y = LoadFile.Read("Hero_Position_y", false, "", false, GenerateDataProgress);
	Map->GetScreenData()->Camera.getPosition()->x = LoadFile.Read("Map_ScreenData_Camera_Position_x", false, "", false, GenerateDataProgress);
	Map->GetScreenData()->Camera.getPosition()->y = LoadFile.Read("Map_ScreenData_Camera_Position_y", false, "", false, GenerateDataProgress);
	ZoneCoord.x = LoadFile.Read("ZoneCoord_x", false, "", false, GenerateDataProgress);
	ZoneCoord.y = LoadFile.Read("ZoneCoord_y", false, "", false, GenerateDataProgress);

	PBook->SetAcolytKill(LoadFile.Read("IBook_AcolytKill", false, "", false, GenerateDataProgress));
	PBook->SetCurseStatus(LoadFile.Read("IBook_CurseStatus", false, "", false, GenerateDataProgress));
	total[0] = LoadFile.Read("TOTAL_CHAR_PNJ_QUEST", false, "", false, GenerateDataProgress);
	for (i = 0; i < total[0]; i++)
	{
		SubIndex[0].Cpy(i+1);
		PDialogue->SetStep(i, LoadFile.Read(SubIndex[0], true, "IDialogue_step", false, GenerateDataProgress));
		PBook->SetStatus(i, LoadFile.Read(SubIndex[0], true, "IBook_status", false, GenerateDataProgress));
	}

	total[0] = LoadFile.Read("TOTAL_ZONE_X", false, "", false, GenerateDataProgress);
	total[1] = LoadFile.Read("TOTAL_ZONE_Y", false, "", false, GenerateDataProgress);
	total[2] = LoadFile.Read("CharPack_ElementCount", false, "", false, GenerateDataProgress);
	for (x = 0; x < total[0]; x++)
		for (y = 0; y < total[1]; y++)
		{
			SubIndex[0].Cpy(x+1);
			SubIndex[0].Cat(y+1);
			RezPoint[y][x] = 0;
			ZoneData[y][x].cursed = LoadFile.Read(SubIndex[0], true, "ZoneData_cursed", false, GenerateDataProgress);
			for (i = 0; i < total[2]; i++)
				if (Mob::getMobElement(i, &CharPack))
				{
					SubIndex[1].Cpy(i+1);
					SubIndex[2] = SubIndex[0];
					SubIndex[2].Cat(1);
					Mob::getMobElement(i, &CharPack)->GetZoneStatus(x, y)->dead = LoadFile.Read(SubIndex[1], true, SubIndex[2], true, GenerateDataProgress);
					SubIndex[2] = SubIndex[0];
					SubIndex[2].Cat(2);
					Mob::getMobElement(i, &CharPack)->GetZoneStatus(x, y)->spyed = LoadFile.Read(SubIndex[1], true, SubIndex[2], true, GenerateDataProgress);
				}
		}

	total[0] = LoadFile.Read("FlowerPack_ElementCount", false, "", false, GenerateDataProgress);
	for (i = 0; i < total[0]; i++)
	{
		SubIndex[0].Cpy(i+1);
		Ingredient::getIngredientElement(i, &FlowerPack)->SetChemicalID(LoadFile.Read(SubIndex[0], true, "FlowerPack_Ingredient_ChemicalID", false, GenerateDataProgress));
		Ingredient::getIngredientElement(i, &FlowerPack)->GenerateEffectFactor();
		Ingredient::getIngredientElement(i, &FlowerPack)->GenerateLivingTile();
	}

	total[0] = LoadFile.Read("FruitPack_ElementCount", false, "", false, GenerateDataProgress);
	for (i = 0; i < total[0]; i++)
	{
		SubIndex[0].Cpy(i+1);
		Ingredient::getIngredientElement(i, &FruitPack)->SetChemicalID(LoadFile.Read(SubIndex[0], true, "FruitPack_Ingredient_ChemicalID", false, GenerateDataProgress));
		Ingredient::getIngredientElement(i, &FruitPack)->GenerateEffectFactor();
	}

	total[0] = LoadFile.Read("DrinkPack_ElementCount", false, "", false, GenerateDataProgress);
	for (i = 0; i < total[0]; i++)
	{
		SubIndex[0].Cpy(i+1);
		Ingredient::getIngredientElement(i, &DrinkPack)->SetChemicalID(LoadFile.Read(SubIndex[0], true, "DrinkPack_Ingredient_ChemicalID", false, GenerateDataProgress));
		Ingredient::getIngredientElement(i, &DrinkPack)->GenerateEffectFactor();
	}

	NewRezPointCount = LoadFile.Read("RezPointCount", false, "", false, GenerateDataProgress);
	if (NewRezPointCount < 0) NewRezPointCount = 0;
	ItemMapData = new ItemMap[NewRezPointCount];
	for (i = 0; i < NewRezPointCount; i++)
	{
		SubIndex[0].Cpy(i+1);
		ItemMapData[i].getDimension()->x = LoadFile.Read(SubIndex[0], true, "LandMap_ItemMap_Dimension_x", false, GenerateDataProgress);
		ItemMapData[i].getDimension()->y = LoadFile.Read(SubIndex[0], true, "LandMap_ItemMap_Dimension_y", false, GenerateDataProgress);
		ItemMapData[i].SetType(LoadFile.Read(SubIndex[0], true, "LandMap_ItemMap_type", false, GenerateDataProgress));
		ItemMapData[i].GetWorldPosition()->x = LoadFile.Read(SubIndex[0], true, "LandMap_ItemMap_WorldPosition_x", false, GenerateDataProgress);
		ItemMapData[i].GetWorldPosition()->y = LoadFile.Read(SubIndex[0], true, "LandMap_ItemMap_WorldPosition_y", false, GenerateDataProgress);
		ItemMapData[i].GetCoord()->x = LoadFile.Read(SubIndex[0], true, "LandMap_ItemMap_Coord_x", false, GenerateDataProgress);
		ItemMapData[i].GetCoord()->y = LoadFile.Read(SubIndex[0], true, "LandMap_ItemMap_Coord_y", false, GenerateDataProgress);
		ItemMapData[i].GetZoneID()->x = LoadFile.Read(SubIndex[0], true, "LandMap_ItemMap_ZoneID_x", false, GenerateDataProgress);
		ItemMapData[i].GetZoneID()->y = LoadFile.Read(SubIndex[0], true, "LandMap_ItemMap_ZoneID_y", false, GenerateDataProgress);
	}

	total[0] = LoadFile.Read("TOTAL_BOOK_COMMENT", false, "", false, GenerateDataProgress);
	CommentDay = new int[total[0]];
	for (i = 0; i < total[0]; i++)
	{
		SubIndex[0].Cpy(i+1);
		PBook->SetComment(i, LoadFile.Read(SubIndex[0], true, "IBook_Comment_noted", false, GenerateDataProgress));
		CommentDay[i] = LoadFile.Read(SubIndex[0], true, "IBook_QuestComment_day", false, GenerateDataProgress);
	}

	LoadFile.Close();

	PData->setName("RECUPERATION DES DONNEES");
	PData->Refresh();

	if (LoadFile.ErrorOccured())
	{
		EventsReporter.AddMessage(SAVE_DATA, 3);
		ExitThread(-2);
	}

	Hero->GenerateName();
	Hero->GenerateTitle();
	Hero->Reborn(false);
	Hero->GenerateStat();
	Hero->GenerateStat2();
	Hero->UpdateCombat(CombatData);
	Hero->EmptyBag();
	Hero->GetCraftPack()->Initialize();

	// On récupère l'inventaire
	while (InPocket.GetElementCount())
	{
		bool equip = Pickup::getPickupElement(0, &InPocket)->GetStatus()->equiped;
		i = OutPocket.SearchElement(InPocket.GetElement(0), true);
		result = NONE_EVENT_TYPE;
		if (i != -1)
		{
			Pickup::getPickupElement(i, &OutPocket)->SetPrice(Pickup::getPickupElement(0, &InPocket)->GetPrice());
			if (Weapon::getWeaponElement(i, &OutPocket))
			{
				Weapon::getWeaponElement(i, &OutPocket)->SetMinDamage(Weapon::getWeaponElement(0, &InPocket)->GetMinDamage());
				Weapon::getWeaponElement(i, &OutPocket)->SetMaxDamage(Weapon::getWeaponElement(0, &InPocket)->GetMaxDamage());
			}
			if (Armor::getArmorElement(i, &OutPocket))
				Armor::getArmorElement(i, &OutPocket)->SetRating(Armor::getArmorElement(0, &InPocket)->GetRating());
			if (Pickup::getPickupElement(i, &OutPocket)->GetOwnerID() != -1)
			{
				//Si l'objet appartient déjà à un autre personnage que le joueur
				if (Pickup::getPickupElement(i, &OutPocket)->GetOwnerID() != Hero->getId())
				{
					j = 0;
					while (CharPack.GetElement(j)->getId() != Pickup::getPickupElement(i, &OutPocket)->GetOwnerID())
						j++;
					if (Humanized::getHumanizedElement(j, &CharPack))
						result = Humanized::getHumanizedElement(j, &CharPack)->Give(Pickup::getPickupElement(i, &OutPocket), Hero);
				}
				else
					InPocket.RemoveElement(0, 1, true);
			}
			else
				result = Hero->AddToEquipment(Pickup::getPickupElement(i, &OutPocket));
			if (equip)
				if (Hero->Equip(Pickup::getPickupElement(i, &OutPocket)) == PICKUP_SUMMONED)
					PInventory->Refresh();
			if (result == ACTION_SUCCESS)
				OutPocket.RemoveElement(i, 1);
			else
				i = i;
			InPocket.RemoveElement(0, 1, true);
		}
		else
		{
			// Si le Pickup n'est pas trouvé c'est qu'il a été créé en cours de jeu, on le reconstruit donc
			switch (Pickup::getPickupElement(0, &InPocket)->GetType())
			{
			case MONEY:
				pPickup = new Pickup(Pickup::getPickupElement(0, &InPocket)->GetPrice(), NONE_PICKUP_UNIQUE_ID, NONE_CHAR_CLASS, 17, 17, "Sac d'or");
				pPickup->cloneAni(MoneyDefaultBag);
				Hero->AddToEquipment(pPickup);
				PickupPack.AddElement(pPickup);
				break;
			case POTION:
				pPotion = new Potion(Potion::getPotionElement(0, &InPocket)->GetPower(), Potion::getPotionElement(0, &InPocket)->GetEffect(), 20, 1, NONE_PICKUP_UNIQUE_ID, NONE_CHAR_CLASS, 6, 15, "Potion");
				pPotion->initAni();
				Hero->AddToEquipment(pPotion);
				Hero->GetCraftPack()->AddElement(pPotion);
				PickupPack.AddElement(pPotion);
				break;
			}
			InPocket.RemoveElement(0, 1, true);
		}
	}

	// On réattribue les sorts appris
	for (i = 0; i < Hero->GetMagicSchoolPack()->GetElementCount(); i++)
	{
		Hero->GetMagicSpellPack(i)->RemoveAllElement(0, true);
		for (j = 0; j < SpellGift[i].GetElementCount(); j++)
		{
			int k = MagicSpellPack.SearchElement(MagicSpell::getMagicSpellElement(j, &SpellGift[i]), true);
			if (k != -1) Hero->GetMagicSpellPack(i)->AddElement(new MagicSpell(*MagicSpell::getMagicSpellElement(k, &MagicSpellPack)));
		}
		SpellGift[i].RemoveAllElement(0, true);
	}
	Hero->UpdateMagicSpell();

	// On remet les points de résurections sur la carte
	RezPointCount = 0;
	LandMap->RemoveAllElement(1);
	for (i = 0; i < NewRezPointCount; i++)
		if (ItemMapData[i].GetType() == 1)
			AddRezPoint(&ItemMapData[i]);
	delete [] ItemMapData;

	// On remet les commentaires du journal
	PBook->RemoveAllElement(0, true);
	for (i = 0; i < TOTAL_BOOK_COMMENT; i++)
		if (PBook->GetComment(i) > 0)
			PBook->AddComment(CommentDay[i], false, (BOOK_COMMENT)i);
	delete [] CommentDay;

	PWorldTime->GetChaoBar()->GenerateBar();
	ChangeZone(Hero, ZoneCoord.x, ZoneCoord.y, Hero->getPosition()->x, Hero->getPosition()->y, Map->GetScreenData()->Camera.getPosition()->x, Map->GetScreenData()->Camera.getPosition()->y, true, false);

	EventsReporter.AddMessage(SAVE_DATA, 1);

	EraseFile = false;

	PData->hide();

	for (i = 0; i < PanelPack.GetElementCount(); i++)
		Panel::getPanelElement(i, &PanelPack)->WaitForRefresh();

	ExitThread(2);
}

void NewGame()
{
	int i, x, y;

	PGreatMago->hide();
	PDialogue->hide();
	PTargetInventory->hide();

	PBook->Start();

	Hero->SetNameID(0);
	Hero->GenerateName();
	Hero->Reborn(true, NULL);
	Hero->EmptyBag();
	Hero->GetCraftPack()->Initialize();

#ifdef _DEBUG
	InitCharacter(Hero, 5, 5, 0, 0, 0, 0, 0, 0);
#else
	InitCharacter(Hero, 2, 1, 0, 0, 0, 0, 0, 0);
#endif

	Hero->UpdateCombat(CombatData);
	Hero->GenerateTitle();

	for (i = 0; i < Hero->GetMagicSchoolPack()->GetElementCount(); i++)
		Hero->GetMagicSpellPack(i)->Initialize();

	GiveMagicSpellToChar(Hero);
	Hero->UpdateMagicSpell();
	MoneyDefaultBag->SetPrice(10);
	Hero->AddToEquipment(MoneyDefaultBag);

#if _DEBUG
	// Pour tester la compétence chimie
	/*for (i = 0; i < FlowerPack.GetElementCount(); i++) Hero->AddToEquipment(Pickup::getPickupElement(i, &FlowerPack));
	for (i = 0; i < FruitPack.GetElementCount(); i++) Hero->AddToEquipment(Pickup::getPickupElement(i, &FruitPack));
	for (i = 0; i < DrinkPack.GetElementCount(); i++) Hero->AddToEquipment(Pickup::getPickupElement(i, &DrinkPack));*/
#endif

	Hero->SetSkillPoints(40);
	Hero->SetMagicPoints(20);

	for (i = 0; i < Hero->GetStatPack()->GetElementCount(); i++)
		Hero->GetStatPack()->getCharStatElement(i)->SetTempScore(Hero->GetStatPack()->getCharStatElement(i)->GetScore());

	for (i = 0; i < Hero->GetMagicSchoolPack()->GetElementCount(); i++)
		Hero->GetMagicSchoolPack()->getCharStatElement(i)->SetTempScore(Hero->GetMagicSchoolPack()->getCharStatElement(i)->GetScore());

	for (i = 0; i < TOTAL_CHAR_PNJ_QUEST; i++)
		PDialogue->SetStep(i, 0);

	TicTac->setCounter(0);
	PWorldTime->Reset();

	for (x = 0; x < TOTAL_ZONE_X; x++)
		for (y = 0; y < TOTAL_ZONE_Y; y++)
		{
			ZoneData[y][x].cursed = 1;
			RezPoint[y][x] = 0;
			for (i = 0; i < CharPack.GetElementCount(); i++)
				if (Mob::getMobElement(i, &CharPack))
				{
					Mob::getMobElement(i, &CharPack)->GetZoneStatus(x, y)->dead = 0;
					Mob::getMobElement(i, &CharPack)->GetZoneStatus(x, y)->spyed = 0;
				}
		}

	RezPointCount = 0;
	LandMap->RemoveAllElement(1);
	ChangeZone(Hero, 3, 2, 0, 0, 0, 0, true, true);

	PCharInfo->show();

	for (i = 0; i < PanelPack.GetElementCount(); i++)
		Panel::getPanelElement(i, &PanelPack)->WaitForRefresh();

	EraseFile = true;
}

void PlayMusic()
{
	int i;

	if (!MusicRandomPack[CurrentMusicCategory].GetElementCount())
		for (i = 0; i < MusicPack[CurrentMusicCategory].GetElementCount(); i++)
			MusicRandomPack[CurrentMusicCategory].AddElement(MusicPack[CurrentMusicCategory].GetElement(i));

	if (MusicRandomPack[CurrentMusicCategory].GetElementCount())
		CurrentMusic = Clock::random(0, MusicRandomPack[CurrentMusicCategory].GetElementCount()-1);

	if (MusicRandomPack[CurrentMusicCategory].ElementExist(CurrentMusic))
		Sound::getSoundElement(CurrentMusic, &MusicRandomPack[CurrentMusicCategory])->play(DMUS_SEGF_DEFAULT, false);
}

bool PlayScene(Zone *pZone)
{
	int i, j, x, y, ObstacleCount, EventID;
	EVENT_TYPE EventResult, ActionResult;
	Axe MapZone;
	CardinalPoint Obstacle[MAX_LIVING_TILE], ScreenEdge;
	Sound *pSound;

	if (PData->GetThread()) return false;

	ObstacleCount = EventID = 0;
	ScreenEdge = CardinalPoint(0, Screen->GetCurrentMode()->GetWidth(), 0, Screen->GetCurrentMode()->GetHeight());

	// On lance une nouvelle chanson de même catégorie si l'actuelle est terminée
	if (MusicRandomPack[CurrentMusicCategory].ElementExist(CurrentMusic))
	{
		pSound = Sound::getSoundElement(CurrentMusic, &MusicRandomPack[CurrentMusicCategory]);
		if (pSound->finished())
		{
			Sound::stop(DMUS_SEGF_DEFAULT);
			MusicRandomPack[CurrentMusicCategory].RemoveElement(CurrentMusic, 1);
			PlayMusic();
		}
	}

	if (pZone->GroundLayer)
		Map->RenderMap(GROUND);
	if (pZone->DecoLayer)
		Map->RenderMap(DECORATION);
	if (pZone->BridgesLayer)
		Map->RenderMap(BRIDGES);
#ifdef _DEBUG
	if (pZone->PathNodesLayer)
		Map->RenderMap(PATHNODES);
#endif

	TicTac->goCounter(1, TicTac->getCounter(), -1, false);

	// On augmente la jauge de chao en fonction du temps écoulé
	if (PBook->GetCurseStatus())
	{
		if (!PWorldTime->GetChao())
		{
			PWorldTime->GetChaoBar()->SetAmount((float)TicTac->getCounter());
			// C'est le chao! Tous les pnj sont contaminés
			if (TicTac->getCounter() >= WORLD_TIME_SEC_BEFORE_CHAO)
			{
				PWorldTime->SetChao(true);
				PWorldTime->GetChaoBar()->SetAmount(PWorldTime->GetChaoBar()->GetMaxAmount());
				UpdateCurse(pZone->CoordX, pZone->CoordY, pZone->cursed, pZone);
				Sound::getSoundElement(CHAO, &SoundPack)->play(DMUS_SEGF_SECONDARY);
				EventsReporter.AddMessage(CURRENT_ACTION, "C'est le chao!! Tout le village est contaminé, le monde est perdu... A moin que vous ayez une idée...", DAMAGE_MSG_COLOR);
			}
		}
	}
	else
		PWorldTime->GetChaoBar()->Decrease(5, 1, false);

	PWorldTime->GetTodayBar()->Increase(1, 1, true);
	PWorldTime->GetTodayBar()->GenerateBar();
	PWorldTime->GetChaoBar()->GenerateBar();

	if (TicTac->getCounter() == (PWorldTime->GetDay()+1) * WORLD_TIME_SEC_PER_DAY)
		PWorldTime->SetNewDay(true);
	
	if (PWorldTime->GetNewDay())
	{
		PWorldTime->GoNewDay();
		PWorldTime->Refresh();
	}

	//La carte
	if (Hero->GetStatus()->dead)
		if (LandMap->getVisible())
		{
			for (i = 1; i < RezPointCount + 1; i++)
				if (cursor->buttonPressed() == Cursor::LEFT_CLICK && cursor->collision(LandMap->GetItemMapElement(i)))
				{
					LandMap->hide();
					Hero->Reborn(false, &EventsReporter);
					*Map->GetScreenData()->Camera.getPosition() = Axe();
					Hero->SetMapPositionByCoord(LandMap->GetItemMapElement(i)->GetCoord()->x, LandMap->GetItemMapElement(i)->GetCoord()->y, false);
					Hero->FocusWithCamera(Screen->GetCurrentMode()->GetWidth() / 2, Screen->GetCurrentMode()->GetHeight() / 2);
					ChangeZone(Hero, LandMap->GetItemMapElement(i)->GetZoneID()->x, LandMap->GetItemMapElement(i)->GetZoneID()->y, Hero->getPosition()->x, Hero->getPosition()->y, Map->GetScreenData()->Camera.getPosition()->x, Map->GetScreenData()->Camera.getPosition()->y, false, false);
					break;
				}
		}
		else
			if (!Hero->Wait(BASIC, 3, 1))
				LandMap->show();

	MapZone.x = Map->GetScreenData()->Camera.getPosition()->x / Map->GetTileData()->getDimension()->x;
	MapZone.y = Map->GetScreenData()->Camera.getPosition()->y / Map->GetTileData()->getDimension()->y;

	//Traite tous les pickups se trouvant dans la zone visible
	for (i = 0; i < ZonePickupPack.GetElementCount(); i++)
		if (Pickup::getPickupElement(i, &ZonePickupPack)->IsInZone(MapZone))
		{
			Pickup::getPickupElement(i, &ZonePickupPack)->setRemoved(false);
			if (Hero->collision(Pickup::getPickupElement(i, &ZonePickupPack)))
			{
				if (Hero->GetStatus()->look)
				{
					PPickup->SetFocus(Pickup::getPickupElement(i, &ZonePickupPack));
					PPickup->show();
					PPickup->Refresh();
				}
			}
			else
				if ((PPickup->GetFocus() == Pickup::getPickupElement(i, &ZonePickupPack)) && (Pickup::getPickupElement(i, &ZonePickupPack)->GetStatus()->picked == false))
				{
					PPickup->hide();
					PPickup->SetFocus(NULL);
				}
		}

	//Traite tous les personnages se trouvant dans la zone visible
	for (i = 0; i < ZoneCharPack.GetElementCount(); i++)
		if (TileActor::getTileActorElement(i, &ZoneCharPack)->IsInZone(MapZone))
		{
			Character::getCharacterElement(i, &ZoneCharPack)->setRemoved(false);
			if (TileIA::getTileIAElement(i, &ZoneCharPack))
			{
				x = MapZone.x;
				y = MapZone.y;
				TileActor::getTileActorElement(i, &ZoneCharPack)->GenerateTileNodeID(x, y);
				TileActor::getTileActorElement(i, &ZoneCharPack)->GenerateAdjNode();
				TileActor::getTileActorElement(i, &ZoneCharPack)->CheckAdjNode(x, y);
			}
		}

	//Traite la zone visible
	for (x = MapZone.x; x < MapZone.x + Map->GetScreenData()->VisibleTileCount.x + 1; x++)
		for (y = MapZone.y; y < MapZone.y + Map->GetScreenData()->VisibleTileCount.y + 1; y++)
		{
			Hero->GenerateTileNodeID(x, y);
			Hero->GenerateAdjNode();
			Hero->CheckAdjNode(x, y);
			if (Map->GetMapData()->obstacle[y][x] == 1)
			{
				Obstacle[ObstacleCount].west = Map->GetMapData()->TileZone[x].west - Map->GetScreenData()->Camera.getPosition()->x;
				Obstacle[ObstacleCount].east = Map->GetMapData()->TileZone[x].east - Map->GetScreenData()->Camera.getPosition()->x;
				Obstacle[ObstacleCount].north = Map->GetMapData()->TileZone[y].north - Map->GetScreenData()->Camera.getPosition()->y;
				Obstacle[ObstacleCount].south = Map->GetMapData()->TileZone[y].south - Map->GetScreenData()->Camera.getPosition()->y;
				ObstacleCount++;
			}
			if (PBook->GetStatus(GUARD_QUEST) == 3)
			{
				if (Map->GetMapData()->TileID[y][x][DECORATION] == 45)
				{
					Map->GetMapData()->TileID[y][x][DECORATION] = 47;
					Map->GetMapData()->obstacle[y][x] = 0;
				}
				if (Map->GetMapData()->TileID[y][x][DECORATION] == 46)
				{
					Map->GetMapData()->TileID[y][x][DECORATION] = 48;
					Map->GetMapData()->obstacle[y][x] = 0;
				}
			}
#ifdef _DEBUG
			if ((pZone->PathNodesLayer == true) && (Hero->GetTileIATarget() != NULL))
				Hero->GetTileIATarget()->DisplayPaths(x, y, PATHNODES);
#endif
		}

	// On traite les pickups
	for (i = 0; i < ZonePickupPack.GetElementCount(); i++)
		if ((Pickup::getPickupElement(i, &ZonePickupPack)->getRemoved() == false) && (Pickup::getPickupElement(i, &ZonePickupPack)->GetStatus()->picked == false))
		{
			Pickup::getPickupElement(i, &ZonePickupPack)->setRemoved(true);
			Pickup::getPickupElement(i, &ZonePickupPack)->GenerateAni(0);
			Pickup::getPickupElement(i, &ZonePickupPack)->GenerateMapPosition();
		}

	// On trie les personnages par plan
	if (!ZoneCharPack.GetIsOrdered())
		ZoneCharPack.OrderBy(VisualList::ORDER_BY_PLAN);

	// On met à jour la clible du joueur
	if (cursor->buttonReleased() == Cursor::LEFT_CLICK)
		for (i = ZoneCharPack.GetElementCount()-1; i >= 0; i--)
			if (Character::getCharacterElement(i, &ZoneCharPack) != Hero && cursor->collision(Character::getCharacterElement(i, &ZoneCharPack)))
			{
				Hero->SetTarget(Character::getCharacterElement(i, &ZoneCharPack));
				Hero->Undo();
				PTargetGauge->SetFocus(Character::getCharacterElement(i, &ZoneCharPack));
				PTargetInventory->SetFocus(Humanized::getHumanizedElement(i, &ZoneCharPack));
				PTargetGauge->show();
				PTargetGauge->Refresh();
				PTargetInventory->Refresh();
				break;
			}

	// On traite chaque personnage
	for (i = ZoneCharPack.GetElementCount()-1; i >= 0; i--)
		if ((Item::getItemElement(i, &ZoneCharPack)->getRemoved() == false) | (Character::getCharacterElement(i, &ZoneCharPack)->GetStatus()->hitted == true))
		{
			Character::getCharacterElement(i, &ZoneCharPack)->GenerateAni(&SoundPack);
			Character::getCharacterElement(i, &ZoneCharPack)->GenerateStatus(PWorldTime->GetMeditateProgress(), &EventsReporter, &ActionReporter);
			EventResult = Character::getCharacterElement(i, &ZoneCharPack)->GenerateSkill(ORCData, &EventsReporter, &ActionReporter);
			switch (EventResult)
			{
			case SKILL_UP:
				Sound::getSoundElement(LEVEL_UP, &SoundPack)->play(DMUS_SEGF_SECONDARY);
				Humanized::getHumanizedElement(i, &ZoneCharPack)->GenerateTitle();
				PCharInfo->GetPSkills()->WaitForRefresh();
				PGauge->Refresh();
				break;
			case COMBAT_SKILL_UP:
				Sound::getSoundElement(LEVEL_UP, &SoundPack)->play(DMUS_SEGF_SECONDARY);
				Humanized::getHumanizedElement(i, &ZoneCharPack)->GenerateTitle();
				PCharInfo->GetPCombatSkills()->WaitForRefresh();
				PGauge->Refresh();
				break;
			case MAGIC_SKILL_UP:
				Sound::getSoundElement(LEVEL_UP, &SoundPack)->play(DMUS_SEGF_SECONDARY);
				Humanized::getHumanizedElement(i, &ZoneCharPack)->GenerateTitle();
				PCharInfo->GetPMagicSkill()->WaitForRefresh();
				PGauge->Refresh();
				break;
			}
			ActionResult = Character::getCharacterElement(i, &ZoneCharPack)->CheckActionResult(ORCData, &EventsReporter, &ActionReporter);
			switch (ActionResult)
			{
			case PICKUP_STEALED:
				PInventory->WaitForRefresh();
				PTargetInventory->WaitForRefresh();
				break;
			case POTIONGIFT_ADDED:
				if (PBook->GetStatus(PRIEST_QUEST) != 3)
				{
					PBook->show();
					PBook->WaitForRefresh();
				}
				break;
			default:
				break;
			}
			if (Player::getPlayerElement(i, &ZoneCharPack))
			{
				if ((Player::getPlayerElement(i, &ZoneCharPack)->GetStatus()->attack == false) && (Player::getPlayerElement(i, &ZoneCharPack)->GetStatus()->concentrate == false) && (Player::getPlayerElement(i, &ZoneCharPack)->GetStatus()->sleep == false))
					Player::getPlayerElement(i, &ZoneCharPack)->GenerateDisplacement(Map, &ScreenEdge, Obstacle, ObstacleCount, true, false);
				if (Player::getPlayerElement(i, &ZoneCharPack)->GetMagnitude())
					Player::getPlayerElement(i, &ZoneCharPack)->GetStatus()->attacking = false;
				if (Player::getPlayerElement(i, &ZoneCharPack)->GetTarget() == Player::getPlayerElement(i, &ZoneCharPack))
					PTargetGauge->hide();
				PBook->SetPotionGift(Player::getPlayerElement(i, &ZoneCharPack)->GetPotionGift()->GetElementCount());
				if (PBook->GetPotionGift() >= TOTAL_POTION_GIFT)
				{
					if (!PBook->GetMinPotionGift())
					{
						PBook->SetMinPotionGift(true);
						PBook->AddComment(PWorldTime->GetDay(), true, PRIEST_STEP3, &EventsReporter);
					}
				}
				else
					PBook->SetMinPotionGift(false);
				if ((Player::getPlayerElement(i, &ZoneCharPack)->GetStatus()->spy == true) && (Player::getPlayerElement(i, &ZoneCharPack)->GetStatus()->Action == SPY))
				{
					Player::getPlayerElement(i, &ZoneCharPack)->GetStatus()->Action = NONE_CHAR_ACTION;
					*PTargetInventory->getPosition() = Axe(Player::getPlayerElement(i, &ZoneCharPack)->GetTarget()->getPosition()->x, Player::getPlayerElement(i, &ZoneCharPack)->GetTarget()->getPosition()->y + Player::getPlayerElement(i, &ZoneCharPack)->GetTarget()->getDimension()->y);
					PTargetInventory->show();
					PTargetInventory->Refresh();
					if (Player::getPlayerElement(i, &ZoneCharPack)->GetMobTarget())
					{
						if (!Player::getPlayerElement(i, &ZoneCharPack)->GetMobTarget()->GetZoneStatus(pZone->CoordX, pZone->CoordY)->spyed)
						{
							int xp = Player::getPlayerElement(i, &ZoneCharPack)->GetSkillPack()->GetSkill(SPYING)->GainXP(NULL, NULL, MAX_CHAR_SKILL_LEVEL, &EventsReporter);
							if (xp) ActionReporter.ReleaseEvent(Player::getPlayerElement(i, &ZoneCharPack)->getPosition()->x, Player::getPlayerElement(i, &ZoneCharPack)->getPosition()->y, BuildString("+%d xp", xp), XP_MSG_COLOR);
							Player::getPlayerElement(i, &ZoneCharPack)->GetMobTarget()->GetZoneStatus(pZone->CoordX, pZone->CoordY)->spyed = 1;
						}
						else
							EventsReporter.AddMessage(XP_GAIN_DENIED, 0);
					}
				}
				if (!Player::getPlayerElement(i, &ZoneCharPack)->GetAdjacent(Player::getPlayerElement(i, &ZoneCharPack)->GetTarget()))
				{
					if (!Player::getPlayerElement(i, &ZoneCharPack)->GetStatus()->spy)
						PTargetInventory->hide();
					PGreatMago->hide();
					PDialogue->hide();
				}
				if (Player::getPlayerElement(i, &ZoneCharPack)->GetPnjTarget())
					if ((Player::getPlayerElement(i, &ZoneCharPack)->GetAdjacent(Player::getPlayerElement(i, &ZoneCharPack)->GetPnjTarget()) == true) && (Player::getPlayerElement(i, &ZoneCharPack)->GetPnjTarget()->GetPNJJob() == TRADER) && (Player::getPlayerElement(i, &ZoneCharPack)->GetPnjTarget()->GetStatus()->fight == false))
					{
						Player::getPlayerElement(i, &ZoneCharPack)->GetStatus()->trading = true;
						Player::getPlayerElement(i, &ZoneCharPack)->GetPnjTarget()->GetStatus()->trading = true;
					}
					else
					{
						Player::getPlayerElement(i, &ZoneCharPack)->GetStatus()->trading = false;
						Player::getPlayerElement(i, &ZoneCharPack)->GetPnjTarget()->GetStatus()->trading = false;
					}
				if ((Player::getPlayerElement(i, &ZoneCharPack)->getPosition()->x + Map->GetScreenData()->Camera.getPosition()->x < 0) | (Player::getPlayerElement(i, &ZoneCharPack)->getPosition()->x + Player::getPlayerElement(i, &ZoneCharPack)->getDimension()->x + Map->GetScreenData()->Camera.getPosition()->x > Map->GetMapData()->TileCount.x * Map->GetTileData()->getDimension()->x))
					*Character::getCharacterElement(i, &ZoneCharPack)->GetBlockedDirection() = CardinalPoint(0, 0, 1, 1);
				if ((Player::getPlayerElement(i, &ZoneCharPack)->getPosition()->y + Map->GetScreenData()->Camera.getPosition()->y < 0) | (Player::getPlayerElement(i, &ZoneCharPack)->getPosition()->y + Player::getPlayerElement(i, &ZoneCharPack)->getDimension()->y + Map->GetScreenData()->Camera.getPosition()->y > Map->GetMapData()->TileCount.y * Map->GetTileData()->getDimension()->y))
					*Character::getCharacterElement(i, &ZoneCharPack)->GetBlockedDirection() = CardinalPoint(1, 1, 0, 0);
				if (Player::getPlayerElement(i, &ZoneCharPack)->getPosition()->x + Player::getPlayerElement(i, &ZoneCharPack)->getDimension()->x + Map->GetScreenData()->Camera.getPosition()->x < 0)
					ChangeZone(Player::getPlayerElement(i, &ZoneCharPack), ZoneCoord.x - 1, ZoneCoord.y, Screen->GetCurrentMode()->GetWidth() - Player::getPlayerElement(i, &ZoneCharPack)->getDimension()->x, Player::getPlayerElement(i, &ZoneCharPack)->getPosition()->y, Map->GetScreenData()->MaxCamera.east, Map->GetScreenData()->Camera.getPosition()->y, false, false);
				if (Player::getPlayerElement(i, &ZoneCharPack)->getPosition()->x + Map->GetScreenData()->Camera.getPosition()->x > Map->GetMapData()->TileCount.x * Map->GetTileData()->getDimension()->x)
					ChangeZone(Player::getPlayerElement(i, &ZoneCharPack), ZoneCoord.x + 1, ZoneCoord.y, 0, Player::getPlayerElement(i, &ZoneCharPack)->getPosition()->y, 0, Map->GetScreenData()->Camera.getPosition()->y, false, false);
				if (Player::getPlayerElement(i, &ZoneCharPack)->getPosition()->y + Player::getPlayerElement(i, &ZoneCharPack)->getDimension()->y + Map->GetScreenData()->Camera.getPosition()->y < 0)
					ChangeZone(Player::getPlayerElement(i, &ZoneCharPack), ZoneCoord.x, ZoneCoord.y - 1, Player::getPlayerElement(i, &ZoneCharPack)->getPosition()->x, Screen->GetCurrentMode()->GetHeight() - Player::getPlayerElement(i, &ZoneCharPack)->getDimension()->y, Map->GetScreenData()->Camera.getPosition()->x, Map->GetScreenData()->MaxCamera.south, false, false);
				if (Player::getPlayerElement(i, &ZoneCharPack)->getPosition()->y + Map->GetScreenData()->Camera.getPosition()->y > Map->GetMapData()->TileCount.y * Map->GetTileData()->getDimension()->y)
					ChangeZone(Player::getPlayerElement(i, &ZoneCharPack), ZoneCoord.x, ZoneCoord.y + 1, Player::getPlayerElement(i, &ZoneCharPack)->getPosition()->x, 0, Map->GetScreenData()->Camera.getPosition()->x, 0, false, false);
			}
			else
				if (TileIA::getTileIAElement(i, &ZoneCharPack))
				{
					if (TileIA::getTileIAElement(i, &ZoneCharPack)->GetCompass()->GetPaths()->GetElementCount())
						TileIA::getTileIAElement(i, &ZoneCharPack)->SetMapPosition();
					if ((Character::getCharacterElement(i, &ZoneCharPack)->GetStatus()->fight == false) && (TileIA::getTileIAElement(i, &ZoneCharPack)->GenerateDests(&ZoneCharPack) == true))
						Character::getCharacterElement(i, &ZoneCharPack)->SetAsOwnTarget();
				}
			// S'inclut dans un combat
			if (BattlePack.DispatchCombatant(Character::getCharacterElement(i, &ZoneCharPack), PBattle, &EventsReporter))
			{
				PCharInfo->hide();
				PInventory->hide();
				PTargetInventory->hide();
				PGreatMago->hide();
				PChemicalLab->hide();
				PMagicSpell->hide();
				PBook->hide();
				PDialogue->hide();
				PPickup->hide();
				PTargetGauge->SetFocus(Hero->GetTarget());
				PTargetGauge->show();
				PTargetGauge->Refresh();
			}
			// N'a plus de vie
			if ((Character::getCharacterElement(i, &ZoneCharPack)->GetStatus()->dead == false) && (Character::getCharacterElement(i, &ZoneCharPack)->GetHitPoints()->GetAmount() <= 0))
			{
				if (Player::getPlayerElement(i, &ZoneCharPack))
					Player::getPlayerElement(i, &ZoneCharPack)->Diiiiiie(&EventsReporter, &ActionReporter);
				else
				{
					if (LinkAcolytPack.SearchElement(Character::getCharacterElement(i, &ZoneCharPack)) != -1) PBook->IncreaseAcolytKill();
					if (Mob::getMobElement(i, &ZoneCharPack)) Mob::getMobElement(i, &ZoneCharPack)->Diiiiiie(pZone->CoordX, pZone->CoordY, &EventsReporter, &ActionReporter);
				}
				j = BattlePack.SearchElementWithCombatant(Character::getCharacterElement(i, &ZoneCharPack));
				if (j != -1) BattlePack.GetCombatSystemElement(j)->RemoveCombatant(Character::getCharacterElement(i, &ZoneCharPack), &EventsReporter);
			}
		}
		else
			Character::getCharacterElement(i, &ZoneCharPack)->ShutUp();

	// On gére les combats
	for (i = 0; i < BattlePack.GetElementCount(); i++)
		if (BattlePack.GetCombatSystemElement(i)->GetRunning())
		{
			BattlePack.GetCombatSystemElement(i)->RunRound(&EventsReporter);
			if (BattlePack.GetCombatSystemElement(i)->GetCombatantPack()->GetElementCount() <= 1)
			{
				BattlePack.GetCombatSystemElement(i)->End(&EventsReporter);
				BattlePack.RemoveElement(i, 1, true);
			}
		}

	// On gére la position des persos sur la map
	for (i = 0; i < ZoneCharPack.GetElementCount(); i++)
		if (!Item::getItemElement(i, &ZoneCharPack)->getRemoved())
			if (TileIA::getTileIAElement(i, &ZoneCharPack))
			{
				TileActor::getTileActorElement(i, &ZoneCharPack)->GenerateMapPosition();
				Character::getCharacterElement(i, &ZoneCharPack)->GenerateDisplacement(NULL, NULL, NULL, NULL, false, false);
			}

	// Mort des acolytes relais, le chao diminue
	if ((PBook->GetCurseStatus() == 2) && (PBook->GetAcolytKill() >= 3))
	{
		PBook->SetCurseStatus(1);
		for (y = 3; y < TOTAL_ZONE_Y; y++)
			for (x = 4; x < TOTAL_ZONE_X; x++)
				UpdateCurse(x, y, 0, pZone);
		for (x = 0; x < 2; x++)
			UpdateCurse(x, 5, 0, pZone);
	}

	// Fin du chao
	if ((PBook->GetCurseStatus() > 0) && (Wizard->GetStatus()->dead == true))
	{
		PBook->SetCurseStatus(0);
		for (y = 0; y < TOTAL_ZONE_Y; y++)
			for (x = 0; x < TOTAL_ZONE_X; x++)
				UpdateCurse(x, y, 0, pZone);
	}

	// Dialogues et quêtes
	for (i = 1; i < TOTAL_CHAR_PNJ_QUEST; i++)
		if (PBook->GetStatus(i) == 1)
			switch (i)
			{
			case WARRIOR_QUEST:
				if (PBook->GetAcolytKill() >= 3)
				{
					PBook->SetStatus(WARRIOR_QUEST, 3);
					PDialogue->SetStep(WARRIOR_QUEST, PDialogue->GetStep(WARRIOR_QUEST) + 1);
					PBook->AddComment(PWorldTime->GetDay(), true, WARRIOR_STEP2, &EventsReporter);
				}
				break;
			case PRIEST_QUEST:
				if (PBook->GetMaxHeal())
					PBook->SetStatus(PRIEST_QUEST, 2);
				break;
			case FINAL_QUEST:
				if (Wizard->GetStatus()->dead)
				{
					PBook->SetStatus(FINAL_QUEST, 3);
					PDialogue->SetStep(FINAL_QUEST, PDialogue->GetStep(FINAL_QUEST) + 1);
					PBook->AddComment(PWorldTime->GetDay(), true, FINAL_STEP2, &EventsReporter);
				}
				break;
			}

	// Les barres d'expériences
	for (i = 0; i < Hero->GetSkillBarPack()->GetElementCount(); i++)
	{
		EnergyBar::getEnergyBarElement(i, Hero->GetSkillBarPack())->SetMaxAmount((float)Hero->GetSkillPack()->getCharSkillElement(i)->GetBPData()->level);
		EnergyBar::getEnergyBarElement(i, Hero->GetSkillBarPack())->SetAmount((float)Hero->GetSkillPack()->getCharSkillElement(i)->GetBPData()->gain);
		EnergyBar::getEnergyBarElement(i, Hero->GetSkillBarPack())->GenerateBar();
	}
	for (i = 0; i < Hero->GetCombatSkillBarPack()->GetElementCount(); i++)
	{
		EnergyBar::getEnergyBarElement(i, Hero->GetCombatSkillBarPack())->SetMaxAmount((float)Hero->GetCombatSkillPack()->getCharSkillElement(i)->GetBPData()->level);
		EnergyBar::getEnergyBarElement(i, Hero->GetCombatSkillBarPack())->SetAmount((float)Hero->GetCombatSkillPack()->getCharSkillElement(i)->GetBPData()->gain);
		EnergyBar::getEnergyBarElement(i, Hero->GetCombatSkillBarPack())->GenerateBar();
	}
	Hero->GetMagicSkillBar()->SetMaxAmount((float)Hero->GetMagicSkill()->GetBPData()->level);
	Hero->GetMagicSkillBar()->SetAmount((float)Hero->GetMagicSkill()->GetBPData()->gain);
	Hero->GetMagicSkillBar()->GenerateBar();

	// On affiche en défilement les mini-événements
	ActionReporter.GenerateCollisions();
	ActionReporter.ScrollEvents();

	// Panel pour les testes
	if (PTest)
	{
		Screen->LoadText(PTest->GetTextData(0, 0)->GetSprite(), NULL, (Str)"fps: " + Screen->GetFPS(), RGB(255, 255, 255), RGB(0, 0, 0));
		PTest->GetTextData(0, 0)->GetSprite()->SetColorKey(RGB(255, 255, 0));
		Screen->LoadText(PTest->GetTextData(1, 0)->GetSprite(), NULL, (Str)"always walk: " + (int)Hero->GetAlwaysWalk(), RGB(255, 255, 255), RGB(0, 0, 0));
		PTest->GetTextData(1, 0)->GetSprite()->SetColorKey(RGB(255, 255, 0));
		PTest->Refresh();
	}

	// On rafraîchit les panels qui l'on demandé
	for (i = 0; i < PanelPack.GetElementCount(); i++)
		Panel::getPanelElement(i, &PanelPack)->StartRefresh();

	// On trie les panels par plan
	if (!PanelPack.GetIsOrdered())
		PanelPack.OrderBy(VisualList::ORDER_BY_PLAN);

	// On bouge, génère, superpose et affiche les panels
	for (i = PanelPack.GetElementCount()-1; i >= 0; i--)
	{
		Panel::getPanelElement(i, &PanelPack)->GenerateForbidden2();
		if (Visual::getVisualElement(i, &PanelPack)->getVisible())
		{
			MovePanel(Panel::getPanelElement(i, &PanelPack), cursor, PanelPack, Hero->GetSwatch(BASIC)->isGoing());
			Panel::getPanelElement(i, &PanelPack)->generate(Hero->GetSwatch(BASIC)->isGoing() == false, cursor);
			Panel::getPanelElement(i, &PanelPack)->Run(cursor);
		}
		else
			StopPanel(Panel::getPanelElement(i, &PanelPack), cursor->getMover());
	}

	if (PTest) PTest->Run(cursor);

	// La carte
	if (LandMap->getVisible())
	{
		LandMap->MoveItemOnMap(LandMap->GetItemMapElement(0), Hero->getPosition()->x, Hero->getPosition()->y, Map->GetScreenData()->Camera.getPosition()->x, Map->GetScreenData()->Camera.getPosition()->y, ZoneCoord.x, ZoneCoord.y, true);
		LandMap->ShowMap();
	}

	// On gère l'IA des personnages
	for (i = 0; i < ZoneCharPack.GetElementCount(); i++)
		ProcessAI(Character::getCharacterElement(i, &ZoneCharPack));

	return true;
}

bool ProcessAI(Character *pCharacter)
{
	int i;
	MagicSpell *pMagicSpell = NULL;
	Character *CharacterTarget;
	Mob *pMob = dynamic_cast<Mob*>(pCharacter);
	Creature *pCreature = dynamic_cast<Creature*>(pCharacter);

	if (PData->GetThread()) return false;

	if ((pMob == NULL) | (pCharacter->getRemoved() == true))
		return false;
		
	if (pCharacter->GetStatus()->access)
	{
		// La créature sélectionne sa cible la plus proche
		if (pCharacter->HasNoTarget())
		{
			pMob->SelectNearestTarget();
			pCharacter->SetTarget(dynamic_cast<Character*>(pMob->GetCurrentTarget()));
		}

		if (pCharacter->GetTarget()->GetStatus()->dead)
			if (pMob->SelectAnotherTarget())
				pCharacter->SetTarget(dynamic_cast<Character*>(pMob->GetCurrentTarget()));
		
		CharacterTarget = pCharacter->GetTarget();

		//Si la créature est arrêtée ou a finit de suivre un chemin ou a parcouru sa distance maxi pendant un combat
		if (!pCharacter->GetMagnitude())
		{
			if (!pCharacter->GetStatus()->fight)
				pCharacter->Wait(BASIC, 3, 1);
			if (pCharacter->GetHitPoints()->GetAmount() >= pCharacter->GetStat2Pack()->GetStat(HIT_POINTS)->GetScore() - pCharacter->GetStat2Pack()->GetStat(HIT_POINTS)->GetScore() / 3)
			{
				if (pCreature)
				{
					if ((pCreature->GetManaPoints()->GetAmount() == pCreature->GetManaPoints()->GetMaxAmount()) | (pCharacter->GetBehavior()->FavoriteSkill1 != MEDITATION) | (pCharacter->GetStatus()->fight == true))
					{
						if ((pCharacter->GetBehavior()->thief == true) && (pCharacter->HasNoTarget() == false))
						{
							if (pCreature->GetStealing())
								if ((pCreature->GetStealing()->GetOwnerID() == pCharacter->getId()) | (pCreature->GetStealing()->GetOwnerID() == -1))
									pCreature->SetStealing(NULL);
							if ((pCharacter->GetSwatch(BASIC)->isGoing() == false) && (pCreature->GetStealing() == NULL) && (pMob->GetRange(CharacterTarget->GetTileNodeID()) <= 200))
								pCreature->Spy();
						}
						if (((pCharacter->GetBehavior()->aggro == true) | (pCharacter->GetStatus()->fight == true)) && ((pCharacter->GetStatus()->fight == false) | (pCharacter->GetStatus()->Action == NONE_CHAR_ACTION)) && (CharacterTarget->GetStatus()->dead == false) && (pCharacter->HasNoTarget() == false))
						{
							//Si la créature touche sa cible
							if (pMob->GetAdjacent(CharacterTarget))
							{
								//Si il existe, mémorise un sort de réflexion
								if (!pCreature->GetCurrentSpell()->GetEffectDuration()->getCounter())
									pCreature->SetMemorizedSpell(pCreature->GetMagicSpellPack(PROTECTION-1)->GetHigherMagicSpell(REFLECTING));
								if (((pCharacter->GetSwatch(BASIC)->isGoing() == false) | (pCharacter->GetStatus()->fight == true)) && (pCharacter->GetStatus()->hitted == false))
									if ((pCreature->GetMemorizedSpell() != NULL) && (pCreature->GetManaPoints()->GetAmount() >= pCreature->GetMemorizedSpell()->GetManaCost()->RealScore) && (pCreature->GetCurrentSpell()->GetEffectDuration()->getCounter() == 0))
										pCreature->Concentrate(false);
								if (!pCharacter->GetStatus()->concentrate)
									pCharacter->Attack();
							}
							else
							{
								//Mémorise un sort de destruction ou de protection
								switch (pCreature->GetMagicSchoolPack()->GetBestStat()->GetStatId())
								{
								case DESTRUCTION:
									if (pCreature)
										pCreature->MemorizeSpell(DESTRUCTION);
									if (((pCharacter->GetSwatch(BASIC)->isGoing() == false) | (pCharacter->GetStatus()->fight == true)) && (pCharacter->GetStatus()->hitted == false))
									{
										if ((pCreature->GetMemorizedSpell() != NULL) && (pMob->GetRange(CharacterTarget->GetTileNodeID()) <= pCreature->GetMemorizedSpell()->GetRange()->data) && (pCreature->GetCurrentSpell()->GetEffectDuration()->isGoing() == false) && (pCreature->GetCurrentSpell()->GetEffectDuration()->getCounter() == 0))
										{
											pMob->Face(CharacterTarget);
											pCreature->Concentrate(NULL);
										}
										if (!pCharacter->GetStatus()->concentrate)
											pCharacter->GetStatus()->Action = MOVEMENT;
									}
									break;
								case PROTECTION:
									if (pCreature)
										pCreature->MemorizeSpell(PROTECTION);
									if (((pCharacter->GetSwatch(BASIC)->isGoing() == false) | (pCharacter->GetStatus()->fight == true)) && (pCharacter->GetStatus()->hitted == false))
									{
										if ((pCreature->GetMemorizedSpell() != NULL) && (pCreature->GetCurrentSpell()->GetEffectDuration()->getCounter() == 0) && (pCharacter->GetStatus()->absorbing == false) && (pCharacter->GetStatus()->reflecting == false))
											pCreature->Concentrate(false);
										if (!pCharacter->GetStatus()->concentrate)
											pCharacter->GetStatus()->Action = MOVEMENT;
									}
									break;
								default:
									pCharacter->GetStatus()->Action = MOVEMENT;
								}
							}
						}
					}
					else
						if ((pCharacter->GetSwatch(BASIC)->isGoing() == false) && (pCharacter->GetStatus()->hitted == false))
							pCharacter->Sleep();
				}
				else
					if (((pCharacter->GetBehavior()->aggro == true) | (pCharacter->GetStatus()->fight == true)) && ((pCharacter->GetStatus()->fight == false) | (pCharacter->GetStatus()->Action == NONE_CHAR_ACTION)) && (CharacterTarget->GetStatus()->dead == false) && (pCharacter->HasNoTarget() == false))
						if (pMob->GetAdjacent(CharacterTarget))
							pCharacter->Attack();
						else
							if (((pCharacter->GetSwatch(BASIC)->isGoing() == false) | (pCharacter->GetStatus()->fight == true)) && (pCharacter->GetStatus()->hitted == false))
								pCharacter->GetStatus()->Action = MOVEMENT;
			}
			else
			{
				//Mémorise un sort de guérison
				if (pCreature)
					pCreature->MemorizeSpell(CURE);
				if (((pCharacter->GetSwatch(BASIC)->isGoing() == false) | (pCharacter->GetStatus()->fight == true)) && (pCharacter->GetStatus()->hitted == false))
				{
					if (pCreature)
						if ((pCreature->GetMemorizedSpell() != NULL) && (pCreature->GetCurrentSpell()->GetHit() == false))
							pCreature->Concentrate(false);
					if (!pCharacter->GetStatus()->concentrate)
					{
						if (pCharacter->GetStatus()->fight)
						{
							if ((pMob->GetAdjacent(CharacterTarget) == true) && (CharacterTarget->GetStatus()->dead == false) && (pCharacter->HasNoTarget() == false))
								pCharacter->Attack();
							else
								pCharacter->GetStatus()->Action = MOVEMENT;
						}
						else
							pCharacter->Sleep();
					}
				}
			}
			//Comportement de voleur: cherche un objet à voler, si trouvé et touche sa cible, lui fait les poches
			if (pCreature) pCreature->LookForStealing();
		}

		//Essaie de trouver son chemin
		if ((pCharacter->GetBehavior()->move == true) && (pCharacter->GetStatus()->Action == MOVEMENT) && (pCharacter->HasNoTarget() == false))
		{
			if ((pCharacter->GetMagnitude() == 0) && (CharacterTarget->GetStatus()->dead == false))
			{
				switch (pMob->GetCompass()->GetThreadResult())
				{
				case -2:
					if (!pCharacter->GetSwatch(SEARCH)->isGoing())
					{
						CharacterTarget->FreeAdjNode(pCharacter->getId());
						CharacterTarget->GetIgnoredBy()->RemoveElement(pCharacter);
						pMob->SetTargetNode(CharacterTarget->GetFreeAdjNodeID());
						if (pMob->GetTargetNode())
						{
							if (CharacterTarget->FillAdjNode(pMob->GetTargetNode(), pCharacter->getId()))
							{
								EventsReporter.AddMessage(DEBUG_ACTION, BuildString("%s a prit la node %d", pCharacter->getName()->Get(), pMob->GetTargetNode()->id), SKILL_RESULT_MSG_COLOR);
								pMob->GetCompass()->SetCheckObstacle(true);
								pMob->GetCompass()->StartSearch(pMob->GetTileNodeID(), pMob->GetTargetNode()->id);
								pMob->GetCompass()->RunThread();
							}
							else
							{
								pMob->SetTargetNode(NULL);
								pMob->GetDests()->RemoveElement(CharacterTarget);
								pCharacter->SetAsOwnTarget();
								pCharacter->Wait(SEARCH, 3, 1);
								EventsReporter.AddMessage(DEBUG_ACTION, BuildString("%s n'a pas réussit à prendre la node %d", pCharacter->getName()->Get(), pMob->GetTargetNode()->id), SKILL_RESULT_MSG_COLOR);
							}
						}
						else
							pCharacter->Wait(SEARCH, 3, 1);
					}
					else
						pCharacter->Wait(SEARCH, 3, 1);
					break;
				case -1:
					EventsReporter.AddMessage(DEBUG_ACTION, BuildString("%s n'a pas trouvé sa cible", pCharacter->getName()->Get()), DAMAGE_MSG_COLOR);
					pMob->SetTargetNode(NULL);
					CharacterTarget->FreeAdjNode(pCharacter->getId());
					if (pMob->SelectAnotherTarget())
						pCharacter->SetTarget(dynamic_cast<Character*>(pMob->GetCurrentTarget()));
					if (pCharacter->GetStatus()->fight)
					{
						i = BattlePack.SearchElementWithCombatant(pCharacter);
						if (i != -1)
						{
							BattlePack.GetCombatSystemElement(i)->RemoveCombatant(pCharacter, &EventsReporter);
							if (BattlePack.SearchElementWithPanel() == i) PBattle->Refresh();
						}
					}
					pCharacter->Wait(SEARCH, 3, 1);
					break;
				case 1:
					EventsReporter.AddMessage(DEBUG_ACTION, BuildString("%s a trouvé sa cible", pCharacter->getName()->Get()), TRADING_MSG_COLOR);
					pMob->GetCompass()->BuildPath();
					pMob->Face(CharacterTarget);
					pCharacter->GenerateAniRotation();
					pCharacter->PlaySoundA(pCharacter->GetGoSound(), 0, false);
					break;
				default:
					break; // La thread est toujours en cours
				}
			}
			//Suit le chemin trouvé
			if (pMob->GetCompass()->GetPaths()->GetElementCount())
				pMob->FollowCompass(CHAR_SPEED);
		}

		//Réactions pendant un combat
		i = BattlePack.SearchElementWithCombatant(pCharacter);
		if ((pCharacter->GetStatus()->fight == true) && (i != -1) && (BattlePack.GetCombatSystemElement(i)->GetRunning() == true))
		{
			if (!pCharacter->GetMagnitude())
			{
				//A finit de bouger
				if (pMob->GetRange(BattlePack.GetCombatSystemElement(i)->GetStartNodeID()))
					BattlePack.GetCombatSystemElement(i)->WaitForNextTurn(pCharacter);
				//Ne peut pas bouger
				if ((pMob->GetCompass()->GetThreadResult() != 0) && (pCharacter->GetStatus()->Action == MOVEMENT))
				{
					BattlePack.GetCombatSystemElement(i)->RemoveCombatant(pCharacter, &EventsReporter);
					if (BattlePack.GetCombatSystemElement(i)->GetPTurn()) BattlePack.GetCombatSystemElement(i)->GetPTurn()->Refresh();
				}
			}
		}
		else
			//Si la créature est agressive ou voleuse, s'arrête lorsque elle croise sa cible
			if ((pCharacter->GetBehavior()->aggro == true) | (pCharacter->GetBehavior()->thief == true))	
				if ((pMob->GetAdjacent(CharacterTarget) == true) && (pCharacter->GetMagnitude() > 0))
					pMob->StopMove();
	}

	if ((pMob->GetCompass()->GetPaths()->GetElementCount() == 0) && (pCharacter->GetStatus()->fight == false))
		pCharacter->setRemoved(true);

	return true;
}

bool ProcessKbInput()
{
	int i;

	if (PData->GetThread())
		return false;

	if (!keybListener.startCaptureKeys(Keyboard))
		return false;

	if (Hero->GetStatus()->access)
	{
		if (keybListener.keyPressed(KEY_LEFT, Config.GetKeyMap()))
			Hero->SetAngle(PI);
		if (keybListener.keyPressed(KEY_RIGHT, Config.GetKeyMap()))
			Hero->SetAngle(0);
		if (keybListener.keyPressed(KEY_UP, Config.GetKeyMap()))
			Hero->SetAngle(3 * PI / 2);
		if (keybListener.keyPressed(KEY_DOWN, Config.GetKeyMap()))
			Hero->SetAngle(PI / 2);
		if (keybListener.keyPressed(KEY_LEFT, Config.GetKeyMap()) && keybListener.keyPressed(KEY_UP, Config.GetKeyMap()))
			Hero->SetAngle(5 * PI / 4);
		if (keybListener.keyPressed(KEY_LEFT, Config.GetKeyMap()) && keybListener.keyPressed(KEY_DOWN, Config.GetKeyMap()))
			Hero->SetAngle(3 * PI / 4);
		if (keybListener.keyPressed(KEY_RIGHT, Config.GetKeyMap()) && keybListener.keyPressed(KEY_UP, Config.GetKeyMap()))
			Hero->SetAngle(7 * PI / 4);
		if (keybListener.keyPressed(KEY_RIGHT, Config.GetKeyMap()) && keybListener.keyPressed(KEY_DOWN, Config.GetKeyMap()))
			Hero->SetAngle(PI / 4);
		if (!keybListener.keyPressed(KEY_LEFT, Config.GetKeyMap()) && !keybListener.keyPressed(KEY_RIGHT, Config.GetKeyMap()) && !keybListener.keyPressed(KEY_UP, Config.GetKeyMap()) && !keybListener.keyPressed(KEY_DOWN, Config.GetKeyMap()))
		{
			if (!Hero->GetAlwaysWalk())
				Hero->StopMove();
		}
		else
			if ((BattlePack.SearchElementWithCombatant(Hero) == -1) | (Hero->GetStatus()->Action != ATTACK))
			{
				Hero->SetMagnitude(CHAR_SPEED);
				Hero->GetStatus()->spy = false;
				Hero->GetStatus()->Action = MOVEMENT;
			}
		if (keybListener.keyPressedOnce(KEY_ALWAYSWALK, Config.GetKeyMap()))
			Hero->SwitchAlwaysWalk();
		if (keybListener.keyPressed(KEY_PICKUP, Config.GetKeyMap()))
			Hero->GetStatus()->look = true;
		else
			Hero->GetStatus()->look = false;
		if ((Hero->GetStatus()->fight == false) | (Hero->GetStatus()->Action == NONE_CHAR_ACTION))
		{
			if (keybListener.keyPressed(KEY_ATTACK, Config.GetKeyMap()))
				Hero->Attack();
			if (keybListener.keyPressedOnce(KEY_CASTSPELL, Config.GetKeyMap()))
				Hero->Concentrate(true, &EventsReporter);
			if (keybListener.keyPressedOnce(KEY_MEDITATE, Config.GetKeyMap()))
				if ((Hero->GetHitPoints()->GetAmount() < Hero->GetHitPoints()->GetMaxAmount()) | (Hero->GetManaPoints()->GetAmount() < Hero->GetManaPoints()->GetMaxAmount()))
					Hero->Sleep();
				else
					EventsReporter.AddMessage(ACTION_DENIED, 3);
		}
		if (keybListener.keyPressedOnce(KEY_NEXTTURN, Config.GetKeyMap()))
		{
			i = BattlePack.SearchElementWithCombatant(Hero);
			if (i != -1)
				BattlePack.GetCombatSystemElement(i)->WaitForNextTurn(Hero);
		}
		if (keybListener.keyPressedOnce(KEY_SHORTCUT, Config.GetKeyMap()))
			if (!PShortCut->hide())
			{
				PShortCut->show();
				PShortCut->Refresh();
			}
		if (keybListener.keyPressedOnce(KEY_SHORT_INVENTORY, Config.GetKeyMap()))
			DisplayPanel(SHORT_INVENTORY);
		if (keybListener.keyPressedOnce(KEY_SHORT_CHARINFO, Config.GetKeyMap()))
			DisplayPanel(SHORT_CHARINFO);
		if (keybListener.keyPressedOnce(KEY_SHORT_SPELLLIST, Config.GetKeyMap()))
			DisplayPanel(SHORT_SPELLLIST);
		if (keybListener.keyPressedOnce(KEY_SHORT_CHEMICALLAB, Config.GetKeyMap()))
			DisplayPanel(SHORT_CHEMICALLAB);
		if (keybListener.keyPressedOnce(KEY_SHORT_LANDMAP, Config.GetKeyMap()))
			DisplayPanel(SHORT_LANDMAP);
		if (keybListener.keyPressedOnce(KEY_SHORT_QUESTBOOK, Config.GetKeyMap()))
			DisplayPanel(SHORT_QUESTBOOK);
		if (keybListener.keyPressedOnce(KEY_SHORT_EVENTS, Config.GetKeyMap()))
			DisplayPanel(SHORT_EVENTS);
#ifdef _DEBUG
		switch (keybListener.keyPressed())
		{
		case DIK_E:
			Hero->GetHitPoints()->Increase(1, 1, true);
			Hero->GetHitPoints()->GenerateBar();
			break;
		case DIK_D:
			Hero->GetHitPoints()->Decrease(1, 1, true);
			Hero->GetHitPoints()->GenerateBar();
			break;
		case DIK_R:
			Hero->GetManaPoints()->Increase(1, 1, true);
			Hero->GetManaPoints()->GenerateBar();
			break;
		case DIK_F:
			Hero->GetManaPoints()->Decrease(1, 1, true);
			Hero->GetManaPoints()->GenerateBar();
			break;
		case DIK_NUMPAD4:
			ChangeZone(Hero, ZoneCoord.x - 1, ZoneCoord.y, 0, 0, 0, 0, false, true);
			break;
		case DIK_NUMPAD6:
			ChangeZone(Hero, ZoneCoord.x + 1, ZoneCoord.y, 0, 0, 0, 0, false, true);
			break;
		case DIK_NUMPAD8:
			ChangeZone(Hero, ZoneCoord.x, ZoneCoord.y - 1, 0, 0, 0, 0, false, true);
			break;
		case DIK_NUMPAD2:
			ChangeZone(Hero, ZoneCoord.x, ZoneCoord.y + 1, 0, 0, 0, 0, false, true);
			break;
		}
#endif
	}

	if (keybListener.keyReleased(DIK_F1))
	{
		i = 0; //Break Point pour debug
	}
	else if (keybListener.keyReleased(DIK_F3))
	{
		ShowWindow(Screen->GetDxWnd(), SW_MINIMIZE);
		ShellExecute(Screen->GetDxWnd(), "open", "GameConfig.exe", NULL, NULL, SW_SHOW);
	}
	else if (keybListener.keyReleased(DIK_F4))
	{
		DestroyDirectDraw();
		DestroyDirectInput();
		Screen->GetCurrentMode()->Switch();
		RestoreDirectDraw();
		InitDirectInput();
	}
	else if (keybListener.keyReleased(DIK_F6))
	{
		if (CanLoadData())
		{
			PData->show();
			PData->Refresh();
			PData->CreateLoadThread();
		}
	}
	else if (keybListener.keyReleased(DIK_ESCAPE))
		DisplayPanel(SHORT_MENU);
#ifdef _DEBUG
	else if (keybListener.keyPressedOnce(DIK_0))
		EventsReporter.FilterMessage(0);
	else if (keybListener.keyPressedOnce(DIK_1))
		EventsReporter.FilterMessage(1);
	else if (keybListener.keyPressedOnce(DIK_2))
		EventsReporter.FilterMessage(2);
	else if (keybListener.keyPressedOnce(DIK_3))
		EventsReporter.FilterMessage(3);
	else if (keybListener.keyPressedOnce(DIK_4))
		EventsReporter.FilterMessage(4);
	else if (keybListener.keyPressedOnce(DIK_5))
		EventsReporter.FilterMessage(5);
	else if (keybListener.keyPressedOnce(DIK_6))
		EventsReporter.FilterMessage(6);
	else if (keybListener.keyPressedOnce(DIK_7))
		EventsReporter.FilterMessage(7);
	else if (keybListener.keyReleased(DIK_F5))
		if (!Hero->GetStatus()->dead)
		{
			PSave->centerPosition(Axe(Screen->GetCurrentMode()->GetWidth(), Screen->GetCurrentMode()->GetHeight()));
			PSave->SetRemainingPoints(Hero->GetSkillPoints(), Hero->GetMagicPoints());
			PSave->show();
			PSave->Refresh();
		}
#endif
	
	keybListener.captureKeys();

	return true;
}

bool ProcessMouseInput()
{
	DIMOUSESTATE DIms;
	HRESULT hr;
	CardinalPoint ScreenEdge = CardinalPoint(0, Screen->GetCurrentMode()->GetWidth(), 0, Screen->GetCurrentMode()->GetHeight());

	if (PData->GetThread()) return false;

	hr = Mouse->GetDevice()->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&DIms);

	if (hr != DI_OK)
	{
		ReportDxError(S_FALSE, DIRECTINPUT, ERR_DI_GETDEVICESTATE);
		return false;
	}
	else
	{
		if ((Hero->GetStatus()->access == true) | (PMenu->getVisible() == true) | ((Hero->GetStatus()->dead == true) && (LandMap->getVisible() == true)))
		{
			cursor->setClickState(DIms.rgbButtons);
			if (cursor->buttonPressed() != Cursor::NO_CLICK)
				Hero->Wait(BASIC, 1, 4);
			cursor->getMover()->GenerateMouseAngle(DIms.lX, DIms.lY);
			if (PEvents->GetSelector()->GetSliding())
				cursor->getMover()->GenerateCollision(DIms.lX, DIms.lY, PEvents->GetSelector()->GetStageLimit(), true);
			if (PBook->GetSelector()->GetSliding())
				cursor->getMover()->GenerateCollision(DIms.lX, DIms.lY, PBook->GetSelector()->GetStageLimit(), true);
			if (PInventory->GetOthers()->GetSelector()->GetSliding())
				cursor->getMover()->GenerateCollision(DIms.lX, DIms.lY, PInventory->GetOthers()->GetSelector()->GetStageLimit(), true);
			if (PInventory->GetWeapons()->GetSelector()->GetSliding())
				cursor->getMover()->GenerateCollision(DIms.lX, DIms.lY, PInventory->GetWeapons()->GetSelector()->GetStageLimit(), true);
			if (PInventory->GetArmors()->GetSelector()->GetSliding())
				cursor->getMover()->GenerateCollision(DIms.lX, DIms.lY, PInventory->GetArmors()->GetSelector()->GetStageLimit(), true);
			if (PInventory->GetIngredients()->GetSelector()->GetSliding())
				cursor->getMover()->GenerateCollision(DIms.lX, DIms.lY, PInventory->GetIngredients()->GetSelector()->GetStageLimit(), true);
			if (PTargetInventory->GetOthers()->GetSelector()->GetSliding())
				cursor->getMover()->GenerateCollision(DIms.lX, DIms.lY, PTargetInventory->GetOthers()->GetSelector()->GetStageLimit(), true);
			if (PTargetInventory->GetWeapons()->GetSelector()->GetSliding())
				cursor->getMover()->GenerateCollision(DIms.lX, DIms.lY, PTargetInventory->GetWeapons()->GetSelector()->GetStageLimit(), true);
			if (PTargetInventory->GetArmors()->GetSelector()->GetSliding())
				cursor->getMover()->GenerateCollision(DIms.lX, DIms.lY, PTargetInventory->GetArmors()->GetSelector()->GetStageLimit(), true);
			if (PTargetInventory->GetIngredients()->GetSelector()->GetSliding())
				cursor->getMover()->GenerateCollision(DIms.lX, DIms.lY, PTargetInventory->GetIngredients()->GetSelector()->GetStageLimit(), true);
			if (PGreatMago->GetDestruction()->GetSelector()->GetSliding())
				cursor->getMover()->GenerateCollision(DIms.lX, DIms.lY, PGreatMago->GetDestruction()->GetSelector()->GetStageLimit(), true);
			if (PGreatMago->GetProtection()->GetSelector()->GetSliding())
				cursor->getMover()->GenerateCollision(DIms.lX, DIms.lY, PGreatMago->GetProtection()->GetSelector()->GetStageLimit(), true);
			if ((PSpellList->GetSelector() != NULL) && (PSpellList->GetSelector()->GetSliding() == true))
				cursor->getMover()->GenerateCollision(DIms.lX, DIms.lY, PSpellList->GetSelector()->GetStageLimit(), true);
			cursor->getMover()->Move(DIms.lX, DIms.lY, &ScreenEdge, true);
			StickToCursor(cursor, cursor->getMover());
		}
	}

	return true;
}

void RestoreDirectDraw()
{
	int i;

	Screen->SetDisplayMode();

	//La map
	Map->GetTileData()->restoreAni();

	//Le curseur
	cursor->restoreAni();

	// Panel pour les testes
	if (PTest)
	{
		PTest->Initialize();
		PTest->Refresh();
	}

	//Panel PMenu
	PMenu->restoreAni();
	PMenu->Initialize();
	PMenu->Refresh();

	//Panel de progression des données
	PData->restoreAni();
	PData->Initialize();
	PData->Refresh();

	// Les autres panels
	for (i = 0; i < PanelPack.GetElementCount(); i++)
	{
		Panel::getPanelElement(i, &PanelPack)->restoreAni();
		Panel::getPanelElement(i, &PanelPack)->Initialize();
		Panel::getPanelElement(i, &PanelPack)->Refresh();
	}

	//La carte
	LandMap->restoreAni();

	//Les personnages
	for (i = 0; i < CharPack.GetElementCount(); i++)
		Visual::getVisualElement(i, &CharPack)->restoreAni();

	//Les pickups
	for (i = 0; i < PickupPack.GetElementCount(); i++)
		Visual::getVisualElement(i, &PickupPack)->restoreAni();
}

DWORD WINAPI SaveData(LPVOID lpParam)
{
	int i, j, x, y;
	Str SubIndex[3];
	DataFileList SaveFile = DataFileList("savegame.dat");

	PData->GetDataBar()->SetAmount(0);
	PData->GetDataBar()->GenerateBar();
	PData->setName("PREPARATION DES DONNEES");
	PData->Refresh();

	if (!SaveFile.Open(false, true, EraseFile)) ExitThread(-1);

	PData->setName("SAUVEGARDE DES DONNEES");
	PData->GetDataBar()->SetMaxAmount((float)SaveFile.GetIndexTotal());
	PData->Refresh();

	SaveFile.Write(TicTac->getCounter(), "TicTac_counter", false, "", false, GenerateDataProgress);
	SaveFile.Write(PWorldTime->GetDay(), "RanOutTime_day", false, "", false, GenerateDataProgress);
	SaveFile.Write(*PWorldTime->GetMeditateProgress(), "RanOutTime_meditate", false, "", false, GenerateDataProgress);
	SaveFile.Write(PWorldTime->GetChao(), "RanOutTime_chao", false, "", false, GenerateDataProgress);
	SaveFile.Write((int)PWorldTime->GetTodayBar()->GetAmount(), "Today_amount", false, "", false, GenerateDataProgress);
	SaveFile.Write((int)PWorldTime->GetChaoBar()->GetAmount(), "Chao_amount", false, "", false, GenerateDataProgress);
	SaveFile.Write(Hero->GetNameID(), "Hero_NameID", false, "", false, GenerateDataProgress);
	SaveFile.Write(Hero->GetSkillPoints(), "Hero_SkillPoints", false, "", false, GenerateDataProgress);
	SaveFile.Write(Hero->GetMagicPoints(), "Hero_MagicPoints", false, "", false, GenerateDataProgress);

	SaveFile.Write(Hero->GetStatPack()->GetElementCount(), "TOTAL_CHAR_STAT", false, "", false, GenerateDataProgress);
	for (i = 0; i < Hero->GetStatPack()->GetElementCount(); i++)
	{
		SubIndex[0].Cpy(i+1);
		SaveFile.Write(Hero->GetStatPack()->getCharStatElement(i)->GetScore(), SubIndex[0], true, "Hero_Stat_score", false, GenerateDataProgress);
		SaveFile.Write(Hero->GetStatPack()->getCharStatElement(i)->GetTempScore(), SubIndex[0], true, "Hero_TempStat_score", false, GenerateDataProgress);
	}

	SaveFile.Write(Hero->GetMagicSchoolPack()->GetElementCount(), "TOTAL_CHAR_MAGIC_SCHOOL", false, "", false, GenerateDataProgress);
	for (i = 0; i < Hero->GetMagicSchoolPack()->GetElementCount(); i++)
	{
		SubIndex[0].Cpy(i+1);
		SaveFile.Write(Hero->GetMagicSchoolPack()->getCharStatElement(i)->GetScore(), SubIndex[0], true, "Hero_MagicSchool_score", false, GenerateDataProgress);
		SaveFile.Write(Hero->GetMagicSchoolPack()->getCharStatElement(i)->GetTempScore(), SubIndex[0], true, "Hero_TempMagicSchool_score", false, GenerateDataProgress);
	}

	SaveFile.Write(Hero->GetSkillPack()->GetElementCount(), "TOTAL_CHAR_SKILL", false, "", false, GenerateDataProgress);
	for (i = 0; i < Hero->GetSkillPack()->GetElementCount(); i++)
	{
		SubIndex[0].Cpy(i+1);
		SaveFile.Write(Hero->GetSkillPack()->getCharSkillElement(i)->GetLevel(), SubIndex[0], true, "Hero_Skill_level", false, GenerateDataProgress);
		SaveFile.Write(Hero->GetSkillPack()->getCharSkillElement(i)->GetBPData()->gain, SubIndex[0], true, "Hero_Skill_BPData_gain", false, GenerateDataProgress);
		SaveFile.Write(Hero->GetSkillPack()->getCharSkillElement(i)->GetBPData()->total, SubIndex[0], true, "Hero_Skill_BPData_total", false, GenerateDataProgress);
	}

	SaveFile.Write(Hero->GetCombatSkillPack()->GetElementCount(), "TOTAL_CHAR_COMBAT_SKILL", false, "", false, GenerateDataProgress);
	for (i = 0; i < Hero->GetCombatSkillPack()->GetElementCount(); i++)
	{
		SubIndex[0].Cpy(i+1);
		SaveFile.Write(Hero->GetCombatSkillPack()->getCharSkillElement(i)->GetLevel(), SubIndex[0], true, "Hero_CombatSkill_level", false, GenerateDataProgress);
		SaveFile.Write(Hero->GetCombatSkillPack()->getCharSkillElement(i)->GetBPData()->gain, SubIndex[0], true, "Hero_CombatSkill_BPData_gain", false, GenerateDataProgress);
		SaveFile.Write(Hero->GetCombatSkillPack()->getCharSkillElement(i)->GetBPData()->total, SubIndex[0], true, "Hero_CombatSkill_BPData_total", false, GenerateDataProgress);
	}

	SaveFile.Write(Hero->GetMagicSkill()->GetLevel(), "Hero_MagicSkill_level", false, "", false, GenerateDataProgress);
	SaveFile.Write(Hero->GetMagicSkill()->GetBPData()->gain, "Hero_MagicSkill_BPData_gain", false, "", false, GenerateDataProgress);
	SaveFile.Write(Hero->GetMagicSkill()->GetBPData()->total, "Hero_MagicSkill_BPData_total", false, "", false, GenerateDataProgress);
	SaveFile.Write((int)Hero->GetHitPoints()->GetAmount(), "Hero_HitPoints_amount", false, "", false, GenerateDataProgress);
	SaveFile.Write((int)Hero->GetManaPoints()->GetAmount(), "Hero_ManaPoints_amount", false, "", false, GenerateDataProgress);

	SaveFile.Write(TOTAL_PICKUP_GROUP, "TOTAL_PICKUP_GROUP", false, "", false, GenerateDataProgress);
	for (i = 0; i < TOTAL_PICKUP_GROUP; i++)
	{
		SubIndex[0].Cpy(i+1);
		SaveFile.Write(Hero->GetPocket(i)->GetElementCount(), SubIndex[0], true, "Hero_Pocket_ElementCount", false, GenerateDataProgress);
		for (j = 0; j < Hero->GetPocket(i)->GetElementCount(); j++)
		{
			SubIndex[1] = SubIndex[0];
			SubIndex[1].Cat(j+1);
			SaveFile.Write(Pickup::getPickupElement(j, Hero->GetPocket(i))->GetPrice(), SubIndex[1], true, "Hero_Pocket_Pickup_price", false, GenerateDataProgress);
			SaveFile.Write(Pickup::getPickupElement(j, Hero->GetPocket(i))->GetUniqueID(), SubIndex[1], true, "Hero_Pocket_Pickup_UniqueID", false, GenerateDataProgress);
			SaveFile.Write(Pickup::getPickupElement(j, Hero->GetPocket(i))->GetType(), SubIndex[1], true, "Hero_Pocket_Pickup_type", false, GenerateDataProgress);
			SaveFile.Write(Pickup::getPickupElement(j, Hero->GetPocket(i))->GetClass(), SubIndex[1], true, "Hero_Pocket_Pickup_Class", false, GenerateDataProgress);
			SaveFile.Write((int)Pickup::getPickupElement(j, Hero->GetPocket(i))->GetStatus()->picked, SubIndex[1], true, "Hero_Pocket_Pickup_Status_picked", false, GenerateDataProgress);
			SaveFile.Write((int)Pickup::getPickupElement(j, Hero->GetPocket(i))->GetStatus()->equiped, SubIndex[1], true, "Hero_Pocket_Pickup_Status_equiped", false, GenerateDataProgress);
			if (Ingredient::getIngredientElement(j, Hero->GetPocket(i)))
				SaveFile.Write(Ingredient::getIngredientElement(j, Hero->GetPocket(i))->GetIngredientType(), SubIndex[1], true, "Hero_Pocket_Ingredient_IngredientType", false, GenerateDataProgress);
			else
				SaveFile.Write(-1, SubIndex[1], true, "Hero_Pocket_Ingredient_IngredientType", false, GenerateDataProgress);
			if (Potion::getPotionElement(j, Hero->GetPocket(i)))
			{
				SaveFile.Write((int)Potion::getPotionElement(j, Hero->GetPocket(i))->GetPower(), SubIndex[1], true, "Hero_Pocket_Potion_power", false, GenerateDataProgress);
				SaveFile.Write(Potion::getPotionElement(j, Hero->GetPocket(i))->GetEffect(), SubIndex[1], true, "Hero_Pocket_Potion_effect", false, GenerateDataProgress);
			}
			else
			{
				SaveFile.Write(-1, SubIndex[1], true, "Hero_Pocket_Potion_power", false, GenerateDataProgress);
				SaveFile.Write(-1, SubIndex[1], true, "Hero_Pocket_Potion_effect", false, GenerateDataProgress);
			}
			if (Weapon::getWeaponElement(j, Hero->GetPocket(i)))
			{
				SaveFile.Write(Weapon::getWeaponElement(j, Hero->GetPocket(i))->GetMinDamage(), SubIndex[1], true, "Hero_Pocket_Weapon_MinDamage", false, GenerateDataProgress);
				SaveFile.Write(Weapon::getWeaponElement(j, Hero->GetPocket(i))->GetMaxDamage(), SubIndex[1], true, "Hero_Pocket_Weapon_MaxDamage", false, GenerateDataProgress);
				SaveFile.Write(Weapon::getWeaponElement(j, Hero->GetPocket(i))->GetCombatSkill(), SubIndex[1], true, "Hero_Pocket_Weapon_CombatSkill", false, GenerateDataProgress);
			}
			else
			{
				SaveFile.Write(-1, SubIndex[1], true, "Hero_Pocket_Weapon_MinDamage", false, GenerateDataProgress);
				SaveFile.Write(-1, SubIndex[1], true, "Hero_Pocket_Weapon_MaxDamage", false, GenerateDataProgress);
				SaveFile.Write(-1, SubIndex[1], true, "Hero_Pocket_Weapon_CombatSkill", false, GenerateDataProgress);
			}
			if (Armor::getArmorElement(j, Hero->GetPocket(i)))
			{
				SaveFile.Write(Armor::getArmorElement(j, Hero->GetPocket(i))->GetRating(), SubIndex[1], true, "Hero_Pocket_Armor_rating", false, GenerateDataProgress);
				SaveFile.Write(Armor::getArmorElement(j, Hero->GetPocket(i))->GetArmorSkill(), SubIndex[1], true, "Hero_Pocket_Armor_ArmorSkill", false, GenerateDataProgress);
			}
			else
			{
				SaveFile.Write(-1, SubIndex[1], true, "Hero_Pocket_Armor_rating", false, GenerateDataProgress);
				SaveFile.Write(-1, SubIndex[1], true, "Hero_Pocket_Armor_ArmorSkill", false, GenerateDataProgress);
			}
		}
	}

	for (i = 0; i < Hero->GetMagicSchoolPack()->GetElementCount(); i++)
	{
		SubIndex[0].Cpy(i+1);
		SaveFile.Write(Hero->GetMagicSpellPack(i)->GetElementCount(), SubIndex[0], true, "TOTAL_CHAR_MAGIC_SPELL", false, GenerateDataProgress);
		for (j = 0; j < Hero->GetMagicSpellPack(i)->GetElementCount(); j++)
		{
			SubIndex[1] = SubIndex[0];
			SubIndex[1].Cat(j+1);
			SaveFile.Write(MagicSpell::getMagicSpellElement(j, Hero->GetMagicSpellPack(i))->GetSchool(), SubIndex[1], true, "Hero_MagicSpell_school", false, GenerateDataProgress);
			SaveFile.Write(MagicSpell::getMagicSpellElement(j, Hero->GetMagicSpellPack(i))->GetLevel(), SubIndex[1], true, "Hero_MagicSpell_level", false, GenerateDataProgress);
			SaveFile.Write(MagicSpell::getMagicSpellElement(j, Hero->GetMagicSpellPack(i))->GetEffect(), SubIndex[1], true, "Hero_MagicSpell_effect", false, GenerateDataProgress);
		}
	}

	SaveFile.Write(Hero->getPosition()->x, "Hero_Position_x", false, "", false, GenerateDataProgress);
	SaveFile.Write(Hero->getPosition()->y, "Hero_Position_y", false, "", false, GenerateDataProgress);
	SaveFile.Write(Map->GetScreenData()->Camera.getPosition()->x, "Map_ScreenData_Camera_Position_x", false, "", false, GenerateDataProgress);
	SaveFile.Write(Map->GetScreenData()->Camera.getPosition()->y, "Map_ScreenData_Camera_Position_y", false, "", false, GenerateDataProgress);
	SaveFile.Write(ZoneCoord.x, "ZoneCoord_x", false, "", false, GenerateDataProgress);
	SaveFile.Write(ZoneCoord.y, "ZoneCoord_y", false, "", false, GenerateDataProgress);

	SaveFile.Write(PBook->GetAcolytKill(), "IBook_AcolytKill", false, "", false, GenerateDataProgress);
	SaveFile.Write(PBook->GetCurseStatus(), "IBook_CurseStatus", false, "", false, GenerateDataProgress);
	SaveFile.Write(TOTAL_CHAR_PNJ_QUEST, "TOTAL_CHAR_PNJ_QUEST", false, "", false, GenerateDataProgress);
	for (i = 0; i < TOTAL_CHAR_PNJ_QUEST; i++)
	{
		SubIndex[0].Cpy(i+1);
		SaveFile.Write(PDialogue->GetStep(i), SubIndex[0], true, "IDialogue_step", false, GenerateDataProgress);
		SaveFile.Write(PBook->GetStatus(i), SubIndex[0], true, "IBook_status", false, GenerateDataProgress);
	}

	SaveFile.Write(TOTAL_ZONE_X, "TOTAL_ZONE_X", false, "", false, GenerateDataProgress);
	SaveFile.Write(TOTAL_ZONE_Y, "TOTAL_ZONE_Y", false, "", false, GenerateDataProgress);
	SaveFile.Write(CharPack.GetElementCount(), "CharPack_ElementCount", false, "", false, GenerateDataProgress);
	for (x = 0; x < TOTAL_ZONE_X; x++)
		for (y = 0; y < TOTAL_ZONE_Y; y++)
		{
			SubIndex[0].Cpy(x+1);
			SubIndex[0].Cat(y+1);
			SaveFile.Write(ZoneData[y][x].cursed, SubIndex[0], true, "ZoneData_cursed", false, GenerateDataProgress);
			for (i = 0; i < CharPack.GetElementCount(); i++)
				if (Mob::getMobElement(i, &CharPack))
				{
					SubIndex[1].Cpy(i+1);
					SubIndex[2] = SubIndex[0];
					SubIndex[2].Cat(1);
					SaveFile.Write(Mob::getMobElement(i, &CharPack)->GetZoneStatus(x, y)->dead, SubIndex[1], true, SubIndex[2], true, GenerateDataProgress);
					SubIndex[2] = SubIndex[0];
					SubIndex[2].Cat(2);
					SaveFile.Write(Mob::getMobElement(i, &CharPack)->GetZoneStatus(x, y)->spyed, SubIndex[1], true, SubIndex[2], true, GenerateDataProgress);
				}
		}

	SaveFile.Write(FlowerPack.GetElementCount(), "FlowerPack_ElementCount", false, "", false, GenerateDataProgress);
	for (i = 0; i < FlowerPack.GetElementCount(); i++)
	{
		SubIndex[0].Cpy(i+1);
		SaveFile.Write(Ingredient::getIngredientElement(i, &FlowerPack)->GetChemicalID(), SubIndex[0], true, "FlowerPack_Ingredient_ChemicalID", false, GenerateDataProgress);
	}

	SaveFile.Write(FruitPack.GetElementCount(), "FruitPack_ElementCount", false, "", false, GenerateDataProgress);
	for (i = 0; i < FruitPack.GetElementCount(); i++)
	{
		SubIndex[0].Cpy(i+1);
		SaveFile.Write(Ingredient::getIngredientElement(i, &FruitPack)->GetChemicalID(), SubIndex[0], true, "FruitPack_Ingredient_ChemicalID", false, GenerateDataProgress);
	}

	SaveFile.Write(DrinkPack.GetElementCount(), "DrinkPack_ElementCount", false, "", false, GenerateDataProgress);
	for (i = 0; i < DrinkPack.GetElementCount(); i++)
	{
		SubIndex[0].Cpy(i+1);
		SaveFile.Write(Ingredient::getIngredientElement(i, &DrinkPack)->GetChemicalID(), SubIndex[0], true, "DrinkPack_Ingredient_ChemicalID", false, GenerateDataProgress);
	}

	j = 0;
	SaveFile.Write(RezPointCount, "RezPointCount", false, "", false, GenerateDataProgress);
	for (i = 0; i < LandMap->GetElementCount(); i++)
		if (LandMap->GetItemMapElement(i)->GetType() != 0)
		{
			SubIndex[0].Cpy(j+1);
			SaveFile.Write(LandMap->GetItemMapElement(i)->getDimension()->x, SubIndex[0], true, "LandMap_ItemMap_Dimension_x", false, GenerateDataProgress);
			SaveFile.Write(LandMap->GetItemMapElement(i)->getDimension()->y, SubIndex[0], true, "LandMap_ItemMap_Dimension_y", false, GenerateDataProgress);
			SaveFile.Write(LandMap->GetItemMapElement(i)->GetType(), SubIndex[0], true, "LandMap_ItemMap_type", false, GenerateDataProgress);
			SaveFile.Write(LandMap->GetItemMapElement(i)->GetWorldPosition()->x, SubIndex[0], true, "LandMap_ItemMap_WorldPosition_x", false, GenerateDataProgress);
			SaveFile.Write(LandMap->GetItemMapElement(i)->GetWorldPosition()->y, SubIndex[0], true, "LandMap_ItemMap_WorldPosition_y", false, GenerateDataProgress);
			SaveFile.Write(LandMap->GetItemMapElement(i)->GetCoord()->x, SubIndex[0], true, "LandMap_ItemMap_Coord_x", false, GenerateDataProgress);
			SaveFile.Write(LandMap->GetItemMapElement(i)->GetCoord()->y, SubIndex[0], true, "LandMap_ItemMap_Coord_y", false, GenerateDataProgress);
			SaveFile.Write(LandMap->GetItemMapElement(i)->GetZoneID()->x, SubIndex[0], true, "LandMap_ItemMap_ZoneID_x", false, GenerateDataProgress);
			SaveFile.Write(LandMap->GetItemMapElement(i)->GetZoneID()->y, SubIndex[0], true, "LandMap_ItemMap_ZoneID_y", false, GenerateDataProgress);
			j++;
		}

	SaveFile.Write(TOTAL_BOOK_COMMENT, "TOTAL_BOOK_COMMENT", false, "", false, GenerateDataProgress);
	for (i = 0; i < TOTAL_BOOK_COMMENT; i++)
	{
		SubIndex[0].Cpy(i+1);
		if (PBook->GetComment(i) > 0)
		{
			SaveFile.Write(PBook->GetComment(i), SubIndex[0], true, "IBook_Comment_noted", false, GenerateDataProgress);
			j = PBook->SearchElementByName(*PBook->GetCommentPack()->GetElement(i)->getName(), false);
			if (j != -1)
				SaveFile.Write(QuestComment::getQuestCommentElement(j, PBook)->GetDay(), SubIndex[0], true, "IBook_QuestComment_day", false, GenerateDataProgress);
			else
				SaveFile.Write(0, SubIndex[0], true, "IBook_QuestComment_day", false, GenerateDataProgress);
		}
		else
		{
			SaveFile.Write(0, SubIndex[0], true, "IBook_Comment_noted", false, GenerateDataProgress);
			SaveFile.Write(0, SubIndex[0], true, "IBook_QuestComment_day", false, GenerateDataProgress);
		}
	}

	SaveFile.Close();

	if (SaveFile.ErrorOccured())
		EventsReporter.AddMessage(SAVE_DATA, 2);
	else
		EventsReporter.AddMessage(SAVE_DATA, 0);

	PData->hide();

	ExitThread(1);
}

void SetZoneTile(int layer)
{
	int x, y, i, ZoneCurrentTile;
	bool ok;

	ZoneTileCount = ZoneCurrentTile = 0;
	for (i = 0; i < TOTAL_LIVING_TILE_ID; i++)
		ZoneTile[i] = 0;

	//Liste toutes les tiles de la zone
	for (x = 0; x < Map->GetMapData()->TileCount.x; x++)
		for (y = 0; y < Map->GetMapData()->TileCount.y; y++)
		{
			ok = true;
			ZoneCurrentTile = Map->GetMapData()->TileID[y][x][layer];
			for (i = 0; i < ZoneTileCount; i++)
				if (ZoneCurrentTile == ZoneTile[i])
				{
					ok = false;
					break;
				}
			if (ok)
			{
				ZoneTile[ZoneTileCount] = ZoneCurrentTile;
				ZoneTileCount++;
			}
		}
}

void UpdateCurse(int x, int y, short value, Zone *pZone)
{
	int i;

	ZoneData[y][x].cursed = value;

	if ((pZone->CoordX == x) && (pZone->CoordY == y))
		for (i = 0; i < ZoneCharPack.GetElementCount(); i++)
			if (value)
				Character::getCharacterElement(i, &ZoneCharPack)->Curse(true, PWorldTime->GetChao());
			else
				Character::getCharacterElement(i, &ZoneCharPack)->Curse(false, PWorldTime->GetChao());
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	int i, x, y;
	DWORD ThreadCode;
	MSG msg;
	HWND hWndMain;
	HMODULE ResModule;

#ifndef _DEBUG
	CheckUpdate();
#endif

	CurrentMusic = 0;
	CurrentMusicCategory = NONE_MUSIC_CATEGORY;
	for (x = 0; x < TOTAL_ZONE_X; x++)
		for (y = 0; y < TOTAL_ZONE_Y; y++)
			RezPoint[y][x] = 0;
	reinit = EraseFile = false;
	CurrentZone = NULL;

	Clock::useRandom();

	hWndMain = InitWindow(hInst, nCmdShow);
	ResModule = LoadLibrary("RLRes.dll");

	if (!ResModule)
	{
		MessageBox(hWndMain, "Impossible de charger la librairie RLRes.dll", NULL, MB_ICONERROR);
		SendMessage(hWndMain, WM_QUIT, 0, NULL);
		return 0;
	}

	if (!Rules.Open(true, false, false))
	{
		MessageBox(hWndMain, "Impossible de charger la base des règles", NULL, MB_ICONERROR);
		SendMessage(hWndMain, WM_QUIT, 0, NULL);
		return 0;
	}

	Screen = new Display(hWndMain, ResModule);
	Keyboard = new Input(hWndMain, hInst);
	Mouse = new Input(hWndMain, hInst);

	//Les sons
	InitDirectMusic();

	if (!LoadConfig())
	{
		MessageBox(hWndMain, "Impossible de charger la configuration", NULL, MB_ICONERROR);
		SendMessage(hWndMain, WM_QUIT, 0, NULL);
		return 0;
	}

	Map = new TileEngine();
	cursor = new Cursor();
	TicTac = new Clock();

	// Les panels indépendants
	PMenu = new Menu(5, 64, 100);
	PData = new DataProgress(5, 2, 6);
#ifdef _DEBUG
	PTest = new Panel(9, 1, 5, 2, 6, false, Text(), NULL, NULL, Str());
#else
	PTest = NULL;
#endif

	// Les panels comprits dans la liste
	PGauge = new Gauge(5, 2, 6);
	PanelPack.AddElement(PGauge);
	PTargetGauge = new TargetGauge(5, 2, 6);
	PanelPack.AddElement(PTargetGauge);
	PCharInfo = new CharInfo(5, 2, 6);
	PanelPack.AddElement(PCharInfo);
	PBook = new QuestBook(5, 2, 6, 320, 256);
	PanelPack.AddElement(PBook);
	PDialogue = new Dialogue(5, 2, 6);
	PanelPack.AddElement(PDialogue);
	PInventory = new Inventory(5, 2, 6, true, "MON INVENTAIRE");
	PanelPack.AddElement(PInventory);
	PTargetInventory = new Inventory(5, 2, 6, false, "INVENTAIRE CIBLE");
	PanelPack.AddElement(PTargetInventory);
	PChemicalLab = new ChemicalLab(5, 2, 6);
	PanelPack.AddElement(PChemicalLab);
	PSpellList = new SpellList(false, true, NONE_CHAR_MAGIC_SCHOOL, false, TOTAL_CHAR_MAGIC_SPELL_EFFECT, 5, 2, 6, true, "SORTS");
	PanelPack.AddElement(PSpellList);
	PShortCut = new ShortCut(5, 2, 6);
	PanelPack.AddElement(PShortCut);
	PGreatMago = new MagicLearner(5, 2, 6);
	PanelPack.AddElement(PGreatMago);
	PPickup = new LookedPickup(5, 2, 6);
	PanelPack.AddElement(PPickup);
	PMagicSpell = new LookedSpell(5, 2, 6);
	PanelPack.AddElement(PMagicSpell);
	PEvents = new Events(5, 2, 6, 320, 256);
	PanelPack.AddElement(PEvents);
	PWorldTime = new WorldTime(5, 2, 6);
	PanelPack.AddElement(PWorldTime);
	PBattle = new BattleInfo(5, 2, 6);
	PanelPack.AddElement(PBattle);
	PSave = new AskForSave(5, 2, 6);
	PanelPack.AddElement(PSave);

	//La carte
	LandMap = new WorldMap(32*80, 32*80, 80, 80, NULL, NULL, 560, 480);

	//Les personnages
	Hero = new Player();
	CharPack.AddElement(Hero);
	for (i = 0; i < TOTAL_CHAR_ANIMAL_SPIDER; i++)
	{
		Spider[i] = new Animal(SPIDER, 1, 0, WEAK, false, true, true, NONE_CHAR_COMBAT_SKILL, BuildString("Araignée %d", i));
		Spider[i]->AddLivingTile(VILLAGE);
		CharPack.AddElement(Spider[i]);
	}
	for (i = 0; i < TOTAL_CHAR_ANIMAL_FLATSNAKE; i++)
	{
		FlatSnake[i] = new Animal(FLAT_SNAKE, 1, 5, WEAK, false, true, true, HAND_TO_HAND, BuildString("Serpent Plat %d", i));
		FlatSnake[i]->AddLivingTile(GRASS);
		FlatSnake[i]->AddLivingTile(MONTAIN);
		FlatSnake[i]->AddLivingTile(FOREST);
		FlatSnake[i]->SetGoSound(Sound::getSoundElement(SNAKE_GO, &SoundPack));
		CharPack.AddElement(FlatSnake[i]);
	}
	for (i = 0; i < TOTAL_CHAR_ANIMAL_PINKSNAKE; i++)
	{
		PinkSnake[i] = new Animal(PINK_SNAKE, 1, 5, MEDIUM, true, true, true, HAND_TO_HAND, BuildString("Serpent Rose %d", i));
		PinkSnake[i]->AddLivingTile(GRASS);
		PinkSnake[i]->AddLivingTile(MONTAIN);
		PinkSnake[i]->AddLivingTile(HIGH_MONTAIN);
		PinkSnake[i]->SetGoSound(Sound::getSoundElement(SNAKE_GO, &SoundPack));
		CharPack.AddElement(PinkSnake[i]);
	}
	for (i = 0; i < TOTAL_CHAR_ANIMAL_BLACKBEAR; i++)
	{
		BlackBear[i] = new Animal(BLACK_BEAR, 2, 10, STRONG, true, true, true, HAND_TO_HAND, BuildString("Ours noir %d", i));
		BlackBear[i]->AddLivingTile(MONTAIN);
		BlackBear[i]->AddLivingTile(FOREST);
		BlackBear[i]->AddLivingTile(SNOW);
		BlackBear[i]->SetGoSound(Sound::getSoundElement(BEAR_GO, &SoundPack));
		CharPack.AddElement(BlackBear[i]);
	}
	for (i = 0; i < TOTAL_CHAR_ANIMAL_MELEFIC_DOG; i++)
	{
		MaleficDog[i] = new Animal(MALEFIC_DOG, 3, 10, STRONG, true, true, false, HAND_TO_HAND, BuildString("Chien maléfique %d", i));
		MaleficDog[i]->SetGoSound(Sound::getSoundElement(DOG_GO, &SoundPack));
		CharPack.AddElement(MaleficDog[i]);
	}
	for (i = 0; i < TOTAL_CHAR_HUMANIZED_ROBERT; i++)
	{
		Robert[i] = new Creature(WEAK, THIEF, true, true, true, true, SPYING, STEALING, MACES, OTHERS, BuildString("Robert des Bois %d", i));
		Robert[i]->AddLivingTile(GRASS);
		Robert[i]->AddLivingTile(FOREST);
		Robert[i]->SetGoSound(Sound::getSoundElement(HUMAN_LAUGH, &SoundPack));
		CharPack.AddElement(Robert[i]);
	}
	for (i = 0; i < TOTAL_CHAR_HUMANIZED_REDFORD; i++)
	{
		Redford[i] = new Creature(MEDIUM, THIEF, true, true, true, true, SPYING, STEALING, SWORDS, WEAPONS, BuildString("Redford Malicieux %d", i));
		Redford[i]->AddLivingTile(GRASS);
		Redford[i]->AddLivingTile(MONTAIN);
		Redford[i]->AddLivingTile(SNOW);
		Redford[i]->SetGoSound(Sound::getSoundElement(HUMAN_LAUGH, &SoundPack));
		CharPack.AddElement(Redford[i]);
	}
	for (i = 0; i < TOTAL_CHAR_HUMANIZED_MINI_POUX; i++)
	{
		MiniPoux[i] = new Creature(MEDIUM, THIEF, true, true, true, true, SPYING, STEALING, MACES, OTHERS, BuildString("Mini Poux %d", i));
		MiniPoux[i]->AddLivingTile(RUINS);
		MiniPoux[i]->SetGoSound(Sound::getSoundElement(POUX_GO, &SoundPack));
		CharPack.AddElement(MiniPoux[i]);
	}
	for (i = 0; i < TOTAL_CHAR_HUMANIZED_POUX; i++)
	{
		Poux[i] = new Creature(STRONG, THIEF, true, true, true, true, MEDITATION, STEALING, DAGGERS, ARMORS, BuildString("Poux %d", i));
		Poux[i]->AddLivingTile(RUINS);
		Poux[i]->SetGoSound(Sound::getSoundElement(POUX_GO, &SoundPack));
		CharPack.AddElement(Poux[i]);
	}
	for (i = 0; i < TOTAL_CHAR_HUMANIZED_TROLL; i++)
	{
		Troll[i] = new Creature(STRONG, AGGRO, false, true, true, true, HEAVY_ARMOR, NONE_CHAR_SKILL, MACES, OTHERS, BuildString("Troll %d", i));
		Troll[i]->AddLivingTile(GRASS);
		Troll[i]->AddLivingTile(MONTAIN);
		Troll[i]->AddLivingTile(HIGH_MONTAIN);
		Troll[i]->AddLivingTile(SNOW);
		Troll[i]->SetGoSound(Sound::getSoundElement(TROLL_GO, &SoundPack));
		CharPack.AddElement(Troll[i]);
	}
	for (i = 0; i < TOTAL_CHAR_HUMANIZED_BAD_LOGGER; i++)
	{
		BadLogger[i] = new Creature(MEDIUM, AGGRO, true, true, true, true, LIGHT_ARMOR, MEDITATION, AXES, OTHERS, BuildString("Bûcheron Malsain %d", i));
		BadLogger[i]->AddLivingTile(FOREST);
		BadLogger[i]->SetGoSound(Sound::getSoundElement(LOGGER_GO, &SoundPack));
		CharPack.AddElement(BadLogger[i]);
	}
	for (i = 0; i < TOTAL_CHAR_HUMANIZED_MAD_LOGGER; i++)
	{
		MadLogger[i] = new Creature(STRONG, AGGRO, true, true, true, true, HEAVY_ARMOR, MEDITATION, AXES, OTHERS, BuildString("Bûcheron Fou Furieux %d", i));
		MadLogger[i]->AddLivingTile(FOREST);
		MadLogger[i]->SetGoSound(Sound::getSoundElement(LOGGER_GO, &SoundPack));
		CharPack.AddElement(MadLogger[i]);
	}
	for (i = 0; i < TOTAL_CHAR_HUMANIZED_MARSHPLANT; i++)
	{
		MarshPlant[i] = new Creature(STRONG, AGGRO, false, false, true, false, MEDITATION, NONE_CHAR_SKILL, HAND_TO_HAND, OTHERS, BuildString("Plante des Marais %d", i));
		MarshPlant[i]->AddLivingTile(MUD);
		CharPack.AddElement(MarshPlant[i]);
	}
	for (i = 0; i < TOTAL_CHAR_HUMANIZED_YUNG_ACOLYT; i++)
	{
		YungAcolyt[i] = new Creature(MEDIUM, AGGRO, false, true, true, true, MEDITATION, LIGHT_ARMOR, HAND_TO_HAND, OTHERS, BuildString("Jeune Acolyte %d", i));
		YungAcolyt[i]->AddLivingTile(MONTAIN);
		YungAcolyt[i]->AddLivingTile(HIGH_MONTAIN);
		YungAcolyt[i]->SetGoSound(Sound::getSoundElement(ACOLYT_GO, &SoundPack));
		CharPack.AddElement(YungAcolyt[i]);
	}
	for (i = 0; i < TOTAL_CHAR_HUMANIZED_OLD_ACOLYT; i++)
	{
		OldAcolyt[i] = new Creature(STRONG, AGGRO, false, true, true, true, MEDITATION, LIGHT_ARMOR, DAGGERS, OTHERS, BuildString("Vieil Acolyte %d", i));
		OldAcolyt[i]->AddLivingTile(HIGH_MONTAIN);
		OldAcolyt[i]->SetGoSound(Sound::getSoundElement(ACOLYT_GO, &SoundPack));
		CharPack.AddElement(OldAcolyt[i]);
	}
	for (i = 0; i < TOTAL_CHAR_HUMANIZED_LINK_ACOLYT; i++)
	{
		LinkAcolyt[i] = new Creature(STRONG, AGGRO, false, true, false, true, MEDITATION, LIGHT_ARMOR, DAGGERS, OTHERS, BuildString("Acolyte Relai %d", i));
		LinkAcolyt[i]->SetGoSound(Sound::getSoundElement(ACOLYT_GO, &SoundPack));
		LinkAcolytPack.AddElement(LinkAcolyt[i]);
		CharPack.AddElement(LinkAcolyt[i]);
	}
	for (i = 0; i < TOTAL_CHAR_PNJ_CITIZEN; i++)
	{
		Citizen[i] = new Pnj(PNJ2, NONE_CHAR_PNJ_JOB, NONE_CHAR_PNJ_QUEST, true, true, LIGHT_ARMOR, MEDITATION, HAND_TO_HAND, BuildString("Villageois %d", i));
		Citizen[i]->AddLivingTile(VILLAGE);
		Citizen[i]->AddLivingTile(GRASS);
		CharPack.AddElement(Citizen[i]);
	}
	Balrogue = new Creature(STRONG, AGGRO, true, true, false, false, HEAVY_ARMOR, MEDITATION, AXES, OTHERS, "Balrogue");
	Balrogue->SetGoSound(Sound::getSoundElement(TROLL_GO, &SoundPack));
	CharPack.AddElement(Balrogue);
	Wizard = new Creature(STRONG, AGGRO, false, true, false, false, MEDITATION, LIGHT_ARMOR, DAGGERS, OTHERS, "Sorcier");
	CharPack.AddElement(Wizard);
	Trader = new Pnj(PNJ1, TRADER, NONE_CHAR_PNJ_QUEST, false, true, LIGHT_ARMOR, MEDITATION, HAND_TO_HAND, "Commerçant");
	CharPack.AddElement(Trader);
	GreatMago = new Pnj(PNJ1, MAGIC_LEARNER, NONE_CHAR_PNJ_QUEST, false, false, MEDITATION, LIGHT_ARMOR, DAGGERS, "Grand mage");
	CharPack.AddElement(GreatMago);
	Captain = new Pnj(PNJ1, QUEST_GIVER, WARRIOR_QUEST, false, true, HEAVY_ARMOR, MEDITATION, SWORDS, "Capitaine");
	CharPack.AddElement(Captain);
	Wise = new Pnj(PNJ1, QUEST_GIVER, PRIEST_QUEST, false, true, MEDITATION, DISCRETION, HAND_TO_HAND, "Sage");
	CharPack.AddElement(Wise);
	Leader = new Pnj(PNJ1, QUEST_GIVER, FINAL_QUEST, false, true, LIGHT_ARMOR, MEDITATION, SWORDS, "Chef du village");
	CharPack.AddElement(Leader);
	Guard = new Pnj(PNJ1, QUEST_GIVER, GUARD_QUEST, false, true, HEAVY_ARMOR, MEDITATION, SWORDS, "Guarde");
	CharPack.AddElement(Guard);

	//Les pickups
	MoneyDefaultBag = new Pickup(10, DEFAULTBAG, NONE_CHAR_CLASS, 17, 17, "Sac d'or de base");
	MoneyDefaultBag->SetWeight(MoneyDefaultBag->GetPrice() * 0.01f);
	PickupPack.AddElement(MoneyDefaultBag);
	Authorization = new Pickup(0, 0.1f, false, AUTHORIZATION, MESSAGE, OTHERS, NONE_CHAR_CLASS, PickupMalus(), Enchant(), 7, 10, "Entrée VIP");
	PickupPack.AddElement(Authorization);
	MindProtector = new Pickup(1000000, 5, false, MINDPROTECTOR, JEWEL, OTHERS, NONE_CHAR_CLASS, PickupMalus(-2, -2, 0), Enchant(30, 50, MAGIC_ABSORBER), 20, 20, "Collier Protécteur d'Esprit");
	PickupPack.AddElement(MindProtector);
	MagicCircle = new Pickup(1000000, 3, false, MAGICCIRCLE, JEWEL, OTHERS, NONE_CHAR_CLASS, PickupMalus(0, 0, 0), Enchant(0, 0, SUMMON_NECROMISERY), 20, 20, "Anneau magique");
	PickupPack.AddElement(MagicCircle);
	for (i = 0; i < TOTAL_PICKUP_POTION_DF; i++)
	{
		DiscretionFlow[i] = new Potion(30, INVISIBILITY, 1000000, 1, DISCRETIONFLOW, NONE_CHAR_CLASS, 6, 15, BuildString("Flux de discrétion %d", i));
		PickupPack.AddElement(DiscretionFlow[i]);
	}
	Axel = new Weapon(8, 12, AXES, 1000000, 25, false, AXEL, NONE_CHAR_CLASS, PickupMalus(-6, -10, -5), Enchant(), 25, 40, "Axel la Vilaine");
	PickupPack.AddElement(Axel);
	WizardStaff = new Weapon(7, 10, MACES, 1000000, 8, false, WIZARDSTAFF, NONE_CHAR_CLASS, PickupMalus(-1, -2, 0), Enchant(5, 10, LIFE_MANA_GENERATOR), 5, 19, "Bâton du sorcier");
	PickupPack.AddElement(WizardStaff);
	Sylvanel = new Armor(9, IRON_ARMOR, HEAVY_ARMOR, 1000000, 45, false, SYLVANEL, NONE_CHAR_CLASS, PickupMalus(-8, -5, -7), Enchant(), 27, 21, "Sylvanel");
	PickupPack.AddElement(Sylvanel);
	for (i = 0; i < TOTAL_PICKUP_MONEY_POORBAG; i++)
	{
		MoneyPoorBag[i] = new Pickup(Clock::random(3, 10), NONE_PICKUP_UNIQUE_ID, WEAK, 17, 17, BuildString("Sac d'or pauvre %d", i));
		PickupPack.AddElement(MoneyPoorBag[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_MONEY_MEDIUMBAG; i++)
	{
		MoneyMediumBag[i] = new Pickup(Clock::random(10, 50), NONE_PICKUP_UNIQUE_ID, MEDIUM, 17, 17, BuildString("Sac d'or moyen %d", i));
		PickupPack.AddElement(MoneyMediumBag[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_MONEY_RICHBAG; i++)
	{
		MoneyRichBag[i] = new Pickup(Clock::random(50, 300), NONE_PICKUP_UNIQUE_ID, STRONG, 17, 17, BuildString("Sac d'or riche %d", i));
		PickupPack.AddElement(MoneyRichBag[i]);
	}
	for (i = 0; i < TOTAL_CHAR_PNJ; i++)
	{
		MoneyPNJ1Bag[i] = new Pickup(Clock::random(800, 1000), NONE_PICKUP_UNIQUE_ID, PNJ1, 17, 17, BuildString("Sac d'or PNJ %d", i));
		PickupPack.AddElement(MoneyPNJ1Bag[i]);
	}
	for (i = 0; i < TOTAL_CHAR_PNJ_CITIZEN; i++)
	{
		MoneyPNJ2Bag[i] = new Pickup(Clock::random(5, 10), NONE_PICKUP_UNIQUE_ID, PNJ2, 17, 17, BuildString("Sac d'or PNJ %d", i));
		PickupPack.AddElement(MoneyPNJ2Bag[i]);
		Pnj2LightArmor[i] = new Armor(CLOTHES, LIGHT_ARMOR, 30, 4, PNJ2, PickupMalus(), Enchant(), 27, 21, BuildString("Vêtements Pnj %d", i));
		PickupPack.AddElement(Pnj2LightArmor[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_FLOWER_WHITE; i++)
	{
		WhiteFlower[i] = new Ingredient(FLOWER_WHITE, 0, 0.3f, FLOWER, NONE_CHAR_CLASS, 32, 32, BuildString("Fleurs Blanches %d", i));
		FlowerPack.AddElement(WhiteFlower[i]);
		PickupPack.AddElement(WhiteFlower[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_FLOWER_RED; i++)
	{
		RedFlower[i] = new Ingredient(FLOWER_RED, 0, 0.3f, FLOWER, NONE_CHAR_CLASS, 32, 32, BuildString("Fleurs Rouges %d", i));
		FlowerPack.AddElement(RedFlower[i]);
		PickupPack.AddElement(RedFlower[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_FLOWER_BLUE; i++)
	{
		BlueFlower[i] = new Ingredient(FLOWER_BLUE, 0, 0.3f, FLOWER, NONE_CHAR_CLASS, 32, 32, BuildString("Fleurs Bleues %d", i));
		FlowerPack.AddElement(BlueFlower[i]);
		PickupPack.AddElement(BlueFlower[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_FRUIT_RED; i++)
	{
		RedFruit[i] = new Ingredient(FRUIT_RED, 0, 0.3f, FRUIT, NONE_CHAR_CLASS, 22, 22, BuildString("Fruits Rouges %d", i));
		FruitPack.AddElement(RedFruit[i]);
		PickupPack.AddElement(RedFruit[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_FRUIT_PINK; i++)
	{
		PinkFruit[i] = new Ingredient(FRUIT_PINK, 0, 0.3f, FRUIT, NONE_CHAR_CLASS, 22, 22, BuildString("Fruits Roses %d", i));
		FruitPack.AddElement(PinkFruit[i]);
		PickupPack.AddElement(PinkFruit[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_FRUIT_BLUE; i++)
	{
		BlueFruit[i] = new Ingredient(FRUIT_BLUE, 0, 0.3f, FRUIT, NONE_CHAR_CLASS, 22, 22, BuildString("Fruits Bleus %d", i));
		FruitPack.AddElement(BlueFruit[i]);
		PickupPack.AddElement(BlueFruit[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_FRUIT_BLACK; i++)
	{
		BlackFruit[i] = new Ingredient(FRUIT_BLACK, 0, 0.3f, FRUIT, NONE_CHAR_CLASS, 22, 22, BuildString("Fruits Noirs %d", i));
		FruitPack.AddElement(BlackFruit[i]);
		PickupPack.AddElement(BlackFruit[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_FRUIT_ORANGE; i++)
	{
		OrangeFruit[i] = new Ingredient(FRUIT_ORANGE, 0, 0.3f, FRUIT, NONE_CHAR_CLASS, 22, 22, BuildString("Fruits Oranges %d", i));
		FruitPack.AddElement(OrangeFruit[i]);
		PickupPack.AddElement(OrangeFruit[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_DRINK_WATER; i++)
	{
		WaterBottle[i] = new Ingredient(DRINK_WATER, 8, 0.5f, DRINK, STRONG, 16, 20, BuildString("Bouteille d'eau %d", i));
		DrinkPack.AddElement(WaterBottle[i]);
		PickupPack.AddElement(WaterBottle[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_DRINK_WINE; i++)
	{
		WineBottle[i] = new Ingredient(DRINK_WINE, 4, 0.5f, DRINK, MEDIUM, 16, 20, BuildString("Bouteille de vin %d", i));
		DrinkPack.AddElement(WineBottle[i]);
		PickupPack.AddElement(WineBottle[i]);
	}
	for (i = 0; i < TOTAL_CHAR_HUMANIZED_TROLL; i++)
	{
		TrollFluid[i] = new Ingredient(DRINK_TROLLFLUID, 16, 0.5f, DRINK, NONE_CHAR_CLASS, 16, 20, BuildString("Urine de Troll %d", i));
		DrinkPack.AddElement(TrollFluid[i]);
		PickupPack.AddElement(TrollFluid[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_POTION_LIFE; i++)
	{
		LifePotion[i] = new Potion(10, LIFE_REGEN, 20, 1, NONE_PICKUP_UNIQUE_ID, STRONG, 6, 15, BuildString("Potion de vie %d", i));
		PickupPack.AddElement(LifePotion[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_POTION_MANA; i++)
	{
		ManaPotion[i] = new Potion(10, MANA_REGEN, 20, 1, NONE_PICKUP_UNIQUE_ID, STRONG, 6, 15, BuildString("Potion de mana %d", i));
		PickupPack.AddElement(ManaPotion[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_POTION_CHEAP; i++)
	{
		CheapPotion[i] = new Potion(0, MANA_TRANS_LIFE, 5, 1, NONE_PICKUP_UNIQUE_ID, MEDIUM, 6, 15, BuildString("Potion bon marché %d", i));
		PickupPack.AddElement(CheapPotion[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_WEAPON_WK_DAGGER; i++)
	{
		WkDagger[i] = new Weapon(DAGGERS, 15, 2, WEAK, PickupMalus(0, 0, 0), Enchant(), 4, 14, BuildString("Dague %d", i));
		PickupPack.AddElement(WkDagger[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_WEAPON_ST_DAGGER; i++)
	{
		StDagger[i] = new Weapon(DAGGERS, 200, 2, STRONG, PickupMalus(0, 0, 0), Enchant(), 4, 14, BuildString("Dague de Glace %d", i));
		PickupPack.AddElement(StDagger[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_WEAPON_MD_SWORD; i++)
	{
		MdSword[i] = new Weapon(SWORDS, 30, 5, MEDIUM, PickupMalus(-1, -2, -3), Enchant(), 7, 21, BuildString("Epée de Percée %d", i));
		PickupPack.AddElement(MdSword[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_WEAPON_ST_SWORD; i++)
	{
		StSword[i] = new Weapon(SWORDS, 80, 8, STRONG, PickupMalus(-2, -4, -6), Enchant(), 7, 21, BuildString("Epée Détripeuse %d", i));
		PickupPack.AddElement(StSword[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_WEAPON_PNJ_SWORD; i++)
	{
		PnjSword[i] = new Weapon(SWORDS, 80, 8, PNJ1, PickupMalus(-2, -4, -6), Enchant(), 7, 21, BuildString("Epée Pnj %d", i));
		PickupPack.AddElement(PnjSword[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_WEAPON_WK_MACE; i++)
	{
		WkMace[i] = new Weapon(MACES, 10, 4, WEAK, PickupMalus(0, -2, 0), Enchant(), 5, 19, BuildString("Blâmeur %d", i));
		PickupPack.AddElement(WkMace[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_WEAPON_MD_MACE; i++)
	{
		MdMace[i] = new Weapon(MACES, 20, 6, MEDIUM, PickupMalus(-1, -3, -3), Enchant(), 7, 23, BuildString("Gourdin %d", i));
		PickupPack.AddElement(MdMace[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_WEAPON_ST_MACE; i++)
	{
		StMace[i] = new Weapon(MACES, 50, 10, STRONG, PickupMalus(-2, -4, -6), Enchant(), 10, 25, BuildString("Gourdin à pics %d", i));
		PickupPack.AddElement(StMace[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_WEAPON_MD_AXE; i++)
	{
		MdAxe[i] = new Weapon(AXES, 25, 5, MEDIUM, PickupMalus(-1, -2, -3), Enchant(), 17, 23, BuildString("Hache Bûcheronne %d", i));
		PickupPack.AddElement(MdAxe[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_WEAPON_ST_AXE; i++)
	{
		StAxe[i] = new Weapon(AXES, 70, 10, STRONG, PickupMalus(-2, -4, -8), Enchant(), 25, 40, BuildString("Hache Double-tranchant %d", i));
		PickupPack.AddElement(StAxe[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_WEAPON_DAGGY; i++)
	{
		Daggy[i] = new Weapon(6, 10, DAGGERS, 1000000, 1, false, (PICKUP_UNIQUE_ID)(PRIESTDAGGY+i), PNJ1, PickupMalus(), Enchant(3, 3, (PICKUP_ENCHANT)(i+1)), 4, 14, BuildString("Daggy %d", (i+1)));
		PickupPack.AddElement(Daggy[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_ARMOR_WK_LIGHT; i++)
	{
		WkLightArmor[i] = new Armor(CLOTHES, LIGHT_ARMOR, 30, 4, WEAK, PickupMalus(), Enchant(), 27, 21, BuildString("Vêtements en tissu %d", i));
		PickupPack.AddElement(WkLightArmor[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_ARMOR_MD_LIGHT; i++)
	{
		MdLightArmor[i] = new Armor(LEATHER_CLOTHES, LIGHT_ARMOR, 90, 3, MEDIUM, PickupMalus(), Enchant(), 27, 21, BuildString("Combinaison de cuir %d", i));
		PickupPack.AddElement(MdLightArmor[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_ARMOR_ST_LIGHT; i++)
	{
		StLightArmor[i] = new Armor(MESH_COAT, LIGHT_ARMOR, 200, 2, STRONG, PickupMalus(), Enchant(), 27, 21, BuildString("Cote de maille %d", i));
		PickupPack.AddElement(StLightArmor[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_ARMOR_PNJ1_LIGHT; i++)
	{
		Pnj1LightArmor[i] = new Armor(MESH_COAT, LIGHT_ARMOR, 200, 2, PNJ1, PickupMalus(), Enchant(), 27, 21, BuildString("Cote de maille Pnj %d", i));
		PickupPack.AddElement(Pnj1LightArmor[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_ARMOR_MAGICIAN_DRESS; i++)
	{
		MagicianDress[i] = new Armor(6, WHITE_DRESS+i, LIGHT_ARMOR, 1000000, 3, false, (PICKUP_UNIQUE_ID)(PRIESTDRESS+i), PNJ1, PickupMalus(0, -2, +2), Enchant(), 27, 21, BuildString("Robe de mage %d", i));
		PickupPack.AddElement(MagicianDress[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_ARMOR_WK_HEAVY; i++)
	{
		WkHeavyArmor[i] = new Armor(PADS_ARMOR, HEAVY_ARMOR, 50, 6, WEAK, PickupMalus(-3, -3, -5), Enchant(), 27, 21, BuildString("Pades %d", i));
		PickupPack.AddElement(WkHeavyArmor[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_ARMOR_MD_HEAVY; i++)
	{
		MdHeavyArmor[i] = new Armor(IRON_ARMOR, HEAVY_ARMOR, 150, 10, MEDIUM, PickupMalus(-4, -4, -10), Enchant(), 27, 21, BuildString("Armure de fer %d", i));
		PickupPack.AddElement(MdHeavyArmor[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_ARMOR_ST_HEAVY; i++)
	{
		StHeavyArmor[i] = new Armor(IRON_ARMOR, HEAVY_ARMOR, 500, 15, STRONG, PickupMalus(-4, -5, -15), Enchant(), 27, 21, BuildString("Armure d'acier %d", i));
		PickupPack.AddElement(StHeavyArmor[i]);
	}
	for (i = 0; i < TOTAL_PICKUP_ARMOR_PNJ_HEAVY; i++)
	{
		PnjHeavyArmor[i] = new Armor(IRON_ARMOR, HEAVY_ARMOR, 500, 15, PNJ1, PickupMalus(-4, -5, -15), Enchant(), 27, 21, BuildString("Armure d'acier Pnj %d", i));
		PickupPack.AddElement(PnjHeavyArmor[i]);
	}

	ApplyConfig();
	Screen->SetDisplayMode();
	InitMessages();
	InitObjects();
	InitPanels();
	InitDirectDraw();
	InitDirectInput();
	InitMagicSpellPack();
	InitORC();
	InitCombat();

	LandMap->AddItemMap(new ItemMap(0, 16, 16));

	PMenu->show();
	PMenu->Refresh();
	PGauge->show();
	PGauge->Refresh();
	PWorldTime->show();
	PWorldTime->Refresh();
	PBattle->hide();

	Rules.Close();

	while (true)
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if(!GetMessage(&msg, NULL, 0, 0 ))
				return (int)msg.wParam;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (running)
			{
				if (reinit)
				{
					reinit = false;
					DestroyDirectDraw();
					DestroyDirectInput();
					RestoreDirectDraw();
					InitDirectInput();
				}
				else
				{
					// Affichage du menu
					if (PMenu->getVisible())
					{
						if (!PMenu->generate(Hero->GetSwatch(BASIC)->isGoing() == false, cursor))
							return 0;
						PMenu->Run();
					}
					else
						PlayScene(CurrentZone);
					// Affichage de la progression des données
					if (PData->getVisible())
					{
						PData->Run(cursor);
						PData->generate();
					}
					// Affichage du curseur
					if (!cursor->getRemoved())
					{
						cursor->setVisibleZone(NULL, cursor->getPosition()->x + Animation::getAnimationElement(0, cursor->getAnimations())->GetSprite()->GetWidth(), NULL, cursor->getPosition()->y + Animation::getAnimationElement(0, cursor->getAnimations())->GetSprite()->GetHeight());
						cursor->runAni(0, 0);
					}
					Screen->FlipFlap();
					ProcessMouseInput();
					ProcessKbInput();
					if ((PData->GetThread() != NULL) && (GetExitCodeThread(PData->GetThread(), &ThreadCode) == TRUE) && (ThreadCode != STILL_ACTIVE))
					{
						switch (ThreadCode)
						{
						case 1:
							break;
						case 2:
							break;
						case -1:
							MessageBox(hWndMain, "Impossible de sauver les données", NULL, MB_ICONERROR);
							*cursor->getPosition() = Axe(0, 0);
							break;
						case -2:
							MessageBox(hWndMain, "Impossible de charger les données", NULL, MB_ICONERROR);
							PMenu->show();
							PMenu->Refresh();
							*cursor->getPosition() = Axe(0, 0);
							break;
						}
						PData->StopThread();
					}
				}
			}
		}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int i, x, y;

	switch (msg)
	{
	case WM_QUIT:
		if (wParam == 1)
			SavePanels(&PanelPack);
		if (Screen && Screen->GetResInst())
			FreeLibrary(Screen->GetResInst());
		if (wParam == 1)
			DestroyAll();
		PostQuitMessage(0); //Ferme la fenêtre
		return 0;
	case WM_SETCURSOR:
		SetCursor(NULL); //Définit la valeur du curseur
		return 0;
	case WM_ACTIVATE:
		//Switché sur une autre application
		if (LOWORD(wParam) == WA_INACTIVE || (BOOL)HIWORD(wParam))
		{
			running = false;
			reinit = true;
		}
		else
			running = true;
		return 0;
	case WM_NULL:
		CurrentZone = GetCurrentZone();
		if (!CurrentZone)
		{
			running = false;
			MessageBox(Screen->GetDxWnd(), "Erreur lors du chargement de la zone", NULL, MB_ICONERROR);
			SendMessage(Screen->GetDxWnd(), WM_QUIT, 1, NULL);
			return 0;
		}
		for (i = 0; i < BattlePack.GetElementCount(); i++)
		{
			BattlePack.GetCombatSystemElement(i)->End(&EventsReporter);
			BattlePack.RemoveAllElement(0, true);
		}
		ZoneCharPack.Initialize();
		ZonePickupPack.Initialize();
		if (wParam == 1)
			InitZone(CurrentZone, true);
		else
			InitZone(CurrentZone, false);
		SetZoneTile(GROUND);
		DispatchCreature(CurrentZone);
		InitCreatureOnZone();
		DispatchPickupsOnZone();
		DispatchPickupsOnChar();
		DispatchTraderPickups(CurrentZone);
		GivePickupToChar();
		for (i = 0; i < ZoneCharPack.GetElementCount(); i++)
		{
			if (Creature::getCreatureElement(i, &ZoneCharPack))
				Creature::getCreatureElement(i, &ZoneCharPack)->CheckEquipment(NULL);
			if (Pnj::getPnjElement(i, &ZoneCharPack))
				Pnj::getPnjElement(i, &ZoneCharPack)->CheckEquipment(NULL);
		}
		for (i = 0; i < SoundPack.GetElementCount(); i++)
			Sound::getSoundElement(i, &SoundPack)->stop();
		if (CurrentZone->category != CurrentMusicCategory)
		{
			if ((CurrentMusicCategory >= 0) && (MusicRandomPack[CurrentMusicCategory].ElementExist(CurrentMusic) == true))
			{
				Sound::stop(DMUS_SEGF_DEFAULT);
				MusicRandomPack[CurrentMusicCategory].RemoveElement(CurrentMusic, 1);
			}
			CurrentMusicCategory = CurrentZone->category;
			PlayMusic();
		}
		if (wParam == 1)
		{
			Hero->SetMapPositionByCoord(false);
			Hero->FocusWithCamera(Screen->GetCurrentMode()->GetWidth() / 2, Screen->GetCurrentMode()->GetHeight() / 2);
		}
		AddZoneChar(Hero, CurrentZone);
		AddRezPoint(ZoneCoord.x, ZoneCoord.y);
		PMenu->hide();
		return 0;
	}

	if (msg == RegisterWindowMessage("GabGamesConfig"))
	{
		if (wParam == 1 && LoadConfig())
		{
			ApplyConfig();
			for (x = 0; x < Map->GetMapData()->TileCount.x; x++)
				for (y = 0; y < Map->GetMapData()->TileCount.y; y++)
					Hero->GenerateTileCoord(x, y);
			*Map->GetScreenData()->Camera.getPosition() = Axe();
			Hero->SetMapPositionByCoord(false);
			Hero->FocusWithCamera(Screen->GetCurrentMode()->GetWidth() / 2, Screen->GetCurrentMode()->GetHeight() / 2);
		}
		ShowWindow(hWnd, SW_RESTORE);
		return 0;
	}
	
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
