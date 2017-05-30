#ifndef CDATAPROGRESS
#define CDATAPROGRESS

#include <cpanel.h>
#include <cenergybar.h>

class DataProgress : public Panel
{
private:
	HANDLE Thread;
	EnergyBar *DataBar;
public:
	DataProgress();
	DataProgress(int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace);
	~DataProgress() {delete DataBar;}
	void Init();
	void initAni();
	void restoreAni();
	void destroyAni();
	bool Refresh();
	bool CreateLoadThread();
	bool CreateSaveThread();
	void StopThread();
	HANDLE GetThread() {return Thread;}
	EnergyBar * GetDataBar() {return DataBar;}
};

#endif