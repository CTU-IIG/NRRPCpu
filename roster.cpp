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

//! Setting of shift type of particular shift in new roster \f$R\f$
/*!
\param roster Current new roster \f$R\f$
\param index Shift
\param shift Type of assigning shift
*/
void setRosterShift(unsigned int* roster, unsigned int index, int shift)
{
	int ind = (index/_dayCount)*32+(index%_dayCount);
	int indm16 = ind%16;
	ind = ind/16;
	/*unsigned int orTmp = shift << (2*(ind%16));
	unsigned int andTmp = INTMAX;
	andTmp = (((andTmp << 2)+shift+1) << (2*(ind%16)))-1;
	roster[ind/16] = roster[ind/16]&andTmp;
	roster[ind/16] = roster[ind/16]|orTmp;*/
	roster[ind] = roster[ind]&((((INTMAX << 2)+shift+1) << (2*(indm16)))-1);
	roster[ind] = roster[ind]|(shift << (2*(indm16)));;
}

//! Setting of shift type of particular shift in new roster \f$R\f$
/*!
\param index Shift
\param shift Type of assigning shift
*/
void setOrigRosterShift(unsigned int index, int shift)
{
	int ind = (index/_dayCount)*32+(index%_dayCount);
	int indm16 = ind%16;
	ind = ind/16;
	/*unsigned int orTmp = shift << (2*(ind%16));
	unsigned int andTmp = INTMAX;
	andTmp = (((andTmp << 2)+shift+1) << (2*(ind%16)))-1;
	_originalRoster[ind/16] = _originalRoster[ind/16]&andTmp;
	_originalRoster[ind/16] = _originalRoster[ind/16]|orTmp;*/
	_originalRoster[ind] = _originalRoster[ind]&((((INTMAX << 2)+shift+1) << (2*(indm16)))-1);
	_originalRoster[ind] = _originalRoster[ind]|(shift << (2*(indm16)));
}


//! Setting of shift type of particular shift in extended roster \f$R^E\f$
/*!
\param roster Extended roster \f$R^E\f$
\param index Shift
\param shift Type of assigning shift
*/
void setExtendedRosterShift(unsigned int* roster, unsigned int index, int shift)
{
	int ind = index/16;
	int indm16 = index%16;
	/*unsigned int orTmp = shift << (2*(ind%16));
	unsigned int andTmp = INTMAX;
	andTmp = (((andTmp << 2)+shift+1) << (2*(ind%16)))-1;
	roster[ind/16] = roster[ind/16]&andTmp;
	roster[ind/16] = roster[ind/16]|orTmp;*/
	roster[ind] = roster[ind]&((((INTMAX << 2)+shift+1) << (2*(indm16)))-1);
	roster[ind] = roster[ind]|(shift << (2*(indm16)));;
}

//! Addresing of particular shift in original roster \f$R^0\f$
/*!
\param index Shift
\return Shift type of particular shift in \f$R^0\f$
*/
int getOrigRosterShift(unsigned int index)
{
	int ind = (index/_dayCount)*32+(index%_dayCount);
	int indm16 = ind%16;
	return (_originalRoster[ind/16]&(3<<(2*(indm16))))>>(2*(indm16));
}

//! Getting of shift type of particular shift in new roster \f$R\f$
/*!
\param index Shift
\param currentRoster Current new roster \f$R\f$
\return Shift type of particular shift in \f$R\f$
*/
int getRosterShift(unsigned int index, unsigned int* currentRoster)
{
	/*int i;
	if(getBitOfBitArray(shiftMap, index) == 0) // If is cell empty return FREE shift
		return FREE;

	for(i = 0; i < _absenceCount; i++)
	{
		if(_shiftChangeArrayInit[i] == index)
			return FREE;
	}

	for(i = 0; i < _shiftChangeMaximum; i++) // Search in changes
	{
		if(shiftChangeArray[2*i] == index) // If index is in changes, return shift type of new shift
		{
				return getOrigRosterShift(shiftChangeArray[2*i+1]);
		}
		if(shiftChangeArray[2*i] == -1) // If there is no another change break.
			break;
	}

	return getOrigRosterShift(index); //Otherwise return shift from original roster*/
	int ind = (index/_dayCount)*32+(index%_dayCount);
	int indm16 = ind%16;
	return (currentRoster[ind/16]&(3<<(2*(indm16))))>>(2*(indm16));
}

