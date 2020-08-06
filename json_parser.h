#ifndef JSON_PARSER_H_
#define JSON_PARSER_H_

#include <stdlib.h>
#include <string.h>

#define JSMN_STATIC
#include "jsmn.h"

#include "debug_if.h"
#include "queues.h"
#include "mqtt_client_app.h"

#define JSON_TIMESTAMP "ts"
#define JSON_SEQUENCE_NUM "sn"
#define JSON_PAYLOAD "pload"

int jsoneq(const char *json, jsmntok_t *tok, const char *s);
int json_unpack(unpackedMsg* unpacked, char* inStr);
int json_pack(unpackedMsg* unpacked, char* outStr);

#endif /* JSON_PARSER_H_ */
