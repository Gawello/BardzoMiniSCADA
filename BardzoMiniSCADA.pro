QT       += core network gui
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = BardzoMiniSCADA
TEMPLATE = app

SOURCES += \
    core/DisplayLogic.cpp \
    gui/ConfigurationPanel.cpp \
    gui/ElementConfigDialog.cpp \
    main.cpp \
    core/DataBuffer.cpp \
    core/NetworkManager.cpp \
    core/DataLogger.cpp \
    core/DataProcessor.cpp \
    gui/Diagram.cpp \
    gui/LineDiagram.cpp \
    gui/PointDiagram.cpp \
    gui/BarDiagram.cpp \
    gui/DataDisplay.cpp \
    gui/TextDisplay.cpp \
    gui/BarDisplay.cpp \
    gui/MainWindow.cpp \
    simulation/DataSimulator.cpp

HEADERS += \
    core/DataBuffer.h \
    core/DisplayLogic.h \
    core/NetworkManager.h \
    core/DataLogger.h \
    core/DataProcessor.h \
    gui/ConfigurationPanel.h \
    gui/Diagram.h \
    gui/ElementConfigDialog.h \
    gui/LineDiagram.h \
    gui/PointDiagram.h \
    gui/BarDiagram.h \
    gui/DataDisplay.h \
    gui/TextDisplay.h \
    gui/BarDisplay.h \
    gui/MainWindow.h \
    simulation/DataSimulator.h

INCLUDEPATH += \
    core \
    gui \
    simulation

RESOURCES += \
    resources/resources.qrc

TRANSLATIONS += \
    BardzoMiniSCADA_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
