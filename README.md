# C JSON parser by Coderitter

A zero copy ultra small footprint JSON parser.

```c
#include <mbd-json.h>

int main() {
    char* json = "{\"property\":1}"

    // Initialize the json parser data struct
    json_data_t data;
    data.max_depth = 5;

    json_parsing_t parsing[5];
    data.parsing = parsing;

    json_start_end_t path[5];
    data.path = path;

    uint8_t value_counter[5];
    data.value_counter = value_counter;

    reset_json_data(&data);

    // Execute the parse function in a loop
    do {
        result = parse_json(json, strlen(json), &data);

        if (result == JSON_RESULT_INVALID) {
            printf("The given JSON string is not valid!");
            return 1;
        }

        if (result == JSON_RESULT_TOO_DEEP) {
            printf("The nesting of the given JSON string is too deep!");
            return 2;
        }

        if (jsonData.depth == 0) {
            if (compare("property", json, data.path[0].start + 1, data.path[0].end - 1)) {
                int value = atoi((const char*) &json[data.value.start]);
                printf("Property has the value = %i", value);
            }
        }
    }
    while (result == JSON_RESULT_VALUE);

    return 0;
}
```

## Initializing the json parser data struct

The struct `json_data` (also defined as type `json_data_t`) contains parser configuration settings, internal parser state and the information related to the found value.

Some of its information need to be stored for every nesting depth. To do so, arrays are used. Here you need to make a choice. How large should those arrays be? Or what is the maximum nesting depth of your application specific JSON formats? You need to set this value and you need to create the corresponding arrays accordingly. Then call the `reset_json_data` function to set the correct starting values.

```c
json_data_t data;
// Set the maximum allowed nesting depth
data.max_depth = 5;

// Initialize the corresponding arrays accordingly
json_parsing_t parsing[5];
data.parsing = parsing;

json_start_end_t path[5];
data.path = path;

uint8_t value_counter[5];
data.value_counter = value_counter;

// Set the correct starting values
reset_json_data(&data);
```

Here is a complete overview of the struct fields.

- `uint16_t pos`: The position in the given JSON string the parser is at.
- `int8_t max_depth`: The maximum allowed depth of nested JSON objects and arrays.
- `int8_t depth`: The nesting depth the parser is currently in. A nesting depth either refers to a JSON object or array.
- `json_parsing_t* parsing`: An array of the size of the maximum allowed nesting depth. It contains information about which JSON token is expected to come next and about which JSON token is being parsed right now. It is needed internally by the parser and not meant to be used by the user of the library.
- `json_start_end_t *path`: An array of the size of the maximum allowed nesting depth. It contains the path to the current JSON value. A path consists of positions of JSON array opening brackets `[` and positions of JSON property names inside the given JSON string.
- `json_start_end_t value`: The position of the current JSON property value inside the given JSON string.
- `json_type_t type`: The type of the current JSON value.
- `uint8_t *value_counter`: An array of the size of the maximum allowed nesting depth. When related to a JSON array it contains the amount of JSON array items which were found so far. When related to a JSON object it contains the amount of JSON properties which were found so far.

## The parsing

The `json_parse` function iterates through the given JSON bytes. It expects the following parameters.

- `const uint8_t* json`: The byte array which contains the JSON string.
- `const uint16_t size`: The size of the JSON byte array. Can be greater than the actual bytes of the JSON string.
- `json_data_t* data`: A struct which contains various parser data like configuration settings, internally used data and information about the current JSON value.

As soon it detects one of the following situations it returns a value of the enum `json_parse_result` (also defined as type `json_parse_result_t`) to the user.

- `JSON_RESULT_FINISH`: The parser successfully finished parsing the JSON.
- `JSON_RESULT_VALUE`: A JSON value was found. A value is either associated to an object property or part of an array item.
- `JSON_RESULT_INVALID`: The parsed JSON is not valid.
- `JSON_RESULT_TOO_DEEP`: The JSON objects and arrays are nested too deep.

When you receive a `JSON_RESULT_INVALID` or a `JSON_RESULT_TOO_DEEP`, you were not able to parse the JSON successfully. If you receive a `JSON_RESULT_VALUE`, the data struct, which was given as a parameter, will contain information about the just found JSON value which you now can evaluate.

