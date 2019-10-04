#include <cstdio>
#include <iostream>

#include "bencode.h"

#include <cstdlib>
#include <uv.h>

// #define DEBUG(out) std::cout << out << std::endl
#define DEBUG(X)


void tracker_request(){

}


using namespace hydrafs;

int main(){

    FILE* file = fopen("/home/setepenre/work/hydrafs/examples/manjaro-kde-18.1.0-stable-x86_64.iso.torrent", "r");

    auto val = bencode::parse_bencode(file);
    pprint(std::cout, val);




    return 0;
}


void on_connect(uv_connect_t* connect, int status);
int client_exemple(){
    auto *loop = static_cast<uv_loop_t *>(malloc(sizeof(uv_loop_t)));
    auto* socket = static_cast<uv_tcp_t*>(malloc(sizeof(uv_tcp_t)));
    auto* connect = static_cast<uv_connect_t*>(malloc(sizeof(uv_connect_t)));

    uv_tcp_init(loop, socket);

    struct sockaddr_in dest;
    uv_ip4_addr("127.0.0.1", 80, &dest);

    uv_tcp_connect(connect, socket, (const struct sockaddr*)&dest, on_connect);
}


void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    *buf = uv_buf_init(static_cast<char*>(malloc(suggested_size)), suggested_size);
}

void read_socket(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {

}

void on_connect(uv_connect_t* connect, int status){
    uv_stream_t* handle = connect->handle;

    uv_read_start(handle, alloc_buffer, read_socket);

    uv_read_stop(handle);

}



void on_new_connection(uv_stream_t *server, int status);
int server_exemple(){
    #define DEFAULT_PORT 80
    #define DEFAULT_BACKLOG 100

    auto *loop = static_cast<uv_loop_t *>(malloc(sizeof(uv_loop_t)));

    uv_tcp_t server;
    uv_tcp_init(loop, &server);

    struct sockaddr_in addr;
    uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);

    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);

    int r = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, on_new_connection);
    if (r) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
        return 1;
    }
    return uv_run(loop, UV_RUN_DEFAULT);
}

/*
void on_new_connection(uv_stream_t *server, int status) {
    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        // error!
        return;
    }

    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, client);
    if (uv_accept(server, (uv_stream_t*) client) == 0) {
        uv_read_start((uv_stream_t*) client, alloc_buffer, echo_read);
    }
}*/

void example_1(){
    auto *loop = static_cast<uv_loop_t *>(malloc(sizeof(uv_loop_t)));
    uv_loop_init(loop);

    printf("Now quitting.\n");
    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
    free(loop);
}
