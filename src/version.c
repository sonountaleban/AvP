#include "version.h"

extern void NewOnScreenMessage(unsigned char *messagePtr);

#ifdef __AROS__
const char *AvPVersionString = "Aliens vs Predator AROS \n     v1.0 \n     Based on Rebellion Developments AvP Gold source \n";
#else
const char *AvPVersionString = "Aliens vs Predator Linux \n     Build 000 (CVS) \n     Based on Rebellion Developments AvP Gold source \n";
#endif // __AROS__

void GiveVersionDetails(void)
{
	NewOnScreenMessage((unsigned char *)AvPVersionString);
}
