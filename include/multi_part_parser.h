#ifndef _MULTI_PART_PARSER_H
#define _MULTI_PART_PARSER_H

#include <string.h>
typedef struct {
    const char *string_part;
    unsigned int string_part_length;

    const void *file_part;
    unsigned int file_part_length;
} multi_part_info;

typedef struct {
    const char *name;
    unsigned int name_length;
    const char *content_type;
    unsigned int content_type_length;
    const void *content;
    unsigned int content_length;
} part_info;

multi_part_info parse_multi_part_content(const void *content, size_t size,  const char *boundry);

part_info get_fisrt_part(const void *content, size_t size, const char *boundry);


#endif
