#include "image_file_handler.h"
#include <string.h>
#include "common_utils.h"
#include "image_processor.h"
#include "errcode.h"
#include <wand/MagickWand.h>
#include "common_define.h"
#include <time.h>

void handler_image_file(const char *param, const void *file, size_t file_length, char *response)
{
    json_object *jobj = json_tokener_parse(param);
    if(json_object_get_type(jobj) != json_type_object){
        put_int_value_to_jsonstring(response, "errcode", INVALID_PARAM);
        return;
    }
    char file_name[100];
    get_string_from_jsonobject(jobj, file_name, "fileName");
    const char *suffix = strchr(file_name, '.');

    image_blobs blobs = {
        file, file_length, 0, 0,
        NULL, 0, 0, 0,
        NULL, 0, 0, 0,
        NULL, 0, 0, 0
    };

    long long start = get_current_time_mil();
    scale_image_four(file, file_length, &blobs);
    printf("info: image scale total time: %lld\n", get_current_time_mil() - start);

    image_links links;
    links.original_link = (char *)malloc(100);
    links.large_link = (char *)malloc(100);
    links.middle_link = (char *)malloc(100);
    links.small_link = (char *)malloc(100);

    start = get_current_time_mil();
    if(write_image_four(&blobs, &links, suffix)){
        printf("info: image file writing total time: %lld\n", get_current_time_mil() - start);
        put_int_value_to_jsonstring(response, "errcode", RESPONSE_OK);
        put_string_value_to_jsonstring(response, "original_link", links.original_link);
        put_string_value_to_jsonstring(response, "large_link", links.large_link);
        put_string_value_to_jsonstring(response, "middle_link", links.middle_link);
        put_string_value_to_jsonstring(response, "small_link", links.small_link);
    }else{
        put_int_value_to_jsonstring(response, "errcode", IMAGE_COMPRESS_ERROR );
        put_string_value_to_jsonstring(response, "original_link", links.original_link);
    }

    free(links.original_link);
    free(links.large_link);
    free(links.middle_link);
    free(links.small_link);
    /* if(blobs.original_img_bytes != NULL) */
        /* free(blobs.original_img_bytes); */
    if(blobs.large_img_bytes != NULL)
        free(blobs.large_img_bytes);
    if(blobs.middle_img_bytes != NULL)
        free(blobs.middle_img_bytes);
    if(blobs.middle_img_bytes != NULL)
        free(blobs.small_img_bytes);
}

