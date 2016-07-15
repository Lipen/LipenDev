#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

uv_loop_t* loop;


void read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
	uv_write_t* req = (uv_write_t*)malloc(sizeof(uv_write_t));
	uv_buf_t new_buf = uv_buf_init(buf->base, nread);
	// uv_write(req, stream, buf, 1, NULL);
	uv_write(req, stream, &new_buf, 1, NULL);

	if (nread == 0) {
		free(buf->base);
	}
}

uv_buf_t alloc_buffer(uv_handle_t* handle, size_t size) {
	return uv_buf_init((char*)malloc(size), size);
}

void alloc_cb(uv_handle_t* handle, size_t size, uv_buf_t* buf) {
	*buf = uv_buf_init((char*) malloc(size), size);
}

void connection_cb(uv_stream_t* server, int status) {
	uv_tcp_t* client = malloc(sizeof(uv_tcp_t));

	uv_tcp_init(loop, client);
	uv_accept(server, (uv_stream_t*)client);
	// uv_read_start((uv_stream_t*)client, alloc_buffer, read_cb);
	uv_read_start((uv_stream_t*)client, alloc_cb, read_cb);
}


int main(int argc, char const *argv[]) {
	loop = uv_default_loop();
	uv_tcp_t server;
	uv_tcp_init(loop, &server);

	struct sockaddr_in addr;
	uv_ip4_addr("0.0.0.0", 5555, &addr);
	uv_tcp_bind(&server, (struct sockaddr*)&addr, 0);

	uv_listen((uv_stream_t*)&server, 128, connection_cb);

	return uv_run(loop, UV_RUN_DEFAULT);
}
