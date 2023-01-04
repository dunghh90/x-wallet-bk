#include "rrunitlib.h"
#include "zl303xx_ExampleMain.h"
#include "zl303xx_DataTypes.h"
#include "zl303xx_PtpDatatypes.h"
#define DPRINT(...) f_rrulib_dmsg("PF", __func__ ,__VA_ARGS__ )

zl303xx_ParamsS *zl303xx_Params0;
Uint8T TARGET_DPLL = 1;
int accepted_clock_classes = 0;
zl303xx_PortAddress examplePtpDestAddr[2] = {{ZL303XX_NETWORK_PROTOCOL_UNKNOWN, 0, {0}}, {ZL303XX_NETWORK_PROTOCOL_UNKNOWN, 0, {0}}};
exampleAppS zlExampleApp;
zl303xx_ClockStreamS *Zl303xx_PtpStreamArray[128] = {NULL};
int keep_announce_clockclass = 0;

zlStatusE zl303xx_GetAprDeviceStatus(zl303xx_ParamsS * Params0){
	return 0;
}

zlStatusE getPtpStatus(void){
	return 0;
}

void getPtpParameter(void){}
