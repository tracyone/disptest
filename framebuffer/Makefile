PROG := fbbmp fb_draw_rect fb_pan_display fb_pan_display_rotate fb_blank

all:${PROG}
	@echo "Start build fbtest"

fbbmp:fbbmp.c fbcommon.c
	$(CC) -Wall $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

fb_draw_rect:fb_draw_rect.c fbcommon.c
	$(CC) -Wall $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

fb_pan_display:fb_pan_display_test.c fbcommon.c
	$(CC) -Wall $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

fb_pan_display_rotate:fb_pan_display_rotate.c fbcommon.c
	$(CC) -Wall $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

fb_blank:fb_blank.c fbcommon.c
	$(CC) -Wall $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	@echo "Start clean"
	-@rm -f ${PROG}
	-@rm -f *.o
install:
	-@mkdir -p ${OUTDIR}/fbtest/
	-mv ${PROG} ${OUTDIR}/fbtest/

.PHONY:clean all install

