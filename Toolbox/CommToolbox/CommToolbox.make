#
#	File:		CommToolbox.make
#
#	Contains:	Makefile for the CommToolbox.
#
#	Written by:	Kurt Clark, Chas Spillar, and Tim Nichols
#
#	Copyright:	� 1992 by Apple Computer, Inc., all rights reserved.
#
#	Change History (most recent first):
#
#	   <SM3>	12/11/92	chp		Removed runtime library routines not needed when compiled for
#									68020 or better.
#	   <SM2>	 12/2/92	kc		Added " || Exit 1" to commands with a double dependency.


CommTerminalDir		=	{CommToolboxDir}TerminalMgr:
CommResourceDir		=	{CommToolboxDir}CommResourceMgr:
CommStartupDir		=	{CommToolboxDir}CommToolboxStartup:
CommUtilityDir		=	{CommToolboxDir}CommToolboxUtilities:
CommConnectionDir	=	{CommToolboxDir}ConnectionMgr:
CommFileTransferDir	=	{CommToolboxDir}FileTransferMgr:
CommNBPDir			=	{CommUtilityDir}StandardNBP:
CommChooseDir		=	{CommUtilityDir}Choose:


ConnectionMgrObjs		=	"{ObjDir}ConnectionMgr.a.o"							�
							"{ObjDir}ConnectionMgr.c.o"							�
							"{ObjDir}CommToolboxUtilityRoutines.c.o"			�
							"{IfObjDir}interface.o"


TerminalMgrObjs			=	"{ObjDir}TerminalMgr.a.o"							�
							"{ObjDir}TerminalMgr.c.o"							�
							"{ObjDir}CommToolboxUtilityRoutines.c.o"			�
							"{IfObjDir}interface.o"								


FileTransferMgrObjs		=	"{ObjDir}FileTransferMgr.a.o"						�
							"{ObjDir}FileTransferMgr.c.o"						�
							"{ObjDir}CommToolboxUtilityRoutines.c.o"			�
							"{IfObjDir}interface.o"								


CommToolboxUtilitiesObjs =	"{ObjDir}CommToolboxUtilities.a.o"					�
							"{ObjDir}CommToolboxUtilities.c.o"					�
							"{ObjDir}CommToolboxCore.c.o"						�
							"{ObjDir}CommToolboxUtilityRoutines.c.o"			�
							"{IfObjDir}interface.o"								


CommResourceMgrObjs		=	"{ObjDir}CommResourceMgr.a.o"						�
							"{ObjDir}CommResourceMgr.c.o"						�
							"{ObjDir}CommResourceMgr.p.o"						�
							"{ObjDir}CommResourceMgrUtilities.a.o"				�
							"{IfObjDir}interface.o"								


DITLObjs				=	"{ObjDir}DITL.a.o"									�
							"{ObjDir}DITL.p.o"									�
							"{IfObjDir}interface.o"								


CommToolboxLDEFObjs		=	"{ObjDir}CommToolboxLDEF.a.o"						�
							"{ObjDir}CommToolboxLDEF.p.o"						�
							"{ObjDir}CommToolboxUtilityRoutines.c.o"			�
							"{IfObjDir}interface.o"								

ChooseObjs				=	"{ObjDir}Choose.a.o"								�
							"{ObjDir}Choose.p.o"								�
							"{ObjDir}ChooseUtilities.a.o"						�
							"{ObjDir}CommToolboxUtilityRoutines.c.o"			�
							"{IfObjDir}interface.o"								


StandardNBPObjs			=	"{ObjDir}StandardNBP.a.o"							�
							"{ObjDir}StandardNBP.p.o"							�
							"{ObjDir}ListUtilities.p.o"							�
							"{ObjDir}StandardNBPUtilities.a.o"					�
							"{ObjDir}ZIPUtilities.p.o"							�
							"{ObjDir}NBPUtilities.p.o"							�
							"{ObjDir}CommToolboxUtilityRoutines.c.o"			�
							"{IfObjDir}interface.o"								


