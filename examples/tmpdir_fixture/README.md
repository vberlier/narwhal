# Temporary directory

This example demonstrates how you could create a fixture that makes a test run in a temporary directory. You can compile and execute the example by running the following command in the current directory.

```bash
$ make test
```

The test writes the string `"Hello, world!\n"` to a file called `data.txt` and checks whether the content of the file is equal to the string. The assertion is true so the test passes but notice how no `data.txt` file gets created in the current directory. This is because the `tmpdir` fixture is specified in the list of test modifiers, making the test run in a temporary directory.

You can remove the generated executable by running `make clean`.

```bash
$ make clean
```
