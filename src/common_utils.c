#include "common_utils.h"
#include "json.h"
#include "common_define.h"

const char *get_string_from_jsonobject(const json_object *src, const char *key)
{
    json_object *jobj = json_object_object_get(src, key);
    if(json_object_get_type(jobj) != json_type_string)
        return NULL;
    return json_object_get_string(jobj);
}

void put_int_value_to_jsonstring(char *dest, const char *key, int value)
{
    json_object *jobj = json_tokener_parse(dest);
    if(json_object_get_type(jobj) != json_type_object){
        json_object *new_jobj = json_object_new_object();
        json_object_object_add(new_jobj, key, json_object_new_int(value));
        const char *jstr = json_object_to_json_string(new_jobj);
        strcpy(dest, jstr);
        json_object_put(new_jobj);
    }else{
        json_object *exist_value = json_object_object_get(jobj, key);
        if(exist_value == NULL){
            json_object_object_add(jobj, key, json_object_new_int(value));
        }else{
            json_object_object_del(jobj, key);
            json_object_object_add(jobj, key, json_object_new_int(value));
        }
        const char *jstr = json_object_to_json_string(jobj);
        strcpy(dest, jstr);
    }

    json_object_put(jobj);
    if(new_jobj != NULL)
        json_object_put(new_jobj);
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
        const char *jstr = json_object_to_json_string(jobj);
        strcpy(dest, jstr);
    }

    json_object_put(jobj);
    if(new_jobj != NULL)
        json_object_put(new_jobj);
}
