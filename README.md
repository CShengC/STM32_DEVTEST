# 说明

这里给出一套关于嵌入式STM32的一套模版工程流程，区别于常用的ide工具如keil，IAR。新鲜的工具，但采用古老的配方。基本工程配置使用STM32cubeMX工具，构建工具使用Cmake，Najia，编译工具使用arm-none-eabi这套GNU交叉编译工具链，调试硬件使用SEGGER的J-Link ，调试软件可以使用SEGGER家的Ozne，以及一些常用嵌入式调试工具，如systemView或者JTAG的边界扫描工具。

> 基本可以用于嵌入式应用软件的开发，以及固件的开发。
>
> 未来期望能够加入自动化开发测试devops这样的工程开发中，实现质量自动化，开发，测试，调试闭环一体的嵌入式开发流程。
>
> 相关软件配置以及编译下载测试的操作。



# 模版工程环境说明

> 这里的模版工程，可以在不同平台使用如windows平台，Ubuntu这类linux平台，Mac等类Unix平台，区别在于其编译环境配置的不同。
>
> 根据不同的环境需要在编译脚本那里进行相应的修改。



安装所必要的软件开发工具，配置好相关环境。


| 工具名称          | 工具说明                                      | 版本号            |
| :---------------- | --------------------------------------------- | ----------------- |
| STM32cubeMX       | ST家的一键配置平台，可以快速搭建工程          | Version 6.12.0    |
| CMake             | 跨平台的软件构建工具The Standard Build System | Version 3.30.0    |
| Ninja             | 增量软件构建工具                              | Version 1.12.1    |
| gcc-arm-none-eabi | 交叉编译工具 The GNU Arm Embedded Toolchain   | Version 13.3.Rel1 |
| J-Link / J-Trace  | SEGGER                                        | Version 7.9.8a    |
| Ozne              | SEGGER                                        | Version 3.34a     |
| Clion/VSCode      | 代码编辑器                                    | --                |

> 以后会考虑使用docker配置相关开发环境



# 模板工作流程

## 配置STM32CubeMX



**打开工程**：

- 打开template.ioc，根据需要选择合适的微控制器型号或开发板。
- 配置外设引脚、时钟树、电源管理等硬件参数。
- 配置软件中间件，如一些通讯组件，操作系统组件，数学计算组件等。

**生成CMake项目文件**：

- 在配置完成后，点击“代码生成”选项卡，选择“CMake”作为生成器。
- 点击“生成”，STM32CubeMX会自动生成相应的CMakeLists.txt 文件。

## 编写用户代码

**编辑源文件**：

- 在生成的用户代码区域（通常是task.h和task.c文件）中添加你的应用程序代码。
- 使用`/*USER CODE BEGIN*/`和`/*USER CODE END*/`标记来区分用户代码和初始化代码。吧

## 编译和调试

本模板相应的提供了一些编译脚本供测试使用，只需要在相应的脚本目录下运行脚本就可以了。

- ```powershell
  PS C:\win> ls
  
  目录: C:\template\Scripts\win
  
  Mode                 LastWriteTime         Length Name
  ----                 -------------         ------ ----
  -a----         2024/7/29     19:50             62 build.bat
  -a----         2024/7/29     20:02            416 program.bat
  -a----         2024/7/29     20:02            159 rebuild.bat
  
  PS C:\template\Scripts\win> .\rebuild.bat
  
  ```

使用program.bat 后需要进行重新上电或者复位重启，这样程序才能继续运行。

该流程大概是会继续留存改进。



# 参考链接 



用于参考文章，项目，资料以及资源

* [STM32CubeMX 下载及安装教程](https://blog.csdn.net/Brendon_Tan/article/details/107685563)

* [GNU Arm Embedded Toolchain Downloads](https://developer.arm.com/downloads/-/gnu-rm)

* [CMake: The Standard Build System](https://cmake.org/features/)

* [SEGGER](https://www.segger.com)

* [STM32全外设原理、配置和常用HAL、LL库API使用详解](https://www.bilibili.com/video/BV1Tv411B7Uw/?vd_source=f16915df3a9cb28ac152974a7ac53975)

* [J-LINK 操作使用指南](https://blog.csdn.net/absinjun/article/details/79667308)

* [使用VScode开发STM32：基于CMake（包含标准库和HAL库工程）](https://blog.csdn.net/weixin_70110771/article/details/138268116)








