#if ! defined SEQ66_LISTSBASE_HPP
#define SEQ66_LISTSBASE_HPP

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
 * \file          listsbase.hpp
 *
 *  An abstract base class for inputslist and clockslist.
 *
 * \library       seq66 application
 * \author        Chris Ahlstrom
 * \date          2020-12-11
 * \updates       2020-12-19
 * \license       GNU GPLv2 or above
 *
 *  Defines the list of MIDI inputs and outputs (clocks).  We've combined them
 *  for "convenience".  :-)
 */

#include <string>                       /* std::string                      */
#include <vector>                       /* std::vector for I/O values       */

#include "midi/midibus_common.hpp"      /* enum class e_clock, etc.         */
#include "midi/midibytes.hpp"           /* bussbyte and other types         */

/*
 *  Do not document a namespace; it breaks Doxygen.
 */

namespace seq66
{

/**
 *  A wrapper for a vector of clocks and inputs values, as used in
 *  mastermidibus and the performer object.
 */

class listsbase
{
    friend std::string output_port_map_list ();
    friend std::string input_port_map_list ();

protected:

    /**
     *  Provides a port name and the input or output values.  Note that the
     *  clock setting will be disabled for all input values and for output
     *  values that are actually disabled by the user.
     */

    using io = struct
    {
        bool io_enabled;            /**<< The status setting for this buss. */
        e_clock out_clock;          /**<< The clock setting for this buss.  */
        std::string io_name;        /**<< The name of the I/O buss.         */
        std::string io_nick_name;   /**<< The short name of the I/O buss.   */
    };

    /**
     *  Saves the input or clock settings obtained from the "rc" (options)
     *  file so that they can be loaded into the mastermidibus once it is
     *  created.
     */

    std::vector<io> m_master_io;

public:

    listsbase () : m_master_io ()
    {
        // Nothing to do
    }

    virtual ~listsbase ()
    {
        // Nothing to do
    }

    void clear ()
    {
        m_master_io.clear();
    }

    void resize (size_t sz)
    {
        if (sz > 0)
            m_master_io.resize(sz);
    }

    int count () const
    {
        return int(m_master_io.size());
    }

    bool not_empty () const
    {
        return ! m_master_io.empty();
    }

    void set_name (bussbyte bus, const std::string & name);
    void set_nick_name (bussbyte bus, const std::string & name);
    std::string get_name (bussbyte bus, bool addnumber = false) const;
    std::string get_nick_name (bussbyte bus, bool addnumber = false) const;
    bussbyte bus_from_nick_name (const std::string & nick) const;
    std::string port_name_from_bus (bussbyte nominalbuss) const;
    void show (const std::string & tag) const;
    bool add_list_line (const std::string & line);

protected:

    std::string extract_nickname (const std::string & name) const;
    std::string e_clock_to_string (e_clock e) const;
    std::string port_map_list () const;
    std::string to_string (const std::string & tag) const;
    bool add (const std::string & name, const std::string & nickname);

};              // class listsbase

}               // namespace seq66

#endif          // SEQ66_LISTSBASE_HPP

/*
 * listsbase.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */

