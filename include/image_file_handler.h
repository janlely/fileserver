#ifndef _IMAGE_FILE_HANDLER_H
#define _IMAGE_FILE_HANDLER_H

#include <stdlib.h>

typedef struct {
    const void *original_img_bytes;
    size_t original_img_length;
    const void *large_img_bytes;
    size_t large_image_length;
    const void *middle_img_bytes;
    size_t middle_img_length;
    const void *small_img_bytes;
    size_t small_img_length;
} olms;
void handler_image_file(const char *param, const void *file, size_t file_length, char *response);

olms scale_image_four(const void *file, size_t file_length);
#endif