StandardNBPLDEFObjs		=	"{ObjDir}StandardNBPLDEF.a.o"						�
							"{ObjDir}StandardNBPLDEF.p.o"						�
							"{ObjDir}CommToolboxUtilityRoutines.c.o"			�
							"{IfObjDir}interface.o"								


CommToolboxPatchObjs	=	"{ObjDir}CommToolboxDispatcher.a.o"					�
							"{ObjDir}CommToolboxPatches.a.o"					�
							"{ObjDir}CommToolboxPatches.c.o"					�
							"{ObjDir}CRMBuiltInSerial.c.o"



"{LibDir}CommToolboxPatch.Lib"	�	{CommToolboxPatchObjs}
	Lib {StdLibOpts} -o "{Targ}" {CommToolboxPatchObjs} 


"{RsrcDir}CommToolbox.rsrc"		��	{ConnectionMgrObjs}
	Link {StdLOpts} {StdAlign} -o "{Targ}" -rt cmtb=-32512 {ConnectionMgrObjs} || Exit 1


"{RsrcDir}CommToolbox.rsrc"		��	{TerminalMgrObjs}
	Link {StdLOpts} {StdAlign} -o "{Targ}" -rt cmtb=-32510 {TerminalMgrObjs} || Exit 1


"{RsrcDir}CommToolbox.rsrc"		��	{FileTransferMgrObjs}
	Link {StdLOpts} {StdAlign} -o "{Targ}" -rt cmtb=-32511 {FileTransferMgrObjs} || Exit 1


"{RsrcDir}CommToolbox.rsrc"		��	{CommToolboxUtilitiesObjs}
	Link {StdLOpts} {StdAlign} -o "{Targ}" -rt cmtb=-32509 {CommToolboxUtilitiesObjs} || Exit 1


"{RsrcDir}CommToolbox.rsrc"		��	{CommResourceMgrObjs}
	Link {StdLOpts} {StdAlign} -o "{Targ}" -rt cmtb=-32508 {CommResourceMgrObjs} || Exit 1


"{RsrcDir}CommToolbox.rsrc"		��	{DITLObjs}
	Link {StdLOpts} {StdAlign} -o "{Targ}" -rt proc=-32511 {DITLObjs} || Exit 1


"{RsrcDir}CommToolbox.rsrc"		��	{CommToolboxLDEFObjs}
	Link {StdLOpts} {StdAlign} -o "{Targ}" -rt LDEF=-32512 {CommToolboxLDEFObjs} || Exit 1


"{RsrcDir}CommToolbox.rsrc"		��	{ChooseObjs}
	Link {StdLOpts} {StdAlign} -o "{Targ}" -rt proc=-32512 {ChooseObjs} || Exit 1


"{RsrcDir}CommToolbox.rsrc"		��	{StandardNBPObjs}
	Link {StdLOpts} {StdAlign} -o "{Targ}" -rt proc=-32510 -m MAIN {StandardNBPObjs} || Exit 1


"{RsrcDir}CommToolbox.rsrc"		��	{StandardNBPLDEFObjs}
	Link {StdLOpts} {StdAlign} -o "{Targ}" -rt LDEF=-32510 {StandardNBPLDEFObjs} || Exit 1


"{RsrcDir}CommToolbox.rsrc"		��	"{CommTerminalDir}TerminalClick.r"				�
									"{RIncludes}SysTypes.r"						�
									"{IntRIncludes}CommToolboxPriv.r"
	Rez -a {StdROpts} -o "{Targ}" "{CommTerminalDir}TerminalClick.r"  || Exit 1


"{RsrcDir}CommToolbox.rsrc"		��	"{CommChooseDir}Choose.r"				�
									"{RIncludes}SysTypes.r"						�
									"{RIncludes}Types.r"						�
									"{RIncludes}CTBTypes.r"						�
									"{IntRIncludes}CommToolboxPriv.r"
	Rez -a {StdROpts} -o "{Targ}" "{CommChooseDir}Choose.r" || Exit 1


