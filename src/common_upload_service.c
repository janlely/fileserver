#include "common_upload_service.h"
#include <stdlib.h>
#include <string.h>
#include "multi_part_parser.h"

const char * serve_common_upload(struct evhttp_request *request)
{

    const struct evkeyvalq *headers = evhttp_request_get_input_headers(request);
    const char *content_length = evhttp_find_header(headers, "Content-Length");
    const char *content_type = evhttp_find_header(headers, "Content-Type");
    int cl = 0;
    if(content_length == NULL || strlen(content_length) <= 0)
    {
        return "Bad request";
    }
    if(content_type )
    //read raw data
    cl = atoi(content_length);
    char *content = (char *)malloc (cl * sizeof(char));
    struct evbuffer *input_buf = evhttp_request_get_input_buffer(request);

    int read_length = 0;
    char * p = content;
    while(read_length < cl){
        int tmp = evbuffer_copyout(input_buf, p, 1024);
        p += tmp;
        read_length += tmp;
        evbuffer_drain(input_buf, tmp);
    }

    /* multi_part_info info = parse_multi_part_content(content, boundry); */

    return "hello world";
}
