#include "../fmill/fmill.c"
#include "stopwatch.c"
#include <stdio.h>
#include <string.h>

static coroutine void serverroutine(int count, chan endch) {
  fmill_sock sock = fmill_sock_bind ("tcp://0.0.0.0:7458");
  struct fmill_event ev = chr (fmill_eventsch (sock), struct fmill_event);
  assert (ev.fr == NULL);
  fmill_sock conn = ev.conn;
  printf ("new conn accepted.\n");
  chan events = fmill_eventsch (conn);
  for (int i = 0; i < count; i++) {
    ev = chr (events, struct fmill_event);
    assert (ev.conn == NULL);
    fmill_send (conn, ev.fr);
  }
  chs (endch, int, 1);
}

static coroutine void clientroutine(int count, chan endch) {
  msleep (100);

  chs (endch, int, 1);
  printf ("cli complete.\n");
}

int main (int argc, char *argv[]) {
  int rc;
  char *addr;
  size_t sz;
  int rts;
  uint64_t total;
  double lat;
  struct nn_stopwatch stopwatch;

  if (argc != 4) {
    printf ("usage: %s connect-to msg-size roundtrip-count\n", argv[0]);
    exit (0);
  }

  addr = argv[1];
  sz = (size_t)atoi(argv[2]);
  rts = atoi(argv[3]);

  nn_stopwatch_init (&stopwatch);
  fmill_sock conn = fmill_sock_connect (addr);
  assert (conn);
  struct frm_frame *fr = malloc (sizeof (struct frm_frame));
  frm_frame_init (fr);
  fr->buf = malloc (sz);
  assert (fr->buf);
  fr->size = sz;
  for (int i = 0; i < sz; i++)
    ((char *)fr->buf)[i] = 'f';

  struct fmill_event ev;
  chan events = fmill_eventsch (conn);
  for (int i = 0; i < rts; i++) {
    fmill_send (conn, fr);
    ev = chr (events, struct fmill_event);
    assert (ev.conn == NULL);
  }

  total = nn_stopwatch_term (&stopwatch);
  lat = (double) total / (rts * 2);
  printf ("message size: %d [B]\n", (int) sz);
  printf ("roundtrip count: %d\n", (int) rts);
  printf ("average latency: %.3f [us]\n", (double) lat);

  return 0;
}
