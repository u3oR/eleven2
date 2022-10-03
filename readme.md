# 2048 Game using lvgl (no animation)

<p align = "center">
	<img alt="ScrcpyWidget" src="image/2048.png">
<br><br>
Why is it called eleven2 ?  -Because 2^11=2048   ;-)

## 概述

![image-20221003204237256](image/readme/image-20221003204237256.png)

## 看板

- [ ] 🔵 支持动画显示
- [ ] 🔵 加入回退上一步功能
- [ ] 🔵 支持横竖屏布局切换 ()
- [ ] 🟡 加入得分显示
- [ ] 🟡 优化新添数字的功能
- [ ] 🔴 实现基本移动逻辑 
- [x] 🟢 实现UI界面 

## 如何使用？(Visual Studio)

下载lvgl模拟器

[lvgl/lv_port_win_visual_studio: LVGL Windows Simulator Visual Studio Edition (github.com)](https://github.com/lvgl/lv_port_win_visual_studio)

进入工程，编辑文件`LVGL.Simulator.cpp` 

包含头文件

```c
 #include "lv_app_eleven2.h"
```

设置分辨率，改为480*272

```c
if (!lv_win32_init(
        GetModuleHandleW(NULL),
        SW_SHOW,
        480,/*横向分辨率*/
        272,/*纵向分辨率*/
        LoadIconW(GetModuleHandleW(NULL), MAKEINTRESOURCE(IDI_LVGL))))
    {
        return -1;
    }
```

注释掉默认的demo

```c
lv_demo_widgets();// ok
```

调用函数

```c
lv_app_eleven2();
```

 