//! Getting of shift type of particular shift in extended roster \f$R^E\f$
/*!
\param index Nurse index
\param currentRoster Extended roster \f$R^E\f$
\return Shift type of particular shift in \f$R^E\f$
*/
int getExtendedRosterShift(unsigned int index, unsigned int* currentRoster)
{
	int ind = index/16;
	int indm16 = index%16;
	return (currentRoster[ind]&(3<<(2*(indm16))))>>(2*(indm16));
}

//! Test of roster sequences admissibility (not used for Moz&Pato instances)
/*!
\param actual Actual shift
\param NIGHTs Number of nights shifts in sequence
\param AMs Number of early shifts in sequence
\param PMs Number of late shifts in sequence
\return Sequence admissibility flag
*/
__inline int minShiftTypeConsecutiveConstrainTest(int actual, int NIGHTs, int AMs, int PMs)
{
	if((actual != NIGHT)&&(NIGHTs != 0)&&(NIGHTs < (int)_settings.mmhcMinShiftTypeConsecutive))
		return 1;
	if((actual != AM)&&(AMs != 0)&&(AMs < (int)_settings.mmhcMinShiftTypeConsecutive))
		return 1;
	if((actual != PM)&&(PMs != 0)&&(PMs < (int)_settings.mmhcMinShiftTypeConsecutive))
		return 1;
	return 0;
}

int utilityFunction2(unsigned int* currentRoster)
{
	int i;
	int result = 0;
	for(i = 0; i <_nurseCount*_dayCount; i++)
	{
		if(getRosterShift(i, currentRoster) == getOrigRosterShift(i) || ((getBitOfBitArray(_realAbsenceArray, i) != 0)&&(getRosterShift(i, currentRoster) == FREE)))
			result += 0;
		else
			result += 1000;
	}
	return result;
}

