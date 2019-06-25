# Basic example

Getting started with Narwhal only requires a [single source file](https://github.com/vberlier/narwhal/blob/master/examples/basic/run_tests.c). You can compile and execute the example by running `make test`.

```bash
$ make test
```

The test is checking whether the strings `"actual"` and `"expected"` are equal. The assertion is false, so running the executable reports that the test fails. The differences between the two strings are highlighted in a diff in the test output.

You can remove the generated executable by running `make clean`.

```bash
$ make clean
```