Once the found JSON value was evaluated, you call `json_parse` again, up until the function returns a `JSON_RESULT_FINISH` value. If programmed like in the following example you will only need to take care of three result values.

```c
do {
    result = parse_json(json, strlen(json), &data);

    if (result == JSON_RESULT_INVALID) {
        printf("The given JSON string is not valid!");
        break;
    }

    if (result == JSON_RESULT_TOO_DEEP) {
        printf("The nesting of the given JSON string is too deep!");
        break;
    }

    // Evaluate the current JSON value with the help of the parser data struct
    }
}
while (result == JSON_RESULT_VALUE);
```

## Parse the root element

At first you will want to find out if the root JSON element is either an object or an array.

```c
result = parse_json(json, strlen(json), &data);

if (result == JSON_RESULT_INVALID) {
    printf("The given JSON string is not valid!");
}

// If the result is not JSON_RESULT_INVALID it will be JSON_RESULT_VALUE
// (When you have set the maximum allowed nesting depth to at least 1)

if (data.depth == 0 && data.type == JSON_OBJECT_START) {
    printf("The given root element of the given JSON string is an object!");
}

if (data.depth == 0 && data.type == JSON_ARRAY_START) {
    printf("The given root element of the given JSON string is an array!");
}
```

The value of depth is `0`, which means the parser it outside of any JSON code. As soon as the first JSON values are found which are inside the root object or array, the depth will have a value of `1`.

## Parse JSON properties

To parse specific JSON properties, you need to know the following things.

1. The nesting depth in which the JSON property is to be found
2. The name of the JSON property
3. The fact that you are parsing a certain object in which the desired property resides in

As for the nesting depth, every nested JSON object or JSON array increase the nesting depth number.

```json
{
    "name": "Arne",
    "address": {
        "city": "Dresden"
    },
    "friends": [
        "Ramon",
        {
            "name": "Henrik"
        }
    ]
}
```

Property `name` of the root object is depth 0. Property `city` is depth 1. The value `Ramon` inside the `friends` array is depth 1. The `name` property inside the object in the `friends` array is depth 2.

The name of the property can be found in the `path` field of the parser data struct. It contains the property name positions for every depth. In the case of the property `name`, it would contain one element with the start and end positions to `"name"` in the JSON string, including the `"` characters. In the case of the property `city`, it would contain two entries. The first one points to the position of `"address"`. The second one pointing to the position of `"city"`.

Now you know how to obtain the needed information. Let us look at some code.

```c
if (data.depth == 0 && compare("name", json, data.path[0].start + 1, data.path[0].end - 1)) {
    printf("Property 'name' in depth 0 detected!");
}

if (data.depth == 1 &&
    compare("address", json, data.path[0].start + 1, data.path[0].end - 1) &&
    compare("city", json, data.path[1].start + 1, data.path[1].end - 1)
) {
    printf("Property 'address.city' in depth 1 detected!");
}
```

To compare a given string with a sub string inside an byte array, you can use the function `compare`.

## Parse JSON values

The parser data struct contains a field `value` which contains the start and end position of the value inside the given JSON string. It also contains a field `type` which designates the type of the value. You need to extract the string values from the byte array and convert them into something that you application wants to work with.

Let us have a look at how to do it for the primitive JSON value types `string`, `number`, `boolean` and `null`.

```c
char name[31]; // Name can be at most 30 characters long plus one null terminator
long birth_year;
bool female;
title_t title;

if (data.depth == 0 && compare("name", json, data.path[0].start + 1, data.path[0].end - 1)) {
    if (data.type == JSON_STRING) {
        memcpy(name, json[data.value.start + 1], data.value.end - data.value.start - 1);
        name[data.value.end - data.value.start] = 0;
    }
}

if (data.depth == 0 && compare("birthYear", json, data.path[0].start + 1, data.path[0].end - 1)) {
    if (data.type == JSON_NUMBER) {
        long birth_year_long = strtol((char*) &json[data.value.start], data.value.end - data.value.start);
    }

if (data.depth == 0 && compare("gender", json, data.path[0].start + 1, data.path[0].end - 1)) {
    if (data.type == JSON_BOOLEAN) {
        female = json[data.value.start] == 't' ? true : false;
    }

if (data.depth == 0 && compare("title", json, data.path[0].start + 1, data.path[0].end - 1)) {
    if (data.type == JSON_NULL) {
        title = TITLE_NONE;
    }
}
```

