
# #!/bin/bash
# echo "Hello let's get compile!"
# # 获取当前脚本所在的目录
# Current_dir=$(dirname $0)
# # 定义上级目录
# Parents_dir=$(cd ..$1; pwd) # 获取传递的第一个参数作为目标目录
# # 定义上级的上级目录
# ParentsParent_dir=$(cd ../../$1; pwd) # 获取传递的第一个参数作为目标目录
# # 定义jlink目录
# JLinkPath="/Applications/SEGGER/JLink_V798a/JFlash.app/Contents/MacOS"
# # 定义jflash烧录配置
# JFlashPath="$ParentsParent_dir"/template.jflash""
# # 定义程序
# BinPath=$ParentsParent_dir"/build/template.elf"

# cd "/Applications/SEGGER/JLink_V798a/JFlash.app/Contents/MacOS" || exit
# echo $JFlashPath
# echo $BinPath
# echo $(cd .$1; pwd)    
# ./JFlashExe -openprj $($JFlashPath) -open $($BinPath),0x08000000 -auto -exit || exit