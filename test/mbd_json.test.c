#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unity.h>

#include "mbd_json.h"

//#define done() return 0

//static int check_count = 0;
//#define TEST_ASSERT_EQUAL(x) check_count++; if (!(x)) { return __LINE__; }

//struct test {
//    int(*func)(void);
//    char const* name;
//};
//
//static int test_suit(struct test const* tests, int test_count) {
//    printf("\n\nTests:\n");
//    int failed = 0;
//
//    for (int i = 0; i < test_count; i++) {
//        printf("\n %02d: %-25s", i, tests[i].name);
//        int error_line = tests[i].func();
//
//        if (error_line > 0) {
//            printf("... failed at line: %i\n", error_line);
//            failed++;
//        }
//        else {
//            printf("... ok\n");
//        }
//    }
//
//    printf("\n%s%d\n", "Total checks: ", check_count);
//    printf("Tests passing: %i/%i\r\n\n\n", test_count - failed, test_count);
//
//    return failed;
//}

void test_primitive_object(void) {
    json_data_t data;
    data.pos = 0;
    data.max_depth = 5;

    json_parsing_t parsing[5];
    data.parsing = parsing;

    json_start_end_t path[5];
    data.path = path;

    uint8_t value_counter[5];
    data.value_counter = value_counter;

    {
        reset_json_data(&data);
        const uint8_t json[] = "{}";
        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n} \r\t\n";
        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":null}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 13);
        TEST_ASSERT_EQUAL(data.value.end, 16);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\nnull \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 29);
        TEST_ASSERT_EQUAL(data.value.end, 32);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":true}";
        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 13);
        TEST_ASSERT_EQUAL(data.value.end, 16);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\ntrue \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 29);
        TEST_ASSERT_EQUAL(data.value.end, 32);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":false}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 13);
        TEST_ASSERT_EQUAL(data.value.end, 17);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\nfalse \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 29);
        TEST_ASSERT_EQUAL(data.value.end, 33);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":0}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 13);
        TEST_ASSERT_EQUAL(data.value.end, 13);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":0.0}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 13);
        TEST_ASSERT_EQUAL(data.value.end, 15);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":-0.0}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 13);
        TEST_ASSERT_EQUAL(data.value.end, 16);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n-0.0 \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 29);
        TEST_ASSERT_EQUAL(data.value.end, 32);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
         const uint8_t json[] = "{\"property1\":\"string\"}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 13);
        TEST_ASSERT_EQUAL(data.value.end, 20);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n\"string\" \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 29);
        TEST_ASSERT_EQUAL(data.value.end, 36);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":null,\"property2\":true,\"property3\":false,\"property4\":-0.0,\"property5\":\"string\"}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 13);
        TEST_ASSERT_EQUAL(data.value.end, 16);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 18);
        TEST_ASSERT_EQUAL(data.path[0].end, 28);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 30);
        TEST_ASSERT_EQUAL(data.value.end, 33);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 35);
        TEST_ASSERT_EQUAL(data.path[0].end, 45);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 47);
        TEST_ASSERT_EQUAL(data.value.end, 51);
        TEST_ASSERT_EQUAL(data.value_counter[0], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 53);
        TEST_ASSERT_EQUAL(data.path[0].end, 63);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 65);
        TEST_ASSERT_EQUAL(data.value.end, 68);
        TEST_ASSERT_EQUAL(data.value_counter[0], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 70);
        TEST_ASSERT_EQUAL(data.path[0].end, 80);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 82);
        TEST_ASSERT_EQUAL(data.value.end, 89);
        TEST_ASSERT_EQUAL(data.value_counter[0], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 5);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\nnull \r\t\n, \r\t\n\"property2\" \r\t\n: \r\t\ntrue \r\t\n, \r\t\n\"property3\" \r\t\n: \r\t\nfalse \r\t\n, \r\t\n\"property4\" \r\t\n: \r\t\n-0.0 \r\t\n, \r\t\n\"property5\" \r\t\n: \r\t\n\"string\" \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 29);
        TEST_ASSERT_EQUAL(data.value.end, 32);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 42);
        TEST_ASSERT_EQUAL(data.path[0].end, 52);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 62);
        TEST_ASSERT_EQUAL(data.value.end, 65);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 75);
        TEST_ASSERT_EQUAL(data.path[0].end, 85);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 95);
        TEST_ASSERT_EQUAL(data.value.end, 99);
        TEST_ASSERT_EQUAL(data.value_counter[0], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 109);
        TEST_ASSERT_EQUAL(data.path[0].end, 119);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 129);
        TEST_ASSERT_EQUAL(data.value.end, 132);
        TEST_ASSERT_EQUAL(data.value_counter[0], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 142);
        TEST_ASSERT_EQUAL(data.path[0].end, 152);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 162);
        TEST_ASSERT_EQUAL(data.value.end, 169);
        TEST_ASSERT_EQUAL(data.value_counter[0], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 5);
    }

}

 void test_primitive_array(void) {
    json_data_t data;
    data.pos = 0;
    data.max_depth = 5;

    json_parsing_t parsing[5];
    data.parsing = parsing;

    json_start_end_t path[5];
    data.path = path;

    uint8_t value_counter[5];
    data.value_counter = value_counter;

    {
        reset_json_data(&data);
        const uint8_t json[] = "[]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[null]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 1);
        TEST_ASSERT_EQUAL(data.value.end, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\nnull \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 9);
        TEST_ASSERT_EQUAL(data.value.end, 12);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[true]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 1);
        TEST_ASSERT_EQUAL(data.value.end, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\ntrue \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 9);
        TEST_ASSERT_EQUAL(data.value.end, 12);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[false]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 1);
        TEST_ASSERT_EQUAL(data.value.end, 5);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\nfalse \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 9);
        TEST_ASSERT_EQUAL(data.value.end, 13);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[-0.0]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 1);
        TEST_ASSERT_EQUAL(data.value.end, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n-0.0 \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 9);
        TEST_ASSERT_EQUAL(data.value.end, 12);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[\"string\"]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 1);
        TEST_ASSERT_EQUAL(data.value.end, 8);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n\"string\" \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 9);
        TEST_ASSERT_EQUAL(data.value.end, 16);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[null,true,false,-0.0,\"string\"]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 1);
        TEST_ASSERT_EQUAL(data.value.end, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 6);
        TEST_ASSERT_EQUAL(data.value.end, 9);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 11);
        TEST_ASSERT_EQUAL(data.value.end, 15);
        TEST_ASSERT_EQUAL(data.value_counter[0], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 17);
        TEST_ASSERT_EQUAL(data.value.end, 20);
        TEST_ASSERT_EQUAL(data.value_counter[0], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 22);
        TEST_ASSERT_EQUAL(data.value.end, 29);
        TEST_ASSERT_EQUAL(data.value_counter[0], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\nnull \r\t\n, \r\t\ntrue \r\t\n, \r\t\nfalse \r\t\n, \r\t\n-0.0 \r\t\n, \r\t\n\"string\" \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 4 );
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 9);
        TEST_ASSERT_EQUAL(data.value.end, 12);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 22);
        TEST_ASSERT_EQUAL(data.value.end, 25);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 35);
        TEST_ASSERT_EQUAL(data.value.end, 39);
        TEST_ASSERT_EQUAL(data.value_counter[0], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 49);
        TEST_ASSERT_EQUAL(data.value.end, 52);
        TEST_ASSERT_EQUAL(data.value_counter[0], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 62);
        TEST_ASSERT_EQUAL(data.value.end, 69);
        TEST_ASSERT_EQUAL(data.value_counter[0], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 5);
    }

}

