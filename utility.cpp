/*
	This file is part of the NRRPCpu program.

	NRRPCpu is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	NRRPCpu is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License

*/

#include "nrrp.h"

//! Deep copy of array
/*!
\param destination Destination array pointer
\param source Source array pointer
\param length Length of array
*/
void copyArray(unsigned int* destination, unsigned int* source, unsigned int length)
{
	unsigned int i;
	for(i = 0; i < length; i++)
		destination[i] = source[i];
}

//! Addresing bit in aligned bit array
/*!
\param array Bit array
\param position Addres of bit
\return Value of particular bit
*/

unsigned int getBitOfBitArray(unsigned int* array, int position)
{
	//int pos = (position/_dayCount)*32+(position%_dayCount);
	//return array[pos/32]&(1<<(pos%32));
	return array[position/_dayCount]&(1<<(position%_dayCount));
}

//! Setting value of particular bit in aligned array
/*!
\param array Bit array
\param position Addres of bit
\param value Value of bit
*/
void setBitOfBitArray(unsigned int* array, int position, int value)
{
	int pos = (position/_dayCount)*32+(position%_dayCount);
	if(value)
		array[pos/32] = array[pos/32]|(1<<(pos%32));
	else
		array[pos/32] = array[pos/32]&(INTMAX - (1<<(pos%32)));
}

//! Index of row and column to one dimension index conversion
/*!
\param row Row indes (nurse)
\param column Column index (day)
\param size Number of columns (days)
\return One dimension index (nurse)
*/
unsigned int getIndex(unsigned int row, unsigned int column, unsigned int size)
{
	return row*size+column;
}

//! One dimension index to row index conversion
/*!
\param index One dimension index (Shift)
\param size Number of columns (days)
\return Index of row (nurse)
*/
unsigned int getRow(unsigned int index, unsigned int size)
{
	return index/size;
}

//! One dimension index to column index conversion
/*!
\param index One dimension index (Shift)
\param size Number of columns (days)
\return Index of row (nurse)
*/
unsigned int getColumn(unsigned int index, unsigned int size)
{
	return index%size;
}

//! Getting of nurse preferences (not used for instances by Moz&Pato)
/*
\param index Shift
\param shift Type of assigning shift
\return Preference of paricular sister to work the particular shift
*/
unsigned int getNursePreference(unsigned int index, unsigned int shift)
{
	return _nursePreferences[index*4+shift];
}
