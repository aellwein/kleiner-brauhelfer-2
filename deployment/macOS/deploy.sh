#!/bin/sh

if ! [ -d "$1" ] || ! [ -d "$2" ]; then
  echo "Usage: $0 <path_to_kleiner-brauhelfer_bundle> <path_to_qt_bin> <optional version_suffix>" >&2
  echo "Example: $0 ./bin/kleiner-brauhelfer.app /usr/local/opt/qt/bin" >&2
  exit 1
fi


# Project basedir
BASE_DIR="$(dirname $0)"

# Path to the created bundle
BUNDLE="$1"

# Path to Qt utilities
QT_DIR="$2"

# Path to the sources directory
SOURCES="${BASE_DIR}/../../kleiner-brauhelfer"

# Path to the .pro-file
PRO="${SOURCES}/kleiner-brauhelfer.pro"

# Path to the deployment resources
RESOURCES="${BASE_DIR}"

VERSION_SUFFIX="$3"

###
### Extract version numbers from .pro file
###

echo "* Extracting version numbers from .pro file:"
VERSION_MAJ=`grep "${PRO}" -e "^ *VER_MAJ *=" -m 1| tr -d '[:space:]' | cut -d= -f2` || exit 1
VERSION_MIN=`grep "${PRO}" -e "^ *VER_MIN *=" -m 1| tr -d '[:space:]' | cut -d= -f2` || exit 1
VERSION_PAT=`grep "${PRO}" -e "^ *VER_PAT *=" -m 1| tr -d '[:space:]' | cut -d= -f2` || exit 1
VERSION="${VERSION_MAJ}.${VERSION_MIN}.${VERSION_PAT}${VERSION_SUFFIX}"
echo "  - Version: '${VERSION}'"

###
### Patch Info.plist in generated bundle
###

PLIST="${BUNDLE}/Contents/Info.plist"
echo "* Patching '${PLIST}'..."

# Remove QT advertising
/usr/libexec/PlistBuddy -c "Delete :NOTE" ${PLIST}

# Remove deprecated creator code for old MacOS
/usr/libexec/PlistBuddy -c "Delete :CFBundleSignature" ${PLIST}

# Remove deprecated get-info string
/usr/libexec/PlistBuddy -c "Delete :CFBundleGetInfoString" ${PLIST}

# Set bundle identifier instead of usieng default of 'yourcompany.com'
/usr/libexec/PlistBuddy -c "Set :CFBundleIdentifier 'com.github.kleiner-brauhelfer.kleiner-brauhelfer-2'" ${PLIST}

# Add finder icon
/usr/libexec/PlistBuddy -c "Set :CFBundleIconFile 'icon'" ${PLIST}

# Minimum OS is High Sierra to be sure
/usr/libexec/PlistBuddy -c "Add :LSMinimumSystemVersion string '10.13'" ${PLIST}

# Add information for Finder
/usr/libexec/PlistBuddy -c "Delete :CFBundleDisplayName" ${PLIST}
/usr/libexec/PlistBuddy -c "Add :CFBundleDisplayName string 'kleiner-brauhelfer-2'" ${PLIST} || exit 1

/usr/libexec/PlistBuddy -c "Delete :CFBundleName" ${PLIST}
/usr/libexec/PlistBuddy -c "Add :CFBundleName string 'Kleiner Brauhelfer 2'" ${PLIST} || exit 1

/usr/libexec/PlistBuddy -c "Delete :CFBundleShortVersionString" ${PLIST}
/usr/libexec/PlistBuddy -c "Add :CFBundleShortVersionString string '${VERSION}'" ${PLIST} || exit 1

/usr/libexec/PlistBuddy -c "Delete :CFBundleVersion" ${PLIST}
/usr/libexec/PlistBuddy -c "Add :CFBundleVersion string '${VERSION}'" ${PLIST} || exit 1

/usr/libexec/PlistBuddy -c "Delete :NSHumanReadableCopyright" ${PLIST}
/usr/libexec/PlistBuddy -c "Add :NSHumanReadableCopyright string 'Copyright © 2018-2023 Gremmelsoft. All rights reserved.'" ${PLIST} || exit 1



###
### Copy resource files
###

echo "* Copying resource files..."
cp "${RESOURCES}/InfoPlist.strings" "${BUNDLE}/Contents/Resources" || exit 1

# remove not needed executable flags
chmod 644 "${BUNDLE}/Contents/Resources/icon.icns"
chmod 644 "${BUNDLE}/Contents/Resources/InfoPlist.strings"

###
### Running QT deployment
###

echo "* Creating self-contained bundle..."

"${QT_DIR}/macdeployqt" ${BUNDLE} \
    -codesign=-\
 || exit 1

rm -f ${BUNDLE}/Contents/PlugIns/sqldrivers/libqsqlodbc.dylib
rm -f ${BUNDLE}/Contents/PlugIns/sqldrivers/libqsqlpsql.dylib

###
### Create distribution archive
###

echo "* Creating ZIP-archive for distribution..."
DIR="$(dirname ${BUNDLE})"
ZIPVERSION=`echo "${VERSION}"`
ZIP="kbh2_v${ZIPVERSION}_macos.zip"
pushd "${DIR}" || exit 1
zip -ry -o "${ZIP}" `basename ${BUNDLE}` || exit 1
popd
echo "  - Created archive: ${DIR}/${ZIP}"

echo "=> Deployment finished successfully"
