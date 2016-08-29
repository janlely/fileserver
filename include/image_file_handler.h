#ifndef _IMAGE_FILE_HANDLER_H
#define _IMAGE_FILE_HANDLER_H

#include <stdlib.h>

typedef struct {
    void *original_img_bytes;
    size_t original_img_length;
    size_t original_img_width;
    size_t original_img_height;

    void *large_img_bytes;
    size_t large_img_length;
    size_t large_img_width;
    size_t large_img_height;

    void *middle_img_bytes;
    size_t middle_img_length;
    size_t middle_img_width;
    size_t middle_img_height;

    void *small_img_bytes;
    size_t small_img_length;
    size_t small_img_width;
    size_t small_img_height;
} image_blobs;

typedef struct {
    char *original_link;
    char *large_link;
    char *middle_link;
    char *small_link;
} image_links;
void handler_image_file(const char *param, const void *file, size_t file_length, char *response);

void scale_image_four(const void *file, size_t file_length, image_blobs *blobs);

int write_image_four(const image_blobs *blobs, image_links *links, const char *suffix);
#endif
