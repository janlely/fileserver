#define _GUN_SOURCE 1
#include "multi_part_parser.h"
#include <string.h>
#include <stdlib.h>
#include <common_define.h>

multi_part_info parse_multi_part_content(const void *content, size_t size,  const char *boundry)
{

    multi_part_info result = {0,0,0,0};

    const void *p = content;
    part_info part = get_fisrt_part(p, size, boundry);
    while(part.content_length != 0){
        if(strncmp(part.content_type, "text/plain", 10) == 0
                && strncmp(part.name, TEXT_PART_NAME, part.name_length) == 0){
            result.string_part = part.content;
            result.string_part_length = part.content_length;
        }else if(strncmp(part.content_type, "application/octet-stream", 24) == 0
                    && strncmp(part.name, "file", 4) == 0){
            result.file_part = part.content;
            result.file_part_length = part.content_length;
        }
        p = part.content + part.content_length;
        p += 4; /*skip \r\n--*/
        part = get_fisrt_part(p, size - (p - content) - 1, boundry);
    }
    return result;
}

part_info get_fisrt_part(const void *content, size_t size,  const char *boundry)
{

    int boundry_len = strlen(boundry);

    if (strncmp((char *)content, boundry, boundry_len) != 0)
    {
        return (part_info){0,0,0,0,0,0};
    }

    const void *part;
    part_info result;

    /*seek first boundry*/
    content = content + boundry_len + 1;
    size = size - boundry_len - 1; /*1 stand for the \n*/
    /* check first part Content-Type*/
    const void *content_type = memmem(content, size,  "Content-Type:", 13);
    const void *content_name = memmem(content, size, "Content-Disposition:", 20);
    const void *content_body = memmem(content, size,  "\r\n\r\n", 4);
    content_body += 4;
    const void *next_part = memmem(content, size, boundry, boundry_len);
    result.content = content_body;
    result.content_length = next_part - content_body - 4; /*-4 to skip \r\n--*/

    if (content_type == NULL || content_name == NULL)
        return (part_info){0,0,0,0,0,0};

    content_type += strlen("Content-Type:");
    /* strip space */
    while(((char *)content_type)[0] == ' ')
        content_type++;

    if (strncmp((char *)content_type, "text/plain", 10) == 0){
        result.content_type= content_type;
        result.content_type_length = 10;
    }else if(strncmp((char*)content_type, "application/octet-stream", 24) == 0){
        result.content_type = content_type;
        result.content_type_length = 24;
    }else{
        return (part_info){0,0,0,0,0,0};
    }

    content_name = strstr((char *)content_name, "name=");
    if (content_name == NULL)
        return (part_info){0,0,0,0,0,0};
    content_name += 5;
    /* strip space */
    while(((char *)content_name)[0] == ' ')
        content_name++;
    /* strip the first " */
    content_name++;
    const char *name_end = strchr((char *)content_name, '"');
    result.name = content_name;
    result.name_length = name_end - (char *)content_name;

    return result;
}