"{RsrcDir}CommToolbox.rsrc"		��	"{CommNBPDir}StandardNBP.r"				�
									"{RIncludes}Types.r"						�
									"{RIncludes}CTBTypes.r"						�
									"{RIncludes}PICT.r"							�
									"{IntRIncludes}CommToolboxPriv.r"
	Rez -a {StdROpts} -o "{Targ}" "{CommNBPDir}StandardNBP.r" || Exit 1


"{ObjDir}CommToolboxUtilityRoutines.c.o"	�	"{CIncludes}Menus.h"								�
												"{CIncludes}Dialogs.h"								�
												"{CIncludes}QuickDraw.h"							�
												"{CIncludes}Fonts.h"								�
												"{CIncludes}SysEqu.h"								�
												"{CIncludes}CTBUtilities.h"							�
												"{CIncludes}CommResources.h"						�
												"{IntCIncludes}CommToolboxPriv.h"					�
												"{CommUtilityDir}CommToolboxUtilityRoutines.c"
	C {StdCOpts} -o "{Targ}" "{CommUtilityDir}CommToolboxUtilityRoutines.c"


"{ObjDir}CommResourceMgr.a.o"				�	"{AIncludes}traps.a"								�
												"{AIncludes}CommResources.a"						�
												"{CommResourceDir}CommResourceMgr.a"
	Asm {StdAOpts} -o "{targ}" "{CommResourceDir}CommResourceMgr.a"


"{ObjDir}CommResourceMgr.c.o"				�	"{CIncludes}SysEqu.h"								�
												"{CIncludes}Types.h"								�
												"{CIncludes}Memory.h"								�
												"{CIncludes}Resources.h"							�
												"{CIncludes}OSUtils.h"								�
												"{CIncludes}Errors.h"								�
												"{CIncludes}Folders.h"								�
												"{CIncludes}CommResources.h"						�
												"{CIncludes}Folders.h"								�
												"{CIncludes}Files.h"								�
												"{IntCIncludes}CommToolboxPriv.h"					�
												"{CommResourceDir}CommResourceMgrExtensions.c"		�
												"{CommResourceDir}CommResourceMgr.h"				�
												"{CommResourceDir}CommResourceMgr.c"
	C {StdCOpts} -o "{targ}" "{CommResourceDir}CommResourceMgr.c"  


"{ObjDir}CommResourceMgr.p.o"				�	"{PInterfaces}MemTypes.p"							�
												"{PInterfaces}Types.p"								�
												"{PInterfaces}Quickdraw.p"							�
												"{PInterfaces}OSIntf.p"								�
												"{PInterfaces}ToolIntf.p"							�
												"{PInterfaces}PackIntf.p"							�
												"{PInterfaces}SysEqu.p"								�
												"{PInterfaces}Appletalk.p"							�
												"{PInterfaces}CTBUtilities.p"						�
												"{IntPInterfaces}CommToolboxPriv.p"					�
												"{CommResourceDir}CommResourceMgr.p"
	Pascal {StdPOpts} -o "{targ}" "{CommResourceDir}CommResourceMgr.p"  


"{ObjDir}CommResourceMgrUtilities.a.o"		�	"{AIncludes}SysEqu.a"								�
												"{IntAIncludes}SysPrivateEqu.a"						�
												"{AIncludes}SysErr.a"								�
												"{AIncludes}ToolUtils.a"							�
												"{AIncludes}Traps.a"								�
												"{AIncludes}Aliases.a"								�
												"{IntAIncludes}CommToolboxPriv.a"					�
												"{CommResourceDir}CommResourceMgrUtilities.a"
	Asm {StdAOpts} -o "{targ}" "{CommResourceDir}CommResourceMgrUtilities.a"


"{ObjDir}CommToolboxUtilities.a.o"			�	"{AIncludes}traps.a"								�
												"{AIncludes}CTBUtilities.a"							�
												"{IntAIncludes}CommToolboxPriv.a"					�
												"{CommUtilityDir}CommToolboxUtilities.a"
	Asm {StdAOpts} -o "{targ}" "{CommUtilityDir}CommToolboxUtilities.a"

