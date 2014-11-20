#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=Cygwin-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1959056617/houghmain.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=`pkg-config opencv --cflags` 
CXXFLAGS=`pkg-config opencv --cflags` 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`pkg-config --libs opencv`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/thresholdproject.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/thresholdproject.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/thresholdproject ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/1959056617/houghmain.o: /cygdrive/C/Users/radio.koza/Desktop/Sources/GitHub/nao-dice-project/thresholdProject/houghmain.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1959056617
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/cygwin/lib/gcc/x86_64-w64-mingw32/4.8.3/include/c++ -I/cygdrive/C/cygwin/lib/gcc/x86_64-w64-mingw32/4.8.3/include -I/cygdrive/C/cygwin/lib/gcc/x86_64-w64-mingw32/4.8.3/include/ssp `pkg-config --cflags opencv`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1959056617/houghmain.o /cygdrive/C/Users/radio.koza/Desktop/Sources/GitHub/nao-dice-project/thresholdProject/houghmain.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/cygdrive/C/cygwin/lib/gcc/x86_64-w64-mingw32/4.8.3/include/c++ -I/cygdrive/C/cygwin/lib/gcc/x86_64-w64-mingw32/4.8.3/include -I/cygdrive/C/cygwin/lib/gcc/x86_64-w64-mingw32/4.8.3/include/ssp `pkg-config --cflags opencv`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/thresholdproject.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
