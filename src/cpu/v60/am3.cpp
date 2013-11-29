
// AM3 Functions (for ReadAM)
// **************************

UINT32 am3Register()
{
	switch (modDim)
	{
	case 0:
		SETREG8(v60.reg[modVal & 0x1F], modWriteValB);
		break;
	case 1:
		SETREG16(v60.reg[modVal & 0x1F], modWriteValH);
		break;
	case 2:
		v60.reg[modVal & 0x1F] = modWriteValW;
		break;
	}

	return 1;
}

UINT32 am3RegisterIndirect()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(v60.reg[modVal & 0x1F], modWriteValB);
		break;
	case 1:
		MemWrite16(v60.reg[modVal & 0x1F], modWriteValH);
		break;
	case 2:
		MemWrite32(v60.reg[modVal & 0x1F], modWriteValW);
		break;
	}

	return 1;
}

UINT32 am3RegisterIndirectIndexed()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(v60.reg[modVal2 & 0x1F] + v60.reg[modVal & 0x1F], modWriteValB);
		break;
	case 1:
		MemWrite16(v60.reg[modVal2 & 0x1F] + v60.reg[modVal & 0x1F] * 2, modWriteValH);
		break;
	case 2:
		MemWrite32(v60.reg[modVal2 & 0x1F] + v60.reg[modVal & 0x1F] * 4, modWriteValW);
		break;
	}

	return 2;
}

UINT32 am3Autoincrement()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(v60.reg[modVal & 0x1F], modWriteValB);
		v60.reg[modVal & 0x1F] += 1;
		break;
	case 1:
		MemWrite16(v60.reg[modVal & 0x1F], modWriteValH);
		v60.reg[modVal & 0x1F] += 2;
		break;
	case 2:
		MemWrite32(v60.reg[modVal & 0x1F], modWriteValW);
		v60.reg[modVal & 0x1F] += 4;
		break;
	}

	return 1;
}

UINT32 am3Autodecrement()
{
	switch (modDim)
	{
	case 0:
		v60.reg[modVal & 0x1F] -= 1;
		MemWrite8(v60.reg[modVal & 0x1F], modWriteValB);
		break;
	case 1:
		v60.reg[modVal & 0x1F] -= 2;
		MemWrite16(v60.reg[modVal & 0x1F], modWriteValH);
		break;
	case 2:
		v60.reg[modVal & 0x1F] -= 4;
		MemWrite32(v60.reg[modVal & 0x1F], modWriteValW);
		break;
	}

	return 1;
}

UINT32 am3Displacement8()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(v60.reg[modVal & 0x1F] + (INT8)MemRead8(modAdd + 1), modWriteValB);
		break;
	case 1:
		MemWrite16(v60.reg[modVal & 0x1F] + (INT8)MemRead8(modAdd + 1), modWriteValH);
		break;
	case 2:
		MemWrite32(v60.reg[modVal & 0x1F] + (INT8)MemRead8(modAdd + 1), modWriteValW);
		break;
	}

	return 2;
}

UINT32 am3Displacement16()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(v60.reg[modVal & 0x1F] + (INT16)MemRead16(modAdd + 1), modWriteValB);
		break;
	case 1:
		MemWrite16(v60.reg[modVal & 0x1F] + (INT16)MemRead16(modAdd + 1), modWriteValH);
		break;
	case 2:
		MemWrite32(v60.reg[modVal & 0x1F] + (INT16)MemRead16(modAdd + 1), modWriteValW);
		break;
	}

	return 3;
}

UINT32 am3Displacement32()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(v60.reg[modVal & 0x1F] + MemRead32(modAdd + 1), modWriteValB);
		break;
	case 1:
		MemWrite16(v60.reg[modVal & 0x1F] + MemRead32(modAdd + 1), modWriteValH);
		break;
	case 2:
		MemWrite32(v60.reg[modVal & 0x1F] + MemRead32(modAdd + 1), modWriteValW);
		break;
	}

	return 5;
}


UINT32 am3DisplacementIndexed8()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(v60.reg[modVal2 & 0x1F] + (INT8)MemRead8(modAdd + 2) + v60.reg[modVal & 0x1F], modWriteValB);
		break;
	case 1:
		MemWrite16(v60.reg[modVal2 & 0x1F] + (INT8)MemRead8(modAdd + 2) + v60.reg[modVal & 0x1F] * 2, modWriteValH);
		break;
	case 2:
		MemWrite32(v60.reg[modVal2 & 0x1F] + (INT8)MemRead8(modAdd + 2) + v60.reg[modVal & 0x1F] * 4, modWriteValW);
		break;
	}

	return 3;
}

