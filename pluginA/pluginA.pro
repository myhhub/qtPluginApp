
TEMPLATE        = lib           #表示这个makefile是一个lib的makefile
CONFIG         += plugin        #应用程序是一个插件
TARGET          = pluginA       #生成插件的名称
win32 {
    CONFIG(debug, debug|release) {
        DESTDIR  = ../Main/debug/plugins
    } else {
        DESTDIR  = ../Main/release/plugins
    }
}#生成插件的目录

HEADERS += \
    plugina.h

SOURCES += \
    plugina.cpp

DISTFILES += \
    programmer.json \
    programmer.json             #插件描述文件

msvc {
    #QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}
