#include <mbd_json/mbd_json.h>

void reset_json_data(json_data_t* data) {
    data->pos = 0;
    data->depth = 0;
    data->parsing[0] = JSON_OBJECT_OR_ARRAY_BEFORE;
}

json_parse_result_t parse_json(const uint8_t* json, const uint16_t size, json_data_t* data) {
    uint8_t c;
    uint8_t number_dot_appeared = 0;

    for (; data->pos < size; data->pos++) {
        c = json[data->pos];

        if ((data->parsing[data->depth] == JSON_OBJECT_OR_ARRAY_BEFORE || data->parsing[data->depth] == JSON_OBJECT_VALUE_BEFORE || data->parsing[data->depth] == JSON_ARRAY_VALUE_BEFORE) && c == '{') {
            if (data->parsing[data->depth] == JSON_OBJECT_VALUE_BEFORE || data->parsing[data->depth] == JSON_ARRAY_VALUE_BEFORE) {
                data->value_counter[data->depth]++;
                data->parsing[data->depth] = data->parsing[data->depth] == JSON_OBJECT_VALUE_BEFORE ? JSON_OBJECT_VALUE_AFTER : JSON_ARRAY_VALUE_AFTER;
                data->depth++;

                if (data->depth > data->max_depth) {
                    return JSON_RESULT_TOO_DEEP;
                }
            }

            data->parsing[data->depth] = JSON_OBJECT_PROPERTY_BEFORE;
            data->value.start = data->pos;
            data->value.end = 0;
            data->type = JSON_OBJECT_START;
            data->value_counter[data->depth] = 0;
            data->pos++;

            return JSON_RESULT_VALUE;
        }

        if ((data->parsing[data->depth] == JSON_OBJECT_PROPERTY_BEFORE || data->parsing[data->depth] == JSON_OBJECT_VALUE_BEFORE || data->parsing[data->depth] == JSON_OBJECT_VALUE_NUMBER || data->parsing[data->depth] == JSON_OBJECT_VALUE_AFTER) && c == '}') {
            if (data->parsing[data->depth] == JSON_OBJECT_VALUE_NUMBER) {
                data->value.end = data->pos - 1;
                data->type = JSON_NUMBER;
                data->parsing[data->depth] = JSON_OBJECT_VALUE_AFTER;
                return JSON_RESULT_VALUE;
            }

            if (data->depth == 0) {
                return JSON_RESULT_FINISH;
            }

            data->type = JSON_OBJECT_END;
            data->value.end = data->pos;
            data->depth--;
            data->pos++;

            return JSON_RESULT_VALUE;
        }

        if ((data->parsing[data->depth] == JSON_OBJECT_OR_ARRAY_BEFORE || data->parsing[data->depth] == JSON_OBJECT_VALUE_BEFORE || data->parsing[data->depth] == JSON_ARRAY_VALUE_BEFORE) && c == '[') {
            if (data->parsing[data->depth] == JSON_OBJECT_VALUE_BEFORE || data->parsing[data->depth] == JSON_ARRAY_VALUE_BEFORE) {
                data->value_counter[data->depth]++;
                data->parsing[data->depth] = data->parsing[data->depth] == JSON_OBJECT_VALUE_BEFORE ? JSON_OBJECT_VALUE_AFTER : JSON_ARRAY_VALUE_AFTER;
                data->depth++;

                if (data->depth > data->max_depth) {
                    return JSON_RESULT_TOO_DEEP;
                }
            }

            data->parsing[data->depth] = JSON_ARRAY_VALUE_BEFORE;
            data->path[data->depth].start = data->pos;
            data->path[data->depth].end = data->pos;
            data->value.start = data->pos;
            data->value.end = 0;
            data->type = JSON_ARRAY_START;
            data->value_counter[data->depth] = 0;
            data->pos++;

            return JSON_RESULT_VALUE;
        }

        if ((data->parsing[data->depth] == JSON_ARRAY_VALUE_BEFORE || data->parsing[data->depth] == JSON_ARRAY_VALUE_NUMBER || data->parsing[data->depth] == JSON_ARRAY_VALUE_AFTER) && c == ']') {
            if (data->parsing[data->depth] == JSON_ARRAY_VALUE_NUMBER) {
                data->value.end = data->pos - 1;
                data->type = JSON_NUMBER;
                data->parsing[data->depth] = JSON_ARRAY_VALUE_AFTER;
                return JSON_RESULT_VALUE;
            }

            if (data->depth == 0) {
                return JSON_RESULT_FINISH;
            }

            data->type = JSON_ARRAY_END;
            data->value.end = data->pos;
            data->depth--;
            data->pos++;

            return JSON_RESULT_VALUE;
        }

        if ((data->parsing[data->depth] == JSON_OBJECT_OR_ARRAY_BEFORE ||
            data->parsing[data->depth] == JSON_OBJECT_PROPERTY_BEFORE ||
            data->parsing[data->depth] == JSON_OBJECT_PROPERTY_AFTER ||
            data->parsing[data->depth] == JSON_OBJECT_VALUE_BEFORE ||
            data->parsing[data->depth] == JSON_OBJECT_VALUE_NUMBER ||
            data->parsing[data->depth] == JSON_OBJECT_VALUE_AFTER ||
            data->parsing[data->depth] == JSON_ARRAY_VALUE_BEFORE ||
            data->parsing[data->depth] == JSON_ARRAY_VALUE_NUMBER ||
            data->parsing[data->depth] == JSON_ARRAY_VALUE_AFTER)
            &&
            (c == ' ' || c == '\t' || c == '\r' || c == '\n')
        ) {
            if (data->parsing[data->depth] == JSON_OBJECT_VALUE_NUMBER || data->parsing[data->depth] == JSON_ARRAY_VALUE_NUMBER) {
                data->value.end = data->pos - 1;
                data->type = JSON_NUMBER;
                data->parsing[data->depth] = data->parsing[data->depth] == JSON_OBJECT_VALUE_NUMBER ? JSON_OBJECT_VALUE_AFTER : JSON_ARRAY_VALUE_AFTER;
                return JSON_RESULT_VALUE;
            }

            continue;
        }

        if (data->parsing[data->depth] == JSON_OBJECT_PROPERTY_BEFORE && c == '\"') {
            data->parsing[data->depth] = JSON_OBJECT_PROPERTY;
            data->path[data->depth].start = data->pos;
            continue;
        }

        if (data->parsing[data->depth] == JSON_OBJECT_PROPERTY && c == '\"') {
            data->parsing[data->depth] = JSON_OBJECT_PROPERTY_AFTER;
            data->path[data->depth].end = data->pos;
            continue;
        }

        if (data->parsing[data->depth] == JSON_OBJECT_VALUE_BEFORE || data->parsing[data->depth] == JSON_ARRAY_VALUE_BEFORE) {
            switch (c) {
                case '\"':
                    data->value.start = data->pos;
                    data->value_counter[data->depth]++;
                    data->parsing[data->depth] = data->parsing[data->depth] == JSON_OBJECT_VALUE_BEFORE ? JSON_OBJECT_VALUE_STRING : JSON_ARRAY_VALUE_STRING;
                    continue;

                case '-': case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                    data->value.start = data->pos;
                    data->value_counter[data->depth]++;
                    data->parsing[data->depth] = data->parsing[data->depth] == JSON_OBJECT_VALUE_BEFORE ? JSON_OBJECT_VALUE_NUMBER : JSON_ARRAY_VALUE_NUMBER;
                    continue;
            }

            if (c == 't') {
                if (data->pos + 3 < size) {
                    if (json[data->pos + 1] == 'r' && json[data->pos + 2] == 'u' && json[data->pos + 3] == 'e') {
                        data->value.start = data->pos;
                        data->value.end = data->pos + 3;
                        data->type = JSON_BOOLEAN;
                        data->value_counter[data->depth]++;
                        data->parsing[data->depth] = data->parsing[data->depth] == JSON_OBJECT_VALUE_BEFORE ? JSON_OBJECT_VALUE_AFTER : JSON_ARRAY_VALUE_AFTER;
                        data->pos += 4;
                        return JSON_RESULT_VALUE;
                    }
                }
            }

            else if (c == 'f') {
                if (data->pos + 4 < size) {
                    if (json[data->pos + 1] == 'a' && json[data->pos + 2] == 'l' && json[data->pos + 3] == 's' && json[data->pos + 4] == 'e') {
                        data->value.start = data->pos;
                        data->value.end = data->pos + 4;
                        data->type = JSON_BOOLEAN;
                        data->value_counter[data->depth]++;
                        data->parsing[data->depth] = data->parsing[data->depth] == JSON_OBJECT_VALUE_BEFORE ? JSON_OBJECT_VALUE_AFTER : JSON_ARRAY_VALUE_AFTER;
                        data->pos += 5;
                        return JSON_RESULT_VALUE;
                    }
                }
            }

            else if (c == 'n') {
                if (data->pos + 3 < size) {
                    if (json[data->pos + 1] == 'u' && json[data->pos + 2] == 'l' && json[data->pos + 3] == 'l') {
                        data->value.start = data->pos;
                        data->value.end = data->pos + 3;
                        data->type = JSON_NULL;
                        data->value_counter[data->depth]++;
                        data->parsing[data->depth] = data->parsing[data->depth] == JSON_OBJECT_VALUE_BEFORE ? JSON_OBJECT_VALUE_AFTER : JSON_ARRAY_VALUE_AFTER;
                        data->pos += 4;
                        return JSON_RESULT_VALUE;
                    }
                }
            }
        }

        if ((data->parsing[data->depth] == JSON_OBJECT_VALUE_STRING || data->parsing[data->depth] == JSON_ARRAY_VALUE_STRING) && c == '\"') {
            data->value.end = data->pos;
            data->type = JSON_STRING;
            data->pos++;
            data->parsing[data->depth] = data->parsing[data->depth] == JSON_OBJECT_VALUE_STRING ? JSON_OBJECT_VALUE_AFTER : JSON_ARRAY_VALUE_AFTER;
            return JSON_RESULT_VALUE;
        }

        if (data->parsing[data->depth] == JSON_OBJECT_PROPERTY || data->parsing[data->depth] == JSON_OBJECT_VALUE_STRING || data->parsing[data->depth] == JSON_ARRAY_VALUE_STRING) {
            if (c == '\\') {
                if (data->pos + 1 < size) {
                    data->pos++;
                    c = json[data->pos];

                    switch (c) {
                        case '\"': case '/': case '\\': case 'b': case 'f': case 'r': case 'n': case 't': continue;
                    }

                    if (c == 'u') {
                        if (data->pos + 4 < size) {
                            uint16_t posPlus4 = data->pos + 4;

                            for (; data->pos <= posPlus4; data->pos++) {
                                c = json[data->pos];

                                if  (! ((c >= 48 && c <= 57) || (c >= 65 && c <= 70) || (c >= 97 && c <= 102))) {
                                    return JSON_RESULT_INVALID;
                                }
                            }
                        }
                    }
                }
            }
            else {
                continue;
            }
        }

        if (data->parsing[data->depth] == JSON_OBJECT_VALUE_NUMBER || data->parsing[data->depth] == JSON_ARRAY_VALUE_NUMBER) {
            switch (c) {
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': continue;
            }

            if (c == '.') {
                if (! number_dot_appeared) {
                    number_dot_appeared = 1;
                    continue;
                }
            }
        }

        if (data->parsing[data->depth] == JSON_OBJECT_PROPERTY_AFTER && c == ':') {
            data->parsing[data->depth] = JSON_OBJECT_VALUE_BEFORE;
            continue;
        }

        if ((data->parsing[data->depth] == JSON_OBJECT_VALUE_NUMBER || data->parsing[data->depth] == JSON_OBJECT_VALUE_AFTER || data->parsing[data->depth] == JSON_ARRAY_VALUE_NUMBER || data->parsing[data->depth] == JSON_ARRAY_VALUE_AFTER) && c == ',') {
            if (data->parsing[data->depth] == JSON_OBJECT_VALUE_NUMBER || data->parsing[data->depth] == JSON_ARRAY_VALUE_NUMBER) {
                data->value.end = data->pos - 1;
                data->type = JSON_NUMBER;
                data->parsing[data->depth] = data->parsing[data->depth] == JSON_OBJECT_VALUE_NUMBER ? JSON_OBJECT_VALUE_AFTER : JSON_ARRAY_VALUE_AFTER;
                return JSON_RESULT_VALUE;
            }

            data->parsing[data->depth] = data->parsing[data->depth] == JSON_OBJECT_VALUE_AFTER ? JSON_OBJECT_PROPERTY_BEFORE : JSON_ARRAY_VALUE_BEFORE;
            continue;
        }

        return JSON_RESULT_INVALID;
    }

    return JSON_RESULT_INVALID;
}
