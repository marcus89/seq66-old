/*
 *  This file is part of seq66.
 *
 *  seq66 is free software; you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the Free Software
 *  Foundation; either version 2 of the License, or (at your option) any later
 *  version.
 *
 *  seq66 is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with seq66; if not, write to the Free Software Foundation, Inc., 59 Temple
 *  Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * \file          qpatternfix.cpp
 *
 *  This module declares/defines the base class for the pattern-fix window.
 *
 * \library       seq66 application
 * \author        Chris Ahlstrom
 * \date          2022-04-09
 * \updates       2022-04-20
 * \license       GNU GPLv2 or above
 *
 *  This dialog provides a way to combine the following pattern adjustments:
 *
 *      -   Left-alignment.
 *      -   Fitting to a given number of measures.
 *      -   Arbitrary scaling for compression and expansion over time.
 *      -   Quantization or tightening.
 *
 *  It acts on all events in the track; no selection needed.  In this, it is
 *  similar to the LFO dialog.
 *
 *  This dialog was inspired by Ahlstrom's poor playing and timing skills.
 *
 */

#include <QButtonGroup>

#include "seq66-config.h"               /* defines SEQ66_QMAKE_RULES        */
#include "play/performer.hpp"           /* seq66::performer class           */
#include "qpatternfix.hpp"              /* seq66::qpatternfix class         */
#include "qseqdata.hpp"                 /* seq66::qseqdata for status, CC   */
#include "qstriggereditor.hpp"          /* seq66::qstriggereditor class     */
#include "qseqeditframe64.hpp"          /* seq66::qseqeditframe64, parent   */
#include "qt5_helper.h"                 /* QT5_HELPER_RADIO_SIGNAL macro    */
#include "qt5_helpers.hpp"              /* seq66::qt() string conversion    */
#include "util/calculations.hpp"        /* seq66::wave enum class values    */
#include "util/strfunctions.hpp"        /* seq66::string_to_double()        */

/*
 *  Qt's uic application allows a different output file-name, but not sure
 *  if qmake can change the file-name.
 */

#if defined SEQ66_QMAKE_RULES
#include "forms/ui_qpatternfix.h"
#else
#include "forms/qpatternfix.ui.h"
#endif

/*
 * Don't document the namespace.
 */