"{ObjDir}CommToolboxUtilities.c.o"			�	"{CIncludes}Memory.h"								�
												"{CIncludes}Resources.h"							�
												"{CIncludes}Dialogs.h"								�
												"{CIncludes}Events.h"								�
												"{CIncludes}AppleTalk.h"							�
												"{CIncludes}CommResources.h"						�
												"{CIncludes}CTBUtilities.h"							�
												"{IntCIncludes}CommToolboxPriv.h"					�
												"{CommUtilityDir}CommToolboxUtilities.c"
	C {StdCOpts} -o "{targ}" "{CommUtilityDir}CommToolboxUtilities.c"  

"{ObjDir}CommToolboxCore.c.o"				�	"{IntCIncludes}CommToolboxPriv.h"					�
												"{CIncludes}CommResources.h"						�
												"{CIncludes}CTBUtilities.h"							�
												"{IntCIncludes}ConnectionsExtensions.h"				�
												"{CIncludes}ConnectionTools.h"						�
												"{CommUtilityDir}CommToolboxCore.c"
	C {StdCOpts} -o "{targ}" "{CommUtilityDir}CommToolboxCore.c"  


"{ObjDir}ChooseUtilities.a.o"				� "{CommChooseDir}ChooseUtilities.a"
	Asm {StdAOpts} -o "{targ}" "{CommChooseDir}ChooseUtilities.a"


"{ObjDir}Choose.a.o"						�	"{IntAIncludes}CommToolboxPriv.a"					�
												"{CommChooseDir}Choose.a"
	Asm {StdAOpts} -o "{targ}" "{CommChooseDir}Choose.a"


"{ObjDir}Choose.p.o"						�	"{PInterfaces}MemTypes.p"							�
												"{PInterfaces}QuickDraw.p"							�
												"{PInterfaces}OSIntf.p"								�
												"{PInterfaces}ToolIntf.p"							�
												"{PInterfaces}PackIntf.p"							�
												"{PInterfaces}AppleTalk.p"							�
												"{PInterfaces}SysEqu.p"								�
												"{PInterfaces}Files.p"								�
												"{PInterfaces}Aliases.p"							�
												"{PInterfaces}Terminals.p"							�
												"{PInterfaces}CommResources.p"						�
												"{PInterfaces}Connections.p"						�
												"{PInterfaces}FileTransfers.p"						�
												"{PInterfaces}CTBUtilities.p"						�
												"{IntPInterfaces}DialogsPriv.p"						�
												"{IntPInterfaces}CommToolboxPriv.p"					�
												"{CommChooseDir}Choose.p"
	Pascal {StdPOpts} -o "{targ}" "{CommChooseDir}Choose.p"  


"{ObjDir}StandardNBP.a.o"					�	"{CommNBPDir}StandardNBP.a"						�
												"{IntAIncludes}CommToolboxPriv.a"
	Asm {StdAOpts} -o "{targ}" "{CommNBPDir}StandardNBP.a"


"{ObjDir}StandardNBP.p.o"					�	"{CommNBPDir}StandardNBP.p"						�
												"{CommNBPDir}StandardNBPStructures.p"				�
												"{CommNBPDir}ListUtilities.p"						�
												"{CommNBPDir}NBPUtilities.p"						�
												"{CommNBPDir}ZipUtilities.p"						�
												"{PInterfaces}MemTypes.p"							�
												"{PInterfaces}QuickDraw.p"							�
												"{PInterfaces}OSIntf.p"								�
												"{PInterfaces}ToolIntf.p"							�
												"{PInterfaces}PackIntf.p"							�
												"{PInterfaces}AppleTalk.p"							�
												"{PInterfaces}Script.p"								�
												"{PInterfaces}Traps.p"								�
												"{PInterfaces}CTBUtilities.p"						�
												"{IntPInterfaces}DialogsPriv.p"						�
												"{IntPInterfaces}IntlUtilsPriv.p"					�
												"{IntPInterfaces}CommToolboxPriv.p"
	Pascal {StdPOpts} -o "{targ}" "{CommNBPDir}StandardNBP.p"  


