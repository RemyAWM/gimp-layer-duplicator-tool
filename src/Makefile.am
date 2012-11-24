## Process this file with automake to produce Makefile.in

bin_PROGRAMS = gimp_plugin_ldt

bindir = $(GIMP_LIBDIR)/plug-ins

gimp_plugin_ldt_SOURCES = \
	plugin-intl.h	\
	types.h		\
	enum.h		\
	interface.c	\
	interface.h	\
	main.c		\
	main.h		\
	render.c	\
	render.h	\
	hidden-image.c	\
	hidden-image.h	\
	live-preview.c	\
	live-preview.h	\
	state-image.c	\
	state-image.h	\
	ldt_list_view_operations.c	\
	ldt_list_view_operations.h	\
	ldt_tree_model_operations.c \
	ldt_tree_model_operations.h \
	ldt_operation_rotate.c		\
	ldt_operation_rotate.h		\
	ldt_operation_translate.c	\
	ldt_operation_translate.h	\
	ldt_operation.c				\
	ldt_operation.h
	

AM_CPPFLAGS = \
	-DLOCALEDIR=\""$(LOCALEDIR)"\"		\
	-DDATADIR=\""$(DATADIR)"\"

INCLUDES =\
	-I$(top_srcdir)		\
	@GIMP_CFLAGS@		\
	-I$(includedir)

LDADD = $(GIMP_LIBS)

