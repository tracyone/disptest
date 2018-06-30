PROG := fbbmp fb_draw_rect fb_pan_display fb_blank fbraw fb_cap

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

fb_cap:fb_cap.c bmp.c fbcommon.c
	$(CC) -Wall $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	@echo "Start clean"
	-@rm -f ${PROG}
	-@rm -f *.o
install:
	-@mkdir -p ${OUT_DIR}/fbtest/
	-mv ${PROG} ${OUT_DIR}/fbtest/

.PHONY:clean all install

