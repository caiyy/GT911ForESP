# GT911ForESP

GT911在ESP系列上的驱动，基于IDF 6.0，ESP32S3编写

本库使用面向对象思想编写，可创建多设备多实例

Github，Gitee同步更新，Gitee仅作为下载仓库，提交Issue和Pull request请到Github

[Github: https://github.com/caiyy/GT911ForESP](https://github.com/caiyy/GT911ForESP)

## 1. ESP-IDF 版本支持

| 版本 | 状态 |
|------|------|
| IDF 6.0 | ✅ 已适配（使用新 I2C Master API） |
| IDF 5.x | 理论可用，未经测试 |
| IDF 4.4 | 理论可用，未经实践 |

## 2. 注意点

> 以下内容基于编程开发手册REV11.0

### 1. 0x8040寄存器貌似不再支持软重启

> 如果按照旧手册使用软重启，读取0x814E会一直返回128(0x80)，即使对寄存器写入0。也是坑，如果你买的设备使用的芯片出厂较早，那么网上一些驱动可用。但是我是新芯片，没错又浪费时间了。。。

### 2. 0x814E状态寄存器必须在读取完或开机之后写入0再读取

> 这个挺坑的，我就被坑了。。一开始找到的是旧的编程文档，并没有说明这一项。。时间被白白浪费了。。。

## 3. 目录结构

```
├─components
│  └─GT911
│      │  CMakeLists.txt
│      │  README.md
│      │
│      └─Vernon_GT911
│          │  Vernon_GT911.c    驱动文件
│          │
│          └─includes
│                  Vernon_GT911.h
│
└─examples
        GT911ForESP.c           示例文件
```

## 4. 快速使用

### 1. 添加到项目

将本仓库克隆到你的项目的 `components` 目录下：

```bash
git clone https://github.com/caiyy/GT911ForESP.git components/GT911
```

### 2. 在 CMakeLists.txt 中添加依赖

```cmake
idf_component_register(
    ...
    REQUIRES GT911
    ...
)
```

### 3. 使用示例

```c
#include "Vernon_GT911.h"

static Vernon_GT911 gt911;

void app_main(void)
{
    // 初始化 GT911
    GT911_init(&gt911, TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST,
               I2C_NUM_0, GT911_ADDR1, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    // 设置旋转方向
    GT911_setRotation(&gt911, ROTATION_NORMAL);
    
    while (1) {
        if (GT911_touched(&gt911)) {
            uint16_t x, y;
            GT911_read_pos(&gt911, &x, &y, 0);
            printf("触摸位置: x=%d, y=%d\n", x, y);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
```

## 5. API 说明

### 初始化函数

```c
void GT911_init(Vernon_GT911 * VernonGt911, int8_t SDA, int8_t SCL, int8_t INT, int8_t RES,
                i2c_port_t i2c_num, uint8_t gt911_addr, uint16_t width, uint16_t height);
```

### 设置旋转方向

```c
void GT911_setRotation(Vernon_GT911 * VernonGt911, uint8_t rot);
```

旋转方向定义：
- `ROTATION_NORMAL` - 正常方向
- `ROTATION_LEFT` - 左旋转90度
- `ROTATION_RIGHT` - 右旋转90度
- `ROTATION_INVERTED` - 180度翻转

### 检测触摸

```c
bool GT911_touched(Vernon_GT911 * VernonGt911);
```

返回 `true` 表示有触摸，`false` 表示无触摸。

### 读取坐标

```c
void GT911_read_pos(Vernon_GT911 * VernonGt911, uint16_t *x, uint16_t *y, uint8_t index);
```

`index` 为触控点索引（0-4），GT911 支持最多5点触控。

## 6. IDF 6.0 适配说明

本库已适配 ESP-IDF 6.0，使用新的 I2C Master API：

- 使用 `i2c_master_bus_config_t` 和 `i2c_new_master_bus()` 配置 I2C 总线
- 使用 `i2c_device_config_t` 和 `i2c_master_bus_add_device()` 添加设备
- 使用 `i2c_master_transmit()` 和 `i2c_master_transmit_receive()` 进行数据传输

## 7. 参考驱动

[https://github.com/TAMCTec/gt911-arduino](https://github.com/TAMCTec/gt911-arduino)

[https://gitee.com/andida/esp32s3-gt911](https://gitee.com/andida/esp32s3-gt911) 

> 此库在我的芯片不可用，修改后可用，已在库中提交Pull Request
