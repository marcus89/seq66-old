#*****************************************************************************
# Makefile.am (seq_rtmidi)
#-----------------------------------------------------------------------------
##
# \file          Makefile.am
# \library       seq_rtmidi
# \author        Chris Ahlstrom
# \date          2016-11-19
# \updates       2016-11-19
# \version       $Revision$
# \license       $MIDICVT_SUITE_GPL_LICENSE$
#
#  	This file is a makefile for the libseq66 library project.  This
#  	makefile provides the skeleton needed to build the libseq66 project
#  	directory using GNU autotools.
#
#-----------------------------------------------------------------------------

#*****************************************************************************
# Packing targets.
#-----------------------------------------------------------------------------
#
#		Always use Automake in foreign mode (adding foreign to
#		AUTOMAKE_OPTIONS in Makefile.am). Otherwise, it requires too many
#		boilerplate files from the GNU coding standards that aren't useful to
#		us. 
#
#-----------------------------------------------------------------------------

AUTOMAKE_OPTIONS = foreign dist-zip dist-bzip2
MAINTAINERCLEANFILES = Makefile.in Makefile $(AUX_DIST)

#*****************************************************************************
# EXTRA_DIST
#-----------------------------------------------------------------------------

EXTRA_DIST =

#*****************************************************************************
# SUBDIRS
#-----------------------------------------------------------------------------

SUBDIRS = include src

#*****************************************************************************
# DIST_SUBDIRS
#-----------------------------------------------------------------------------
#
#      DIST_SUBDIRS is used by targets that need to recurse into /all/
#      directories, even those which have been conditionally left out of the
#      build.
#
#      Precisely, DIST_SUBDIRS is used by:
#
#         -   make dist
#         -   make distclean
#         -   make maintainer-clean.
#
#      All other recursive targets use SUBDIRS.
#
#-----------------------------------------------------------------------------

DIST_SUBDIRS = $(SUBDIRS)

#*****************************************************************************
# all-local
#-----------------------------------------------------------------------------

all-local:
	@echo "Top source-directory 'top_srcdir' is $(top_srcdir)"
	@echo "* * * * * All libseq_rtmidi build items completed * * * * *"

#*****************************************************************************
# Makefile.am (seq_rtmidi)
#-----------------------------------------------------------------------------
# vim: ts=3 sw=3 noet ft=automake
#-----------------------------------------------------------------------------
