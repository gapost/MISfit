# Microsoft Developer Studio Generated NMAKE File, Based on misfit.dsp
!IF "$(CFG)" == ""
CFG=misfit - Win32 Debug
!MESSAGE No configuration specified. Defaulting to misfit - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "misfit - Win32 Release" && "$(CFG)" != "misfit - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "misfit.mak" CFG="misfit - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "misfit - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "misfit - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "misfit - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\misfit.exe"

!ELSE 

ALL : "scps - Win32 Release" "graph - Win32 Release" "cppmath - Win32 Release" "$(OUTDIR)\misfit.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"cppmath - Win32 ReleaseCLEAN" "graph - Win32 ReleaseCLEAN" "scps - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\BasicProp.obj"
	-@erase "$(INTDIR)\DataProp.obj"
	-@erase "$(INTDIR)\FreqDlg.obj"
	-@erase "$(INTDIR)\GVdata.obj"
	-@erase "$(INTDIR)\MISDlg.obj"
	-@erase "$(INTDIR)\misfit.obj"
	-@erase "$(INTDIR)\misfit.pch"
	-@erase "$(INTDIR)\misfit.res"
	-@erase "$(INTDIR)\MISProp.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TrapDlg.obj"
	-@erase "$(INTDIR)\TrapProp.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\misfit.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "../cppmath" /I "../graph" /I "../scps" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\misfit.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\misfit.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\misfit.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\misfit.pdb" /machine:I386 /out:"$(OUTDIR)\misfit.exe" 
LINK32_OBJS= \
	"$(INTDIR)\misfit.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\misfit.res" \
	"$(INTDIR)\BasicProp.obj" \
	"$(INTDIR)\DataProp.obj" \
	"$(INTDIR)\FreqDlg.obj" \
	"$(INTDIR)\GVdata.obj" \
	"$(INTDIR)\MISDlg.obj" \
	"$(INTDIR)\MISProp.obj" \
	"$(INTDIR)\TrapDlg.obj" \
	"$(INTDIR)\TrapProp.obj" \
	"..\cppmath\Release\cppmath.lib" \
	"..\graph\Release\graph.lib" \
	"..\scps\Release\scps.lib"

"$(OUTDIR)\misfit.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "misfit - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\misfit.exe"

!ELSE 

ALL : "scps - Win32 Debug" "graph - Win32 Debug" "cppmath - Win32 Debug" "$(OUTDIR)\misfit.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"cppmath - Win32 DebugCLEAN" "graph - Win32 DebugCLEAN" "scps - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\BasicProp.obj"
	-@erase "$(INTDIR)\DataProp.obj"
	-@erase "$(INTDIR)\FreqDlg.obj"
	-@erase "$(INTDIR)\GVdata.obj"
	-@erase "$(INTDIR)\MISDlg.obj"
	-@erase "$(INTDIR)\misfit.obj"
	-@erase "$(INTDIR)\misfit.pch"
	-@erase "$(INTDIR)\misfit.res"
	-@erase "$(INTDIR)\MISProp.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TrapDlg.obj"
	-@erase "$(INTDIR)\TrapProp.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\misfit.exe"
	-@erase "$(OUTDIR)\misfit.ilk"
	-@erase "$(OUTDIR)\misfit.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../cppmath" /I "../graph" /I "../scps" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\misfit.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ   /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\misfit.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\misfit.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\misfit.pdb" /debug /machine:I386 /out:"$(OUTDIR)\misfit.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\misfit.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\misfit.res" \
	"$(INTDIR)\BasicProp.obj" \
	"$(INTDIR)\DataProp.obj" \
	"$(INTDIR)\FreqDlg.obj" \
	"$(INTDIR)\GVdata.obj" \
	"$(INTDIR)\MISDlg.obj" \
	"$(INTDIR)\MISProp.obj" \
	"$(INTDIR)\TrapDlg.obj" \
	"$(INTDIR)\TrapProp.obj" \
	"..\cppmath\Debug\cppmath.lib" \
	"..\graph\Debug\graph.lib" \
	"..\scps\Debug\scps.lib"

"$(OUTDIR)\misfit.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("misfit.dep")
!INCLUDE "misfit.dep"
!ELSE 
!MESSAGE Warning: cannot find "misfit.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "misfit - Win32 Release" || "$(CFG)" == "misfit - Win32 Debug"
SOURCE=.\BasicProp.cpp

"$(INTDIR)\BasicProp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\misfit.pch"


SOURCE=.\DataProp.cpp

