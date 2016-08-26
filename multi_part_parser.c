#include "multi_part_parser.h"
#include <string.h>
#include <malloc.h>

multi_part_info parse_multi_part_content(const char *content, const char *boundry)
{
    const char *part;

    int boundry_len = strlen(boundry);
    /*seek first boundry*/
    content += boundry_len; 
    /* check first part Content-Type*/
    const char *content_type = strstr(content, "Content-Type");
    if (content_type == NULL)
        return (multi_part_info){0,0,0,0};

    content_type += strlen("Content-Type:");
    /* strip space */
    while(*content_type == ' ')
        content_type++;

    if(strncmp(content_type, "text/plain", 10)){
        part = strstr(content_type, "\n\n");
        if(part == NULL)
            return (multi_part_info){0,0,0,0};
        part += 2;
        const char *tmp = strstr(part, boundry);
        if(tmp ==  NULL)
            return (multi_part_info){0,0,0,0};
    }else if(strncmp(content_type, "application/octet-stream", 24)){
        part = strstr(content_type, "\n\n");
        part += 2;

    }
}

part_info get_fisrt_part(const char *content, const char *boundry)
{

    int boundry_len = strlen(boundry);
    if (strncmp(content, boundry, boundry_len) != 0)
    {
        return (part_info){0,0,0,0};
    }

    const char *part;
    part_info result;

    /*seek first boundry*/
    content += boundry_len; 
    /* check first part Content-Type*/
    const char *content_type = strstr(content, "Content-Type:");
    const char *content_name = strstr(content, "Content-Disposition:");
    if (content_type == NULL || content_name == NULL)
        return (part_info){0,0,0,0};

    content_type += strlen("Content-Type:");
    /* strip space */
    while(*content_type == ' ')
        content_type++;

    if (strncmp(content_type, "text/plain", 10) == 0){
        result.content_type = (char *)malloc(11);
        result.content_type = strcpy(result.content_type, "text/plain");
    }else if(strncmp(content_type, "application/octet-stream", 24) == 0){
        result.content_type = (char *)malloc(25);
        result.content_type = strcpy(result.content_type, "application/octet-stream");
    }else{
        return (part_info){0,0,0,0};
    }

    content_name = strstr(content_name, "name=");
    if (content_name == NULL)
        return (part_info){0,0,0,0};
    /* strip space */
    while(*content_name == ' ')
        content_name++;

    
     


    if(strncmp(content_type, "text/plain", 10)){
        part = strstr(content_type, "\n\n");
        if(part == NULL)
            return (multi_part_info){0,0,0,0};
        part += 2;
        const char *tmp = strstr(part, boundry);
        if(tmp ==  NULL)
            return (multi_part_info){0,0,0,0};
    }else if(strncmp(content_type, "application/octet-stream", 24)){
        part = strstr(content_type, "\n\n");
        part += 2;

    }

}