void test_complex_object(void) {
    json_data_t data;
    data.pos = 0;
    data.max_depth = 5;

    json_parsing_t parsing[5];
    data.parsing = parsing;

    json_start_end_t path[5];
    data.path = path;

    uint8_t value_counter[5];
    data.value_counter = value_counter;

    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":{}}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 13);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 14);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n{ \r\t\n} \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 29);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 34);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"}}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 13);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 14);
        TEST_ASSERT_EQUAL(data.path[1].end, 25);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 27);
        TEST_ASSERT_EQUAL(data.value.end, 30);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 32);
        TEST_ASSERT_EQUAL(data.path[1].end, 43);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 45);
        TEST_ASSERT_EQUAL(data.value.end, 48);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 50);
        TEST_ASSERT_EQUAL(data.path[1].end, 61);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 63);
        TEST_ASSERT_EQUAL(data.value.end, 67);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 69);
        TEST_ASSERT_EQUAL(data.path[1].end, 80);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 82);
        TEST_ASSERT_EQUAL(data.value.end, 85);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 87);
        TEST_ASSERT_EQUAL(data.path[1].end, 98);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 100);
        TEST_ASSERT_EQUAL(data.value.end, 107);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 108);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n{ \r\t\n\"property11\" \r\t\n: \r\t\nnull \r\t\n, \r\t\n\"property12\" \r\t\n: \r\t\ntrue \r\t\n, \r\t\n\"property13\" \r\t\n: \r\t\nfalse \r\t\n, \r\t\n\"property14\" \r\t\n: \r\t\n-0.0 \r\t\n, \r\t\n\"property15\" \r\t\n: \r\t\n\"string\" \r\t\n} \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 29);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 34);
        TEST_ASSERT_EQUAL(data.path[1].end, 45);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 55);
        TEST_ASSERT_EQUAL(data.value.end, 58);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 68);
        TEST_ASSERT_EQUAL(data.path[1].end, 79);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 89);
        TEST_ASSERT_EQUAL(data.value.end, 92);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 102);
        TEST_ASSERT_EQUAL(data.path[1].end, 113);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 123);
        TEST_ASSERT_EQUAL(data.value.end, 127);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 137);
        TEST_ASSERT_EQUAL(data.path[1].end, 148);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 158);
        TEST_ASSERT_EQUAL(data.value.end, 161);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 171);
        TEST_ASSERT_EQUAL(data.path[1].end, 182);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 192);
        TEST_ASSERT_EQUAL(data.value.end, 199);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 204);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":{},\"property2\":{}}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 13);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 14);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 16);
        TEST_ASSERT_EQUAL(data.path[0].end, 26);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 28);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 16);
        TEST_ASSERT_EQUAL(data.path[0].end, 26);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 29);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n{ \r\t\n} \r\t\n, \r\t\n\"property2\" \r\t\n: \r\t\n{ \r\t\n} \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 29);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 34);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 44);
        TEST_ASSERT_EQUAL(data.path[0].end, 54);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 64);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 44);
        TEST_ASSERT_EQUAL(data.path[0].end, 54);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 69);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"},\"property2\":{\"property21\":null,\"property22\":true,\"property23\":false,\"property24\":-0.0,\"property25\":\"string\"}}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 13);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 14);
        TEST_ASSERT_EQUAL(data.path[1].end, 25);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 27);
        TEST_ASSERT_EQUAL(data.value.end, 30);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 32);
        TEST_ASSERT_EQUAL(data.path[1].end, 43);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 45);
        TEST_ASSERT_EQUAL(data.value.end, 48);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 50);
        TEST_ASSERT_EQUAL(data.path[1].end, 61);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 63);
        TEST_ASSERT_EQUAL(data.value.end, 67);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 69);
        TEST_ASSERT_EQUAL(data.path[1].end, 80);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 82);
        TEST_ASSERT_EQUAL(data.value.end, 85);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 87);
        TEST_ASSERT_EQUAL(data.path[1].end, 98);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 100);
        TEST_ASSERT_EQUAL(data.value.end, 107);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 108);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 110);
        TEST_ASSERT_EQUAL(data.path[0].end, 120);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 122);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 110);
        TEST_ASSERT_EQUAL(data.path[0].end, 120);
        TEST_ASSERT_EQUAL(data.path[1].start, 123);
        TEST_ASSERT_EQUAL(data.path[1].end, 134);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 136);
        TEST_ASSERT_EQUAL(data.value.end, 139);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 110);
        TEST_ASSERT_EQUAL(data.path[0].end, 120);
        TEST_ASSERT_EQUAL(data.path[1].start, 141);
        TEST_ASSERT_EQUAL(data.path[1].end, 152);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 154);
        TEST_ASSERT_EQUAL(data.value.end, 157);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 110);
        TEST_ASSERT_EQUAL(data.path[0].end, 120);
        TEST_ASSERT_EQUAL(data.path[1].start, 159);
        TEST_ASSERT_EQUAL(data.path[1].end, 170);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 172);
        TEST_ASSERT_EQUAL(data.value.end, 176);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 110);
        TEST_ASSERT_EQUAL(data.path[0].end, 120);
        TEST_ASSERT_EQUAL(data.path[1].start, 178);
        TEST_ASSERT_EQUAL(data.path[1].end, 189);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 191);
        TEST_ASSERT_EQUAL(data.value.end, 194);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 110);
        TEST_ASSERT_EQUAL(data.path[0].end, 120);
        TEST_ASSERT_EQUAL(data.path[1].start, 196);
        TEST_ASSERT_EQUAL(data.path[1].end, 207);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 209);
        TEST_ASSERT_EQUAL(data.value.end, 216);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 110);
        TEST_ASSERT_EQUAL(data.path[0].end, 120);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 217);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n{ \r\t\n\"property11\" \r\t\n: \r\t\nnull \r\t\n, \r\t\n\"property12\" \r\t\n: \r\t\ntrue \r\t\n, \r\t\n\"property13\" \r\t\n: \r\t\nfalse \r\t\n, \r\t\n\"property14\" \r\t\n: \r\t\n-0.0 \r\t\n, \r\t\n\"property15\" \r\t\n: \r\t\n\"string\" \r\t\n}, \r\t\n\"property2\" \r\t\n: \r\t\n{ \r\t\n\"property21\" \r\t\n: \r\t\nnull \r\t\n, \r\t\n\"property22\" \r\t\n: \r\t\ntrue \r\t\n, \r\t\n\"property23\" \r\t\n: \r\t\nfalse \r\t\n, \r\t\n\"property24\" \r\t\n: \r\t\n-0.0 \r\t\n, \r\t\n\"property25\" \r\t\n: \r\t\n\"string\" \r\t\n} \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 29);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 34);
        TEST_ASSERT_EQUAL(data.path[1].end, 45);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 55);
        TEST_ASSERT_EQUAL(data.value.end, 58);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 68);
        TEST_ASSERT_EQUAL(data.path[1].end, 79);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 89);
        TEST_ASSERT_EQUAL(data.value.end, 92);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 102);
        TEST_ASSERT_EQUAL(data.path[1].end, 113);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 123);
        TEST_ASSERT_EQUAL(data.value.end, 127);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 137);
        TEST_ASSERT_EQUAL(data.path[1].end, 148);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 158);
        TEST_ASSERT_EQUAL(data.value.end, 161);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 171);
        TEST_ASSERT_EQUAL(data.path[1].end, 182);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 192);
        TEST_ASSERT_EQUAL(data.value.end, 199);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 204);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 210);
        TEST_ASSERT_EQUAL(data.path[0].end, 220);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 230);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 210);
        TEST_ASSERT_EQUAL(data.path[0].end, 220);
        TEST_ASSERT_EQUAL(data.path[1].start, 235);
        TEST_ASSERT_EQUAL(data.path[1].end, 246);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 256);
        TEST_ASSERT_EQUAL(data.value.end, 259);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 210);
        TEST_ASSERT_EQUAL(data.path[0].end, 220);
        TEST_ASSERT_EQUAL(data.path[1].start, 269);
        TEST_ASSERT_EQUAL(data.path[1].end, 280);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 290);
        TEST_ASSERT_EQUAL(data.value.end, 293);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 210);
        TEST_ASSERT_EQUAL(data.path[0].end, 220);
        TEST_ASSERT_EQUAL(data.path[1].start, 303);
        TEST_ASSERT_EQUAL(data.path[1].end, 314);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 324);
        TEST_ASSERT_EQUAL(data.value.end, 328);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 210);
        TEST_ASSERT_EQUAL(data.path[0].end, 220);
        TEST_ASSERT_EQUAL(data.path[1].start, 338);
        TEST_ASSERT_EQUAL(data.path[1].end, 349);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 359);
        TEST_ASSERT_EQUAL(data.value.end, 362);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 210);
        TEST_ASSERT_EQUAL(data.path[0].end, 220);
        TEST_ASSERT_EQUAL(data.path[1].start, 372);
        TEST_ASSERT_EQUAL(data.path[1].end, 383);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 393);
        TEST_ASSERT_EQUAL(data.value.end, 400);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 210);
        TEST_ASSERT_EQUAL(data.path[0].end, 220);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 405);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":[]}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 13);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 14);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n[ \r\t\n] \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 29);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 29);
        TEST_ASSERT_EQUAL(data.path[1].end, 29);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 34);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":[null,true,false,-0.0,\"string\"]}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 13);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 14);
        TEST_ASSERT_EQUAL(data.value.end, 17);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 19);
        TEST_ASSERT_EQUAL(data.value.end, 22);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 24);
        TEST_ASSERT_EQUAL(data.value.end, 28);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 30);
        TEST_ASSERT_EQUAL(data.value.end, 33);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 35);
        TEST_ASSERT_EQUAL(data.value.end, 42);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 43);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n[ \r\t\nnull \r\t\n, \r\t\ntrue \r\t\n, \r\t\nfalse \r\t\n, \r\t\n-0.0 \r\t\n, \r\t\n\"string\" \r\t\n] \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 29);
        TEST_ASSERT_EQUAL(data.path[1].end, 29);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 29);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 29);
        TEST_ASSERT_EQUAL(data.path[1].end, 29);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 34);
        TEST_ASSERT_EQUAL(data.value.end, 37);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 29);
        TEST_ASSERT_EQUAL(data.path[1].end, 29);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 47);
        TEST_ASSERT_EQUAL(data.value.end, 50);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 29);
        TEST_ASSERT_EQUAL(data.path[1].end, 29);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 60);
        TEST_ASSERT_EQUAL(data.value.end, 64);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 29);
        TEST_ASSERT_EQUAL(data.path[1].end, 29);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 74);
        TEST_ASSERT_EQUAL(data.value.end, 77);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 29);
        TEST_ASSERT_EQUAL(data.path[1].end, 29);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 87);
        TEST_ASSERT_EQUAL(data.value.end, 94);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 29);
        TEST_ASSERT_EQUAL(data.path[1].end, 29);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 99);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":[],\"property2\":[]}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 13);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 14);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 16);
        TEST_ASSERT_EQUAL(data.path[0].end, 26);
        TEST_ASSERT_EQUAL(data.path[1].start, 28);
        TEST_ASSERT_EQUAL(data.path[1].end, 28);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 28);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 16);
        TEST_ASSERT_EQUAL(data.path[0].end, 26);
        TEST_ASSERT_EQUAL(data.path[1].start, 28);
        TEST_ASSERT_EQUAL(data.path[1].end, 28);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 29);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n[ \r\t\n] \r\t\n, \r\t\n\"property2\" \r\t\n: \r\t\n[ \r\t\n] \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 29);
        TEST_ASSERT_EQUAL(data.path[1].end, 29);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 29);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 29);
        TEST_ASSERT_EQUAL(data.path[1].end, 29);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 34);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 44);
        TEST_ASSERT_EQUAL(data.path[0].end, 54);
        TEST_ASSERT_EQUAL(data.path[1].start, 64);
        TEST_ASSERT_EQUAL(data.path[1].end, 64);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 64);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 44);
        TEST_ASSERT_EQUAL(data.path[0].end, 54);
        TEST_ASSERT_EQUAL(data.path[1].start, 64);
        TEST_ASSERT_EQUAL(data.path[1].end, 64);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 69);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":[null,true,false,-0.0,\"string\"],\"property2\":[null,true,false,-0.0,\"string\"]}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 13);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 14);
        TEST_ASSERT_EQUAL(data.value.end, 17);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 19);
        TEST_ASSERT_EQUAL(data.value.end, 22);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 24);
        TEST_ASSERT_EQUAL(data.value.end, 28);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 30);
        TEST_ASSERT_EQUAL(data.value.end, 33);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 35);
        TEST_ASSERT_EQUAL(data.value.end, 42);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 43);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 45);
        TEST_ASSERT_EQUAL(data.path[0].end, 55);
        TEST_ASSERT_EQUAL(data.path[1].start, 57);
        TEST_ASSERT_EQUAL(data.path[1].end, 57);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 57);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 45);
        TEST_ASSERT_EQUAL(data.path[0].end, 55);
        TEST_ASSERT_EQUAL(data.path[1].start, 57);
        TEST_ASSERT_EQUAL(data.path[1].end, 57);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 58);
        TEST_ASSERT_EQUAL(data.value.end, 61);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 45);
        TEST_ASSERT_EQUAL(data.path[0].end, 55);
        TEST_ASSERT_EQUAL(data.path[1].start, 57);
        TEST_ASSERT_EQUAL(data.path[1].end, 57);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 63);
        TEST_ASSERT_EQUAL(data.value.end, 66);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 45);
        TEST_ASSERT_EQUAL(data.path[0].end, 55);
        TEST_ASSERT_EQUAL(data.path[1].start, 57);
        TEST_ASSERT_EQUAL(data.path[1].end, 57);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 68);
        TEST_ASSERT_EQUAL(data.value.end, 72);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 45);
        TEST_ASSERT_EQUAL(data.path[0].end, 55);
        TEST_ASSERT_EQUAL(data.path[1].start, 57);
        TEST_ASSERT_EQUAL(data.path[1].end, 57);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 74);
        TEST_ASSERT_EQUAL(data.value.end, 77);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 45);
        TEST_ASSERT_EQUAL(data.path[0].end, 55);
        TEST_ASSERT_EQUAL(data.path[1].start, 57);
        TEST_ASSERT_EQUAL(data.path[1].end, 57);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 79);
        TEST_ASSERT_EQUAL(data.value.end, 86);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 45);
        TEST_ASSERT_EQUAL(data.path[0].end, 55);
        TEST_ASSERT_EQUAL(data.path[1].start, 57);
        TEST_ASSERT_EQUAL(data.path[1].end, 57);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 87);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n[ \r\t\nnull \r\t\n, \r\t\ntrue \r\t\n, \r\t\nfalse \r\t\n, \r\t\n-0.0 \r\t\n, \r\t\n\"string\" \r\t\n] \r\t\n, \r\t\n\"property2\" \r\t\n: \r\t\n[ \r\t\nnull \r\t\n, \r\t\ntrue \r\t\n, \r\t\nfalse \r\t\n, \r\t\n-0.0 \r\t\n, \r\t\n\"string\" \r\t\n] \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 29);
        TEST_ASSERT_EQUAL(data.path[1].end, 29);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 29);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 29);
        TEST_ASSERT_EQUAL(data.path[1].end, 29);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 34);
        TEST_ASSERT_EQUAL(data.value.end, 37);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 29);
        TEST_ASSERT_EQUAL(data.path[1].end, 29);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 47);
        TEST_ASSERT_EQUAL(data.value.end, 50);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 29);
        TEST_ASSERT_EQUAL(data.path[1].end, 29);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 60);
        TEST_ASSERT_EQUAL(data.value.end, 64);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 29);
        TEST_ASSERT_EQUAL(data.path[1].end, 29);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 74);
        TEST_ASSERT_EQUAL(data.value.end, 77);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 29);
        TEST_ASSERT_EQUAL(data.path[1].end, 29);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 87);
        TEST_ASSERT_EQUAL(data.value.end, 94);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 9);
        TEST_ASSERT_EQUAL(data.path[0].end, 19);
        TEST_ASSERT_EQUAL(data.path[1].start, 29);
        TEST_ASSERT_EQUAL(data.path[1].end, 29);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 99);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 109);
        TEST_ASSERT_EQUAL(data.path[0].end, 119);
        TEST_ASSERT_EQUAL(data.path[1].start, 129);
        TEST_ASSERT_EQUAL(data.path[1].end, 129);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 129);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 109);
        TEST_ASSERT_EQUAL(data.path[0].end, 119);
        TEST_ASSERT_EQUAL(data.path[1].start, 129);
        TEST_ASSERT_EQUAL(data.path[1].end, 129);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 134);
        TEST_ASSERT_EQUAL(data.value.end, 137);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 109);
        TEST_ASSERT_EQUAL(data.path[0].end, 119);
        TEST_ASSERT_EQUAL(data.path[1].start, 129);
        TEST_ASSERT_EQUAL(data.path[1].end, 129);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 147);
        TEST_ASSERT_EQUAL(data.value.end, 150);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 109);
        TEST_ASSERT_EQUAL(data.path[0].end, 119);
        TEST_ASSERT_EQUAL(data.path[1].start, 129);
        TEST_ASSERT_EQUAL(data.path[1].end, 129);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 160);
        TEST_ASSERT_EQUAL(data.value.end, 164);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 109);
        TEST_ASSERT_EQUAL(data.path[0].end, 119);
        TEST_ASSERT_EQUAL(data.path[1].start, 129);
        TEST_ASSERT_EQUAL(data.path[1].end, 129);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 174);
        TEST_ASSERT_EQUAL(data.value.end, 177);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 109);
        TEST_ASSERT_EQUAL(data.path[0].end, 119);
        TEST_ASSERT_EQUAL(data.path[1].start, 129);
        TEST_ASSERT_EQUAL(data.path[1].end, 129);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 187);
        TEST_ASSERT_EQUAL(data.value.end, 194);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 109);
        TEST_ASSERT_EQUAL(data.path[0].end, 119);
        TEST_ASSERT_EQUAL(data.path[1].start, 129);
        TEST_ASSERT_EQUAL(data.path[1].end, 129);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 199);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"},\"property2\":[null,true,false,-0.0,\"string\"]}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 13);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 14);
        TEST_ASSERT_EQUAL(data.path[1].end, 25);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 27);
        TEST_ASSERT_EQUAL(data.value.end, 30);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 32);
        TEST_ASSERT_EQUAL(data.path[1].end, 43);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 45);
        TEST_ASSERT_EQUAL(data.value.end, 48);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 50);
        TEST_ASSERT_EQUAL(data.path[1].end, 61);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 63);
        TEST_ASSERT_EQUAL(data.value.end, 67);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 69);
        TEST_ASSERT_EQUAL(data.path[1].end, 80);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 82);
        TEST_ASSERT_EQUAL(data.value.end, 85);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 87);
        TEST_ASSERT_EQUAL(data.path[1].end, 98);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 100);
        TEST_ASSERT_EQUAL(data.value.end, 107);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 108);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 110);
        TEST_ASSERT_EQUAL(data.path[0].end, 120);
        TEST_ASSERT_EQUAL(data.path[1].start, 122);
        TEST_ASSERT_EQUAL(data.path[1].end, 122);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 122);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 110);
        TEST_ASSERT_EQUAL(data.path[0].end, 120);
        TEST_ASSERT_EQUAL(data.path[1].start, 122);
        TEST_ASSERT_EQUAL(data.path[1].end, 122);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 123);
        TEST_ASSERT_EQUAL(data.value.end, 126);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 110);
        TEST_ASSERT_EQUAL(data.path[0].end, 120);
        TEST_ASSERT_EQUAL(data.path[1].start, 122);
        TEST_ASSERT_EQUAL(data.path[1].end, 122);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 128);
        TEST_ASSERT_EQUAL(data.value.end, 131);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 110);
        TEST_ASSERT_EQUAL(data.path[0].end, 120);
        TEST_ASSERT_EQUAL(data.path[1].start, 122);
        TEST_ASSERT_EQUAL(data.path[1].end, 122);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 133);
        TEST_ASSERT_EQUAL(data.value.end, 137);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 110);
        TEST_ASSERT_EQUAL(data.path[0].end, 120);
        TEST_ASSERT_EQUAL(data.path[1].start, 122);
        TEST_ASSERT_EQUAL(data.path[1].end, 122);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 139);
        TEST_ASSERT_EQUAL(data.value.end, 142);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 110);
        TEST_ASSERT_EQUAL(data.path[0].end, 120);
        TEST_ASSERT_EQUAL(data.path[1].start, 122);
        TEST_ASSERT_EQUAL(data.path[1].end, 122);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 144);
        TEST_ASSERT_EQUAL(data.value.end, 151);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 110);
        TEST_ASSERT_EQUAL(data.path[0].end, 120);
        TEST_ASSERT_EQUAL(data.path[1].start, 122);
        TEST_ASSERT_EQUAL(data.path[1].end, 122);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 152);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":[null,true,false,-0.0,\"string\"],\"property2\":{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"}}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 13);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 14);
        TEST_ASSERT_EQUAL(data.value.end, 17);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 19);
        TEST_ASSERT_EQUAL(data.value.end, 22);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 24);
        TEST_ASSERT_EQUAL(data.value.end, 28);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 30);
        TEST_ASSERT_EQUAL(data.value.end, 33);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 35);
        TEST_ASSERT_EQUAL(data.value.end, 42);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 1);
        TEST_ASSERT_EQUAL(data.path[0].end, 11);
        TEST_ASSERT_EQUAL(data.path[1].start, 13);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 43);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 45);
        TEST_ASSERT_EQUAL(data.path[0].end, 55);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 57);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 45);
        TEST_ASSERT_EQUAL(data.path[0].end, 55);
        TEST_ASSERT_EQUAL(data.path[1].start, 58);
        TEST_ASSERT_EQUAL(data.path[1].end, 69);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 71);
        TEST_ASSERT_EQUAL(data.value.end, 74);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 45);
        TEST_ASSERT_EQUAL(data.path[0].end, 55);
        TEST_ASSERT_EQUAL(data.path[1].start, 76);
        TEST_ASSERT_EQUAL(data.path[1].end, 87);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 89);
        TEST_ASSERT_EQUAL(data.value.end, 92);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 45);
        TEST_ASSERT_EQUAL(data.path[0].end, 55);
        TEST_ASSERT_EQUAL(data.path[1].start, 94);
        TEST_ASSERT_EQUAL(data.path[1].end, 105);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 107);
        TEST_ASSERT_EQUAL(data.value.end, 111);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 45);
        TEST_ASSERT_EQUAL(data.path[0].end, 55);
        TEST_ASSERT_EQUAL(data.path[1].start, 113);
        TEST_ASSERT_EQUAL(data.path[1].end, 124);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 126);
        TEST_ASSERT_EQUAL(data.value.end, 129);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 45);
        TEST_ASSERT_EQUAL(data.path[0].end, 55);
        TEST_ASSERT_EQUAL(data.path[1].start, 131);
        TEST_ASSERT_EQUAL(data.path[1].end, 142);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 144);
        TEST_ASSERT_EQUAL(data.value.end, 151);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 45);
        TEST_ASSERT_EQUAL(data.path[0].end, 55);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 152);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
    }


}

