# Microsoft Developer Studio Project File - Name="VRENAsim25" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=VRENAsim25 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VRENAsim25.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VRENAsim25.mak" CFG="VRENAsim25 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VRENAsim25 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "VRENAsim25 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VRENAsim25 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "NDEBUG"
# ADD RSC /l 0xc07 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib msimg32.lib comctl32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "VRENAsim25 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc07 /d "_DEBUG"
# ADD RSC /l 0xc07 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../../../framework/lib/WIN32/FATEsimD.lib wsock32.lib msimg32.lib comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/VRENAsim25Debug.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "VRENAsim25 - Win32 Release"
# Name "VRENAsim25 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\FPPTPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\FScreens.cpp
# End Source File
# Begin Source File

SOURCE=.\FVRena.cpp
# End Source File
# Begin Source File

SOURCE=.\FVRPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\Script1.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\FPPTPanel.h
# End Source File
# Begin Source File

SOURCE=.\FScreens.h
# End Source File
# Begin Source File

SOURCE=.\FVRena.h
# End Source File
# Begin Source File

SOURCE=.\FVRPanel.h
# End Source File
# Begin Source File

SOURCE=.\ids.h
# End Source File
# Begin Source File

SOURCE=.\ppt_protocol.h
# End Source File
# Begin Source File

SOURCE=.\protocol.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res_pics\active_left.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\active_mid.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\active_right.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\arsbox_icon.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\arsbox_pic.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\blanc_left.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\blanc_mid.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\blanc_right.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\button_3D.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\button_3D_p.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\button_black.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\button_black_p.bmp
# End Source File
# Begin Source File

SOURCE=.\button_folien.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\button_folien.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\button_next_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\button_next_s_p.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\button_pics.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\button_pics_p.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\button_ppt.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\button_ppt_p.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\button_prev_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\button_prev_s_p.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\button_video.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\button_video_p.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\button_vr.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\button_vr_p.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\button_white.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\button_white_p.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\current_slide.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\down_scroll.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\drop_arrow.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\frame0.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\frame1.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\frame10.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\frame2.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\frame3.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\frame4.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\frame5.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\frame6.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\frame7.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\frame8.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\frame9.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\intro.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\li_up.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\links.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\links_end.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\links_pr.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\mi_up.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\next_slide.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\pause.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\play.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\re_up.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\rechts.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\rechts_end.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\rechts_pr.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\reset.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\screens_back.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\set_left.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\set_mid.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\set_right.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\skip.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\skip_dis.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\skip_small.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\title.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\up_scroll.bmp
# End Source File
# Begin Source File

SOURCE=.\res_pics\vr_title.bmp
# End Source File
# End Group
# End Target
# End Project
