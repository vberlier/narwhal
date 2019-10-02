#ifndef NARWHAL_UNUSED_ATTRIBUTE_H
#define NARWHAL_UNUSED_ATTRIBUTE_H

#ifdef __GNUC__
#define _NARWHAL_UNUSED __attribute__((unused))
#else
#define _NARWHAL_UNUSED
#endif

#endif
