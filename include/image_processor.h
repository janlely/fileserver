#ifndef _IMAGE_PROCESSOR_H
#define _IMAGE_PROCESSOR_H

#include <wand/MagickWand.h>

#define ThrowWandException(wand) \
{ \
  char \
    *description; \
 \
  ExceptionType \
    severity; \
 \
  description=MagickGetException(wand,&severity); \
  (void) fprintf(stderr,"%s %s %lu %s\n",GetMagickModule(),description); \
  description=(char *) MagickRelinquishMemory(description); \
}

int read_image_from_btyes(void *file, size_t file_length, MagickWand *wand);

void calc_new_image_size(size_t w, size_t h, size_t *w_new, size_t *h_new, size_t boundry);

int scale_image_to_blob(MagickWand *wand, void **blob, size_t *size, size_t width, size_t height);
#endif
