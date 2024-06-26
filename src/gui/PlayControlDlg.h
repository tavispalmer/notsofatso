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
//  PlayControlDlg.h
//
//

#ifndef __PLAY_CONTROL_DLG_H_
#define __PLAY_CONTROL_DLG_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "../core/NSF_File.h"
#include "../dfc/DFC.h"

class CNSF;

class CPlayControlDlg_Slide : public CDDialog
{
public:
				CPlayControlDlg_Slide();
	void		OnOK() { }
	void		OnCancel() { }
	void		OnInitDialog();
	void		LoadValues();

	void		UpdateTrack(BYTE resetsel);
	void		UpdateSlider();

	void		OnNext();
	void		OnPrev();
	void		OnPlay();

	int			WndProc(UINT msg,WPARAM wParam,LPARAM lParam);


	CNSF*		pNSF;
	CNSFFile*	pFile;
	BYTE		bShadow;

	CDStatic	m_fileinfo;
	CDSlider	m_slider;
	CDStatic	m_tracktext;
};

#endif
