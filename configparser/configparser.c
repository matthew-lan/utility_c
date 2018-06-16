/*
* @Author: matthew
* @Date:   2018-05-19
* @Last Modified by:   matthew
* @Last Modified time: 2018-06-16
*/

/**********************************************************
 * Includes
**********************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "configparser.h"


/**********************************************************
 * Macros
**********************************************************/
#define DEBUG_LOG_ON

#ifdef  DEBUG_LOG_ON
#define DEBUG_LOG(tag, format, ...) \
        printf("D/%-6s: [%-26s] "format"\n", tag, __func__, ##__VA_ARGS__)
#else
#define DEBUG_LOG(tag, format, ...)
#endif

#define TAG_INI     "INI"


/**********************************************************
 * Types
**********************************************************/
enum LINE_TYPE
{
      LINE_TYPE_NONE
    , LINE_TYPE_SECTION
    , LINE_TYPE_KEY_VALUE
};


/**********************************************************
 * Fucntions
**********************************************************/
/**
 * [cfgp_init description]
 * initialize the configparser.
 *
 * @param   hdl  [configparser handle]
 *
 * @return       [success: 0; error: -1]
 */
int cfgp_init(struct configparser *hdl)
{
    if (hdl) {
        hdl->c_path = NULL;
        q_init_list_head(&hdl->sections.s_list);
    }
    return hdl ? 0 : -1;
}

/**
 * [cfgp_release description]
 * release the configparser.
 *
 * @param   hdl  [configparser handle]
 *
 * @return       [success: 0; error: -1]
 */
int cfgp_release(struct configparser *hdl)
{
    if (hdl) {
        struct q_list_head *s_head  = NULL;
        struct q_list_head *s_pos   = NULL;
        struct cfgp_sec    *s_entry = NULL;
        struct q_list_head *s_del   = NULL;

        struct q_list_head *kv_head  = NULL;
        struct q_list_head *kv_pos   = NULL;
        struct cfgp_kv     *kv_entry = NULL;
        struct q_list_head *kv_del   = NULL;

        s_head = &hdl->sections.s_list;
        q_list_for_each(s_pos, s_head) {
            s_entry = q_list_entry(s_pos, struct cfgp_sec, s_list);
            // DEBUG_LOG(TAG_INI, "> %s", s_entry->s_name);

            kv_head = &s_entry->kvs.kv_list;
            q_list_for_each(kv_pos, kv_head) {
                kv_entry = q_list_entry(kv_pos, struct cfgp_kv, kv_list);
                // DEBUG_LOG(TAG_INI, "  >> %s = %s", kv_entry->key, kv_entry->value);
                if (kv_entry->key) {
                    free(kv_entry->key);
                    kv_entry->key = NULL;
                }
                if (kv_entry->value) {
                    free(kv_entry->value);
                    kv_entry->value = NULL;
                }
                if (kv_del) {
                    q_list_del(kv_del);
                    free(q_list_entry(kv_del, struct cfgp_kv, kv_list));
                    kv_del = NULL;
                }
                kv_del = kv_pos;
            }
            if (kv_del) {
                q_list_del(kv_del);
                free(q_list_entry(kv_del, struct cfgp_kv, kv_list));
                kv_del = NULL;
            }

            if (s_entry->s_name) {
                free(s_entry->s_name);
                s_entry->s_name = NULL;
            }
            if (s_del) {
                q_list_del(s_del);
                free(q_list_entry(s_del, struct cfgp_sec, s_list));
                s_del = NULL;
            }
            s_del = s_pos;
        }
        if (s_del) {
            q_list_del(s_del);
            free(q_list_entry(s_del, struct cfgp_sec, s_list));
            s_del = NULL;
        }

        if (hdl->c_path) {
            free(hdl->c_path);
            hdl->c_path = NULL;
        }
    }
    return hdl ? 0 : -1;
}

/**
 * [cfgp_add_path description]
 * add the path argument into the configparser.
 *
 * @param   hdl   [configparser handle]
 * @param   path  [file path string]
 *
 * if path is NULL, keep the handle's c_path;
 * if path is not NULL, replace handle's c_path;
 *
 * @return        [success: 0; error: -1]
 */
int cfgp_add_path(struct configparser *hdl, const char *path)
{
    int  ret     = 0;
    char *c_path = NULL;
    do {
        if (!hdl) {
            ret = -1;
            break;
        }
        if (!path) {
            break;
        }
        if (!(c_path = strdup(path))) {
            break;
        }
        DEBUG_LOG(TAG_INI, "path: %s", c_path);
        free(hdl->c_path);
        hdl->c_path = c_path;
    } while (0);
    DEBUG_LOG(TAG_INI, "hdl: %p, path: %p, ret: %d", hdl, path, ret);
    return ret;
}

/**
 * [cfgp_fix_section description]
 * fix the section string;
 *
 * @param   section  [section string]
 *
 * remove '[' and ']' from section string;
 *
 * @return           [the position of valid character]
 */
static char *cfgp_fix_section(char *section)
{
    char *p = NULL;
    if ((p = strchr(section, ']'))) {
        *p = '\0';
    }
    if ((p = strchr(section, '['))) {
        ++p;
    }
    return p;
}

/**
 * [cfgp_fx_value description]
 * fix the value string;
 *
 * @param   value  [value string]
 *
 * remove space from the head/tail of value string;
 *
 * @return         [the position of valid character]
 */
static char *cfgp_fx_value(char *value)
{
    size_t len = 0;
    char   *q  = NULL;
    char   *p  = value;
    while (' ' == *p) {
        ++p;
    }
    len = strlen(p);
    q   = p + len -1;
    while (' ' == *q) {
        --q;
    }
    ++q;
    *q = '\0';
    return p;
}

/**
 * [cfgp_find_section description]
 * find the struct cfgp_sec item;
 *
 * @param   head     [the head of struct cfgp_sec list]
 * @param   section  [section string to be find]
 *
 * @return           [success: not NULL; error: NULL]
 */
static struct cfgp_sec *cfgp_find_section(struct q_list_head *head,
                                                       const char          *section
                                                       )
{
    struct cfgp_sec   *s_item = NULL;
    struct cfgp_sec   *entry  = NULL;
    struct q_list_head *pos    = NULL;

    q_list_for_each(pos, head) {
        entry = q_list_entry(pos, struct cfgp_sec, s_list);
        DEBUG_LOG(TAG_INI, "section: %s, s_name: %s", section, entry->s_name);
        if (!strcmp(entry->s_name, section)) {
            s_item = entry;
            break;
        }
    }
    DEBUG_LOG(TAG_INI, "s_item: %p", s_item);
    return s_item;
}

/**
 * [cfgp_find_kv description]
 * find the struct cfgp_kv item;
 *
 * @param   head  [the head of struct cfgp_kv list]
 * @param   key   [key string]
 *
 * @return        [success: not NULL; error: NULL]
 */
static struct cfgp_kv *cfgp_find_kv(struct q_list_head *head,
                                                   const char          *key
                                                   )
{
    struct cfgp_kv  *kv_item = NULL;
    struct cfgp_kv  *entry   = NULL;
    struct q_list_head *pos     = NULL;

    q_list_for_each(pos, head) {
        entry = q_list_entry(pos, struct cfgp_kv, kv_list);
        if (!strcmp(entry->key, key)) {
            kv_item = entry;
            break;
        }
    }
    DEBUG_LOG(TAG_INI, "kv_item: %p", kv_item);
    return kv_item;
}


/**
 * [cfgp_add_section description]
 * add a section into configparser.
 *
 * @param   hdl      [configparser handle]
 * @param   section  [section string]
 *
 * @return           [success: not NULL; error: NULL]
 */
struct cfgp_sec *cfgp_add_section(struct configparser *hdl,
                                               const char             *section
                                               )
{
    struct cfgp_sec   *s_item = NULL;
    char                *s_name = NULL;
    struct q_list_head *head   = NULL;
    struct q_list_head *pos    = NULL;

    do {
        if (!hdl || !section \
            || strchr(section, '[') \
            || strchr(section, ']') \
            || strchr(section, ' ')) {
            break;
        }
        head = &hdl->sections.s_list;
        if ((s_item = cfgp_find_section(head, section))) {
            DEBUG_LOG(TAG_INI, "section: %s exists", section);
            break;
        }
        if (!(s_item = (struct cfgp_sec *)calloc(1, sizeof(struct cfgp_sec)))) {
            break;
        }
        q_init_list_head(&s_item->kvs.kv_list);
        if (!(s_name = strdup(section))) {
            free(s_item);
            s_item = NULL;
            break;
        }
        s_item->s_name = s_name;

        q_list_for_each(pos, head) {
            if (q_list_is_last(pos, head)) {
                break;
            }
        }
        q_list_add(&s_item->s_list, pos);
    } while (0);
    DEBUG_LOG(TAG_INI, "hdl: %p, section: %p, s_item: %p", hdl, section, s_item);
    return s_item;
}

/**
 * [cfgp_mod_kv description]
 * modify the value of kv_item.
 *
 * @param   kv_item  [key-value item]
 * @param   value    [value string]
 *
 * @return           [success: 0; error: -1]
 */
static int cfgp_mod_kv(struct cfgp_kv *kv_item, const char *value)
{
    int  ret       = -1;
    char *kv_value = NULL;
    do {
        if (!strcmp(kv_item->value, value)) {
            DEBUG_LOG(TAG_INI, "value is the same");
            ret = 0;
            break;
        }
        if (!(kv_value = strdup(value))) {
            DEBUG_LOG(TAG_INI, "modify value failed");
            break;
        }
        DEBUG_LOG(TAG_INI, "value: %s -> %s", kv_item->value, kv_value);
        free(kv_item->value);
        kv_item->value = kv_value;
        ret               = 0;
    } while (0);
    DEBUG_LOG(TAG_INI, "ret: %d", ret);
    return ret;
}

/**
 * [cfgp_add_kv description]
 * add kv_item into the kv_list;
 *
 * @param   head   [the head of truct cfgp_kv list]
 * @param   key    [key string]
 * @param   value  [value string]
 *
 * @return         [success: not NULL; error: NULL]
 */
static struct cfgp_kv *cfgp_add_kv(struct q_list_head *head,
                                                  const char          *key,
                                                  const char          *value
                                                  )
{
    int            ret       = -1;
    struct cfgp_kv *kv_item  = NULL;
    char           *kv_key   = NULL;
    char           *kv_value = NULL;
    do {
        if (!(kv_item = (struct cfgp_kv *)calloc(1, sizeof(struct cfgp_kv))) \
            || !(kv_key = strdup(key)) \
            || !(kv_value = strdup(value))) {
            break;
        }
        ret            = 0;
        kv_item->key   = kv_key;
        kv_item->value = kv_value;
        q_list_add(&kv_item->kv_list, head);
    } while (0);
    if (ret) {
        if (kv_item)  { free(kv_item);  kv_item  = NULL; }
        if (kv_key)   { free(kv_key);   kv_key   = NULL; }
        if (kv_value) { free(kv_value); kv_value = NULL; }
    }
    DEBUG_LOG(TAG_INI, "kv_item: %p", kv_item);
    return kv_item;
}

/**
 * [cfgp_add_key_value description]
 * add/modify the key-value into/of configparser.
 *
 * @param   hdl      [configparser handle]
 * @param   section  [section string]
 * @param   key      [key string]
 * @param   value    [value string]
 *
 * @return           [success: not NULL; error: NULL]
 */
struct cfgp_kv *cfgp_add_key_value(struct configparser *hdl,
                                                  const char             *section,
                                                  const char             *key,
                                                  const char             *value
                                                  )
{
    struct cfgp_sec   *s_item  = NULL;
    struct cfgp_kv    *kv_item = NULL;
    struct q_list_head *head    = NULL;
    struct q_list_head *pos     = NULL;

    do {
        if (!hdl || !key || !value \
            || strchr(key, ' ') \
            || strchr(value, ' ')) {
            break;
        }
        head = &hdl->sections.s_list;
        if (!(s_item = cfgp_find_section(head, section))) {
            break;
        }
        head = &s_item->kvs.kv_list;
        if ((kv_item = cfgp_find_kv(head, key))) {
            cfgp_mod_kv(kv_item, value);
        }
        else {
            q_list_for_each(pos, head) {
                if (q_list_is_last(pos, head)) {
                    break;
                }
            }
            kv_item = cfgp_add_kv(pos, key, value);
        }
    } while (0);
    DEBUG_LOG(TAG_INI, "kv_item: %p", kv_item);
    return kv_item;
}

/**
 * [cfgp_linetype description]
 * distinguish the line string LINE_TYPE.
 *
 * @param   line  [line string]
 *
 * @return        [LINE_TYPE_*]
 */
static int cfgp_linetype(const char *line)
{
    int line_type = LINE_TYPE_NONE;
    if ((line == strchr(line, '[')) && strchr(line, ']')) {
        line_type = LINE_TYPE_SECTION;
    }
    else if (strchr(line, '=')) {
        line_type = LINE_TYPE_KEY_VALUE;
    }
    return line_type;
}

/**
 * [cfgp_read description]
 * read data into configparser from the filepath.
 *
 * @param   hdl   [configparser handle]
 * @param   path  [file path]
 *
 * @return        [success: 0; error: -1]
 */
int cfgp_read(struct configparser *hdl, const char *path)
{
    int     ret       = -1;
    FILE    *fp       = NULL;
    char    *line     = NULL;
    size_t  len       = 0;
    ssize_t nread     = 0;
    int     line_type = 0;
    char    *section  = NULL;

    do {
        if (!hdl || !path) {
            break;
        }
        if (access(path, F_OK)) {
            DEBUG_LOG(TAG_INI, "file does not exist");
            break;
        }
        if (!(fp = fopen(path, "rb"))) {
            break;
        }
        if (cfgp_add_path(hdl, path)) {
            break;
        }
        while (-1 != (nread = getline(&line, &len, fp))) {
            --nread;
            line[nread] = '\0';
            line_type   = cfgp_linetype(line);
            DEBUG_LOG(TAG_INI, "line_type: %d, line: %2ld -> %s", line_type, nread, line);

            if (LINE_TYPE_SECTION == line_type) {
                section = cfgp_fix_section(line);
                DEBUG_LOG(TAG_INI, "section: %s", section);
                section = cfgp_add_section(hdl, section)->s_name;
            }
            else if (LINE_TYPE_KEY_VALUE == line_type) {
                char *value = line;
                char *key   = NULL;
                key   = strsep(&value, "=");
                key   = cfgp_fx_value(key);
                value = cfgp_fx_value(value);
                DEBUG_LOG(TAG_INI, "section: %s, key: %s; value: %s", section, key, value);
                cfgp_add_key_value(hdl, section, key, value);
            }
        }
        free(line);
        ret = 0;
    } while (0);
    if (ret && hdl) {
        cfgp_release(hdl);
    }
    if (fp) {
        fclose(fp);
    }
    DEBUG_LOG(TAG_INI, "ret: %d", ret);
    return ret;
}

/**
 * [cfgp_get_sections_count description]
 * get the count of sections from configparser.
 *
 * @param   hdl  [configparser handle]
 *
 * @return       [success: >=0; error: -1]
 */
int cfgp_get_sections_count(struct configparser *hdl)
{
    int count = -1;
    if (hdl) {
        struct q_list_head *s_head = NULL;
        struct q_list_head *s_pos  = NULL;

        count  = 0;
        s_head = &hdl->sections.s_list;
        q_list_for_each(s_pos, s_head) {
            ++count;
        }
    }
    DEBUG_LOG(TAG_INI, "hdl: %p, count: %d", hdl, count);
    return count;
}

/**
 * [cfgp_get_sections description]
 * get the [index] section from configparser.
 *
 * @param   hdl      [configparser handle]
 * @param   section  [section must be enough]
 * @param   index    [the index of section in configparser]
 *
 * @return           [success: 0; error: -1]
 */
int cfgp_get_sections(struct configparser *hdl,
                                 char                   *section,
                                 int                    index
                                 )
{
    int pos = -1;
    if (hdl && section) {
        struct q_list_head *s_head = NULL;
        struct q_list_head *s_pos  = NULL;
        struct cfgp_sec   *s_entry = NULL;

        s_head = &hdl->sections.s_list;
        q_list_for_each(s_pos, s_head) {
            ++pos;
            if (pos == index) {
                s_entry = q_list_entry(s_pos, struct cfgp_sec, s_list);
                strcpy(section, s_entry->s_name);
                break;
            }
        }
    }
    DEBUG_LOG(TAG_INI, "hdl: %p, index: %d, pos: %d", hdl, index, pos);
    return (pos == index ? 0 : -1);
}

/**
 * [cfgp_get_kv_value description]
 * get the value of key by section from configparser.
 *
 * @param   hdl      [configparser handle]
 * @param   section  [section string]
 * @param   key      [section string]
 * @param   value    [value must be enough]
 *
 * @return           [success: 0; error: -1]
 */
int cfgp_get_kv_value(struct configparser *hdl,
                                 const char             *section,
                                 const char             *key,
                                 char                   *value
                                 )
{
    struct cfgp_sec   *s_item  = NULL;
    struct cfgp_kv  *kv_item = NULL;
    struct q_list_head *head    = NULL;

    do {
        if (!hdl || !section || !key || !value) {
            break;
        }
        head = &hdl->sections.s_list;
        if (!(s_item = cfgp_find_section(head, section))) {
            break;
        }
        head = &s_item->kvs.kv_list;
        if (!(kv_item = cfgp_find_kv(head, key))) {
            break;
        }
        strcpy(value, kv_item->value);
    } while (0);
    DEBUG_LOG(TAG_INI, "hdl: %p, kv_item: %p", hdl, kv_item);
    return (kv_item ? 0 : -1);
}

/**
 * [cfgp_mod_kv_value description]
 * modify the value of key by section into configparser.
 *
 * @param   hdl      [configparser handle]
 * @param   section  [section string]
 * @param   key      [key string]
 * @param   value    [value string]
 *
 * @return           [success: 0; error: -1]
 */
int cfgp_mod_kv_value(struct configparser *hdl,
                                 const char             *section,
                                 const char             *key,
                                 const char             *value
                                 )
{
    return cfgp_add_key_value(hdl, section, key, value) ? 0 : -1;
}

/**
 * [cfgp_write_to_fp description]
 * write configparser data into file.
 *
 * @param   hdl  [configparser handle]
 *
 * @return       [success: 0; error: -1]
 */
static int cfgp_write_to_fp(struct configparser *hdl)
{
    int                 ret      = 0;
    FILE                *fp      = NULL;
    struct q_list_head *s_head  = NULL;
    struct q_list_head *s_pos   = NULL;
    struct cfgp_sec   *s_entry = NULL;

    struct q_list_head *kv_head  = NULL;
    struct q_list_head *kv_pos   = NULL;
    struct cfgp_kv  *kv_entry = NULL;

    do {
        if (!(hdl->c_path)) {
            break;
        }
        DEBUG_LOG(TAG_INI, "[path] %s", hdl->c_path);
        if (!(fp = fopen(hdl->c_path, "wb"))) {
            break;
        }

        s_head = &hdl->sections.s_list;
        q_list_for_each(s_pos, s_head) {
            s_entry = q_list_entry(s_pos, struct cfgp_sec, s_list);
            DEBUG_LOG(TAG_INI, "> %s", s_entry->s_name);
            fprintf(fp, "\n[%s]\n", s_entry->s_name);

            kv_head = &s_entry->kvs.kv_list;
            q_list_for_each(kv_pos, kv_head) {
                kv_entry = q_list_entry(kv_pos, struct cfgp_kv, kv_list);
                DEBUG_LOG(TAG_INI, "  >> %s = %s", kv_entry->key, kv_entry->value);
                fprintf(fp, "%s = %s\n", kv_entry->key, kv_entry->value);
            }
        }
    } while (0);
    if (fp) {
        fclose(fp);
    }
    DEBUG_LOG(TAG_INI, "ret: %d", ret);
    return ret;
}

/**
 * [cfgp_write description]
 * write configparser data into the filepath (path/buid-in).
 *
 * @param   hdl   [configparser handle]
 * @param   path  [description]
 *
 * @return        [success: 0; error: -1]
 */
int cfgp_write(struct configparser *hdl, const char *path)
{
    int ret = -1;

    if (hdl) {
        cfgp_add_path(hdl, path);
        ret = cfgp_write_to_fp(hdl);
    }
    DEBUG_LOG(TAG_INI, "ret: %d", ret);
    return ret;
}


#ifdef LIST_CONFIGPARSER
/**
 * [cfgp_list description]
 * list all data of configparser.
 *
 * @param  hdl  [configparser handle]
 */
void cfgp_list(struct configparser *hdl)
{
    struct q_list_head *s_head  = NULL;
    struct q_list_head *s_pos   = NULL;
    struct cfgp_sec   *s_entry = NULL;

    struct q_list_head *kv_head  = NULL;
    struct q_list_head *kv_pos   = NULL;
    struct cfgp_kv  *kv_entry = NULL;

    DEBUG_LOG(TAG_INI, "[path] %s", hdl->c_path);
    s_head = &hdl->sections.s_list;
    q_list_for_each(s_pos, s_head) {
        s_entry = q_list_entry(s_pos, struct cfgp_sec, s_list);
        DEBUG_LOG(TAG_INI, "> %s", s_entry->s_name);

        kv_head = &s_entry->kvs.kv_list;
        q_list_for_each(kv_pos, kv_head) {
            kv_entry = q_list_entry(kv_pos, struct cfgp_kv, kv_list);
            DEBUG_LOG(TAG_INI, "  >> %s = %s", kv_entry->key, kv_entry->value);
        }
    }
}
#endif
