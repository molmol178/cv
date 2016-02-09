/*
 * buffer.h
 */



#define DEFAULT_BUFFER_NUM 256


#define buffer__num(e)  (e)->buffer.num


void       buffer__open(Edit e);
long       buffer__select(Edit e,xw_object obj,XEvent *event,long i_offset);
long       buffer__close(Edit e);
image_item buffer__item(Edit e,long i);
image_item buffer__item_offset(Edit e,long i_offset);
image_item buffer__item_current(Edit e);
long       buffer__number(Edit e,long i_offset);
long       buffer__scroll(Edit e,xw_object obj,XEvent *event,long value);
void       buffer__redraw_scroll(Edit e);
long       buffer__redraw(Edit e);
void       buffer__redraw_all_images(Edit e);
void       buffer__redraw_current(Edit e);
void       buffer__redraw_image(Edit e,long i_offset);
long       buffer__kill(Edit e,long i_offset);
void       buffer__focus(Edit e,long i_offset);
void       buffer__unfocus(Edit e,long i_offset);
void       buffer__set_state(Edit e,long i_offset,long state);
void       buffer__set_current(Edit e,long i_offset);
void       buffer__swap_or_copy(Edit e,long i_offset);
void       buffer__copy(Edit e);
void       buffer__swap(Edit e);
void       buffer__copy_original(Edit e,long i_offset);
void       buffer__swap_original(Edit e,long i_offset);
void       buffer__copy_current_somewhere(Edit e);
void       buffer__copy_somewhere(Edit e,long i_offset);
void       buffer__copy_area(Edit e,long x1,long y1,long x2,long y2);
long       buffer__search_free_number(Edit e);
void       buffer__set_offset(Edit e,long offset);

