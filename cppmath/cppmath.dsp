# Microsoft Developer Studio Project File - Name="cppmath" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=cppmath - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cppmath.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cppmath - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "cppmath - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "cppmath - Win32 Release"
# Name "cppmath - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\bcuint.cpp
# End Source File
# Begin Source File

SOURCE=.\bmp.cpp
# End Source File
# Begin Source File

SOURCE=.\bvp.cpp
# End Source File
# Begin Source File

SOURCE=.\calculus.cpp
# End Source File
# Begin Source File

SOURCE=.\cppfft.cpp
# End Source File
# Begin Source File

SOURCE=.\Enorm.cpp
# End Source File
# Begin Source File

SOURCE=.\fileutil.cpp
# End Source File
# Begin Source File

SOURCE=.\four1.cpp
# End Source File
# Begin Source File

SOURCE=.\gridrefiner.cpp
# End Source File
# Begin Source File

SOURCE=.\imtql2.cpp
# End Source File
# Begin Source File

SOURCE=.\integ.cpp
# End Source File
# Begin Source File

SOURCE=.\interp.cpp
# End Source File
# Begin Source File

SOURCE=.\linalg.cpp
# End Source File
# Begin Source File

SOURCE=.\Lmdif.cpp
# End Source File
# Begin Source File

SOURCE=.\Lmpar.cpp
# End Source File
# Begin Source File

SOURCE=.\ptable.cpp
# End Source File
# Begin Source File

SOURCE=.\Qrfac.cpp
# End Source File
# Begin Source File

SOURCE=.\Qrsolv.cpp
# End Source File
# Begin Source File

SOURCE=.\rungekutta.cpp
# End Source File
# Begin Source File

SOURCE=.\sfunc.cpp
# End Source File
# Begin Source File

SOURCE=.\stoermer.cpp
# End Source File
# Begin Source File

SOURCE=.\util.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\cppfft.h
# End Source File
# Begin Source File

SOURCE=.\cppmath.h
# End Source File
# Begin Source File

SOURCE=.\cppminpack.h
# End Source File
# Begin Source File

SOURCE=.\filters.h
# End Source File
# Begin Source File

SOURCE=.\funcs.h
# End Source File
# Begin Source File

SOURCE=.\marsaglia.h
# End Source File
# Begin Source File

SOURCE=.\ode.h
# End Source File
# Begin Source File

SOURCE=.\ptable.h
# End Source File
# Begin Source File

SOURCE=.\stopwatch.h
# End Source File
# Begin Source File

SOURCE=.\tridiag.h
# End Source File
# Begin Source File

SOURCE=.\tridiagsym.h
# End Source File
# Begin Source File

SOURCE=.\utils.h
# End Source File
# End Group
# End Target
# End Project
