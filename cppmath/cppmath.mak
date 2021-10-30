# Microsoft Developer Studio Generated NMAKE File, Based on cppmath.dsp
!IF "$(CFG)" == ""
CFG=cppmath - Win32 Debug
!MESSAGE No configuration specified. Defaulting to cppmath - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "cppmath - Win32 Release" && "$(CFG)" != "cppmath - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cppmath.mak" CFG="cppmath - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cppmath - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "cppmath - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "cppmath - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\cppmath.lib"


CLEAN :
	-@erase "$(INTDIR)\bcuint.obj"
	-@erase "$(INTDIR)\bmp.obj"
	-@erase "$(INTDIR)\bvp.obj"
	-@erase "$(INTDIR)\calculus.obj"
	-@erase "$(INTDIR)\cppfft.obj"
	-@erase "$(INTDIR)\Enorm.obj"
	-@erase "$(INTDIR)\fileutil.obj"
	-@erase "$(INTDIR)\four1.obj"
	-@erase "$(INTDIR)\gridrefiner.obj"
	-@erase "$(INTDIR)\imtql2.obj"
	-@erase "$(INTDIR)\integ.obj"
	-@erase "$(INTDIR)\interp.obj"
	-@erase "$(INTDIR)\linalg.obj"
	-@erase "$(INTDIR)\Lmdif.obj"
	-@erase "$(INTDIR)\Lmpar.obj"
	-@erase "$(INTDIR)\ptable.obj"
	-@erase "$(INTDIR)\Qrfac.obj"
	-@erase "$(INTDIR)\Qrsolv.obj"
	-@erase "$(INTDIR)\rungekutta.obj"
	-@erase "$(INTDIR)\sfunc.obj"
	-@erase "$(INTDIR)\stoermer.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\cppmath.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\cppmath.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cppmath.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\cppmath.lib" 
LIB32_OBJS= \
	"$(INTDIR)\bcuint.obj" \
	"$(INTDIR)\bmp.obj" \
	"$(INTDIR)\bvp.obj" \
	"$(INTDIR)\calculus.obj" \
	"$(INTDIR)\cppfft.obj" \
	"$(INTDIR)\Enorm.obj" \
	"$(INTDIR)\fileutil.obj" \
	"$(INTDIR)\four1.obj" \
	"$(INTDIR)\gridrefiner.obj" \
	"$(INTDIR)\imtql2.obj" \
	"$(INTDIR)\integ.obj" \
	"$(INTDIR)\interp.obj" \
	"$(INTDIR)\linalg.obj" \
	"$(INTDIR)\Lmdif.obj" \
	"$(INTDIR)\Lmpar.obj" \
	"$(INTDIR)\ptable.obj" \
	"$(INTDIR)\Qrfac.obj" \
	"$(INTDIR)\Qrsolv.obj" \
	"$(INTDIR)\rungekutta.obj" \
	"$(INTDIR)\sfunc.obj" \
	"$(INTDIR)\stoermer.obj" \
	"$(INTDIR)\util.obj"

"$(OUTDIR)\cppmath.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\cppmath.lib" "$(OUTDIR)\cppmath.bsc"


CLEAN :
	-@erase "$(INTDIR)\bcuint.obj"
	-@erase "$(INTDIR)\bcuint.sbr"
	-@erase "$(INTDIR)\bmp.obj"
	-@erase "$(INTDIR)\bmp.sbr"
	-@erase "$(INTDIR)\bvp.obj"
	-@erase "$(INTDIR)\bvp.sbr"
	-@erase "$(INTDIR)\calculus.obj"
	-@erase "$(INTDIR)\calculus.sbr"
	-@erase "$(INTDIR)\cppfft.obj"
	-@erase "$(INTDIR)\cppfft.sbr"
	-@erase "$(INTDIR)\Enorm.obj"
	-@erase "$(INTDIR)\Enorm.sbr"
	-@erase "$(INTDIR)\fileutil.obj"
	-@erase "$(INTDIR)\fileutil.sbr"
	-@erase "$(INTDIR)\four1.obj"
	-@erase "$(INTDIR)\four1.sbr"
	-@erase "$(INTDIR)\gridrefiner.obj"
	-@erase "$(INTDIR)\gridrefiner.sbr"
	-@erase "$(INTDIR)\imtql2.obj"
	-@erase "$(INTDIR)\imtql2.sbr"
	-@erase "$(INTDIR)\integ.obj"
	-@erase "$(INTDIR)\integ.sbr"
	-@erase "$(INTDIR)\interp.obj"
	-@erase "$(INTDIR)\interp.sbr"
	-@erase "$(INTDIR)\linalg.obj"
	-@erase "$(INTDIR)\linalg.sbr"
	-@erase "$(INTDIR)\Lmdif.obj"
	-@erase "$(INTDIR)\Lmdif.sbr"
	-@erase "$(INTDIR)\Lmpar.obj"
	-@erase "$(INTDIR)\Lmpar.sbr"
	-@erase "$(INTDIR)\ptable.obj"
	-@erase "$(INTDIR)\ptable.sbr"
	-@erase "$(INTDIR)\Qrfac.obj"
	-@erase "$(INTDIR)\Qrfac.sbr"
	-@erase "$(INTDIR)\Qrsolv.obj"
	-@erase "$(INTDIR)\Qrsolv.sbr"
	-@erase "$(INTDIR)\rungekutta.obj"
	-@erase "$(INTDIR)\rungekutta.sbr"
	-@erase "$(INTDIR)\sfunc.obj"
	-@erase "$(INTDIR)\sfunc.sbr"
	-@erase "$(INTDIR)\stoermer.obj"
	-@erase "$(INTDIR)\stoermer.sbr"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\cppmath.bsc"
	-@erase "$(OUTDIR)\cppmath.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\cppmath.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\cppmath.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\bcuint.sbr" \
	"$(INTDIR)\bmp.sbr" \
	"$(INTDIR)\bvp.sbr" \
	"$(INTDIR)\calculus.sbr" \
	"$(INTDIR)\cppfft.sbr" \
	"$(INTDIR)\Enorm.sbr" \
	"$(INTDIR)\fileutil.sbr" \
	"$(INTDIR)\four1.sbr" \
	"$(INTDIR)\gridrefiner.sbr" \
	"$(INTDIR)\imtql2.sbr" \
	"$(INTDIR)\integ.sbr" \
	"$(INTDIR)\interp.sbr" \
	"$(INTDIR)\linalg.sbr" \
	"$(INTDIR)\Lmdif.sbr" \
	"$(INTDIR)\Lmpar.sbr" \
	"$(INTDIR)\ptable.sbr" \
	"$(INTDIR)\Qrfac.sbr" \
	"$(INTDIR)\Qrsolv.sbr" \
	"$(INTDIR)\rungekutta.sbr" \
	"$(INTDIR)\sfunc.sbr" \
	"$(INTDIR)\stoermer.sbr" \
	"$(INTDIR)\util.sbr"

