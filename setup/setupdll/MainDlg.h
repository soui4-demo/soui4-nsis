/**
* Copyright (C) 2014-2050 SongJia
* All rights reserved.
*
* @file       MainDlg.h
* @brief	  SOUI主界面程序
* @version    v1.0
* @author     SongJia
* @date       2015-04-23
*
* Describe
*/

//
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <map>
using namespace std;

#include "pluginapi.h" // nsis plugin
#include "event.h"
#include <helper/smenu.h>

#define TMR_AUTOCHANGE     10000001
extern extra_parameters *g_pluginParms;
extern HWND g_hwndParent;
extern SApplication *theApp;
extern SComMgr2 *pComMgr;

class CMainDlg : public SHostWnd
{
public:
    CMainDlg();
    ~CMainDlg();

    void OnClose();
    void OnMinimize();
	void OnChkAgreementClick();
	void OnChkCustomClick();
	void OnLinkAgreementClick();
	void OnBtnAgreementSure();

    void OnBtnMsgBox();

	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

	LRESULT OnStopMsgLoop(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/);

    //卸载第一页
    LRESULT  OnUnstallStartNext(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/);

    void OnTimer(UINT_PTR nIDEvent);

protected:
    //soui消息
    EVENT_MAP_BEGIN()
        EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
		EVENT_NAME_COMMAND(L"chk_agreement",OnChkAgreementClick)
		EVENT_NAME_COMMAND(L"chk_custom",OnChkCustomClick)
		EVENT_NAME_COMMAND(L"link_agreement",OnLinkAgreementClick)
		EVENT_NAME_COMMAND(L"btn_agreement_sure",OnBtnAgreementSure)
    EVENT_MAP_END()

    //HostWnd真实窗口消息处理
    BEGIN_MSG_MAP_EX(CMainDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_TIMER(OnTimer)
        //自定义消息
        MESSAGE_HANDLER(WM_SJMSGLOOPSTOP, OnStopMsgLoop)
        //卸载
        MESSAGE_HANDLER(WM_SJUNSTALLSTARTNEXT, OnUnstallStartNext)

		REFLECT_NOTIFICATIONS_EX()
		CHAIN_MSG_MAP(SHostWnd)
    END_MSG_MAP()

private:
    //安装主导航
    STabCtrl *m_tabMain;

    ////////Welcome Page/////////
    //立刻安装按钮
    SWindow *m_btnInstall;
    //自定义安装按钮
    SCheckBox *m_btnCustom;
    //同意协议按钮
    SCheckBox *m_chkAgreement;
    //安装路径
    SEdit *m_editFolder;

    //子控件与外部脚本绑定，方便程序反向调用触发脚本函数
    // wstring : 控件名称	int: 脚本函数地址
    std::map<wstring, int> m_controlCallbackMap;

public:
    //执行NSIS脚本
    BOOL DoExecuteCodeSegment(EventArgs* evt) {

        std::map<wstring, int >::iterator iter = this->m_controlCallbackMap.find(evt->NameFrom());
        if (iter != m_controlCallbackMap.end()) {
            g_pluginParms->ExecuteCodeSegment(iter->second - 1, g_hwndParent);
        }
        return TRUE;
    }

    //外部调用-添加子控件与外部脚本绑定
    void AddToControlCallbackMap(wstring ctlName, int callback)
    {
        m_controlCallbackMap[ctlName] = callback;
        SWindow *child = FindChildByName(ctlName.c_str());
        child->GetEventSet()->subscribeEvent(EVT_CMD, Subscriber(&CMainDlg::DoExecuteCodeSegment, this));
    }



};
