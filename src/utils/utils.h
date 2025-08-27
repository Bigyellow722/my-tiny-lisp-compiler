#ifndef _UTILS_H_
#define _UTILS_H_
#define __ALIGN_UP_MASK(x, mask) (((x) + (mask)) & ~(mask))
#define __ALIGN_UP(x, a) __ALIGN_UP_MASK(x, (typeof(x))(a) - 1)
#define ALIGN_UP(x, a) __ALIGN_UP((x), (a))
#define __ALIGN_DOWN_MASK(x, mask) ((x) & ~(mask))
#define __ALIGN_DOWN(x, a) __ALIGN_DOWN_MASK(x, (typeof(x))(a) - 1)
#define ALIGN_DOWN(x, a) __ALIGN_DOWN((x), (a))
#endif
