#ifndef CCHEMICALLAB
#define CCHEMICALLAB

#include <cpanel.h>
#include "cplayer.h"

class ChemicalLab : public Panel
{
private:
	Player *Chemist;
public:
	ChemicalLab() {Chemist = NULL;}
	ChemicalLab(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace);
	~ChemicalLab() {;}
	void SetChemist(Player *CL_Chemist) {Chemist = CL_Chemist;}
	void Init();
	void initAni();
	void restoreAni();
	void destroyAni();
	bool Refresh();
	bool generate(bool waited=false, Cursor *cursor=NULL);
};


#endif