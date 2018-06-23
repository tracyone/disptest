
all:fbbmp fb_draw_rect
	@echo "Start build fbtest"

fbbmp:fbbmp.c fbcommon.c 
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

fb_draw_rect:fb_draw_rect.c fbcommon.c 
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	@echo "Start clean"
	-@rm -f fbbmp fb_draw_rect
	-@rm -f *.o

install:
	-@mkdir -p ${OUTDIR}/fbtest/
	-mv fbbmp fb_draw_rect ${OUTDIR}/fbtest/

.PHONY:clean all install

