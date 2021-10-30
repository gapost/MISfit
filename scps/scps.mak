# Microsoft Developer Studio Generated NMAKE File, Based on scps.dsp
!IF "$(CFG)" == ""
CFG=scps - Win32 Debug
!MESSAGE No configuration specified. Defaulting to scps - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "scps - Win32 Release" && "$(CFG)" != "scps - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "scps.mak" CFG="scps - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "scps - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "scps - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "scps - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\scps.lib"

!ELSE 

ALL : "cppmath - Win32 Release" "$(OUTDIR)\scps.lib"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"cppmath - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\DielectricLayer.obj"
	-@erase "$(INTDIR)\FermiFunc.obj"
	-@erase "$(INTDIR)\Ge_MIS.obj"
	-@erase "$(INTDIR)\MIS.obj"
	-@erase "$(INTDIR)\misint.obj"
	-@erase "$(INTDIR)\Poisson1D.obj"
	-@erase "$(INTDIR)\Poisson1DItem.obj"
	-@erase "$(INTDIR)\qit.obj"
	-@erase "$(INTDIR)\Schrodinger1D.obj"
	-@erase "$(INTDIR)\Schrodinger1DItem.obj"
	-@erase "$(INTDIR)\SchrodingerBands.obj"
	-@erase "$(INTDIR)\scps.pch"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\scps.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "../cppmath" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\scps.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\scps.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\scps.lib" 
LIB32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\DielectricLayer.obj" \
	"$(INTDIR)\FermiFunc.obj" \
	"$(INTDIR)\Ge_MIS.obj" \
	"$(INTDIR)\MIS.obj" \
	"$(INTDIR)\misint.obj" \
	"$(INTDIR)\Poisson1D.obj" \
	"$(INTDIR)\Poisson1DItem.obj" \
	"$(INTDIR)\qit.obj" \
	"$(INTDIR)\Schrodinger1D.obj" \
	"$(INTDIR)\Schrodinger1DItem.obj" \
	"$(INTDIR)\SchrodingerBands.obj" \
	"..\cppmath\Release\cppmath.lib"

"$(OUTDIR)\scps.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "scps - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\scps.lib"

!ELSE 

ALL : "cppmath - Win32 Debug" "$(OUTDIR)\scps.lib"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"cppmath - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\DielectricLayer.obj"
	-@erase "$(INTDIR)\FermiFunc.obj"
	-@erase "$(INTDIR)\Ge_MIS.obj"
	-@erase "$(INTDIR)\MIS.obj"
	-@erase "$(INTDIR)\misint.obj"
	-@erase "$(INTDIR)\Poisson1D.obj"
	-@erase "$(INTDIR)\Poisson1DItem.obj"
	-@erase "$(INTDIR)\qit.obj"
	-@erase "$(INTDIR)\Schrodinger1D.obj"
	-@erase "$(INTDIR)\Schrodinger1DItem.obj"
	-@erase "$(INTDIR)\SchrodingerBands.obj"
	-@erase "$(INTDIR)\scps.pch"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\scps.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../cppmath" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\scps.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ  /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\scps.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\scps.lib" 
LIB32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\DielectricLayer.obj" \
	"$(INTDIR)\FermiFunc.obj" \
	"$(INTDIR)\Ge_MIS.obj" \
	"$(INTDIR)\MIS.obj" \
	"$(INTDIR)\misint.obj" \
	"$(INTDIR)\Poisson1D.obj" \
	"$(INTDIR)\Poisson1DItem.obj" \
	"$(INTDIR)\qit.obj" \
	"$(INTDIR)\Schrodinger1D.obj" \
	"$(INTDIR)\Schrodinger1DItem.obj" \
	"$(INTDIR)\SchrodingerBands.obj" \
	"..\cppmath\Debug\cppmath.lib"

"$(OUTDIR)\scps.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
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
!IF EXISTS("scps.dep")
!INCLUDE "scps.dep"
!ELSE 
!MESSAGE Warning: cannot find "scps.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "scps - Win32 Release" || "$(CFG)" == "scps - Win32 Debug"
SOURCE=.\DielectricLayer.cpp

"$(INTDIR)\DielectricLayer.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\scps.pch"


SOURCE=.\FermiFunc.cpp

"$(INTDIR)\FermiFunc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\scps.pch"


SOURCE=.\Ge_MIS.cpp

"$(INTDIR)\Ge_MIS.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\scps.pch"


SOURCE=.\MIS.cpp

"$(INTDIR)\MIS.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\scps.pch"


SOURCE=.\misint.cpp

"$(INTDIR)\misint.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\scps.pch"


SOURCE=.\Poisson1D.cpp

"$(INTDIR)\Poisson1D.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\scps.pch"


SOURCE=.\Poisson1DItem.cpp

"$(INTDIR)\Poisson1DItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\scps.pch"


SOURCE=.\qit.cpp

"$(INTDIR)\qit.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\scps.pch"


SOURCE=.\Schrodinger1D.cpp

"$(INTDIR)\Schrodinger1D.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\scps.pch"


SOURCE=.\Schrodinger1DItem.cpp

"$(INTDIR)\Schrodinger1DItem.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\scps.pch"


SOURCE=.\SchrodingerBands.cpp

"$(INTDIR)\SchrodingerBands.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\scps.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "scps - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /I "../cppmath" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\scps.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\scps.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "scps - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "../cppmath" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\scps.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ  /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\scps.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

!IF  "$(CFG)" == "scps - Win32 Release"

"cppmath - Win32 Release" : 
   cd "\Dev\OldWin\misfit\cppmath"
   $(MAKE) /$(MAKEFLAGS) /F .\cppmath.mak CFG="cppmath - Win32 Release" 
   cd "..\scps"

"cppmath - Win32 ReleaseCLEAN" : 
   cd "\Dev\OldWin\misfit\cppmath"
   $(MAKE) /$(MAKEFLAGS) /F .\cppmath.mak CFG="cppmath - Win32 Release" RECURSE=1 CLEAN 
   cd "..\scps"

!ELSEIF  "$(CFG)" == "scps - Win32 Debug"

"cppmath - Win32 Debug" : 
   cd "\Dev\OldWin\misfit\cppmath"
   $(MAKE) /$(MAKEFLAGS) /F .\cppmath.mak CFG="cppmath - Win32 Debug" 
   cd "..\scps"

"cppmath - Win32 DebugCLEAN" : 
   cd "\Dev\OldWin\misfit\cppmath"
   $(MAKE) /$(MAKEFLAGS) /F .\cppmath.mak CFG="cppmath - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\scps"

!ENDIF 


!ENDIF 

