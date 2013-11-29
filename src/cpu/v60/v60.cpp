// V60.CPP ported from Mame 056 and 151 to Mame4All-RPI by Ridrok aka Neo2003
// Undiscover the beast!
// Main hacking and coding by Farfetch'd
// Portability fixes by Richter Belmont

#include "cpuintrf.h"
#include "osd_cpu.h"
#include "osdepend.h"

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include "v60.h"

// macros stolen from MAME for flags calc
// note that these types are in x86 naming:
// byte = 8 bit, word = 16 bit, long = 32 bit

// parameter x = result, y = source 1, z = source 2

#define SetOFL_Add(x, y,z)	(_OV = (((x) ^ (y)) & ((x) ^ (z)) & 0x80000000) ? 1: 0)
#define SetOFW_Add(x, y,z)	(_OV = (((x) ^ (y)) & ((x) ^ (z)) & 0x8000) ? 1 : 0)
#define SetOFB_Add(x, y,z)	(_OV = (((x) ^ (y)) & ((x) ^ (z)) & 0x80) ? 1 : 0)

#define SetOFL_Sub(x, y,z)	(_OV = (((z) ^ (y)) & ((z) ^ (x)) & 0x80000000) ? 1 : 0)
#define SetOFW_Sub(x, y,z)	(_OV = (((z) ^ (y)) & ((z) ^ (x)) & 0x8000) ? 1 : 0)
#define SetOFB_Sub(x, y,z)	(_OV = (((z) ^ (y)) & ((z) ^ (x)) & 0x80) ? 1 : 0)

#define SetCFB(x)		{_CY = ((x) & 0x100) ? 1 : 0; }
#define SetCFW(x)		{_CY = ((x) & 0x10000) ? 1 : 0; }
#define SetCFL(x)		{_CY = ((x) & (((UINT64)1) << 32)) ? 1 : 0; }

#define SetSF(x)		(_S = (x))
#define SetZF(x)		(_Z = (x))

#define SetSZPF_Byte(x) 	{_Z = ((UINT8)(x) == 0);  _S = ((x)&0x80) ? 1 : 0; }
#define SetSZPF_Word(x) 	{_Z = ((UINT16)(x) == 0);  _S = ((x)&0x8000) ? 1 : 0; }
#define SetSZPF_Long(x) 	{_Z = ((UINT32)(x) == 0);  _S = ((x)&0x80000000) ? 1 : 0; }

#define ORB(dst, src)		{ (dst) |= (src); _CY = _OV = 0; SetSZPF_Byte(dst); }
#define ORW(dst, src)		{ (dst) |= (src); _CY = _OV = 0; SetSZPF_Word(dst); }
#define ORL(dst, src)		{ (dst) |= (src); _CY = _OV = 0; SetSZPF_Long(dst); }

#define ANDB(dst, src)		{ (dst) &= (src); _CY = _OV = 0; SetSZPF_Byte(dst); }
#define ANDW(dst, src)		{ (dst) &= (src); _CY = _OV = 0; SetSZPF_Word(dst); }
#define ANDL(dst, src)		{ (dst) &= (src); _CY = _OV = 0; SetSZPF_Long(dst); }

#define XORB(dst, src)		{ (dst) ^= (src); _CY = _OV = 0; SetSZPF_Byte(dst); }
#define XORW(dst, src)		{ (dst) ^= (src); _CY = _OV = 0; SetSZPF_Word(dst); }
#define XORL(dst, src)		{ (dst) ^= (src); _CY = _OV = 0; SetSZPF_Long(dst); }

#define SUBB(dst, src)		{ unsigned res = (dst) - (src); SetCFB(res); SetOFB_Sub(res, src, dst); SetSZPF_Byte(res); dst = (UINT8)res; }
#define SUBW(dst, src)		{ unsigned res = (dst) - (src); SetCFW(res); SetOFW_Sub(res, src, dst); SetSZPF_Word(res); dst = (UINT16)res; }
#define SUBL(dst, src)		{ UINT64 res = (UINT64)(dst) - (INT64)(src); SetCFL(res); SetOFL_Sub(res, src, dst); SetSZPF_Long(res); dst = (UINT32)res; }

