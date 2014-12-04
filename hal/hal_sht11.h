/*
 * Copyright (c) 2007, Swedish Institute of Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * @(#)$Id: sht11.h,v 1.2 2008/07/08 08:23:47 adamdunkels Exp $
 */

#ifndef SHT11_H
#define SHT11_H

#define SHT11_ARCH_SDA  5       /* P1.5 */
#define SHT11_ARCH_SCL  6       /* P1.6 */
#define SHT11_ARCH_PWR  7       /* P1.7 */

#define SHT11_PxDIR     P1DIR
#define SHT11_PxIN      P1IN
#define SHT11_PxOUT     P1OUT
#define SHT11_PxSEL     P1SEL

 // Humidity Coefficients RH = C1 + C2 * SOrh + C3 * SOrh^2
#define C1_12BIT  -2.0468
#define C2_12BIT  0.0367
#define C3_12BIT  -0.0000015955
#define C1_8BIT  -4
#define C2_8BIT  0.648
#define C3_8BIT  -0.00072

 // Temprature Coefficients for Celsius TEMP = D1 + D2 * SOt
#define D1_CEL35  -39.66
#define D1_CEL30  -39.60
#define D2_12BIT  0.04
#define D2_14BIT  0.01

void sht11_init(void);
void sht11_off(void);

UINT16 sht11_temp(void);
UINT16 sht11_humidity(void);
UINT8 sht11_sreg(void);
UINT8 sht11_set_sreg(UINT8);
UINT8 sht11_reset(void);

/*
 * Relative Humidity in percent (h in 12 bits resolution)
 * RH = -4 + 0.0405*h - 2.8e-6*(h*h)
 *
 * Temperature in Celsius (t in 14 bits resolution at 3 Volts)
 * T = -39.60 + 0.01*t
 */

#endif /* SHT11_H */
