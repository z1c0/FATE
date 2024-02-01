# Microsoft Developer Studio Project File - Name="FATEsim" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=FATEsim - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FATEsim.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FATEsim.mak" CFG="FATEsim - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FATEsim - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "FATEsim - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FATEsim - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "lib\WIN32"
# PROP Intermediate_Dir "src\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "FATEsim - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "lib\WIN32"
# PROP Intermediate_Dir "src\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"lib\WIN32\FATEsimD.lib"

!ENDIF 

# Begin Target

# Name "FATEsim - Win32 Release"
# Name "FATEsim - Win32 Debug"
# Begin Group "base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\base\FATE_WIN32.cpp
# End Source File
# Begin Source File

SOURCE=.\base\FateApp.cpp
# End Source File
# Begin Source File

SOURCE=.\base\FateApp.h
# End Source File
# Begin Source File

SOURCE=.\base\FateTypeDefs.h
# End Source File
# Begin Source File

SOURCE=.\base\FBitmap.h
# End Source File
# Begin Source File

SOURCE=.\base\FBitmap_WIN32.cpp
# End Source File
# Begin Source File

SOURCE=.\base\FBitmap_WIN32.h
# End Source File
# Begin Source File

SOURCE=.\base\FFile.h
# End Source File
# Begin Source File

SOURCE=.\base\FFile_WIN32.cpp
# End Source File
# Begin Source File

SOURCE=.\base\FFile_WIN32.h
# End Source File
# Begin Source File

SOURCE=.\base\FLinkedList.h
# End Source File
# Begin Source File

SOURCE=.\base\FPoint.h
# End Source File
# Begin Source File

SOURCE=.\base\FSystem.h
# End Source File
# Begin Source File

SOURCE=.\base\FSystem_WIN32.cpp
# End Source File
# Begin Source File

SOURCE=.\base\FSystem_WIN32.h
# End Source File
# Begin Source File

SOURCE=.\base\IFateComponent.h
# End Source File
# Begin Source File

SOURCE=.\base\IFateContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\base\IFateContainer.h
# End Source File
# Begin Source File

SOURCE=.\base\IFButtonListener.h
# End Source File
# Begin Source File

SOURCE=.\base\IFCharListener.h
# End Source File
# Begin Source File

SOURCE=.\base\IFConnectionListener.h
# End Source File
# Begin Source File

SOURCE=.\base\IFDropListListener.h
# End Source File
# Begin Source File

SOURCE=.\base\IFItemListListener.h
# End Source File
# Begin Source File

SOURCE=.\base\IFMenuListener.h
# End Source File
# Begin Source File

SOURCE=.\base\IFSliderListener.h
# End Source File
# End Group
# Begin Group "comm"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\comm\FComPort.h
# End Source File
# Begin Source File

SOURCE=.\comm\FInetAddr_WIN32.h
# End Source File
# Begin Source File

SOURCE=.\comm\FServer.cpp
# End Source File
# Begin Source File

SOURCE=.\comm\FSocket_WIN32.cpp
# End Source File
# Begin Source File

SOURCE=.\comm\FSocket_WIN32.h
# End Source File
# Begin Source File

SOURCE=.\comm\FUDPSocket_WIN32.cpp
# End Source File
# Begin Source File

SOURCE=.\comm\FUDPSocket_WIN32.h
# End Source File
# Begin Source File

SOURCE=.\comm\IFSocket_WIN32.cpp
# End Source File
# Begin Source File

SOURCE=.\comm\IFSocket_WIN32.h
# End Source File
# End Group
# Begin Group "gui"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\gui\FButton.cpp
# End Source File
# Begin Source File

SOURCE=.\gui\FButton.h
# End Source File
# Begin Source File

SOURCE=.\gui\FDropList.cpp
# End Source File
# Begin Source File

SOURCE=.\gui\FDropList.h
# End Source File
# Begin Source File

SOURCE=.\gui\FItemList.cpp
# End Source File
# Begin Source File

SOURCE=.\gui\FItemList.h
# End Source File
# Begin Source File

SOURCE=.\gui\FLabel.cpp
# End Source File
# Begin Source File

SOURCE=.\gui\FLabel.h
# End Source File
# Begin Source File

SOURCE=.\gui\FMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\gui\FMenu.h
# End Source File
# Begin Source File

SOURCE=.\gui\FMsgPanel.h
# End Source File
# Begin Source File

SOURCE=.\gui\FPanel.h
# End Source File
# End Group
# Begin Group "include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\include\fate.h
# End Source File
# End Group
# Begin Group "util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\util\FLinkedList.h
# End Source File
# Begin Source File

SOURCE=.\util\FPoint.h
# End Source File
# Begin Source File

SOURCE=.\util\FRect.h
# End Source File
# Begin Source File

SOURCE=.\util\FTicker.h
# End Source File
# End Group
# Begin Group "ext"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ext\FController.cpp
# End Source File
# Begin Source File

SOURCE=.\ext\FController.h
# End Source File
# Begin Source File

SOURCE=.\ext\FNaviMap.cpp
# End Source File
# Begin Source File

SOURCE=.\ext\FNaviMap.h
# End Source File
# Begin Source File

SOURCE=.\ext\FOffscreenRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\ext\FOffscreenRenderer.h
# End Source File
# Begin Source File

SOURCE=.\ext\FStartMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\ext\FStartMenu.h
# End Source File
# Begin Source File

SOURCE=.\ext\FVEObj.cpp
# End Source File
# Begin Source File

SOURCE=.\ext\FVEObj.h
# End Source File
# Begin Source File

SOURCE=.\ext\FVEObjAttr.cpp
# End Source File
# Begin Source File

SOURCE=.\ext\FVEObjAttr.h
# End Source File
# Begin Source File

SOURCE=.\ext\FVEObjManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ext\FVEObjManager.h
# End Source File
# Begin Source File

SOURCE=.\ext\FVEObjPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\ext\FVEObjPanel.h
# End Source File
# Begin Source File

SOURCE=.\ext\ve_protocol.h
# End Source File
# End Group
# End Target
# End Project