#define ADDB(dst, src)		{ unsigned res = (dst) + (src); SetCFB(res); SetOFB_Add(res, src, dst); SetSZPF_Byte(res); dst = (UINT8)res; }
#define ADDW(dst, src)		{ unsigned res = (dst) + (src); SetCFW(res); SetOFW_Add(res, src, dst); SetSZPF_Word(res); dst = (UINT16)res; }
#define ADDL(dst, src)		{ UINT64 res = (UINT64)(dst) + (UINT64)(src); SetCFL(res); SetOFL_Add(res, src, dst); SetSZPF_Long(res); dst = (UINT32)res; }

#define SETREG8(a, b)		(a) = ((a) & ~0xff) | ((b) & 0xff)
#define SETREG16(a, b)		(a) = ((a) & ~0xffff) | ((b) & 0xffff)

static UINT32 (*OpCodeTable[256])(void);

static offs_t start_pc, fetch_xor;

typedef struct
{
	UINT8 CY;
	UINT8 OV;
	UINT8 S;
	UINT8 Z;
} Flags;

// v60 Register Inside (Hm... It's not a pentium inside :-))) )
//typedef struct v60Info v60_state;
struct v60info
{
	UINT32				reg[68];
	Flags				flags;
	UINT8				irq_line;
	UINT8				nmi_line;
        int (*irq_cb)(int irqline);
	UINT32				PPC;
	int				stall_io;
} v60;

int v60_ICount;

#undef _S

#define _CY 		v60.flags.CY
#define _OV 		v60.flags.OV
#define _S		v60.flags.S
#define _Z		v60.flags.Z

// Defines of all v60 register...
#define R0		v60.reg[0]
#define R1		v60.reg[1]
#define R2		v60.reg[2]
#define R3		v60.reg[3]
#define R4		v60.reg[4]
#define R5		v60.reg[5]
#define R6		v60.reg[6]
#define R7		v60.reg[7]
#define R8		v60.reg[8]
#define R9		v60.reg[9]
#define R10		v60.reg[10]
#define R11		v60.reg[11]
#define R12		v60.reg[12]
#define R13		v60.reg[13]
#define R14		v60.reg[14]
#define R15		v60.reg[15]
#define R16		v60.reg[16]
#define R17		v60.reg[17]
#define R18		v60.reg[18]
#define R19		v60.reg[19]
#define R20		v60.reg[20]
#define R21		v60.reg[21]
#define R22		v60.reg[22]
#define R23		v60.reg[23]
#define R24		v60.reg[24]
#define R25		v60.reg[25]
#define R26		v60.reg[26]
#define R27		v60.reg[27]
#define R28		v60.reg[28]
#define AP		v60.reg[29]
#define FP		v60.reg[30]
#define SP		v60.reg[31]

#define PC		v60.reg[32]
#define PSW		v60.reg[33]
#define PPC		v60.PPC

// Privileged registers
#define ISP		v60.reg[36]
#define L0SP		v60.reg[37]
#define L1SP		v60.reg[38]
#define L2SP		v60.reg[39]
#define L3SP		v60.reg[40]
#define SBR		v60.reg[41]
#define TR		v60.reg[42]
#define SYCW		v60.reg[43]
#define TKCW		v60.reg[44]
#define PIR		v60.reg[45]
//10-14 reserved
#define PSW2	v60.reg[51]
#define ATBR0	v60.reg[52]
#define ATLR0	v60.reg[53]
#define ATBR1	v60.reg[54]
#define ATLR1	v60.reg[55]
#define ATBR2	v60.reg[56]
#define ATLR2	v60.reg[57]
#define ATBR3	v60.reg[58]
#define ATLR3	v60.reg[59]
#define TRMODE	v60.reg[60]
#define ADTR0	v60.reg[61]
#define ADTR1	v60.reg[62]
#define ADTMR0	v60.reg[63]
#define ADTMR1	v60.reg[64]
//29-31 reserved

