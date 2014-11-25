#ifndef _HAL_TIMER_H_
#define _HAL_TIMER_H_

#define TIMER_CNT_16 0                 // 0xFFFF
#define TIMER_CNT_12 BIT11             // 0xFFF
#define TIMER_CNT_10 BIT12             // 0x3FF
#define TIMER_CNT_8  ( BIT12 | BIT11 ) // 0xFF

#define TIMER_CLKSRC_ACLK  BIT8             // Alternative Clock
#define TIMER_CLKSRC_MCLK  ( BIT9 | BIT8 )  // Master Clock
#define TIMER_CLKSRC_SMCLK BIT9             // Subsystem Master Clock

#define TIMER_MODE_STOP       0
#define TIMER_MODE_UP         BIT4
#define TIMER_MODE_CONTINUOUS BIT5
#define TIMER_MODE_UPDOWN     ( BIT5 | BIT4 )

#define TIMER_CCR0    0  // 0000
#define TIMER_CCR1    2  // 0010
#define TIMER_CCR2    4  // 0100
#define TIMER_CCR3    6  // 0110
#define TIMER_CCR4    8  // 1000
#define TIMER_CCR5    10 // 1010
#define TIMER_CCR6    12 // 1100

void halTimer_a_initialize(UINT16 source, UINT16 mode);
void halTimer_a_shutdown();

#define halTimer_a_setTicks(x) \
	do { \
		TAR = 0; \
		TACCR0 = x; \
	} while (0)

#define halTimer_a_enableInterrupt()  TACCTL0 |= BIT4
#define halTimer_a_disableInterrupt() TACCTL0 &= ~BIT4

#endif
