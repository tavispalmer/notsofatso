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
//  NSF_File.cpp
//
//

#include <exception>
#include <stdio.h>

#include "file.h"
#include "NSF_Core.h"
#include "NSF_File.h"

int		CNSFFile::LoadFile(LPCSTR path,BYTE needdata,BYTE ignoreversion)
{
	try {
		File file = File::open(path);
		return CNSFFile::LoadStream(&file,needdata,ignoreversion);
	} catch (const std::exception &e) {
		return -1;
	}
}

void	CNSFFile::Destroy()
{
	SAFE_DELETE(pDataBuffer);
	SAFE_DELETE(pPlaylist);
	SAFE_DELETE(pTrackTime);
	SAFE_DELETE(pTrackFade);
	if(szTrackLabels)
	{
		for(int i = 0; i < nTrackCount; i++)
			SAFE_DELETE(szTrackLabels[i]);
		SAFE_DELETE(szTrackLabels);
	}
	SAFE_DELETE(szGameTitle);
	SAFE_DELETE(szArtist);
	SAFE_DELETE(szCopyright);
	SAFE_DELETE(szRipper);

	ZeroMemory(this,sizeof(CNSFFile));
}

//////////////////////////////////////////////////////////////////////////
//  File saving

int CNSFFile::SaveFile(LPCSTR path)
{
	if(!pDataBuffer)		//if we didn't grab the data, we can't save it
		return 1;

	FILE* file = fopen(path,"wb");
	if(file == NULL) return 1;

	int ret;
	if(bIsExtended)		ret = SaveFile_NSFE(file);
	else				ret = SaveFile_NESM(file);

	fclose(file);
	return ret;
}

int CNSFFile::SaveFile_NESM(FILE* file)
{
	NESM_HEADER			hdr;
	ZeroMemory(&hdr,0x80);

	hdr.nHeader =				HEADERTYPE_NESM;
	hdr.nHeaderExtra =			0x1A;
	hdr.nVersion =				1;
	hdr.nTrackCount =			nTrackCount;
	hdr.nInitialTrack =			nInitialTrack + 1;
	hdr.nLoadAddress =			nLoadAddress;
	hdr.nInitAddress =			nInitAddress;
	hdr.nPlayAddress =			nPlayAddress;

	if(szGameTitle)				memcpy(hdr.szGameTitle,szGameTitle,MIN(lstrlen(szGameTitle),31));
	if(szArtist)				memcpy(hdr.szArtist   ,szArtist   ,MIN(lstrlen(szArtist)   ,31));
	if(szCopyright)				memcpy(hdr.szCopyright,szCopyright,MIN(lstrlen(szCopyright),31));

	hdr.nSpeedNTSC =			nNTSC_PlaySpeed;
	memcpy(hdr.nBankSwitch,nBankswitch,8);
	hdr.nSpeedPAL =				nPAL_PlaySpeed;
	hdr.nNTSC_PAL =				nIsPal;
	hdr.nExtraChip =			nChipExtensions;

	//the header is all set... slap it in
	fwrite(&hdr,0x80,1,file);

	//slap in the NSF info
	fwrite(pDataBuffer,nDataBufferSize,1,file);

	//we're done.. all the other info that isn't recorded is dropped for regular NSFs
	return 0;
}

