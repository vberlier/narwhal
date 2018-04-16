# Basic example

This is the most basic usage of Unicorn possible. To run this example, Unicorn should be installed globally. Check out the [`run_tests.c`](https://github.com/vberlier/unicorn/blob/master/examples/basic/run_tests.c) file for a very simple introduction to Unicorn.

You can compile this example by running `make`.

```bash
$ make
```

Then, run the example with the generated executable.

```bash
$ ./run_tests
```

The test is adding `2` and `3` together instead of multiplying them. This causes the assertion to be false, so running the executable will report that the test fails.

![Test output](test_output.png)
