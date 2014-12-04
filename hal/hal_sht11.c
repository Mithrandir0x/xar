/*
 * Device driver for the Sensirion SHT1x/SHT7x family of humidity and
 * temperature sensors.
 */

//#include <stdio.h>
//#include <io.h>
#include "hal_common.h"
#include "hal_sht11.h"
//#include "sht11-arch.h"

#define SDA_0()   (SHT11_PxDIR |=  BM(SHT11_ARCH_SDA))	/* SDA Output=0 */
#define SDA_1()   (SHT11_PxDIR &= ~BM(SHT11_ARCH_SDA))	/* SDA Input */
#define SDA_IS_1  (SHT11_PxIN & BM(SHT11_ARCH_SDA))

#define SCL_0()   (SHT11_PxOUT &= ~BM(SHT11_ARCH_SCL))	/* SCL Output=0 */
#define SCL_1()   (SHT11_PxOUT |=  BM(SHT11_ARCH_SCL))	/* SCL Output=1 */

				/* adr   command  r/w */
#define  STATUS_REG_W   0x06	/* 000    0011    0 */
#define  STATUS_REG_R   0x07	/* 000    0011    1 */
#define  MEASURE_TEMP   0x03	/* 000    0001    1 */
#define  MEASURE_HUMI   0x05	/* 000    0010    1 */
#define  RESET          0x1e	/* 000    1111    0 */

/* This can probably be reduced to 250ns according to data sheet. */
#define delay_400ns() halWait(100)

/*---------------------------------------------------------------------------*/
void sstart(void)
{
	SDA_1(); SCL_0();
	delay_400ns();
	SCL_1();
	delay_400ns();
	SDA_0();
	delay_400ns();
	SCL_0();
	delay_400ns();
	SCL_1();
	delay_400ns();
	SDA_1();
	delay_400ns();
	SCL_0();
}
/*---------------------------------------------------------------------------*/
void sreset(void)
{
	UINT8 i;

	SDA_1();
	SCL_0();
	for(i = 0; i < 9 ; i++) {
		SCL_1();
		delay_400ns();
		SCL_0();
	}
	sstart();			/* Start transmission, why??? */
}
/*---------------------------------------------------------------------------*/
/*
 * Return true if we received an ACK.
 */
UINT8 swrite(UINT8 _c)
{
	UINT8 c = _c;
	UINT8 i;
	UINT8 ret;

	for(i = 0; i < 8; i++, c <<= 1) {
		if(c & 0x80) {
			SDA_1();
		} else {
			SDA_0();
		}
		SCL_1();
		delay_400ns();
		SCL_0();
	}

	SDA_1();
	SCL_1();
	delay_400ns();
	ret =  !(SDA_IS_1);

	SCL_0();

  return ret;
}
/*---------------------------------------------------------------------------*/
UINT8 sread(UINT8 send_ack)
{
	UINT8 i;
	UINT8 c = 0x00;

	SDA_1();
	for(i = 0; i < 8; i++) {
		c <<= 1;
		SCL_1();
		delay_400ns();
		if(SDA_IS_1) {
			c |= 0x1;
		}
		SCL_0();
	}

	if(send_ack) {
		SDA_0();
	}
	SCL_1();
	delay_400ns();
	SCL_0();

	SDA_1();			/* Release SDA */

	return c;
}
/*---------------------------------------------------------------------------*/
#define CRC_CHECK
#ifdef CRC_CHECK
UINT8 rev8bits(UINT8 v)
{
	unsigned char r = v;
	int s = 7;

	for (v >>= 1; v; v >>= 1) {
		r <<= 1;
		r |= v & 1;
		s--;
	}
	r <<= s;		    /* Shift when v's highest bits are zero */
	return r;
}
/*---------------------------------------------------------------------------*/
/* BEWARE: Bit reversed CRC8 using polynomial ^8 + ^5 + ^4 + 1 */
UINT8 crc8_add(UINT8 acc, UINT8 byte)
{
	UINT8 i;

	acc ^= byte;
	for(i = 0; i < 8; i++) {
		if(acc & 0x80) {
			acc = (acc << 1) ^ 0x31;
		} else {
			acc <<= 1;
		}
	}
	return acc & 0xff;
}
#endif /* CRC_CHECK */
/*---------------------------------------------------------------------------*/
/*
 * Power up the device. The device can be used after an additional
 * 11ms waiting time.
 */
