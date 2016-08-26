#ifndef _REQUEST_HANDLER_H
#define _REQUEST_HANDLER_H

#include "common_upload_service.h"

void handler_request(const struct  evhttp_request *request, struct evbuffer *response_buf);
#endif
