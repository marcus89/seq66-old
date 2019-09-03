/*
 *  This file is part of seq66.
 *
 *  seq66 is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  seq66 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with seq66; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * \file          qslivebase.cpp
 *
 *  This module declares/defines the base class for holding pattern slots.
 *
 * \library       seq66 application
 * \author        Chris Ahlstrom
 * \date          2019-06-22
 * \updates       2019-08-30
 * \license       GNU GPLv2 or above
 *
 *  This class is the Qt counterpart to the mainwid class.
 */

#include "cfg/settings.hpp"             /* seq66::usr().key_height(), etc.  */
#include "ctrl/keystroke.hpp"           /* seq66::keystroke class           */
#include "play/performer.hpp"           /* seq66::performer class           */
#include "qskeymaps.hpp"                /* mapping between Gtkmm and Qt     */
#include "qslivebase.hpp"

/*
 * Do not document a namespace, it breaks Doxygen.
 */

namespace seq66
{

/**
 *
 * \param p
 *      Provides the performer object to use for interacting with this sequence.
 *
 * \param window
 *      Provides the functional parent of this live frame.
 *
 * \param parent
 *      Provides the Qt-parent window/widget for this container window.
 *      Defaults to null.  Normally, this is a pointer to the tab-widget
 *      containing this frame.  If null, there is no parent, and this frame is
 *      in an external window.
 */

qslivebase::qslivebase (performer & p, qsmainwnd * window, QWidget * parent) :
    QFrame              (parent),
    m_performer         (p),
    m_parent            (window),
    m_font              (),
    m_bank_id           (p.playscreen_number()),            // EXPERIMENTAL
    m_mainwnd_rows      (usr().mainwnd_rows()),
    m_mainwnd_cols      (usr().mainwnd_cols()),
    m_mainwid_spacing   (usr().mainwid_spacing()),
    m_space_rows        (m_mainwid_spacing * m_mainwnd_rows),
    m_space_cols        (m_mainwid_spacing * m_mainwnd_cols),
    m_screenset_slots   (m_mainwnd_rows * m_mainwnd_cols),
    m_slot_w            (0),
    m_slot_h            (0),
    m_last_metro        (0),
    m_alpha             (0),
    m_current_seq       (sequence::unassigned()),            // mouse interaction
    m_button_down       (false),
    m_moving            (false),
    m_adding_new        (false),
    m_can_paste         (false),
    m_has_focus         (false),
    m_is_external       (is_nullptr(parent))
{
    /*
     *  int fontsize = usr().scale_size(6);
     *  m_font.setPointSize(fontsize);
     *  m_font.setBold(true);
     *  m_font.setLetterSpacing(QFont::AbsoluteSpacing, 1);
     */
}

/**
 *  Virtual destructor.
 */

qslivebase::~qslivebase()
{
    // more?
}

/**
 *  Sets the bank number retrieved from performer.
 */

void
qslivebase::set_bank ()
{
    int bank = int(perf().playscreen_number());
    set_bank(bank);
}

/**
 *  Roughly similar to mainwid::log_screenset().
 *
 * \return
 *      Returns true if the bank was successfully changed.
 */

bool
qslivebase::set_bank (int bankid, bool hasfocus)
{
    bool result = bankid != m_bank_id && perf().is_screenset_valid(bankid);
    if (result)
    {
        m_bank_id = bankid;
        if (hasfocus)
        {
            std::string bankname = perf().bank_name(bankid);
            (void) perf().set_playing_screenset(bankid);
            set_bank_values(bankname, bankid);         /* update the GUI   */
        }
        reupdate();
    }
    return result;
}

/**
 *  We rely on the caller (the parent window) to call this function only when
 *  the bank ID has actually changed.
 */

void
qslivebase::update_bank (int bank)
{
    m_has_focus = true;                                 /* widget active    */
    set_bank(bank, true);
}

/**
 *
 */

void
qslivebase::color_by_number (int i)
{
    perf().color(m_current_seq, i);
}

/**
 *
 */

bool
qslivebase::copy_seq ()
{
    bool result = perf().copy_sequence(m_current_seq);
    if (result)
        m_can_paste = true;

    return result;
}

/**
 * TODO: dialog warning that the editor is the reason this seq cant be cut
 */

bool
qslivebase::cut_seq ()
{
    bool result = perf().cut_sequence(m_current_seq);
    if (result)
        m_can_paste = true;

    return result;
}

/**
 *  If the sequence/pattern is delete-able (valid and not being edited), then
 *  it is deleted via the performer object.  Note that in seq66 the
 *  screenset::remove() function makes this check now.
 */

bool
qslivebase::delete_seq ()
{
    return perf().remove_sequence(m_current_seq);
}

/**
 *
 */

bool
qslivebase::paste_seq ()
{
    return m_can_paste ? perf().paste_sequence(m_current_seq) : false ;
}

}           // namespace seq66

/*
 * qslivebase.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
