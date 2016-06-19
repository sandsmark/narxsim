TEMPLATE = app
TARGET = narxsim
INCLUDEPATH += .

QT += widgets

# Input
HEADERS += Activation_functions.h \
           EvaluationEngine.h \
           FeedbackInfo.h \
           InputUnit.h \
           NARX.h \
           narx2.h \
           narx_util.h \
           OutputUnit.h \
           resource.h \
           Unit.h
FORMS += narx2.ui
SOURCES += Activation_functions.cpp \
           EvaluationEngine.cpp \
           FeedbackInfo.cpp \
           InputUnit.cpp \
           main.cpp \
           NARX.cpp \
           narx2.cpp \
           OutputUnit.cpp \
           Unit.cpp
