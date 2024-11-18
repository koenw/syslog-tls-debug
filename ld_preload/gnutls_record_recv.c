#include <stdio.h>
#include <dlfcn.h>
#include <gnutls/gnutls.h>

char *LOG_PREFIX = "[TLS-DEBUG]";

ssize_t gnutls_record_recv(gnutls_session_t session, void* buf, size_t len) {
  ssize_t (*org)() = dlsym(RTLD_NEXT, "gnutls_record_recv");
  ssize_t ret = org(session, buf, len);

  fprintf(stderr, "%s `gnutls_record_recv` - Read %ld (decrypted) bytes from session:\n", LOG_PREFIX, ret);
  if (fwrite(buf, sizeof(char), ret, stdout) <= 0) {
    fprintf(stderr, "Failed to write (decrypted) output to stdout");
  }
  fprintf(stderr, "\n");
  return ret;
}
