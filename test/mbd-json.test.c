#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "../include/mbd-json.h"

#define done() return 0

static int check_count = 0;
#define check(x) check_count++; if (!(x)) { return __LINE__; }

struct test {
    int(*func)(void);
    char const* name;
};

static int test_suit(struct test const* tests, int test_count) {
    printf("\n\nTests:\n");
    int failed = 0;
    
    for (int i = 0; i < test_count; i++) {
        printf("\n %02d: %-25s", i, tests[i].name);
        int error_line = tests[i].func();

        if (error_line > 0) {
            printf("... failed at line: %i\n", error_line);
            failed++;
        }
        else {
            printf("... ok\n");
        }
    }

    printf("\n%s%d\n", "Total checks: ", check_count);
    printf("Tests passing: %i/%i\r\n\n\n", test_count - failed, test_count);

    return failed;
}

static int test_primitive_object(void) {
    json_data_t data;
    data.pos = 0;
    data.max_depth = 5;

    json_parsing_t parsing[5];
    data.parsing = &parsing[0];

    json_start_end_t path[5];
    data.path = &path[0];

    uint8_t item_counter[5];
    data.item_counter = &item_counter[0];

    {
        reset_json_data(&data);
        const uint8_t json[] = "{}";
        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.depth == 0);
        check(data.item_counter[0] == 0);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n} \r\t\n";
        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.depth == 0);
        check(data.item_counter[0] == 0);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":null}";
        
        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.type == JSON_NULL);
        check(data.value.start == 13);
        check(data.value.end == 16);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\nnull \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.type == JSON_NULL);
        check(data.value.start == 29);
        check(data.value.end == 32);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":true}";
        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 13);
        check(data.value.end == 16);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);
        
        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\ntrue \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 29);
        check(data.value.end == 32);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":false}";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 13);
        check(data.value.end == 17);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\nfalse \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 29);
        check(data.value.end == 33);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":0}";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 13);
        check(data.value.end == 13);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":0.0}";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 13);
        check(data.value.end == 15);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":-0.0}";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 13);
        check(data.value.end == 16);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n-0.0 \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 29);
        check(data.value.end == 32);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
         const uint8_t json[] = "{\"property1\":\"string\"}";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.type == JSON_STRING);
        check(data.value.start == 13);
        check(data.value.end == 20);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n\"string\" \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.type == JSON_STRING);
        check(data.value.start == 29);
        check(data.value.end == 36);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":null,\"property2\":true,\"property3\":false,\"property4\":-0.0,\"property5\":\"string\"}";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.type == JSON_NULL);
        check(data.value.start == 13);
        check(data.value.end == 16);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 18);
        check(data.path[0].end == 28);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 30);
        check(data.value.end == 33);
        check(data.item_counter[0] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 35);
        check(data.path[0].end == 45);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 47);
        check(data.value.end == 51);
        check(data.item_counter[0] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 53);
        check(data.path[0].end == 63);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 65);
        check(data.value.end == 68);
        check(data.item_counter[0] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 70);
        check(data.path[0].end == 80);
        check(data.type == JSON_STRING);
        check(data.value.start == 82);
        check(data.value.end == 89);
        check(data.item_counter[0] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 5);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\nnull \r\t\n, \r\t\n\"property2\" \r\t\n: \r\t\ntrue \r\t\n, \r\t\n\"property3\" \r\t\n: \r\t\nfalse \r\t\n, \r\t\n\"property4\" \r\t\n: \r\t\n-0.0 \r\t\n, \r\t\n\"property5\" \r\t\n: \r\t\n\"string\" \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.type == JSON_NULL);
        check(data.value.start == 29);
        check(data.value.end == 32);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 42);
        check(data.path[0].end == 52);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 62);
        check(data.value.end == 65);
        check(data.item_counter[0] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 75);
        check(data.path[0].end == 85);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 95);
        check(data.value.end == 99);
        check(data.item_counter[0] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 109);
        check(data.path[0].end == 119);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 129);
        check(data.value.end == 132);
        check(data.item_counter[0] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 142);
        check(data.path[0].end == 152);
        check(data.type == JSON_STRING);
        check(data.value.start == 162);
        check(data.value.end == 169);
        check(data.item_counter[0] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 5);
    }

    return 0;
}

