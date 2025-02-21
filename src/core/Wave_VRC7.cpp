/* 
 *  Copyright (C) ????		Ben Parnell / Quietust
 *  Copyright (C) 2004      Disch
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

/************************************************************************/
/* NOTICE!!!!!!!!!!!!
 *
 *		Most of this code was copy/pasted from FCEUltra's source.  I only made
 *	minor adjustments to add volume/pan control, and to get things working with
 *	the way I have NotSo set up.   Credit for most of this code goes to
 *  Xodnizel and Quietust.
 *
 *		Thanks to both of you guys!  ^_^
 *
 ************************************************************************/


//////////////////////////////////////////////////////////////////////////
//
//   Wave_VRC7.cpp
//

#include "config.h"


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define OSD_CPU_H				//defined here to prevent a few typedefs from being redefined in fmopl.h
#include <windows.h>
#endif
#include "NSF_Core.h"
#include "fmopl.h"


void CNSFCore::VRC7_Init()
{
	VRC7_Destroy();
	if(!pFMOPL)
	{	
		if(!( pFMOPL = OPLCreate(OPL_TYPE_WAVESEL,(int)(NTSC_FREQUENCY * 2),nSampleRate)))
		return;
	}
	VRC7_Reset();
}

void CNSFCore::VRC7_RecalcMultiplier(BYTE chan)
{
	float l,r;
	if(!pFMOPL) return;

	l = 1.5f * fMasterVolume * nChannelVol[chan + 19] / 255.0f;
	if(bFade)
		l *= fFadeVolume;

	((FM_OPL*)pFMOPL)->bInvert[chan] = bVRC7Inv[chan];

	if(nMonoStereo == 2)
	{
		r = l;
		if(nChannelPan[chan + 19] < 0)		r = (r * (127 + nChannelPan[chan + 19])) / 127.0f;
		if(nChannelPan[chan + 19] > 0)		l = (l * (127 - nChannelPan[chan + 19])) / 127.0f;

		((FM_OPL*)pFMOPL)->fLeftMultiplier[chan] = l;
		((FM_OPL*)pFMOPL)->fRightMultiplier[chan] = r;
	}
	else
		((FM_OPL*)pFMOPL)->fLeftMultiplier[chan] = l;
}

void CNSFCore::VRC7_Reset()
{
	int x;
	OPLResetChip((FM_OPL*)pFMOPL);

	for(x=0x1;x<0xF6;x++)
		OPLWrite((FM_OPL*)pFMOPL,x,0);
	OPLWrite((FM_OPL*)pFMOPL,0xBD,0xC0);
	OPLWrite((FM_OPL*)pFMOPL,1,0x20);      /* Enable waveform type manipulation */
	
	VRC7_RecalcMultiplier(0);
	VRC7_RecalcMultiplier(1);
	VRC7_RecalcMultiplier(2);
	VRC7_RecalcMultiplier(3);
	VRC7_RecalcMultiplier(4);
	VRC7_RecalcMultiplier(5);
	VRC7_ChangeInversionFreq();
}

void CNSFCore::VRC7_Destroy()
{
	if(!pFMOPL)
		return;

	OPLDestroy((FM_OPL*)pFMOPL);
	pFMOPL = NULL;
}

void CNSFCore::VRC7_Mix()
{
	if(bVRC7_FadeChanged)
	{
		VRC7_RecalcMultiplier(0);
		VRC7_RecalcMultiplier(1);
		VRC7_RecalcMultiplier(2);
		VRC7_RecalcMultiplier(3);
		VRC7_RecalcMultiplier(4);
		VRC7_RecalcMultiplier(5);
		bVRC7_FadeChanged = 0;
	}
	YM3812UpdateOne((FM_OPL*)pFMOPL,pVRC7Buffer,(int)(pOutput - pVRC7Buffer),&bChannelMix[14],nMonoStereo == 2);
	pVRC7Buffer = pOutput;
}

