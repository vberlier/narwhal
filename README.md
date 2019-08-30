# ⚓️ narwhal

[![Build Status](https://travis-ci.com/vberlier/narwhal.svg?branch=master)](https://travis-ci.com/vberlier/narwhal)
![GitHub tag (latest SemVer)](https://img.shields.io/github/tag/vberlier/narwhal.svg)

<img align="right" width="56%" src="https://raw.githubusercontent.com/vberlier/narwhal/master/examples/demo.svg?sanitize=true">

> The testing unicorn of the C.

Narwhal is a framework that makes it easy to write readable and maintainable tests for C programs and libraries.

```c
#include "narwhal.h"

TEST(example)
{
    ASSERT_EQ("actual", "expected");
}

int main()
{
    return RUN_TESTS(example);
}
```

> Check out the [basic example](https://github.com/vberlier/narwhal/tree/master/examples/basic) for more details about this code snippet.

## Features

- Organize tests in arbitrarily nested groups
- Use the same generic assertions everywhere
- Assertion failures reported as diffs
- Test output captured and displayed on failure
- Create, combine and reuse test parameters
- Externalize and compose setup and teardown code with fixtures
- Easy-to-use resource management and output capturing utilities
- Amalgamated source file and header ready to drop in your project
- Mocking support with [Narmock](https://github.com/vberlier/narmock)
- Works well with errors reported by sanitizers

## Installation

Narwhal is distributed as an [amalgamated](https://sqlite.org/amalgamation.html) source file and header:

- [`narwhal.c`](https://raw.githubusercontent.com/vberlier/narwhal/master/dist/narwhal.c)
- [`narwhal.h`](https://raw.githubusercontent.com/vberlier/narwhal/master/dist/narwhal.h)

Drop the two files in your project, make sure `narwhal.c` is compiled and linked just like your other source files and you should be good to go.

<details>
<summary>
Narwhal can also be installed as a shared library. <strong>Click to see more</strong>
</summary>

### Building and installing the shared library

First, you'll need to download the source code.

```bash
$ git clone git://github.com/vberlier/narwhal.git && cd narwhal
```

You can now install the project with `make install`. The default destination is `/usr` so you'll need to run the command with the appropriate permissions if you want to install Narwhal globally.

```bash
$ sudo make install
```

You can use the `DESTDIR` variable to specify where Narwhal should be installed.

```bash
$ make install DESTDIR=~/my_project
```

This command will copy the `libnarwhal.so` library to `~/my_project/lib/` and the necessary headers to `~/my_project/include`.

### Uninstalling the shared library

You can uninstall Narwhal with `make uninstall`. The `DESTDIR` variable lets you specify from where the Narwhal library and the related headers should be removed.

```bash
$ make uninstall DESTDIR=~/my_project
```

The default destination is `/usr` so you'll need to run the command with the appropriate permissions if you want to uninstall a global Narwhal installation.

```bash
$ sudo make uninstall
```

</details>

## Framework overview

### Defining tests

Narwhal lets you define tests using the `TEST` macro. The first argument is the name of the test. Note that the test name must be a valid identifier. The macro invocation should be followed by the test body, defined between curly braces.

```c
TEST(example)
{
    // Test body
}
```

The test body is simply a function body in which you can write your test.

As your test suite grows, you'll need to split your test definitions across multiple files to keep everything organized. You can use the `DECLARE_TEST` macro in order to declare tests in header files.

```c
// test_example.h

#ifndef TEST_EXAMPLE_H
#define TEST_EXAMPLE_H

#include "narwhal.h"

DECLARE_TEST(example);

#endif
```

```c
// test_example.c

#include "narwhal.h"

TEST(example)
{
    // Test body
}
```

### Grouping related tests together

You can define test groups for grouping related tests together with the `TEST_GROUP` macro. The first argument of the macro is the name of the test group. Note that the group name must be a valid identifier. The second argument of the macro must be an array literal where each element is either a test or a test group. This means that you can put test groups inside of other test groups.

```c
TEST(example1) {}
TEST(example2) {}

TEST_GROUP(example_group, { example1, example2 });
```

In order to declare test groups inside of header files you'll need to use the `DECLARE_GROUP` macro.

```c
// test_example.h

#ifndef TEST_EXAMPLE_H
#define TEST_EXAMPLE_H

#include "narwhal.h"

DECLARE_GROUP(example_group);

#endif
```

```c
// test_example.c

#include "narwhal.h"

TEST(example1) {}
TEST(example2) {}

TEST_GROUP(example_group, { example1, example2 });
```

### Running tests and test groups

Narwhal defines the `RUN_TESTS` macro. It lets you specify a list of tests and test groups to run and will return `EXIT_SUCCESS` if all the tests passed and `EXIT_FAILURE` otherwise.

```c
TEST(foo) {}
TEST(bar) {}

TEST_GROUP(example_group, {});

int main()
{
    return RUN_TESTS(foo, bar, example_group);
}
```

### Using assertions

Narwhal defines a few macros that are meant to be used inside of tests to report failures. The most basic one is `FAIL`. It simply notifies Narwhal that the test failed and stops the test execution. You can optionally include an error message and use formatting to provide more details.

```c
TEST(example1)
{
    /* ... */

    if (result != 42)
    {
        FAIL();  // The test execution stops and Narwhal reports a failure
    }
}

TEST(example2)
{
    /* ... */

    if (result != 42)
    {
        FAIL("The result should be 42.");
    }
}

TEST(example3)
{
    /* ... */

    if (result != 42)
    {
        FAIL("The result should be 42 but got %d.", result);
    }
}
```

In practice, you might not want to use `FAIL` directly unless the way you determine that the test has failed doesn't rely on a meaningful expression. Most of the time, you'll actually use an assertion macro. The `ASSERT` macro essentially replaces the `if (!assertion) FAIL()` construct. The first argument of the macro should be the assertion that needs to be true for the test to succeed. If the assertion evaluates to false, Narwhal will report a failure and stop the test execution. You can also include an error message with formatting after the assertion.

```c
TEST(example1)
{
    /* ... */

    ASSERT(result == 42);
}

TEST(example2)
{
    /* ... */

    ASSERT(result == 42, "The result should be 42.");
}

TEST(example3)
{
    /* ... */

    ASSERT(result == 42, "The result should be 42 but got %d.", result);
}
```

If the assertion is a simple equality check, you can let Narwhal perform the comparison and format the error message for you by using the `ASSERT_EQ` macro. The macro is generic and works with most signed and unsigned integers of various sizes, floats and doubles. It can compare pointers and if the arguments are strings, it will check that they are identical using `strcmp`. Upon failure, Narwhal will display the values of both the expected and the actual result.

```c
TEST(example)
{
    /* ... */

    ASSERT_EQ(result, 42);
}
```

The `ASSERT_NE` macro can be used to perform the opposite check.

```c
TEST(example)
{
    /* ... */

    ASSERT_NE(result, -1);
}
```

In addition, four basic comparison assertion macros are also available: `ASSERT_LT`, `ASSERT_LE`, `ASSERT_GT` and `ASSERT_GE` standing respectively for _less than_, _less than or equal to_, _greater than_ and _greater than or equal to_.

```c
TEST(example)
{
    /* ... */

    ASSERT_LT(result, 43);
    ASSERT_LE(result, 42);
    ASSERT_GT(result, 41);
    ASSERT_GE(result, 42);
}
```

Narwhal also provides the `ASSERT_SUBSTRING` and `ASSERT_NOT_SUBSTRING` macros. They both take two strings as parameters and check that the first one contains or doesn't contain the second respectively.

```c
TEST(example)
{
    /* ... */

    ASSERT_SUBSTRING(long_string, "Hello, world!");
    ASSERT_NOT_SUBSTRING(long_string, "An error occurred");
}
```

### Adding parameters to tests

Running a test with various different inputs can be quite useful. Instead of duplicating the test and only changing some hard-coded values, you can let Narwhal run your test several times with different inputs by using a test parameter. You can create a test parameter with the `TEST_PARAM` macro. The first argument of the macro is the name of the test parameter. It must be a valid identifer. The second argument is the type of the parameter. The last argument must be an array literal that contains all the values that you want the parameter to take.

```c
TEST_PARAM(input_number, int, { 0, -1, 8, 42 });
```

In order to apply a parameter to a test, you must include it in the list of test modifiers. The test modifiers are specified right after the test name in a test definition.

```c
TEST(example, input_number)
{
    // The test will run for each possible value of the parameter
}
```

If you specify multiple test parameters in the list of test modifiers, the test will run for every possible combination.

```c
TEST_PARAM(input1, char *, { "one", "two" });
TEST_PARAM(input2, int, { 1, 2, 3, 4 });

TEST(example, input1, input2)
{
    // The test will run with 8 different parameter combinations
}
```

In order to have access to the current value of a parameter inside of the test body, you'll need to use the `GET_PARAM` macro. The only argument of the macro is the name of the parameter that you want to bring in scope.

```c
TEST(example, input1, input2)
{
    GET_PARAM(input1);
    GET_PARAM(input2);

    // The current value of each parameter can now be used in the test
}
```

If you want to declare a test parameter inside of a header file, you'll need to use the `DECLARE_PARAM` macro. The first argument is the name of the parameter and the second one is the type.

```c
// input_number.h

#ifndef INPUT_NUMBER_H
#define INPUT_NUMBER_H

#include "narwhal.h"

DECLARE_PARAM(input_number, int);

#endif
```

```c
// input_number.c

#include "narwhal.h"

TEST_PARAM(input_number, int, { 0, -1, 8, 42 });
```

### Using test fixtures

Test fixtures let you provide data to a test from the outside. This allows you to keep the body of the test focused on making assertions instead of executing setup and teardown code. In addition, by extracting cleanup instructions outside of the test, you can be sure that they will be executed even if the test fails. Narwhal fixtures are inspired by [pytest](https://docs.pytest.org/en/latest/fixture.html).

In order to define a test fixture, you'll need to use the `TEST_FIXTURE` macro. The first argument of the macro is the name of the fixture. The name must be a valid identifier. The second argument is the type of the fixture. The macro invocation must be followed by the fixture body, defined between curly braces.

```c
TEST_FIXTURE(number, int)
{
    *number = 42;
}
```

The fixture body is simply a function body in which you can set the value of the fixture. Narwhal allocates the necessary memory according to the type you specified. You can then initialize this memory with the provided pointer.

You can additionally specify cleanup instructions using the `CLEANUP_FIXTURE` macro. It must be invoked at the end of the fixture body and the first argument must be the name of the fixture. The invocation should be followed by a code block in which you'll be able to put your cleanup code.

```c
TEST_FIXTURE(int_array, int *)
{
    size_t count = 10;
    *int_array = malloc(count * sizeof(int));

    for (size_t i = 0; i < count; i++)
    {
        (*int_array)[i] = i;
    }

    CLEANUP_FIXTURE(int_array)
    {
        free(*int_array);
    }
}
```

In order to apply a fixture to a particular test, you'll need to specify the name of the fixture inside of the list of test modifiers.

```c
TEST(example, int_array)
{
    // Narwhal will execute the necessary setup and teardown code
}
```

To have access to the value of the fixture, you'll need to use the `GET_FIXTURE` macro. The only argument of the macro is the name of the fixture that you want to bring in scope.

```c
TEST(example, int_array)
{
    GET_FIXTURE(int_array);

    // The value of the fixture can now be used in the test
}
```

If you want to declare a test fixture inside of a header file, you'll need to use the `DECLARE_FIXTURE` macro. The first argument is the name of the fixture and the second one is the type.

```c
// number.h

#ifndef NUMBER_H
#define NUMBER_H

#include "narwhal.h"

DECLARE_FIXTURE(number, int);

#endif
```

```c
// number.c

#include "narwhal.h"

TEST_FIXTURE(number, int)
{
    *number = 42;
}
```

You can check out the [tmpdir fixture example](https://github.com/vberlier/narwhal/tree/master/examples/tmpdir_fixture) for an example of a more practical fixture.

Fixtures are really similar to actual tests as they allow you to use assertions and test modifiers. Assertions make it possible to abort test execution if there's a problem during setup or teardown code. If the setup code fails, Narwhal will report the error right away without attempting to execute the test itself.

```c
TEST_FIXTURE(text_file, FILE *)
{
    *text_file = fopen("foo.txt", "r");
    ASSERT(*text_file != NULL, "Couldn't open \"foo.txt\".");

    CLEANUP_FIXTURE(text_file)
    {
        fclose(*text_file);
    }
}
```

You can apply test modifiers by including them in the list of modifiers right after the type of the fixture. You'll need to use the `GET_PARAM` and `GET_FIXTURE` macros if you want to access the value of a modifier inside of the fixture body.

```c
TEST_PARAM(filename, char *, { "foo.txt", "bar.txt" });

TEST_FIXTURE(text_file, FILE *, filename)
{
    GET_PARAM(filename);

    *text_file = fopen(filename, "r");
    ASSERT(*text_file != NULL, "Couldn't open \"%s\".", filename);

    CLEANUP_FIXTURE(text_file)
    {
        fclose(*text_file);
    }
}
```

Note that when a fixture with modifiers is applied to a test, all the modifiers are registered on the test itself. For each test, Narwhal recursively resolves all the parameters and fixtures that are being used and applies them directly to the test. If several fixtures all require a particular modifier, they will share the same instance.

### Managing test resources

Narwhal can take care of freeing memory for you at the end of a test. You can register a pointer to be automatically freed by using the `auto_free()` function. This allows you to eliminate calls to `free()` from the end of your tests and ensures that no matter the outcome of the test, the allocated memory is always released.

```c
TEST(example)
{
    char *buffer = malloc(256);
    auto_free(buffer);

    // The allocated memory will be released at the end of the test
}
```

You can also use the `test_resource()` function instead of `malloc()` if you don't want to worry about calling `auto_free()` after allocating the memory.

```c
TEST(example)
{
    char *buffer = test_resource(256);

    // The allocated memory will be released at the end of the test
}
```

It's worth mentioning that letting Narwhal release memory for you can often eliminate the need for cleanup code in fixtures.

### Capturing `stdout` and `stderr`

Sometimes, you might want to write tests to ensure that what your code prints out is correct. To help with that, Narwhal provides a utility that makes it easy to capture the output of your code. The `CAPTURE_OUTPUT` macro lets you collect the combined output of `stdout` and `stderr` in a string. The only argument of the macro is an identifier that will be used to declare the string containing the output of your code. The macro invocation should then be followed by a code block.

```c
TEST(example)
{
    CAPTURE_OUTPUT(message)
    {
        printf("Hello, world!\n");
    }

    ASSERT_EQ(message, "Hello, world!\n");
}
```

The output of the code that runs inside of the code block is redirected and collected in the output buffer as a string. You don't need to free the buffer created by the macro. The allocated memory will be automatically released at the end of the test by Narwhal's resource management utilities.

Note that combining `CAPTURE_OUTPUT` with `ASSERT_SUBSTRING` makes it very easy to analyse the output of your code.

```c
TEST(example)
{
    CAPTURE_OUTPUT(code_output)
    {
        /* ... */
    }

    ASSERT_SUBSTRING(code_output, "Success");
}
```

### Mocking

You can mock functions with [Narmock](https://github.com/vberlier/narmock), a companion mocking utility that can also be used on its own.

```c
#include <time.h>

#include "__mocks__.h"
#include "narwhal.h"

TEST(example)
{
    MOCK(time)->mock_return(42);
    ASSERT_EQ(time(NULL), 42);
}
```

> Check out the [repository](https://github.com/vberlier/narmock) for more details.

## Contributing

Contributions are welcome. Feel free to open issues and suggest improvements.

The test suite is built with Narwhal itself. You can run it with `make test`. You can set the `DEBUG` variable to `1` to compile the test executable with AddressSanitizer enabled.

```bash
$ make test DEBUG=1
```

---

License - [MIT](https://github.com/vberlier/narwhal/blob/master/LICENSE)
