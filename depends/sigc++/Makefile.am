SUBDIRS		= . macros config

EXTRA_DIST	= README

sigc_fixed_cc	= node.cc slot.cc connection.cc signal.cc		\
		  object_slot.cc class_slot.cc object.cc adaptor.cc	\
		  bind.cc method_slot.cc

sigc_fixed_h	= adaptor.h connection.h marshal.h node.h object.h	\
		  sigc++.h trait.h
sigc_built_cc	=
sigc_built_h	= bind_return.h	\
		  object_slot.h retype_return.h slot.h bind.h		\
		  class_slot.h hide.h retype.h signal.h       \
                  method_slot.h method_slot.h

built_sources	= $(sigc_built_cc) $(sigc_built_h)
dist_sources	= $(sigc_fixed_cc) $(sigc_fixed_h)
DISTFILES	= $(DIST_COMMON) $(dist_sources) $(TEXINFOS) $(EXTRA_DIST)

# Support for DLL on cygwin/mingw using libtool > 1.4
if PLATFORM_WIN32
no_undefined = -no-undefined
else
# -no-undefined causes problems for Sun
no_undefined =
endif

# Override DEFS and DEFAULT_INCLUDES to suppress "-I. -I$(srcdir)".
DEFS = @DEFS@ -DLIBSIGC_COMPILATION
DEFAULT_INCLUDES =
INCLUDES = -I$(top_builddir) -I$(top_srcdir) -I$(top_builddir)/sigc++/config

M4_DIR			= $(top_srcdir)/sigc++/macros
M4_INCLUDES		= -I$(M4_DIR)

lib_LTLIBRARIES		= libsigc-1.2.la
libsigc_1_2_la_SOURCES	= $(sigc_built_cc) $(sigc_fixed_cc)
libsigc_1_2_la_LDFLAGS	= $(no_undefined) -version-info $(LIBSIGC_SO_VERSION) $(LIBSTDCPP_LIBS)

libsigc_includedir	= $(includedir)/sigc++-1.2/sigc++
libsigc_include_HEADERS	= $(sigc_fixed_h)
libsigc_include_DATA	= $(sigc_built_h)

# With automake-1.4*, the Makefile itself depends on $(BUILT_SOURCES).
# Thus, as a side effect the Makefile will be regenerated too.
BUILT_SOURCES = $(sigc_built_h)

%.h: $(M4_DIR)/%.h.m4 $(M4_DIR)/template.macros.m4
	$(M4) $(M4_INCLUDES) $(DEFINES) $<  > $@

distclean-local:
	rm -f $(built_sources)

if PLATFORM_WIN32
libtool_import_libs = $(lib_LTLIBRARIES:.la=.dll.a)
install-libtool-import-lib:
	@for import_lib in $(libtool_import_libs); do \
	  $(INSTALL) .libs/$$import_lib $(DESTDIR)$(libdir); \
	done
uninstall-libtool-import-lib:
	@for import_lib in $(libtool_import_libs); do \
	  -rm $(DESTDIR)$(libdir)/$$import_lib; \
	done
else
install-libtool-import-lib:
uninstall-libtool-import-lib:
endif

install-data-local: install-libtool-import-lib

uninstall-local: uninstall-libtool-import-lib
