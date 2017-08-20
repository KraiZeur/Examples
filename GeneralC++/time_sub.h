/**
 * Behavior is result=a-b
 * call example : tsub(end,start,result)
 */
#include <time.h>

#define NS_SEC 1000000000
#define US_SEC 1000000

#define tsub(a, b, result) \
	do { \
		(result).tv_sec = (a).tv_sec - (b).tv_sec; \
		(result).tv_nsec = (a).tv_nsec - (b).tv_nsec; \
		if ((result).tv_nsec < 0) { \
		--(result).tv_sec; \
		(result).tv_nsec += NS_SEC; \
		} \
	} while (0)

	#define tadd(a, b, result) \
		do { \
			(result).tv_sec = (a).tv_sec + (b).tv_sec; \
			(result).tv_nsec = (a).tv_nsec + (b).tv_nsec; \
			} \
		} while (0)

#define asMicroSeconds(time) \
	do { \
		(time_t) ((time).tv_sec*1000000+(time).tv_nsec/1000); \
	} while (0)