BYTE VRC7Instrument[16][8] = {

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},  //custom instrument
	
	//  Combination instrument values... made from a combination of the below 2 (best of both
	// so to speak.
{0x03,0x21,0x04,0x06,0x8D,0xF2,0x42,0x17},	//1
{0x13,0x41,0x10,0x0B,0xFF,0xF2,0x32,0xD6},	//2
{0x31,0x11,0x10,0x0A,0xF0,0x9C,0x32,0x02},	//3
{0x21,0x61,0x1D,0x07,0x9F,0x64,0x20,0x27},	//4
{0x22,0x21,0x1E,0x06,0xF0,0x76,0x08,0x28},	//5
{0x02,0x01,0x06,0x00,0xF0,0xF2,0x03,0x95},	//6
{0x21,0x61,0x1C,0x07,0x82,0x81,0x16,0x07},	//7
{0x23,0x21,0x1A,0x17,0xEF,0x82,0x25,0x15},	//8
{0x25,0x11,0x1F,0x00,0x86,0x41,0x20,0x11},	//9
{0x85,0x01,0x1F,0x0F,0xE4,0xA2,0x11,0x12},	//10
{0x07,0x81,0x2B,0x05,0xF4,0xF2,0x14,0xF4},	//11
{0x61,0x23,0x11,0x06,0x96,0x96,0x13,0x16},	//12
{0x01,0x02,0x20,0x00,0xF9,0x92,0x41,0x75},	//13
{0x61,0x22,0x0D,0x02,0xC3,0x7F,0x24,0x05},	//14
{0x21,0x62,0x0E,0x00,0xA1,0xA0,0x44,0x17}	//15

#if 0

	//  Old instrument values... they seem to work
{0x03,0x01,0x14,0x80,0xC2,0x90,0x43,0x14},	/* Currently working on this one */
{0x13,0x41,0x10,0x0B,0xFF,0xF2,0x32,0xD6},
{0x01,0x01,0x10,0x08,0xF0,0xF4,0x00,0x04},	/* 90% perfect */
{0x21,0x41,0x1B,0x08,0x66,0x80,0x30,0x85},
{0x22,0x21,0x20,0x03,0x75,0x70,0x24,0x14},
{0x02,0x01,0x06,0x00,0xF0,0xF2,0x03,0x95},	/* Do not touch! 98% perfect! */
{0x21,0x41,0x18,0x10,0x93,0xE0,0x21,0x15},
{0x01,0x22,0x13,0x00,0xF0,0x82,0x00,0x15},
{0x05,0x01,0x22,0x00,0x60,0xE3,0xA0,0xF5},	/* 90% perfect */
{0x85,0x01,0x20,0x00,0xD7,0xA2,0x22,0xF5},	/* 90% perfect */
{0x07,0x81,0x2B,0x05,0xF4,0xF2,0x14,0xF4},	/* 95% perfect */
{0x21,0x41,0x20,0x18,0xF3,0x80,0x13,0x95},
{0x01,0x02,0x20,0x00,0xF9,0x92,0x41,0x75},	/* Do not touch! 98% perfect! */
{0x21,0x62,0x0E,0x00,0x84,0x85,0x45,0x15},	/* 90% perfect */
{0x21,0x62,0x0E,0x00,0xA1,0xA0,0x34,0x16},	/* Do not touch! 98% perfect! */

		//  New instrument values... they're supposedly moste accurate, but
		// they mess up the sound when implimented.  Probably a flaw somewhere..
		// perhaps I messed something up when adding my weird nonsense... or perhaps
		// OPL2 emulation is screwing it up?  Who knows.

{0x03,0x21,0x04,0x06,0x8D,0xF2,0x42,0x17},
{0x13,0x41,0x05,0x0E,0x99,0x96,0x63,0x12},
{0x31,0x11,0x10,0x0A,0xF0,0x9C,0x32,0x02},
{0x21,0x61,0x1D,0x07,0x9F,0x64,0x20,0x27},
{0x22,0x21,0x1E,0x06,0xF0,0x76,0x08,0x28},
{0x02,0x01,0x06,0x00,0xF0,0xF2,0x03,0x95},
{0x21,0x61,0x1C,0x07,0x82,0x81,0x16,0x07},
{0x23,0x21,0x1A,0x17,0xEF,0x82,0x25,0x15},
{0x25,0x11,0x1F,0x00,0x86,0x41,0x20,0x11},
{0x85,0x01,0x1F,0x0F,0xE4,0xA2,0x11,0x12},
{0x07,0xC1,0x2B,0x45,0xB4,0xF1,0x24,0xF4},
{0x61,0x23,0x11,0x06,0x96,0x96,0x13,0x16},
{0x01,0x02,0xD3,0x05,0x82,0xA2,0x31,0x51},
{0x61,0x22,0x0D,0x02,0xC3,0x7F,0x24,0x05},
{0x21,0x62,0x0E,0x00,0xA1,0xA0,0x44,0x17}