"{ObjDir}ListUtilities.p.o"					�	"{CommNBPDir}ListUtilities.p"						�
												"{CommNBPDir}StandardNBPStructures.p"				�
												"{PInterfaces}MemTypes.p"							�
												"{PInterfaces}QuickDraw.p"							�
												"{PInterfaces}OSIntf.p"								�
												"{PInterfaces}ToolIntf.p"							�
												"{PInterfaces}PackIntf.p"							�
												"{PInterfaces}AppleTalk.p"							�
												"{PInterfaces}CTBUtilities.p"						�
												"{IntPInterfaces}IntlUtilsPriv.p"					�
												"{IntPInterfaces}CommToolboxPriv.p"
	Pascal {StdPOpts} -o "{targ}" "{CommNBPDir}ListUtilities.p"  


"{ObjDir}ZIPUtilities.p.o"					�	"{CommNBPDir}ZIPUtilities.p"						�
												"{CommNBPDir}StandardNBPStructures.p"				�
												"{CommNBPDir}ListUtilities.p"						�
												"{PInterfaces}MemTypes.p"							�
												"{PInterfaces}QuickDraw.p"							�
												"{PInterfaces}OSIntf.p"								�
												"{PInterfaces}ToolIntf.p"							�
												"{PInterfaces}PackIntf.p"							�
												"{PInterfaces}AppleTalk.p"							�
												"{PInterfaces}CTBUtilities.p"						�
												"{IntPInterfaces}IntlUtilsPriv.p"					�
												"{IntPInterfaces}CommToolboxPriv.p"
	Pascal {StdPOpts} -o "{targ}" "{CommNBPDir}ZIPUtilities.p"  


"{ObjDir}NBPUtilities.p.o"					�	"{CommNBPDir}NBPUtilities.p"						�
												"{CommNBPDir}StandardNBPStructures.p"				�
												"{CommNBPDir}ListUtilities.p"						�
												"{PInterfaces}MemTypes.p"							�
												"{PInterfaces}QuickDraw.p"							�
												"{PInterfaces}OSIntf.p"								�
												"{PInterfaces}ToolIntf.p"							�
												"{PInterfaces}PackIntf.p"							�
												"{PInterfaces}AppleTalk.p"							�
												"{PInterfaces}CTBUtilities.p"						�
												"{IntPInterfaces}IntlUtilsPriv.p"					�
												"{IntPInterfaces}CommToolboxPriv.p"
	Pascal {StdPOpts} -o "{targ}" "{CommNBPDir}NBPUtilities.p"  


"{ObjDir}StandardNBPUtilities.a.o"			� "{CommNBPDir}StandardNBPUtilities.a"
	Asm {StdAOpts} -o "{targ}" "{CommNBPDir}StandardNBPUtilities.a"


"{ObjDir}StandardNBPLDEF.a.o"				�	"{CommNBPDir}StandardNBPLDEF.a"					�
												"{IntAIncludes}CommToolboxPriv.a"
	Asm {StdAOpts} -o "{Targ}" "{CommNBPDir}StandardNBPLDEF.a"


"{ObjDir}StandardNBPLDEF.p.o"				�	"{CommNBPDir}StandardNBPLDEF.p" 					�
												"{PInterfaces}MemTypes.p"							�
												"{PInterfaces}QuickDraw.p"							�
												"{PInterfaces}OSIntf.p"								�
												"{PInterfaces}ToolIntf.p"							�
												"{PInterfaces}PackIntf.p"							�
												"{PInterfaces}AppleTalk.p"							�
												"{PInterfaces}Script.p"								�
												"{PInterfaces}Traps.p"								�
												"{PInterfaces}SysEqu.p"								�
												"{PInterfaces}CTBUtilities.p"						�
												"{IntPInterfaces}CommToolboxPriv.p"					�
												"{IntPInterfaces}IntlUtilsPriv.p"					�
												"{CommNBPDir}StandardNBPStructures.p"
	Pascal {StdPOpts} -o "{targ}" "{CommNBPDir}StandardNBPLDEF.p"  


