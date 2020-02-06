TEMPLATE+=app
TARGET=../bin/abi2xml
DEFINES += HAVE_GETOPT_LONG HAVE_STRDUP


SOURCES += main.cpp \
           abi2xml.cpp \
           datarecord.cpp \
           datarecordmaker.cpp \
           gengetopt/cmdline.c
HEADERS += abi2xml.h \
           datarecord.h \
           datarecordmaker.h \
           gengetopt/cmdline.h \
           params.h
CONFIG += qt console

QT +=  xml
