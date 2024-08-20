
#!/bin/bash
echo "Hello let's get compile!"
# 获取当前脚本所在的目录
Current_dir=$(dirname $0)
# 定义上级目录
Parents_dir=$(cd ..$1; pwd) # 获取传递的第一个参数作为目标目录
# 定义上级的上级目录
ParentsParent_dir=$(cd ../../$1; pwd) # 获取传递的第一个参数作为目标目录
# 进入上级的上级目录
echo $ParentsParent_dir
cd $ParentsParent_dir || exit
# 使用cmake+ninja进行构建 cmake指令分开执行
cmake -B "build" -G "Ninja"  || exit
ninja -C "build"  || exit
