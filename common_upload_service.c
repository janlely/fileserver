#include "common_upload_service.h"
#include <stdlib.h>
#include <string.h>

const char * serve_common_upload(const struct evhttp_request *request)
{
    
    const struct evkeyvalq *headers = evhttp_request_get_input_headers(request);
    const char *content_length = evhttp_find_header(headers, "Content-Length");
    int cl = 0;
    if(content_length == NULL || strlen(content_length) <= 0)
    {
        return "Bad request";
    }
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
    

    return "hello world";
}