namespace seq66
{

/**
 *  Static members.
 */

static const double c_scale_min =   0.01;
static const double c_scale_max = 100.00;

/*
 *  Signal buttonClicked(int) is overloaded in this class. To connect to this
 *  signal by using the function pointer syntax, Qt provides a convenient
 *  helper for obtaining the function pointer as used below, using a lambda
 *  function.
 */

qpatternfix::qpatternfix
(
    performer & p,
    seq::pointer seqptr,
    qseqeditframe64 * editparent,
    QWidget * parent
) :
    QFrame              (parent),
    ui                  (new Ui::qpatternfix),
    m_fixlength_group   (nullptr),
    m_quan_group        (nullptr),
    m_performer         (p),
    m_seq               (seqptr),
    m_backup_events     (seqp()->events()),             /* for slot_reset() */
    m_backup_measures   (seqp()->get_measures()),
    m_backup_beats      (seqp()->get_beats_per_bar()),
    m_backup_width      (seqp()->get_beat_width()),
    m_edit_frame        (editparent),
    m_length_type       (lengthfix::none),
    m_quan_type         (quantization::none),
    m_measures          (double(m_backup_measures)),
    m_scale_factor      (1.0),
    m_align_left        (false),
    m_save_note_length  (true),
    m_use_time_sig      (false),
    m_time_sig_beats    (0),
    m_time_sig_width    (0),
    m_is_modified       (false)
{
    ui->setupUi(this);
    initialize(true);
}

/**
 *  Deletes the user-interface object.
 */

qpatternfix::~qpatternfix()
{
    delete ui;

    /*
     *  if (m_is_modified)
     *      perf().modify();
     */
}

void
qpatternfix::initialize (bool startup)
{
    std::string value = std::to_string(int(seqp()->get_length()));
    ui->label_pulses->setText(qt(value));
    value = std::to_string(int(m_measures));
    ui->line_edit_pick->setText(qt(value));
    value = std::to_string(1);                          /* actually a float */
    ui->line_edit_scale->setText(qt(value));
    ui->btn_effect_shift->setChecked(false);
    ui->btn_effect_shrink->setChecked(false);
    ui->btn_effect_expand->setChecked(false);
    ui->btn_effect_time_sig->setChecked(false);
    ui->btn_align_left->setChecked(false);
    ui->btn_save_note_length->setChecked(true);
    ui->btn_set->setEnabled(false);
    ui->btn_reset->setEnabled(false);
    if (startup)
    {
        /*
         * Read-only labelling. Pattern number and pattern length.
         */

        std::string value = std::to_string(int(seqp()->seq_number()));
        ui->label_pattern->setText(qt(value));

        std::string plabel = "Pattern #";
        plabel += value;
        setWindowTitle(qt(plabel));

        /*
         * Length (Measures or Scaling) Change.
         */

        m_fixlength_group = new QButtonGroup(this);
        m_fixlength_group->addButton(ui->btn_change_none, cast(lengthfix::none));
        m_fixlength_group->addButton
        (
            ui->btn_change_pick, cast(lengthfix::measures)
        );
        m_fixlength_group->addButton
        (
            ui->btn_change_scale, cast(lengthfix::rescale)
        );
        ui->btn_change_none->setChecked(true);
        connect
        (
            m_fixlength_group, QT5_HELPER_RADIO_SIGNAL,
            [=](int id) { slot_length_fix(id); }        /* lambda function  */
        );

        connect
        (
            ui->line_edit_pick, SIGNAL(editingFinished()),
            this, SLOT(slot_measure_change())
        );
        connect
        (
            ui->line_edit_scale, SIGNAL(editingFinished()),
            this, SLOT(slot_scale_change())
        );
        ui->line_edit_none->hide();                     /* unused, hide it  */

        /*
         * Quantization.
         */

        m_quan_group = new QButtonGroup(this);
        ui->group_box_quantize->setEnabled(true);
        m_quan_group->addButton(ui->btn_quan_none, cast(quantization::none));
        m_quan_group->addButton
        (
            ui->btn_quan_tighten, cast(quantization::tighten)
        );
        m_quan_group->addButton(ui->btn_quan_full, cast(quantization::full));
        ui->btn_quan_none->setChecked(true);
        connect
        (
            m_quan_group, QT5_HELPER_RADIO_SIGNAL,
            [=](int id) { slot_quan_change(id); }       /* lambda function  */
        );

        /*
         * The "read-only" Effect group.  The slot here merely keeps them from
         * being checked by the user.  If we make them readonly they text is
         * difficult to read in many Qt themes.
         */

        connect
        (
            ui->btn_effect_shift, SIGNAL(clicked()), this, SLOT(slot_effect())
        );
        connect
        (
            ui->btn_effect_shrink, SIGNAL(clicked()), this, SLOT(slot_effect())
        );
        connect
        (
            ui->btn_effect_expand, SIGNAL(clicked()), this, SLOT(slot_effect())
        );
        connect
        (
            ui->btn_effect_time_sig, SIGNAL(clicked()), this, SLOT(slot_effect())
        );

        /*
         * Other Fixes.
         */

        ui->group_box_effect->setEnabled(true);
        ui->group_box_other->setEnabled(true);
        connect
        (
            ui->btn_align_left, SIGNAL(stateChanged(int)),
            this, SLOT(slot_align_change(int))
        );
        connect
        (
            ui->btn_save_note_length, SIGNAL(stateChanged(int)),
            this, SLOT(slot_save_note_length(int))
        );

        /*
         * Bottom buttons.
         */

        ui->btn_close->setEnabled(true);
        connect(ui->btn_set, SIGNAL(clicked()), this, SLOT(slot_set()));
        connect(ui->btn_reset, SIGNAL(clicked()), this, SLOT(slot_reset()));
        connect(ui->btn_close, SIGNAL(clicked()), this, SLOT(close()));
    }
    else
    {
        /*
         * Grouped buttons.
         */

        ui->btn_change_none->setChecked(true);
        ui->btn_quan_none->setChecked(true);
    }
}

void
qpatternfix::modify ()
{
    ui->btn_set->setEnabled(true);
    ui->btn_reset->setEnabled(true);
    m_is_modified = true;
}

/**
 *  All this does is ...
 *  because we don't want to force the user to have to modify anything to
 *  Set the values again.
 */

void
qpatternfix::unmodify (bool reset_fields)
{
    if (reset_fields)
    {
        std::string temp = std::to_string(seqp()->get_measures());
        ui->line_edit_pick->setText(qt(temp));
        ui->line_edit_scale->setText("1.0");
        ui->btn_effect_shift->setChecked(false);
        ui->btn_effect_shrink->setChecked(false);
        ui->btn_effect_expand->setChecked(false);
        ui->btn_reset->setEnabled(false);
    }

    /*
     * ui->btn_set->setEnabled(false);
     */

    m_is_modified = false;
}

void
qpatternfix::slot_effect ()
{
    ui->btn_effect_shift->setChecked(false);
    ui->btn_effect_shrink->setChecked(false);
    ui->btn_effect_expand->setChecked(false);
    ui->btn_effect_time_sig->setChecked(false);
}

void
qpatternfix::slot_length_fix (int fixlengthid)
{
    m_length_type = lengthfix_cast(fixlengthid);
    if (m_length_type != lengthfix::none)
        modify();
}

void
qpatternfix::slot_measure_change ()
{
    QString t = ui->line_edit_pick->text();
    std::string tc = t.toStdString();
    double m = string_to_double(tc, 1.0);
    if (m != m_measures)
    {
        int beats, width;
        bool is_time_sig = string_to_time_signature(tc, beats, width);
        ui->btn_change_pick->setChecked(true);
        m_measures = m;
        m_length_type = lengthfix::measures;
        m_time_sig_beats = beats;
        m_time_sig_width = width;
        m_use_time_sig = is_time_sig;
        ui->btn_effect_time_sig->setChecked(is_time_sig);
        modify();
    }
    else
    {
        int beats, width;
        bool is_time_sig = string_to_time_signature(tc, beats, width);
        ui->btn_effect_time_sig->setChecked(is_time_sig);
    }
}

void
qpatternfix::slot_scale_change ()
{
    QString t = ui->line_edit_scale->text();
    std::string tc = t.toStdString();
    double v = string_to_double(tc, 1.0);
    if (v >= c_scale_min && v <= c_scale_max)
    {
        if (v != m_scale_factor)
        {
            ui->btn_change_scale->setChecked(true);
            m_scale_factor = v;
            m_length_type = lengthfix::rescale;
            m_time_sig_beats = m_time_sig_width = 0;
            m_use_time_sig = false;
            ui->btn_effect_time_sig->setChecked(false);
            modify();
        }
    }
}

void
qpatternfix::slot_quan_change (int quanid)
{
    m_quan_type = quantization_cast(quanid);
    if (m_quan_type != quantization::none)
        modify();
}

void
qpatternfix::slot_align_change (int state)
{
    bool is_set = state == Qt::Checked;
    bool changed = is_set != m_align_left;
    if (changed)
    {
        m_align_left = is_set;
        modify();
    }
}

void
qpatternfix::slot_save_note_length (int state)
{
    bool is_set = state == Qt::Checked;
    bool changed = is_set != m_save_note_length;
    if (changed)
    {
        m_save_note_length = is_set;
        modify();
    }
}

/**
 *  A convenience/encapsulation function to trigger redrawing.
 */

void
qpatternfix::set_dirty ()
{
    seqp()->set_dirty();
    if (not_nullptr(m_edit_frame))
        m_edit_frame->set_dirty();
}

void
qpatternfix::slot_set ()
{
    fixeffect efx;
    fixparameters fp =                                  /* value structure  */
    {
        m_length_type, m_quan_type,
        m_align_left, m_save_note_length,
        m_use_time_sig, m_time_sig_beats, m_time_sig_width,
        m_measures, m_scale_factor, efx
    };
    bool success = seqp()->fix_pattern(fp);             /* side-effects     */
    if (success)
    {
        bool bitshifted = bit_test(efx, fixeffect::shifted);
        bool bitshrunk = bit_test(efx, fixeffect::shrunk);
        bool bitexpanded = bit_test(efx, fixeffect::expanded);
        std::string temp = std::to_string(int(seqp()->get_length()));
        ui->label_pulses->setText(qt(temp));
        if (m_use_time_sig)
        {
            temp = std::to_string(m_time_sig_beats);
            temp += "/";
            temp += std::to_string(m_time_sig_width);
        }
        else
            temp = double_to_string(m_measures);

        ui->line_edit_pick->setText(qt(temp));
        temp = std::to_string(m_scale_factor);
        ui->line_edit_scale->setText(qt(temp));
        ui->btn_effect_shift->setChecked(bitshifted);
        ui->btn_effect_shrink->setChecked(bitshrunk);
        ui->btn_effect_expand->setChecked(bitexpanded);
        set_dirty();                                    /* for redrawing    */
        perf().modify();
        unmodify(false);                                /* keep fields      */
    }
}

void
qpatternfix::slot_reset ()
{
    seqp()->set_beats_per_bar(m_backup_beats);          /* restore original */
    seqp()->set_beat_width(m_backup_width);             /* ditto            */
    seqp()->apply_length(m_backup_measures);            /* simple overload  */
    seqp()->events() = m_backup_events;                 /* restore events   */
    m_measures = double(m_backup_measures);
    m_align_left = m_use_time_sig = false;
    m_save_note_length = true;
    m_time_sig_beats = m_time_sig_width = 0;
    m_scale_factor = 1.0;
    m_length_type = lengthfix::none;
    m_quan_type = quantization::none;
    initialize(false);
    unmodify();                                         /* change fields    */
    set_dirty();                                        /* for redrawing    */
}

void
qpatternfix::closeEvent (QCloseEvent * event)
{
    if (not_nullptr(m_edit_frame))
        m_edit_frame->remove_patternfix_frame();

    /*
     * if (m_is_modified)
     *     perf().modify();
     */

    event->accept();
}

}               // namespace seq66

/*
 * qpatternfix.cpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */

