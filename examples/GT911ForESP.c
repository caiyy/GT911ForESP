#include <stdio.h>
#include "Vernon_GT911.h"

#define TOUCH_GT911_SDA 41
#define TOUCH_GT911_SCL 42
#define TOUCH_GT911_INT -1
#define TOUCH_GT911_RES -1
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 480

static Vernon_GT911 gt911;

void GT911_test(void * pvPara)
{
    uint16_t x, y;
    while(1){
        if(GT911_touched(&gt911)){
            // 循环读取每个触控点的位置值
            for (int i = 0; i < 5; ++i) {
                GT911_read_pos(&gt911, &x, &y, i);
                printf("No: %d, touched x: %d, touched y: %d\n", i, x, y);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main(void)
{
    // 初始化 GT911
    GT911_init(&gt911, TOUCH_GT911_SDA, TOUCH_GT911_SCL, TOUCH_GT911_INT,
               TOUCH_GT911_RES, I2C_NUM_0, GT911_ADDR1,
               SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // 初始化之后必须设置旋转方向
    GT911_setRotation(&gt911, ROTATION_NORMAL);

    xTaskCreate(GT911_test, "GT911_test", 1024 * 3,
                NULL, 12, NULL);
}