UINT32 am3DisplacementIndexed16()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(v60.reg[modVal2 & 0x1F] + (INT16)MemRead16(modAdd + 2) + v60.reg[modVal & 0x1F], modWriteValB);
		break;
	case 1:
		MemWrite16(v60.reg[modVal2 & 0x1F] + (INT16)MemRead16(modAdd + 2) + v60.reg[modVal & 0x1F] * 2, modWriteValH);
		break;
	case 2:
		MemWrite32(v60.reg[modVal2 & 0x1F] + (INT16)MemRead16(modAdd + 2) + v60.reg[modVal & 0x1F] * 4, modWriteValW);
		break;
	}

	return 4;
}

UINT32 am3DisplacementIndexed32()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(v60.reg[modVal2 & 0x1F] + MemRead32(modAdd + 2) + v60.reg[modVal & 0x1F], modWriteValB);
		break;
	case 1:
		MemWrite16(v60.reg[modVal2 & 0x1F] + MemRead32(modAdd + 2) + v60.reg[modVal & 0x1F] * 2, modWriteValH);
		break;
	case 2:
		MemWrite32(v60.reg[modVal2 & 0x1F] + MemRead32(modAdd + 2) + v60.reg[modVal & 0x1F] * 4, modWriteValW);
		break;
	}

	return 6;
}


UINT32 am3PCDisplacement8()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(PC + (INT8)MemRead8(modAdd + 1), modWriteValB);
		break;
	case 1:
		MemWrite16(PC + (INT8)MemRead8(modAdd + 1), modWriteValH);
		break;
	case 2:
		MemWrite32(PC + (INT8)MemRead8(modAdd + 1), modWriteValW);
		break;
	}

	return 2;
}

UINT32 am3PCDisplacement16()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(PC + (INT16)MemRead16(modAdd + 1), modWriteValB);
		break;
	case 1:
		MemWrite16(PC + (INT16)MemRead16(modAdd + 1), modWriteValH);
		break;
	case 2:
		MemWrite32(PC + (INT16)MemRead16(modAdd + 1), modWriteValW);
		break;
	}

	return 3;
}

UINT32 am3PCDisplacement32()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(PC + MemRead32(modAdd + 1), modWriteValB);
		break;
	case 1:
		MemWrite16(PC + MemRead32(modAdd + 1), modWriteValH);
		break;
	case 2:
		MemWrite32(PC + MemRead32(modAdd + 1), modWriteValW);
		break;
	}

	return 5;
}

UINT32 am3PCDisplacementIndexed8()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(PC + (INT8)MemRead8(modAdd + 2) + v60.reg[modVal & 0x1F], modWriteValB);
		break;
	case 1:
		MemWrite16(PC + (INT8)MemRead8(modAdd + 2) + v60.reg[modVal & 0x1F] * 2, modWriteValH);
		break;
	case 2:
		MemWrite32(PC + (INT8)MemRead8(modAdd + 2) + v60.reg[modVal & 0x1F] * 4, modWriteValW);
		break;
	}

	return 3;
}

UINT32 am3PCDisplacementIndexed16()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(PC + (INT16)MemRead16(modAdd + 2) + v60.reg[modVal & 0x1F], modWriteValB);
		break;
	case 1:
		MemWrite16(PC + (INT16)MemRead16(modAdd + 2) + v60.reg[modVal & 0x1F] * 2, modWriteValH);
		break;
	case 2:
		MemWrite32(PC + (INT16)MemRead16(modAdd + 2) + v60.reg[modVal & 0x1F] * 4, modWriteValW);
		break;
	}

	return 4;
}

UINT32 am3PCDisplacementIndexed32()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(PC + MemRead32(modAdd + 2) + v60.reg[modVal & 0x1F], modWriteValB);
		break;
	case 1:
		MemWrite16(PC + MemRead32(modAdd + 2) + v60.reg[modVal & 0x1F] * 2, modWriteValH);
		break;
	case 2:
		MemWrite32(PC + MemRead32(modAdd + 2) + v60.reg[modVal & 0x1F] * 4, modWriteValW);
		break;
	}

	return 6;
}

UINT32 am3DisplacementIndirect8()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(v60.reg[modVal & 0x1F] + (INT8)MemRead8(modAdd + 1)), modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(v60.reg[modVal & 0x1F] + (INT8)MemRead8(modAdd + 1)), modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(v60.reg[modVal & 0x1F] + (INT8)MemRead8(modAdd + 1)), modWriteValW);
		break;
	}

	return 2;
}

