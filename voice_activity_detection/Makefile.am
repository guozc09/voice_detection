voice_activity_detectiondir = $(libdir)

SUBDIRS = vad

voice_activity_detection_PROGRAMS = libvoice_activity_detection.so
libvoice_activity_detection_so_SOURCES = VoiceActivityDetection.cpp

libvoice_activity_detection_so_CFLAGS = -I$(top_srcdir)/include \
                                        -Ivad/include \
                                        -I$(SYSTEM_INCLUDE_PATH)
libvoice_activity_detection_so_CFLAGS += -fPIC -std=c99
libvoice_activity_detection_so_CFLAGS += \
    -DVOICE_DETECTION_VERSION_MAJOR=$(VOICE_DETECTION_VERSION_MAJOR) \
    -DVOICE_DETECTION_VERSION_MINOR=$(VOICE_DETECTION_VERSION_MINOR) \
    -DVOICE_DETECTION_VERSION_RELEASE=$(VOICE_DETECTION_VERSION_RELEASE)

libvoice_activity_detection_so_CXXFLAGS = -I$(top_srcdir)/include \
                                          -Ivad/include \
                                          -I$(SYSTEM_INCLUDE_PATH)
libvoice_activity_detection_so_CXXFLAGS += -fPIC -std=c++11
libvoice_activity_detection_so_CXXFLAGS += \
    -DVOICE_DETECTION_VERSION_MAJOR=$(VOICE_DETECTION_VERSION_MAJOR) \
    -DVOICE_DETECTION_VERSION_MINOR=$(VOICE_DETECTION_VERSION_MINOR) \
    -DVOICE_DETECTION_VERSION_RELEASE=$(VOICE_DETECTION_VERSION_RELEASE)

libvoice_activity_detection_so_LDFLAGS = -lpthread -Lvad/lib -lvad
libvoice_activity_detection_so_LDFLAGS += -fPIC -shared
libvoice_activity_detection_so_LDFLAGS += -Wl,-z,defs

voice_activity_detection_includedir=$(includedir)/voice_detection
voice_activity_detection_include_HEADERS = VoiceActivityDetection.h