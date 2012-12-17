#-------------------------------------------------
#
# Project created by QtCreator 2012-12-08T17:32:32
#
#-------------------------------------------------

QT       += core gui

TARGET = cssSprites
TEMPLATE = app

RESOURCES += \
	Resources/res.qrc

FORMS += \
	Dialog/dlgproperty.ui\
	Dialog/mainwindow.ui\


SOURCES += \
	main/alertexception.cpp\
	main/main.cpp\
	WndCtl/abstractwindow.cpp\
	WndCtl/abstractwindow_move.cpp\
	WndCtl/windowmananger.cpp\
	Dialog/dlgproperty.cpp\
	Dialog/mainwindow.cpp\
	Dialog/mainwindow_actions.cpp\
	Dialog/dlgtextwindow.cpp\
	Object/imageframe.cpp 

HEADERS += \
	main/alertexception.h\
	main/main.h\
	WndCtl/abstractwindow.h\
	WndCtl/windowmananger.h\
	Dialog/mainwindow.h\
	Dialog/dlgproperty.h\
	Dialog/dlgtextwindow.h\
	Object/imageframe.h

INCLUDEPATH += main WndCtl Dialog Object