void test_complex_array(void) {
    json_data_t data;
    data.pos = 0;
    data.max_depth = 5;

    json_parsing_t parsing[5];
    data.parsing = parsing;

    json_start_end_t path[5];
    data.path = path;

    uint8_t value_counter[5];
    data.value_counter = value_counter;

    {
        reset_json_data(&data);
        const uint8_t json[] = "[[]]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 1);
        TEST_ASSERT_EQUAL(data.path[1].end, 1);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 1);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 2);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n[ \r\t\n] \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 9);
        TEST_ASSERT_EQUAL(data.path[1].end, 9);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 9);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 14);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[[null,true,false,-0.0,\"string\"]]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 1);
        TEST_ASSERT_EQUAL(data.path[1].end, 1);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 1);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 1);
        TEST_ASSERT_EQUAL(data.path[1].end, 1);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 2);
        TEST_ASSERT_EQUAL(data.value.end, 5);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 1);
        TEST_ASSERT_EQUAL(data.path[1].end, 1);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 7);
        TEST_ASSERT_EQUAL(data.value.end, 10);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 1);
        TEST_ASSERT_EQUAL(data.path[1].end, 1);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 12);
        TEST_ASSERT_EQUAL(data.value.end, 16);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 1);
        TEST_ASSERT_EQUAL(data.path[1].end, 1);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 18);
        TEST_ASSERT_EQUAL(data.value.end, 21);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 1);
        TEST_ASSERT_EQUAL(data.path[1].end, 1);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 23);
        TEST_ASSERT_EQUAL(data.value.end, 30);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 31);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n[ \r\t\nnull \r\t\n, \r\t\ntrue \r\t\n, \r\t\nfalse \r\t\n, \r\t\n-0.0 \r\t\n, \r\t\n\"string\" \r\t\n] \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 9);
        TEST_ASSERT_EQUAL(data.path[1].end, 9);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 9);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 9);
        TEST_ASSERT_EQUAL(data.path[1].end, 9);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 14);
        TEST_ASSERT_EQUAL(data.value.end, 17);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 9);
        TEST_ASSERT_EQUAL(data.path[1].end, 9);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 27);
        TEST_ASSERT_EQUAL(data.value.end, 30);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 9);
        TEST_ASSERT_EQUAL(data.path[1].end, 9);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 40);
        TEST_ASSERT_EQUAL(data.value.end, 44);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 9);
        TEST_ASSERT_EQUAL(data.path[1].end, 9);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 54);
        TEST_ASSERT_EQUAL(data.value.end, 57);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 9);
        TEST_ASSERT_EQUAL(data.path[1].end, 9);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 67);
        TEST_ASSERT_EQUAL(data.value.end, 74);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 79);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[[],[]]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 1);
        TEST_ASSERT_EQUAL(data.path[1].end, 1);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 1);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 2);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 4);
        TEST_ASSERT_EQUAL(data.path[1].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 5);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n[ \r\t\n] \r\t\n, \r\t\n[ \r\t\n] \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 9);
        TEST_ASSERT_EQUAL(data.path[1].end, 9);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 9);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 14);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 24);
        TEST_ASSERT_EQUAL(data.path[1].end, 24);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 24);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 29);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[[null,true,false,-0.0,\"string\"],[null,true,false,-0.0,\"string\"]]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 1);
        TEST_ASSERT_EQUAL(data.path[1].end, 1);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 1);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 1);
        TEST_ASSERT_EQUAL(data.path[1].end, 1);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 2);
        TEST_ASSERT_EQUAL(data.value.end, 5);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 1);
        TEST_ASSERT_EQUAL(data.path[1].end, 1);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 7);
        TEST_ASSERT_EQUAL(data.value.end, 10);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 1);
        TEST_ASSERT_EQUAL(data.path[1].end, 1);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 12);
        TEST_ASSERT_EQUAL(data.value.end, 16);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 1);
        TEST_ASSERT_EQUAL(data.path[1].end, 1);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 18);
        TEST_ASSERT_EQUAL(data.value.end, 21);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 1);
        TEST_ASSERT_EQUAL(data.path[1].end, 1);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 23);
        TEST_ASSERT_EQUAL(data.value.end, 30);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 31);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 33);
        TEST_ASSERT_EQUAL(data.path[1].end, 33);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 33);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 33);
        TEST_ASSERT_EQUAL(data.path[1].end, 33);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 34);
        TEST_ASSERT_EQUAL(data.value.end, 37);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 33);
        TEST_ASSERT_EQUAL(data.path[1].end, 33);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 39);
        TEST_ASSERT_EQUAL(data.value.end, 42);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 33);
        TEST_ASSERT_EQUAL(data.path[1].end, 33);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 44);
        TEST_ASSERT_EQUAL(data.value.end, 48);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 33);
        TEST_ASSERT_EQUAL(data.path[1].end, 33);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 50);
        TEST_ASSERT_EQUAL(data.value.end, 53);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 33);
        TEST_ASSERT_EQUAL(data.path[1].end, 33);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 55);
        TEST_ASSERT_EQUAL(data.value.end, 62);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 63);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n[ \r\t\nnull \r\t\n, \r\t\ntrue \r\t\n, \r\t\nfalse \r\t\n, \r\t\n-0.0 \r\t\n, \r\t\n\"string\" \r\t\n] \r\t\n, \r\t\n[ \r\t\nnull \r\t\n, \r\t\ntrue \r\t\n, \r\t\nfalse \r\t\n, \r\t\n-0.0 \r\t\n, \r\t\n\"string\" \r\t\n] \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 9);
        TEST_ASSERT_EQUAL(data.path[1].end, 9);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 9);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 9);
        TEST_ASSERT_EQUAL(data.path[1].end, 9);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 14);
        TEST_ASSERT_EQUAL(data.value.end, 17);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 9);
        TEST_ASSERT_EQUAL(data.path[1].end, 9);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 27);
        TEST_ASSERT_EQUAL(data.value.end, 30);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 9);
        TEST_ASSERT_EQUAL(data.path[1].end, 9);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 40);
        TEST_ASSERT_EQUAL(data.value.end, 44);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 9);
        TEST_ASSERT_EQUAL(data.path[1].end, 9);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 54);
        TEST_ASSERT_EQUAL(data.value.end, 57);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 9);
        TEST_ASSERT_EQUAL(data.path[1].end, 9);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 67);
        TEST_ASSERT_EQUAL(data.value.end, 74);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 79);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 89);
        TEST_ASSERT_EQUAL(data.path[1].end, 89);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 89);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 89);
        TEST_ASSERT_EQUAL(data.path[1].end, 89);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 94);
        TEST_ASSERT_EQUAL(data.value.end, 97);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 89);
        TEST_ASSERT_EQUAL(data.path[1].end, 89);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 107);
        TEST_ASSERT_EQUAL(data.value.end, 110);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 89);
        TEST_ASSERT_EQUAL(data.path[1].end, 89);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 120);
        TEST_ASSERT_EQUAL(data.value.end, 124);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 89);
        TEST_ASSERT_EQUAL(data.path[1].end, 89);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 134);
        TEST_ASSERT_EQUAL(data.value.end, 137);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 89);
        TEST_ASSERT_EQUAL(data.path[1].end, 89);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 147);
        TEST_ASSERT_EQUAL(data.value.end, 154);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 159);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[{}]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 1);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 2);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n{ \r\t\n} \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 9);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 14);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"}]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 1);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 2);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 15);
        TEST_ASSERT_EQUAL(data.value.end, 18);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 20);
        TEST_ASSERT_EQUAL(data.path[1].end, 31);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 33);
        TEST_ASSERT_EQUAL(data.value.end, 36);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 38);
        TEST_ASSERT_EQUAL(data.path[1].end, 49);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 51);
        TEST_ASSERT_EQUAL(data.value.end, 55);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 57);
        TEST_ASSERT_EQUAL(data.path[1].end, 68);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 70);
        TEST_ASSERT_EQUAL(data.value.end, 73);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 75);
        TEST_ASSERT_EQUAL(data.path[1].end, 86);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 88);
        TEST_ASSERT_EQUAL(data.value.end, 95);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 96);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n{ \r\t\n\"property11\" \r\t\n: \r\t\nnull \r\t\n, \r\t\n\"property12\" \r\t\n: \r\t\ntrue \r\t\n, \r\t\n\"property13\" \r\t\n: \r\t\nfalse \r\t\n, \r\t\n\"property14\" \r\t\n: \r\t\n-0.0 \r\t\n, \r\t\n\"property15\" \r\t\n: \r\t\n\"string\" \r\t\n} \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 9);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 14);
        TEST_ASSERT_EQUAL(data.path[1].end, 25);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 35);
        TEST_ASSERT_EQUAL(data.value.end, 38);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 48);
        TEST_ASSERT_EQUAL(data.path[1].end, 59);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 69);
        TEST_ASSERT_EQUAL(data.value.end, 72);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 82);
        TEST_ASSERT_EQUAL(data.path[1].end, 93);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 103);
        TEST_ASSERT_EQUAL(data.value.end, 107);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 117);
        TEST_ASSERT_EQUAL(data.path[1].end, 128);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 138);
        TEST_ASSERT_EQUAL(data.value.end, 141);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 151);
        TEST_ASSERT_EQUAL(data.path[1].end, 162);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 172);
        TEST_ASSERT_EQUAL(data.value.end, 179);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 184);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[{},{}]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 1);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 2);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 5);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n{ \r\t\n} \r\t\n, \r\t\n{ \r\t\n} \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 9);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 14);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 24);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 29);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"},{\"property21\":null,\"property22\":true,\"property23\":false,\"property24\":-0.0,\"property25\":\"string\"}]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 1);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 2);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 15);
        TEST_ASSERT_EQUAL(data.value.end, 18);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 20);
        TEST_ASSERT_EQUAL(data.path[1].end, 31);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 33);
        TEST_ASSERT_EQUAL(data.value.end, 36);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 38);
        TEST_ASSERT_EQUAL(data.path[1].end, 49);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 51);
        TEST_ASSERT_EQUAL(data.value.end, 55);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 57);
        TEST_ASSERT_EQUAL(data.path[1].end, 68);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 70);
        TEST_ASSERT_EQUAL(data.value.end, 73);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 75);
        TEST_ASSERT_EQUAL(data.path[1].end, 86);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 88);
        TEST_ASSERT_EQUAL(data.value.end, 95);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 96);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 98);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 99);
        TEST_ASSERT_EQUAL(data.path[1].end, 110);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 112);
        TEST_ASSERT_EQUAL(data.value.end, 115);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 117);
        TEST_ASSERT_EQUAL(data.path[1].end, 128);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 130);
        TEST_ASSERT_EQUAL(data.value.end, 133);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 135);
        TEST_ASSERT_EQUAL(data.path[1].end, 146);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 148);
        TEST_ASSERT_EQUAL(data.value.end, 152);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 154);
        TEST_ASSERT_EQUAL(data.path[1].end, 165);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 167);
        TEST_ASSERT_EQUAL(data.value.end, 170);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 172);
        TEST_ASSERT_EQUAL(data.path[1].end, 183);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 185);
        TEST_ASSERT_EQUAL(data.value.end, 192);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 193);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n{ \r\t\n\"property11\" \r\t\n: \r\t\nnull \r\t\n, \r\t\n\"property12\" \r\t\n: \r\t\ntrue \r\t\n, \r\t\n\"property13\" \r\t\n: \r\t\nfalse \r\t\n, \r\t\n\"property14\" \r\t\n: \r\t\n-0.0 \r\t\n, \r\t\n\"property15\" \r\t\n: \r\t\n\"string\" \r\t\n} \r\t\n, \r\t\n{ \r\t\n\"property21\" \r\t\n: \r\t\nnull \r\t\n, \r\t\n\"property22\" \r\t\n: \r\t\ntrue \r\t\n, \r\t\n\"property23\" \r\t\n: \r\t\nfalse \r\t\n, \r\t\n\"property24\" \r\t\n: \r\t\n-0.0 \r\t\n, \r\t\n\"property25\" \r\t\n: \r\t\n\"string\" \r\t\n} \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 4);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 9);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 14);
        TEST_ASSERT_EQUAL(data.path[1].end, 25);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 35);
        TEST_ASSERT_EQUAL(data.value.end, 38);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 48);
        TEST_ASSERT_EQUAL(data.path[1].end, 59);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 69);
        TEST_ASSERT_EQUAL(data.value.end, 72);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 82);
        TEST_ASSERT_EQUAL(data.path[1].end, 93);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 103);
        TEST_ASSERT_EQUAL(data.value.end, 107);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 117);
        TEST_ASSERT_EQUAL(data.path[1].end, 128);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 138);
        TEST_ASSERT_EQUAL(data.value.end, 141);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 151);
        TEST_ASSERT_EQUAL(data.path[1].end, 162);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 172);
        TEST_ASSERT_EQUAL(data.value.end, 179);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 184);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 194);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 199);
        TEST_ASSERT_EQUAL(data.path[1].end, 210);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 220);
        TEST_ASSERT_EQUAL(data.value.end, 223);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 233);
        TEST_ASSERT_EQUAL(data.path[1].end, 244);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 254);
        TEST_ASSERT_EQUAL(data.value.end, 257);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 267);
        TEST_ASSERT_EQUAL(data.path[1].end, 278);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 288);
        TEST_ASSERT_EQUAL(data.value.end, 292);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 302);
        TEST_ASSERT_EQUAL(data.path[1].end, 313);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 323);
        TEST_ASSERT_EQUAL(data.value.end, 326);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.path[1].start, 336);
        TEST_ASSERT_EQUAL(data.path[1].end, 347);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 357);
        TEST_ASSERT_EQUAL(data.value.end, 364);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 4);
        TEST_ASSERT_EQUAL(data.path[0].end, 4);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 369);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[[null,true,false,-0.0,\"string\"],{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"}]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 1);
        TEST_ASSERT_EQUAL(data.path[1].end, 1);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 1);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 1);
        TEST_ASSERT_EQUAL(data.path[1].end, 1);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 2);
        TEST_ASSERT_EQUAL(data.value.end, 5);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 1);
        TEST_ASSERT_EQUAL(data.path[1].end, 1);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 7);
        TEST_ASSERT_EQUAL(data.value.end, 10);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 1);
        TEST_ASSERT_EQUAL(data.path[1].end, 1);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 12);
        TEST_ASSERT_EQUAL(data.value.end, 16);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 1);
        TEST_ASSERT_EQUAL(data.path[1].end, 1);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 18);
        TEST_ASSERT_EQUAL(data.value.end, 21);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 1);
        TEST_ASSERT_EQUAL(data.path[1].end, 1);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 23);
        TEST_ASSERT_EQUAL(data.value.end, 30);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 31);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 33);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 34);
        TEST_ASSERT_EQUAL(data.path[1].end, 45);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 47);
        TEST_ASSERT_EQUAL(data.value.end, 50);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 52);
        TEST_ASSERT_EQUAL(data.path[1].end, 63);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 65);
        TEST_ASSERT_EQUAL(data.value.end, 68);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 70);
        TEST_ASSERT_EQUAL(data.path[1].end, 81);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 83);
        TEST_ASSERT_EQUAL(data.value.end, 87);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 89);
        TEST_ASSERT_EQUAL(data.path[1].end, 100);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 102);
        TEST_ASSERT_EQUAL(data.value.end, 105);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 107);
        TEST_ASSERT_EQUAL(data.path[1].end, 118);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 120);
        TEST_ASSERT_EQUAL(data.value.end, 127);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 128);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"},[null,true,false,-0.0,\"string\"]]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 0);
        TEST_ASSERT_EQUAL(data.value_counter[0], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_START);
        TEST_ASSERT_EQUAL(data.value.start, 1);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 2);
        TEST_ASSERT_EQUAL(data.path[1].end, 13);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 15);
        TEST_ASSERT_EQUAL(data.value.end, 18);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 20);
        TEST_ASSERT_EQUAL(data.path[1].end, 31);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 33);
        TEST_ASSERT_EQUAL(data.value.end, 36);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 38);
        TEST_ASSERT_EQUAL(data.path[1].end, 49);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 51);
        TEST_ASSERT_EQUAL(data.value.end, 55);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 57);
        TEST_ASSERT_EQUAL(data.path[1].end, 68);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 70);
        TEST_ASSERT_EQUAL(data.value.end, 73);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 75);
        TEST_ASSERT_EQUAL(data.path[1].end, 86);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 88);
        TEST_ASSERT_EQUAL(data.value.end, 95);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.type, JSON_OBJECT_END);
        TEST_ASSERT_EQUAL(data.value.end, 96);
        TEST_ASSERT_EQUAL(data.value_counter[0], 1);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 98);
        TEST_ASSERT_EQUAL(data.path[1].end, 98);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_START);
        TEST_ASSERT_EQUAL(data.value.start, 98);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 0);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 98);
        TEST_ASSERT_EQUAL(data.path[1].end, 98);
        TEST_ASSERT_EQUAL(data.type, JSON_NULL);
        TEST_ASSERT_EQUAL(data.value.start, 99);
        TEST_ASSERT_EQUAL(data.value.end, 102);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 1);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 98);
        TEST_ASSERT_EQUAL(data.path[1].end, 98);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 104);
        TEST_ASSERT_EQUAL(data.value.end, 107);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 2);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 98);
        TEST_ASSERT_EQUAL(data.path[1].end, 98);
        TEST_ASSERT_EQUAL(data.type, JSON_BOOLEAN);
        TEST_ASSERT_EQUAL(data.value.start, 109);
        TEST_ASSERT_EQUAL(data.value.end, 113);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 3);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 98);
        TEST_ASSERT_EQUAL(data.path[1].end, 98);
        TEST_ASSERT_EQUAL(data.type, JSON_NUMBER);
        TEST_ASSERT_EQUAL(data.value.start, 115);
        TEST_ASSERT_EQUAL(data.value.end, 118);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 4);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 1);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 98);
        TEST_ASSERT_EQUAL(data.path[1].end, 98);
        TEST_ASSERT_EQUAL(data.type, JSON_STRING);
        TEST_ASSERT_EQUAL(data.value.start, 120);
        TEST_ASSERT_EQUAL(data.value.end, 127);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_VALUE);
        TEST_ASSERT_EQUAL(data.depth, 0);
        TEST_ASSERT_EQUAL(data.path[0].start, 0);
        TEST_ASSERT_EQUAL(data.path[0].end, 0);
        TEST_ASSERT_EQUAL(data.path[1].start, 98);
        TEST_ASSERT_EQUAL(data.path[1].end, 98);
        TEST_ASSERT_EQUAL(data.type, JSON_ARRAY_END);
        TEST_ASSERT_EQUAL(data.value.end, 128);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
        TEST_ASSERT_EQUAL(data.value_counter[1], 5);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(result, JSON_RESULT_FINISH);
        TEST_ASSERT_EQUAL(data.value_counter[0], 2);
    }

}

void setUp(void){
    //Set things up
};

void tearDown(void){
    // clean up stuff
};

int main( void ) {
   UNITY_BEGIN();
    RUN_TEST(test_primitive_object);
    RUN_TEST(test_primitive_array);
    RUN_TEST(test_complex_object);
    RUN_TEST(test_complex_array);
   return UNITY_END(); 
   // static struct test const tests[] = {
   //     { test_primitive_object, "Object with primitive values" },
   //     { test_primitive_array, "Array with primitive values" },
   //     { test_complex_object, "Objects with complex values" },
   //     { test_complex_array, "Arrays with complex values" }
   // };

   // return test_suit(tests, sizeof tests / sizeof *tests);
}
