set library=stdio.obj conv.obj ctype.obj math.obj  portio.obj  mathlib.obj string.obj bios.obj int.obj keyboard.obj

set kernel=startpoint.obj main.obj vdu.obj mem.obj disk.obj fs.obj vfs.obj fat32.obj  shell.obj ntfs.obj fat12.obj      


.\VC152\CL.EXE    /AT  /G3 /Gs /Gx /c /Zl /Zp1  /I include kernel\*.cpp

.\VC152\CL.EXE   /AT  /G3 /Gs /Gx /c /Zl /Zp1   /I include  lib\*.cpp


.\VC152\ML.EXE  /AT  /c   kernel\*.asm 
.\VC152\ML.EXE  /AT  /c   lib\*.asm 


.\VC152\LINK.EXE   /T  /NOD %kernel% %library%  

del *.obj
ren *.com KRNLDR.COM
mkdir bin\Boot
move /Y *.com bin\Boot\
pause