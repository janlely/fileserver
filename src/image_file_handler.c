#include "image_file_handler.h"
#include "common_utils.h"
#include "image_processer.h"
#include "errcode.h"
#include <wand/MagickWand.h>
#include "common_define.h"

void handler_image_file(const char *param, const void *file, size_t file_length, char *response)
{
    json_object *jobj = json_tokener_parse(param);
    if(json_object_get_type(jobj) != json_type_string){
        put_int_value_to_jsonstring(response, "errcode", INVALID_PARAM);
        return;
    }
    const char *file_name = get_string_from_jsonobject(jobj, "fileName");
    const char *suffix = strchr(file_name, '.');

    olms images = scale_image_four(file, file_length);
}

olms scale_image_four(const void *file, size_t file_length)
{
    MagickWand *wand = NULL;
    void *large_img = NULL;
    size_t large_img_length = 0;
    void *middle_img = NULL;
    size_t middle_img_length = 0;
    void *small_img = NULL;
    size_t small_img_length = 0;
    olms result = {
        file, file_length,
        large_img, large_img_length,
        middle_img, middle_img_length,
        small_img, small_img_length
    };

    /* read image bytes */
    if(read_image_from_btyes(file, file_length, wand) != 0){
        printf("error: can't read image!\n");
        return olms;
    }

    /* scale large image*/
    size_t width_org = MagickGetImageWidth(wand);
    size_t height_org = MagickGetImageHeight(wand);
    int width_large, height_large;
    if(width_org <= LARGE_IMAGE_BOUNDRY && height_org <= LARGE_IMAGE_BOUNDRY){
        result.large_img_bytes = file;
        result.large_img_length = file_length;
    }else{
        calc_new_image_size(width_org, height_org, &width_large, &height_large, LARGE_IMAGE_BOUNDRY);
        if(scale_image_to_blob(wand, &large_img, &large_img_length, width_large, height_large) != 0){
            printf("error: cant't scale large image!\n");
            return olms;
        }
    }

    /* scale middle img */
    int width_middle, height_middle;
    if(large_img == NULL){
        if(width_org <= MIDDLE_IMAGE_BOUNDRY && height_org <= MIDDLE_IMAGE_BOUNDRY){
            result.middle_img_bytes = file;
            result.middle_img_length = file.length;
        }else{
            calc_new_image_size(width_org, height_org, &width_middle, &height_middle, MIDDLE_IMAGE_BOUNDRY);
            if(scale_image_to_blob(wand, &middle_img, &middle_img_length, width_middle, height_middle) != 0){
                printf("error: can't scale middle image!\n");
                return olms;
            }
        }
    }else{
        if(width_large <= MIDDLE_IMAGE_BOUNDRY && height_large <= MIDDLE_IMAGE_BOUNDRY){
            result.middle_img_bytes = large_img;
            result.middle_img_length = large_img_length;
        }else{
            calc_new_image_size(width_large, height_large, &width_middle, &height_middle, MIDDLE_IMAGE_BOUNDRY);
            if(scale_image_to_blob(wand, &middle_img, &middle_img_length, width_middle, height_middle) != 0){
                printf("error: can't scale middle image!\n");
                return olms;
            }
        }
    }

    /* scale small img */
    int width_small, height_small;
    if(middle_img == NULL && large_img == NULL){
        if(width_org <= SMALL_IMAGE_BOUNDRY && height_org <= SMALL_IMAGE_BOUNDRY){
            result.small_img_bytes = file;
            result.small_img_length = file.length;
        }else{
            calc_new_image_size(width_org, height_org, &width_small, &height_small, SMALL_IMAGE_BOUNDRY);
            if(scale_image_to_blob(wand, &small_img, &small_img_length, width_small, height_small) != 0){
                printf("error: can't scale small image!\n");
                return olms;
            }
        }
    }else{
        if(width_middle <= SMALL_IMAGE_BOUNDRY && height_middle <= SMALL_IMAGE_BOUNDRY){
            result.small_img_bytes = middle_img;
            result.small_img_length = middle_img_length;
        }else{
            calc_new_image_size(width_middle, height_middle, &width_small, &height_small, SMALL_IMAGE_BOUNDRY);
            if(scale_image_to_blob(wand, &small_img, &small_img_length, width_small, height_small) != 0){
                printf("error: can't scale middle image!\n");
                return olms;
            }
        }
    }


    return olms;
}
