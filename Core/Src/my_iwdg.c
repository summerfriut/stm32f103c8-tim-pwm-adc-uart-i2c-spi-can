#include "my_iwdg.h"


void IWDG_Feed()
{
    HAL_IWDG_Refresh(&hiwdg);
}









