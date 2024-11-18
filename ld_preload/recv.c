#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/socket.h>
#include <dlfcn.h>

char *LOG_PREFIX = "[TLS-DEBUG]";

ssize_t recv(int sockfd, void *buf, size_t len, int flags) {
  fprintf(stderr, "%s `recv` - Reading bytes from socket %d\n", LOG_PREFIX, sockfd);

  ssize_t (*org)() = dlsym(RTLD_NEXT, "recv");
  ssize_t ret = org(sockfd, buf, len, flags);

  fprintf(stderr, "%s `recv` - Read %ld bytes from socket %d:\n", LOG_PREFIX, len, sockfd);
  if (fwrite(buf, sizeof(char), len, stdout) <= 0) {
    fprintf(stderr, "Failed to write output to stdout");
  }
  fprintf(stderr, "\n");
  return ret;
};


