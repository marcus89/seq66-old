# README for Seq66 0.98.10

Chris Ahlstrom
2015-09-10 to 2022-07-18

__Seq66__ is a MIDI sequencer and live-looper with a hardware-sampler-like
grid-pattern interface, sets and playlists for song management, a scale and
chord-aware piano-roll interface, song editor for creative composition, and
control via MIDI automation for live performance.  Mute-groups enable/disable
multiple patterns with one keystroke or MIDI control. Supports NSM (Non Session
Manager) on Linux; can also run headless.  It does not support audio samples,
just MIDI.

__Seq66__ is a major refactoring of Sequencer64/Kepler34, rebooting __Seq24__
with modern C++ and new features.  Linux users can build this application from
the source code.  See the INSTALL file; it has notes on many types on
installation.  Windows users can get an installer package on GitHub or build it
with Qt Creator.  Provides a comprehensive PDF user-manual.

Support sites (still in progress):

    *   https://ahlstromcj.github.io/
    *   https://github.com/ahlstromcj/ahlstromcj.github.io/wiki

![Alt text](doc/latex/images/main-window/main-windows.png?raw=true "Seq66")

# Major Features

##  User interface

    *   Qt 5 (cross-platform).  A grid of loop buttons, unlimited external
        windows.  Qt style-sheet support.
    *   Tabs for management of sets, mute-groups, song mode, pattern
        editing, event-editing, play-lists, and session information.
    *   Low-frequency oscillator (LFO) for modifying continuous controller
        (CC) and velocity values.
    *   New. An editor for expansion/compression/alignment of note patterns.
    *   Each pattern slot can be colored; the color palette for slots and
        the piano rolls can be saved and modified.
    *   Horizontal and vertical zoom in the pattern and song editors.
    *   A headless version can be built.

##  Configuration files

    *   Separates MIDI control and mute-group setting into their own files.
    *   Supports configuration files: ".rc", ".usr", ".ctrl", ".mutes",
        ".playlist", ".drums" (note-mapping), ".palette", and Qt ".qss".
    *   Unified keystroke and MIDI controls in the ".ctrl" file; defines MIDI
        controls for automating Seq66 and for displaying Seq66 status in grid
        controllers (e.g. LaunchPad).  Basic ".ctrl" files are provided for the
        Launchpad Mini.

##  Non Session Manager

    *   Support for NSM, New Session Manager, RaySession, Agordejo....
    *   Handles starting, stopping, hiding, and session saving.
    *   Displays details about the session.

##  Multiple Builds

    *   ALSA/JACK: `qseq66`
    *   Command-line/headless: `seq66cli`
    *   PortMidi: `qpseq66`
    *   Windows: `qpseq66.exe`

##  More Features

    *   Transposable triggers to re-use patterns more comprehensively.
        Works with Song Export.
    *   Improved non-U.S. keyboard support.
    *   Many demonstration and test MIDI files.
    *   SMF 0 import and export.
    *   See **Recent Changes** below, and the **NEWS** file.

##  Internal

    *   More consistent use of modern C++, auto, and lambda functions.
    *   Additional performer callbacks to reduce the need for polling.
    *   A ton of clean-up and refactoring.

Seq66 uses a Qt 5 user-interface based on Kepler34 and the Seq66 *rtmidi*
(Linux) and *portmidi* (Windows) engines.  MIDI devices are detected,
inaccessible devices are ignored, with playback (e.g. to the Windows wavetable
synth). It is built easily via *GNU Autotools*, *Qt Creator* or *qmake*, using
*MingW*.  See the INSTALL file for build-from-source instructions for Linux or
Windows, and using a conventional source tarball.

## Recent Changes

    *   Version 0.98.10:
        *   Revisited issue #83, improved GUI editing of control/display
            automation.
        *   Fixes for issue #87: segfault due to mute-group on larger set-sizes,
            inability to modify some usr options in Edit / Preferences, and
            related bugs found during these fixes. Made performer the owner of
            mutegroups.
        *   Fixes for issue #88: 4/16 pattern not shown/played properly until
            opened in editor.
        *   Fixed minor issue with port-naming, port-lists.
        *   Many tweaks to documentation, vim files, midibytes....
    *   Version 0.98.9.1:
        *   Added files needed for ./configure.
        *   Documentation and tutorial updates.
    *   Version 0.98.9:
        *   Fixed nasty issue #85 which was recreating the slot buttons, and
            in a different thread, leading to a seqfault.
        *   Fixed issue: Preference / MIDI Input port check did not change
            Apply state.
        *   Fixed mute/record/thru state display between live grid and a
            pattern editor.
        *   Updated/cleaned the tutorial.
    *   Version 0.98.8:
        *   Fixed issue #84, now able to build Qt and CLI version in one pass.
            Also fixed out-of-source builds and removed function call tracing.
            Streamlined the bootstrap script; it was always "configuring".
        *   Changed the Apply button from Edit / Preferences to a Restart
            button.  Further tightening of change detection.
        *   Moved midibyte/midiboolean functions from strfunctions to midibytes
            and mutegroup. Much header-file cleanup. Do a --full-clean!
        *   Added Ctrl-Home and Ctrl-End support to the song editor.
        *   Added an initial HTML tutorial and commands to access it.  Add
            shellexecute module to replace the ill-performing QDesktopServices.
        *   Important configure.ac/Makefile.am upgrades.
    *   Version 0.98.7:
        *   Fixed issue #80 where some MIDI controls were getting recorded.
        *   Fixed issue #81, adding <stdexcept> to code catching
            std::invalid_argument.
        *   Fixed issue #83 where parsing 'rc' port lines failed with a
            port name having a trailing space. Also fixed short-port-name
            detection.
        *   Added a "Pattern Fix" dialog to allow a whole pattern to be shifted,
            quantized, and changed in length all at once. Useful for fixing a
            badly played pattern or scaling the duration.
        *   Fixed the issue of leftover child windows of qseqeditframe64.
        *   Removed odd beat-widths from time signature dropdowns.  Can
            still enter odd values manually, but unsupported by MIDI format.
        *   Refactored drop-down lists to use the settings module.
        *   Tightend string_to_xxx() functions and replaced the std::stoxxx()
            functions to avoid throwing exceptions.
        *   Trying to get rolls, time, data, and event panes to line up no
            matter what the Qt theme is. Difficult.
        *   Tightening the setting/clearing of performer modification re
            sequence changes to reduce unnecessary prompts to save, flag
            modification via the asterisk change marker, and enable/disable the
            File / Save option in the correct way.
        *   Tightened the saving of WRK and MIDI files.
        *   Major refactoring to replace seq::pointers with seq::refs in many
            places.
        *   Updated the bootstrap script to make 'release' the default, and
            fixed the portmidi automake build process for Linux.

    See the "NEWS" file for changes in earlier versions.

// vim: sw=4 ts=4 wm=2 et ft=markdown
