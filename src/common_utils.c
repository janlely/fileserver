#include "common_utils.h"
#include <string.h>
#include "json-c/json.h"
#include "common_define.h"
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <ossp/uuid.h>
#include <sys/timeb.h>

int is_json_string(const char *dest)
{
    if(dest == NULL || strlen(dest) == 0)
        return 0;
    json_object *jobj = json_tokener_parse(dest);
    if(json_object_get_type(jobj) != json_type_object)
        return 0;
    return 1;
}

void get_string_from_jsonobject(const json_object *src, char *dest, const char *key)
{
    json_object *jobj = json_object_object_get(src, key);
    if(json_object_get_type(jobj) != json_type_string)
        strcpy(dest, "");
    strcpy(dest, json_object_get_string(jobj));
}

void put_int_value_to_jsonstring(char *dest, const char *key, int value)
{
    if(dest == NULL){
        return;
    }
    json_object *jobj = NULL;
    if(strlen(dest) == 0 || !is_json_string(dest)){
        json_object *new_jobj = json_object_new_object();
        json_object_object_add(new_jobj, key, json_object_new_int(value));
        const char *jstr = json_object_to_json_string(new_jobj);
        strcpy(dest, jstr);
        json_object_put(new_jobj);
    }else{
        jobj = json_tokener_parse(dest);
        json_object *exist_value = json_object_object_get(jobj, key);
        if(exist_value == NULL){
            json_object_object_add(jobj, key, json_object_new_int(value));
        }else{
            json_object_object_del(jobj, key);
            json_object_object_add(jobj, key, json_object_new_int(value));
        }
        const char *jstr = json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_NOSLASHESCAPE);
        strcpy(dest, jstr);
        json_object_put(jobj);
    }
}

void put_string_value_to_jsonstring(char *dest, const char *key, const char *value)
{
    json_object *jobj = json_tokener_parse(dest);
    if(json_object_get_type(jobj) != json_type_object){
        json_object *new_jobj = json_object_new_object();
        json_object_object_add(new_jobj, key, json_object_new_string(value));
        const char *jstr = json_object_to_json_string(new_jobj);
        strcpy(dest, jstr);
        json_object_put(new_jobj);
    }else{
        json_object *exist_value = json_object_object_get(jobj, key);
        if(exist_value == NULL){
            json_object_object_add(jobj, key, json_object_new_string(value));
        }else{
            json_object_object_del(jobj, key);
            json_object_object_add(jobj, key, json_object_new_string(value));
        }
        const char *jstr = json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_NOSLASHESCAPE);
        strcpy(dest, jstr);
    }

    json_object_put(jobj);
}

void get_uuid(char *dest)
{
    char *str = NULL;
    uuid_t *uuid;
    uuid_create(&uuid);
    uuid_make(uuid, UUID_MAKE_V1);
    uuid_export(uuid, UUID_FMT_STR, &str, NULL);
    uuid_destroy(uuid);

    strncpy(dest, str, 8);/*strncpy won't append \0 to the end*/
    dest[8] = 0; /*append \0 manually*/
    strncat(dest, str+9, 4);
    strncat(dest, str+14, 4);
    strncat(dest, str+19, 4);
    strncat(dest, str+24, 12);

    free(str);
}



void get_base_path(char *str, size_t len, const char *type)
{
    sprintf(str, "/upload/%s", type);
    time_t now = time(NULL);
    struct tm *t_info;
    t_info = localtime(&now);
    char tmp[20];
    strftime (tmp, sizeof(tmp), "/%Y-%m/%d", t_info);
    if(strlen(tmp) > len)
    {
        printf("error: base_path array not enougth space!\n");
        return;
    }
    strcat(str, tmp);
}

int mkdirs(const char *dir)
{
    char tmp[256];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp),"%s",dir);
    len = strlen(tmp);
    if(tmp[len - 1] == '/')
        tmp[len - 1] = 0;
    for(p = tmp + 1; *p; p++)
        if(*p == '/') {
            *p = 0;
            if(!dir_exist(tmp) && mkdir(tmp, S_IRWXU) != 0){
                printf("error: can't create dir: %s!\n", tmp);
                return 0;
            }
            *p = '/';
        }
    if(!dir_exist(tmp) && mkdir(tmp, S_IRWXU) != 0){
        printf("error: can't create dir: %s!\n", tmp);
        return 0;
    }
    return 1;
}



int dir_exist(const char *dir)
{
    if(access(dir, F_OK) == 0)
        return 1;/*exist*/
    else
        return 0;/*not exist*/
}

int dir_writeable(const char *dir)
{
    if(access(dir, W_OK) == 0)
        return 1;/*readable*/
    else
        return 0;/*un readable*/
}

long long get_current_time_mil()
{
    struct timeb t;
    ftime(&t);
    return 1000 * t.time + t.millitm;
}

void *memstr(const void *l, size_t l_len, const void *s, size_t s_len)
{
    char *cur, *last;
	const char *cl = (const char *)l;
	const char *cs = (const char *)s;

	if (l_len == 0 || s_len == 0)
		return NULL;

	if (l_len < s_len)
		return NULL;

	if (s_len == 1)
		return memchr(l, (int)*cs, l_len);

	last = (char *)cl + l_len - s_len;

	for (cur = (char *)cl; cur <= last; cur++)
		if (cur[0] == cs[0] && memcmp(cur, cs, s_len) == 0)
			return cur;

	return NULL;
}


