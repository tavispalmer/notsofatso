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
//  GeneralFileInfoDlg.h
//
//

#ifndef __GENERAL_FILE_INFO_DLG_H_
#define __GENERAL_FILE_INFO_DLG_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "../dfc/DFC.h"
#include "../NSF.h"

class CGeneralFileInfoDlg : public CDDialog
{
public:
	void		OnOK() { }
	void		OnCancel() { }
	void		OnInitDialog();
	void		Refresh();

	void		OnConvert();
	void		OnShadowWinamp();
	void		OnShadowFile();


	CNSFFile*		pFile;
	char*			szPath;
	CNSF*			pNSF;
	BYTE			bShadow;
	BYTE			nShadowTrack;

	CDEdit			m_PathName;
	CDStatic		m_Shadow;
	CDStatic		m_Info;
	CDButton		m_Convert;
};

#endif