void scale_image_four(const void *file, size_t file_length, image_blobs *blobs)
{
    MagickWand *wand = NewMagickWand();
    size_t large_img_length = 0;
    size_t middle_img_length = 0;
    size_t small_img_length = 0;

    /* read image bytes */
    if(read_image_from_btyes(file, file_length, wand) != 0){
        printf("error: can't read image!\n");
        return;
    }

    /* scale large image*/
    size_t width_org = MagickGetImageWidth(wand);
    size_t height_org = MagickGetImageHeight(wand);
    blobs->original_img_width = width_org;
    blobs->original_img_height = height_org;

    size_t width_large, height_large;
    if(width_org <= LARGE_IMAGE_BOUNDRY && height_org <= LARGE_IMAGE_BOUNDRY){
        /* blobs->large_img_bytes = file; */
        /* blobs->large_img_length = file_length; */
        blobs->large_img_width = width_org;
        blobs->large_img_height = height_org;
    }else{
        calc_new_image_size(width_org, height_org, &width_large, &height_large, LARGE_IMAGE_BOUNDRY);
        void *tmp = NULL;
        if(scale_image_to_blob(wand, &tmp, &large_img_length, width_large, height_large) != 0){
            printf("error: cant't scale large image!\n");
            return;
        }
        blobs->large_img_bytes = malloc(large_img_length);
        memcpy(blobs->large_img_bytes, tmp, large_img_length);
        blobs->large_img_length = large_img_length;
        blobs->large_img_width = width_large;
        blobs->large_img_height = height_large;
    }

    /* scale middle img */
    size_t width_middle, height_middle;
    if(blobs->large_img_bytes == NULL){
        if(width_org <= MIDDLE_IMAGE_BOUNDRY && height_org <= MIDDLE_IMAGE_BOUNDRY){
            /* blobs->middle_img_bytes = file; */
            /* blobs->middle_img_length = file.length; */
            blobs->middle_img_width = width_org;
            blobs->middle_img_height = height_org;
        }else{
            calc_new_image_size(width_org, height_org, &width_middle, &height_middle, MIDDLE_IMAGE_BOUNDRY);
            void *tmp = NULL;
            if(scale_image_to_blob(wand, &tmp, &middle_img_length, width_middle, height_middle) != 0){
                printf("error: can't scale middle image!\n");
                return;
            }
            blobs->middle_img_bytes = malloc(middle_img_length);
            memcpy(blobs->middle_img_bytes, tmp, middle_img_length);
            blobs->middle_img_length = middle_img_length;
            blobs->middle_img_width = width_middle;
            blobs->middle_img_height = height_middle;
        }
    }else{
        /* if(width_large <= MIDDLE_IMAGE_BOUNDRY && height_large <= MIDDLE_IMAGE_BOUNDRY){ */
            /* result.middle_img_bytes = large_img; */
            /* result.middle_img_length = large_img_length; */
            /* result.middle_img_width = width_large; */
            /* result.middle_img_height = height_large; */
            calc_new_image_size(width_large, height_large, &width_middle, &height_middle, MIDDLE_IMAGE_BOUNDRY);
            void *tmp = NULL;
            if(scale_image_to_blob(wand, &tmp, &middle_img_length, width_middle, height_middle) != 0){
                printf("error: can't scale middle image!\n");
                return;
            }
            blobs->middle_img_bytes = malloc(middle_img_length);
            memcpy(blobs->middle_img_bytes, tmp, middle_img_length);
            blobs->middle_img_length = middle_img_length;
            blobs->middle_img_width = width_middle;
            blobs->middle_img_height = height_middle;
        /* } */
    }

    /* scale small img */
    size_t width_small, height_small;
    if(blobs->middle_img_bytes == NULL && blobs->large_img_bytes == NULL){
        if(width_org <= SMALL_IMAGE_BOUNDRY && height_org <= SMALL_IMAGE_BOUNDRY){
            /* blobs->small_img_bytes = file; */
            /* blobs->small_img_length = file.length; */
            blobs->small_img_width = width_org;
            blobs->small_img_height = height_org;
        }else{
            calc_new_image_size(width_org, height_org, &width_small, &height_small, SMALL_IMAGE_BOUNDRY);
            void *tmp = NULL;
            if(scale_image_to_blob(wand, &tmp, &small_img_length, width_small, height_small) != 0){
                printf("error: can't scale small image!\n");
                return;
            }
            blobs->small_img_bytes = malloc(small_img_length);
            memcpy(blobs->small_img_bytes, tmp, small_img_length);
            blobs->small_img_length = small_img_length;
            blobs->small_img_width = width_small;
            blobs->small_img_height = height_small;
        }
    }else{
        /* if(width_middle <= SMALL_IMAGE_BOUNDRY && height_middle <= SMALL_IMAGE_BOUNDRY){ */
            /* result.small_img_bytes = middle_img; */
            /* result.small_img_length = middle_img_length; */
        /* }else{ */
            calc_new_image_size(width_middle, height_middle, &width_small, &height_small, SMALL_IMAGE_BOUNDRY);
            void *tmp = NULL;
            if(scale_image_to_blob(wand, &tmp, &small_img_length, width_small, height_small) != 0){
                printf("error: can't scale middle image!\n");
                return;
            }
            blobs->small_img_bytes = malloc(small_img_length);
            memcpy(blobs->small_img_bytes, tmp, small_img_length);
            blobs->small_img_length = small_img_length;
            blobs->small_img_width = width_small;
            blobs->small_img_height = height_small;
        /* } */
    }

    wand=DestroyMagickWand(wand);
    /* MagickWandTerminus(); */

}