UINT32 am3DisplacementIndirect16()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(v60.reg[modVal & 0x1F] + (INT16)MemRead16(modAdd + 1)), modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(v60.reg[modVal & 0x1F] + (INT16)MemRead16(modAdd + 1)), modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(v60.reg[modVal & 0x1F] + (INT16)MemRead16(modAdd + 1)), modWriteValW);
		break;
	}

	return 3;
}

UINT32 am3DisplacementIndirect32()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(v60.reg[modVal & 0x1F] + MemRead32(modAdd + 1)), modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(v60.reg[modVal & 0x1F] + MemRead32(modAdd + 1)), modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(v60.reg[modVal & 0x1F] + MemRead32(modAdd + 1)), modWriteValW);
		break;
	}

	return 5;
}


UINT32 am3DisplacementIndirectIndexed8()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(v60.reg[modVal2 & 0x1F] + (INT8)MemRead8(modAdd + 2)) + v60.reg[modVal & 0x1F], modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(v60.reg[modVal2 & 0x1F] + (INT8)MemRead8(modAdd + 2)) + v60.reg[modVal & 0x1F] * 2, modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(v60.reg[modVal2 & 0x1F] + (INT8)MemRead8(modAdd + 2)) + v60.reg[modVal & 0x1F] * 4, modWriteValW);
		break;
	}

	return 3;
}

UINT32 am3DisplacementIndirectIndexed16()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(v60.reg[modVal2 & 0x1F] + (INT16)MemRead16(modAdd + 2)) + v60.reg[modVal & 0x1F], modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(v60.reg[modVal2 & 0x1F] + (INT16)MemRead16(modAdd + 2)) + v60.reg[modVal & 0x1F] * 2, modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(v60.reg[modVal2 & 0x1F] + (INT16)MemRead16(modAdd + 2)) + v60.reg[modVal & 0x1F] * 4, modWriteValW);
		break;
	}

	return 4;
}

UINT32 am3DisplacementIndirectIndexed32()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(v60.reg[modVal2 & 0x1F] + MemRead32(modAdd + 2)) + v60.reg[modVal & 0x1F], modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(v60.reg[modVal2 & 0x1F] + MemRead32(modAdd + 2)) + v60.reg[modVal & 0x1F] * 2, modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(v60.reg[modVal2 & 0x1F] + MemRead32(modAdd + 2)) + v60.reg[modVal & 0x1F] * 4, modWriteValW);
		break;
	}

	return 6;
}

UINT32 am3PCDisplacementIndirect8()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(PC + (INT8)MemRead8(modAdd + 1)), modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(PC + (INT8)MemRead8(modAdd + 1)), modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(PC + (INT8)MemRead8(modAdd + 1)), modWriteValW);
		break;
	}

	return 2;
}

UINT32 am3PCDisplacementIndirect16()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(PC + (INT16)MemRead16(modAdd + 1)), modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(PC + (INT16)MemRead16(modAdd + 1)), modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(PC + (INT16)MemRead16(modAdd + 1)), modWriteValW);
		break;
	}

	return 3;
}

UINT32 am3PCDisplacementIndirect32()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(PC + MemRead32(modAdd + 1)), modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(PC + MemRead32(modAdd + 1)), modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(PC + MemRead32(modAdd + 1)), modWriteValW);
		break;
	}

	return 5;
}


UINT32 am3PCDisplacementIndirectIndexed8()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(PC + (INT8)MemRead8(modAdd + 2)) + v60.reg[modVal & 0x1F], modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(PC + (INT8)MemRead8(modAdd + 2)) + v60.reg[modVal & 0x1F] * 2, modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(PC + (INT8)MemRead8(modAdd + 2)) + v60.reg[modVal & 0x1F] * 4, modWriteValW);
		break;
	}

	return 3;
}

UINT32 am3PCDisplacementIndirectIndexed16()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(PC + (INT16)MemRead16(modAdd + 2)) + v60.reg[modVal & 0x1F], modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(PC + (INT16)MemRead16(modAdd + 2)) + v60.reg[modVal & 0x1F] * 2, modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(PC + (INT16)MemRead16(modAdd + 2)) + v60.reg[modVal & 0x1F] * 4, modWriteValW);
		break;
	}

	return 4;
}

