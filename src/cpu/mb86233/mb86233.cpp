/***************************************************************************

    mb86233.c
    Core implementation for the portable Fujitsu MB86233 series DSP emulator.

    Written by ElSemi
    MAME version by Ernesto Corvi

    TODO:
    - Properly emulate the TGP Tables from the ROM (See GETEXTERNAL)
    - Many unknown opcodes and addressing modes
    - Interrupts?

***************************************************************************/

#include "mb86233.h"

/***************************************************************************
    STRUCTURES & TYPEDEFS
***************************************************************************/

typedef union
{
	INT32	i;
	UINT32	u;
	float	f;
} MB86233_REG;

typedef struct _mb86233_state mb86233_state;
struct _mb86233_state
{
	UINT16			pc;
	MB86233_REG		a;
	MB86233_REG		b;
	MB86233_REG		d;
	MB86233_REG		p;

	UINT16			reps;
	UINT16			pcs[4];
	UINT8			pcsp;
	UINT32			eb;
	UINT32			shift;
	UINT32			repcnt;
	UINT16			sr;

	UINT32			gpr[16];
	UINT32			extport[0x30];

//	legacy_cpu_device *device;
//	address_space *program;
//	direct_read_data *direct;
//	int icount;

	/* FIFO */
//	int				fifo_wait;
//	mb86233_fifo_read_func fifo_read_cb;
//	mb86233_fifo_write_func fifo_write_cb;

	/* internal RAM */
	UINT32			*RAM;
	UINT32			*ARAM, *BRAM;
	UINT32			*Tables;
};

int mb86233_ICount;

void mb86233_exit(void) {}
int mb86233_execute(int cycles) { return 0; }    /* NS 970908 */
unsigned mb86233_get_context(void *dst) { return 0; }
void mb86233_set_context(void *src) {}
unsigned mb86233_get_pc(void) { return 0; }
void mb86233_set_pc(unsigned val) {}
unsigned mb86233_get_sp(void) { return 0; }
void mb86233_set_sp(unsigned val) {}
unsigned mb86233_get_reg(int regnum) { return 0; }
void mb86233_set_reg(int regnum, unsigned val) {}
void mb86233_set_nmi_line(int state) {}
void mb86233_set_irq_line(int irqline, int state) {}
void mb86233_set_irq_callback(int (*callback)(int irqline)) {}
const char *mb86233_info(void *context, int regnum) { return ""; }

void mb86233_reset(void *param)
{
}

#ifndef MAME_DEBUG
unsigned mb86233_dasm(char *buffer,  unsigned pc)
{
        return 1;
}
#endif

