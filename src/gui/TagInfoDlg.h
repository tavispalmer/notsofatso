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
//  TagInfoDlg.h
//
//

#ifndef __TAG_INFO_DLG_H_
#define __TAG_INFO_DLG_H_

#include "../core/NSF_File.h"
#include "../dfc/DFC.h"

class CTagInfoDlg : public CDDialog
{	
public:
	void		OnOK() { }
	void		OnCancel() { }
	void		OnInitDialog();

	void		LoadValues();
	void		StoreValues();

	CNSFFile*		pFile;

	CDEdit			m_Title;
	CDEdit			m_Artist;
	CDEdit			m_Copyright;
	CDEdit			m_Ripper;
};

#endif
