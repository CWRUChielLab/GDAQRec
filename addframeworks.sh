#!/bin/bash

mkdir -p GDAQrec.app/Contents/Frameworks
cp -R /Library/Frameworks/QtGui.framework GDAQrec.app/Contents/Frameworks
cp -R /Library/Frameworks/QtCore.framework GDAQrec.app/Contents/Frameworks

install_name_tool -id @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore GDAQrec.app/Contents/Frameworks/QtCore.framework/Versions/4/QtCore
install_name_tool -id @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui GDAQrec.app/Contents/Frameworks/QtGui.framework/Versions/4/QtGui

install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore GDAQrec.app/Contents/MacOs/GDAQrec
install_name_tool -change QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui GDAQrec.app/Contents/MacOs/GDAQrec

install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore GDAQrec.app/Contents/Frameworks/QtGui.framework/Versions/4/QtGui
