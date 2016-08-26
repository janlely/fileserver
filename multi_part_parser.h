#ifndef _MULTI_PART_PARSER_H
#define _MULTI_PART_PARSER_H

typedef struct {
    unsigned int string_part_start;
    unsigned int string_part_length;

    unsigned int file_part_start;
    unsigned int file_part_length;
} multi_part_info;

typedef struct {
    char *name;
    char *content_type;
    const char *content;
    unsigned int length;
} part_info;

multi_part_info parse_multi_part_content(const char *content, const char *boundry);

part_info get_fisrt_part(const char *content, const char *boundry);


#endif
