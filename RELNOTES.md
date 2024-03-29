# Seq66 Release Notes 0.98.10 - 2022-07-18

This file lists __major__ changes from version 9.98.0 to 0.98.10 (to catch up).

## Changes

*   Version 0.98.10:
    *   Revisited issue #83: [midi-input]: in-bus data line error, to improve
        GUI editing of automation port.
    *   Fixed issue #87: Segfault due to mute-group on larger set-sizes.
    *   Fixed issue #88: 4/16 pattern not shown/played properly at first.
*   Version 0.98.9:
    *   Fixed issue #85: Seqfault from recreating slot buttons in thread.
    *   Improved: Change-detection in Edit / Preferences.
    *   Improved: Cross-dialog change detection (e.g.  mute/record/thru display)
*   Version 0.98.8:
    *   Fixed issue #84: Now able to build Qt and CLI version in one pass.
    *   Added: "Restart" button to Edit / Preferences.
    *   Added: Ctrl-Home and Ctrl-End support to the song editor.
    *   Added: An HTML tutorial and buttons to access it and the user manual.
        See [Ahlstrom Projects](https://ahlstromcj.github.io/).
*   Version 0.98.7:
    *   Fixed issue #80: some MIDI controls were getting recorded.
    *   Fixed issue #81: added code to catch bad numeric conversions.
    *   Fixed issue #83: parsing 'rc' port lines with trailing space failed.
    *   Added: "Pattern Fix" tool to allow a multiple changes to a pattern.
*   Version 0.98.6:
    *   Revisited issue #41: to make sure "Quit" is "Hide" under NSM.
*   Version 0.98.5:
    *   Added: Locking for the event-drawing loops to prevent segfaults.
*   Version 0.98.4:
    *   Fixed issue #75: some metadata problems preventing use of app icons.
*   Version 0.98.3:
    *   Fixed issue #76: broken MIDI Start handling.
*   Version 0.98.2:
    *   Fixed issue #74: -1 for "no buss-override" was being converted to 0.
*   Version 0.98.1:
    *   Fixes: Song-editor set-names display; segfault in --help option; more.
*   Version 0.98.0:
    *   Fixed issue #41: Hide Seq66 on closing window in an NSM session.
    *   Fixed issue #73: Compile error because of jack_get_version_string.
    *   Added: "MIDI macros" to the 'ctrl' file.  Can send SysEx, etc.

## Final Notes

All too many bug fixes, code refactoring, minor improvements, too many to
cover. Read the NEWS and README files.  Never-ending!

