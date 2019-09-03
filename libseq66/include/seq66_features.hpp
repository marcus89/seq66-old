#if ! defined SEQ66_FEATURES_HPP
#define SEQ66_FEATURES_HPP

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
 * \file          seq66_features.hpp
 *
 *    This module summarizes or defines all of the configure and build-time
 *    options available for Seq66.
 *
 * \library       seq66 application
 * \author        Chris Ahlstrom
 * \date          2018-11-24
 * \updates       2019-09-01
 * \license       GNU GPLv2 or above
 *
 *    Some options (the "USE_xxx" options) specify experimental and
 *    unimplemented features.  Some options (the "SEQ66_xxx" options)
 *    might be experimental, or not, but are definitely supported, if defined,
 *    and may become configure-time options.
 *
 *    Some options are available (or can be disabled) by running the
 *    "configure" script generated using the configure.ac file.  These
 *    options are things that a normal user or a seq66 aficianado might want to
 *    disable.  They are defined as desired, in the auto-generated
 *    seq66-config.h file in the top-level "include" directory.
 *
 *    The rest of the options can be modified only by editing the source code
 *    (soon to be this file) to enable or disable features.  These options are
 *    those that we feel more strongly about.
 *
 *    Currently, we've tested all the experimental options to the extent of
 *    building them successfully.  However, enabling them is currently
 *    TEMPORARY; we want to build with them all disabled, and enable them
 *    one-by-one in a controlled, tested manner.
 */

#include <string>

#include "seq66_features.h"             /* the C-compatible definitions     */

/*
 * This is the main namespace of Seq66.  Do not attempt to
 * Doxygenate the documentation here; it breaks Doxygen.
 */

namespace seq66
{

/*
 * Global (free) functions.
 */

extern void set_app_name (const std::string & aname);
extern void set_client_name (const std::string & cname);
extern const std::string & seq_app_name ();
extern const std::string & seq_client_name ();
extern const std::string & seq_version ();
extern const std::string & seq_version_text ();
extern const std::string & seq_app_tag ();
extern std::string seq_build_details ();

}           // namespace seq66

#endif      // SEQ66_FEATURES_HPP

/*
 * seq66_features.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */
