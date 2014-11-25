#include "hal_common.h"
#include "hal_timer.h"

#define TX_CTL_TXSSEL ( BIT9 | BIT8 )
#define TX_CTL_MC     BIT5 | BIT4
#define TX_CCTL_CCIE  BIT4

#define TB_CTL_CNTL ( BIT12 | BIT11 )

void halTimer_a_initialize(UINT16 source, UINT16 mode)
{
    TACTL = 0;
    TACTL |= ( source & TX_CTL_TXSSEL );
    TACTL |= ( mode & TX_CTL_MC );
}

void halTimer_a_shutdown()
{
    halTimer_a_disableInterrupt();
    TACTL = 0;
}