//! Utility function computation
/*!
\param currentRoster Current new rosters \f$R^0\f$
\return Value of utility function
*/
int utilityFunction(unsigned int* currentRoster)
{
	unsigned int i;
	int result = 0;
	int shift;
	int NIGHTs = 0, AMs = 0, PMs = 0, FREEs = 0; //consecutive days
	unsigned int countNIGHT = 0, countAM = 0, countNight = 0;
	unsigned int dutyCount = 0, consecutiveDutyCount = 0;
	for(i = 0; i <= _nurseCount*_dayCount; i++)
	{
		if((getColumn(i, _dayCount) == 0) && (getRow(i, _dayCount) > 0))
		{
			if(_settings.isSevenDaySequenceConstrain)
			{
				//if(_settings.sdscFreeDays[getRow(i, _dayCount)-1] == 1)
				//result += _settings.fairnessPenalization*((dutyCount - _fairnessAverage)>0?(dutyCount-_fairnessAverage):(_fairnessAverage-dutyCount));
			}
			else
			{
				result += _settings.fairnessPenalization*((dutyCount - _fairnessAverage)>0?(dutyCount-_fairnessAverage):(_fairnessAverage-dutyCount));
			}
			if((_settings.isMinMaxHardConstrain)&&(dutyCount < _settings.mmhcMinWorking))
				result += _settings.softInfeasiblePenalization;
			if((_settings.isMinMaxHardConstrain)&&(countNIGHT < _settings.mmhcMinShiftType)&&(countAM < _settings.mmhcMinShiftType)&&(countNight < _settings.mmhcMinShiftType))
				result += _settings.softInfeasiblePenalization;
			if(getRow(i, _dayCount) >= _nurseCount)
				break;
			dutyCount = 0;
			consecutiveDutyCount = 0;
			countNIGHT = countAM = countNight = 0;
		}
		shift = getRosterShift(i, currentRoster);
		if(_settings.isNursePreferences)
			result += getNursePreference(i, shift);
		if(shift != FREE)
			dutyCount++;
		if(shift != getOrigRosterShift(i))
			result += _settings.changePenalization;
		if(getColumn(i, _dayCount) == 0)
			AMs = PMs = FREEs = NIGHTs = 0;
		if(shift == NIGHT)
		{
			if((_settings.isMinMaxHardConstrain)&&(minShiftTypeConsecutiveConstrainTest(NIGHT, NIGHTs, AMs, PMs)))
				result += _settings.softInfeasiblePenalization;
			AMs = PMs = FREEs = 0;
			consecutiveDutyCount++;
			NIGHTs++;
			countNIGHT++;
		}
		else if(shift == AM)
		{
			if((_settings.isMinMaxHardConstrain)&&(minShiftTypeConsecutiveConstrainTest(AM, NIGHTs, AMs, PMs)))
				result += _settings.softInfeasiblePenalization;
			PMs = FREEs = NIGHTs = 0;
			consecutiveDutyCount++;
			AMs++;
			countAM++;
			if(AMs > 3)
				result += _settings.mfcPmPenalization;
		}
		else if(shift == PM)
		{
			if((_settings.isMinMaxHardConstrain)&&(minShiftTypeConsecutiveConstrainTest(PM, NIGHTs, AMs, PMs)))
				result += _settings.softInfeasiblePenalization;
			AMs = FREEs = NIGHTs = 0;
			consecutiveDutyCount++;
			PMs++;
			countNight++;
			if(PMs > 1)
				result += _settings.mfcNightPenalization;
		}
		else if(shift == FREE)
		{
			if((_settings.isMinMaxHardConstrain)&&(consecutiveDutyCount!=0)&&(consecutiveDutyCount < _settings.mmhcMinConsecutive))
				result += _settings.softInfeasiblePenalization;
			if((_settings.isMinMaxHardConstrain)&&(minShiftTypeConsecutiveConstrainTest(FREE, NIGHTs, AMs, PMs)))
				result += _settings.softInfeasiblePenalization;
			AMs = PMs = NIGHTs = 0;
			FREEs++;
			consecutiveDutyCount = 0;
			if(FREEs > 3)
				result += _settings.mfcFreePenalization;
		}

		// Fatal error algorithm test
		/*if(isHardConstrainBroken(i, _dayCount, getRosterShift(i, shiftMap, shiftChangeArray), shiftMap, shiftChangeArray))
		{
			printRoster(shiftMap, shiftChangeArray);
			isHardConstrainBroken(i, _dayCount, getRosterShift(i, shiftMap, shiftChangeArray), shiftMap, shiftChangeArray);
			error("Algorithm is broken");
			return INTMAX;
		}*/
	}
	return result - _settings.changePenalization*_initialChangeCount;
}

//! Shuffeling of oredered list of shifts
/*!
\param shiftList Ordered list of shifts \f$SL\f$
\param isLocal Local search flag
\param probability Actual probability for local search
*/