It is also possible to detect the start and end of object and of array values. Beware that only when the parser has detected the end it is able to tell you then end position of the complete object or array value.

```c
if (compare("address", json, data.path[0].start + 1, data.path[0].end - 1)) {
    if (data.depth == 1 && data.type == JSON_OBJECT_START)
        // data.value.end == 0 since the end of the object is not known yet!
    }

    if (data.depth == 0 && data.type == JSON_OBJECT_END) {
        // data.value.end now has the correct value!
    }
}

if (compare("friends", json, data.path[0].start + 1, data.path[0].end - 1)) {
    if (data.depth == 1 && data.type == JSON_ARRAY_START) {
        // data.value.end == 0 since the end of the object is not known yet!
    }

    if (data.depth == 0 && data.type == JSON_ARRAY_END) {
        // data.value.end now has the correct value!
    }
}
```

When you receive `JSON_OBJECT_START` or `JSON_ARRAY_START`, then the value of the `depth` field holds the depth of that found object or array. The other way around, when you receive `JSON_OBJECT_END` or `JSON_ARRAY_END`, the `depth` points to the object or array above.

### Parse JSON arrays

To be able to successfully parse the content of an array, you need to know that you are inside one. The field `path` of the parser data struct will contain this information. If will contain the position of the opening brackets `[` of the current array.

Let us have a look at an example handling a primitive typed value, in that case the name of a friend as a `string`.

```c
char friends[10][31]; // Ten friends each having a name of maximum 30 characters plus one null terminator
int friend_count = 0;

if (data.depth == 1 &&
    compare("friends", json, data.path[0].start + 1, data.path[0].end - 1) &&
    json[data.path[1].start] == '['
) {
    if (data.type == JSON_STRING) {
        memcpy(name[friend_count], json[data.value.start + 1], data.value.end - data.value.start - 1);
        name[friend_count][data.value.end - data.value.start] = 0;
        friend_count++;
    }
}
```

Here is an example of how to deal with objects inside of arrays.

```c
char friends[10][31]; // Ten friends each having a name of maximum 30 characters plus one null terminator
int friend_count = 0;

if (data.depth == 1 &&
    compare("friends", json, data.path[0].start + 1, data.path[0].end - 1) &&
    json[data.path[1].start] == '['
) {
    if (data.depth == 3 &&
        compare("name", json, data.path[2].start + 1, data.path[2].end - 1)
    ) {
        if (data.type == JSON_STRING) {
            memcpy(name[friend_count], json[data.value.start + 1], data.value.end - data.value.start - 1);
            name[friend_count][data.value.end - data.value.start] = 0;
            friend_count++;
        }
    }
}
```

## Build C JSON Parser
Create build environment folder and move into
```
mkdir build && cd build
```

Call cmake to generate make files and call make for building lib:
```
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

To install lib package to default path `/usr/local` call make again:
```
make install
```

To install lib package to a custom folder, e.g. into an installation folder within the project folder, call cmake like this:
```
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${PWD}/../install
make
make install
```

This will change the cmake install prefix to `$PROJECT_ROOT/install` where now the lib package will be installed.

## Link and use CMake library package

In CMake projects the library package has to be known to the `CMAKE_PREFIX_PATH`.
This is the case, if it is installed to a `$PATH`-known location like `/usr/local`
or if the library installation path is added manually to `CMAKE_PREFIX_PATH` within the projects CMakeLists.txt.

This can be done like this:
```
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "<PATH_TO_MBD_JSON_INSTALL_DIR>")
```

To use mbd-json lib in cmake project, call within your CMakeLists.txt:
```
find_package(mbd-json REQUIRED)
target_link_libraries(${TargetName} mbd-json::mbd-json)
```