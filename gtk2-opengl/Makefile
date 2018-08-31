GTKGL_CFLAGS := $(shell pkg-config gtk+-2.0 glib-2.0 gtkgl-2.0 --cflags)
GTKGLEXT_CFLAGS := $(shell pkg-config gtk+-2.0 glib-2.0 gtkglext-1.0 --cflags)

GTKGL_LIBS := $(shell pkg-config gtk+-2.0 glib-2.0 gtkgl-2.0 --libs)
GTKGLEXT_LIBS := $(shell pkg-config gtk+-2.0 glib-2.0 gtkglext-1.0 --libs)

all: gtkglarea_demo gtkglext_demo

gtkglarea_demo: gtkglarea_demo.c
	$(CC) $(GTKGL_CFLAGS) -o $@ $< $(GTKGL_LIBS)

gtkglext_demo: gtkglext_demo.c
	$(CC) $(GTKGLEXT_CFLAGS) -o $@ $< $(GTKGLEXT_LIBS)
