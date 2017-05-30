#ifndef CWORLDTIME
#define CWORLDTIME

#include <cpanel.h>
#include <cenergybar.h>

class WorldTime : public Panel
{
private:
	int day;
	short MeditateProgress, chao;
	bool NewDay;
	EnergyBar *TodayBar, *ChaoBar;
public:
	WorldTime();
	WorldTime(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace);
	~WorldTime();
	void SetDay(int WT_day) {day = WT_day;}
	void SetMeditateProgress(short WT_MeditateProgress) {MeditateProgress = WT_MeditateProgress;}
	void SetChao(short WT_chao) {chao = WT_chao;}
	void SetNewDay(bool WT_NewDay) {NewDay = WT_NewDay;}
	void Reset();
	void Init();
	void initAni();
	void restoreAni();
	void destroyAni();
	void GoNewDay();
	bool Refresh();
	bool generate(bool waited=false, Cursor *cursor=NULL);
	int GetDay() {return day;}
	short * GetMeditateProgress() {return &MeditateProgress;}
	short GetChao() {return chao;}
	bool GetNewDay() {return NewDay;}
	EnergyBar * GetChaoBar() {return ChaoBar;}
	EnergyBar * GetTodayBar() {return TodayBar;}
};

#endif