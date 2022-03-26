#include "my_wwdg.h"

#include "my_gpio.h"


void WWDG_Feed()
{
    HAL_WWDG_Refresh(&hwwdg);
}


void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *wwdg)
{
    static uint8_t sta;
    HAL_WWDG_Refresh(wwdg);
    //Çå¿Õ×´Ì¬¼Ä´æÆ÷EWIFÎ»(Èí¼þÐ´Èë0)
    __HAL_WWDG_CLEAR_FLAG(wwdg, WWDG_FLAG_EWIF);
    sta = ~sta;
    CTRL_GPIO(LED_GREE_GPIO_Port, LED_GREE_Pin, (GPIO_PinState)sta);
}


