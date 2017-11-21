/*
	skyeye_mach_at91.c - define machine at91 for skyeye
	Copyright (C) 2003 Skyeye Develop Group
	for help please send mail to <skyeye-developer@lists.gro.clinux.org>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/
/*
 * 06/15/2005 	rewrite interrupt simulation
 *			walimis <wlm@student.dlut.edu.cn>
 *
 * 3/24/2003 	init this file.
 * 		add machine at91's function.Most taken from original armio.c
 * 		include: at91_mach_init, at91_io_do_cycle
 * 		at91_io_read_word, at91_io_write_word
 *			walimis <wlm@student.dlut.edu.cn>
 *
 *3/24/2003	chenyu <chenyu-tmlinux@hpclab.cs.tsinghua.edu.cn> has done the
 *		necessary work to armio.c
 * */

#include <skyeye_config.h>
#include <skyeye_arch.h>
#include <skyeye_sched.h>
#include <skyeye_lock.h>
#include <bank_defs.h>
#include "at91.h"

#define DEBUG 0
#if DEBUG
#define DBG_PRINT(a...) fprintf(stderr, ##a)
#else
#define DBG_PRINT(a...)
#endif

/* chy 2005-07-28 */
/* zzc:2005-1-1 */
#ifdef __CYGWIN__
#include <time.h>
/* teawater add DBCT_TEST_SPEED 2005.10.04--------------------------------------- */
/*struct timeval
{
	int tv_sec;
	int tv_usec;
};*/
/* AJ2D-------------------------------------------------------------------------- */
#endif

/* 2007-01-18 added by Anthony Lee : for new uart device frame */
#include "skyeye_uart.h"
#ifndef u32
#define u32 uint32_t
#endif

/* extern unsigned int Pen_buffer[8]; */

typedef struct at91_timer_s {
	uint32_t ccr;
	uint32_t cmr;
	uint32_t cv;
	uint32_t ra;
	uint32_t rb;
	uint32_t rc;
	uint32_t sr;
	uint32_t ier;
	uint32_t idr;
	uint32_t imr;
} at91_timer_t;

typedef struct at91_usart_s {
	int rcr;
	uint32_t csr;
	uint32_t rhr;
	uint32_t rpr;
	uint32_t ier;

} at91_usart_t;

typedef struct at91_io {
	uint32_t syscon;		/* System control */
	uint32_t sysflg;		/* System status flags */

	/* rewrite interrupt code.
	 * */
	u32 ivr;
	u32 fvr;
	u32 isr;
	u32 ipr;
	u32 imr;
	u32 cisr;
	u32 iecr;
	u32 idcr;
	u32 iccr;
	u32 iscr;
	u32 eoicr;
	u32 spu;

	uint32_t tcd[3];		/* Timer/counter data */
	uint32_t tcd_reload[3];	/* Last value written */
	at91_timer_t tc_channel[3];

	at91_usart_t usart[2];

	uint32_t lcdcon;		/* LCD control */
	uint32_t lcd_limit;	/* 0xc0000000 <= LCD buffer < lcd_limi
				   t */
	uint32_t ts_buffer[8];
	uint32_t ts_addr_begin;
	uint32_t ts_addr_end;

} at91_io_t;
static at91_io_t at91_io;
#define io at91_io

static void
at91_update_int (void *arch_instance)
{
	interrupt_signal_t interrupt_signal;
	uint32_t requests = io.ipr & io.imr;

	interrupt_signal.arm_signal.firq = (requests & 0x00001) ? Low_level : High_level;
	//state->NfiqSig = (requests & 0x00001) ? LOW : HIGH;
	interrupt_signal.arm_signal.irq = (requests & 0x3fffe) ? Low_level : High_level;
	//state->NirqSig = (requests & 0x3fffe) ? LOW : HIGH;
	interrupt_signal.arm_signal.reset = Prev_level;
	send_signal(&interrupt_signal);
}

/* new added functions
 * */
static void
at91_set_intr (u32 interrupt)
{
	io.ipr |= (1 << interrupt);
}
static int
at91_pending_intr (u32 interrupt)
{
	return ((io.ipr & (1 << interrupt)));
}

