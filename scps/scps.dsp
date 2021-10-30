# Microsoft Developer Studio Project File - Name="scps" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=scps - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "scps.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "scps - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../cppmath" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "scps - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ  /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../cppmath" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ  /c
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

# Name "scps - Win32 Release"
# Name "scps - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DielectricLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\FermiFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\Ge_MIS.cpp
# End Source File
# Begin Source File

SOURCE=.\MIS.cpp
# End Source File
# Begin Source File

SOURCE=.\misint.cpp
# End Source File
# Begin Source File

SOURCE=.\Poisson1D.cpp
# End Source File
# Begin Source File

SOURCE=.\Poisson1DItem.cpp
# End Source File
# Begin Source File

SOURCE=.\qit.cpp
# End Source File
# Begin Source File

SOURCE=.\Schrodinger1D.cpp
# End Source File
# Begin Source File

SOURCE=.\Schrodinger1DItem.cpp
# End Source File
# Begin Source File

SOURCE=.\SchrodingerBands.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\constants.h
# End Source File
# Begin Source File

SOURCE=.\DielectricLayer.h
# End Source File
# Begin Source File

SOURCE=.\FermiFunc.h
# End Source File
# Begin Source File

SOURCE=.\Grid.h
# End Source File
# Begin Source File

SOURCE=.\MIS.h
# End Source File
# Begin Source File

SOURCE=.\misint.h
# End Source File
# Begin Source File

SOURCE=.\Poisson1D.h
# End Source File
# Begin Source File

SOURCE=.\Poisson1DItem.h
# End Source File
# Begin Source File

SOURCE=.\Qit.h
# End Source File
# Begin Source File

SOURCE=.\Schrodinger1D.h
# End Source File
# Begin Source File

SOURCE=.\Schrodinger1DItem.h
# End Source File
# Begin Source File

SOURCE=.\SchrodingerBands.h
# End Source File
# Begin Source File

SOURCE=.\scps.h
# End Source File
# Begin Source File

SOURCE=.\SiLayer.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
