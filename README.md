# syslog-tls-debug

Collection of ad-hoc tools to troubleshoot Syslog TLS
issues.


## LD_PRELOAD

The [`ld_preload`](./ld_preload) directory contains
files that each replace a common networking function
with a version that logs whatever it received to stdout.

After compiling with `nix build`, add `./result/lib` to
`LD_LIBRARY_PATH` and the file you'd like to use to
`LD_PRELOAD`.


### Example

When you're following along and creating self-signed
certificates for the TLS server as below, you can just
answer 'y' to all yes/no questions and leave the rest
empty.

```sh
# Listen to whatever `gnutls_record_recv` is receiving
export LD_LIBRARY_PATH=$PWD/result/lib
export LD_PRELOAD=gnutls_record_recv.o

# Run a TLS server to debug
certtool --generate-privkey --outfile key.pem
certool --generate-self-sign --load-privkey key.pem --outfile cert.pem
gnutls-serv --x509certfile=cert.pem --x509keyfile=key.pem

# From another terminal window connect to our TLS server
openssl s_client -connect localhost:5556
```

You'll now see the received & decrypted data scrolling
through your screen.