#endif
};

const BYTE InstTrans[6] = {0x00,0x01,0x02,0x08,0x09,0x0A};

void CNSFCore::VRC7_LoadInstrument(BYTE Chan)
{
	if(!pFMOPL)
		return;

	const BYTE *i;
	BYTE x = InstTrans[Chan];
	BYTE y = (VRC7Chan[2][Chan] >> 4) & 0xF;
	
	i=VRC7Instrument[y];

	OPLWrite((FM_OPL*)pFMOPL,(0x20+x),i[0]);
	OPLWrite((FM_OPL*)pFMOPL,(0x23+x),i[1]);
	OPLWrite((FM_OPL*)pFMOPL,(0x40+x),i[2]);
	OPLWrite((FM_OPL*)pFMOPL,(0x43+x),((i[3] & 0xC0) | ((VRC7Chan[2][Chan] << 2) & 0x3C)));	// quiet
	OPLWrite((FM_OPL*)pFMOPL,0xe0+x,(i[3] >> 3) & 0x01);
	OPLWrite((FM_OPL*)pFMOPL,0xe3+x,(i[3] >> 4) & 0x01);
	OPLWrite((FM_OPL*)pFMOPL,0xC0+Chan,(i[3] << 1) & 0x0E);
	OPLWrite((FM_OPL*)pFMOPL,0x60+x,i[4]);
	OPLWrite((FM_OPL*)pFMOPL,0x63+x,i[5]);
	OPLWrite((FM_OPL*)pFMOPL,0x80+x,i[6]);
	OPLWrite((FM_OPL*)pFMOPL,0x83+x,i[7]);
}

void CNSFCore::VRC7_Write(BYTE V)
{
	BYTE x = nVRC7Address & 0x0F;
	BYTE y;

	switch(nVRC7Address >> 4)
	{
		case 0:
			if(x & 0x08) break;
				VRC7Instrument[0][x] = V;
			for(y = 0; y < 6; y++)
			{
				if(!(VRC7Chan[2][y] & 0xF0))
					VRC7_LoadInstrument(y);
			}
			break;

		case 1:
			if(x > 5) break;
			VRC7Chan[0][x] = V;
			((FM_OPL*)pFMOPL)->nFreqReg[x] = (((FM_OPL*)pFMOPL)->nFreqReg[x] & 0x0F00) | V;
			OPLWrite((FM_OPL*)pFMOPL,0xA0 + x,(VRC7Chan[0][x] << 1) & 0xFE);
			OPLWrite((FM_OPL*)pFMOPL,0xB0 + x,((VRC7Chan[0][x] >> 7) & 0x01) | ((VRC7Chan[1][x] << 1) & 0x3E));
			break;
		case 2:
			if(x>5) break;
			VRC7Chan[1][x] = V;
			((FM_OPL*)pFMOPL)->nFreqReg[x] = (((FM_OPL*)pFMOPL)->nFreqReg[x] & 0x00FF) | ((V & 0x0F) << 8);
			OPLWrite((FM_OPL*)pFMOPL,0xB0 + x,(((VRC7Chan[0][x] >> 7) & 0x01) | ((VRC7Chan[1][x] << 1) & 0x3E)));
			break;
		case 3:
			if(x>5) break;
			VRC7Chan[2][x] = V;
			VRC7_LoadInstrument(x);
			break;
	}
}

void CNSFCore::VRC7_ChangeInversion(BYTE chan,BYTE inv)
{
	if(pFMOPL)
		((FM_OPL*)pFMOPL)->bInvert[chan] = inv;
}

void CNSFCore::VRC7_ChangeInversionFreq()
{
	if(!pFMOPL)
		return;

	if(nInvertCutoffHz <= 0)
		((FM_OPL*)pFMOPL)->nInvertFreqCutoff = 0;

	((FM_OPL*)pFMOPL)->nInvertFreqCutoff = (0x80000 * nInvertCutoffHz) / 49722;
}