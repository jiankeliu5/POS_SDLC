# Microsoft Developer Studio Project File - Name="sdlc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=sdlc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "sdlc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sdlc.mak" CFG="sdlc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sdlc - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir ""
# PROP BASE Cmd_Line "NMAKE /f sdlc.MAK"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "sdlc.EXE"
# PROP BASE Bsc_Name "sdlc.BSC"
# PROP BASE Target_Dir ""
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Cmd_Line "NMAKE /f sdlc.smk"
# PROP Rebuild_Opt "/a"
# PROP Target_File "sdlc.EXE"
# PROP Bsc_Name "sdlc.BSC"
# PROP Target_Dir ""
# Begin Target

# Name "sdlc - Win32 Debug"

!IF  "$(CFG)" == "sdlc - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Group "app"

# PROP Default_Filter ""
# Begin Group "inc_1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\SOURCE\app\inc\var_types.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\SOURCE\App\aplic.c
# End Source File
# End Group
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Group "inc_2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\SOURCE\lib\Inc\app_sys.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\SOURCE\lib\app_sys.c
# End Source File
# End Group
# Begin Group "porting"

# PROP Default_Filter ""
# Begin Group "inc_3"

# PROP Default_Filter ""
# End Group
# End Group
# Begin Group "Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Output\Download\Download.BAT
# End Source File
# Begin Source File

SOURCE=..\..\Output\Download\sdlc.dld
# End Source File
# Begin Source File

SOURCE=.\SDLC.fst
# End Source File
# Begin Source File

SOURCE=.\sdlc.smk
# End Source File
# End Group
# End Group
# End Target
# End Project
