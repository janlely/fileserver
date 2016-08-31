#include "request_handler.h"
#include <string.h>
#include <stdlib.h>


void handler_request(struct evhttp_request *request, char *response)
{

    /* check uri */
    const char *uri = evhttp_request_get_uri(request);

    if(0 == strcmp(uri, "/file/uploadfile"))
    {
        serve_common_upload(request, response);
    }else{
        strcpy(response, "NOT_FOUND");
    }


}
