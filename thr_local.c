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
  struct fmill_event ev;

  if (argc != 4) {
    printf ("usage: %s connect-to msg-size msg-count\n", argv[0]);
    exit (0);
  }

  addr = argv[1];
  sz = (size_t)atoi(argv[2]);
  count = atoi(argv[3]);

  nn_stopwatch_init (&sw);
  fmill_sock conn = fmill_sock_connect (addr);
  assert (conn);
  chan events = fmill_eventsch (conn);

  for (int i = 0; i < count; i++) {
    ev = chr (events, struct fmill_event);
    assert (ev.conn == NULL && ev.fr->size == sz);
  }

  total = nn_stopwatch_term (&sw);
  if (total == 0)
    total = 1;

  thr = (uint64_t) ((double) count / (double) total * 1000000);
  mbs = (double) (thr * sz * 8) / 1000000;
  
  printf ("message size: %d [B]\n", (int) sz);
  printf ("message count: %d\n", (int) count);
  printf ("throughput: %d [msg/s]\n", (int) thr);
  printf ("throughput: %.3f [Mb/s]\n", (double) mbs);
  return 0;
}
