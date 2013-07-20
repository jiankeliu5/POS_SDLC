#
# Paths
#

# [Vx520 - SDK Verix Evo ]
RVCTDIR=C:\Arquivos de programas\ARM\RVCT\Programs\4.0\436\multi1\win_32-pentium
RVCTBIN=C:\Arquivos de programas\ARM\RVCT\Programs\4.0\436\multi1\win_32-pentium

VRXSDK=C:\VerixVAps\VRXSDK\VRXSDK_2.5.0_SK

ACT2000  = $(VACT2000)\ACT2000_2.20.0.1_SK
VREOSSDK = $(VEOSSDK)\EOSSDK_9_SK
VFSDK    = $(VRXSDK)

ADSOUT=$(ACT2000)\output\rv
ADSSAM=C:\Projetos\SDLC_SAMPLE

# SDS Tools Related Paths
SDSIncludes = $(VRXSDK)\include
ACTIncludes = $(ACT2000)\include
EOSIncludes = $(VEOSSDK_EVO)\include

# Library Paths
ACTLibraries = $(ADSOUT)\files\Static\Release
EOSLibrary = $(VEOSSDK_EVO)\lib

#  App Source Paths
SrcDir = $(ADSSAM)\Vx510_SDLC\SOURCE
HdwDir = $(SrcDir)

APPIncludes = $(SrcDir)\App\inc -I$(SrcDir)\lib\inc
HDWIncludes = $(HdwDir)\Porting\inc

#  Compiler/Linker/Outhdr Output Paths
ObjDir = $(ADSSAM)\Vx520_SDLC\Obj
OutDir = $(ADSSAM)\Vx520_SDLC\output\files\debug

# Compiler Options
Includes = -I$(SDSIncludes) -I$(ACTIncludes) -I$(EOSIncludes)  -I$(APPIncludes) -I$(HDWIncludes)

COptions = -b -DVx520 -DDEBUG_H

#
# Dependencies
#

SDLCAppObjects = \
		$(ObjDir)\aplic.o \
		$(ObjDir)\app_sys.o \
		$(ObjDir)\porting.o \

Libs =						\
$(ACTLibraries)\act2000.a \
$(VRXSDK)\lib\voyn.lib	\

#  sample Target Definition

pseudoOut : $(OutDir)\sdlc.out 
	$(VRXSDK)\bin\vrxhdr -s 131072 -h 6144 $(OutDir)\sdlc.out

$(OutDir)\sdlc.out : $(SDLCAppObjects)
  $(VRXSDK)\bin\vrxcc $(SDLCAppObjects) $(Libs) -b -o sdlc.out
  move sdlc.out  $(OutDir)\sdlc.out


######  Compile #######

### APLICACAO ###

$(ObjDir)\aplic.o : $(SrcDir)\app\aplic.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\app\aplic.c
  move aplic.o  $(ObjDir)\aplic.o

### LIB ###

$(ObjDir)\app_sys.o : $(SrcDir)\lib\app_sys.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\lib\app_sys.c
  move app_sys.o  $(ObjDir)\app_sys.o

### PORTING ###

$(ObjDir)\porting.o : $(SrcDir)\porting\porting.c 
  $(VRXSDK)\bin\vrxcc -c $(Includes)  $(COptions) $(SrcDir)\porting\porting.c
  move porting.o  $(ObjDir)\porting.o
