#ifndef _COMMON_UTILS_H
#define _COMMON_UTILS_H
#include <json-c/json.h>

#define lc_strncmp(s1, s2, n)  strncmp((const char *) s1, (const char *) s2, n)

void get_string_from_jsonobject(const json_object *src, char *dest, const char *key);

void put_string_value_to_jsonstring(char *dest, const char *key, const char *value);

void get_uuid(char *str);

void get_base_path(char *str, size_t len, const char *type);

void put_int_value_to_jsonstring(char *dest, const char *key, int value);

int mkdirs(const char *dir);

int dir_exist(const char *dir);

int dir_writeable(const char *dir);

long long get_current_time_mil();

void *memstr(const void *l, size_t l_len, const void *s, size_t s_len);

#endif