"{ObjDir}CommToolboxLDEF.a.o"				�	"{CommUtilityDir}CommToolboxLDEF.a"			�
												"{IntAIncludes}CommToolboxPriv.a"
	Asm {StdAOpts} -o "{targ}" "{CommUtilityDir}CommToolboxLDEF.a"


"{ObjDir}CommToolboxLDEF.p.o"				�	"{CommUtilityDir}CommToolboxLDEF.p" 			�
												"{PInterfaces}MemTypes.p"							�
												"{PInterfaces}QuickDraw.p"							�
												"{PInterfaces}OSIntf.p"								�
												"{PInterfaces}ToolIntf.p"							�
												"{PInterfaces}PackIntf.p"							�
												"{PInterfaces}AppleTalk.p"							�
												"{PInterfaces}Script.p"								�
												"{PInterfaces}Traps.p"								�
												"{PInterfaces}SysEqu.p"								�
												"{IntPInterfaces}CommToolboxPriv.p"
	Pascal {StdPOpts} -o "{targ}" "{CommUtilityDir}CommToolboxLDEF.p"  


"{ObjDir}DITL.a.o"							�	"{CommUtilityDir}DITL.a"						�
												"{IntAIncludes}CommToolboxPriv.a"
	Asm {StdAOpts} -o "{targ}" "{CommUtilityDir}DITL.a"


"{ObjDir}DITL.p.o"							�	"{CommUtilityDir}DITL.p" 						�
												"{PInterfaces}MemTypes.p"							�
												"{PInterfaces}QuickDraw.p"							�
												"{PInterfaces}OSIntf.p"								�
												"{PInterfaces}ToolIntf.p"							�
												"{PInterfaces}PackIntf.p"							�
												"{PInterfaces}AppleTalk.p"							�
												"{PInterfaces}CTBUtilities.p"						�
												"{IntPInterfaces}CommToolboxPriv.p"
	Pascal {StdPOpts} -o "{targ}" "{CommUtilityDir}DITL.p"  


"{ObjDir}ConnectionMgr.a.o"					�	"{CommConnectionDir}ConnectionMgr.a"					�
												"{AIncludes}Connections.a"							�
												"{AIncludes}CTBUtilities.a"							�
												"{AIncludes}ConnectionTools.a"						�
												"{AIncludes}traps.a"								�
												"{IntAIncludes}CommToolboxPriv.a"
	Asm {StdAOpts} -o "{targ}" "{CommConnectionDir}ConnectionMgr.a"


"{ObjDir}ConnectionMgr.c.o"					�	"{CommConnectionDir}ConnectionMgr.c"					�
												"{CIncludes}Errors.h"								�
												"{CIncludes}Memory.h"								�
												"{CIncludes}Resources.h"							�
												"{CIncludes}OSUtils.h"								�
												"{CIncludes}SysEqu.h"								�
												"{CIncludes}CTBUtilities.h"							�
												"{CIncludes}CommResources.h"						�
												"{CIncludes}Connections.h"							�
												"{CIncludes}ConnectionTools.h"						�
												"{IntCIncludes}ConnectionsExtensions.h"				�
												"{IntCIncludes}ConnectionsPriv.h"					�
												"{IntCIncludes}CommToolboxPriv.h"					�
												"{CommConnectionDir}ConnectionMgrUtilities.c"			�
												"{CommConnectionDir}ConnectionMgrExtensions.c"
	C {StdCOpts} -o "{targ}" "{CommConnectionDir}ConnectionMgr.c"  


"{ObjDir}TerminalMgr.a.o"					�	"{CommTerminalDir}TerminalMgr.a"						�
												"{AIncludes}Terminals.a"							�
												"{AIncludes}traps.a"								�
												"{IntAIncludes}CommToolboxPriv.a"
	Asm {StdAOpts} -o "{targ}" "{CommTerminalDir}TerminalMgr.a"


