/*
* @Author: matthew
* @Date:   2018-05-19
* @Last Modified by:   matthew
* @Last Modified time: 2018-06-16
*/

#include <stdio.h>
#include "configparser/configparser.h"


int main(int argc, char const *argv[])
{
    struct configparser config;

    cfgp_init(&config);
    cfgp_read(&config, "./config.ini");

    printf("\n\n\n");
    cfgp_get_sections_count(&config);
    char section[20] = {0};
    cfgp_get_sections(&config, section, 1);
    printf("section: %s\n", section);

    printf("\n\n\n");
    char value[20] = {0};
    cfgp_get_kv_value(&config, section, "minute", value);
    printf("minute = %s\n", value);

    cfgp_mod_kv_value(&config, section, "minute", "11");
    printf("\n\n\n");
    // cfgp_write(&config, "./config_bak.ini");
    cfgp_write(&config, NULL);

#ifdef LIST_CONFIGPARSER
    printf("\n\n\n");
    cfgp_list(&config);
#endif

    cfgp_release(&config);
    return 0;
}
