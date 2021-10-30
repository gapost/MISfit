# Microsoft Developer Studio Generated NMAKE File, Based on graph.dsp
!IF "$(CFG)" == ""
CFG=graph - Win32 Debug
!MESSAGE No configuration specified. Defaulting to graph - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "graph - Win32 Release" && "$(CFG)" != "graph - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "graph.mak" CFG="graph - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "graph - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "graph - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "graph - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\graph.dll"

!ELSE 

ALL : "cppmath - Win32 Release" "$(OUTDIR)\graph.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"cppmath - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\axis.obj"
	-@erase "$(INTDIR)\AxisWnd.obj"
	-@erase "$(INTDIR)\graph.obj"
	-@erase "$(INTDIR)\graph.pch"
	-@erase "$(INTDIR)\graph.res"
	-@erase "$(INTDIR)\GraphData.obj"
	-@erase "$(INTDIR)\GraphDlg.obj"
	-@erase "$(INTDIR)\graphframewnd.obj"
	-@erase "$(INTDIR)\GraphPaneWnd.obj"
	-@erase "$(INTDIR)\GraphWnd.obj"
	-@erase "$(INTDIR)\offscreen.obj"
	-@erase "$(INTDIR)\Series.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\graph.dll"
	-@erase "$(OUTDIR)\graph.exp"
	-@erase "$(OUTDIR)\graph.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "../cppmath" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\graph.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\graph.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\graph.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\graph.pdb" /machine:I386 /def:".\graph.def" /out:"$(OUTDIR)\graph.dll" /implib:"$(OUTDIR)\graph.lib" 
DEF_FILE= \
	".\graph.def"
LINK32_OBJS= \
	"$(INTDIR)\axis.obj" \
	"$(INTDIR)\AxisWnd.obj" \
	"$(INTDIR)\graph.obj" \
	"$(INTDIR)\GraphData.obj" \
	"$(INTDIR)\GraphDlg.obj" \
	"$(INTDIR)\graphframewnd.obj" \
	"$(INTDIR)\GraphPaneWnd.obj" \
	"$(INTDIR)\GraphWnd.obj" \
	"$(INTDIR)\offscreen.obj" \
	"$(INTDIR)\Series.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\graph.res" \
	"..\cppmath\Release\cppmath.lib"

"$(OUTDIR)\graph.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "graph - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\graph.dll"

!ELSE 

ALL : "cppmath - Win32 Debug" "$(OUTDIR)\graph.dll"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"cppmath - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\axis.obj"
	-@erase "$(INTDIR)\AxisWnd.obj"
	-@erase "$(INTDIR)\graph.obj"
	-@erase "$(INTDIR)\graph.pch"
	-@erase "$(INTDIR)\graph.res"
	-@erase "$(INTDIR)\GraphData.obj"
	-@erase "$(INTDIR)\GraphDlg.obj"
	-@erase "$(INTDIR)\graphframewnd.obj"
	-@erase "$(INTDIR)\GraphPaneWnd.obj"
	-@erase "$(INTDIR)\GraphWnd.obj"
	-@erase "$(INTDIR)\offscreen.obj"
	-@erase "$(INTDIR)\Series.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\graph.dll"
	-@erase "$(OUTDIR)\graph.exp"
	-@erase "$(OUTDIR)\graph.ilk"
	-@erase "$(OUTDIR)\graph.lib"
	-@erase "$(OUTDIR)\graph.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../cppmath" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\graph.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\graph.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\graph.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\graph.pdb" /debug /machine:I386 /def:".\graph.def" /out:"$(OUTDIR)\graph.dll" /implib:"$(OUTDIR)\graph.lib" /pdbtype:sept 
DEF_FILE= \
	".\graph.def"
LINK32_OBJS= \
	"$(INTDIR)\axis.obj" \
	"$(INTDIR)\AxisWnd.obj" \
	"$(INTDIR)\graph.obj" \
	"$(INTDIR)\GraphData.obj" \
	"$(INTDIR)\GraphDlg.obj" \
	"$(INTDIR)\graphframewnd.obj" \
	"$(INTDIR)\GraphPaneWnd.obj" \
	"$(INTDIR)\GraphWnd.obj" \
	"$(INTDIR)\offscreen.obj" \
	"$(INTDIR)\Series.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\graph.res" \
	"..\cppmath\Debug\cppmath.lib"

"$(OUTDIR)\graph.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("graph.dep")
!INCLUDE "graph.dep"
!ELSE 
!MESSAGE Warning: cannot find "graph.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "graph - Win32 Release" || "$(CFG)" == "graph - Win32 Debug"
SOURCE=.\axis.cpp

"$(INTDIR)\axis.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\graph.pch"


SOURCE=.\AxisWnd.cpp

"$(INTDIR)\AxisWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\graph.pch"


SOURCE=.\graph.cpp

"$(INTDIR)\graph.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\graph.pch"


SOURCE=.\graph.rc

"$(INTDIR)\graph.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\GraphData.cpp

"$(INTDIR)\GraphData.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\graph.pch"


SOURCE=.\GraphDlg.cpp

"$(INTDIR)\GraphDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\graph.pch"


SOURCE=.\graphframewnd.cpp

"$(INTDIR)\graphframewnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\graph.pch"


SOURCE=.\GraphPaneWnd.cpp

"$(INTDIR)\GraphPaneWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\graph.pch"


SOURCE=.\GraphWnd.cpp

"$(INTDIR)\GraphWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\graph.pch"


SOURCE=.\offscreen.cpp

"$(INTDIR)\offscreen.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\graph.pch"


SOURCE=.\Series.cpp

"$(INTDIR)\Series.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\graph.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "graph - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "../cppmath" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\graph.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\graph.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "graph - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../cppmath" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fp"$(INTDIR)\graph.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\graph.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

!IF  "$(CFG)" == "graph - Win32 Release"

"cppmath - Win32 Release" : 
   cd "\Dev\OldWin\misfit\cppmath"
   $(MAKE) /$(MAKEFLAGS) /F .\cppmath.mak CFG="cppmath - Win32 Release" 
   cd "..\graph"

"cppmath - Win32 ReleaseCLEAN" : 
   cd "\Dev\OldWin\misfit\cppmath"
   $(MAKE) /$(MAKEFLAGS) /F .\cppmath.mak CFG="cppmath - Win32 Release" RECURSE=1 CLEAN 
   cd "..\graph"

!ELSEIF  "$(CFG)" == "graph - Win32 Debug"

"cppmath - Win32 Debug" : 
   cd "\Dev\OldWin\misfit\cppmath"
   $(MAKE) /$(MAKEFLAGS) /F .\cppmath.mak CFG="cppmath - Win32 Debug" 
   cd "..\graph"

"cppmath - Win32 DebugCLEAN" : 
   cd "\Dev\OldWin\misfit\cppmath"
   $(MAKE) /$(MAKEFLAGS) /F .\cppmath.mak CFG="cppmath - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\graph"

!ENDIF 


!ENDIF 

