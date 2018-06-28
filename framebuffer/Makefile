PROG := fbbmp fb_draw_rect fb_pan_display fb_blank fbraw

all:${PROG}
	@echo "Start build fbtest"

fbbmp:fbbmp.c fbcommon.c bmp.c
	$(CC) -Wall $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

fb_draw_rect:fb_draw_rect.c fbcommon.c
	$(CC) -Wall $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

fb_pan_display:fb_pan_display.c fbcommon.c
	$(CC) -Wall $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)


fb_blank:fb_blank.c fbcommon.c
	$(CC) -Wall $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)
fbraw:fbraw.c fbcommon.c
	$(CC) -Wall $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	@echo "Start clean"
	-@rm -f ${PROG}
	-@rm -f *.o
install:
	-@mkdir -p ${OUTDIR}/fbtest/
	-scp ${PROG} ${SHARE_FOLDER}
	-mv ${PROG} ${OUTDIR}/fbtest/

.PHONY:clean all install

