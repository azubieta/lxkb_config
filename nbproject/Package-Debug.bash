#!/bin/bash -x

#
# Generated - do not edit!
#

# Macros
TOP=`pwd`
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build
NBTMPDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tmp-packaging
TMPDIRNAME=tmp-packaging
OUTPUT_PATH=${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lxkb-config
OUTPUT_BASENAME=lxkb-config
PACKAGE_TOP_DIR=/usr/

# Functions
function checkReturnCode
{
    rc=$?
    if [ $rc != 0 ]
    then
        exit $rc
    fi
}
function makeDirectory
# $1 directory path
# $2 permission (optional)
{
    mkdir -p "$1"
    checkReturnCode
    if [ "$2" != "" ]
    then
      chmod $2 "$1"
      checkReturnCode
    fi
}
function copyFileToTmpDir
# $1 from-file path
# $2 to-file path
# $3 permission
{
    cp "$1" "$2"
    checkReturnCode
    if [ "$3" != "" ]
    then
        chmod $3 "$2"
        checkReturnCode
    fi
}

# Setup
cd "${TOP}"
mkdir -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package
rm -rf ${NBTMPDIR}
mkdir -p ${NBTMPDIR}

# Copy files and create directories and links
cd "${TOP}"
makeDirectory "${NBTMPDIR}//usr/bin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}//usr/share/applications"
copyFileToTmpDir "RESOURCES/keyboard.desktop.in.in" "${NBTMPDIR}//usr/share/applications/keyboard.desktop" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}//etc/xdg/autostart"
copyFileToTmpDir "RESOURCES/keyboard.desktop.startup.in" "${NBTMPDIR}//etc/xdg/autostart/keyboard.desktop.startup" 0444


# Create control file
cd "${TOP}"
CONTROL_FILE=${NBTMPDIR}/DEBIAN/control
rm -f ${CONTROL_FILE}
mkdir -p ${NBTMPDIR}/DEBIAN

cd "${TOP}"
echo 'Package: lxkb-config' >> ${CONTROL_FILE}
echo 'Version: 0.2' >> ${CONTROL_FILE}
echo 'Architecture: i386' >> ${CONTROL_FILE}
echo 'Maintainer: Alexis López Zubieta' >> ${CONTROL_FILE}
echo 'email: azubieta@estudiantes.uci.cu' >> ${CONTROL_FILE}
echo 'Description: Tool to configure the keyboard preferences.   Functionalities:    - Select keyboard layout and variant.    - Test keyboard layout and variant before set it.    - Set a configuration to the global sistem  (writing the .xorg configuaration files).    - Remember configurations between sessions.      Advice:    - Probably you must add this command to your startup secuence in      order to set up the stored configuration.   lxkb-config -s     Contact info:   Alexis López Zubieta  azubieta@estudiantes.uci.cu  ' >> ${CONTROL_FILE}
echo 'Depends: libglib2.0-0 (>= 2.16.0), libgtk2.0-0 (>= 2.8.0), libpango1.0-0 (>= 1.14.0), libpng12-0 (>= 1.2.13-4), libxml2 (>= 2.7.4), dpkg (>= 1.15.4)' >> ${CONTROL_FILE}

# Create Debian Package
cd "${TOP}"
cd "${NBTMPDIR}/.."
dpkg-deb  --build ${TMPDIRNAME}
checkReturnCode
cd "${TOP}"
mkdir -p  ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package
mv ${NBTMPDIR}.deb ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package/lxkb-config.deb
checkReturnCode
echo Debian: ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package/lxkb-config.deb

# Cleanup
cd "${TOP}"
rm -rf ${NBTMPDIR}