static void
at91_update_intr (void *mach)
{
	struct machine_config *mc = (struct machine_config *) mach;
	interrupt_signal_t interrupt_signal;
	uint32_t requests = io.ipr & io.imr;

	interrupt_signal.arm_signal.firq = (requests & 0x00001) ? Low_level : High_level;
	//state->NfiqSig = (requests & 0x00001) ? LOW : HIGH;
	interrupt_signal.arm_signal.irq = (requests & 0x3fffe) ? Low_level : High_level;
	//state->NirqSig = (requests & 0x3fffe) ? LOW : HIGH;
	interrupt_signal.arm_signal.reset = Prev_level;
	send_signal(&interrupt_signal);
}

static int
at91_mem_read_byte (void *arch_instance, u32 addr, u32 *data)
{
//	*data = (u32) mem_read_char (arch_instance, addr);
	printf("in mem read \n");
	return No_exp;
}

static int
at91_mem_write_byte (void *arch_instance, u32 addr, u32 data)
{
//	mem_write_char (arch_instance, addr, (char) data);
	printf("in mem write \n");
	return No_exp;
}

static void
at91_io_reset (generic_arch_t *arch_instance)
{
	io.syscon = 0;
	io.sysflg = URXFE;
	io.tcd[0] = 0xffff;
	io.tcd[1] = 0xffff;
	io.tcd[2] = 0xffff;
	io.tcd_reload[0] = 0xffff;
	io.tcd_reload[1] = 0xffff;
	io.tcd_reload[2] = 0xffff;
	io.lcdcon = 0;
	io.lcd_limit = 0;

	io.usart[0].csr = US_TXEMPTY | US_TXRDY;
	io.usart[1].csr = US_TXEMPTY | US_TXRDY;

	io.ts_addr_begin = 0xff00b000;
	io.ts_addr_end = 0xff00b01f;
}

/*at91 io_do_cycle*/
void
at91_io_do_cycle (generic_arch_t *state)
{
	int t;

	for (t = 0; t < 3; t++) {
		if (io.tcd[t] == 0) {
			if (io.syscon & (t ? TC2M : TC1M)) {

				io.tcd[t] = io.tcd_reload[t];
			} else {
				io.tcd[t] = 0xffff;
			}
			at91_set_intr (t ? IRQ_TC2 : IRQ_TC1);
			at91_update_int (state);
			io.tc_channel[2].sr |= 0x10; /* Set CPCS bit of SR */

		} else {
			io.tcd[t]--;
			if(io.tcd[t] == 0x1000){
				io.tc_channel[t].sr |= 0x10; /* Set CPCS bit of SR */
			}
		}
	}
	if (!(io.ipr & IRQ_USART0)) {
		/* 2007-01-18 modified by Anthony Lee : for new uart device frame */
		struct timeval tv;
		unsigned char buf[16];
		int n, i;
		int maxread;
		int hasrcr = (io.usart[0].rcr > 0);

		tv.tv_sec = 0;
		tv.tv_usec = 0;

		maxread = hasrcr ?
			(io.usart[0].rcr > sizeof (buf) ? sizeof (buf) : io.usart[0].rcr) :
			(io.usart[0].csr & US_RXRDY) ? 0 : 1;

		if((n = skyeye_uart_read(-1, buf, maxread, &tv, NULL)) > 0)
		{
			if (hasrcr)
			{
				for (i = 0; i < n; i++, io.usart[0].rcr--, io.usart[0].rpr++)
				{
					//mem_write_char (state, io.usart[0].rpr, buf[i]);
					bus_write(8, io.usart[0].rpr, buf[i]);
				}
				if (io.usart[0].rcr == 0)
					io.sysflg &= ~URXFE;
			} else {
				io.usart[0].rhr = buf[0];
			}

			io.usart[0].csr |= (US_RXRDY | US_ENDRX);
			if (io.usart[0].ier != 0) { // Only interrupt if someone wants us to do so (otherwise we'll never get here agein
				at91_set_intr (IRQ_USART0);
				at91_update_int (state);
			}
		}
	}

	/* if (rcr > 0 && ... */
	//#ifndef NO_LCD
	if (!(io.ipr & IRQ_EXT1)) {	//if now has no ts interrupt,then query

		unsigned int* Pen_buffer = get_pen_buffer();
		//printf("In %s,Pen_buffer=0x%x\n", __func__, Pen_buffer);
		/* test if Pen_buffer is registed. */
		if(Pen_buffer != NULL){
			if (Pen_buffer[6] == 1) {	//should trigger a interrupt
				*(io.ts_buffer + 0) = Pen_buffer[0];
				*(io.ts_buffer + 1) = Pen_buffer[1];
				*(io.ts_buffer + 4) = Pen_buffer[4];
				*(io.ts_buffer + 6) = Pen_buffer[6];
				at91_set_intr (IRQ_EXT1);
				Pen_buffer[6] = 0;
			}
		}
	}
//#endif
	at91_update_int (state);
}