// Register names
const char *v60_reg_names[68] = {
	"R0", "R1", "R2", "R3",
	"R4", "R5", "R6", "R7",
	"R8", "R9", "R10", "R11",
	"R12", "R13", "R14", "R15",
	"R16", "R17", "R18", "R19",
	"R20", "R21", "R22", "R23",
	"R24", "R25", "R26", "R27",
	"R28", "AP", "FP", "SP",
	"PC", "PSW", "R34", "R35",
	"ISP", "L0SP", "L1SP", "L2SP",
	"L3SP", "SBR", "TR", "SYCW", "TKCW", "PIR",
        "R46", "R47", "R48", "R49", "R50",
	"PSW2", "ATBR0", "ATLR0", "ATBR1",
	"ATLR1", "ATBR2", "ATLR2", "ATBR3",
	"ATLR3", "TRMODE", "ADTR0", "ADTR1",
	"ADTMR0", "ADTMR1", "R65", "R66", "R67"
};

// Defines...
#define NORMALIZEFLAGS(cs) \
{ \
	_S	= _S  ? 1 : 0; \
	_OV	= _OV ? 1 : 0; \
	_Z	= _Z  ? 1 : 0; \
	_CY	= _CY ? 1 : 0; \
}

#define MemRead8		cpu_readmem24lew
#define MemRead16		cpu_readmem24lew_word
#define MemWrite8		cpu_writemem24lew
#define MemWrite16		cpu_writemem24lew_word
#define PortRead8		cpu_readport24lew
#define PortRead16		cpu_readport24lew_word
#define PortWrite8		cpu_writeport24lew
#define PortWrite16		cpu_writeport24lew_word

static UINT32 MemRead32(offs_t address)
{
	UINT32 result = cpu_readmem24lew_word(address);
	return result | (cpu_readmem24lew_word(address + 2) << 16);
}

static void MemWrite32(offs_t address, UINT32 data)
{
	cpu_writemem24lew_word(address, data);
	cpu_writemem24lew_word(address + 2, data >> 16);
}

static UINT32 PortRead32(offs_t address)
{
	UINT32 result = cpu_readport24lew_word(address);
	return result | (cpu_readport24lew_word(address + 2) << 16);
}

static void PortWrite32(offs_t address, UINT32 data)
{
	cpu_writeport24lew_word(address, data);
	cpu_writeport24lew_word(address + 2, data >> 16);
}

static void fatalerror(const char *msg)
{
	logerror("%s", msg);
	throw;
}

static void fatalerror(const char *msg, UINT32 pc)
{
	char foo[1024];
	sprintf(foo, "%s (PC=%06x)", msg, (int)pc);
	fatalerror(foo);
}

static void v60_try_irq(void);


INLINE void v60SaveStack(void)
{
	if (PSW & 0x10000000)
		ISP = SP;
	else
		v60.reg[37 + ((PSW >> 24) & 3)] = SP;
}

INLINE void v60ReloadStack(void)
{
	if (PSW & 0x10000000)
		SP = ISP;
	else
		SP = v60.reg[37 + ((PSW >> 24) & 3)];
}

INLINE UINT32 v60ReadPSW(void)
{
	PSW &= 0xfffffff0;
	PSW |= (_Z?1:0) | (_S?2:0) | (_OV?4:0) | (_CY?8:0);
	return PSW;
}

INLINE void v60WritePSW(UINT32 newval)
{
	/* determine if we need to save / restore the stacks */
	int updateStack = 0;

	/* if the interrupt state is changing, we definitely need to update */
	if ((newval ^ PSW) & 0x10000000)
		updateStack = 1;

	/* if we are not in interrupt mode and the level is changing, we also must update */
	else if (!(PSW & 0x10000000) && ((newval ^ PSW) & 0x03000000))
		updateStack = 1;

	/* save the previous stack value */
	if (updateStack)
		v60SaveStack();

	/* set the new value and update the flags */
	PSW = newval;
	_Z =  (UINT8)(PSW & 1);
	_S =  (UINT8)(PSW & 2);
	_OV = (UINT8)(PSW & 4);
	_CY = (UINT8)(PSW & 8);

	/* fetch the new stack value */
	if (updateStack)
		v60ReloadStack();
}


