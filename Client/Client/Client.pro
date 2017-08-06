#-------------------------------------------------
#
# Project created by QtCreator 2015-01-20T14:04:54
#
#-------------------------------------------------

QT       += core gui \
            network \
            xml \
            multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
qtHaveModule(printsupport): QT += printsupport

TARGET = Client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clienttcp.cpp \
    dessin.cpp \
    xmlService.cpp \
    threadenvoidonnees.cpp \
    threaddessin.cpp \
    threadlecturedonnees.cpp \
    joueur.cpp


HEADERS  += mainwindow.h \
    clienttcp.h \
    dessin.h \
    xmlService.h \
    threadenvoidonnees.h \
    threaddessin.h \
    threadlecturedonnees.h \
    joueur.h

FORMS    += mainwindow.ui

RESOURCES += \
    icone/images.qrc \
    documents/documents.qrc

DISTFILES += \
    documents/Documentation.pdf \
    documents/RegleDuJeu.pdf
