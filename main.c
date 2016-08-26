#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>
#include <stdlib.h>
#include <stdio.h>
#include "request_handler.h"


void generic_handler(struct evhttp_request *req, void *arg)
{
    struct evbuffer *buf = evbuffer_new();
    /* struct evbuffer *input_buf = evhttp_request_get_input_buffer(req); */
    if(!buf)
    {
        puts("failed to create response buffer \n");
        return;
    }


    handler_request(req, buf);
    /* const struct evkeyvalq *headers = evhttp_request_get_input_headers(req); */
    /* const char *content_length = evhttp_find_header(headers, "Content-Length"); */
    /* int cl = atoi(content_length); */
    /* char *content = (char *)malloc (cl * sizeof(char)); */
    /* printf("length: %d\n\n", cl); */
    /* int read_length = 0; */
    /* char * p = content; */
    /* while(read_length < cl){ */
        /* int tmp = evbuffer_copyout(input_buf, p, 1024); */
        /* p += tmp; */
        /* read_length += tmp; */
        /* evbuffer_drain(input_buf, tmp); */
    /* } */
    /* printf("content:\n %s\n", content); */

     
    /* evbuffer_add_printf(buf, "Server Responsed. Requested: %s\n", evhttp_request_get_uri(req)); */
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
}

int main(int argc, char* argv[])
{

    short          http_port = 8081;
    char          *http_addr = "127.0.0.1";

    struct event_base * base = event_base_new();

    struct evhttp * http_server = evhttp_new(base);
    if(!http_server)
    {
        return -1;
    }

    int ret = evhttp_bind_socket(http_server,http_addr,http_port);
    if(ret!=0)
    {
        return -1;
    }

    evhttp_set_gencb(http_server, generic_handler, NULL);

    printf("http server start OK! \n");

    event_base_dispatch(base);

    evhttp_free(http_server);

    return 0;
}
