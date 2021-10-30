# Microsoft Developer Studio Generated NMAKE File, Based on gentbl.dsp
!IF "$(CFG)" == ""
CFG=gentbl - Win32 Debug
!MESSAGE No configuration specified. Defaulting to gentbl - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "gentbl - Win32 Release" && "$(CFG)" != "gentbl - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gentbl.mak" CFG="gentbl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gentbl - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "gentbl - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "gentbl - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\gentbl.exe"


CLEAN :
	-@erase "$(INTDIR)\calc.obj"
	-@erase "$(INTDIR)\calc_gaas.obj"
	-@erase "$(INTDIR)\calc_ge.obj"
	-@erase "$(INTDIR)\calc_q.obj"
	-@erase "$(INTDIR)\calc_q_gaas.obj"
	-@erase "$(INTDIR)\calc_q_ge.obj"
	-@erase "$(INTDIR)\gentbl.obj"
	-@erase "$(INTDIR)\gentbl.pch"
	-@erase "$(INTDIR)\gentbl.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\gentbl.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /D "_WINDOWS" /Fp"$(INTDIR)\gentbl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
RSC_PROJ=/l 0x408 /fo"$(INTDIR)\gentbl.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\gentbl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\scps\release\scps.lib graph.lib cppmath.lib /nologo /subsystem:console /profile /machine:I386 /out:"$(OUTDIR)\gentbl.exe" 
LINK32_OBJS= \
	"$(INTDIR)\calc.obj" \
	"$(INTDIR)\calc_gaas.obj" \
	"$(INTDIR)\calc_ge.obj" \
	"$(INTDIR)\calc_q.obj" \
	"$(INTDIR)\calc_q_gaas.obj" \
	"$(INTDIR)\calc_q_ge.obj" \
	"$(INTDIR)\gentbl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\gentbl.res"

"$(OUTDIR)\gentbl.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "gentbl - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\gentbl.exe"


CLEAN :
	-@erase "$(INTDIR)\calc.obj"
	-@erase "$(INTDIR)\calc_gaas.obj"
	-@erase "$(INTDIR)\calc_ge.obj"
	-@erase "$(INTDIR)\calc_q.obj"
	-@erase "$(INTDIR)\calc_q_gaas.obj"
	-@erase "$(INTDIR)\calc_q_ge.obj"
	-@erase "$(INTDIR)\gentbl.obj"
	-@erase "$(INTDIR)\gentbl.pch"
	-@erase "$(INTDIR)\gentbl.res"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\gentbl.exe"
	-@erase "$(OUTDIR)\gentbl.ilk"
	-@erase "$(OUTDIR)\gentbl.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=df.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /D "_WINDOWS" /Fp"$(INTDIR)\gentbl.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
RSC_PROJ=/l 0x408 /fo"$(INTDIR)\gentbl.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\gentbl.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\scps\debug\scps.lib graphdb.lib cppmathdb.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\gentbl.pdb" /debug /machine:I386 /out:"$(OUTDIR)\gentbl.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\calc.obj" \
	"$(INTDIR)\calc_gaas.obj" \
	"$(INTDIR)\calc_ge.obj" \
	"$(INTDIR)\calc_q.obj" \
	"$(INTDIR)\calc_q_gaas.obj" \
	"$(INTDIR)\calc_q_ge.obj" \
	"$(INTDIR)\gentbl.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\gentbl.res"

"$(OUTDIR)\gentbl.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("gentbl.dep")
!INCLUDE "gentbl.dep"
!ELSE 
!MESSAGE Warning: cannot find "gentbl.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "gentbl - Win32 Release" || "$(CFG)" == "gentbl - Win32 Debug"
SOURCE=.\calc.cpp

"$(INTDIR)\calc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\gentbl.pch"


SOURCE=.\calc_gaas.cpp

"$(INTDIR)\calc_gaas.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\gentbl.pch"


SOURCE=.\calc_ge.cpp

"$(INTDIR)\calc_ge.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\gentbl.pch"


SOURCE=.\calc_q.cpp

"$(INTDIR)\calc_q.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\gentbl.pch"


SOURCE=.\calc_q_gaas.cpp

"$(INTDIR)\calc_q_gaas.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\gentbl.pch"


SOURCE=.\calc_q_ge.cpp

"$(INTDIR)\calc_q_ge.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\gentbl.pch"


SOURCE=.\gentbl.cpp

"$(INTDIR)\gentbl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\gentbl.pch"


SOURCE=.\gentbl.rc

"$(INTDIR)\gentbl.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "gentbl - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /D "_WINDOWS" /Fp"$(INTDIR)\gentbl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\gentbl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "gentbl - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /D "_WINDOWS" /Fp"$(INTDIR)\gentbl.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\gentbl.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