/*
 *  * Atmel serial support for uClinux console.
 *   */

static char *uart_reg[] = {
	"CR",
	"MR",
	"IER",
	"IDR",
	"IMR",
	"CSR",
	"RHR",
	"THR",
	"BRGR",
	"RTOR",
	"TTGR",
	"RES1",
	"RPR",
	"RCR",
	"TPR",
	"TCR",
};
static uint32_t
uart_read (generic_arch_t *state, uint32_t addr, int uartno)
{
	uint32_t data;

	if (uartno < 0 || uartno > 1) {
		fprintf(stderr, "Invalid UART number in %s, %u\n", __FUNCTION__, uartno);
		return 0;
	}

	switch ((addr & 0xfff) >> 2) {
	case 0xd:		// RCR
		data = io.usart[uartno].rcr;
		break;
	case 0xf:		// TCR
		data = 0;
		break;
	case 0x5:		// CSR
		data = io.usart[uartno].csr;
		break;
	case 0x6:		// RHR
		data = io.usart[uartno].rhr;
		io.usart[uartno].csr &= ~US_RXRDY;
		break;
	case 0x0:		// CR
	case 0x1:		// MR
	case 0x2:		// IER
	case 0x3:		// IDR
	case 0x4:		// IMR
	case 0x7:		// THR
	case 0x8:		// BRGR
	case 0x9:		// RTOR
	case 0xa:		// TTGR
	case 0xb:		// RES1
	case 0xc:		// RPR
	case 0xe:		// TPR
		DBG_PRINT ("uart_read(%s=0x%08x)\n",
			   uart_reg[(addr & 0xfff) >> 2], addr);
		break;
	default:
                fprintf(stderr, "IO erro in %s, addr=0x%x\n", __FUNCTION__, addr);
//                skyeye_exit(-1);
	}
	return (data);
}

static void
uart_write (generic_arch_t *state, uint32_t addr, uint32_t data, int uartno)
{
	static uint32_t tx_buf = 0;

	if (uartno < 0 || uartno > 1) {
		fprintf(stderr, "Invalid UART number in %s, %u\n", __FUNCTION__, uartno);
		return;
	}

	if (uartno != 0)
		return;

	DBG_PRINT ("uart_write(0x%x, 0x%x)\n", (addr & 0xfff) >> 2, data);
	switch ((addr & 0xfff) >> 2) {
	case 0x0:		// CR
	case 0x3:		// IDR
	case 0x9:		// RTOR
	case 0x2:		// IER
		io.usart[uartno].ier = data;
		break;
	case 0x1:		// MR
	case 0x4:		// IMR
	case 0x5:		// CSR
	case 0x6:		// RHR
		DBG_PRINT ("uart_write(%s=0x%x) = 0x%08x\n",
			   uart_reg[(addr & 0xfff) >> 2], addr, data);
		break;
	case 0x7:		// THR
		{
			char c = data;

			/* 2007-01-18 modified by Anthony Lee : for new uart device frame */
			skyeye_uart_write(-1, &c, 1, NULL);
		}
		//printf("%c", data); fflush(stdout);
		break;
	case 0x8:		// BRGR
	case 0xa:		// TTGR
	case 0xb:		// RES1
		DBG_PRINT ("uart_write(%s=0x%x) = 0x%08x\n",
			   uart_reg[(addr & 0xfff) >> 2], addr, data);
		break;
	case 0xd:		// RCR
		io.usart[uartno].rcr = data;
		if (io.usart[uartno].rcr == 0)
			io.sysflg &= ~URXFE;
		else
			io.sysflg |= URXFE;
		break;
	case 0xc:		// RPR
		DBG_PRINT ("uart_write(%s=0x%x) = 0x%08x\n",
			   uart_reg[(addr & 0xfff) >> 2], addr, data);
		io.usart[uartno].rpr = data;
		break;
	case 0xe:		// TPR
		tx_buf = data;
		break;
	case 0xf:		// TCR
		for (; tx_buf && data > 0; data--) {
//			char c = mem_read_char (state, tx_buf++);
			char c;
			bus_read(8, tx_buf++, &c);
			/* 2007-01-18 modified by Anthony Lee : for new uart device frame */
			skyeye_uart_write(-1, &c, 1, NULL);

			//printf("%c", mem_read_char(state, tx_buf++));
			//fflush(stdout);
		}
		tx_buf = 0;
		break;
	default:
		fprintf(stderr, "IO erro in %s\n", __FUNCTION__);
		skyeye_exit(-1);
	}
}

