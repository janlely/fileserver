#include "request_handler.h"
#include <string.h>


void handler_request(struct evhttp_request *request, struct evbuffer *response_buf)
{

    char *response = (char *)malloc(1000);
    memset(response, 0, 1000);
    /* check uri */
    const char *uri = evhttp_request_get_uri(request);

    if(0 == strcmp(uri, "/file/uploadfile"))
    {
        serve_common_upload(request, response);
    }

    evbuffer_add_printf(response_buf, "%s\n", response_body);
    free(response);

}
