/*
* @Author: matthew.lan
* @Date:   2018-05-19
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-16
*/

#include "configparser/configparser.h"
#include "log/log.h"

#define TAG_TEST    "TEST"


int main(int argc, char const *argv[])
{
    struct configparser config;

    log_tags_set("TEST CFGP ");

    cfgp_init(&config);
    cfgp_read(&config, "./config.ini");

    Q_INFO_LOG(TAG_TEST, "\n\n\n");
    cfgp_get_sections_count(&config);
    char section[20] = {0};
    cfgp_get_sections(&config, section, 1);
    Q_INFO_LOG(TAG_TEST, "section: %s\n", section);

    Q_INFO_LOG(TAG_TEST, "\n\n\n");
    char value[20] = {0};
    cfgp_get_kv_value(&config, section, "minute", value);
    Q_INFO_LOG(TAG_TEST, "minute = %s\n", value);

    cfgp_mod_kv_value(&config, section, "minute", "11");
    Q_INFO_LOG(TAG_TEST, "\n\n\n");
    // cfgp_write(&config, "./config_bak.ini");
    cfgp_write(&config, NULL);

#ifdef LIST_CONFIGPARSER
    Q_INFO_LOG(TAG_TEST, "\n\n\n");
    cfgp_list(&config);
#endif

    cfgp_release(&config);
    return 0;
}