static uint32_t timer_read(generic_arch_t *state, int index, int offset){
	uint32_t data;
	at91_timer_t *timer = &io.tc_channel[index];
	switch(offset){
	case 0x0:	/* TIMER 1 CCR */
		data = timer->ccr;

		break;
	case 0x4:	/* TIMER 1 CMR */
		data = timer->cmr;
		break;
	case 0x10:	/* TIMER 1 CV */
		data = timer->cv;
		data = io.tcd[0];
		break;
	case 0x14:      /* TIMER 1 RA */
		data = timer->ra;
                break;
	case 0x18:      /* TIMER 1 RB */
                data = timer->rb;
                break;

	case 0x1c:	/* TIMER 1 RC */
		data = timer->rc;
		break;
	case 0x20:	/* TIMER 1 SR */
		data = timer->sr;
		if(timer->sr & 0x10)
			timer->sr &= ~0x10;
		//fprintf(stderr, "timer %d read sr,data=0x%x\n",index, data);
		//io.ipr &= ~(1<<IRQ_TC1);
		break;
	case 0x24:	/* TIMER 1 IER */
		data = timer->ier;
		break;
	case 0x28:	/* TIMER 1 IDR */
		data = timer->idr;
		break;
        case 0x2C:      /* TIMER 1 IMR */
                data = timer->imr;
                break;

	default:
		fprintf(stderr, "IO erro in %s, offset=0x%x\n", __FUNCTION__, offset);
                //skyeye_exit(-1);
	}
	return data;
}
static void timer_write(generic_arch_t *state, int index, int offset, uint32_t data){
	at91_timer_t *timer = &io.tc_channel[index];
	switch(offset){
	case 0x0:	/* TIMER 1 CCR */
		timer->ccr = data;
		if (data & 0x2) {
                        io.syscon &= ~TC1M;
                }
                else {
                        io.syscon |= TC1M;
                }
		break;
	case 0x4:	/* TIMER 1 CMR */
		timer->cmr = data;
		break;
	case 0x10:	/* TIMER 1 CV */
		timer->cv = data ;
		io.tcd[0] = io.tcd_reload[0] = data & 0xffff;
		break;
	case 0x14:      /* TIMER 1 RA */
		timer->ra = data;
                break;
	case 0x18:      /* TIMER 1 RB */
                timer->rb = data;
                break;

	case 0x1c:	/* TIMER 1 RC */
		timer->rc = data;
		io.tcd[0] = io.tcd_reload[0] = data & 0xffff;
		break;
	case 0x20:	/* TIMER 1 SR */
		timer->sr = data;
		//io.ipr &= ~(1<<IRQ_TC1);
		break;
	case 0x24:	/* TIMER 1 IER */
		timer->ier = data;
		break;
	case 0x28:	/* TIMER 1 IDR */
		timer->idr = data;
		break;
        case 0x2C:      /* TIMER 1 IMR */
                timer->imr = data;
                break;

	default:
		fprintf(stderr, "IO erro in %s ,addr=0x%x\n", __FUNCTION__, offset);
                //skyeye_exit(-1);
	}
}

