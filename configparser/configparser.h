/*
* @Author: matthew
* @Date:   2018-05-19
* @Last Modified by:   matthew
* @Last Modified time: 2018-05-19
*/


#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

/**********************************************************
 * Includes
**********************************************************/
#include "list/list.h"


/**********************************************************
 * Macros
**********************************************************/
// #define LIST_CONFIGPARSER


/**********************************************************
 * Types
**********************************************************/
/* key- value */
struct cfgp_kv
{
    struct q_list_head kv_list;
    char               *key;
    char               *value;
};

/* section */
struct cfgp_sec
{
    struct q_list_head s_list;
    char               *s_name;
    struct cfgp_kv     kvs;
};

struct configparser
{
    char            *c_path;
    struct cfgp_sec sections;
};


/**********************************************************
 * Fucntions
**********************************************************/
/* initialize the configparser */
extern int cfgp_init(struct configparser *hdl);

/* release the configparser */
extern int cfgp_release(struct configparser *hdl);

/**
 * add the path argument into the handle;
 *
 * if path is NULL, keep the handle's c_path;
 * if path is not NULL, replace handle's c_path;
 */
extern int cfgp_add_path(struct configparser *hdl, const char *path);

/* add a section into configparser */
extern struct cfgp_sec *cfgp_add_section(struct configparser *hdl,
                                         const char          *section
                                         );

/* add/modify the key-value into/of configparser */
extern struct cfgp_kv *cfgp_add_key_value(struct configparser *hdl,
                                           const char          *section,
                                           const char          *key,
                                           const char          *value
                                           );

/* read data into configparser from the filepath */
extern int cfgp_read(struct configparser *hdl, const char *path);

/* get the count of sections from configparser */
extern int cfgp_get_sections_count(struct configparser *hdl);

/* get the [index] section from configparser */
extern int cfgp_get_sections(struct configparser *hdl,
                             char                *section,
                             int                  index
                             );

/* get the value of key by section from configparser */
extern int cfgp_get_kv_value(struct configparser *hdl,
                             const char          *section,
                             const char          *key,
                             char                *value
                             );

/* modify the value of key by section into configparser */
extern int cfgp_mod_kv_value(struct configparser *hdl,
                             const char          *section,
                             const char          *key,
                             const char          *value
                             );

/* write configparser data into the filepath (path/buid-in) */
extern int cfgp_write(struct configparser *hdl, const char *path);


#ifdef LIST_CONFIGPARSER
/* list all data of configparser */
extern void cfgp_list(struct configparser *hdl);
#endif


#endif // CONFIGPARSER_H
