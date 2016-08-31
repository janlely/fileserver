#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include "request_handler.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>


int httpserver_bindsocket(int port, int backlog);
int httpserver_start(int port, int nthreads, int backlog);
void* httpserver_Dispatch(void *arg);
void httpserver_GenericHandler(struct evhttp_request *req, void *arg);
void httpserver_ProcessRequest(struct evhttp_request *req);

int httpserver_bindsocket(int port, int backlog) {
    int r;
    int nfd;
    nfd = socket(AF_INET, SOCK_STREAM, 0);
    if (nfd < 0) return -1;

    int one = 1;
    r = setsockopt(nfd, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(int));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    r = bind(nfd, (struct sockaddr*)&addr, sizeof(addr));
    if (r < 0) return -1;
    r = listen(nfd, backlog);
    if (r < 0) return -1;

    int flags;
    if ((flags = fcntl(nfd, F_GETFL, 0)) < 0
            || fcntl(nfd, F_SETFL, flags | O_NONBLOCK) < 0)
        return -1;

    return nfd;
}

int httpserver_start(int port, int nthreads, int backlog) {
    int r, i;
    int nfd = httpserver_bindsocket(port, backlog);
    if (nfd < 0) return -1;
    pthread_t ths[nthreads];
    for (i = 0; i < nthreads; i++) {
        struct event_base *base = event_base_new();
        if (base == NULL) return -1;
        struct evhttp *httpd = evhttp_new(base);
        if (httpd == NULL) return -1;
        r = evhttp_accept_socket(httpd, nfd);
        if (r != 0) return -1;
        evhttp_set_gencb(httpd, httpserver_GenericHandler, NULL);
        r = pthread_create(&ths[i], NULL, httpserver_Dispatch, base);
        if (r != 0) return -1;
    }
    printf("Server start success\n");
    for (i = 0; i < nthreads; i++) {
        pthread_join(ths[i], NULL);
    }
}

void* httpserver_Dispatch(void *arg) {
    event_base_dispatch((struct event_base*)arg);
    return NULL;
}

void httpserver_GenericHandler(struct evhttp_request *req, void *arg) {
    httpserver_ProcessRequest(req);
}

void httpserver_ProcessRequest(struct evhttp_request *req) {

    struct evbuffer *buf = evbuffer_new();
    /* struct evbuffer *input_buf = evhttp_request_get_input_buffer(req); */
    if(!buf)
    {
        puts("failed to create response buffer \n");
        return;
    }

    handler_request(req, buf);

    /* evbuffer_add_printf(buf, "Server Responsed. Requested: %s\n", evhttp_request_get_uri(req)); */
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
}

int main(int argc, char *argv[]) {
    if(argc < 4){
        printf("Usage: ./fileserver <port> <thread number> <backlog> \n");
        return 0;
    }
    int port = atoi(argv[1]);
    int threads = atoi(argv[2]);
    int backlog = atoi(argv[3]);
    httpserver_start(port, threads, backlog);
    return 0;
}