static uint32_t
at91_io_read_byte (void *arch_instance, uint32_t addr)
{
	fprintf(stderr, "IO erro in %s, addr=0x%x\n", __FUNCTION__,addr);
	return 0;
}

uint32_t
at91_io_read_halfword (void *arch_instance, uint32_t addr)
{
	fprintf(stderr, "IO erro in %s\n", __FUNCTION__);
	return 0;
}

uint32_t
at91_io_read_word (void *arch_instance, uint32_t addr)
{

	uint32_t data = -1;
	int i;
	uint32_t ts_addr;
	ts_addr = addr & ~3;	/* 1 word==4 byte */
	if (ts_addr >= io.ts_addr_begin && ts_addr <= io.ts_addr_end) {
		data = io.ts_buffer[(ts_addr - io.ts_addr_begin) / 4];
		return data;
	}

	switch (addr) {
	case 0xfffff100:	/* IVR */
		data = io.ipr;
		DBG_PRINT ("IVR irqs=%x ", data);
		for (i = 0; i < 32; i++)
			if (data & (1 << i))
				break;
		if (i < 32) {
			data = i;
			io.ipr &= ~(1 << data);
			at91_update_int (arch_instance);
		}
		else
			data = 0;
		io.ivr = data;
		//current_ivr = data;
		DBG_PRINT ("read IVR=%d\n", data);
		break;
	case 0xfffff108:	/* interrupt status register */
		//      data = current_ivr;
		data = io.ivr;
		break;
	case 0xfffff110:	/* IMR */
		data = io.imr;
		break;
	case 0xfffff114:	/* CORE interrupt status register */
		data = io.cisr;
		data = io.imr;
		break;
	case 0xfffff120:	/* IECR */
		data = io.iecr;
		data = io.imr;
		break;
	case 0xfffff124:	/* IDCR */
		break;
	case 0xfff00000:	/* CPU ID */
		data = 0x2078aa0;
		data = 0x14000040;
		break;
	case 0xfffe00c0:	/* TIMER 1 BCR */
		DBG_PRINT ("T1-BCR io_read_word(0x%08x) = 0x%08x\n", addr,
			   data);
		break;
	case 0xfffe00c4:	/* TIMER 1 BMR */
		DBG_PRINT ("T1-BMR io_read_word(0x%08x) = 0x%08x\n", addr,
			   data);
		break;
	default:
		if ((addr & 0xffffff00) == 0xfffff000)
			break;
		if ((addr & 0xfffff000) == 0xfffd0000) {
			return uart_read (arch_instance, addr, 0);
		}
		if ((addr & 0xfffff000) == 0xfffcc000) {
			return uart_read (arch_instance, addr, 1);
			break;
		}
		if(addr >= 0xfffe0000 && addr < 0xfffe00c0){
			return timer_read(arch_instance, ((addr & 0xc0) >> 6), (addr & 0x3f));
		}
		//fprintf (stderr,
		//       "NumInstr %llu, io_read_word unknown addr(0x%08x) = 0x%08x\n",
		//       state->NumInstrs, addr, data);
		//SKYEYE_OUTREGS (stderr);
		//ARMul_Debug(state, 0, 0);
		break;
	}
	return data;
}

void
at91_io_write_byte (generic_arch_t *state, uint32_t addr, uint32_t data)
{
	fprintf(stderr," IO error in %s,addr=0x%x\n", __FUNCTION__);
	return;
}

void
at91_io_write_halfword (generic_arch_t *state, uint32_t addr, uint32_t data)
{
	fprintf(stderr," IO error in %s\n", __FUNCTION__);
	return;
}

