#ifndef _SA1100_H_
#define _SA1100_H_

enum
{
	RTC_ALARM_IRQ = (1 << 31),
	RTC_HZ_IRQ = (1 << 30),
	OS_IRQ_SHF = 26,
	UART3_IRQ = (1 << 17),
};

enum
{
	ICIP = 0x90050000,
	ICMR = 0x90050004,
	ICLR = 0x90050008,
	ICFP = 0x90050010,
	ICPR = 0x90050020,
	ICCR = 0x9005000C,

	RTAR = 0x90010000,
	RCNR = 0x90010004,
	RTTR = 0x90010008,
	RTSR = 0x90010010,

	OSMR0 = 0x90000000,
	OSMR1 = 0x90000004,
	OSMR2 = 0x90000008,
	OSMR3 = 0x9000000C,
	OSCR = 0x90000010,
	OSSR = 0x90000014,
	OWER = 0x90000018,
	OIER = 0x9000001C,

	/*UART3 */
	UTCR0 = 0x80050000,
	UTCR1 = 0x80050004,
	UTCR2 = 0x80050008,
	UTCR3 = 0x8005000C,
	UTDR = 0x80050014,
	UTSR0 = 0x8005001C,
	UTSR1 = 0x80050020,
};

void
sa1100_mach_init (void *arch_instance, machine_config_t *mc);
#endif
