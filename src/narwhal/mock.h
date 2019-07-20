#ifndef NARWHAL_MOCK_H
#define NARWHAL_MOCK_H

#define MOCK(function)        \
    ({                        \
        (void)function;       \
        narmock_##function(); \
    })

#endif
