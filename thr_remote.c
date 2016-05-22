#include "../fmill/fmill.c"
#include "stopwatch.c"
#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  int rc;
  char *addr;
  size_t sz;
  int count;
  uint64_t total;
  uint64_t thr;
  double mbs;
  struct nn_stopwatch sw;

  if (argc != 4) {
    printf ("usage: %s bind-to msg-size msg-count\n", argv[0]);
    exit (0);
  }
  addr = argv[1];
  sz = (size_t)atoi(argv[2]);
  count = atoi(argv[3]);

  nn_stopwatch_init (&sw);
  fmill_sock sock = fmill_sock_bind (addr);
  assert (sock);
  struct fmill_event ev = chr (fmill_eventsch (sock), struct fmill_event);
  assert (ev.fr == NULL);
  fmill_sock conn = ev.conn;
  struct frm_frame *fr = malloc (sizeof (struct frm_frame));
  assert (fr);
  frm_frame_init (fr);
  fr->buf = malloc(sz);
  fr->size = sz;
  assert (fr->buf);
  for (int i = 0; i < sz; i++)
    ((char *)fr->buf)[i] = 'f';

  for (int i = 0; i < count; i++) {
    fmill_send (conn, fr);
  }

  msleep (now() + 1000000);

  return 0;
}