int CNSFFile::SaveFile_NSFE(FILE* file)
{
	//////////////////////////////////////////////////////////////////////////
	// I must admit... NESM files are a bit easier to work with than NSFEs =P

	UINT			nChunkType;
	int				nChunkSize;
	NSFE_INFOCHUNK	info;

	//write the header
	nChunkType = HEADERTYPE_NSFE;
	fwrite(&nChunkType,4,1,file);


	//write the info chunk
	nChunkType =			CHUNKTYPE_INFO;
	nChunkSize =			sizeof(NSFE_INFOCHUNK);
	info.nExt =				nChipExtensions;
	info.nInitAddress =		nInitAddress;
	info.nIsPal =			nIsPal;
	info.nLoadAddress =		nLoadAddress;
	info.nPlayAddress =		nPlayAddress;
	info.nStartingTrack =	nInitialTrack;
	info.nTrackCount =		nTrackCount;

	fwrite(&nChunkSize,4,1,file);
	fwrite(&nChunkType,4,1,file);
	fwrite(&info,nChunkSize,1,file);

	//if we need bankswitching... throw it in
	for(nChunkSize = 0; nChunkSize < 8; nChunkSize++)
	{
		if(nBankswitch[nChunkSize])
		{
			nChunkType = CHUNKTYPE_BANK;
			nChunkSize = 8;
			fwrite(&nChunkSize,4,1,file);
			fwrite(&nChunkType,4,1,file);
			fwrite(nBankswitch,nChunkSize,1,file);
			break;
		}
	}

	//if there's a time chunk, slap it in
	if(pTrackTime)
	{
		nChunkType =		CHUNKTYPE_TIME;
		nChunkSize =		4 * nTrackCount;
		fwrite(&nChunkSize,4,1,file);
		fwrite(&nChunkType,4,1,file);
		fwrite(pTrackTime,nChunkSize,1,file);
	}

	//slap in a fade chunk if needed
	if(pTrackFade)
	{
		nChunkType =		CHUNKTYPE_FADE;
		nChunkSize =		4 * nTrackCount;
		fwrite(&nChunkSize,4,1,file);
		fwrite(&nChunkType,4,1,file);
		fwrite(pTrackFade,nChunkSize,1,file);
	}

	//auth!
	if(szGameTitle || szCopyright || szArtist || szRipper)
	{
		nChunkType =		CHUNKTYPE_AUTH;
		nChunkSize =		4;
		if(szGameTitle)		nChunkSize += lstrlen(szGameTitle);
		if(szArtist)		nChunkSize += lstrlen(szArtist);
		if(szCopyright)		nChunkSize += lstrlen(szCopyright);
		if(szRipper)		nChunkSize += lstrlen(szRipper);
		fwrite(&nChunkSize,4,1,file);
		fwrite(&nChunkType,4,1,file);

		if(szGameTitle)		fwrite(szGameTitle,lstrlen(szGameTitle) + 1,1,file);
		else				fwrite("",1,1,file);
		if(szArtist)		fwrite(szArtist,lstrlen(szArtist) + 1,1,file);
		else				fwrite("",1,1,file);
		if(szCopyright)		fwrite(szCopyright,lstrlen(szCopyright) + 1,1,file);
		else				fwrite("",1,1,file);
		if(szRipper)		fwrite(szRipper,lstrlen(szRipper) + 1,1,file);
		else				fwrite("",1,1,file);
	}

	//plst
	if(pPlaylist)
	{
		nChunkType =		CHUNKTYPE_PLST;
		nChunkSize =		nPlaylistSize;
		fwrite(&nChunkSize,4,1,file);
		fwrite(&nChunkType,4,1,file);
		fwrite(pPlaylist,nChunkSize,1,file);
	}

	//tlbl
	if(szTrackLabels)
	{
		nChunkType =		CHUNKTYPE_TLBL;
		nChunkSize =		nTrackCount;

		for(int i = 0; i < nTrackCount; i++)
			nChunkSize += lstrlen(szTrackLabels[i]);

		fwrite(&nChunkSize,4,1,file);
		fwrite(&nChunkType,4,1,file);

		for(int i = 0; i < nTrackCount; i++)
		{
			if(szTrackLabels[i])
				fwrite(szTrackLabels[i],lstrlen(szTrackLabels[i]) + 1,1,file);
			else
				fwrite("",1,1,file);
		}
	}

	//data
	nChunkType =			CHUNKTYPE_DATA;
	nChunkSize =			nDataBufferSize;
	fwrite(&nChunkSize,4,1,file);
	fwrite(&nChunkType,4,1,file);
	fwrite(pDataBuffer,nChunkSize,1,file);

	//END
	nChunkType =			CHUNKTYPE_NEND;
	nChunkSize =			0;
	fwrite(&nChunkSize,4,1,file);
	fwrite(&nChunkType,4,1,file);

	//w00t
	return 0;
}