void
at91_io_write_word (generic_arch_t *state, uint32_t addr, uint32_t data)
{
	/*
	 * The Atmel system registers
	 */
	if(addr >= 0xfffe0000 && addr < 0xfffe00c0) {
		timer_write(state, (addr & 0xc0) >> 6, (addr & 0x3f), data);
		return;
        }

	switch (addr) {
	case 0xfffff108:	/* ISR */
		//DBG_PRINT("write ISR=0x%x\n", data);
		//io.isr = data;
		break;
	case 0xfffff110:	/* IMR */
		//io.imr = data;
		break;
	case 0xfffff114:	/* CORE interrupt status register */
		//io.cisr = data;
		DBG_PRINT ("write CISR=0x%x\n", data);
		break;
	case 0xfffff120:	/* IECR */
		DBG_PRINT ("IECR=0x%x\n", data);
		io.iecr = data;
		io.imr |= data;
		break;
	case 0xfffff124:	/* IDCR */
		DBG_PRINT ("IDCR=0x%x\n", data);
		io.idcr = data;
		io.imr &= ~data;
		break;
	case 0xfffff128:	/* CLEAR interrupts */
		DBG_PRINT ("ICCR=0x%x\n", data);
		io.iccr = data;
		io.ipr &= ~data;
		break;
	case 0xfffff130:	/* EOI */
		DBG_PRINT (stderr, "EOI=0x%x\n", data);
               /**
                * 2.4.32 in uClinux is broken somehow and always sends
                * 0xfffff130 as both address and data, sometimes clearing
                * not yet processed interrupt. Anyway, the thing
                * works without the following
               */
#if 0
		io.eoicr = data;
		io.ipr &= ~data;
#endif
		at91_update_int (state);
		break;
	case 0xfff00000:	/* CPU ID */
		break;
	case 0xfffe00c0:	/* TIMER 1 BCR */
		DBG_PRINT ("T1-BCR io_write_word(0x%08x) = 0x%08x\n", addr,
			   data);
		break;
	case 0xfffe00c4:	/* TIMER 1 BMR */
		DBG_PRINT ("T1-BMR io_write_word(0x%08x) = 0x%08x\n", addr,
			   data);
		break;
	default:
		if ((addr & 0xfffff000) == 0xfffd0000) {
			uart_write (state, addr, data, 0);
			break;
		}
		if ((addr & 0xfffff000) == 0xfffcc000) {
			uart_write (state, addr, data, 1);
			break;
		}
		if ((addr & 0xffffff00) == 0xfffff000)
			break;
		if ((addr & 0xffff0000) == 0xffff0000) {
			DBG_PRINT ("io_write_word(0x%08x) = 0x%08x\n", addr,
				   data);
			break;
		}
		//DBG_PRINT("io_write_word(0x%08x) = 0x%08x\n", addr, data);
		//fprintf (stderr,
		//       "NumInstr %llu,io_write_word unknown addr(0x%08x) = 0x%08x\n",
		//       state->NumInstrs, addr, data);
		//SKYEYE_OUTREGS (stderr);
		//ARMul_Debug(state, 0, 0);
		break;
	}
}

void
at91_mach_init (void *arch_instance, machine_config_t *this_mach)
{
#if 0
	/* chy 2003-08-19, setprocessor */
	ARMul_SelectProcessor (state, ARM_v4_Prop);
	/* chy 2004-05-09, set lateabtSig */
	state->lateabtSig = HIGH;
#endif

	this_mach->mach_io_do_cycle = at91_io_do_cycle;
	this_mach->mach_io_reset = at91_io_reset;
	this_mach->mach_io_read_byte = at91_io_read_byte;
	this_mach->mach_io_write_byte = at91_io_write_byte;
	this_mach->mach_io_read_halfword = at91_io_read_halfword;
	this_mach->mach_io_write_halfword = at91_io_write_halfword;
	this_mach->mach_io_read_word = at91_io_read_word;
	this_mach->mach_io_write_word = at91_io_write_word;

	this_mach->mach_update_int = at91_update_int;

	this_mach->mach_set_intr = at91_set_intr;
	this_mach->mach_pending_intr = at91_pending_intr;
	this_mach->mach_update_intr = at91_update_intr;

	this_mach->mach_mem_read_byte = at91_mem_read_byte;
	this_mach->mach_mem_write_byte = at91_mem_write_byte;
}
