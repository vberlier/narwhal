#ifndef NARWHAL_CONCAT_MACRO_H
#define NARWHAL_CONCAT_MACRO_H

#define __NARWHAL_CONCAT(A, B) A##B
#define _NARWHAL_CONCAT(A, B) __NARWHAL_CONCAT(A, B)

#endif