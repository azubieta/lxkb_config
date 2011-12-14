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
CND_CONF=gtk-gui
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/gtk+2.x.o \
	${OBJECTDIR}/setxkbmap_interface.o \
	${OBJECTDIR}/parse_xml_rules.o \
	${OBJECTDIR}/RESOURCES/manage_global_preferences.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/manage_user_preferences.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

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
LDLIBSOPTIONS=`pkg-config --libs libxml-2.0` `pkg-config --libs gconf-2.0` `pkg-config --libs gtk+-2.0`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lxkb-config

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lxkb-config: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lxkb-config ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/gtk+2.x.o: gtk+2.x.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0` `pkg-config --cflags gtk+-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/gtk+2.x.o gtk+2.x.c

${OBJECTDIR}/setxkbmap_interface.o: setxkbmap_interface.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0` `pkg-config --cflags gtk+-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/setxkbmap_interface.o setxkbmap_interface.c

${OBJECTDIR}/parse_xml_rules.o: parse_xml_rules.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0` `pkg-config --cflags gtk+-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/parse_xml_rules.o parse_xml_rules.c

${OBJECTDIR}/RESOURCES/manage_global_preferences.o: RESOURCES/manage_global_preferences.c 
	${MKDIR} -p ${OBJECTDIR}/RESOURCES
	${RM} $@.d
	$(COMPILE.c) -g `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0` `pkg-config --cflags gtk+-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/RESOURCES/manage_global_preferences.o RESOURCES/manage_global_preferences.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0` `pkg-config --cflags gtk+-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/manage_user_preferences.o: manage_user_preferences.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0` `pkg-config --cflags gtk+-2.0`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/manage_user_preferences.o manage_user_preferences.c

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f1: ${TESTDIR}/RESOURCES/tests/newsimpletest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} 


${TESTDIR}/RESOURCES/tests/newsimpletest.o: RESOURCES/tests/newsimpletest.c 
	${MKDIR} -p ${TESTDIR}/RESOURCES/tests
	${RM} $@.d
	$(COMPILE.c) -g -I. `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0` `pkg-config --cflags gtk+-2.0`    -MMD -MP -MF $@.d -o ${TESTDIR}/RESOURCES/tests/newsimpletest.o RESOURCES/tests/newsimpletest.c


${OBJECTDIR}/gtk+2.x_nomain.o: ${OBJECTDIR}/gtk+2.x.o gtk+2.x.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/gtk+2.x.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0` `pkg-config --cflags gtk+-2.0`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/gtk+2.x_nomain.o gtk+2.x.c;\
	else  \
	    ${CP} ${OBJECTDIR}/gtk+2.x.o ${OBJECTDIR}/gtk+2.x_nomain.o;\
	fi

${OBJECTDIR}/setxkbmap_interface_nomain.o: ${OBJECTDIR}/setxkbmap_interface.o setxkbmap_interface.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/setxkbmap_interface.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0` `pkg-config --cflags gtk+-2.0`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/setxkbmap_interface_nomain.o setxkbmap_interface.c;\
	else  \
	    ${CP} ${OBJECTDIR}/setxkbmap_interface.o ${OBJECTDIR}/setxkbmap_interface_nomain.o;\
	fi

${OBJECTDIR}/parse_xml_rules_nomain.o: ${OBJECTDIR}/parse_xml_rules.o parse_xml_rules.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/parse_xml_rules.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0` `pkg-config --cflags gtk+-2.0`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/parse_xml_rules_nomain.o parse_xml_rules.c;\
	else  \
	    ${CP} ${OBJECTDIR}/parse_xml_rules.o ${OBJECTDIR}/parse_xml_rules_nomain.o;\
	fi

${OBJECTDIR}/RESOURCES/manage_global_preferences_nomain.o: ${OBJECTDIR}/RESOURCES/manage_global_preferences.o RESOURCES/manage_global_preferences.c 
	${MKDIR} -p ${OBJECTDIR}/RESOURCES
	@NMOUTPUT=`${NM} ${OBJECTDIR}/RESOURCES/manage_global_preferences.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0` `pkg-config --cflags gtk+-2.0`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/RESOURCES/manage_global_preferences_nomain.o RESOURCES/manage_global_preferences.c;\
	else  \
	    ${CP} ${OBJECTDIR}/RESOURCES/manage_global_preferences.o ${OBJECTDIR}/RESOURCES/manage_global_preferences_nomain.o;\
	fi

${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0` `pkg-config --cflags gtk+-2.0`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/main_nomain.o main.c;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
	fi

${OBJECTDIR}/manage_user_preferences_nomain.o: ${OBJECTDIR}/manage_user_preferences.o manage_user_preferences.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/manage_user_preferences.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g `pkg-config --cflags libxml-2.0` `pkg-config --cflags gconf-2.0` `pkg-config --cflags gtk+-2.0`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/manage_user_preferences_nomain.o manage_user_preferences.c;\
	else  \
	    ${CP} ${OBJECTDIR}/manage_user_preferences.o ${OBJECTDIR}/manage_user_preferences_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lxkb-config

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
