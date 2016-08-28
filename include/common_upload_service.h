#ifndef _COMMON_UPLOAD_SERVICE_H
#define _COMMON_UPLOAD_SERVICE_H

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>

void serve_common_upload(struct evhttp_request *request, char * response);


#endif
