#******************************************************************************
# seq66.pro (qpseq66)
#------------------------------------------------------------------------------
##
# \file         seq66.pro
# \library      qpseq66 application
# \author       Chris Ahlstrom
# \date         2018-11-15
# \update       2020-07-18
# \version      $Revision$
# \license      $XPC_SUITE_GPL_LICENSE$
#
#   Created for Qt Creator. This file was created for editing the project
#   sources and for allowing the developer to use "qmake" to configure the
#   builds.
#
#   This project file is designed only for Qt 5 and, it is to be hoped, above.
#   Note the "qtc_runnable" flag in CONFIG. It prevents Qt Creator from
#   automatically creating run configurations for SUBDIRS projects. Qt Creator
#   creates run configurations only for subprojects that also have "CONFIG +=
#   qtc_runnable" set in their ".pro" files.
#
#   Also note that one can add "rtmidi" to CONFIG in order to replace the
#   internal PortMidi library with the preferable internal RtMidi library.  The
#   internal PortMidi library is currently meant for Windows and Mac, but can
#   be used to make a Linux build to test a PortMidi on our preferred platform.
#
#   The application generated by this profile is named "qpseq66", and uses the
#   built-in Seq66 "portmidi" library.  We recommend runnning qmake and make
#   from a "shadow" directory.  See "contrib/scripts/q-make".
#
#   Unsupported (use automake): Seqtool
#
#------------------------------------------------------------------------------

TEMPLATE = subdirs
CONFIG += static link_prl ordered qtc_runnable c++14
contains (CONFIG, rtmidi) {
    SUBDIRS =  libseq66 libsessions seq_rtmidi seq_qt5 Seq66qt5
    Seq66qt5.depends = libseq66 libsessions seq_rtmidi seq_qt5
} else {
    SEQ66_MIDILIB = portmidi
    SUBDIRS =  libseq66 seq_portmidi seq_qt5 Seq66qt5
    Seq66qt5.depends = libseq66 seq_portmidi seq_qt5
}
message("SUBDIRS is set to: $${SUBDIRS}")

# These do not work on 32-bit Linux using Qt 5.3:
#
# CONFIG += c++14 -or-  QMAKE_CXXFLAGS += -std=gnu++14

QMAKE_CXXFLAGS += -std=c++14

# Use only automake to build this side app, for now:
#
# Seqtool.depends = libseq66

#******************************************************************************
# seq66.pro (qpseq66)
#------------------------------------------------------------------------------
# vim: ts=4 sw=4 ft=automake
#------------------------------------------------------------------------------
