#
# Paths
#

VRXSDK=C:\VerixVAps\VRXSDK\VRXSDK_2.1.2

ACT2000 = $(VACT2000)\ACT2000_2.2.0.02
VFSDK   = $(VRXSDK)
VRTCP	= $(VTCPIP)\TCPIP_2.4.0.3
VRUCL	= $(VUCL)\UCL_2.5.0.5

ADSOUT=$(ACT2000)\output\rv
ADSSAM=C:\Projetos\SDLC_SAMPLE

# SDS Tools Related Paths
sdsTools    = $(SDS)\cmd
SDSIncludes = $(VFSDK)\include
ACTLibraries = $(ADSOUT)\files\static\debug
ACTIncludes = $(ACT2000)\include

#  App Source Paths
SrcDir = $(ADSSAM)\Vx510_SDLC\SOURCE
HdwDir = $(SrcDir)

APPIncludes = $(SrcDir)\App\inc -I$(SrcDir)\lib\inc
HDWIncludes = $(HdwDir)\Porting\inc

#  Compiler/Linker/Outhdr Output Paths
ObjDir = $(ADSSAM)\Vx510_SDLC\Obj
OutDir = $(ADSSAM)\Vx510_SDLC\output\files\debug

# Options for Tools
#

# Compiler Options
Includes = -I $(SDSIncludes)  -I$(ACTIncludes)  -I$(APPIncludes)  -I$(HDWIncludes)

COptions = -DVx510 -DDEBUG_H

#
# Dependencies
#

SDLCAppObjects = \
		$(ObjDir)\aplic.o \
		$(ObjDir)\app_sys.o \
		$(ObjDir)\porting.o \

Libs =						\
$(ACTLibraries)\act2000.a \
$(VRXSDK)\lib\voy.lib	\

#  sample Target Definition
#

pseudoOut : $(OutDir)\sdlc.out 
    $(VRXSDK)\bin\vrxhdr -s 500000 -h 300000 $(OutDir)\sdlc.out


#   Use VeriShield for FileSigning
	"$(VSFSTOOL)\filesignature" .\sdlc.fst -nogui		
	copy $(OutDir)\sdlc.out.p7s $(OutDir)\signfile\flash\sdlc.out.p7s
	del $(OutDir)\sdlc.out.p7s

$(OutDir)\sdlc.out : $(SDLCAppObjects)
  $(VRXSDK)\bin\vrxcc $(SDLCAppObjects) $(Libs) $(COptions) -symbols -o sdlc.out
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