#pragma once

#ifndef __MB86233_H__
#define __MB86233_H__

#include "cpuintrf.h"
#include "osd_cpu.h"
#include "osdepend.h"

/***************************************************************************
    REGISTER ENUMERATION
***************************************************************************/

enum
{
	MB86233_PC=1,
	MB86233_A,
	MB86233_B,
	MB86233_D,
	MB86233_P,
	MB86233_REP,
	MB86233_SP,
	MB86233_EB,
	MB86233_SHIFT,
	MB86233_FLAGS,
	MB86233_R0,
	MB86233_R1,
	MB86233_R2,
	MB86233_R3,
	MB86233_R4,
	MB86233_R5,
	MB86233_R6,
	MB86233_R7,
	MB86233_R8,
	MB86233_R9,
	MB86233_R10,
	MB86233_R11,
	MB86233_R12,
	MB86233_R13,
	MB86233_R14,
	MB86233_R15
};

/***************************************************************************
    STRUCTURES
***************************************************************************/
/*
typedef int (*mb86233_fifo_read_func)(device_t *device, UINT32 *data);
typedef void (*mb86233_fifo_write_func)(device_t *device, UINT32 data);

typedef struct _mb86233_cpu_core mb86233_cpu_core;
struct _mb86233_cpu_core
{
	mb86233_fifo_read_func fifo_read_cb;
	mb86233_fifo_write_func fifo_write_cb;
	const char *tablergn;
};
*/

/* PUBLIC GLOBALS */
extern int  mb86233_ICount;


/* PUBLIC FUNCTIONS */
extern void mb86233_reset(void *param);
extern void mb86233_exit(void);
extern int mb86233_execute(int cycles);    /* NS 970908 */
extern unsigned mb86233_get_context(void *dst);
extern void mb86233_set_context(void *src);
extern unsigned mb86233_get_pc(void);
extern void mb86233_set_pc(unsigned val);
extern unsigned mb86233_get_sp(void);
extern void mb86233_set_sp(unsigned val);
extern unsigned mb86233_get_reg(int regnum);
extern void mb86233_set_reg(int regnum, unsigned val);
extern void mb86233_set_nmi_line(int state);
extern void mb86233_set_irq_line(int irqline, int state);
extern void mb86233_set_irq_callback(int (*callback)(int irqline));
extern const char *mb86233_info(void *context, int regnum);
extern unsigned mb86233_dasm(char *buffer, unsigned pc);

#endif /* __MB86233_H__ */

