## Makefile.am -- Process this file with automake to produce Makefile.in
## Copyright (C) 2002 James Su
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

MAINTAINERCLEANFILES	= Makefile.in

INCLUDES		= -I$(top_builddir) \
	    		  -I$(top_srcdir) \
			  -I$(top_srcdir)/src \
			  -DSCIM_ICONDIR=\"@SCIM_ICONDIR@\" \
			  -DSCIM_WNN_LOCALEDIR=\"$(datadir)/locale\" \
			  -DSCIM_WNN_DATADIR=\"@SCIM_DATADIR@/scim-wnn\" \
			  -DSCIM_WNN_ICON_FILE=\"@SCIM_ICONDIR@/scim-wnn.png\"

noinst_HEADERS = scim_wnn_imengine.h wnnproto.h romkan.h romkan_table.h \
				wnnconversion.h scim_wnn_def.h preeditor.h kanainput.h convertor.h 

moduledir		= @SCIM_MODULEDIR@/IMEngine
module_LTLIBRARIES	= wnn.la

wnn_la_SOURCES = scim_wnn_imengine.cpp romkan.cpp wnnconversion.cpp \
			preeditor.cpp kanainput.cpp convertor.cpp 

wnn_la_CFLAGS		= @SCIM_CFLAGS@ @SCIM_DEBUG_FLAGS@
wnn_la_CXXFLAGS		= @SCIM_CFLAGS@ @SCIM_DEBUG_FLAGS@

wnn_la_LDFLAGS = -avoid-version -module -rpath $(moduledir) \
	@LIBTOOL_EXPORT_OPTIONS@ @INTLLIBS@ @SCIM_LIBS@

if SCIM_BUILD_SETUP
SETUP_MODULE = wnn-imengine-setup.la
endif

setupmoduledir           = @SCIM_MODULEDIR@/SetupUI
setupmodule_LTLIBRARIES  = $(SETUP_MODULE)

wnn_imengine_setup_la_SOURCES = scim_wnn_imengine_setup.cpp

wnn_imengine_setup_la_CFLAGS  = @SCIM_GTKUTILS_CFLAGS@ @SCIM_CFLAGS@ @SCIM_DEBUG_FLAGS@

wnn_imengine_setup_la_CXXFLAGS= @SCIM_GTKUTILS_CFLAGS@ @SCIM_CFLAGS@ @SCIM_DEBUG_FLAGS@

wnn_imengine_setup_la_LDFLAGS = -avoid-version \
				   -rpath $(setupmoduledir) \
				   -module \
				   @LIBTOOL_EXPORT_OPTIONS@ \
				   @SCIM_GTKUTILS_LIBS@ \
				   @INTLLIBS@ \
				   @SCIM_LIBS@
wnn_la_LIBADD = 
