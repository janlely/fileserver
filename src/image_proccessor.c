#include "image_processor.h"
#include <magick/MagickCore.h>


int read_image_from_btyes(void *file, size_t file_length, MagickWand *wand)
{
    MagickBooleanType status;
    status = MagickReadImageBlob(wand, file, file_length);
    if (status == MagickFalse){
        ThrowWandException(wand);
        return -1;
    }
    return 0;
}

void calc_new_image_size(size_t w, size_t h, size_t *w_new, size_t *h_new, size_t boundry)
{
    if(w > h){
        *w_new = boundry;
        *h_new = boundry * h / w;
    }else{
        *h_new = boundry;
        *w_new = boundry * w / h;
    }
}

int scale_image_to_blob(MagickWand *wand, void **blob, size_t *size, size_t width, size_t height)
{

    /* scale image */
    MagickScaleImage(wand, width, height);

    /* get blob */
    *blob = MagickGetImageBlob(wand, size);
    if(*blob == NULL || size == 0)
        return -1;

    return 0;
}