INLINE UINT32 v60_update_psw_for_exception(int is_interrupt, int target_level)
{
	UINT32 oldPSW = v60ReadPSW();
	UINT32 newPSW = oldPSW;

	// Change to interrupt context
	newPSW &= ~(3 << 24);  // PSW.EL = 0
	newPSW |= target_level << 24; // set target level
	newPSW &= ~(1 << 18);  // PSW.IE = 0
	newPSW &= ~(1 << 16);  // PSW.TE = 0
	newPSW &= ~(1 << 27);  // PSW.TP = 0
	newPSW &= ~(1 << 17);  // PSW.AE = 0
	newPSW &= ~(1 << 29);  // PSW.EM = 0
	if (is_interrupt)
		newPSW |=  (1 << 28);// PSW.IS = 1
	newPSW |=  (1 << 31);  // PSW.ASA = 1
	v60WritePSW(newPSW);

	return oldPSW;
}


#define GETINTVECT(nint)	MemRead32((SBR & ~0xfff) + (nint) * 4)
#define EXCEPTION_CODE_AND_SIZE(code, size)	(((code) << 16) | (size))


// Addressing mode decoding functions
#include "am.cpp"

// Opcode functions
#include "op12.cpp"
#include "op2.cpp"
#include "op3.cpp"
#include "op4.cpp"
#include "op5.cpp"
#include "op6.cpp"
#include "op7a.cpp"

UINT32 opUNHANDLED()
{
        char foo[1024];
        sprintf(foo, "Unhandled OpCode found : %02x at %08x", MemRead16(PC), PC);
        fatalerror(foo);
        return 0; /* never reached, fatalerror won't return */
}

// Opcode jump table
#include "optable.cpp"

static int v60_default_irq_cb(int irqline)
{
	return 0;
}

void v60_init(void)
{
	static int opt_init = 0;
	if(!opt_init)
	{
		InitTables();	// set up opcode tables
		opt_init = 1;
	}

	// Set cpustate->PIR (Processor ID) for NEC cpustate-> LSB is reserved to NEC,
	// so I don't know what it contains.
	PIR = 0x00006000;
	v60.irq_cb = v60_default_irq_cb;
	fetch_xor = BYTE_XOR_LE(0);
	start_pc = 0xfffff0;
	v60.irq_line = CLEAR_LINE;
	v60.nmi_line = CLEAR_LINE;
}

void v70_init(void)
{
        static int opt_init = 0;
        if(!opt_init)
        {
                InitTables();   // set up opcode tables
                opt_init = 1;
        }

        // Set cpustate->PIR (Processor ID) for NEC cpustate-> LSB is reserved to NEC,
        // so I don't know what it contains.
        PIR = 0x00007000;
        v60.irq_cb = v60_default_irq_cb;
        fetch_xor = BYTE4_XOR_LE(0);
        start_pc = 0xfffffff0;
        v60.irq_line = CLEAR_LINE;
        v60.nmi_line = CLEAR_LINE;
}

void v60_reset(void *param)
{
	PSW	= 0x10000000;
	PC	= start_pc;
	SBR	= 0x00000000;
	SYCW	= 0x00000070;
	TKCW	= 0x0000e000;
	PSW2	= 0x0000f002;

	_CY	= 0;
	_OV	= 0;
	_S	= 0;
	_Z	= 0;
}

void v60_exit(void)
{
}

static void v60_do_irq(int vector)
{
	UINT32 oldPSW = v60_update_psw_for_exception(1, 0);

	// Push PC and PSW onto the stack
	SP-=4;
	MemWrite32(SP, oldPSW);
	SP-=4;
	MemWrite32(SP, PC);

	// Jump to vector for user interrupt
	PC = GETINTVECT(vector);
}

static void v60_try_irq()
{
	if(v60.irq_line == CLEAR_LINE)
		return;
	if((PSW & (1 << 18)) != 0) {
		int vector;
		if(v60.irq_line != ASSERT_LINE)
			v60.irq_line = CLEAR_LINE;

		vector = v60.irq_cb(0);

		v60_do_irq(vector + 0x40);
	}
}

void v60_set_irq_line(int irqline, int state)
{
	if(irqline == INPUT_LINE_NMI) {
		switch(state) {
		case ASSERT_LINE:
			if(v60.nmi_line == CLEAR_LINE) {
				v60.nmi_line = ASSERT_LINE;
				v60_do_irq(2);
			}
			break;
		case CLEAR_LINE:
			v60.nmi_line = CLEAR_LINE;
			break;
		}
	} else {
		v60.irq_line = state;
		v60_try_irq();
	}
}