UINT32 am3PCDisplacementIndirectIndexed32()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(PC + MemRead32(modAdd + 2)) + v60.reg[modVal & 0x1F], modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(PC + MemRead32(modAdd + 2)) + v60.reg[modVal & 0x1F] * 2, modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(PC + MemRead32(modAdd + 2)) + v60.reg[modVal & 0x1F] * 4, modWriteValW);
		break;
	}

	return 6;
}


UINT32 am3DoubleDisplacement8()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(v60.reg[modVal & 0x1F] + (INT8)MemRead8(modAdd + 1)) + (INT8)MemRead8(modAdd + 2), modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(v60.reg[modVal & 0x1F] + (INT8)MemRead8(modAdd + 1)) + (INT8)MemRead8(modAdd + 2), modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(v60.reg[modVal & 0x1F] + (INT8)MemRead8(modAdd + 1)) + (INT8)MemRead8(modAdd + 2), modWriteValW);
		break;
	}

	return 3;
}

UINT32 am3DoubleDisplacement16()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(v60.reg[modVal & 0x1F] + (INT16)MemRead16(modAdd + 1)) + (INT16)MemRead16(modAdd + 3), modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(v60.reg[modVal & 0x1F] + (INT16)MemRead16(modAdd + 1)) + (INT16)MemRead16(modAdd + 3), modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(v60.reg[modVal & 0x1F] + (INT16)MemRead16(modAdd + 1)) + (INT16)MemRead16(modAdd + 3), modWriteValW);
		break;
	}

	return 5;
}

UINT32 am3DoubleDisplacement32()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(v60.reg[modVal & 0x1F] + MemRead32(modAdd + 1)) + MemRead32(modAdd + 5), modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(v60.reg[modVal & 0x1F] + MemRead32(modAdd + 1)) + MemRead32(modAdd + 5), modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(v60.reg[modVal & 0x1F] + MemRead32(modAdd + 1)) + MemRead32(modAdd + 5), modWriteValW);
		break;
	}

	return 9;
}


UINT32 am3PCDoubleDisplacement8()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(PC + (INT8)MemRead8(modAdd + 1)) + (INT8)MemRead8(modAdd + 2), modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(PC + (INT8)MemRead8(modAdd + 1)) + (INT8)MemRead8(modAdd + 2), modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(PC + (INT8)MemRead8(modAdd + 1)) + (INT8)MemRead8(modAdd + 2), modWriteValW);
		break;
	}

	return 3;
}

UINT32 am3PCDoubleDisplacement16()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(PC + (INT16)MemRead16(modAdd + 1)) + (INT16)MemRead16(modAdd + 3), modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(PC + (INT16)MemRead16(modAdd + 1)) + (INT16)MemRead16(modAdd + 3), modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(PC + (INT16)MemRead16(modAdd + 1)) + (INT16)MemRead16(modAdd + 3), modWriteValW);
		break;
	}

	return 5;
}

UINT32 am3PCDoubleDisplacement32()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(PC + MemRead32(modAdd + 1)) + MemRead32(modAdd + 5), modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(PC + MemRead32(modAdd + 1)) + MemRead32(modAdd + 5), modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(PC + MemRead32(modAdd + 1)) + MemRead32(modAdd + 5), modWriteValW);
		break;
	}

	return 9;
}

UINT32 am3DirectAddress()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(modAdd + 1), modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(modAdd + 1), modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(modAdd + 1), modWriteValW);
		break;
	}

	return 5;
}

UINT32 am3DirectAddressIndexed()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(modAdd + 2) + v60.reg[modVal & 0x1F], modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(modAdd + 2) + v60.reg[modVal & 0x1F] * 2, modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(modAdd + 2) + v60.reg[modVal & 0x1F] * 4, modWriteValW);
		break;
	}

	return 6;
}

UINT32 am3DirectAddressDeferred()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(MemRead32(modAdd + 1)), modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(MemRead32(modAdd + 1)), modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(MemRead32(modAdd + 1)), modWriteValW);
		break;
	}

	return 5;
}

UINT32 am3DirectAddressDeferredIndexed()
{
	switch (modDim)
	{
	case 0:
		MemWrite8(MemRead32(MemRead32(modAdd + 2)) + v60.reg[modVal & 0x1F], modWriteValB);
		break;
	case 1:
		MemWrite16(MemRead32(MemRead32(modAdd + 2)) + v60.reg[modVal & 0x1F], modWriteValH);
		break;
	case 2:
		MemWrite32(MemRead32(MemRead32(modAdd + 2)) + v60.reg[modVal & 0x1F], modWriteValW);
		break;
	}

	return 6;
}

