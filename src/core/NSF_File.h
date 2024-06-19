/* 
 * Copyright (C) 2004      Disch
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

//////////////////////////////////////////////////////////////////////////
//
//  NSF_File.h
//
//

#ifndef __NSF_FILE_H_
#define __NSF_FILE_H_

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <windows.h>

#include "../config.h"

#include "stream.h"

#define					HEADERTYPE_NESM			'MSEN'
#define					HEADERTYPE_NSFE			'EFSN'

#define					CHUNKTYPE_INFO			'OFNI'
#define					CHUNKTYPE_DATA			'ATAD'
#define					CHUNKTYPE_NEND			'DNEN'
#define					CHUNKTYPE_PLST			'tslp'
#define					CHUNKTYPE_TIME			'emit'
#define					CHUNKTYPE_FADE			'edaf'
#define					CHUNKTYPE_TLBL			'lblt'
#define					CHUNKTYPE_AUTH			'htua'
#define					CHUNKTYPE_BANK			'KNAB'


struct NESM_HEADER
{
	UINT			nHeader;
	BYTE			nHeaderExtra;
	BYTE			nVersion;
	BYTE			nTrackCount;
	BYTE			nInitialTrack;
	WORD			nLoadAddress;
	WORD			nInitAddress;
	WORD			nPlayAddress;
	char			szGameTitle[32];
	char			szArtist[32];
	char			szCopyright[32];
	WORD			nSpeedNTSC;
	BYTE			nBankSwitch[8];
	WORD			nSpeedPAL;
	BYTE			nNTSC_PAL;
	BYTE			nExtraChip;
	BYTE			nExpansion[4];
};

struct NSFE_INFOCHUNK
{
	WORD			nLoadAddress;
	WORD			nInitAddress;
	WORD			nPlayAddress;
	BYTE			nIsPal;
	BYTE			nExt;
	BYTE			nTrackCount;
	BYTE			nStartingTrack;
};



class CNSFFile
{
public:
	CNSFFile() { ZeroMemory(this,sizeof(CNSFFile)); }
	~CNSFFile() { Destroy(); }
	int				LoadFile(LPCSTR path,BYTE needdata,BYTE ignoreversion);//Loads a file from a specified path.  If needdata is false,
														//  the NSF code is not loaded, only the other information
														//  (like track times, game title, Author, etc)
														//If you're loading an NSF with intention to play it, needdata
														//  must be true
	template <class S>
	int 			LoadStream(S* stream,BYTE needdata,BYTE ignoreversion);
	int				SaveFile(LPCSTR path);				//Saves the NSF to a file... including any changes you made (like to track times, etc)
	void			Destroy();							//Cleans up memory

protected:
	// int		LoadFile_NESM(FILE* file,BYTE needdata,BYTE ignoreversion);	//these functions are used internally and should not be called
	template <class S>
	int		LoadStream_NESM(S* stream,BYTE needdata,BYTE ignoreversion);
	// int		LoadFile_NSFE(FILE* file,BYTE needdata);
	template <class S>
	int 	LoadStream_NSFE(S* stream,BYTE needdata);

	int		SaveFile_NESM(FILE* file);
	int		SaveFile_NSFE(FILE* file);

public:

	///////////////////////////////////
	//data members

	//basic NSF info
	bool				bIsExtended;		//0 = NSF, 1 = NSFE
	BYTE				nIsPal;				//0 = NTSC, 1 = PAL, 2,3 = mixed NTSC/PAL (interpretted as NTSC)
	int					nLoadAddress;		//The address to which the NSF code is loaded to
	int					nInitAddress;		//The address of the Init routine (called at track change)
	int					nPlayAddress;		//The address of the Play routine (called several times a second)
	BYTE				nChipExtensions;	//Bitwise representation of the external chips used by this NSF.  Read NSFSpec.txt for details.
	
	//old NESM speed stuff (blarg)
	int					nNTSC_PlaySpeed;
	int					nPAL_PlaySpeed;

	//track info
	int					nTrackCount;		//The number of tracks in the NSF (1 = 1 track, 5 = 5 tracks, etc)
	int					nInitialTrack;		//The initial track (ZERO BASED:  0 = 1st track, 4 = 5th track, etc)

	//nsf data
	BYTE*				pDataBuffer;		//the buffer containing NSF code.  If needdata was false when loading the NSF, this is NULL
	int					nDataBufferSize;	//the size of the above buffer.  0 if needdata was false

	//playlist
	BYTE*				pPlaylist;			//the buffer containing the playlist (NULL if none exists).  Each entry is the zero based index of the song to play
	int					nPlaylistSize;		//the size of the above buffer (and the number of tracks in the playlist)

	//track time / fade
	int*				pTrackTime;			//the buffer containing the track times.  NULL if no track times specified.  Otherwise this buffer MUST BE (nTrackCount * sizeof(int)) in size
	int*				pTrackFade;			//the buffer containing the track fade times.  NULL if none are specified.  Same conditions as pTrackTime

	//track labels
	char**				szTrackLabels;		//the buffer containing track labels.  NULL if there are no labels.  There must be nTrackCount char pointers (or none if NULL).
											//Each pointer must point to it's own buffer containing a string (the length of this buffer doesn't matter, just so long as the string is NULL terminated)
											// the string's buffer may be NULL if a string isn't needed
											//szTrackLabels as well as all of the buffers it points to are destroyed upon
											// a call to Destroy (or the destructor).

	//string info
	char*				szGameTitle;		//pointer to a NULL-terminated string containing the name of the game.  Or NULL if no name specified
	char*				szArtist;			//pointer to a NULL-terminated string containing the author of the NSF.  Or NULL if none specified
	char*				szCopyright;		//pointer to a NULL-terminated string containing the copyright info.  Or NULL if none specified
	char*				szRipper;			//pointer to a NULL-terminated string containing the 'hacker' who ripped the NSF.  Or NULL if none specified

	//bankswitching info
	BYTE				nBankswitch[8];		//The initial bankswitching registers needed for some NSFs.  If the NSF does not use bankswitching, these values will all be zero
};

#include "NSF_Core.h"

#define SAFE_DELETE(p) { if(p){ delete[] p; p = NULL; } }
#define SAFE_NEW(p,t,s,r) p = new t[s]; if(!p) return r; ZeroMemory(p,sizeof(t) * s)

template <class S>
int 	CNSFFile::LoadStream(S *stream,BYTE needdata,BYTE ignoreversion) {
	Destroy();

	if(!stream) return -1;

	UINT type = 0;
	stream->read((uint8_t*)&type,4);
	int ret = -1;

	if(type == HEADERTYPE_NESM)		ret = LoadStream_NESM(stream,needdata,ignoreversion);
	if(type == HEADERTYPE_NSFE)		ret = LoadStream_NSFE(stream,needdata);

	stream->seek(SeekFrom::Start(0));

	// Snake's revenge puts '00' for the initial track, which (after subtracting 1) makes it 256 or -1 (bad!)
	// This prevents that crap
	if(nInitialTrack >= nTrackCount)
		nInitialTrack = 0;
	if(nInitialTrack < 0)
		nInitialTrack = 0;

	// if there's no tracks... this is a crap NSF
	if(nTrackCount < 1)
	{
		Destroy();
		return -1;
	}

	return ret;
}

template <class S>
int		CNSFFile::LoadStream_NESM(S* stream,BYTE needdata,BYTE ignoreversion) {
	int len;

	len = stream->seek(SeekFrom::End(0)) - 0x80;
	stream->seek(SeekFrom::Start(0));

	if(len < 1) return -1;

	//read the info
	NESM_HEADER					hdr;
	stream->read((uint8_t*)&hdr,0x80);

	//confirm the header
	if(hdr.nHeader != HEADERTYPE_NESM)			return -1;
	if(hdr.nHeaderExtra != 0x1A)				return -1;
	if((!ignoreversion) && (hdr.nVersion != 1))	return -1; //stupid NSFs claim to be above version 1  >_>

	//NESM is generally easier to work with (but limited!)
	//  just move the data over from NESM_HEADER over to our member data

	bIsExtended =				0;
	nIsPal =					hdr.nNTSC_PAL & 0x03;
	nPAL_PlaySpeed =			hdr.nSpeedPAL;			//blarg
	nNTSC_PlaySpeed =			hdr.nSpeedNTSC;			//blarg
	nLoadAddress =				hdr.nLoadAddress;
	nInitAddress =				hdr.nInitAddress;
	nPlayAddress =				hdr.nPlayAddress;
	nChipExtensions =			hdr.nExtraChip;


	nTrackCount =				hdr.nTrackCount;
	nInitialTrack =				hdr.nInitialTrack - 1;	//stupid 1-based number =P

	memcpy(nBankswitch,hdr.nBankSwitch,8);

	SAFE_NEW(szGameTitle,char,33,1);
	SAFE_NEW(szArtist   ,char,33,1);
	SAFE_NEW(szCopyright,char,33,1);

	memcpy(szGameTitle,hdr.szGameTitle,32);
	memcpy(szArtist   ,hdr.szArtist   ,32);
	memcpy(szCopyright,hdr.szCopyright,32);

	//read the NSF data
	if(needdata)
	{
		SAFE_NEW(pDataBuffer,BYTE,len,1);
		stream->read(pDataBuffer,len);
		nDataBufferSize = len;
	}

	//if we got this far... it was a successful read
	return 0;
}

template <class S>
int 	CNSFFile::LoadStream_NSFE(S* stream,BYTE needdata) {
	//restart the file
	stream->seek(SeekFrom::Start(0));

	//the vars we'll be using
	UINT nChunkType;
	int  nChunkSize;
	int  nChunkUsed;
	int  nDataPos = 0;
	BYTE	bInfoFound = 0;
	BYTE	bEndFound = 0;
	BYTE	bBankFound = 0;

	NSFE_INFOCHUNK	info;
	ZeroMemory(&info,sizeof(NSFE_INFOCHUNK));
	info.nTrackCount = 1;		//default values

	//confirm the header!
	stream->read((uint8_t*)&nChunkType,4);
	if(nChunkType != HEADERTYPE_NSFE)			return -1;

	//begin reading chunks
	while(!bEndFound)
	{
		if(!stream->read((uint8_t*)&nChunkSize,4))		return -1;
		stream->read((uint8_t*)&nChunkType,4);

		switch(nChunkType)
		{
		case CHUNKTYPE_INFO:
			if(bInfoFound)						return -1;	//only one info chunk permitted
			if(nChunkSize < 8)					return -1;	//minimum size

			bInfoFound = 1;
			nChunkUsed = MIN((int)sizeof(NSFE_INFOCHUNK),nChunkSize);

			stream->read((uint8_t*)&info,nChunkUsed);
			stream->seek(SeekFrom::Current(nChunkSize - nChunkUsed));

			bIsExtended =			1;
			nIsPal =				info.nIsPal & 3;
			nLoadAddress =			info.nLoadAddress;
			nInitAddress =			info.nInitAddress;
			nPlayAddress =			info.nPlayAddress;
			nChipExtensions =		info.nExt;
			nTrackCount =			info.nTrackCount;
			nInitialTrack =			info.nStartingTrack;

			nPAL_PlaySpeed =		(WORD)(1000000 / PAL_NMIRATE);		//blarg
			nNTSC_PlaySpeed =		(WORD)(1000000 / NTSC_NMIRATE);		//blarg
			break;

		case CHUNKTYPE_DATA:
			if(!bInfoFound)						return -1;
			if(nDataPos)						return -1;
			if(nChunkSize < 1)					return -1;

			nDataBufferSize = nChunkSize;
			nDataPos = stream->seek(SeekFrom::Current(0));

			stream->seek(SeekFrom::Current(nChunkSize));
			break;

		case CHUNKTYPE_NEND:
			bEndFound = 1;
			break;

		case CHUNKTYPE_TIME:
			if(!bInfoFound)						return -1;
			if(pTrackTime)						return -1;

			SAFE_NEW(pTrackTime,int,nTrackCount,1);
			nChunkUsed = MIN(nChunkSize / 4,nTrackCount);

			stream->read((uint8_t*)pTrackTime,nChunkUsed*4);
			stream->seek(SeekFrom::Current(nChunkSize - (nChunkUsed * 4)));

			for(; nChunkUsed < nTrackCount; nChunkUsed++)
				pTrackTime[nChunkUsed] = -1;	//negative signals to use default time

			break;

		case CHUNKTYPE_FADE:
			if(!bInfoFound)						return -1;
			if(pTrackFade)						return -1;

			SAFE_NEW(pTrackFade,int,nTrackCount,1);
			nChunkUsed = MIN(nChunkSize / 4,nTrackCount);

			stream->read((uint8_t*)pTrackFade,nChunkUsed*4);
			stream->seek(SeekFrom::Current(nChunkSize - (nChunkUsed * 4)));

			for(; nChunkUsed < nTrackCount; nChunkUsed++)
				pTrackFade[nChunkUsed] = -1;	//negative signals to use default time

			break;

		case CHUNKTYPE_BANK:
			if(bBankFound)						return -1;

			bBankFound = 1;
			nChunkUsed = MIN(8,nChunkSize);

			stream->read(nBankswitch,nChunkUsed);
			stream->seek(SeekFrom::Current(nChunkSize - nChunkUsed));
			break;

		case CHUNKTYPE_PLST:
			if(pPlaylist)						return -1;

			nPlaylistSize = nChunkSize;
			if(nPlaylistSize < 1)				break;  //no playlist?

			SAFE_NEW(pPlaylist,BYTE,nPlaylistSize,1);
			stream->read(pPlaylist,nChunkSize);
			break;

		case CHUNKTYPE_AUTH:		{
			if(szGameTitle)						return -1;

			char*		buffer;
			char*		ptr;
			SAFE_NEW(buffer,char,nChunkSize + 4,1);

			stream->read((uint8_t*)buffer,nChunkSize);
			ptr = buffer;

			char**		ar[4] = {&szGameTitle,&szArtist,&szCopyright,&szRipper};
			int			i;
			for(i = 0; i < 4; i++)
			{
				nChunkUsed = lstrlen(ptr) + 1;
				*ar[i] = new char[nChunkUsed];
				if(!*ar[i]) { SAFE_DELETE(buffer); return 0; }
				memcpy(*ar[i],ptr,nChunkUsed);
				ptr += nChunkUsed;
			}
			SAFE_DELETE(buffer);
									}break;

		case CHUNKTYPE_TLBL:		{
			if(!bInfoFound)						return -1;
			if(szTrackLabels)					return -1;

			SAFE_NEW(szTrackLabels,char*,nTrackCount,1);

			char*		buffer;
			char*		ptr;
			SAFE_NEW(buffer,char,nChunkSize + nTrackCount,1);

			stream->read((uint8_t*)buffer,nChunkSize);
			ptr = buffer;

			int			i;
			for(i = 0; i < nTrackCount; i++)
			{
				nChunkUsed = lstrlen(ptr) + 1;
				szTrackLabels[i] = new char[nChunkUsed];
				if(!szTrackLabels[i]) { SAFE_DELETE(buffer); return 0; }
				memcpy(szTrackLabels[i],ptr,nChunkUsed);
				ptr += nChunkUsed;
			}
			SAFE_DELETE(buffer);
									}break;

		default:		//unknown chunk
			nChunkType &= 0x000000FF;  //check the first byte
			if((nChunkType >= 'A') && (nChunkType <= 'Z'))	//chunk is vital... don't continue
				return -1;
			//otherwise, just skip it
			stream->seek(SeekFrom::Current(nChunkSize));

			break;
		}		//end switch
	}			//end while

	//if we exited the while loop without a 'return', we must have hit an NEND chunk
	//  if this is the case, the file was layed out as it was expected.
	//  now.. make sure we found both an info chunk, AND a data chunk... since these are
	//  minimum requirements for a valid NSFE file

	if(!bInfoFound)			return -1;
	if(!nDataPos)			return -1;

	//if both those chunks existed, this file is valid.  Load the data if it's needed

	if(needdata)
	{
		stream->seek(SeekFrom::Start(nDataPos));
		SAFE_NEW(pDataBuffer,BYTE,nDataBufferSize,1);
		stream->read(pDataBuffer,nDataBufferSize);
	}
	else
		nDataBufferSize = 0;

	//return success!
	return 0;
}

#endif