"$(OUTDIR)\cppmath.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\cppmath.lib" 
LIB32_OBJS= \
	"$(INTDIR)\bcuint.obj" \
	"$(INTDIR)\bmp.obj" \
	"$(INTDIR)\bvp.obj" \
	"$(INTDIR)\calculus.obj" \
	"$(INTDIR)\cppfft.obj" \
	"$(INTDIR)\Enorm.obj" \
	"$(INTDIR)\fileutil.obj" \
	"$(INTDIR)\four1.obj" \
	"$(INTDIR)\gridrefiner.obj" \
	"$(INTDIR)\imtql2.obj" \
	"$(INTDIR)\integ.obj" \
	"$(INTDIR)\interp.obj" \
	"$(INTDIR)\linalg.obj" \
	"$(INTDIR)\Lmdif.obj" \
	"$(INTDIR)\Lmpar.obj" \
	"$(INTDIR)\ptable.obj" \
	"$(INTDIR)\Qrfac.obj" \
	"$(INTDIR)\Qrsolv.obj" \
	"$(INTDIR)\rungekutta.obj" \
	"$(INTDIR)\sfunc.obj" \
	"$(INTDIR)\stoermer.obj" \
	"$(INTDIR)\util.obj"

"$(OUTDIR)\cppmath.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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
!IF EXISTS("cppmath.dep")
!INCLUDE "cppmath.dep"
!ELSE 
!MESSAGE Warning: cannot find "cppmath.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "cppmath - Win32 Release" || "$(CFG)" == "cppmath - Win32 Debug"
SOURCE=.\bcuint.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\bcuint.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\bcuint.obj"	"$(INTDIR)\bcuint.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\bmp.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\bmp.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\bmp.obj"	"$(INTDIR)\bmp.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\bvp.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\bvp.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\bvp.obj"	"$(INTDIR)\bvp.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\calculus.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\calculus.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\calculus.obj"	"$(INTDIR)\calculus.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\cppfft.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\cppfft.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\cppfft.obj"	"$(INTDIR)\cppfft.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Enorm.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\Enorm.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\Enorm.obj"	"$(INTDIR)\Enorm.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fileutil.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\fileutil.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\fileutil.obj"	"$(INTDIR)\fileutil.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\four1.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\four1.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\four1.obj"	"$(INTDIR)\four1.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\gridrefiner.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\gridrefiner.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\gridrefiner.obj"	"$(INTDIR)\gridrefiner.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\imtql2.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\imtql2.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\imtql2.obj"	"$(INTDIR)\imtql2.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\integ.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\integ.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\integ.obj"	"$(INTDIR)\integ.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\interp.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\interp.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\interp.obj"	"$(INTDIR)\interp.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\linalg.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\linalg.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\linalg.obj"	"$(INTDIR)\linalg.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Lmdif.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\Lmdif.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\Lmdif.obj"	"$(INTDIR)\Lmdif.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Lmpar.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\Lmpar.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\Lmpar.obj"	"$(INTDIR)\Lmpar.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ptable.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\ptable.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\ptable.obj"	"$(INTDIR)\ptable.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Qrfac.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\Qrfac.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\Qrfac.obj"	"$(INTDIR)\Qrfac.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Qrsolv.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\Qrsolv.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\Qrsolv.obj"	"$(INTDIR)\Qrsolv.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rungekutta.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\rungekutta.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\rungekutta.obj"	"$(INTDIR)\rungekutta.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\sfunc.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\sfunc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\sfunc.obj"	"$(INTDIR)\sfunc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\stoermer.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\stoermer.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\stoermer.obj"	"$(INTDIR)\stoermer.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\util.cpp

!IF  "$(CFG)" == "cppmath - Win32 Release"


"$(INTDIR)\util.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"


"$(INTDIR)\util.obj"	"$(INTDIR)\util.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

