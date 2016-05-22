#include "../fmill/fmill.c"
#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
  //fmill_sock sock;
  //sock = fmill_sock();
  int rc;
  char *addrcpy = NULL;
  int tcp = 0;
  int port = 0;
  rc = fmill_parse_addr ("tcp://0.0.0.0:2584", &addrcpy, &tcp, &port);
  printf ("rc: %d\n", rc);
  printf ("addrbuf: %s\n", addrcpy);
  printf ("port: %d\n", port);
  rc = fmill_parse_addr ("unix:///tmp/fmill.sock.{pid}", &addrcpy, &tcp, &port);
  printf ("rc: %d\n", rc);
  printf ("addrbuf: %s\n", addrcpy);
  printf ("port: %d\n", port);
  chan endch = chmake (int, 0);
  fmill_sock sock = fmill_sock_bind ("tcp://0.0.0.0:7458");
  fmill_sock conn = fmill_sock_connect ("tcp://0.0.0.0:7458");
  fmill_send2 (conn, "fatih", 5);
  printf ("sock: %p\n", sock);
  printf ("conn: %p\n", conn);
  struct fmill_event ev = chr (fmill_eventsch (sock), struct fmill_event);
  assert (ev.fr == NULL);
  printf ("conn: %p\n", ev.conn);
  fmill_sock connout = ev.conn;
  ev = chr (fmill_eventsch (connout), struct fmill_event);
  assert (ev.conn == NULL);
  printf ("received frame: %.*s\n", ev.fr->size, ev.fr->buf);
  chr (endch, int);
  return 0;
}
