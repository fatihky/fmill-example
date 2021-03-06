#include "../fmill/fmill.c"
#include "stopwatch.c"
#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  int rc;
  char *addr;
  size_t sz;
  int rts;
  uint64_t total;
  double lat;
  struct nn_stopwatch stopwatch;

  if (argc != 4) {
    printf ("usage: %s bind-to msg-size roundtrip-count\n", argv[0]);
    exit (0);
  }

  addr = argv[1];
  sz = (size_t)atoi(argv[2]);
  rts = atoi(argv[3]);

  nn_stopwatch_init (&stopwatch);
  fmill_sock sock = fmill_sock_bind (addr);
  struct fmill_event ev = chr (fmill_eventsch (sock), struct fmill_event);
  assert (ev.fr == NULL);
  fmill_sock conn = ev.conn;
  printf ("new conn accepted.\n");
  chan events = fmill_eventsch (conn);
  for (int i = 0; i < rts; i++) {
    ev = chr (events, struct fmill_event);
    assert (ev.conn == NULL);
    assert (ev.fr->size == sz);
    fmill_send (conn, ev.fr);
  }

  ev = chr (events, struct fmill_event);
  assert (ev.fr == NULL && fmill_sock_dead (ev.conn));

  return 0;
}
