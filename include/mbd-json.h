#ifndef KNIGHT_JSON_H
#define KNIGHT_JSON_H

#include <stdint.h>

typedef enum json_type {
    JSON_ARRAY_START = 0,
    JSON_ARRAY_END,
    JSON_BOOLEAN,
    JSON_NULL,
    JSON_NUMBER,
    JSON_OBJECT_START,
    JSON_OBJECT_END,
    JSON_STRING
} json_type_t;

typedef enum json_parsing {
    JSON_OBJECT_OR_ARRAY_BEFORE = 0,
    JSON_OBJECT_PROPERTY_BEFORE,
    JSON_OBJECT_PROPERTY,
    JSON_OBJECT_PROPERTY_AFTER,
    JSON_OBJECT_VALUE_BEFORE,
    JSON_OBJECT_VALUE_STRING,
    JSON_OBJECT_VALUE_NUMBER,
    JSON_OBJECT_VALUE_AFTER,
    JSON_ARRAY_VALUE_BEFORE,
    JSON_ARRAY_VALUE_STRING,
    JSON_ARRAY_VALUE_NUMBER,
    JSON_ARRAY_VALUE_AFTER
} json_parsing_t;

typedef enum json_parse_result {
    JSON_RESULT_VALID = 0,
    JSON_RESULT_ITEM,
    JSON_RESULT_INVALID,
    JSON_RESULT_TOO_DEEP
} json_parse_result_t;

typedef struct json_start_end {
    uint16_t start;
    uint16_t end;
} json_start_end_t;

typedef struct json_data {
    uint16_t pos;

    int8_t max_depth;
    int8_t depth;
    
    json_parsing_t* parsing;

    json_start_end_t *path;
    json_start_end_t value;
    json_type_t type;
    uint8_t *item_counter;
} json_data_t;

void reset_json_data(json_data_t* data);
json_parse_result_t parse_json(const uint8_t* bytes, const uint16_t size, json_data_t* data);

#endif