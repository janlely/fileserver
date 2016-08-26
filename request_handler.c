#include "request_handler.h"
#include <string.h>


void handler_request(const struct evhttp_request *request, struct evbuffer *response_buf)
{

    const char *response_body;
    /* check uri */
    const char *uri = evhttp_request_get_uri(request);

    if(strcmp(uri, "/file/uploadfile"))
    {
        response_body = serve_common_upload(request);
    }
    
    evbuffer_add_printf(response_buf, "%s\n", response_body);

}