"$(INTDIR)\DataProp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\misfit.pch"


SOURCE=.\FreqDlg.cpp

"$(INTDIR)\FreqDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\misfit.pch"


SOURCE=.\GVdata.cpp

"$(INTDIR)\GVdata.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\misfit.pch"


SOURCE=.\MISDlg.cpp

"$(INTDIR)\MISDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\misfit.pch"


SOURCE=.\misfit.cpp

"$(INTDIR)\misfit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\misfit.pch"


SOURCE=.\misfit.rc

"$(INTDIR)\misfit.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\MISProp.cpp

"$(INTDIR)\MISProp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\misfit.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "misfit - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../cppmath" /I "../graph" /I "../scps" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\misfit.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\misfit.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "misfit - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../cppmath" /I "../graph" /I "../scps" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\misfit.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ   /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\misfit.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TrapDlg.cpp

"$(INTDIR)\TrapDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\misfit.pch"


SOURCE=.\TrapProp.cpp

"$(INTDIR)\TrapProp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\misfit.pch"


!IF  "$(CFG)" == "misfit - Win32 Release"

"cppmath - Win32 Release" : 
   cd "\Dev\OldWin\misfit\cppmath"
   $(MAKE) /$(MAKEFLAGS) /F .\cppmath.mak CFG="cppmath - Win32 Release" 
   cd "..\misfit"

"cppmath - Win32 ReleaseCLEAN" : 
   cd "\Dev\OldWin\misfit\cppmath"
   $(MAKE) /$(MAKEFLAGS) /F .\cppmath.mak CFG="cppmath - Win32 Release" RECURSE=1 CLEAN 
   cd "..\misfit"

!ELSEIF  "$(CFG)" == "misfit - Win32 Debug"

"cppmath - Win32 Debug" : 
   cd "\Dev\OldWin\misfit\cppmath"
   $(MAKE) /$(MAKEFLAGS) /F .\cppmath.mak CFG="cppmath - Win32 Debug" 
   cd "..\misfit"

"cppmath - Win32 DebugCLEAN" : 
   cd "\Dev\OldWin\misfit\cppmath"
   $(MAKE) /$(MAKEFLAGS) /F .\cppmath.mak CFG="cppmath - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\misfit"

!ENDIF 

!IF  "$(CFG)" == "misfit - Win32 Release"

"graph - Win32 Release" : 
   cd "\Dev\OldWin\misfit\graph"
   $(MAKE) /$(MAKEFLAGS) /F .\graph.mak CFG="graph - Win32 Release" 
   cd "..\misfit"

"graph - Win32 ReleaseCLEAN" : 
   cd "\Dev\OldWin\misfit\graph"
   $(MAKE) /$(MAKEFLAGS) /F .\graph.mak CFG="graph - Win32 Release" RECURSE=1 CLEAN 
   cd "..\misfit"

!ELSEIF  "$(CFG)" == "misfit - Win32 Debug"

"graph - Win32 Debug" : 
   cd "\Dev\OldWin\misfit\graph"
   $(MAKE) /$(MAKEFLAGS) /F .\graph.mak CFG="graph - Win32 Debug" 
   cd "..\misfit"

"graph - Win32 DebugCLEAN" : 
   cd "\Dev\OldWin\misfit\graph"
   $(MAKE) /$(MAKEFLAGS) /F .\graph.mak CFG="graph - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\misfit"

!ENDIF 

!IF  "$(CFG)" == "misfit - Win32 Release"

"scps - Win32 Release" : 
   cd "\Dev\OldWin\misfit\scps"
   $(MAKE) /$(MAKEFLAGS) /F .\scps.mak CFG="scps - Win32 Release" 
   cd "..\misfit"

"scps - Win32 ReleaseCLEAN" : 
   cd "\Dev\OldWin\misfit\scps"
   $(MAKE) /$(MAKEFLAGS) /F .\scps.mak CFG="scps - Win32 Release" RECURSE=1 CLEAN 
   cd "..\misfit"

!ELSEIF  "$(CFG)" == "misfit - Win32 Debug"

"scps - Win32 Debug" : 
   cd "\Dev\OldWin\misfit\scps"
   $(MAKE) /$(MAKEFLAGS) /F .\scps.mak CFG="scps - Win32 Debug" 
   cd "..\misfit"

"scps - Win32 DebugCLEAN" : 
   cd "\Dev\OldWin\misfit\scps"
   $(MAKE) /$(MAKEFLAGS) /F .\scps.mak CFG="scps - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\misfit"

!ENDIF 


!ENDIF 

