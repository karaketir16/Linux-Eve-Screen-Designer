QT -= gui

CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += ColibriiMX6_PLATFORM
DEFINES += DISPLAY_RESOLUTION_QVGA
DEFINES += EVE_GRAPHICS_AVAILABLE
DEFINES += EVE_DISPLAY_AVAILABLE
DEFINES += FT813_ENABLE

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEPENDPATH  += $$PWD/Libraries/FT_Eve_Hal/Hdr/
INCLUDEPATH += $$PWD/Libraries/FT_Eve_Hal/Hdr/

DEPENDPATH  += $$PWD/Libraries/FT_Eve_Hal/
INCLUDEPATH += $$PWD/Libraries/FT_Eve_Hal/

DEPENDPATH  += $$PWD/Libraries/FT_Esd_Framework/
INCLUDEPATH += $$PWD/Libraries/FT_Esd_Framework/

DEPENDPATH  += $$PWD/Libraries/FT_Esd_Widgets/
INCLUDEPATH += $$PWD/Libraries/FT_Esd_Widgets/

DEPENDPATH  += $$PWD/Generated/FT_Esd_Framework/
INCLUDEPATH += $$PWD/Generated/FT_Esd_Framework/

DEPENDPATH  += $$PWD/Generated/FT_Esd_Widgets/
INCLUDEPATH += $$PWD/Generated/FT_Esd_Widgets/

DEPENDPATH  += $$PWD/Generated/App/
INCLUDEPATH += $$PWD/Generated/App/

DEPENDPATH  += $$PWD/Colibri_Hal/colibri/
INCLUDEPATH += $$PWD/Colibri_Hal/colibri/

DEPENDPATH  += $$PWD/Colibri_Hal/
INCLUDEPATH += $$PWD/Colibri_Hal/

DEPENDPATH  += $$PWD/Libraries/FT_Eve_Hal/Hdr/
INCLUDEPATH += $$PWD/Libraries/FT_Eve_Hal/Hdr/

SOURCES += \
        $$files(*.c, true)
#        $$PWD/Generated/*/*/*.c \
#        $$PWD/Libraries/FT_Esd_Framework/*.c \
#        $$PWD/Generated/*/FT_Esd_Widgets/*.c \
#        $$PWD/Libraries/FT_Eve_Hal/*.c \
#        $$PWD/Libraries/FT_Esd_Widgets/*.c \
#        $$PWD/Generated/*/FT_Esd_Framework/*.c \
#        $$PWD/Colibri_Hal/colibri_spi/*.c* \
#        $$PWD/Colibri_Hal/*.c*

HEADERS += \
        $$files(*.h, true)
#        $$PWD/Generated/*/App/*.h \
#        $$PWD/Libraries/FT_Esd_Framework/*.h \
#        $$PWD/Generated/*/FT_Esd_Widgets/*.h \
#        $$PWD/Libraries/FT_Eve_Hal/*.h \
#        $$PWD/Libraries/FT_Eve_Hal/Hdr/*.h \
#        $$PWD/Libraries/FT_Esd_Widgets/*.h \
#        $$PWD/Generated/*/FT_Esd_Framework/*.h \
#        $$PWD/Colibri_Hal/colibri_spi/*.h



unix {
  target.path=$$PREFIX/usr/bin
  INSTALLS += target
}