void checkRoaster(unsigned int* currentRoster)
{
	int i, j;
	int nPM, nAM, nNIGHT;
	int firstDayOfAbsence = _FirstAbsenceDay;
	for(i = 0; i < _dayCount; i++)
	{
		nPM = 0;
		nAM = 0;
		nNIGHT = 0;
		for(j = 0; j < _nurseCount; j++)
		{
			//if((firstDayOfAbsence == -1)&&(getBitOfBitArray(_absenceArray, j*_dayCount+i) != 0))
			//	firstDayOfAbsence = i;
			if((getBitOfBitArray(_freePreferenceMap, j*_dayCount + i) != 0) && (getRosterShift(j*_dayCount + i, currentRoster) != FREE))
			{
				printf("Roster is not feasible (absence) day: %d nurse: %d!\n", i, j);
			}
			switch(getRosterShift(j*_dayCount+i, currentRoster))
			{
			case PM: nPM++; break;
			case AM: nAM++; break;
			case NIGHT: nNIGHT++; break;
			default: break;
			}
			if(isLeftHardConstrainBroken(j*_dayCount+i, _dayCount, getRosterShift(j*_dayCount+i, currentRoster), currentRoster))
			{
				printf("Roster is not feasible (left) day: %d nurse: %d!\n", i, j);
				//return;
			}
			if(isRightHardConstrainBroken(j*_dayCount+i, _dayCount, getRosterShift(j*_dayCount+i, currentRoster), currentRoster))
			{
				printf("Roster is not feasible (right) day: %d nurse: %d!\n", i, j);
				//return;
			}
			
		}
		if((nNIGHT < _matrixRS[3*i]) || (nAM < _matrixRS[3*i+1]) || (nPM < _matrixRS[3*i+2]))
		{
			printf("Roster is not feasible (matrix RS) - day: %d!\n", i);
			//return;
		}
	}

	int ind;
	for(ind = 0; ind < _dayCount*_nurseCount; ind++)
	{
		int day;
		int subsum, isFree;
		for(i = -6; i <= 0; i++)
		{
			subsum = 0;
			for(j = 0; j < 7; j++)
			{
				isFree = 0;
				day = i + j + getColumn(ind, _dayCount);
				if(day == getColumn(ind, _dayCount))
				{
					isFree = (getRosterShift(ind, currentRoster)==FREE)?1:0;
				}
				else if(day < 0)
				{
					if(_settings.isExtendedRoster)
						isFree = (getBitOfBitArray(_erShiftMap, getRow(ind, _dayCount)*_dayCount-day))?0:1;
					else
						isFree = 1;
				}
				else if(day >= _dayCount)
				{
					isFree = 1;
				}
				else
				{
					isFree = (getRosterShift(getRow(ind, _dayCount)*_dayCount+day, currentRoster)==FREE)?1:0;
				}
				subsum += isFree;
			}
			if(subsum < _sdscFreeDays[getRow(ind, _dayCount)])
			{
				printf("Roster is not feasible (seven) day: %d nurse: %d!\n", getColumn(ind, _dayCount), getRow(ind, _dayCount));
				//return;
			}
		}
	}
	if(firstDayOfAbsence != -1)
	{
		for(i = 0; i < firstDayOfAbsence; i++)
		{
			for(j = 0; j < _nurseCount; j++)
			{
				if(getRosterShift(j*_dayCount+i, currentRoster) != getOrigRosterShift(j*_dayCount+i))
				{
					printRoster(_originalRoster);
					printf("Roster is not feasible (orig) day: %d nurse: %d!\n", i, j);
				}
			}
		}
	}
	printf("Checked! First day of absence: %d\n", firstDayOfAbsence);
}
void shuffleShiftList(unsigned int* shiftList, int isLocal, float probability)
{
	unsigned int i, j;
	int swap, swapIndex;
	if(isLocal)
	{
		copyArray(shiftList, _bestShiftList, _shiftToReroster);
	}
	for(i = 0; i < _dayCount; i++)
	{
		for(j = 0; j < _nurseCount; j++)
		{
			if((!isLocal)||((rand()/(double)(RAND_MAX+1.0)) < probability))
			{
				swapIndex = (unsigned int)((rand()/(double)(RAND_MAX+1.0))*_dayCount);
				swapIndex = swapIndex*_nurseCount+j;
				swap = shiftList[swapIndex];
				shiftList[swapIndex] = shiftList[i*_nurseCount+j];
				shiftList[i*_nurseCount+j] = swap;
			}
		}
	}
}
