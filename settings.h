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
	
#ifndef SETTINGS_H_
#define SETTINGS_H_

//! Instances type
enum DataSetType
{
MOZPATO = 0, //!< Instances by Moz a Pato
MAENHOUT = 1 //!< Instances by Maenhout a Vanhoucke
};

//! Stricture with algorithm settings
struct Settings{
	unsigned int changePenalization; //!< Penalization for roster disruption

	unsigned int isFairness; //!< Is fairness algorithm used?
	unsigned int fairnessPenalization; //!< Penalization for fairness disruption

	unsigned int isSevenDaySequenceConstrain; //!< Does algorithm use minimal number of free days at every seven day sequence constrain?

	unsigned int isLeftRightConstrainSoft; //!< Is 16 free hours between two shifts constrain handled as soft constrain
	unsigned int lrcsPenalization; //!< Penalization for 16 free hours between two shifts constrain disruption

	unsigned int isExtendedRoster; //!< Is extended roster used?

	unsigned int isDayShiftTypeMin; //!< Is used matrix \f$RS\f$

	unsigned int isShiftListOptimalization; //!< Is used optimalization of ordered list of shifts length
	
	unsigned int isMinMaxHardConstrain; //!< Does use algorithm Min&Max constrains
	unsigned int mmhcMinWorking; //!< Minimal number of assigned shifts
	unsigned int mmhcMaxWorking; //!< Maximal number of assigned shifts
	unsigned int mmhcMinConsecutive; //!< Minimal number of assigned shifts in sequece
	unsigned int mmhcMaxConsecutive; //!< Maximal number of assigned shifts in sequnce
	unsigned int mmhcMinShiftType; //!< Minimal number of assigned shifts of one shift type
	unsigned int mmhcMaxShiftType; //!< Maximal number of assigned shifts of one shift type
	unsigned int mmhcMinShiftTypeConsecutive; //!< Minimal number of assigned shifts of one shift type in sequence
	unsigned int mmhcMaxShiftTypeConsecutive; //!< Maximal number of assigned shifts of one shift type in sequence

	unsigned int softInfeasiblePenalization; //!< Treshold of roster infeasibility

	unsigned int isNursePreferences; //!< Does algorithm use nurse preferences?

	unsigned int isFreezeShiftSupport; //!< Does algorithm use freezed shift?
	
	DataSetType inputDataSetType; //!< Instance types - 0=Moz&Pato, 1=Maenhout

	unsigned int isMulticommodityFlowConstrins; //!< Does algorithm use soft constrains of article by Moz&Pato about solving NRRP by multicomodity flows
	unsigned int mfcNightPenalization; //!< Penalization of night shifts in sequence
	unsigned int mfcPmPenalization; //!< Penalization of more than 3 late shifts in sequence
	unsigned int mfcFreePenalization; //!< Penalization of more than 3 free shifts in sequence

	unsigned int isMPHeuristic; //!< Does algorithm use compatibility definition by Moz&Pato?
	unsigned int mphPenalization; //!< Penalization of not compatible shifts

};

extern Settings _settings;

void setMaenhout();
void setMozPato(); 
#endif /* SETTINGS_H_ */