"{ObjDir}TerminalMgr.c.o"					�	"{CommTerminalDir}TerminalMgr.c"					�
												"{CIncludes}Memory.h"								�
												"{CIncludes}Resources.h"							�
												"{CIncludes}ToolUtils.h"							�
												"{CIncludes}SysEqu.h"								�
												"{CIncludes}CTBUtilities.h"							�
												"{CIncludes}CommResources.h"						�
												"{CIncludes}Terminals.h"							�
												"{CIncludes}TerminalTools.h"						�
												"{IntCIncludes}TerminalsPriv.h"						�
												"{IntCIncludes}TerminalToolsPriv.h"					�
												"{IntCIncludes}CommToolboxPriv.h"					�
												"{CommTerminalDir}TerminalMgrUtilities.c"			�
												"{CommTerminalDir}TerminalMgrExtensions.c"
	C {StdCOpts} -o "{targ}" "{CommTerminalDir}TerminalMgr.c"  


"{ObjDir}FileTransferMgr.a.o"				�	"{CommFileTransferDir}FileTransferMgr.a"				�
												"{AIncludes}FileTransfers.a"						�
												"{AIncludes}CTBUtilities.a"							�
												"{AIncludes}traps.a"								�
												"{IntAIncludes}CommToolboxPriv.a"
	Asm {StdAOpts} -o "{targ}" "{CommFileTransferDir}FileTransferMgr.a"


"{ObjDir}FileTransferMgr.c.o"				�	"{CommFileTransferDir}FileTransferMgr.c"				�
												"{CIncludes}Memory.h"								�
												"{CIncludes}Resources.h"							�
												"{CIncludes}Types.h"								�
												"{CIncludes}SysEqu.h"								�
												"{CIncludes}CTBUtilities.h"							�
												"{CIncludes}CommResources.h"						�
												"{CIncludes}FileTransfers.h"						�
												"{CIncludes}FileTransferTools.h"					�
												"{IntCIncludes}FileTransferToolsPriv.h"				�
												"{IntCIncludes}CommToolboxPriv.h"					�
												"{CommFileTransferDir}FileTransferMgrUtilities.c"		�
												"{CommFileTransferDir}FileTransferMgrExtensions.c"
	C {StdCOpts} -o "{targ}" "{CommFileTransferDir}FileTransferMgr.c"  


"{ObjDir}CommToolboxDispatcher.a.o"			�	"{CommStartupDir}CommToolboxDispatcher.a"			�
												"{AIncludes}Traps.a"								�
												"{AIncludes}ToolUtils.a"							�
												"{AIncludes}SysEqu.a"								�
												"{IntAIncludes}SysPrivateEqu.a"						�
												"{AIncludes}CommResources.a"						�
												"{IntAIncludes}CommToolboxPriv.a"
	Asm {StdAOpts} -o "{targ}" "{CommStartupDir}CommToolboxDispatcher.a"


"{ObjDir}CRMBuiltInSerial.c.o"				�	"{CommStartupDir}CRMBuiltInSerial.c"				�
												"{CIncludes}Memory.h"								�
												"{CIncludes}Resources.h"							�
												"{CIncludes}ToolUtils.h"							�
												"{CIncludes}OSUtils.h"								�
												"{CIncludes}CommResources.h"						�
												"{CIncludes}CRMSerialDevices.h"						�
												"{IntCIncludes}CommToolboxPriv.h"					�
												"{IntCIncludes}IconUtilsPriv.h"
	C {StdCOpts} -o "{targ}" "{CommStartupDir}CRMBuiltInSerial.c"  


"{ObjDir}CommToolboxPatches.c.o"			�	"{CommStartupDir}CommToolboxPatches.c"				�
												"{IntCIncludes}CommToolboxPriv.h"					�
												"{CommResourceDir}CommResourceMgr.h"
	C {StdCOpts} -o "{Targ}" "{CommStartupDir}CommToolboxPatches.c" -i "{CommResourceDir}" 


"{ObjDir}CommToolboxPatches.a.o"			� "{CommStartupDir}CommToolboxPatches.a"
	Asm {StdAOpts} -o "{targ}" "{CommStartupDir}CommToolboxPatches.a"
