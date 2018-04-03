
/*
 * The first thing that we need to do in order to use Unicorn is to
 * include the <unicorn/unicorn.h> header. This will declare all the
 * necessary functions and define a bunch of helpful macros.
 */

#include <unicorn/unicorn.h>


/*
 * In order to create a test with Unicorn, you have to use the `TEST`
 * macro. You'll need to give your test a name and a test body. The name
 * of your test must be a valid identifier.
 * The test body is just a function body. You can write code here and it
 * will get executed when the test runs. Inside of the test body, you
 * use assertions defined by Unicorn to check that your code behaves as
 * expected. For instance, the macro called `ASSERT_EQ` will check that
 * the first argument is equal to the second one. If the two arguments
 * are different, the assertion will stop the test and report a failure.
 * `ASSERT_EQ` is a generic assertion and can compare a lot of different
 * types together. For instance, it supports strings, integers, floats
 * doubles, unsigned integers etc.
 */

TEST(example)
{
    int result = 2 + 3;
    ASSERT_EQ(result, 5);
}


/*
 * To run our test, we need to use the `RUN_TESTS` macro. The macro
 * can take multiple tests as arguments and will return `EXIT_SUCCESS`
 * if all of them passed. The return value will be `EXIT_FAILURE`
 * otherwise.
 */

int main()
{
    return RUN_TESTS(example);
}
