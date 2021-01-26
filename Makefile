BDIR = build
TUIBDIR = $(BDIR)/tui
LIBBDIR = $(BDIR)/lib

TUISRC = $(wildcard src/*.c)
TUIOBJ = $(patsubst src/%,$(TUIBDIR)/%.o,$(TUISRC))
TUIDEP = $(addsuffix .d,$(TUIOBJ))

LIBSRC = $(wildcard lib/*.c)
LIBOBJ = $(patsubst lib/%,$(LIBBDIR)/%.o,$(LIBSRC))
LIBDEP = $(addsuffix .d,$(LIBOBJ))

TUIBIN = $(TUIBDIR)/cop
STATICLIB = $(LIBBDIR)/libcop.a

CFLAGS += -Iinclude -O3 -W -Wall -pedantic -std=c11
CPPFLAGS += -D_XOPEN_SOURCE=700 -D_DEFAULT_SOURCE

.PHONY: all tui lib clean

all: $(STATICLIB) $(TUIBIN)

tui: lib $(TUIBIN)

lib: $(STATICLIB)

$(TUIBIN): $(STATICLIB) $(TUIOBJ)
	$(CC) $(CFLAGS) -o $@ $(TUIOBJ) -L$(LIBBDIR) -lcop

$(STATICLIB): $(LIBOBJ)
	$(AR) -crs $@ $?

-include $(TUIDEP)
-include $(LIBDEP)

$(LIBBDIR)/%.o: lib/% | $(LIBBDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -MP -o $@ -c $<

$(TUIBDIR)/%.o: src/% | $(TUIBDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -MP -o $@ -c $<

$(LIBBDIR):
	mkdir -p $@

$(TUIBDIR):
	mkdir -p $@

clean:
	$(RM) -r $(BDIR)
