
cd ..
cd ..

set curdir=%cd%

echo %curdir%
set BuildPath=%curdir%\build

rd /s /q "%BuildPath%"
cmake -B build -G "Ninja"
ninja -C build

 pause