
all:fbbmp fb_draw_rect fb_pan_display
	@echo "Start build fbtest"

fbbmp:fbbmp.c fbcommon.c 
	$(CC) -Wall $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

fb_draw_rect:fb_draw_rect.c fbcommon.c 
	$(CC) -Wall $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

fb_pan_display:fb_pan_display_test.c fbcommon.c 
	$(CC) -Wall $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	@echo "Start clean"
	-@rm -f fbbmp fb_draw_rect fb_pan_display
	-@rm -f *.o
install:
	-@mkdir -p ${OUTDIR}/fbtest/
	-mv fbbmp fb_draw_rect fb_pan_display ${OUTDIR}/fbtest/

.PHONY:clean all install

