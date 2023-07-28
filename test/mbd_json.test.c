#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unity.h>

#include "mbd_json/mbd_json.h"

//#define done() return 0

//static int check_count = 0;
//#define TEST_ASSERT_EQUAL(int test_count, x) check_count++; if (!(x)) { return __LINE__; }

//struct test {
//    int(*func)(void);
//    char const* name;
//};
//
//static int test_suit(struct test const* tests) {
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

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n} \r\t\n";
        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":null}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(13, data.value.start);
        TEST_ASSERT_EQUAL(16, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\nnull \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(29, data.value.start);
        TEST_ASSERT_EQUAL(32, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":true}";
        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(13, data.value.start);
        TEST_ASSERT_EQUAL(16, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\ntrue \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(29, data.value.start);
        TEST_ASSERT_EQUAL(32, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":false}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(13, data.value.start);
        TEST_ASSERT_EQUAL(17, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\nfalse \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(29, data.value.start);
        TEST_ASSERT_EQUAL(33, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":0}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(13, data.value.start);
        TEST_ASSERT_EQUAL(13, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":0.0}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(13, data.value.start);
        TEST_ASSERT_EQUAL(15, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":-0.0}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(13, data.value.start);
        TEST_ASSERT_EQUAL(16, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n-0.0 \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(29, data.value.start);
        TEST_ASSERT_EQUAL(32, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
         const uint8_t json[] = "{\"property1\":\"string\"}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(13, data.value.start);
        TEST_ASSERT_EQUAL(20, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n\"string\" \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(29, data.value.start);
        TEST_ASSERT_EQUAL(36, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":null,\"property2\":true,\"property3\":false,\"property4\":-0.0,\"property5\":\"string\"}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(13, data.value.start);
        TEST_ASSERT_EQUAL(16, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(18, data.path[0].start);
        TEST_ASSERT_EQUAL(28, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(30, data.value.start);
        TEST_ASSERT_EQUAL(33, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(35, data.path[0].start);
        TEST_ASSERT_EQUAL(45, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(47, data.value.start);
        TEST_ASSERT_EQUAL(51, data.value.end);
        TEST_ASSERT_EQUAL(3, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(53, data.path[0].start);
        TEST_ASSERT_EQUAL(63, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(65, data.value.start);
        TEST_ASSERT_EQUAL(68, data.value.end);
        TEST_ASSERT_EQUAL(4, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(70, data.path[0].start);
        TEST_ASSERT_EQUAL(80, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(82, data.value.start);
        TEST_ASSERT_EQUAL(89, data.value.end);
        TEST_ASSERT_EQUAL(5, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(5, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\nnull \r\t\n, \r\t\n\"property2\" \r\t\n: \r\t\ntrue \r\t\n, \r\t\n\"property3\" \r\t\n: \r\t\nfalse \r\t\n, \r\t\n\"property4\" \r\t\n: \r\t\n-0.0 \r\t\n, \r\t\n\"property5\" \r\t\n: \r\t\n\"string\" \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(29, data.value.start);
        TEST_ASSERT_EQUAL(32, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(42, data.path[0].start);
        TEST_ASSERT_EQUAL(52, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(62, data.value.start);
        TEST_ASSERT_EQUAL(65, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(75, data.path[0].start);
        TEST_ASSERT_EQUAL(85, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(95, data.value.start);
        TEST_ASSERT_EQUAL(99, data.value.end);
        TEST_ASSERT_EQUAL(3, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(109, data.path[0].start);
        TEST_ASSERT_EQUAL(119, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(129, data.value.start);
        TEST_ASSERT_EQUAL(132, data.value.end);
        TEST_ASSERT_EQUAL(4, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(142, data.path[0].start);
        TEST_ASSERT_EQUAL(152, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(162, data.value.start);
        TEST_ASSERT_EQUAL(169, data.value.end);
        TEST_ASSERT_EQUAL(5, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(5, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":null,\"property2\":{\"sub_prob1\":true,\"sub_prob2\":1234},\"property3\":false,\"property4\":-0.0,\"property5\":\"string\"}";
        int result;
        int object_start_count = 0;
        int object_end_count = 0;

        do
        {
            result = parse_json(json, sizeof(json), &data);
            if(data.type == JSON_OBJECT_START)
                object_start_count++;

            if(data.type == JSON_OBJECT_END)
                object_end_count++;

        } while (result != JSON_RESULT_FINISH);
        
        TEST_ASSERT_EQUAL(object_start_count, object_end_count);
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

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[null]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(1, data.value.start);
        TEST_ASSERT_EQUAL(4, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\nnull \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(9, data.value.start);
        TEST_ASSERT_EQUAL(12, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[true]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(1, data.value.start);
        TEST_ASSERT_EQUAL(4, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\ntrue \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(9, data.value.start);
        TEST_ASSERT_EQUAL(12, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[false]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(1, data.value.start);
        TEST_ASSERT_EQUAL(5, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\nfalse \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(9, data.value.start);
        TEST_ASSERT_EQUAL(13, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[-0.0]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(1, data.value.start);
        TEST_ASSERT_EQUAL(4, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n-0.0 \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(9, data.value.start);
        TEST_ASSERT_EQUAL(12, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[\"string\"]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(1, data.value.start);
        TEST_ASSERT_EQUAL(8, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n\"string\" \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(9, data.value.start);
        TEST_ASSERT_EQUAL(16, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[null,true,false,-0.0,\"string\"]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(1, data.value.start);
        TEST_ASSERT_EQUAL(4, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(6, data.value.start);
        TEST_ASSERT_EQUAL(9, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(11, data.value.start);
        TEST_ASSERT_EQUAL(15, data.value.end);
        TEST_ASSERT_EQUAL(3, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(17, data.value.start);
        TEST_ASSERT_EQUAL(20, data.value.end);
        TEST_ASSERT_EQUAL(4, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(22, data.value.start);
        TEST_ASSERT_EQUAL(29, data.value.end);
        TEST_ASSERT_EQUAL(5, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\nnull \r\t\n, \r\t\ntrue \r\t\n, \r\t\nfalse \r\t\n, \r\t\n-0.0 \r\t\n, \r\t\n\"string\" \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(4 , data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(9, data.value.start);
        TEST_ASSERT_EQUAL(12, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(22, data.value.start);
        TEST_ASSERT_EQUAL(25, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(35, data.value.start);
        TEST_ASSERT_EQUAL(39, data.value.end);
        TEST_ASSERT_EQUAL(3, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(49, data.value.start);
        TEST_ASSERT_EQUAL(52, data.value.end);
        TEST_ASSERT_EQUAL(4, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(62, data.value.start);
        TEST_ASSERT_EQUAL(69, data.value.end);
        TEST_ASSERT_EQUAL(5, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(5, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property2\":[{\"property21\":123},{\"property22\":456}]}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(0 , data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(13 , data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
                
        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(2, data.depth);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(14, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(2, data.depth);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(28 , data.value.start);
        TEST_ASSERT_EQUAL(30, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);        

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(28 , data.value.start);
        TEST_ASSERT_EQUAL(31, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]); 

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(2, data.depth);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(33 , data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]); 

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(2, data.depth);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(47, data.value.start);
        TEST_ASSERT_EQUAL(49, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]); 

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(47, data.value.start);
        TEST_ASSERT_EQUAL(50, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]); 
        
        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(47, data.value.start);
        TEST_ASSERT_EQUAL(51, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);         

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(47, data.value.start);
        TEST_ASSERT_EQUAL(52, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);          
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property2\":[{\"property21\":123},{\"property22\":456}]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(0 , data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(13 , data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
                
        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(2, data.depth);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(14, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(2, data.depth);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(28 , data.value.start);
        TEST_ASSERT_EQUAL(30, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);        

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(28 , data.value.start);
        TEST_ASSERT_EQUAL(31, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]); 

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(2, data.depth);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(33 , data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]); 

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(2, data.depth);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(47, data.value.start);
        TEST_ASSERT_EQUAL(49, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]); 

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(47, data.value.start);
        TEST_ASSERT_EQUAL(50, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]); 
        
        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(47, data.value.start);
        TEST_ASSERT_EQUAL(51, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);         

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_INVALID, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(47, data.value.start);
        TEST_ASSERT_EQUAL(51, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]); 
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

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(13, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(14, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n{ \r\t\n} \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(29, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(34, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"}}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(13, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(14, data.path[1].start);
        TEST_ASSERT_EQUAL(25, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(27, data.value.start);
        TEST_ASSERT_EQUAL(30, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(32, data.path[1].start);
        TEST_ASSERT_EQUAL(43, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(45, data.value.start);
        TEST_ASSERT_EQUAL(48, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(50, data.path[1].start);
        TEST_ASSERT_EQUAL(61, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(63, data.value.start);
        TEST_ASSERT_EQUAL(67, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(69, data.path[1].start);
        TEST_ASSERT_EQUAL(80, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(82, data.value.start);
        TEST_ASSERT_EQUAL(85, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(87, data.path[1].start);
        TEST_ASSERT_EQUAL(98, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(100, data.value.start);
        TEST_ASSERT_EQUAL(107, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(108, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n{ \r\t\n\"property11\" \r\t\n: \r\t\nnull \r\t\n, \r\t\n\"property12\" \r\t\n: \r\t\ntrue \r\t\n, \r\t\n\"property13\" \r\t\n: \r\t\nfalse \r\t\n, \r\t\n\"property14\" \r\t\n: \r\t\n-0.0 \r\t\n, \r\t\n\"property15\" \r\t\n: \r\t\n\"string\" \r\t\n} \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(29, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(34, data.path[1].start);
        TEST_ASSERT_EQUAL(45, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(55, data.value.start);
        TEST_ASSERT_EQUAL(58, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(68, data.path[1].start);
        TEST_ASSERT_EQUAL(79, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(89, data.value.start);
        TEST_ASSERT_EQUAL(92, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(102, data.path[1].start);
        TEST_ASSERT_EQUAL(113, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(123, data.value.start);
        TEST_ASSERT_EQUAL(127, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(137, data.path[1].start);
        TEST_ASSERT_EQUAL(148, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(158, data.value.start);
        TEST_ASSERT_EQUAL(161, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(171, data.path[1].start);
        TEST_ASSERT_EQUAL(182, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(192, data.value.start);
        TEST_ASSERT_EQUAL(199, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(204, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":{},\"property2\":{}}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(13, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(14, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(16, data.path[0].start);
        TEST_ASSERT_EQUAL(26, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(28, data.value.start);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(16, data.path[0].start);
        TEST_ASSERT_EQUAL(26, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(29, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n{ \r\t\n} \r\t\n, \r\t\n\"property2\" \r\t\n: \r\t\n{ \r\t\n} \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(29, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(34, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(44, data.path[0].start);
        TEST_ASSERT_EQUAL(54, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(64, data.value.start);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(44, data.path[0].start);
        TEST_ASSERT_EQUAL(54, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(69, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"},\"property2\":{\"property21\":null,\"property22\":true,\"property23\":false,\"property24\":-0.0,\"property25\":\"string\"}}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(13, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(14, data.path[1].start);
        TEST_ASSERT_EQUAL(25, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(27, data.value.start);
        TEST_ASSERT_EQUAL(30, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(32, data.path[1].start);
        TEST_ASSERT_EQUAL(43, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(45, data.value.start);
        TEST_ASSERT_EQUAL(48, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(50, data.path[1].start);
        TEST_ASSERT_EQUAL(61, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(63, data.value.start);
        TEST_ASSERT_EQUAL(67, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(69, data.path[1].start);
        TEST_ASSERT_EQUAL(80, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(82, data.value.start);
        TEST_ASSERT_EQUAL(85, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(87, data.path[1].start);
        TEST_ASSERT_EQUAL(98, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(100, data.value.start);
        TEST_ASSERT_EQUAL(107, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(108, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(110, data.path[0].start);
        TEST_ASSERT_EQUAL(120, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(122, data.value.start);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(110, data.path[0].start);
        TEST_ASSERT_EQUAL(120, data.path[0].end);
        TEST_ASSERT_EQUAL(123, data.path[1].start);
        TEST_ASSERT_EQUAL(134, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(136, data.value.start);
        TEST_ASSERT_EQUAL(139, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(110, data.path[0].start);
        TEST_ASSERT_EQUAL(120, data.path[0].end);
        TEST_ASSERT_EQUAL(141, data.path[1].start);
        TEST_ASSERT_EQUAL(152, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(154, data.value.start);
        TEST_ASSERT_EQUAL(157, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(110, data.path[0].start);
        TEST_ASSERT_EQUAL(120, data.path[0].end);
        TEST_ASSERT_EQUAL(159, data.path[1].start);
        TEST_ASSERT_EQUAL(170, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(172, data.value.start);
        TEST_ASSERT_EQUAL(176, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(110, data.path[0].start);
        TEST_ASSERT_EQUAL(120, data.path[0].end);
        TEST_ASSERT_EQUAL(178, data.path[1].start);
        TEST_ASSERT_EQUAL(189, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(191, data.value.start);
        TEST_ASSERT_EQUAL(194, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(110, data.path[0].start);
        TEST_ASSERT_EQUAL(120, data.path[0].end);
        TEST_ASSERT_EQUAL(196, data.path[1].start);
        TEST_ASSERT_EQUAL(207, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(209, data.value.start);
        TEST_ASSERT_EQUAL(216, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(110, data.path[0].start);
        TEST_ASSERT_EQUAL(120, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(217, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n{ \r\t\n\"property11\" \r\t\n: \r\t\nnull \r\t\n, \r\t\n\"property12\" \r\t\n: \r\t\ntrue \r\t\n, \r\t\n\"property13\" \r\t\n: \r\t\nfalse \r\t\n, \r\t\n\"property14\" \r\t\n: \r\t\n-0.0 \r\t\n, \r\t\n\"property15\" \r\t\n: \r\t\n\"string\" \r\t\n}, \r\t\n\"property2\" \r\t\n: \r\t\n{ \r\t\n\"property21\" \r\t\n: \r\t\nnull \r\t\n, \r\t\n\"property22\" \r\t\n: \r\t\ntrue \r\t\n, \r\t\n\"property23\" \r\t\n: \r\t\nfalse \r\t\n, \r\t\n\"property24\" \r\t\n: \r\t\n-0.0 \r\t\n, \r\t\n\"property25\" \r\t\n: \r\t\n\"string\" \r\t\n} \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(29, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(34, data.path[1].start);
        TEST_ASSERT_EQUAL(45, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(55, data.value.start);
        TEST_ASSERT_EQUAL(58, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(68, data.path[1].start);
        TEST_ASSERT_EQUAL(79, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(89, data.value.start);
        TEST_ASSERT_EQUAL(92, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(102, data.path[1].start);
        TEST_ASSERT_EQUAL(113, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(123, data.value.start);
        TEST_ASSERT_EQUAL(127, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(137, data.path[1].start);
        TEST_ASSERT_EQUAL(148, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(158, data.value.start);
        TEST_ASSERT_EQUAL(161, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(171, data.path[1].start);
        TEST_ASSERT_EQUAL(182, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(192, data.value.start);
        TEST_ASSERT_EQUAL(199, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(204, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(210, data.path[0].start);
        TEST_ASSERT_EQUAL(220, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(230, data.value.start);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(210, data.path[0].start);
        TEST_ASSERT_EQUAL(220, data.path[0].end);
        TEST_ASSERT_EQUAL(235, data.path[1].start);
        TEST_ASSERT_EQUAL(246, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(256, data.value.start);
        TEST_ASSERT_EQUAL(259, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(210, data.path[0].start);
        TEST_ASSERT_EQUAL(220, data.path[0].end);
        TEST_ASSERT_EQUAL(269, data.path[1].start);
        TEST_ASSERT_EQUAL(280, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(290, data.value.start);
        TEST_ASSERT_EQUAL(293, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(210, data.path[0].start);
        TEST_ASSERT_EQUAL(220, data.path[0].end);
        TEST_ASSERT_EQUAL(303, data.path[1].start);
        TEST_ASSERT_EQUAL(314, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(324, data.value.start);
        TEST_ASSERT_EQUAL(328, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(210, data.path[0].start);
        TEST_ASSERT_EQUAL(220, data.path[0].end);
        TEST_ASSERT_EQUAL(338, data.path[1].start);
        TEST_ASSERT_EQUAL(349, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(359, data.value.start);
        TEST_ASSERT_EQUAL(362, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(210, data.path[0].start);
        TEST_ASSERT_EQUAL(220, data.path[0].end);
        TEST_ASSERT_EQUAL(372, data.path[1].start);
        TEST_ASSERT_EQUAL(383, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(393, data.value.start);
        TEST_ASSERT_EQUAL(400, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(210, data.path[0].start);
        TEST_ASSERT_EQUAL(220, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(405, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":[]}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(13, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(14, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n[ \r\t\n] \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(29, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(29, data.path[1].start);
        TEST_ASSERT_EQUAL(29, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(34, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":[null,true,false,-0.0,\"string\"]}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(13, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(14, data.value.start);
        TEST_ASSERT_EQUAL(17, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(19, data.value.start);
        TEST_ASSERT_EQUAL(22, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(24, data.value.start);
        TEST_ASSERT_EQUAL(28, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(30, data.value.start);
        TEST_ASSERT_EQUAL(33, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(35, data.value.start);
        TEST_ASSERT_EQUAL(42, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(43, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n[ \r\t\nnull \r\t\n, \r\t\ntrue \r\t\n, \r\t\nfalse \r\t\n, \r\t\n-0.0 \r\t\n, \r\t\n\"string\" \r\t\n] \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(29, data.path[1].start);
        TEST_ASSERT_EQUAL(29, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(29, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(29, data.path[1].start);
        TEST_ASSERT_EQUAL(29, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(34, data.value.start);
        TEST_ASSERT_EQUAL(37, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(29, data.path[1].start);
        TEST_ASSERT_EQUAL(29, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(47, data.value.start);
        TEST_ASSERT_EQUAL(50, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(29, data.path[1].start);
        TEST_ASSERT_EQUAL(29, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(60, data.value.start);
        TEST_ASSERT_EQUAL(64, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(29, data.path[1].start);
        TEST_ASSERT_EQUAL(29, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(74, data.value.start);
        TEST_ASSERT_EQUAL(77, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(29, data.path[1].start);
        TEST_ASSERT_EQUAL(29, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(87, data.value.start);
        TEST_ASSERT_EQUAL(94, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(29, data.path[1].start);
        TEST_ASSERT_EQUAL(29, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(99, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":[],\"property2\":[]}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(13, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(14, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(16, data.path[0].start);
        TEST_ASSERT_EQUAL(26, data.path[0].end);
        TEST_ASSERT_EQUAL(28, data.path[1].start);
        TEST_ASSERT_EQUAL(28, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(28, data.value.start);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(16, data.path[0].start);
        TEST_ASSERT_EQUAL(26, data.path[0].end);
        TEST_ASSERT_EQUAL(28, data.path[1].start);
        TEST_ASSERT_EQUAL(28, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(29, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n[ \r\t\n] \r\t\n, \r\t\n\"property2\" \r\t\n: \r\t\n[ \r\t\n] \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(29, data.path[1].start);
        TEST_ASSERT_EQUAL(29, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(29, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(29, data.path[1].start);
        TEST_ASSERT_EQUAL(29, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(34, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(44, data.path[0].start);
        TEST_ASSERT_EQUAL(54, data.path[0].end);
        TEST_ASSERT_EQUAL(64, data.path[1].start);
        TEST_ASSERT_EQUAL(64, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(64, data.value.start);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(44, data.path[0].start);
        TEST_ASSERT_EQUAL(54, data.path[0].end);
        TEST_ASSERT_EQUAL(64, data.path[1].start);
        TEST_ASSERT_EQUAL(64, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(69, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":[null,true,false,-0.0,\"string\"],\"property2\":[null,true,false,-0.0,\"string\"]}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(13, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(14, data.value.start);
        TEST_ASSERT_EQUAL(17, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(19, data.value.start);
        TEST_ASSERT_EQUAL(22, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(24, data.value.start);
        TEST_ASSERT_EQUAL(28, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(30, data.value.start);
        TEST_ASSERT_EQUAL(33, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(35, data.value.start);
        TEST_ASSERT_EQUAL(42, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(43, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(45, data.path[0].start);
        TEST_ASSERT_EQUAL(55, data.path[0].end);
        TEST_ASSERT_EQUAL(57, data.path[1].start);
        TEST_ASSERT_EQUAL(57, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(57, data.value.start);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(45, data.path[0].start);
        TEST_ASSERT_EQUAL(55, data.path[0].end);
        TEST_ASSERT_EQUAL(57, data.path[1].start);
        TEST_ASSERT_EQUAL(57, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(58, data.value.start);
        TEST_ASSERT_EQUAL(61, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(45, data.path[0].start);
        TEST_ASSERT_EQUAL(55, data.path[0].end);
        TEST_ASSERT_EQUAL(57, data.path[1].start);
        TEST_ASSERT_EQUAL(57, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(63, data.value.start);
        TEST_ASSERT_EQUAL(66, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(45, data.path[0].start);
        TEST_ASSERT_EQUAL(55, data.path[0].end);
        TEST_ASSERT_EQUAL(57, data.path[1].start);
        TEST_ASSERT_EQUAL(57, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(68, data.value.start);
        TEST_ASSERT_EQUAL(72, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(45, data.path[0].start);
        TEST_ASSERT_EQUAL(55, data.path[0].end);
        TEST_ASSERT_EQUAL(57, data.path[1].start);
        TEST_ASSERT_EQUAL(57, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(74, data.value.start);
        TEST_ASSERT_EQUAL(77, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(45, data.path[0].start);
        TEST_ASSERT_EQUAL(55, data.path[0].end);
        TEST_ASSERT_EQUAL(57, data.path[1].start);
        TEST_ASSERT_EQUAL(57, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(79, data.value.start);
        TEST_ASSERT_EQUAL(86, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(45, data.path[0].start);
        TEST_ASSERT_EQUAL(55, data.path[0].end);
        TEST_ASSERT_EQUAL(57, data.path[1].start);
        TEST_ASSERT_EQUAL(57, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(87, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n{ \r\t\n\"property1\" \r\t\n: \r\t\n[ \r\t\nnull \r\t\n, \r\t\ntrue \r\t\n, \r\t\nfalse \r\t\n, \r\t\n-0.0 \r\t\n, \r\t\n\"string\" \r\t\n] \r\t\n, \r\t\n\"property2\" \r\t\n: \r\t\n[ \r\t\nnull \r\t\n, \r\t\ntrue \r\t\n, \r\t\nfalse \r\t\n, \r\t\n-0.0 \r\t\n, \r\t\n\"string\" \r\t\n] \r\t\n} \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(29, data.path[1].start);
        TEST_ASSERT_EQUAL(29, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(29, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(29, data.path[1].start);
        TEST_ASSERT_EQUAL(29, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(34, data.value.start);
        TEST_ASSERT_EQUAL(37, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(29, data.path[1].start);
        TEST_ASSERT_EQUAL(29, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(47, data.value.start);
        TEST_ASSERT_EQUAL(50, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(29, data.path[1].start);
        TEST_ASSERT_EQUAL(29, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(60, data.value.start);
        TEST_ASSERT_EQUAL(64, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(29, data.path[1].start);
        TEST_ASSERT_EQUAL(29, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(74, data.value.start);
        TEST_ASSERT_EQUAL(77, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(29, data.path[1].start);
        TEST_ASSERT_EQUAL(29, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(87, data.value.start);
        TEST_ASSERT_EQUAL(94, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(9, data.path[0].start);
        TEST_ASSERT_EQUAL(19, data.path[0].end);
        TEST_ASSERT_EQUAL(29, data.path[1].start);
        TEST_ASSERT_EQUAL(29, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(99, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(109, data.path[0].start);
        TEST_ASSERT_EQUAL(119, data.path[0].end);
        TEST_ASSERT_EQUAL(129, data.path[1].start);
        TEST_ASSERT_EQUAL(129, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(129, data.value.start);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(109, data.path[0].start);
        TEST_ASSERT_EQUAL(119, data.path[0].end);
        TEST_ASSERT_EQUAL(129, data.path[1].start);
        TEST_ASSERT_EQUAL(129, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(134, data.value.start);
        TEST_ASSERT_EQUAL(137, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(109, data.path[0].start);
        TEST_ASSERT_EQUAL(119, data.path[0].end);
        TEST_ASSERT_EQUAL(129, data.path[1].start);
        TEST_ASSERT_EQUAL(129, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(147, data.value.start);
        TEST_ASSERT_EQUAL(150, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(109, data.path[0].start);
        TEST_ASSERT_EQUAL(119, data.path[0].end);
        TEST_ASSERT_EQUAL(129, data.path[1].start);
        TEST_ASSERT_EQUAL(129, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(160, data.value.start);
        TEST_ASSERT_EQUAL(164, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(109, data.path[0].start);
        TEST_ASSERT_EQUAL(119, data.path[0].end);
        TEST_ASSERT_EQUAL(129, data.path[1].start);
        TEST_ASSERT_EQUAL(129, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(174, data.value.start);
        TEST_ASSERT_EQUAL(177, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(109, data.path[0].start);
        TEST_ASSERT_EQUAL(119, data.path[0].end);
        TEST_ASSERT_EQUAL(129, data.path[1].start);
        TEST_ASSERT_EQUAL(129, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(187, data.value.start);
        TEST_ASSERT_EQUAL(194, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(109, data.path[0].start);
        TEST_ASSERT_EQUAL(119, data.path[0].end);
        TEST_ASSERT_EQUAL(129, data.path[1].start);
        TEST_ASSERT_EQUAL(129, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(199, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"},\"property2\":[null,true,false,-0.0,\"string\"]}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(13, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(14, data.path[1].start);
        TEST_ASSERT_EQUAL(25, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(27, data.value.start);
        TEST_ASSERT_EQUAL(30, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(32, data.path[1].start);
        TEST_ASSERT_EQUAL(43, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(45, data.value.start);
        TEST_ASSERT_EQUAL(48, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(50, data.path[1].start);
        TEST_ASSERT_EQUAL(61, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(63, data.value.start);
        TEST_ASSERT_EQUAL(67, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(69, data.path[1].start);
        TEST_ASSERT_EQUAL(80, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(82, data.value.start);
        TEST_ASSERT_EQUAL(85, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(87, data.path[1].start);
        TEST_ASSERT_EQUAL(98, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(100, data.value.start);
        TEST_ASSERT_EQUAL(107, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(108, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(110, data.path[0].start);
        TEST_ASSERT_EQUAL(120, data.path[0].end);
        TEST_ASSERT_EQUAL(122, data.path[1].start);
        TEST_ASSERT_EQUAL(122, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(122, data.value.start);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(110, data.path[0].start);
        TEST_ASSERT_EQUAL(120, data.path[0].end);
        TEST_ASSERT_EQUAL(122, data.path[1].start);
        TEST_ASSERT_EQUAL(122, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(123, data.value.start);
        TEST_ASSERT_EQUAL(126, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(110, data.path[0].start);
        TEST_ASSERT_EQUAL(120, data.path[0].end);
        TEST_ASSERT_EQUAL(122, data.path[1].start);
        TEST_ASSERT_EQUAL(122, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(128, data.value.start);
        TEST_ASSERT_EQUAL(131, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(110, data.path[0].start);
        TEST_ASSERT_EQUAL(120, data.path[0].end);
        TEST_ASSERT_EQUAL(122, data.path[1].start);
        TEST_ASSERT_EQUAL(122, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(133, data.value.start);
        TEST_ASSERT_EQUAL(137, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(110, data.path[0].start);
        TEST_ASSERT_EQUAL(120, data.path[0].end);
        TEST_ASSERT_EQUAL(122, data.path[1].start);
        TEST_ASSERT_EQUAL(122, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(139, data.value.start);
        TEST_ASSERT_EQUAL(142, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(110, data.path[0].start);
        TEST_ASSERT_EQUAL(120, data.path[0].end);
        TEST_ASSERT_EQUAL(122, data.path[1].start);
        TEST_ASSERT_EQUAL(122, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(144, data.value.start);
        TEST_ASSERT_EQUAL(151, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(110, data.path[0].start);
        TEST_ASSERT_EQUAL(120, data.path[0].end);
        TEST_ASSERT_EQUAL(122, data.path[1].start);
        TEST_ASSERT_EQUAL(122, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(152, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "{\"property1\":[null,true,false,-0.0,\"string\"],\"property2\":{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"}}";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(13, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(14, data.value.start);
        TEST_ASSERT_EQUAL(17, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(19, data.value.start);
        TEST_ASSERT_EQUAL(22, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(24, data.value.start);
        TEST_ASSERT_EQUAL(28, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(30, data.value.start);
        TEST_ASSERT_EQUAL(33, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(35, data.value.start);
        TEST_ASSERT_EQUAL(42, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(1, data.path[0].start);
        TEST_ASSERT_EQUAL(11, data.path[0].end);
        TEST_ASSERT_EQUAL(13, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(43, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(45, data.path[0].start);
        TEST_ASSERT_EQUAL(55, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(57, data.value.start);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(45, data.path[0].start);
        TEST_ASSERT_EQUAL(55, data.path[0].end);
        TEST_ASSERT_EQUAL(58, data.path[1].start);
        TEST_ASSERT_EQUAL(69, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(71, data.value.start);
        TEST_ASSERT_EQUAL(74, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(45, data.path[0].start);
        TEST_ASSERT_EQUAL(55, data.path[0].end);
        TEST_ASSERT_EQUAL(76, data.path[1].start);
        TEST_ASSERT_EQUAL(87, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(89, data.value.start);
        TEST_ASSERT_EQUAL(92, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(45, data.path[0].start);
        TEST_ASSERT_EQUAL(55, data.path[0].end);
        TEST_ASSERT_EQUAL(94, data.path[1].start);
        TEST_ASSERT_EQUAL(105, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(107, data.value.start);
        TEST_ASSERT_EQUAL(111, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(45, data.path[0].start);
        TEST_ASSERT_EQUAL(55, data.path[0].end);
        TEST_ASSERT_EQUAL(113, data.path[1].start);
        TEST_ASSERT_EQUAL(124, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(126, data.value.start);
        TEST_ASSERT_EQUAL(129, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(45, data.path[0].start);
        TEST_ASSERT_EQUAL(55, data.path[0].end);
        TEST_ASSERT_EQUAL(131, data.path[1].start);
        TEST_ASSERT_EQUAL(142, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(144, data.value.start);
        TEST_ASSERT_EQUAL(151, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(45, data.path[0].start);
        TEST_ASSERT_EQUAL(55, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(152, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
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

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(1, data.path[1].start);
        TEST_ASSERT_EQUAL(1, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(1, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(2, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n[ \r\t\n] \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(9, data.path[1].start);
        TEST_ASSERT_EQUAL(9, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(9, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(14, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[[null,true,false,-0.0,\"string\"]]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(1, data.path[1].start);
        TEST_ASSERT_EQUAL(1, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(1, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(1, data.path[1].start);
        TEST_ASSERT_EQUAL(1, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(2, data.value.start);
        TEST_ASSERT_EQUAL(5, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(1, data.path[1].start);
        TEST_ASSERT_EQUAL(1, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(7, data.value.start);
        TEST_ASSERT_EQUAL(10, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(1, data.path[1].start);
        TEST_ASSERT_EQUAL(1, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(12, data.value.start);
        TEST_ASSERT_EQUAL(16, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(1, data.path[1].start);
        TEST_ASSERT_EQUAL(1, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(18, data.value.start);
        TEST_ASSERT_EQUAL(21, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(1, data.path[1].start);
        TEST_ASSERT_EQUAL(1, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(23, data.value.start);
        TEST_ASSERT_EQUAL(30, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(31, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n[ \r\t\nnull \r\t\n, \r\t\ntrue \r\t\n, \r\t\nfalse \r\t\n, \r\t\n-0.0 \r\t\n, \r\t\n\"string\" \r\t\n] \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(9, data.path[1].start);
        TEST_ASSERT_EQUAL(9, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(9, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(9, data.path[1].start);
        TEST_ASSERT_EQUAL(9, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(14, data.value.start);
        TEST_ASSERT_EQUAL(17, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(9, data.path[1].start);
        TEST_ASSERT_EQUAL(9, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(27, data.value.start);
        TEST_ASSERT_EQUAL(30, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(9, data.path[1].start);
        TEST_ASSERT_EQUAL(9, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(40, data.value.start);
        TEST_ASSERT_EQUAL(44, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(9, data.path[1].start);
        TEST_ASSERT_EQUAL(9, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(54, data.value.start);
        TEST_ASSERT_EQUAL(57, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(9, data.path[1].start);
        TEST_ASSERT_EQUAL(9, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(67, data.value.start);
        TEST_ASSERT_EQUAL(74, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(79, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[[],[]]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(1, data.path[1].start);
        TEST_ASSERT_EQUAL(1, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(1, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(2, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(4, data.path[1].start);
        TEST_ASSERT_EQUAL(4, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(5, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n[ \r\t\n] \r\t\n, \r\t\n[ \r\t\n] \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(9, data.path[1].start);
        TEST_ASSERT_EQUAL(9, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(9, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(14, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(24, data.path[1].start);
        TEST_ASSERT_EQUAL(24, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(24, data.value.start);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(29, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[[null,true,false,-0.0,\"string\"],[null,true,false,-0.0,\"string\"]]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(1, data.path[1].start);
        TEST_ASSERT_EQUAL(1, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(1, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(1, data.path[1].start);
        TEST_ASSERT_EQUAL(1, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(2, data.value.start);
        TEST_ASSERT_EQUAL(5, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(1, data.path[1].start);
        TEST_ASSERT_EQUAL(1, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(7, data.value.start);
        TEST_ASSERT_EQUAL(10, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(1, data.path[1].start);
        TEST_ASSERT_EQUAL(1, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(12, data.value.start);
        TEST_ASSERT_EQUAL(16, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(1, data.path[1].start);
        TEST_ASSERT_EQUAL(1, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(18, data.value.start);
        TEST_ASSERT_EQUAL(21, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(1, data.path[1].start);
        TEST_ASSERT_EQUAL(1, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(23, data.value.start);
        TEST_ASSERT_EQUAL(30, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(31, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(33, data.path[1].start);
        TEST_ASSERT_EQUAL(33, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(33, data.value.start);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(33, data.path[1].start);
        TEST_ASSERT_EQUAL(33, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(34, data.value.start);
        TEST_ASSERT_EQUAL(37, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(33, data.path[1].start);
        TEST_ASSERT_EQUAL(33, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(39, data.value.start);
        TEST_ASSERT_EQUAL(42, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(33, data.path[1].start);
        TEST_ASSERT_EQUAL(33, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(44, data.value.start);
        TEST_ASSERT_EQUAL(48, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(33, data.path[1].start);
        TEST_ASSERT_EQUAL(33, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(50, data.value.start);
        TEST_ASSERT_EQUAL(53, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(33, data.path[1].start);
        TEST_ASSERT_EQUAL(33, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(55, data.value.start);
        TEST_ASSERT_EQUAL(62, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(63, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n[ \r\t\nnull \r\t\n, \r\t\ntrue \r\t\n, \r\t\nfalse \r\t\n, \r\t\n-0.0 \r\t\n, \r\t\n\"string\" \r\t\n] \r\t\n, \r\t\n[ \r\t\nnull \r\t\n, \r\t\ntrue \r\t\n, \r\t\nfalse \r\t\n, \r\t\n-0.0 \r\t\n, \r\t\n\"string\" \r\t\n] \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(9, data.path[1].start);
        TEST_ASSERT_EQUAL(9, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(9, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(9, data.path[1].start);
        TEST_ASSERT_EQUAL(9, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(14, data.value.start);
        TEST_ASSERT_EQUAL(17, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(9, data.path[1].start);
        TEST_ASSERT_EQUAL(9, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(27, data.value.start);
        TEST_ASSERT_EQUAL(30, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(9, data.path[1].start);
        TEST_ASSERT_EQUAL(9, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(40, data.value.start);
        TEST_ASSERT_EQUAL(44, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(9, data.path[1].start);
        TEST_ASSERT_EQUAL(9, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(54, data.value.start);
        TEST_ASSERT_EQUAL(57, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(9, data.path[1].start);
        TEST_ASSERT_EQUAL(9, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(67, data.value.start);
        TEST_ASSERT_EQUAL(74, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(79, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(89, data.path[1].start);
        TEST_ASSERT_EQUAL(89, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(89, data.value.start);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(89, data.path[1].start);
        TEST_ASSERT_EQUAL(89, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(94, data.value.start);
        TEST_ASSERT_EQUAL(97, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(89, data.path[1].start);
        TEST_ASSERT_EQUAL(89, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(107, data.value.start);
        TEST_ASSERT_EQUAL(110, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(89, data.path[1].start);
        TEST_ASSERT_EQUAL(89, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(120, data.value.start);
        TEST_ASSERT_EQUAL(124, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(89, data.path[1].start);
        TEST_ASSERT_EQUAL(89, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(134, data.value.start);
        TEST_ASSERT_EQUAL(137, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(89, data.path[1].start);
        TEST_ASSERT_EQUAL(89, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(147, data.value.start);
        TEST_ASSERT_EQUAL(154, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(159, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[{}]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(1, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(2, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n{ \r\t\n} \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(9, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(14, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"}]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(1, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(2, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(15, data.value.start);
        TEST_ASSERT_EQUAL(18, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(20, data.path[1].start);
        TEST_ASSERT_EQUAL(31, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(33, data.value.start);
        TEST_ASSERT_EQUAL(36, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(38, data.path[1].start);
        TEST_ASSERT_EQUAL(49, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(51, data.value.start);
        TEST_ASSERT_EQUAL(55, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(57, data.path[1].start);
        TEST_ASSERT_EQUAL(68, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(70, data.value.start);
        TEST_ASSERT_EQUAL(73, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(75, data.path[1].start);
        TEST_ASSERT_EQUAL(86, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(88, data.value.start);
        TEST_ASSERT_EQUAL(95, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(96, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n{ \r\t\n\"property11\" \r\t\n: \r\t\nnull \r\t\n, \r\t\n\"property12\" \r\t\n: \r\t\ntrue \r\t\n, \r\t\n\"property13\" \r\t\n: \r\t\nfalse \r\t\n, \r\t\n\"property14\" \r\t\n: \r\t\n-0.0 \r\t\n, \r\t\n\"property15\" \r\t\n: \r\t\n\"string\" \r\t\n} \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(9, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(14, data.path[1].start);
        TEST_ASSERT_EQUAL(25, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(35, data.value.start);
        TEST_ASSERT_EQUAL(38, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(48, data.path[1].start);
        TEST_ASSERT_EQUAL(59, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(69, data.value.start);
        TEST_ASSERT_EQUAL(72, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(82, data.path[1].start);
        TEST_ASSERT_EQUAL(93, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(103, data.value.start);
        TEST_ASSERT_EQUAL(107, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(117, data.path[1].start);
        TEST_ASSERT_EQUAL(128, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(138, data.value.start);
        TEST_ASSERT_EQUAL(141, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(151, data.path[1].start);
        TEST_ASSERT_EQUAL(162, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(172, data.value.start);
        TEST_ASSERT_EQUAL(179, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(184, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[{},{}]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(1, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(2, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(5, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n{ \r\t\n} \r\t\n, \r\t\n{ \r\t\n} \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(9, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(14, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(24, data.value.start);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(29, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"},{\"property21\":null,\"property22\":true,\"property23\":false,\"property24\":-0.0,\"property25\":\"string\"}]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(1, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(2, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(15, data.value.start);
        TEST_ASSERT_EQUAL(18, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(20, data.path[1].start);
        TEST_ASSERT_EQUAL(31, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(33, data.value.start);
        TEST_ASSERT_EQUAL(36, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(38, data.path[1].start);
        TEST_ASSERT_EQUAL(49, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(51, data.value.start);
        TEST_ASSERT_EQUAL(55, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(57, data.path[1].start);
        TEST_ASSERT_EQUAL(68, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(70, data.value.start);
        TEST_ASSERT_EQUAL(73, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(75, data.path[1].start);
        TEST_ASSERT_EQUAL(86, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(88, data.value.start);
        TEST_ASSERT_EQUAL(95, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(96, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(98, data.value.start);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(99, data.path[1].start);
        TEST_ASSERT_EQUAL(110, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(112, data.value.start);
        TEST_ASSERT_EQUAL(115, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(117, data.path[1].start);
        TEST_ASSERT_EQUAL(128, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(130, data.value.start);
        TEST_ASSERT_EQUAL(133, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(135, data.path[1].start);
        TEST_ASSERT_EQUAL(146, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(148, data.value.start);
        TEST_ASSERT_EQUAL(152, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(154, data.path[1].start);
        TEST_ASSERT_EQUAL(165, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(167, data.value.start);
        TEST_ASSERT_EQUAL(170, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(172, data.path[1].start);
        TEST_ASSERT_EQUAL(183, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(185, data.value.start);
        TEST_ASSERT_EQUAL(192, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(193, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = " \r\t\n[ \r\t\n{ \r\t\n\"property11\" \r\t\n: \r\t\nnull \r\t\n, \r\t\n\"property12\" \r\t\n: \r\t\ntrue \r\t\n, \r\t\n\"property13\" \r\t\n: \r\t\nfalse \r\t\n, \r\t\n\"property14\" \r\t\n: \r\t\n-0.0 \r\t\n, \r\t\n\"property15\" \r\t\n: \r\t\n\"string\" \r\t\n} \r\t\n, \r\t\n{ \r\t\n\"property21\" \r\t\n: \r\t\nnull \r\t\n, \r\t\n\"property22\" \r\t\n: \r\t\ntrue \r\t\n, \r\t\n\"property23\" \r\t\n: \r\t\nfalse \r\t\n, \r\t\n\"property24\" \r\t\n: \r\t\n-0.0 \r\t\n, \r\t\n\"property25\" \r\t\n: \r\t\n\"string\" \r\t\n} \r\t\n] \r\t\n";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(4, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(9, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(14, data.path[1].start);
        TEST_ASSERT_EQUAL(25, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(35, data.value.start);
        TEST_ASSERT_EQUAL(38, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(48, data.path[1].start);
        TEST_ASSERT_EQUAL(59, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(69, data.value.start);
        TEST_ASSERT_EQUAL(72, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(82, data.path[1].start);
        TEST_ASSERT_EQUAL(93, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(103, data.value.start);
        TEST_ASSERT_EQUAL(107, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(117, data.path[1].start);
        TEST_ASSERT_EQUAL(128, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(138, data.value.start);
        TEST_ASSERT_EQUAL(141, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(151, data.path[1].start);
        TEST_ASSERT_EQUAL(162, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(172, data.value.start);
        TEST_ASSERT_EQUAL(179, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(184, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(194, data.value.start);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(199, data.path[1].start);
        TEST_ASSERT_EQUAL(210, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(220, data.value.start);
        TEST_ASSERT_EQUAL(223, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(233, data.path[1].start);
        TEST_ASSERT_EQUAL(244, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(254, data.value.start);
        TEST_ASSERT_EQUAL(257, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(267, data.path[1].start);
        TEST_ASSERT_EQUAL(278, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(288, data.value.start);
        TEST_ASSERT_EQUAL(292, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(302, data.path[1].start);
        TEST_ASSERT_EQUAL(313, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(323, data.value.start);
        TEST_ASSERT_EQUAL(326, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(336, data.path[1].start);
        TEST_ASSERT_EQUAL(347, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(357, data.value.start);
        TEST_ASSERT_EQUAL(364, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(4, data.path[0].start);
        TEST_ASSERT_EQUAL(4, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(369, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[[null,true,false,-0.0,\"string\"],{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"}]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(1, data.path[1].start);
        TEST_ASSERT_EQUAL(1, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(1, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(1, data.path[1].start);
        TEST_ASSERT_EQUAL(1, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(2, data.value.start);
        TEST_ASSERT_EQUAL(5, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(1, data.path[1].start);
        TEST_ASSERT_EQUAL(1, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(7, data.value.start);
        TEST_ASSERT_EQUAL(10, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(1, data.path[1].start);
        TEST_ASSERT_EQUAL(1, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(12, data.value.start);
        TEST_ASSERT_EQUAL(16, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(1, data.path[1].start);
        TEST_ASSERT_EQUAL(1, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(18, data.value.start);
        TEST_ASSERT_EQUAL(21, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(1, data.path[1].start);
        TEST_ASSERT_EQUAL(1, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(23, data.value.start);
        TEST_ASSERT_EQUAL(30, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(31, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(33, data.value.start);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(34, data.path[1].start);
        TEST_ASSERT_EQUAL(45, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(47, data.value.start);
        TEST_ASSERT_EQUAL(50, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(52, data.path[1].start);
        TEST_ASSERT_EQUAL(63, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(65, data.value.start);
        TEST_ASSERT_EQUAL(68, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(70, data.path[1].start);
        TEST_ASSERT_EQUAL(81, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(83, data.value.start);
        TEST_ASSERT_EQUAL(87, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(89, data.path[1].start);
        TEST_ASSERT_EQUAL(100, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(102, data.value.start);
        TEST_ASSERT_EQUAL(105, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(107, data.path[1].start);
        TEST_ASSERT_EQUAL(118, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(120, data.value.start);
        TEST_ASSERT_EQUAL(127, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(128, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
    }
    {
        reset_json_data(&data);
        const uint8_t json[] = "[{\"property11\":null,\"property12\":true,\"property13\":false,\"property14\":-0.0,\"property15\":\"string\"},[null,true,false,-0.0,\"string\"]]";

        int result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(0, data.value.start);
        TEST_ASSERT_EQUAL(0, data.value_counter[0]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_START, data.type);
        TEST_ASSERT_EQUAL(1, data.value.start);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(2, data.path[1].start);
        TEST_ASSERT_EQUAL(13, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(15, data.value.start);
        TEST_ASSERT_EQUAL(18, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(20, data.path[1].start);
        TEST_ASSERT_EQUAL(31, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(33, data.value.start);
        TEST_ASSERT_EQUAL(36, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(38, data.path[1].start);
        TEST_ASSERT_EQUAL(49, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(51, data.value.start);
        TEST_ASSERT_EQUAL(55, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(57, data.path[1].start);
        TEST_ASSERT_EQUAL(68, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(70, data.value.start);
        TEST_ASSERT_EQUAL(73, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(75, data.path[1].start);
        TEST_ASSERT_EQUAL(86, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(88, data.value.start);
        TEST_ASSERT_EQUAL(95, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(JSON_OBJECT_END, data.type);
        TEST_ASSERT_EQUAL(96, data.value.end);
        TEST_ASSERT_EQUAL(1, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(98, data.path[1].start);
        TEST_ASSERT_EQUAL(98, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_START, data.type);
        TEST_ASSERT_EQUAL(98, data.value.start);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(0, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(98, data.path[1].start);
        TEST_ASSERT_EQUAL(98, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NULL, data.type);
        TEST_ASSERT_EQUAL(99, data.value.start);
        TEST_ASSERT_EQUAL(102, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(1, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(98, data.path[1].start);
        TEST_ASSERT_EQUAL(98, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(104, data.value.start);
        TEST_ASSERT_EQUAL(107, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(2, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(98, data.path[1].start);
        TEST_ASSERT_EQUAL(98, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_BOOLEAN, data.type);
        TEST_ASSERT_EQUAL(109, data.value.start);
        TEST_ASSERT_EQUAL(113, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(3, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(98, data.path[1].start);
        TEST_ASSERT_EQUAL(98, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_NUMBER, data.type);
        TEST_ASSERT_EQUAL(115, data.value.start);
        TEST_ASSERT_EQUAL(118, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(4, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(1, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(98, data.path[1].start);
        TEST_ASSERT_EQUAL(98, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_STRING, data.type);
        TEST_ASSERT_EQUAL(120, data.value.start);
        TEST_ASSERT_EQUAL(127, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_VALUE, result);
        TEST_ASSERT_EQUAL(0, data.depth);
        TEST_ASSERT_EQUAL(0, data.path[0].start);
        TEST_ASSERT_EQUAL(0, data.path[0].end);
        TEST_ASSERT_EQUAL(98, data.path[1].start);
        TEST_ASSERT_EQUAL(98, data.path[1].end);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
        TEST_ASSERT_EQUAL(128, data.value.end);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(5, data.value_counter[1]);

        result = parse_json(json, sizeof(json), &data);

        TEST_ASSERT_EQUAL(JSON_RESULT_FINISH, result);
        TEST_ASSERT_EQUAL(2, data.value_counter[0]);
        TEST_ASSERT_EQUAL(JSON_ARRAY_END, data.type);
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
}
