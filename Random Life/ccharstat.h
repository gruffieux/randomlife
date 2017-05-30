#ifndef CCHARSTAT
#define CCHARSTAT

#include <cobject.h>
#include <clist.h>

class CharStat : public Object
{
private:
	int StatId, score, TempScore, modifier;
	Str Description, Label;
public:
	CharStat() {StatId = score = TempScore = modifier = 0;}
	CharStat(CharStat &model);
	~CharStat() {}
	void SetStatId(int CharStat_StatId) {StatId = CharStat_StatId;}
	void SetScore(int CharStat_score) {score = CharStat_score;}
	void SetTempScore(int CharStat_TempScore) {TempScore = CharStat_TempScore;}
	void SetModifier() {modifier = score - 7;}
	void SetDescription(Str CharStat_Description) {Description = CharStat_Description;}
	void SetLabel(Str CharStat_Label) {Label = CharStat_Label;}
	void IncreaseMalus(int amount) {modifier += amount;}
	void DecreaseMalus(int amount) {modifier -= amount;}
	int GetStatId() {return StatId;}
	int GetScore() {return score;}
	int GetTempScore() {return TempScore;}
	int* GetpScore() {return &score;}
	int* GetpTempScore() {return &TempScore;}
	int GetModifier() {return modifier;}
	Str GetDescription() {return Description;}
	Str GetLabel() {return Label;}
	static CharStat* getCharStatElement(int index, List *pList) {return dynamic_cast<CharStat*>(pList->GetElement(index));}
};


#endif