# Basic example

Getting started with Narwhal only requires a [single source file](https://github.com/vberlier/narwhal/blob/master/examples/basic/run_tests.c). You can compile the example by running `make`.

```bash
$ make
```

You can now run the generated executable.

```bash
$ ./run_tests
```

The test is checking whether the strings `"actual"` and `"expected"` are equal. The assertion is false, so running the executable reports that the test fails. The differences between the two strings are highlighted in a diff in the test output.
