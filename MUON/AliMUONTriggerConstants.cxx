/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id$ */

#include "AliMUONTriggerConstants.h"

ClassImp(AliMUONTriggerConstants)

//----------------------------------------------------------------------
AliMUONTriggerConstants::AliMUONTriggerConstants()
  : TNamed()
{
// constructor
    ;
}
//----------------------------------------------------------------------
AliMUONTriggerConstants::~AliMUONTriggerConstants()
{
// destructor
    ;
}

//----------------------------------------------------------------------
const Int_t AliMUONTriggerConstants::fgkNmodule = 126;

//----------------------------------------------------------------------
// conv : line-column (line : from top to bottom, column : from left to right)
const Int_t AliMUONTriggerConstants::fgkModuleId[126] = 
{11,12,13,14,15,16,17,         // right side of the chamber
 21,22,23,24,25,26,27,
 31,32,33,34,35,36,37,
 41,42,43,44,45,46,47,
 51,52,53,54,55,56,57,
 61,62,63,64,65,66,67,
 71,72,73,74,75,76,77,
 81,82,83,84,85,86,87,
 91,92,93,94,95,96,97,   
 -11,-12,-13,-14,-15,-16,-17,  // right side of chamber
 -21,-22,-23,-24,-25,-26,-27,
 -31,-32,-33,-34,-35,-36,-37,
 -41,-42,-43,-44,-45,-46,-47,
 -51,-52,-53,-54,-55,-56,-57,
 -61,-62,-63,-64,-65,-66,-67,
 -71,-72,-73,-74,-75,-76,-77,
 -81,-82,-83,-84,-85,-86,-87,
 -91,-92,-93,-94,-95,-96,-97};

//----------------------------------------------------------------------
const Int_t AliMUONTriggerConstants::fgkNstripX[126]=
{16,16,16,16,16,16,16,  // right side of the chamber 
 32,32,32,32,32,32,16,
 32,32,32,32,32,32,16,
 48,64,64,32,32,32,16,
 0,64,64,32,32,32,16,
 48,64,64,32,32,32,16,
 32,32,32,32,32,32,16,
 32,32,32,32,32,32,16,
 16,16,16,16,16,16,16,  // left side of the chamber
 16,16,16,16,16,16,16,
 32,32,32,32,32,32,16,
 32,32,32,32,32,32,16,
 48,64,64,32,32,32,16,
 0,64,64,32,32,32,16,
 48,64,64,32,32,32,16,
 32,32,32,32,32,32,16,
 32,32,32,32,32,32,16,
 16,16,16,16,16,16,16};

//----------------------------------------------------------------------  
const Int_t AliMUONTriggerConstants::fgkNstripY[126]=
{ 8, 8, 8, 8, 8, 8,16,  // right side of the chamber
  8, 8, 8, 8, 8, 8,16,
  16,16,16,16,16, 8,16,
  16,16,16,16,16, 8,16,
  0, 8,16,16,16, 8,16,
  16,16,16,16,16, 8,16,
  16,16,16,16,16, 8,16,
  8, 8, 8, 8, 8, 8,16,
  8, 8, 8, 8, 8, 8,16,  
  8, 8, 8, 8, 8, 8,16,  // left side of the chamber
  8, 8, 8, 8, 8, 8,16,
  16,16,16,16,16, 8,16,
  16,16,16,16,16, 8,16,
  0, 8,16,16,16, 8,16,
  16,16,16,16,16, 8,16,
  16,16,16,16,16, 8,16,
  8, 8, 8, 8, 8, 8,16,
  8, 8, 8, 8, 8, 8,16};

//----------------------------------------------------------------------  
const Float_t AliMUONTriggerConstants::fgkXcMin[126]=
{-36.,-70.,-104.,-138.,-172.,-206.,-274., //right
 -36.,-70.,-104.,-138.,-172.,-206.,-274.,
 -36.,-70.,-104.,-138.,-172.,-206.,-274.,
 -36.,-70.,-104.,-138.,-172.,-206.,-274.,
   0.,-70.,-104.,-138.,-172.,-206.,-274.,
 -36.,-70.,-104.,-138.,-172.,-206.,-274.,
 -36.,-70.,-104.,-138.,-172.,-206.,-274.,
 -36.,-70.,-104.,-138.,-172.,-206.,-274.,
 -36.,-70.,-104.,-138.,-172.,-206.,-274.,
 2.,36.,70.,104.,138.,172.,206., // left
 2.,36.,70.,104.,138.,172.,206.,
 2.,36.,70.,104.,138.,172.,206.,
 2.,36.,70.,104.,138.,172.,206.,
 0.,53.,70.,104.,138.,172.,206.,
 2.,36.,70.,104.,138.,172.,206.,
 2.,36.,70.,104.,138.,172.,206.,
 2.,36.,70.,104.,138.,172.,206.,
 2.,36.,70.,104.,138.,172.,206.,
};

