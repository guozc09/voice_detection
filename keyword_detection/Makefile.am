keyword_detectiondir = $(libdir)

SUBDIRS = kwd

keyword_detection_PROGRAMS = libkeyword_detection.so
libkeyword_detection_so_SOURCES = KeywordDetection.cpp

libkeyword_detection_so_CFLAGS = -I$(top_srcdir)/include \
                                 -Ikwd/include \
                                 -I$(SYSTEM_INCLUDE_PATH)
libkeyword_detection_so_CFLAGS += -fPIC -std=c99
libkeyword_detection_so_CFLAGS += \
    -DVOICE_DETECTION_VERSION_MAJOR=$(VOICE_DETECTION_VERSION_MAJOR) \
    -DVOICE_DETECTION_VERSION_MINOR=$(VOICE_DETECTION_VERSION_MINOR) \
    -DVOICE_DETECTION_VERSION_RELEASE=$(VOICE_DETECTION_VERSION_RELEASE)

libkeyword_detection_so_CXXFLAGS = -I$(top_srcdir)/include \
                                   -Ikwd/include \
                                   -I$(SYSTEM_INCLUDE_PATH)
libkeyword_detection_so_CXXFLAGS += -fPIC -std=c++11
libkeyword_detection_so_CXXFLAGS += \
    -DVOICE_DETECTION_VERSION_MAJOR=$(VOICE_DETECTION_VERSION_MAJOR) \
    -DVOICE_DETECTION_VERSION_MINOR=$(VOICE_DETECTION_VERSION_MINOR) \
    -DVOICE_DETECTION_VERSION_RELEASE=$(VOICE_DETECTION_VERSION_RELEASE)

libkeyword_detection_so_LDFLAGS = -lpthread -Lkwd/lib -lkwd
libkeyword_detection_so_LDFLAGS += -fPIC -shared
libkeyword_detection_so_LDFLAGS += -Wl,-z,defs

keyword_detection_includedir=$(includedir)/voice_detection
keyword_detection_include_HEADERS = KeywordDetection.h
