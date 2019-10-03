QT -= gui

#CONFIG += c99

#QMAKE_CXX = gcc

#QMAKE_CXXFLAGS = -x c

#QMAKE_LINK = gcc

#QMAKE_LINK_SHLIB = $$QMAKE_CXX

CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += ColibriiMX6_PLATFORM
DEFINES += NHD_3_5C_FT813

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#DESIGNER_PATH = $$quote($$absolute_path($$PWD/designerOut))

#DEPENDPATH  += $$quote($$DESIGNER_PATH\Libraries)
#INCLUDEPATH += $$quote($$DESIGNER_PATH\Generated)

#DEPENDPATH  += $$quote($$DESIGNER_PATH\Libraries)
#INCLUDEPATH += $$quote($$DESIGNER_PATH\Generated)


DEPENDPATH  += $$PWD/designerOut/Libraries/FT_Eve_Hal/Hdr/
INCLUDEPATH += $$PWD/designerOut/Libraries/FT_Eve_Hal/Hdr/

DEPENDPATH  += $$PWD/designerOut/Libraries/FT_Eve_Hal/
INCLUDEPATH += $$PWD/designerOut/Libraries/FT_Eve_Hal/

DEPENDPATH  += $$PWD/designerOut/Libraries/FT_Esd_Framework/
INCLUDEPATH += $$PWD/designerOut/Libraries/FT_Esd_Framework/

DEPENDPATH  += $$PWD/designerOut/Libraries/FT_Esd_Widgets/
INCLUDEPATH += $$PWD/designerOut/Libraries/FT_Esd_Widgets/

DEPENDPATH  += $$PWD/designerOut/Generated/ME812A_WH50R/FT_Esd_Framework/
INCLUDEPATH += $$PWD/designerOut/Generated/ME812A_WH50R/FT_Esd_Framework/

DEPENDPATH  += $$PWD/designerOut/Generated/ME812A_WH50R/FT_Esd_Widgets/
INCLUDEPATH += $$PWD/designerOut/Generated/ME812A_WH50R/FT_Esd_Widgets/

DEPENDPATH  += $$PWD/designerOut/Generated/ME812A_WH50R/App/
INCLUDEPATH += $$PWD/designerOut/Generated/ME812A_WH50R/App/

DEPENDPATH  += $$PWD/spi/
INCLUDEPATH += $$PWD/spi/


SOURCES += \
        $$PWD/designerOut/Generated/ME812A_WH50R/App/*.c \
        $$PWD/designerOut/Libraries/FT_Esd_Framework/*.c \
        $$PWD/designerOut/Generated/ME812A_WH50R/FT_Esd_Widgets/*.c \
        $$PWD/designerOut/Libraries/FT_Eve_Hal/*.c \
        $$PWD/designerOut/Libraries/FT_Esd_Widgets/*.c \
        $$PWD/designerOut/Generated/ME812A_WH50R/FT_Esd_Framework/*.c \
        $$PWD/spi/*.c* \
    backup.cpp \
    main.cpp

HEADERS += \
        $$PWD/designerOut/Generated/ME812A_WH50R/App/*.h \
        $$PWD/designerOut/Libraries/FT_Esd_Framework/*.h \
        $$PWD/designerOut/Generated/ME812A_WH50R/FT_Esd_Widgets/*.h \
        $$PWD/designerOut/Libraries/FT_Eve_Hal/*.h \
        $$PWD/designerOut/Libraries/FT_Eve_Hal/Hdr/*.h \
        $$PWD/designerOut/Libraries/FT_Esd_Widgets/*.h \
        $$PWD/designerOut/Generated/ME812A_WH50R/FT_Esd_Framework/*.h \
        $$PWD/spi/*.h \



unix {
  target.path=$$PREFIX/usr/bin
  INSTALLS += target
}