static int test_primitive_array(void) {
    json_data_t data;
    data.pos = 0;
    data.max_depth = 5;

    json_parsing_t parsing[5];
    data.parsing = &parsing[0];

    json_start_end_t path[5];
    data.path = &path[0];

    uint8_t item_counter[5];
    data.item_counter = &item_counter[0];

    {
        reset_json_data(&data);
        const uint8_t json[] = "[]";
    
        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.depth == 0);
        check(data.item_counter[0] == 0);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.depth == 0);
        check(data.item_counter[0] == 0);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[null]";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_NULL);
        check(data.value.start == 1);
        check(data.value.end == 4);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\nnull \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_NULL);
        check(data.value.start == 9);
        check(data.value.end == 12);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[true]";

        int result = parse_json(json, sizeof(json), &data);
    
        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 1);
        check(data.value.end == 4);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\ntrue \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 9);
        check(data.value.end == 12);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[false]";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 1);
        check(data.value.end == 5);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\nfalse \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 9);
        check(data.value.end == 13);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[-0.0]";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 1);
        check(data.value.end == 4);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);    
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n-0.0 \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 9);
        check(data.value.end == 12);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[\"string\"]";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_STRING);
        check(data.value.start == 1);
        check(data.value.end == 8);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n\"string\" \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_STRING);
        check(data.value.start == 9);
        check(data.value.end == 16);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[null,true,false,-0.0,\"string\"]";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_NULL);
        check(data.value.start == 1);
        check(data.value.end == 4);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 6);
        check(data.value.end == 9);
        check(data.item_counter[0] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 11);
        check(data.value.end == 15);
        check(data.item_counter[0] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 17);
        check(data.value.end == 20);
        check(data.item_counter[0] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_STRING);
        check(data.value.start == 22);
        check(data.value.end == 29);
        check(data.item_counter[0] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\nnull \r\t\n, \r\t\ntrue \r\t\n, \r\t\nfalse \r\t\n, \r\t\n-0.0 \r\t\n, \r\t\n\"string\" \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 4 );
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_NULL);
        check(data.value.start == 9);
        check(data.value.end == 12);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 22);
        check(data.value.end == 25);
        check(data.item_counter[0] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 35);
        check(data.value.end == 39);
        check(data.item_counter[0] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 49);
        check(data.value.end == 52);
        check(data.item_counter[0] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_STRING);
        check(data.value.start == 62);
        check(data.value.end == 69);
        check(data.item_counter[0] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 5);
    }

    return 0;
}

