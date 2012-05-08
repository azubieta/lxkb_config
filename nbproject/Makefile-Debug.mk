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
	${OBJECTDIR}/src/main.o \
	${OBJECTDIR}/src/setxkbmap_interface.o \
	${OBJECTDIR}/src/gtk+2.x_tab_page_distribution.o \
	${OBJECTDIR}/src/manage_user_preferences.o \
	${OBJECTDIR}/src/manage_xorg_rules.o \
	${OBJECTDIR}/src/manage_desktop_preferences.o \
	${OBJECTDIR}/src/gtk+2.x.o \
	${OBJECTDIR}/src/xkblib_interface.o \
	${OBJECTDIR}/src/gtk+2.x_tab_page_credits.o \
	${OBJECTDIR}/src/manage_xorg_preferences.o


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
LDLIBSOPTIONS=-L/usr/lib/libgtk2.0-0 `pkg-config --libs gtk+-2.0` `pkg-config --libs libxml-2.0`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lxkb_config

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lxkb_config: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lxkb_config ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/src/main.o: nbproject/Makefile-${CND_CONF}.mk src/main.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) `pkg-config --cflags gtk+-2.0` `pkg-config --cflags libxml-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/main.o src/main.c

${OBJECTDIR}/src/setxkbmap_interface.o: nbproject/Makefile-${CND_CONF}.mk src/setxkbmap_interface.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) `pkg-config --cflags gtk+-2.0` `pkg-config --cflags libxml-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/setxkbmap_interface.o src/setxkbmap_interface.c

${OBJECTDIR}/src/gtk+2.x_tab_page_distribution.o: nbproject/Makefile-${CND_CONF}.mk src/gtk+2.x_tab_page_distribution.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) `pkg-config --cflags gtk+-2.0` `pkg-config --cflags libxml-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/gtk+2.x_tab_page_distribution.o src/gtk+2.x_tab_page_distribution.c

${OBJECTDIR}/src/manage_user_preferences.o: nbproject/Makefile-${CND_CONF}.mk src/manage_user_preferences.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) `pkg-config --cflags gtk+-2.0` `pkg-config --cflags libxml-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/manage_user_preferences.o src/manage_user_preferences.c

${OBJECTDIR}/src/manage_xorg_rules.o: nbproject/Makefile-${CND_CONF}.mk src/manage_xorg_rules.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) `pkg-config --cflags gtk+-2.0` `pkg-config --cflags libxml-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/manage_xorg_rules.o src/manage_xorg_rules.c

${OBJECTDIR}/src/manage_desktop_preferences.o: nbproject/Makefile-${CND_CONF}.mk src/manage_desktop_preferences.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) `pkg-config --cflags gtk+-2.0` `pkg-config --cflags libxml-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/manage_desktop_preferences.o src/manage_desktop_preferences.c

${OBJECTDIR}/src/gtk+2.x.o: nbproject/Makefile-${CND_CONF}.mk src/gtk+2.x.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) `pkg-config --cflags gtk+-2.0` `pkg-config --cflags libxml-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/gtk+2.x.o src/gtk+2.x.c

${OBJECTDIR}/src/xkblib_interface.o: nbproject/Makefile-${CND_CONF}.mk src/xkblib_interface.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) `pkg-config --cflags gtk+-2.0` `pkg-config --cflags libxml-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/xkblib_interface.o src/xkblib_interface.c

${OBJECTDIR}/src/gtk+2.x_tab_page_credits.o: nbproject/Makefile-${CND_CONF}.mk src/gtk+2.x_tab_page_credits.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) `pkg-config --cflags gtk+-2.0` `pkg-config --cflags libxml-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/gtk+2.x_tab_page_credits.o src/gtk+2.x_tab_page_credits.c

${OBJECTDIR}/src/manage_xorg_preferences.o: nbproject/Makefile-${CND_CONF}.mk src/manage_xorg_preferences.c 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.c) `pkg-config --cflags gtk+-2.0` `pkg-config --cflags libxml-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/manage_xorg_preferences.o src/manage_xorg_preferences.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lxkb_config

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
