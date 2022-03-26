#ifndef __MY_WWDG_H__
#define __MY_WWDG_H__
#include "wwdg.h"
#include "stm32f1xx_hal_wwdg.h"







void WWDG_Feed(void);
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *wwdg);



#endif

