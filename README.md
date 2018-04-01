# Unicorn

> A magical unit testing framework written in C.

Unicorn makes it easy to test C programs and libraries by providing a very simple and straight-forward API. The framework strives to reduce boilerplate code as much as possible to make the process of writing tests more enjoyable.

**:warning: Disclaimer :warning:**

This project is experimental. I'm relatively new to the C ecosystem, and I initially only developed Unicorn for personal use. The reason why I'm open-sourcing it is because it's actually been working pretty well for me so far.

```c
#include <unicorn/unicorn.h>

TEST(example)
{
    int result = 2 + 3;
    ASSERT_EQ(result, 5);
}

int main()
{
    return RUN_TESTS(example);
}
```

## Features

- Nested test groups
- Test parameters
- Test fixtures
- Generic assertions
- Pretty output

## Installation

First, you'll need to download the source code.

```bash
$ git clone git@github.com:vberlier/unicorn.git
```

Now, compile the project with `make`.

```bash
$ make
```

Once the project is compiled, you're now ready to install Unicorn. The preferred way of installing it would be to install it on a per-project basis. Using the `DESTDIR` variable, you can specify where Unicorn should be installed.

```bash
$ make install DESTDIR=~/my_project
```

By using the command just above, `make` will copy the `libunicorn.so` library in `~/my_project/lib/` and the necessary headers in `~/my_project/include`.

You can also install Unicorn globally if you need to. The default destination is `/usr` so you'll need to run the command with the appropriate permissions.

```bash
$ sudo make install
```

## Uninstall

You can uninstall Unicorn using `make uninstall`. Just like in the installation process, use the `DESTDIR` variable to specify from where the Unicorn library and the related headers should be removed.

```bash
$ make uninstall DESTDIR=~/my_project
```

Once again, the default destination is `/usr` so you'll need to run the command with the appropriate permissions if you want to uninstall Unicorn globally.

```bash
$ sudo make uninstall
```

## Contributing

Contributions are welcome. I'm by no means an expert with C, so feel free to open an issue if you're having troubles or if you feel like something could be done differently.

The test suite for Unicorn is built with Unicorn. It's currently far from complete but you can run it with `make test`. If you set the `DEBUG` variable to `1`, the test executable will be compiled with AddressSanitizer enabled.

```bash
$ make test DEBUG=1
```

---

License - [MIT](https://github.com/vberlier/unicorn/blob/master/LICENSE)