static int test_complex_object(void) {
    json_data_t data;
    data.pos = 0;
    data.max_depth = 5;

    json_parsing_t parsing[5];
    data.parsing = &parsing[0];

    json_start_end_t path[5];
    data.path = &path[0];

    uint8_t item_counter[5];
    data.item_counter = &item_counter[0];

    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":{}}";
        
        int result = parse_json(json, sizeof(json), &data);
        
        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 13);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 14);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n{ \r\t\n} \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);
        
        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 29);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 34);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"}}";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 13);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 14);
        check(data.path[1].end == 25);
        check(data.type == JSON_NULL);
        check(data.value.start == 27);
        check(data.value.end == 30);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 1);

        result = parse_json(json, sizeof(json), &data);
        
        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 32);
        check(data.path[1].end == 43);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 45);
        check(data.value.end == 48);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 50);
        check(data.path[1].end == 61);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 63);
        check(data.value.end == 67);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 69);
        check(data.path[1].end == 80);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 82);
        check(data.value.end == 85);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 87);
        check(data.path[1].end == 98);
        check(data.type == JSON_STRING);
        check(data.value.start == 100);
        check(data.value.end == 107);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 108);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n{ \r\t\n\"property11\" \r\t\n: \r\t\nnull \r\t\n, \r\t\n\"property12\" \r\t\n: \r\t\ntrue \r\t\n, \r\t\n\"property13\" \r\t\n: \r\t\nfalse \r\t\n, \r\t\n\"property14\" \r\t\n: \r\t\n-0.0 \r\t\n, \r\t\n\"property15\" \r\t\n: \r\t\n\"string\" \r\t\n} \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 29);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 34);
        check(data.path[1].end == 45);
        check(data.type == JSON_NULL);
        check(data.value.start == 55);
        check(data.value.end == 58);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 68);
        check(data.path[1].end == 79);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 89);
        check(data.value.end == 92);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 102);
        check(data.path[1].end == 113);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 123);
        check(data.value.end == 127);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 137);
        check(data.path[1].end == 148);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 158);
        check(data.value.end == 161);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 171);
        check(data.path[1].end == 182);
        check(data.type == JSON_STRING);
        check(data.value.start == 192);
        check(data.value.end == 199);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 204);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":{},\"property2\":{}}";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 13);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 14);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 16);
        check(data.path[0].end == 26);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 28);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 16);
        check(data.path[0].end == 26);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 29);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n{ \r\t\n} \r\t\n, \r\t\n\"property2\" \r\t\n: \r\t\n{ \r\t\n} \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 29);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 34);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 44);
        check(data.path[0].end == 54);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 64);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 44);
        check(data.path[0].end == 54);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 69);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"},\"property2\":{\"property21\":null,\"property22\":true,\"property23\":false,\"property24\":-0.0,\"property25\":\"string\"}}";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 13);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 14);
        check(data.path[1].end == 25);
        check(data.type == JSON_NULL);
        check(data.value.start == 27);
        check(data.value.end == 30);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 32);
        check(data.path[1].end == 43);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 45);
        check(data.value.end == 48);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 50);
        check(data.path[1].end == 61);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 63);
        check(data.value.end == 67);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 69);
        check(data.path[1].end == 80);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 82);
        check(data.value.end == 85);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 87);
        check(data.path[1].end == 98);
        check(data.type == JSON_STRING);
        check(data.value.start == 100);
        check(data.value.end == 107);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 108);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 110);
        check(data.path[0].end == 120);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 122);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 110);
        check(data.path[0].end == 120);
        check(data.path[1].start == 123);
        check(data.path[1].end == 134);
        check(data.type == JSON_NULL);
        check(data.value.start == 136);
        check(data.value.end == 139);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 110);
        check(data.path[0].end == 120);
        check(data.path[1].start == 141);
        check(data.path[1].end == 152);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 154);
        check(data.value.end == 157);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 110);
        check(data.path[0].end == 120);
        check(data.path[1].start == 159);
        check(data.path[1].end == 170);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 172);
        check(data.value.end == 176);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 110);
        check(data.path[0].end == 120);
        check(data.path[1].start == 178);
        check(data.path[1].end == 189);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 191);
        check(data.value.end == 194);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 110);
        check(data.path[0].end == 120);
        check(data.path[1].start == 196);
        check(data.path[1].end == 207);
        check(data.type == JSON_STRING);
        check(data.value.start == 209);
        check(data.value.end == 216);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 110);
        check(data.path[0].end == 120);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 217);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n{ \r\t\n\"property11\" \r\t\n: \r\t\nnull \r\t\n, \r\t\n\"property12\" \r\t\n: \r\t\ntrue \r\t\n, \r\t\n\"property13\" \r\t\n: \r\t\nfalse \r\t\n, \r\t\n\"property14\" \r\t\n: \r\t\n-0.0 \r\t\n, \r\t\n\"property15\" \r\t\n: \r\t\n\"string\" \r\t\n}, \r\t\n\"property2\" \r\t\n: \r\t\n{ \r\t\n\"property21\" \r\t\n: \r\t\nnull \r\t\n, \r\t\n\"property22\" \r\t\n: \r\t\ntrue \r\t\n, \r\t\n\"property23\" \r\t\n: \r\t\nfalse \r\t\n, \r\t\n\"property24\" \r\t\n: \r\t\n-0.0 \r\t\n, \r\t\n\"property25\" \r\t\n: \r\t\n\"string\" \r\t\n} \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 29);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 34);
        check(data.path[1].end == 45);
        check(data.type == JSON_NULL);
        check(data.value.start == 55);
        check(data.value.end == 58);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 1);
        
        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 68);
        check(data.path[1].end == 79);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 89);
        check(data.value.end == 92);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 102);
        check(data.path[1].end == 113);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 123);
        check(data.value.end == 127);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 137);
        check(data.path[1].end == 148);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 158);
        check(data.value.end == 161);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 171);
        check(data.path[1].end == 182);
        check(data.type == JSON_STRING);
        check(data.value.start == 192);
        check(data.value.end == 199);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 204);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 210);
        check(data.path[0].end == 220);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 230);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 210);
        check(data.path[0].end == 220);
        check(data.path[1].start == 235);
        check(data.path[1].end == 246);
        check(data.type == JSON_NULL);
        check(data.value.start == 256);
        check(data.value.end == 259);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 210);
        check(data.path[0].end == 220);
        check(data.path[1].start == 269);
        check(data.path[1].end == 280);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 290);
        check(data.value.end == 293);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 210);
        check(data.path[0].end == 220);
        check(data.path[1].start == 303);
        check(data.path[1].end == 314);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 324);
        check(data.value.end == 328);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 210);
        check(data.path[0].end == 220);
        check(data.path[1].start == 338);
        check(data.path[1].end == 349);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 359);
        check(data.value.end == 362);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 210);
        check(data.path[0].end == 220);
        check(data.path[1].start == 372);
        check(data.path[1].end == 383);
        check(data.type == JSON_STRING);
        check(data.value.start == 393);
        check(data.value.end == 400);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 210);
        check(data.path[0].end == 220);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 405);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":[]}";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 13);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 14);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n[ \r\t\n] \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 29);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 29);
        check(data.path[1].end == 29);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 34);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":[null,true,false,-0.0,\"string\"]}";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 13);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_NULL);
        check(data.value.start == 14);
        check(data.value.end == 17);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 19);
        check(data.value.end == 22);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 24);
        check(data.value.end == 28);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 30);
        check(data.value.end == 33);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_STRING);
        check(data.value.start == 35);
        check(data.value.end == 42);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 43);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n[ \r\t\nnull \r\t\n, \r\t\ntrue \r\t\n, \r\t\nfalse \r\t\n, \r\t\n-0.0 \r\t\n, \r\t\n\"string\" \r\t\n] \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 29);
        check(data.path[1].end == 29);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 29);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 29);
        check(data.path[1].end == 29);
        check(data.type == JSON_NULL);
        check(data.value.start == 34);
        check(data.value.end == 37);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 29);
        check(data.path[1].end == 29);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 47);
        check(data.value.end == 50);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 29);
        check(data.path[1].end == 29);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 60);
        check(data.value.end == 64);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 29);
        check(data.path[1].end == 29);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 74);
        check(data.value.end == 77);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 29);
        check(data.path[1].end == 29);
        check(data.type == JSON_STRING);
        check(data.value.start == 87);
        check(data.value.end == 94);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 29);
        check(data.path[1].end == 29);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 99);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":[],\"property2\":[]}";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 13);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 14);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 16);
        check(data.path[0].end == 26);
        check(data.path[1].start == 28);
        check(data.path[1].end == 28);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 28);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 16);
        check(data.path[0].end == 26);
        check(data.path[1].start == 28);
        check(data.path[1].end == 28);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 29);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n[ \r\t\n] \r\t\n, \r\t\n\"property2\" \r\t\n: \r\t\n[ \r\t\n] \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 29);
        check(data.path[1].end == 29);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 29);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 29);
        check(data.path[1].end == 29);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 34);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 44);
        check(data.path[0].end == 54);
        check(data.path[1].start == 64);
        check(data.path[1].end == 64);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 64);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 44);
        check(data.path[0].end == 54);
        check(data.path[1].start == 64);
        check(data.path[1].end == 64);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 69);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":[null,true,false,-0.0,\"string\"],\"property2\":[null,true,false,-0.0,\"string\"]}";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 13);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_NULL);
        check(data.value.start == 14);
        check(data.value.end == 17);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 1);
        
        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 19);
        check(data.value.end == 22);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 24);
        check(data.value.end == 28);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 30);
        check(data.value.end == 33);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_STRING);
        check(data.value.start == 35);
        check(data.value.end == 42);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 43);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 45);
        check(data.path[0].end == 55);
        check(data.path[1].start == 57);
        check(data.path[1].end == 57);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 57);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 45);
        check(data.path[0].end == 55);
        check(data.path[1].start == 57);
        check(data.path[1].end == 57);
        check(data.type == JSON_NULL);
        check(data.value.start == 58);
        check(data.value.end == 61);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 45);
        check(data.path[0].end == 55);
        check(data.path[1].start == 57);
        check(data.path[1].end == 57);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 63);
        check(data.value.end == 66);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 45);
        check(data.path[0].end == 55);
        check(data.path[1].start == 57);
        check(data.path[1].end == 57);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 68);
        check(data.value.end == 72);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 45);
        check(data.path[0].end == 55);
        check(data.path[1].start == 57);
        check(data.path[1].end == 57);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 74);
        check(data.value.end == 77);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 45);
        check(data.path[0].end == 55);
        check(data.path[1].start == 57);
        check(data.path[1].end == 57);
        check(data.type == JSON_STRING);
        check(data.value.start == 79);
        check(data.value.end == 86);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 45);
        check(data.path[0].end == 55);
        check(data.path[1].start == 57);
        check(data.path[1].end == 57);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 87);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n[ \r\t\nnull \r\t\n, \r\t\ntrue \r\t\n, \r\t\nfalse \r\t\n, \r\t\n-0.0 \r\t\n, \r\t\n\"string\" \r\t\n] \r\t\n, \r\t\n\"property2\" \r\t\n: \r\t\n[ \r\t\nnull \r\t\n, \r\t\ntrue \r\t\n, \r\t\nfalse \r\t\n, \r\t\n-0.0 \r\t\n, \r\t\n\"string\" \r\t\n] \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 29);
        check(data.path[1].end == 29);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 29);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 29);
        check(data.path[1].end == 29);
        check(data.type == JSON_NULL);
        check(data.value.start == 34);
        check(data.value.end == 37);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 29);
        check(data.path[1].end == 29);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 47);
        check(data.value.end == 50);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 2);
        
        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 29);
        check(data.path[1].end == 29);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 60);
        check(data.value.end == 64);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 29);
        check(data.path[1].end == 29);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 74);
        check(data.value.end == 77);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 29);
        check(data.path[1].end == 29);
        check(data.type == JSON_STRING);
        check(data.value.start == 87);
        check(data.value.end == 94);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 9);
        check(data.path[0].end == 19);
        check(data.path[1].start == 29);
        check(data.path[1].end == 29);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 99);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 109);
        check(data.path[0].end == 119);
        check(data.path[1].start == 129);
        check(data.path[1].end == 129);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 129);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 109);
        check(data.path[0].end == 119);
        check(data.path[1].start == 129);
        check(data.path[1].end == 129);
        check(data.type == JSON_NULL);
        check(data.value.start == 134);
        check(data.value.end == 137);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 109);
        check(data.path[0].end == 119);
        check(data.path[1].start == 129);
        check(data.path[1].end == 129);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 147);
        check(data.value.end == 150);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 109);
        check(data.path[0].end == 119);
        check(data.path[1].start == 129);
        check(data.path[1].end == 129);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 160);
        check(data.value.end == 164);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 109);
        check(data.path[0].end == 119);
        check(data.path[1].start == 129);
        check(data.path[1].end == 129);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 174);
        check(data.value.end == 177);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 109);
        check(data.path[0].end == 119);
        check(data.path[1].start == 129);
        check(data.path[1].end == 129);
        check(data.type == JSON_STRING);
        check(data.value.start == 187);
        check(data.value.end == 194);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 109);
        check(data.path[0].end == 119);
        check(data.path[1].start == 129);
        check(data.path[1].end == 129);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 199);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"},\"property2\":[null,true,false,-0.0,\"string\"]}";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 13);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 14);
        check(data.path[1].end == 25);
        check(data.type == JSON_NULL);
        check(data.value.start == 27);
        check(data.value.end == 30);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 32);
        check(data.path[1].end == 43);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 45);
        check(data.value.end == 48);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 50);
        check(data.path[1].end == 61);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 63);
        check(data.value.end == 67);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 69);
        check(data.path[1].end == 80);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 82);
        check(data.value.end == 85);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 87);
        check(data.path[1].end == 98);
        check(data.type == JSON_STRING);
        check(data.value.start == 100);
        check(data.value.end == 107);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 108);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 110);
        check(data.path[0].end == 120);
        check(data.path[1].start == 122);
        check(data.path[1].end == 122);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 122);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(data.depth == 1);
        check(data.path[0].start == 110);
        check(data.path[0].end == 120);
        check(data.path[1].start == 122);
        check(data.path[1].end == 122);
        check(data.type == JSON_NULL);
        check(data.value.start == 123);
        check(data.value.end == 126);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(data.depth == 1);
        check(data.path[0].start == 110);
        check(data.path[0].end == 120);
        check(data.path[1].start == 122);
        check(data.path[1].end == 122);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 128);
        check(data.value.end == 131);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(data.depth == 1);
        check(data.path[0].start == 110);
        check(data.path[0].end == 120);
        check(data.path[1].start == 122);
        check(data.path[1].end == 122);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 133);
        check(data.value.end == 137);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(data.depth == 1);
        check(data.path[0].start == 110);
        check(data.path[0].end == 120);
        check(data.path[1].start == 122);
        check(data.path[1].end == 122);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 139);
        check(data.value.end == 142);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(data.depth == 1);
        check(data.path[0].start == 110);
        check(data.path[0].end == 120);
        check(data.path[1].start == 122);
        check(data.path[1].end == 122);
        check(data.type == JSON_STRING);
        check(data.value.start == 144);
        check(data.value.end == 151);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 110);
        check(data.path[0].end == 120);
        check(data.path[1].start == 122);
        check(data.path[1].end == 122);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 152);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":[null,true,false,-0.0,\"string\"],\"property2\":{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"}}";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 13);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_NULL);
        check(data.value.start == 14);
        check(data.value.end == 17);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 1);
        
        result = parse_json(json, sizeof(json), &data);

        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 19);
        check(data.value.end == 22);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 24);
        check(data.value.end == 28);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 30);
        check(data.value.end == 33);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(data.depth == 1);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_STRING);
        check(data.value.start == 35);
        check(data.value.end == 42);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 1);
        check(data.path[0].end == 11);
        check(data.path[1].start == 13);
        check(data.path[1].end == 13);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 43);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 45);
        check(data.path[0].end == 55);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 57);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(data.depth == 1);
        check(data.path[0].start == 45);
        check(data.path[0].end == 55);
        check(data.path[1].start == 58);
        check(data.path[1].end == 69);
        check(data.type == JSON_NULL);
        check(data.value.start == 71);
        check(data.value.end == 74);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(data.depth == 1);
        check(data.path[0].start == 45);
        check(data.path[0].end == 55);
        check(data.path[1].start == 76);
        check(data.path[1].end == 87);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 89);
        check(data.value.end == 92);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(data.depth == 1);
        check(data.path[0].start == 45);
        check(data.path[0].end == 55);
        check(data.path[1].start == 94);
        check(data.path[1].end == 105);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 107);
        check(data.value.end == 111);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(data.depth == 1);
        check(data.path[0].start == 45);
        check(data.path[0].end == 55);
        check(data.path[1].start == 113);
        check(data.path[1].end == 124);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 126);
        check(data.value.end == 129);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(data.depth == 1);
        check(data.path[0].start == 45);
        check(data.path[0].end == 55);
        check(data.path[1].start == 131);
        check(data.path[1].end == 142);
        check(data.type == JSON_STRING);
        check(data.value.start == 144);
        check(data.value.end == 151);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 45);
        check(data.path[0].end == 55);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 152);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 2);
    }

    return 0;    
}

