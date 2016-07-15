#include <ev.h>


static void my_cb(struct ev_loop* loop, struct ev_io* w, int revents) {
	ev_io_stop(loop, w);
	ev_unloop(loop, EVUNLOOP_ALL);
}


int main() {
	struct ev_loop* loop = ev_default_loop(0);
	struct ev_io stdin_watcher;

	ev_init(&stdin_watcher, my_cb);
	ev_io_set(&stdin_watcher, 0, EV_READ);
	ev_io_start(loop, &stdin_watcher);
	ev_loop(loop, 0);

	return 0;
}
