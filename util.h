#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

#define DEBUG

#ifdef DEBUG
    #define log(fmt,arg...) \
        printf("%s:%s:%u " fmt,__FILE__, __FUNCTION__, __LINE__, ##arg)
#else
    #define log(fmt,arg...)
#endif

#define max(a, b) (a < b ? b : a)


#endif /* end of include guard: UTIL_H */
