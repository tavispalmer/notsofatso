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
//  DFC_Array.h
//

#ifndef __DFC_ARRAY_H_
#define __DFC_ARRAY_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class CDByteArray
{
public:
	CDByteArray();
	~CDByteArray();

	int				Add(BYTE add);
	int				GetCount() { return nCount; }
	const BYTE*		GetData() { return pData; }
	void			RemoveAll() { nCount = 0; }
	void			RemoveAt(int index);

	BYTE& operator [] (int index) {return pData[index]; }

protected:
	void			IncreaseBufferSize(int required);

	BYTE*			pData;
	int				nBufferSize;
	int				nCount;
	int				nGrowBy;
};

#endif