static int test_complex_array(void) {
    json_data_t data;
    data.pos = 0;
    data.max_depth = 5;

    json_parsing_t parsing[5];
    data.parsing = &parsing[0];

    json_start_end_t path[5];
    data.path = &path[0];

    uint8_t item_counter[5];
    data.item_counter = &item_counter[0];

    {
        reset_json_data(&data);
        const uint8_t json[] = "[[]]";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 1);
        check(data.path[1].end == 1);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 1);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 2);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n[ \r\t\n] \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 9);
        check(data.path[1].end == 9);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 9);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 14);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[[null,true,false,-0.0,\"string\"]]";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 1);
        check(data.path[1].end == 1);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 1);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 1);
        check(data.path[1].end == 1);
        check(data.type == JSON_NULL);
        check(data.value.start == 2);
        check(data.value.end == 5);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 1);
        check(data.path[1].end == 1);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 7);
        check(data.value.end == 10);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 1);
        check(data.path[1].end == 1);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 12);
        check(data.value.end == 16);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 1);
        check(data.path[1].end == 1);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 18);
        check(data.value.end == 21);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 1);
        check(data.path[1].end == 1);
        check(data.type == JSON_STRING);
        check(data.value.start == 23);
        check(data.value.end == 30);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 31);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n[ \r\t\nnull \r\t\n, \r\t\ntrue \r\t\n, \r\t\nfalse \r\t\n, \r\t\n-0.0 \r\t\n, \r\t\n\"string\" \r\t\n] \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 9);
        check(data.path[1].end == 9);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 9);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 9);
        check(data.path[1].end == 9);
        check(data.type == JSON_NULL);
        check(data.value.start == 14);
        check(data.value.end == 17);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 9);
        check(data.path[1].end == 9);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 27);
        check(data.value.end == 30);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 9);
        check(data.path[1].end == 9);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 40);
        check(data.value.end == 44);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 9);
        check(data.path[1].end == 9);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 54);
        check(data.value.end == 57);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 9);
        check(data.path[1].end == 9);
        check(data.type == JSON_STRING);
        check(data.value.start == 67);
        check(data.value.end == 74);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 79);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[[],[]]";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 1);
        check(data.path[1].end == 1);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 1);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 2);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 4);
        check(data.path[1].end == 4);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 5);
        check(data.item_counter[0] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n[ \r\t\n] \r\t\n, \r\t\n[ \r\t\n] \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 9);
        check(data.path[1].end == 9);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 9);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 14);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 24);
        check(data.path[1].end == 24);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 24);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 29);
        check(data.item_counter[0] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[[null,true,false,-0.0,\"string\"],[null,true,false,-0.0,\"string\"]]";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 1);
        check(data.path[1].end == 1);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 1);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 1);
        check(data.path[1].end == 1);
        check(data.type == JSON_NULL);
        check(data.value.start == 2);
        check(data.value.end == 5);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 1);
        check(data.path[1].end == 1);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 7);
        check(data.value.end == 10);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 1);
        check(data.path[1].end == 1);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 12);
        check(data.value.end == 16);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 1);
        check(data.path[1].end == 1);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 18);
        check(data.value.end == 21);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 1);
        check(data.path[1].end == 1);
        check(data.type == JSON_STRING);
        check(data.value.start == 23);
        check(data.value.end == 30);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 31);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 33);
        check(data.path[1].end == 33);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 33);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 33);
        check(data.path[1].end == 33);
        check(data.type == JSON_NULL);
        check(data.value.start == 34);
        check(data.value.end == 37);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 33);
        check(data.path[1].end == 33);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 39);
        check(data.value.end == 42);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 33);
        check(data.path[1].end == 33);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 44);
        check(data.value.end == 48);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 33);
        check(data.path[1].end == 33);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 50);
        check(data.value.end == 53);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 33);
        check(data.path[1].end == 33);
        check(data.type == JSON_STRING);
        check(data.value.start == 55);
        check(data.value.end == 62);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 63);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n[ \r\t\nnull \r\t\n, \r\t\ntrue \r\t\n, \r\t\nfalse \r\t\n, \r\t\n-0.0 \r\t\n, \r\t\n\"string\" \r\t\n] \r\t\n, \r\t\n[ \r\t\nnull \r\t\n, \r\t\ntrue \r\t\n, \r\t\nfalse \r\t\n, \r\t\n-0.0 \r\t\n, \r\t\n\"string\" \r\t\n] \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 9);
        check(data.path[1].end == 9);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 9);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 9);
        check(data.path[1].end == 9);
        check(data.type == JSON_NULL);
        check(data.value.start == 14);
        check(data.value.end == 17);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 9);
        check(data.path[1].end == 9);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 27);
        check(data.value.end == 30);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 9);
        check(data.path[1].end == 9);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 40);
        check(data.value.end == 44);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 9);
        check(data.path[1].end == 9);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 54);
        check(data.value.end == 57);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 9);
        check(data.path[1].end == 9);
        check(data.type == JSON_STRING);
        check(data.value.start == 67);
        check(data.value.end == 74);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 79);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 89);
        check(data.path[1].end == 89);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 89);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 89);
        check(data.path[1].end == 89);
        check(data.type == JSON_NULL);
        check(data.value.start == 94);
        check(data.value.end == 97);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 89);
        check(data.path[1].end == 89);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 107);
        check(data.value.end == 110);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 89);
        check(data.path[1].end == 89);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 120);
        check(data.value.end == 124);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 89);
        check(data.path[1].end == 89);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 134);
        check(data.value.end == 137);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 89);
        check(data.path[1].end == 89);
        check(data.type == JSON_STRING);
        check(data.value.start == 147);
        check(data.value.end == 154);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 159);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[{}]";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 1);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 2);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n{ \r\t\n} \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 9);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 14);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"}]";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 1);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 2);
        check(data.path[1].end == 13);
        check(data.type == JSON_NULL);
        check(data.value.start == 15);
        check(data.value.end == 18);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 1);
        
        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 20);
        check(data.path[1].end == 31);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 33);
        check(data.value.end == 36);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 38);
        check(data.path[1].end == 49);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 51);
        check(data.value.end == 55);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 57);
        check(data.path[1].end == 68);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 70);
        check(data.value.end == 73);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 75);
        check(data.path[1].end == 86);
        check(data.type == JSON_STRING);
        check(data.value.start == 88);
        check(data.value.end == 95);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 96);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n{ \r\t\n\"property11\" \r\t\n: \r\t\nnull \r\t\n, \r\t\n\"property12\" \r\t\n: \r\t\ntrue \r\t\n, \r\t\n\"property13\" \r\t\n: \r\t\nfalse \r\t\n, \r\t\n\"property14\" \r\t\n: \r\t\n-0.0 \r\t\n, \r\t\n\"property15\" \r\t\n: \r\t\n\"string\" \r\t\n} \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 9);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 14);
        check(data.path[1].end == 25);
        check(data.type == JSON_NULL);
        check(data.value.start == 35);
        check(data.value.end == 38);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 1);
        
        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 48);
        check(data.path[1].end == 59);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 69);
        check(data.value.end == 72);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 82);
        check(data.path[1].end == 93);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 103);
        check(data.value.end == 107);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 117);
        check(data.path[1].end == 128);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 138);
        check(data.value.end == 141);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 151);
        check(data.path[1].end == 162);
        check(data.type == JSON_STRING);
        check(data.value.start == 172);
        check(data.value.end == 179);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 184);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[{},{}]";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 1);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 2);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 5);
        check(data.item_counter[0] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n{ \r\t\n} \r\t\n, \r\t\n{ \r\t\n} \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 9);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 14);
        check(data.item_counter[0] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 24);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 29);
        check(data.item_counter[0] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"},{\"property21\":null,\"property22\":true,\"property23\":false,\"property24\":-0.0,\"property25\":\"string\"}]";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 1);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 2);
        check(data.path[1].end == 13);
        check(data.type == JSON_NULL);
        check(data.value.start == 15);
        check(data.value.end == 18);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 1);
        
        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 20);
        check(data.path[1].end == 31);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 33);
        check(data.value.end == 36);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 38);
        check(data.path[1].end == 49);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 51);
        check(data.value.end == 55);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 57);
        check(data.path[1].end == 68);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 70);
        check(data.value.end == 73);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 75);
        check(data.path[1].end == 86);
        check(data.type == JSON_STRING);
        check(data.value.start == 88);
        check(data.value.end == 95);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 96);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 98);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 99);
        check(data.path[1].end == 110);
        check(data.type == JSON_NULL);
        check(data.value.start == 112);
        check(data.value.end == 115);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 1);
        
        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 117);
        check(data.path[1].end == 128);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 130);
        check(data.value.end == 133);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 135);
        check(data.path[1].end == 146);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 148);
        check(data.value.end == 152);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 154);
        check(data.path[1].end == 165);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 167);
        check(data.value.end == 170);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 172);
        check(data.path[1].end == 183);
        check(data.type == JSON_STRING);
        check(data.value.start == 185);
        check(data.value.end == 192);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 193);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n{ \r\t\n\"property11\" \r\t\n: \r\t\nnull \r\t\n, \r\t\n\"property12\" \r\t\n: \r\t\ntrue \r\t\n, \r\t\n\"property13\" \r\t\n: \r\t\nfalse \r\t\n, \r\t\n\"property14\" \r\t\n: \r\t\n-0.0 \r\t\n, \r\t\n\"property15\" \r\t\n: \r\t\n\"string\" \r\t\n} \r\t\n, \r\t\n{ \r\t\n\"property21\" \r\t\n: \r\t\nnull \r\t\n, \r\t\n\"property22\" \r\t\n: \r\t\ntrue \r\t\n, \r\t\n\"property23\" \r\t\n: \r\t\nfalse \r\t\n, \r\t\n\"property24\" \r\t\n: \r\t\n-0.0 \r\t\n, \r\t\n\"property25\" \r\t\n: \r\t\n\"string\" \r\t\n} \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 4);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 9);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 14);
        check(data.path[1].end == 25);
        check(data.type == JSON_NULL);
        check(data.value.start == 35);
        check(data.value.end == 38);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 1);
        
        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 48);
        check(data.path[1].end == 59);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 69);
        check(data.value.end == 72);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 82);
        check(data.path[1].end == 93);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 103);
        check(data.value.end == 107);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 117);
        check(data.path[1].end == 128);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 138);
        check(data.value.end == 141);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 151);
        check(data.path[1].end == 162);
        check(data.type == JSON_STRING);
        check(data.value.start == 172);
        check(data.value.end == 179);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 184);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 194);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 199);
        check(data.path[1].end == 210);
        check(data.type == JSON_NULL);
        check(data.value.start == 220);
        check(data.value.end == 223);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 1);
        
        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 233);
        check(data.path[1].end == 244);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 254);
        check(data.value.end == 257);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 267);
        check(data.path[1].end == 278);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 288);
        check(data.value.end == 292);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 302);
        check(data.path[1].end == 313);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 323);
        check(data.value.end == 326);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.path[1].start == 336);
        check(data.path[1].end == 347);
        check(data.type == JSON_STRING);
        check(data.value.start == 357);
        check(data.value.end == 364);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 4);
        check(data.path[0].end == 4);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 369);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[[null,true,false,-0.0,\"string\"],{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"}]";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 1);
        check(data.path[1].end == 1);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 1);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 1);
        check(data.path[1].end == 1);
        check(data.type == JSON_NULL);
        check(data.value.start == 2);
        check(data.value.end == 5);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 1);
        check(data.path[1].end == 1);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 7);
        check(data.value.end == 10);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 1);
        check(data.path[1].end == 1);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 12);
        check(data.value.end == 16);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 1);
        check(data.path[1].end == 1);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 18);
        check(data.value.end == 21);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 1);
        check(data.path[1].end == 1);
        check(data.type == JSON_STRING);
        check(data.value.start == 23);
        check(data.value.end == 30);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 31);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 33);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 34);
        check(data.path[1].end == 45);
        check(data.type == JSON_NULL);
        check(data.value.start == 47);
        check(data.value.end == 50);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 1);
        
        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 52);
        check(data.path[1].end == 63);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 65);
        check(data.value.end == 68);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 70);
        check(data.path[1].end == 81);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 83);
        check(data.value.end == 87);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 89);
        check(data.path[1].end == 100);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 102);
        check(data.value.end == 105);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 107);
        check(data.path[1].end == 118);
        check(data.type == JSON_STRING);
        check(data.value.start == 120);
        check(data.value.end == 127);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 128);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"},[null,true,false,-0.0,\"string\"]]";

        int result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 0);
        check(data.item_counter[0] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_OBJECT_START);
        check(data.value.start == 1);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 2);
        check(data.path[1].end == 13);
        check(data.type == JSON_NULL);
        check(data.value.start == 15);
        check(data.value.end == 18);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 1);
        
        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 20);
        check(data.path[1].end == 31);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 33);
        check(data.value.end == 36);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 38);
        check(data.path[1].end == 49);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 51);
        check(data.value.end == 55);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 57);
        check(data.path[1].end == 68);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 70);
        check(data.value.end == 73);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 75);
        check(data.path[1].end == 86);
        check(data.type == JSON_STRING);
        check(data.value.start == 88);
        check(data.value.end == 95);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.type == JSON_OBJECT_END);
        check(data.value.end == 96);
        check(data.item_counter[0] == 1);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 98);
        check(data.path[1].end == 98);
        check(data.type == JSON_ARRAY_START);
        check(data.value.start == 98);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 0);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 98);
        check(data.path[1].end == 98);
        check(data.type == JSON_NULL);
        check(data.value.start == 99);
        check(data.value.end == 102);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 1);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 98);
        check(data.path[1].end == 98);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 104);
        check(data.value.end == 107);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 2);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 98);
        check(data.path[1].end == 98);
        check(data.type == JSON_BOOLEAN);
        check(data.value.start == 109);
        check(data.value.end == 113);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 3);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 98);
        check(data.path[1].end == 98);
        check(data.type == JSON_NUMBER);
        check(data.value.start == 115);
        check(data.value.end == 118);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 4);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 1);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 98);
        check(data.path[1].end == 98);
        check(data.type == JSON_STRING);
        check(data.value.start == 120);
        check(data.value.end == 127);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_ITEM);
        check(data.depth == 0);
        check(data.path[0].start == 0);
        check(data.path[0].end == 0);
        check(data.path[1].start == 98);
        check(data.path[1].end == 98);
        check(data.type == JSON_ARRAY_END);
        check(data.value.end == 128);
        check(data.item_counter[0] == 2);
        check(data.item_counter[1] == 5);

        result = parse_json(json, sizeof(json), &data);

        check(result == JSON_RESULT_VALID);
        check(data.item_counter[0] == 2);
    }

    return 0;
}

int main( void ) {
    static struct test const tests[] = {
        { test_primitive_object, "Object with primitive values" },
        { test_primitive_array, "Array with primitive values" },
        { test_complex_object, "Objects with complex values" },
        { test_complex_array, "Arrays with complex values" }
    };

    return test_suit(tests, sizeof tests / sizeof *tests);
}