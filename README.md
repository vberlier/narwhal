# Unicorn

> A magical unit testing framework written in C.

Unicorn makes it easy to test C programs and libraries by providing a very simple and straight-forward API. Unicorn also strives to reduce boilerplate code as much as possible to make the process of writing tests more enjoyable.

```c
#include "unicorn/unicorn.h"

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
- Generic assertion
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

---

License - [MIT](https://github.com/vberlier/unicorn/blob/master/LICENSE)