//----------------------------------------------------------------------  
const Float_t AliMUONTriggerConstants::fgkXcMax[126]=
{-2.,-36.,-70.,-104.,-138.,-172.,-206., // right
 -2.,-36.,-70.,-104.,-138.,-172.,-206.,
 -2.,-36.,-70.,-104.,-138.,-172.,-206.,
 -2.,-36.,-70.,-104.,-138.,-172.,-206.,
  0.,-53.,-70.,-104.,-138.,-172.,-206.,
 -2.,-36.,-70.,-104.,-138.,-172.,-206.,
 -2.,-36.,-70.,-104.,-138.,-172.,-206.,
 -2.,-36.,-70.,-104.,-138.,-172.,-206.,
 -2.,-36.,-70.,-104.,-138.,-172.,-206.,
 36.,70.,104.,138.,172.,206.,274., //left
 36.,70.,104.,138.,172.,206.,274.,
 36.,70.,104.,138.,172.,206.,274.,
 36.,70.,104.,138.,172.,206.,274.,
  0.,70.,104.,138.,172.,206.,274.,
 36.,70.,104.,138.,172.,206.,274.,
 36.,70.,104.,138.,172.,206.,274.,
 36.,70.,104.,138.,172.,206.,274.,
 36.,70.,104.,138.,172.,206.,274., 
};

//----------------------------------------------------------------------  
const Int_t AliMUONTriggerConstants::fgkCircuitId[234]=
{111, 121, 131, 141, 151, 161, 171,
 211, 212, 221, 222, 231, 232, 241, 242, 251, 252, 261, 262, 271,
 311, 312, 321, 322, 331, 332, 341, 342, 351, 352, 361, 362, 371,
 411, 412, 413, 421, 422, 423, 424, 431, 432, 433, 434, 441, 442, 
 451, 452, 461, 462, 471,
 521, 522, 523, 524, 531, 532, 533, 534, 541, 542, 551, 552, 561, 562, 571, 
 611, 612, 613, 621, 622, 623, 624, 631, 632, 633, 634, 641, 642,
 651, 652, 661, 662, 671,
 711, 712, 721, 722, 731, 732, 741, 742, 751, 752, 761, 762, 771,
 811, 812, 821, 822, 831, 832, 841, 842, 851, 852, 861, 862, 871,
 911, 921, 931, 941, 951, 961, 971,
 -111, -121, -131, -141, -151, -161, -171,
 -211, -212, -221, -222, -231, -232, -241, -242, -251, -252, -261, -262, -271,
 -311, -312, -321, -322, -331, -332, -341, -342, -351, -352, -361, -362, -371,
 -411, -412, -413, -421, -422, -423, -424, -431, -432, -433, -434, -441, -442, 
 -451, -452, -461, -462, -471,
 -521, -522, -523, -524, -531, -532, -533, -534, -541, -542, -551, -552, -561, -562, -571, 
 -611, -612, -613, -621, -622, -623, -624, -631, -632, -633, -634, -641, -642,
 -651, -652, -661, -662, -671,
 -711, -712, -721, -722, -731, -732, -741, -742, -751, -752, -761, -762, -771,
 -811, -812, -821, -822, -831, -832, -841, -842, -851, -852, -861, -862, -871,
 -911, -921, -931, -941, -951, -961, -971};



//----------------------------------------------------------------------
// methods which returns public static const data
//----------------------------------------------------------------------
Int_t AliMUONTriggerConstants::Nmodule()
{
// returns fgkNmodule
    return fgkNmodule;    
}
//----------------------------------------------------------------------
Int_t AliMUONTriggerConstants::ModuleId(Int_t imodule)
{
// returns fgkModuleId
    return fgkModuleId[imodule];
}
//----------------------------------------------------------------------
Int_t AliMUONTriggerConstants::NstripX(Int_t imodule) 
{
// returns fgkNstripX
    return fgkNstripX[imodule];
}
//----------------------------------------------------------------------
Int_t AliMUONTriggerConstants::NstripY(Int_t imodule)
{
// return fgkNstripY
    return fgkNstripY[imodule];
}
//----------------------------------------------------------------------
Float_t AliMUONTriggerConstants::XcMin(Int_t imodule)
{
// returns fgkXcMin
    return fgkXcMin[imodule];
}
//----------------------------------------------------------------------
Float_t AliMUONTriggerConstants::XcMax(Int_t imodule)
{
// returns fgkXcMax
    return fgkXcMax[imodule];
}
//----------------------------------------------------------------------
Int_t AliMUONTriggerConstants::CircuitId(Int_t icircuit)
{
// returns fgkCircuitId
    return fgkCircuitId[icircuit];
}











