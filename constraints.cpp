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

// Method tests if there is hard constrain broken between actual shift and shift before
__inline int isLeftHardConstrainBroken(unsigned int index, int size, unsigned int shiftType, unsigned int* currentRoster)
{
	int left;
	if(getColumn(index, size) == 0)
	{
		if(_settings.isExtendedRoster)
			left = getExtendedRosterShift(getRow(index, size), _erRoster);
		else
			return 0;
	}
	else
	{
		left = getRosterShift(index-1, currentRoster);
	}
	if(((left == PM)&&((shiftType == NIGHT)||(shiftType == AM)))||((left == AM)&&(shiftType == NIGHT)))
		return 1;
	return 0;
}

// Method tests if there is hard constrain broken between actual and next shift
__inline int isRightHardConstrainBroken(unsigned int index, int size, unsigned int shiftType, unsigned int* currentRoster)
{
	int right;
	if(getColumn(index, size) == size-1)
		return 0;

	right = getRosterShift(index+1, currentRoster);
	if((((right == NIGHT)||(right == AM))&&(shiftType == PM))||((right == NIGHT)&&(shiftType == AM)))
		return 1;
	return 0;
}

// Method tests if there is hard constrain broken for actual shift
int isHardConstrainBroken(unsigned int index, int size, unsigned int shiftType, unsigned int* shiftMap, unsigned int isFirst, unsigned int* currentRoster)
{
	if(isFirst || !_settings.isLeftRightConstrainSoft)
	{
		if(isLeftHardConstrainBroken(index, size, shiftType, currentRoster)
				|| isRightHardConstrainBroken(index, size, shiftType, currentRoster))
			return 1;
	}
	if(_settings.isSevenDaySequenceConstrain)
	{
		unsigned int day, shifts, shift, frees, nurse, shiftMapNurse;
		int tmp = 0;
		int i, startDay;
		day = getColumn(index, size);
		nurse = getRow(index, size);
		shiftMapNurse = shiftMap[nurse];
		shiftMapNurse = shiftMapNurse&(~_freePreferenceMap[nurse]);
		shifts = 0;
		frees = 0;
		if(_settings.isExtendedRoster)
			startDay = day-6;	
		else
			startDay = day>5?day-6:0;
		for(i = startDay; i < startDay+7; i++)
		{
			if(i >= (int)_dayCount)
			{
				frees++;
				shifts = shifts<<1;
			}
			else
			{
				if(i != day)
				{
					if(i < 0)
						shift = (getBitOfBitArray(_erShiftMap, nurse*_dayCount-i))?1:0;
					else
						shift = (getBitOfBitArray(&shiftMapNurse, i))?1:0;
				}
				else
					shift = (shiftType==FREE)?0:1;
				shifts += shift;
				shifts = shifts << 1;
				if(!shift)
					frees++;
			}
		}
		if(frees < _sdscFreeDays[nurse])
		{
			return 1;
		}
		for(i = startDay+7; i<(int)day+7; i++) // Check all other seven-day sequencies
		{
			if(i < (int)_dayCount)
				shift = (getBitOfBitArray(&shiftMapNurse, i))?1:0;
			else
				return 0;
			if(!shift) frees++;
			if(!((shifts>>(7-tmp))&1))
				frees--;
			if(frees < _sdscFreeDays[nurse])
				return 1;
			tmp++;
		}
	}
	return 0;
}

// Method computes penalization of utility function for shift rostered in specific position at the current time
unsigned int CompatibilityPenalization(unsigned int index, int size, unsigned int shiftType, unsigned int* shiftMap, unsigned int isFirst,unsigned int* currentRoster)
{
	unsigned int left, right, conCount = 0, result = 0;
	unsigned int i = 2;

	if(getBitOfBitArray(shiftMap, index))
	{
		return INTMAX;
	}
	if(isHardConstrainBroken(index, size, shiftType, shiftMap, isFirst, currentRoster))
		return INTMAX;
	if(isFirst)
		return 0;

	if(_settings.isMPHeuristic == 1)
	{
		result += 2*_settings.mphPenalization;
		if(getColumn(index, size) == 0)
		{
			if(_settings.isExtendedRoster)
				left = getExtendedRosterShift(getRow(index, size), _erRoster);
			else
				left = FREE;
		}
		else
			left = getRosterShift(index-1, currentRoster);

		if(getColumn(index, size) == size-1)
			right = FREE;
		else
			right = getRosterShift(index+1, currentRoster);
		if((left!=FREE)||(right!=FREE))
			result -= _settings.mphPenalization;
		if((left!=FREE)&&(right!=FREE))
			result -= _settings.mphPenalization;
	}

	if(_settings.isMPHeuristic == 2)
	{
		result += 2*_settings.mphPenalization;
		if(getColumn(index, size) == 0)
		{
			if(_settings.isExtendedRoster)
				left = getExtendedRosterShift(getRow(index, size), _erRoster);
			else
				left = FREE;
		}
		else
			left = getOrigRosterShift(index-1);

		if(getColumn(index, size) == size-1)
			right = FREE;
		else
			right = getOrigRosterShift(index+1);
		if((!(((left == PM)&&((shiftType == NIGHT)||(shiftType == AM)))||((left == AM)&&(shiftType == NIGHT))))||(!((((right == NIGHT)||(right == AM))&&(shiftType == PM))||((right == NIGHT)&&(shiftType == AM)))))
			result -= _settings.mphPenalization;
		if((!(((left == PM)&&((shiftType == NIGHT)||(shiftType == AM)))||((left == AM)&&(shiftType == NIGHT))))&&(!((((right == NIGHT)||(right == AM))&&(shiftType == PM))||((right == NIGHT)&&(shiftType == AM)))))
			result -= _settings.mphPenalization;
	}
	return result;
}