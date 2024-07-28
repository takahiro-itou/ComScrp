# Microsoft Developer Studio Project File - Name="Cslib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Cslib - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "Cslib.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "Cslib.mak" CFG="Cslib - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Cslib - Win32 Release" ("Win32 (x86) Static Library" 用)
!MESSAGE "Cslib - Win32 Debug" ("Win32 (x86) Static Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Cslib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Cslib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\Cslibd.lib"

!ENDIF 

# Begin Target

# Name "Cslib - Win32 Release"
# Name "Cslib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Command.cpp
# End Source File
# Begin Source File

SOURCE=..\Compile.cpp
# End Source File
# Begin Source File

SOURCE=..\ComScrp.cpp
# End Source File
# Begin Source File

SOURCE=..\Expression.cpp
# End Source File
# Begin Source File

SOURCE=..\IfElse.cpp
# End Source File
# Begin Source File

SOURCE=..\Lexer.cpp
# End Source File
# Begin Source File

SOURCE=..\Player.cpp
# End Source File
# Begin Source File

SOURCE=..\ReadText.cpp
# End Source File
# Begin Source File

SOURCE=..\Script.cpp

!IF  "$(CFG)" == "Cslib - Win32 Release"

!ELSEIF  "$(CFG)" == "Cslib - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\StdAfx.cpp

!IF  "$(CFG)" == "Cslib - Win32 Release"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Cslib - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Stream.cpp
# End Source File
# Begin Source File

SOURCE=..\Syntax.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Command.h
# End Source File
# Begin Source File

SOURCE=..\Compile.h
# End Source File
# Begin Source File

SOURCE=..\ComScrp.h
# End Source File
# Begin Source File

SOURCE=..\Def.h
# End Source File
# Begin Source File

SOURCE=..\Errors.h
# End Source File
# Begin Source File

SOURCE=..\Expression.h
# End Source File
# Begin Source File

SOURCE=..\IfElse.h
# End Source File
# Begin Source File

SOURCE=..\Lexer.h
# End Source File
# Begin Source File

SOURCE=..\Macro.h
# End Source File
# Begin Source File

SOURCE=..\Player.h
# End Source File
# Begin Source File

SOURCE=..\Script.h
# End Source File
# Begin Source File

SOURCE=..\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Stream.h
# End Source File
# Begin Source File

SOURCE=..\Syntax.h
# End Source File
# Begin Source File

SOURCE=..\Types.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
