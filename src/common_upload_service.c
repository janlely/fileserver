#include "common_upload_service.h"
#include <stdlib.h>
#include <string.h>
#include "multi_part_parser.h"
#include <regex.h>
#include <json.h>

void serve_common_upload(struct evhttp_request *request, char * response)
{

    const struct evkeyvalq *headers = evhttp_request_get_input_headers(request);
    const char *content_length = evhttp_find_header(headers, "Content-Length");
    const char *content_type = evhttp_find_header(headers, "Content-Type");
    printf("Content-Type: %s\n", content_type);
    int cl = 0;
    if(content_length == NULL || strlen(content_length) <= 0)
    {
        return "Bad request";
    }

    regex_t reg;
    const char *pattern = "(multipart/form-data); *boundary=([A-z0-9_-]+)";
    regmatch_t pmatch[3];
    regcomp(&reg, pattern, REG_EXTENDED);
    int status = regexec(&reg, content_type, 3, pmatch, 0);

    if(status == REG_NOMATCH)
        return "Bad request";

    int bs = pmatch[2].rm_so; /*boundry start index*/
    int be = pmatch[2].rm_eo; /*boundry end index*/
    char *boundry = (char *)malloc(be - bs + 2);
    strncpy(boundry, content_type + bs, be - bs + 1);



    //read raw data
    cl = atoi(content_length);
    printf("content length: %d\n", cl);
    void *content = malloc (cl); /*malloc*/
    struct evbuffer *input_buf = evhttp_request_get_input_buffer(request);

    int read_length = 0;
    void *p = content;
    while(read_length < cl){
        int tmp = evbuffer_copyout(input_buf, p, 1024);
        p += tmp;
        read_length += tmp;
        evbuffer_drain(input_buf, tmp);
    }

   /* parse multipart request body  */
    multi_part_info info = parse_multi_part_content(content+2, cl, boundry);/* content+2 to skip the first -- */
    if(info.string_part_length == 0 || info.file_part_length == 0)
        return "Bad request";

    char *string_part = (char *)malloc(info.string_part_length + 1);/*+1 for \0*/
    strncpy(string_part, info.string_part, info.string_part_length);
    json_object *jobj = json_tokener_parse(string_part);
    if(json_object_get_type(jobj) != json_type_object)
        return "Bad request";

    const char *file_type = get_string_from_jsonobject(jobj, "type");
    if(strcmp("image", file_type) == 0){
        handler_image_file(string_part, info.file_part, info.file_part_length, response);
    }

    json_object_put(jobj);
    free(string_part);
    free(content);
    free(boundry);
}
