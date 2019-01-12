#
# Source level Makefile for JVips.
#
# NOTE: Define DESTDIR if you are not installing JVips
#       to its final directories. This is meant for the
#       purpose of packaging.
#
MAKE_PATH = ../../../..
include $(MAKE_PATH)/Make.def

# JNI specifications
JNI_LIB_NAME    =	JVips
JNI_SRC_FILES   =	Vips.java

JNI_LINK_LIBS   =	$(VIPS_LIBS)

INCLUDES        =	$(JAVA_INCLUDES) $(VIPS_INCLUDES)

CLEAN_LIST	=	$(JNI_LIB) $(LIB_DIR)/.libs

include $(MAKE_PATH)/Make.rules

extra-install::
	$(INSTALL) -d $(DESTDIR)$(libdir)
	$(LIBTOOL) --mode=install $(INSTALL) $(JNI_LIB) $(DESTDIR)$(libdir)
