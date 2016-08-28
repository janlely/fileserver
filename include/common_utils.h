#ifndef _COMMON_UTILS_H
#define _COMMON_UTILS_H
#include <json.h>

const char * get_string_from_jsonobejct(const json_object *src, const char *key);
void put_string_value_to_jsonstring(char *dest, const char *key, const char *value);
#endif
