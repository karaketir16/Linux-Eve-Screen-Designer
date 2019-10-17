QT -= gui

CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += ColibriiMX6_PLATFORM
DEFINES += NHD_3_5C_FT813
DEFINES += DISPLAY_RESOLUTION_QVGA
DEFINES += EVE_GRAPHICS_AVAILABLE
DEFINES += EVE_DISPLAY_AVAILABLE
DEFINES += FT813_ENABLE

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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

DEPENDPATH  += $$PWD/Colibri_Hal/colibri_spi/
INCLUDEPATH += $$PWD/Colibri_Hal/colibri_spi/

DEPENDPATH  += $$PWD/Colibri_Hal/
INCLUDEPATH += $$PWD/Colibri_Hal/


SOURCES += \
        $$PWD/designerOut/Generated/ME812A_WH50R/App/*.c \
        $$PWD/designerOut/Libraries/FT_Esd_Framework/*.c \
        $$PWD/designerOut/Generated/ME812A_WH50R/FT_Esd_Widgets/*.c \
        $$PWD/designerOut/Libraries/FT_Eve_Hal/*.c \
        $$PWD/designerOut/Libraries/FT_Esd_Widgets/*.c \
        $$PWD/designerOut/Generated/ME812A_WH50R/FT_Esd_Framework/*.c \
        $$PWD/Colibri_Hal/colibri_spi/*.c* \
        $$PWD/Colibri_Hal/*.c*

HEADERS += \
        $$PWD/designerOut/Generated/ME812A_WH50R/App/*.h \
        $$PWD/designerOut/Libraries/FT_Esd_Framework/*.h \
        $$PWD/designerOut/Generated/ME812A_WH50R/FT_Esd_Widgets/*.h \
        $$PWD/designerOut/Libraries/FT_Eve_Hal/*.h \
        $$PWD/designerOut/Libraries/FT_Eve_Hal/Hdr/*.h \
        $$PWD/designerOut/Libraries/FT_Esd_Widgets/*.h \
        $$PWD/designerOut/Generated/ME812A_WH50R/FT_Esd_Framework/*.h \
        $$PWD/Colibri_Hal/colibri_spi/*.h \



unix {
  target.path=$$PREFIX/usr/bin
  INSTALLS += target
}

