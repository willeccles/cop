#ifndef _COP_LIBCOP_H_
#define _COP_LIBCOP_H_

#include <stdbool.h>
#include <stddef.h>

/* Structure representing a cop instance */
struct cop_t {
  // x and y positions of the top left corner (starting from 0)
  size_t x;
  size_t y;

  // cop width and height
  size_t width;
  size_t height;

  // items to be displayed in the list
  const char** elements;
  // number of elements
  size_t nelem;

  /* do not modify */
  size_t _selection;
  size_t _cursorpos;
  bool _redraw;
};

// returns zero on success or a negative value on error
// sets errno in the case of an error
int cop_init(struct cop_t* cop);

// returns zero on success or a negative value on error
// sets errno on error
int cop_redraw(struct cop_t* cop);

int cop_resize(struct cop_t* cop,
               size_t x, size_t y,
               size_t width, size_t height);

void cop_cursor_up(struct cop_t* cop);
void cop_cursor_down(struct cop_t* cop);
void cop_cursor_set(struct cop_t* cop, size_t pos);

// returns the selected item index
size_t cop_select(struct cop_t* cop);

#endif // _COP_LIBCOP_H_
