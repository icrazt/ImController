#pragma once                            
struct AutoRC_DataStruct {
	// Control Parameters
	uint32_t command;
	float	 setspeed[2];
	float	 bearing;
	// Status Parameters
	float systick;
	float motorspeed[2];
};
extern AutoRC_DataStruct AutoCurrentStatus, AutoRC_TxData;

void AutoRC_DataPlot(void);

