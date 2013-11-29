#pragma once

#ifndef __V60_H__
#define __V60_H__

#include "memory.h"

extern data_t cpu_readport24lew(offs_t address);
extern data_t cpu_readport24lew_word(offs_t address);
extern void cpu_writeport24lew(offs_t address, data_t data);
extern void cpu_writeport24lew_word(offs_t address, data_t data);

//static struct handler_data 	rporthandler16[ENTRY_COUNT];

/*
GENERATE_HANDLERS_16BIT_LE(port, abits)
#define GENERATE_HANDLERS_16BIT_LE(type, abits) \
	 READBYTE16LE(cpu_read##type##abits##lew,        abits, read##type##_lookup,  r##type##handler16, type##_amask) \
	   READWORD16(cpu_read##type##abits##lew_word,   abits, read##type##_lookup,  r##type##handler16, type##_amask) \
	WRITEBYTE16LE(cpu_write##type##abits##lew,       abits, write##type##_lookup, w##type##handler16, type##_amask) \
	  WRITEWORD16(cpu_write##type##abits##lew_word,  abits, write##type##_lookup, w##type##handler16, type##_amask)

READBYTE16LE(cpu_readport24lew,     24, readport_lookup,  rporthandler16, port_amask)
#define READBYTE16LE(name,abits,lookup,handlist,mask)
*/

//data8_t cpu_readport24lew(offs_t address)
//{
//	UINT8 entry;
//	MEMREADSTART
//
//	/* perform lookup */
//	address &= port_amask;
//	entry = readport_lookup[LEVEL1_INDEX(address,24 ,1)];
//	if (entry >= SUBTABLE_BASE)
//		entry = readport_lookup[LEVEL2_INDEX(entry,address,24 ,1)];
//
//	/* handle banks inline */
//	address -= rporthandler16[entry].offset;
//	if (entry <= STATIC_RAM)
//		MEMREADEND(cpu_bankbase[entry][BYTE_XOR_LE(address)])
//
//	/* fall back to the handler */
//	else
//	{
//		int shift = 8 * (address & 1);
//		read16_handler handler = (read16_handler)rporthandler16[entry].handler;
//		MEMREADEND((*handler)(address >> 1, ~(0xff << shift)) >> shift)
//	}
//	return 0;
//}

/*
#define READWORD16(cpu_readport24lew_word,24, readport_lookup,rporthandler16,port_amask)
*/

//data16_t cpu_readport24lew_word(offs_t address)
//{
//	UINT8 entry;
//	MEMREADSTART
//
//	/* perform lookup */
//	address &= port_amask;
//	entry = readport_lookup[LEVEL1_INDEX(address,24 ,1)];
//	if (entry >= SUBTABLE_BASE)
//		entry = lookup[LEVEL2_INDEX(entry,address,24 ,1)];
//
//	/* handle banks inline */
//	address -= rporthandler16[entry].offset;
//	if (entry <= STATIC_RAM)
//		MEMREADEND(*(data16_t *)&cpu_bankbase[entry][address])
//
//	/* fall back to the handler */
//	else
//	{
//		read16_handler handler = (read16_handler)rporthandler16[entry].handler;
//		MEMREADEND((*handler)(address >> 1,0))
//	}
//	return 0;																			\
//}																						\

enum
{
	V60_R0 = 1,
	V60_R1,
	V60_R2,
	V60_R3,
	V60_R4,
	V60_R5,
	V60_R6,
	V60_R7,
	V60_R8,
	V60_R9,
	V60_R10,
	V60_R11,
	V60_R12,
	V60_R13,
	V60_R14,
	V60_R15,
	V60_R16,
	V60_R17,
	V60_R18,
	V60_R19,
	V60_R20,
	V60_R21,
	V60_R22,
	V60_R23,
	V60_R24,
	V60_R25,
	V60_R26,
	V60_R27,
	V60_R28,
	V60_AP,
	V60_FP,
	V60_SP,
	V60_PC,
	V60_PSW,
	V60_U1,
	V60_U2,
	V60_ISP,
	V60_L0SP,
	V60_L1SP,
	V60_L2SP,
	V60_L3SP,
	V60_SBR,
	V60_TR,
	V60_SYCW,
	V60_TKCW,
	V60_PIR,
	V60_Res1,
	V60_Res2,
	V60_Res3,
	V60_Res4,
	V60_Res5,
	V60_PSW2,
	V60_ATBR0,
	V60_ATLR0,
	V60_ATBR1,
	V60_ATLR1,
	V60_ATBR2,
	V60_ATLR2,
	V60_ATBR3,
	V60_ATLR3,
	V60_TRMODE,
	V60_ADTR0,
	V60_ADTR1,
	V60_ADTMR0,
	V60_ADTMR1,
	V60_Res6,
	V60_Res7,
	V60_Res8,
	V60_REGMAX
};

#endif /* __V60_H__ */
