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
 * \file          qperfbase.cpp
 *
 *  This module declares/defines the base class for drawing on the piano
 *  roll of the song editor.
 *
 * \library       seq66 application
 * \author        Chris Ahlstrom
 * \date          2018-07-14
 * \updates       2019-08-06
 * \license       GNU GPLv2 or above
 *
 *  We are currently moving toward making this class a base class.
 *
 *  User jean-emmanual added support for disabling the following of the
 *  progress bar during playback.  See the seqroll::m_progress_follow member.
 */

#include "play/performer.hpp"           /* seq66::performer class           */
#include "qperfbase.hpp"

/*
 *  Do not document a namespace; it breaks Doxygen.
 */

namespace seq66
{

/**
 *
 */

qperfbase::qperfbase
(
    performer & p,
    int zoom,
    int snap,
    int unitheight,
    int totalheight
) :
    qeditbase
    (
        p, zoom, snap, c_perf_scale_x, 0, unitheight, totalheight
    )
{
    // no code
}

/**
 *
 */

int
qperfbase::horizSizeHint () const
{
    int hint = perf().get_max_trigger() / scale_zoom() + 2000;
    return hint;
}

#if 0

/**
 *  Handles changes to the PPQN value in one place.  Useful mainly at startup
 *  time for adjusting the user-interface zooming.
 *
 *  The m_ticks_per_bar member replaces the global ppqn times 16.  This
 *  construct is parts-per-quarter-note times 4 quarter notes times 4
 *  sixteenth notes in a bar.  (We think...)
 *
 *  The m_scale member starts out at c_perf_scale_x, which is 32 ticks
 *  per pixel at the default tick rate of 192 PPQN.  We adjust this now.
 *  But note that this calculation still involves the c_perf_scale_x constant.
 *
 * \todo
 *      Resolve the issue of c_perf_scale_x versus m_scale in perfroll.
 */

bool
qperfbase::set_ppqn (int p)
{
    bool result = qbase::set_ppqn(p);
    if (result)
    {
        m_scale_zoom = zoom() * m_scale;
        m_beat_length = ppqn();
        m_measure_length = m_beat_length * 4;       /* what about beat width? */
    }
    return result;
}

#endif

/**
 *
 */

void
qperfbase::convert_x (int x, midipulse & tick)
{
    midipulse tick_offset = 0;                  // it's always this!!!
    tick = x * m_scale_zoom;
    tick += tick_offset;
}

/**
 *
 */

void
qperfbase::convert_xy (int x, int y, midipulse & tick, int & seq)
{
    //  tick = x * m_zoom;
    //  seq = (m_total_height - y - 2) / m_unit_height;

    midipulse tick_offset =  0;                 // again, always 0!!!
    tick = x * m_scale_zoom;
    seq = y / c_names_y;
    tick += tick_offset;
    if (seq >= c_max_sequence)
        seq = c_max_sequence - 1;

    if (seq < 0)
        seq = 0;
}

/**
 *
 */

void
qperfbase::convert_ts (midipulse ticks, int seq, int & x, int & y)
{
    x = tix_to_pix(ticks);
    y = m_total_height - ((seq + 1) * m_unit_height) - 1;
}

/**
 *
 * \param tick_s
 *      The starting tick of the rectangle.
 *
 * \param tick_f
 *      The finishing tick of the rectangle.
 *
 * \param seq_h
 *      The high sequence row of the rectangle.
 *
 * \param seq_l
 *      The low sequence row of the rectangle.
 *
 * \param [out] r
 *      The destination rectangle for the calculations.
 */

void
qperfbase::convert_ts_box_to_rect
(
    midipulse tick_s, midipulse tick_f, int seq_h, int seq_l,
    seq66::rect & r
)
{
    int x1, y1, x2, y2;
    convert_ts(tick_s, seq_h, x1, y1);         /* convert box to X,Y values */
    convert_ts(tick_f, seq_l, x2, y2);
    rect::xy_to_rect(x1, y1, x2, y2, r);
    r.height_incr(m_unit_height);
}

}           // namespace seq66

/*
 * qperfbase.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
