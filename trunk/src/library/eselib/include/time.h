#ifndef __DATE_H__
#define __DATE_H__
#ifdef __cplusplus
extern "C" {
#endif


/* Fields are packed into successive bytes, each viewed as unsigned and
 * big-endian, unless otherwise noted:
 *
 *  0           second   1 byte, 0-INTMAX
 *  1           msecond  3 bytes, 0-1000
 */

#  define CLOCKS_PER_SEC  0l

typedef struct mini_timeval {
    uint16_t tv_sec;		/* Seconds.  */
    uint16_t tv_msec;	/* Millisecond.  */
} mini_timeval_t;



/* Macros for operations on mini_timevals.
   NOTE: `timercmp' does not work for >= or <=.  */
# define timerisset(tvp)	((tvp)->tv_sec || (tvp)->tv_msec)
# define timerclear(tvp)	((tvp)->tv_sec = (tvp)->tv_msec = 0)
# define timercmp(a, b, CMP) 						      \
  (((a)->tv_sec == (b)->tv_sec) ? 					      \
   ((a)->tv_msec CMP (b)->tv_msec) : 					      \
   ((a)->tv_sec CMP (b)->tv_sec))
# define timeradd(a, b, result)						      \
  do {									      \
    (result)->tv_sec = (a)->tv_sec + (b)->tv_sec;			      \
    (result)->tv_msec = (a)->tv_msec + (b)->tv_msec;			      \
    while ((result)->tv_msec >= 1000) {       \
    	++(result)->tv_sec;					  \
    	(result)->tv_msec -= 1000;			  \
      }									      \
  } while (0)
# define timersub(a, b, result)			      \
  do {									      \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;		\
    (result)->tv_msec = (a)->tv_msec - (b)->tv_msec;	\
    if ((result)->tv_msec < 0) {					    \
      --(result)->tv_sec;						      	\
      (result)->tv_msec += 1000;					    \
    }									      \
  } while (0)

#endif __DATE_H__
