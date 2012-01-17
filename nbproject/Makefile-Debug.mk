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
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/gtk+2.x_tab_page_credits.o \
	${OBJECTDIR}/gtk+2.x.o \
	${OBJECTDIR}/setxkbmap_interface.o \
	${OBJECTDIR}/manage_xorg_rules.o \
	${OBJECTDIR}/RESOURCES/xkblib_interface.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/manage_user_preferences.o \
	${OBJECTDIR}/gtk+2.x_tab_page_distribution.o \
	${OBJECTDIR}/manage_xorg_preferences.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/lib/libgtk2.0-0 `pkg-config --libs gtk+-2.0` `pkg-config --libs libxml-2.0` `pkg-config --libs gconf-2.0`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lxkb-config

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lxkb-config: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lxkb-config ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/gtk+2.x_tab_page_credits.o: nbproject/Makefile-${CND_CONF}.mk gtk+2.x_tab_page_credits.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) `pkg-config --cflags gtk+-2.0` `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/gtk+2.x_tab_page_credits.o gtk+2.x_tab_page_credits.c

${OBJECTDIR}/gtk+2.x.o: nbproject/Makefile-${CND_CONF}.mk gtk+2.x.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) `pkg-config --cflags gtk+-2.0` `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/gtk+2.x.o gtk+2.x.c

${OBJECTDIR}/setxkbmap_interface.o: nbproject/Makefile-${CND_CONF}.mk setxkbmap_interface.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) `pkg-config --cflags gtk+-2.0` `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/setxkbmap_interface.o setxkbmap_interface.c

${OBJECTDIR}/manage_xorg_rules.o: nbproject/Makefile-${CND_CONF}.mk manage_xorg_rules.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) `pkg-config --cflags gtk+-2.0` `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/manage_xorg_rules.o manage_xorg_rules.c

${OBJECTDIR}/RESOURCES/xkblib_interface.o: nbproject/Makefile-${CND_CONF}.mk RESOURCES/xkblib_interface.c 
	${MKDIR} -p ${OBJECTDIR}/RESOURCES
	${RM} $@.d
	$(COMPILE.c) `pkg-config --cflags gtk+-2.0` `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/RESOURCES/xkblib_interface.o RESOURCES/xkblib_interface.c

${OBJECTDIR}/main.o: nbproject/Makefile-${CND_CONF}.mk main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) `pkg-config --cflags gtk+-2.0` `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/manage_user_preferences.o: nbproject/Makefile-${CND_CONF}.mk manage_user_preferences.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) `pkg-config --cflags gtk+-2.0` `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/manage_user_preferences.o manage_user_preferences.c

${OBJECTDIR}/gtk+2.x_tab_page_distribution.o: nbproject/Makefile-${CND_CONF}.mk gtk+2.x_tab_page_distribution.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) `pkg-config --cflags gtk+-2.0` `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/gtk+2.x_tab_page_distribution.o gtk+2.x_tab_page_distribution.c

${OBJECTDIR}/manage_xorg_preferences.o: nbproject/Makefile-${CND_CONF}.mk manage_xorg_preferences.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) `pkg-config --cflags gtk+-2.0` `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/manage_xorg_preferences.o manage_xorg_preferences.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lxkb-config

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