int write_image_four(const image_blobs *blobs, image_links *links, const char *suffix)
{
    char uuid[40];
    char tmp[20];
    get_uuid(uuid);
    char base_path[100];
    get_base_path(base_path, 99, "picture");
    char write_base_path[100] = {0};
    strcat(strcat(write_base_path, SAVE_PATH_PREFIX), base_path);
    if(!mkdirs(write_base_path) || !dir_writeable(write_base_path)){
        printf("error: can't write %s, permission denied!\n", write_base_path);
        return 0;
    }

    /* original link */
    strcpy(links->original_link, DOWNLOAD_LINK_PREFIX);
    strcat(strcat(links->original_link, base_path), "/original/");
    strcat(strcat(links->original_link, uuid), suffix);
    /* original path*/
    char org_path[100] = {0};
    strcat(strcpy(org_path, write_base_path), "/original/");
    if(!mkdirs(org_path) || !dir_writeable(org_path)){
        printf("error: can't write %s, permission denied!\n", org_path);
        return 0;
    }
    strcat(strcat(org_path, uuid), suffix);
    /* write original file */
    FILE *fp = fopen(org_path, "wb");
    if(fp == NULL){
        printf("error: can't write original image file!\n");
        return 0;
    }
    fwrite(blobs->original_img_bytes, 1, blobs->original_img_length, fp);
    fclose(fp);

    /* large link  and large path*/
    if(blobs->large_img_bytes == NULL){
        strcpy(links->large_link, links->original_link);
    }else{
        strcat(strcat(links->large_link, base_path), "/large/");
        strcat(strcat(links->large_link, uuid), suffix);
    }
    char large_path[100] = {0};
    strcat(strcpy(large_path, write_base_path), "/large/"); /*large path*/
    if(!mkdirs(large_path) || !dir_writeable(large_path)){
        printf("error: can't write %s, permission denied!\n", large_path);
        return 0;
    }
    strcat(strcat(large_path, uuid), ".jpg");
    sprintf(tmp, "_%lux%lu", blobs->large_img_width, blobs->large_img_height);
    strcat(links->large_link, tmp);/*large link*/
    /*write lagre file*/
    fp = fopen(large_path, "wb");
    if(fp == NULL){
        printf("error: can't write large image file!\n");
        return 0;
    }
    if(blobs->large_img_bytes != NULL){
        fwrite(blobs->large_img_bytes, 1, blobs->large_img_length, fp);
        fclose(fp);
    }

    /* middle link and middle path */
    if(blobs->middle_img_bytes == NULL){
        strcpy(links->middle_link, links->original_link);
    }else{
        strcat(strcat(links->middle_link, base_path), "/middle/");
        strcat(strcat(links->middle_link, uuid), suffix);
    }
    char middle_path[100] = {0};
    strcat(strcpy(middle_path, write_base_path), "/middle/"); /*large path*/
    if(!mkdirs(middle_path) || !dir_writeable(middle_path)){
        printf("error: can't write %s, permission denied!\n", middle_path);
        return 0;
    }
    strcat(strcat(middle_path, uuid), ".jpg");
    sprintf(tmp, "_%lux%lu", blobs->middle_img_width, blobs->middle_img_height);
    strcat(links->middle_link, tmp);
    /* write middle file */
    fp = fopen(middle_path, "wb");
    if(fp == NULL){
        printf("error: can't write middle image file!\n");
        return 0;
    }
    if(blobs->middle_img_bytes != NULL){
        fwrite(blobs->middle_img_bytes, 1, blobs->middle_img_length, fp);
        fclose(fp);
    }

    /* small link */
    if(blobs->small_img_bytes == NULL){
        strcpy(links->small_link, links->original_link);
    }else{
        strcat(strcat(links->small_link, base_path), "/small/");
        strcat(strcat(links->small_link, uuid), suffix);
    }
    char small_path[100] = {0};
    strcat(strcpy(small_path, write_base_path), "/small/"); /*large path*/
    if(!mkdirs(small_path) || !dir_writeable(small_path)){
        printf("error: can't write %s, permission denied!\n", small_path);
        return 0;
    }
    strcat(strcat(small_path, uuid), ".jpg");
    sprintf(tmp, "_%lux%lu", blobs->small_img_width, blobs->small_img_height);
    strcat(links->small_link, tmp);
    /* write small file */
    fp = fopen(small_path, "wb");
    if(fp == NULL){
        printf("error: can't write small image file!\n");
        return 0;
    }
    if(blobs->small_img_bytes != NULL){
        fwrite(blobs->small_img_bytes, 1, blobs->small_img_length, fp);
        fclose(fp);
    }


    return 1;
}
