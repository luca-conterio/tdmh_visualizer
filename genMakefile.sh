#!/bin/sh


#QT version required 5.12.6

#if not present run
#qtchooser -install qt5.12.6 <pathtoqmake>
#e.g. qtchooser -install qt5.12.6 /opt/Qt/5.12.6/gcc_64/bin/qmake
#then export Q_SELECT=qt5.12.6


if [ "$1" = "debug" ]
    then
	mkdir -p build.Debug
        qmake -o build.Debug/Makefile ../tdmh_visualizer/tdmh_visualizer.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug
	exit 0
    fi
mkdir -p build.Release
qmake -o build.Release/Makefile ./tdmh_visualizer.pro -spec linux-g++ CONFIG+=qtquickcompiler
