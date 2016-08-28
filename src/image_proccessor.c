#include "image_proccessor.h"


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