void v60_set_irq_callback(int (*irq_cb)(int irqline))
{
	v60.irq_cb = irq_cb;
}

// Actual cycles / instruction is unknown

int v60_execute(int cycles)
{
	if (v60.irq_line != CLEAR_LINE)
		v60_try_irq();

	while (v60_ICount > 0)
	{
		UINT32 inc;
		PPC = PC;
		v60_ICount -= 8;	/* fix me -- this is just an average */
		inc = OpCodeTable[MemRead8(PC)]();
		PC += inc;
		if (v60.irq_line != CLEAR_LINE)
			v60_try_irq();
	}
	return cycles;
}

unsigned v60_get_context(void *dst)
{
	if(dst)
		*(struct v60info *)dst = v60;
	return sizeof(struct v60info);
}

void v60_set_context(void *src)
{
	if(src)
		v60 = *(struct v60info *)src;
}

unsigned v60_get_reg(int regnum)
{
	switch(regnum) {
	case REG_PC:
		return PC;
	case REG_PREVIOUSPC:
		return PPC;
	case REG_SP:
		return SP;
	}
	if(regnum >= 1 && regnum < V60_REGMAX)
		return v60.reg[regnum - 1];
	return 0;
}

void v60_set_reg(int regnum, unsigned val)
{
	switch(regnum) {
	case REG_PC:
		PC = val;
		return;
	case REG_SP:
		SP = val;
		return;
	}
	if(regnum >= 1 && regnum < V60_REGMAX)
		v60.reg[regnum - 1] = val;
}

unsigned v60_get_sp(void)
{
        return v60_get_reg(REG_SP);
}

void v60_set_sp(unsigned val)
{
        v60_set_reg(REG_SP, val);
}

unsigned v60_get_pc(void)
{
        return v60_get_reg(REG_PC);
}

void v60_set_pc(unsigned val)
{
        v60_set_reg(REG_PC, val);
}

void v60_set_nmi_line(int state)
{
	v60_set_irq_line(INPUT_LINE_NMI, state);
}

static UINT8 v60_reg_layout[] = {
	V60_PC, V60_TR, -1,
	-1,
	V60_R0, V60_R1, -1,
	V60_R2, V60_R3, -1,
	V60_R4, V60_R5, -1,
	V60_R6, V60_R7, -1,
	V60_R8, V60_R9, -1,
	V60_R10, V60_R11, -1,
	V60_R12, V60_R13, -1,
	V60_R14, V60_R15, -1,
	V60_R16, V60_R17, -1,
	V60_R18, V60_R19, -1,
	V60_R20, V60_R21, -1,
	V60_R22, V60_R23, -1,
	V60_R24, V60_R25, -1,
	V60_R26, V60_R27, -1,
	V60_R28, V60_AP, -1,
	V60_FP, V60_SP, -1,
	-1,
	V60_SBR, V60_ISP, -1,
	V60_L0SP, V60_L1SP, -1,
	V60_L2SP, V60_L3SP, 0
};


const char *v60_info(void *context, int regnum)
{
//	struct v60info *r = context ? context : &v60;
	static char buffer[32][47+1];
	static int which = 0;

	switch(regnum) {
	case CPU_INFO_NAME:
		return "V60";
	case CPU_INFO_FAMILY:
		return "NEC V60";
	case CPU_INFO_VERSION:
		return "1.0";
	case CPU_INFO_CREDITS:
		return "Farfetch'd and R.Belmont";
	case CPU_INFO_REG_LAYOUT:
		return (const char *)v60_reg_layout;
	//case CPU_INFO_WIN_LAYOUT:
	//	return (const char *)v60_win_layout;
	}

	which = (which+1) % 32;
	buffer[which][0] = '\0';

//	if(regnum > CPU_INFO_REG && regnum < CPU_INFO_REG + V60_REGMAX) {
//		int reg = regnum - CPU_INFO_REG - 1;
//		sprintf(buffer[which], "%s:%08X", v60_reg_names[reg], r->reg[reg]);
//	}

	return buffer[which];
}

#ifndef MAME_DEBUG
unsigned v60_dasm(char *buffer,  unsigned pc)
{
	change_pc24lew(pc);
	sprintf(buffer, "$%02X", cpu_readop(pc));
	return 1;
}
#endif
