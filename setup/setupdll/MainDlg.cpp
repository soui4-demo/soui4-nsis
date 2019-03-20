// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
#include <sstream>



CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
{
}

CMainDlg::~CMainDlg()
{
}


BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
    //////初始化控件变量/////
    m_tabMain = FindChildByName2<STabCtrl>(L"tab_main");

    //////Welcome Page//////
    m_btnInstall = FindChildByName(L"btn_install");

    m_btnCustom = FindChildByName2<SCheckBox>(L"chk_custom");

    m_chkAgreement = FindChildByName2<SCheckBox>(L"chk_agreement");

    m_editFolder = FindChildByName2<SEdit>(L"edit_choose_url");

	m_btnCustom->EnableWindow(FALSE,TRUE);
	m_btnInstall->EnableWindow(FALSE,TRUE);

    return 0;
}


//TODO:消息映射
void CMainDlg::OnClose()
{
    delete theApp;
    delete pComMgr;
    OleUninitialize();
    ExitProcess(0);
}

void CMainDlg::OnMinimize()
{
    SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}


void CMainDlg::OnChkAgreementClick() {
    if (m_chkAgreement && m_chkAgreement->IsChecked()) {
        m_btnInstall->EnableWindow(TRUE, TRUE);
        m_btnCustom->EnableWindow(TRUE, TRUE);
    }
    else if (m_chkAgreement && !m_chkAgreement->IsChecked()) {
        m_btnInstall->EnableWindow(FALSE, TRUE);
        m_btnCustom->EnableWindow(FALSE, TRUE);
    }
}

void CMainDlg::OnChkCustomClick()
{
	FindChildByName(L"div_change_folder")->SetVisible(m_btnCustom->IsChecked(),TRUE);
}


void CMainDlg::OnLinkAgreementClick()
{
	m_tabMain->SetCurSel(1);//show agreement
}


void CMainDlg::OnBtnAgreementSure()
{
	m_tabMain->SetCurSel(0);//goto welcome
}

LRESULT CMainDlg::OnStopMsgLoop(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/) {
    SWindow* pbtn_close = FindChildByName(L"btn_close");
    SWindow* pbtn_min = FindChildByName(L"btn_min");
    if (pbtn_close)
        pbtn_close->EnableWindow(FALSE, TRUE);
    if (pbtn_min)
        pbtn_min->EnableWindow(FALSE, TRUE);

	m_tabMain->SetCurSel(2);//goto installing page
    SetTimer(TMR_AUTOCHANGE, 3000);
    PostMessage(WM_QUIT);//quit soui message loop
    return 0;
}

LRESULT CMainDlg::OnUnstallStartNext(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/) {
	m_tabMain->SetCurSel(4);
    return 0;
}

void CMainDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == TMR_AUTOCHANGE)
    {
        if (m_tabMain)
        {
            size_t tab_index = m_tabMain->GetCurSel();
            if (tab_index == 2)
            {
                STabCtrl* ptab_installing = FindChildByName2<STabCtrl>(L"tab_installing");
                if (ptab_installing)
                {
                    size_t index = ptab_installing->GetCurSel();
                    if (index >= ptab_installing->GetItemCount() - 1)
                    {
                        index = 0;
                    }
                    else
                    {
                        index++;
                    }
                    ptab_installing->SetCurSel(index);
                }
            }
        }

    }else
	{
		SetMsgHandled(FALSE);
	}
}

