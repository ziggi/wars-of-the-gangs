set name=WoG
compiler\pawncc.exe -;+ -(+ -icompiler/includes -d2 sources\%name%.pwn
if exist %name%.amx ^
move %name%.amx gamemodes\
pause
