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
//  MainControlDlg.h
//
//

#ifndef __MAIN_CONTROL_DLG_H_
#define __MAIN_CONTROL_DLG_H_

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <windows.h>

#include "../core/NSF_File.h"
#include "../dfc/DFC.h"
#include "AboutDlg.h"
#include "ChannelsDlg.h"
#include "ConfigDlg.h"
#include "FME07Dlg.h"
#include "MMC5Dlg.h"
#include "N106Dlg.h"
#include "PlayControlDlg.h"
#include "VRC6Dlg.h"
#include "VRC7Dlg.h"

class CNSF;

class CMainControlDlg : public CDDialog
{
public:
				CMainControlDlg();
	void		OnOK() { }
	void		OnInitDialog();
	void		Build(HWND parent);
	void		UnBuild();

	void		OpenAsPlay();
	void		OpenAsConfig();
	void		OpenAsAbout();

	void		ClosePlay();

	void		OnSelChangeTab();

	void		PutWindow(int x, int y);

	int			WndProc(UINT msg,WPARAM wParam,LPARAM lParam);


	CNSFFile*	pFile;
	BYTE		bShadow;
	CNSF*		pNSF;
	HINSTANCE	hInst;

	CDTab		m_tab;
	CDStatic	m_clientbox;

	CPlayControlDlg_Slide	m_playcontrol_slide;
	CConfigDlg				m_config;
	CConfigDlg2				m_config2;
	CConfigDlg3				m_config3;
	CChannelsDlg			m_channels;
	CVRC6Dlg				m_vrc6;
	CMMC5Dlg				m_mmc5;
	CN106Dlg				m_n106;
	CVRC7Dlg				m_vrc7;
	CFME07Dlg				m_fme07;
	CAboutDlg				m_about;

	int			x_pos;
	int			y_pos;

};

#endif