UINT32 am3Immediate()
{
	fatalerror("CPU - AM3 - IMM (PC=%06x)", PC);
	return 0; /* never reached, fatalerror won't return */
}

UINT32 am3ImmediateQuick()
{
	fatalerror("CPU - AM3 - IMMQ (PC=%06x)", PC);
	return 0; /* never reached, fatalerror won't return */
}



// AM3 Tables (for ReadAMAddress)
// ******************************

UINT32 am3Error1()
{
	fatalerror("CPU - AM3 - 1 (PC=%06x)", PC);
	return 0; /* never reached, fatalerror won't return */
}

UINT32 am3Error2()
{
	fatalerror("CPU - AM3 - 2 (PC=%06x)", PC);
	return 0; /* never reached, fatalerror won't return */
}

#ifdef UNUSED_FUNCTION
UINT32 am3Error3()
{
	fatalerror("CPU - AM3 - 3 (PC=%06x)", PC);
	return 0; /* never reached, fatalerror won't return */
}
#endif

UINT32 am3Error4()
{
	fatalerror("CPU - AM3 - 4 (PC=%06x)", PC);
	return 0; /* never reached, fatalerror won't return */
}

UINT32 am3Error5()
{
	fatalerror("CPU - AM3 - 5 (PC=%06x)", PC);
	return 0; /* never reached, fatalerror won't return */
}

UINT32 (*const AMTable3_G7a[16])(void) =
{
	am3PCDisplacementIndexed8,
	am3PCDisplacementIndexed16,
	am3PCDisplacementIndexed32,
	am3DirectAddressIndexed,
	am3Error5,
	am3Error5,
	am3Error5,
	am3Error5,
	am3PCDisplacementIndirectIndexed8,
	am3PCDisplacementIndirectIndexed16,
	am3PCDisplacementIndirectIndexed32,
	am3DirectAddressDeferredIndexed,
	am3Error5,
	am3Error5,
	am3Error5,
	am3Error5
};

UINT32 am3Group7a()
{
	if (!(modVal2 & 0x10))
		return am3Error4();

	return AMTable3_G7a[modVal2 & 0xF]();
}

UINT32 (*const AMTable3_G7[32])(void) =
{
	am3ImmediateQuick,
	am3ImmediateQuick,
	am3ImmediateQuick,
	am3ImmediateQuick,
	am3ImmediateQuick,
	am3ImmediateQuick,
	am3ImmediateQuick,
	am3ImmediateQuick,
	am3ImmediateQuick,
	am3ImmediateQuick,
	am3ImmediateQuick,
	am3ImmediateQuick,
	am3ImmediateQuick,
	am3ImmediateQuick,
	am3ImmediateQuick,
	am3ImmediateQuick,
	am3PCDisplacement8,
	am3PCDisplacement16,
	am3PCDisplacement32,
  am3DirectAddress,
	am3Immediate,
	am3Error2,
	am3Error2,
	am3Error2,
  am3PCDisplacementIndirect8,
  am3PCDisplacementIndirect16,
  am3PCDisplacementIndirect32,
	am3DirectAddressDeferred,
	am3PCDoubleDisplacement8,
	am3PCDoubleDisplacement16,
	am3PCDoubleDisplacement32,
  am3Error2
};

UINT32 (*const AMTable3_G6[8])(void) =
{
	am3DisplacementIndexed8,
	am3DisplacementIndexed16,
	am3DisplacementIndexed32,
	am3RegisterIndirectIndexed,
	am3DisplacementIndirectIndexed8,
	am3DisplacementIndirectIndexed16,
	am3DisplacementIndirectIndexed32,
	am3Group7a
};




UINT32 am3Group6()
{
	modVal2 = MemRead8(modAdd + 1);
	return AMTable3_G6[modVal2 >> 5]();
}


UINT32 am3Group7()
{
	return AMTable3_G7[modVal & 0x1F]();
}



UINT32 (*const AMTable3[2][8])(void) =
{
	{
		am3Displacement8,
		am3Displacement16,
		am3Displacement32,
		am3RegisterIndirect,
		am3DisplacementIndirect8,
		am3DisplacementIndirect16,
		am3DisplacementIndirect32,
		am3Group7
	},

	{
		am3DoubleDisplacement8,
		am3DoubleDisplacement16,
		am3DoubleDisplacement32,
		am3Register,
		am3Autoincrement,
		am3Autodecrement,
		am3Group6,
		am3Error1
	}
};
