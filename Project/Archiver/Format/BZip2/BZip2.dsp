# Microsoft Developer Studio Project File - Name="BZip2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BZip2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BZip2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BZip2.mak" CFG="BZip2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BZip2 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BZip2 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BZip2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BZIP2_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\SDK" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BZIP2_EXPORTS" /Yu"StdAfx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"C:\Program Files\7-ZIP\Format\BZip2.dll" /opt:NOWIN98
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "BZip2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BZIP2_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\SDK" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BZIP2_EXPORTS" /Yu"StdAfx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"C:\Program Files\7-ZIP\Format\BZip2.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "BZip2 - Win32 Release"
# Name "BZip2 - Win32 Debug"
# Begin Group "Spec"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BZip2.def
# End Source File
# Begin Source File

SOURCE=.\BZip2.ico
# End Source File
# Begin Source File

SOURCE=.\DllExports.cpp
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"StdAfx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\SDK\Common\NewHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Common\NewHandler.h
# End Source File
# End Group
# Begin Group "Windows"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\SDK\Windows\PropVariant.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Windows\PropVariant.h
# End Source File
# End Group
# Begin Group "Interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\SDK\Interface\EnumStatProp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Interface\EnumStatProp.h
# End Source File
# Begin Source File

SOURCE=..\..\Handler\IArchiveHandler.h
# End Source File
# Begin Source File

SOURCE=..\..\Common\IArchiveHandler2.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Interface\ProgressUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Interface\ProgressUtils.h
# End Source File
# End Group
# Begin Group "Compression"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\SDK\Compression\CopyCoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\SDK\Compression\CopyCoder.h
# End Source File
# End Group
# Begin Group "Format Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\DummyOutStream.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\DummyOutStream.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Handler.h
# End Source File
# Begin Source File

SOURCE=.\ItemInfoEx.h
# End Source File
# Begin Source File

SOURCE=.\OutHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\UpdateEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\UpdateEngine.h
# End Source File
# End Target
# End Project
