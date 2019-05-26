CONFIG   += console


REPC_SOURCE += ../accounting_protocol.rep
QT = remoteobjects core sql

SOURCES += main.cpp \
    accounting_server.cpp \
    databasecontroller.cpp
HEADERS += \
    accounting_server.h \
    databasecontroller.h

contains(QT_CONFIG, c++11): CONFIG += c++11