void sht11_init(void)
{
	/*
	* SCL Output={0,1}
	* SDA 0: Output=0
	*     1: Input and pull-up (Output=0)
	*/

	SHT11_PxOUT |= BM(SHT11_ARCH_PWR);
	SHT11_PxOUT &= ~(BM(SHT11_ARCH_SDA) | BM(SHT11_ARCH_SCL));
	SHT11_PxDIR |= BM(SHT11_ARCH_PWR) | BM(SHT11_ARCH_SCL);
}
/*---------------------------------------------------------------------------*/
/*
 * Power of device.
 */
void sht11_off(void)
{
	SHT11_PxOUT &= ~BM(SHT11_ARCH_PWR);
	SHT11_PxOUT &= ~(BM(SHT11_ARCH_SDA) | BM(SHT11_ARCH_SCL));
	SHT11_PxDIR |= BM(SHT11_ARCH_PWR) | BM(SHT11_ARCH_SCL);
}
/*---------------------------------------------------------------------------*/
/*
 * Only commands MEASURE_HUMI or MEASURE_TEMP!
 */
UINT16 scmd(UINT8 cmd)
{
	UINT32 n;
	UINT16 t;

	if(cmd != MEASURE_HUMI && cmd != MEASURE_TEMP) {
		return FALSE;
	}

	sstart();			/* Start transmission */


	if(!swrite(cmd)){	/* Write command */
    	//goto fail;
		sreset();
		return FALSE;
	}

	for(n = 0; n < 250000; n++) {
		if(!SDA_IS_1) {
			UINT8 t0, t1;//, rcrc;
			t0 = sread(1);
			t1 = sread(1);
			//rcrc = sread(0);
			/*
#ifdef CRC_CHECK
      {
	unsigned crc;
	crc = crc8_add(0x0, cmd);
	crc = crc8_add(crc, t0);
	crc = crc8_add(crc, t1);
	if(crc != rev8bits(rcrc)) {
	  goto fail;
	}
      }
#endif*/
			t = (t0 << 8) | t1;

			sht11_reset();

			return t;
		}
	}

//	fail:
	sreset();
	return FALSE;
}
/*---------------------------------------------------------------------------*/
/*
 * Call may take up to 210ms.
 */
UINT16 sht11_temp(void)
{
	return scmd(MEASURE_TEMP);
}
/*---------------------------------------------------------------------------*/
/*
 * Call may take up to 210ms.
 */
UINT16 sht11_humidity(void)
{
	return scmd(MEASURE_HUMI);
}
/*---------------------------------------------------------------------------*/
#if 1 /* But ok! */
UINT8 sht11_sreg(void)
{
	UINT8 sreg, rcrc;

	sstart();			/* Start transmission */
	if(!swrite(STATUS_REG_R)) {
		goto fail;
	}

	sreg = sread(1);
	rcrc = sread(0);

#ifdef CRC_CHECK
{
	UINT8 crc;
    crc = crc8_add(0x0, STATUS_REG_R);
    crc = crc8_add(crc, sreg);
    if (crc != rev8bits(rcrc))
      goto fail;
}
#endif

	return sreg;

	fail:
	sreset();
	return FALSE;
}
#endif
/*---------------------------------------------------------------------------*/
#if 0
UINT8 sht11_set_sreg(unsigned sreg)
{
  sstart();			/* Start transmission */
  if(!swrite(STATUS_REG_W)) {
    goto fail;
  }
  if(!swrite(sreg)) {
    goto fail;
  }

  return 0;

 fail:
  sreset();
  return -1;
}
#endif
/*---------------------------------------------------------------------------*/
#if 1
UINT8 sht11_reset(void)
{
  sstart();			/* Start transmission */
  if(!swrite(RESET)) {
    goto fail;
  }

  return 0;

 fail:
  sreset();
  return 0xFF;
}
#endif
/*---------------------------------------------------------------------------*/
