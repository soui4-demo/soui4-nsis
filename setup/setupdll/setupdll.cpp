/**
* Copyright (C) 2014-2050 SongJia
* All rights reserved.
*
* @file       setupdll.cpp
* @brief	  ���� DLL Ӧ�ó���ĵ���������
* @version    v1.0
* @author     SongJia
* @date       2015-04-23
*
* Describe    �����ɵ�setupdll.DLL����NSIS��pluginsĿ¼�£��ڽű��е���DLL��::�������ȿɡ�
*			  ���ű�����DLL��Ҫ���Ǽ��ϲ�����/NOUNLOAD��
*/
#include "stdafx.h"
#include "MainDlg.h"
#include "Utils.h"
#include <sstream>
#include <shlobj.h>
#include <SouiFactory.h>

#define INIT_R_DATA
#include "res/resource.h"

#define NSISAPI extern "C" __declspec(dllexport) void __cdecl

//replace path of uires just for debug.
#define RES_PATH _T("d:\\work\\soui4_nsis\\setup\\setupdll\\uires")

/////ȫ�ֱ���/////
HINSTANCE g_hInstance;
HWND g_hwndParent;//NSIS�����ھ��
extra_parameters* g_pluginParms;
std::map<HWND, WNDPROC> g_windowInfoMap;

SApplication *theApp = NULL;
SComMgr2 *pComMgr = NULL;
CMainDlg *pMainDlg = NULL;

wstring progressName = _T("");


static UINT_PTR PluginCallback(enum NSPIM msg)
{
    return 0;
}


class SMsgDlg: public SHostDialog
{
public:
	SMsgDlg(LPCTSTR pszMsg):m_strMsg(pszMsg),SHostDialog(UIRES.LAYOUT.dlg_msgbox){}
protected:
	BOOL OnInitDialog(HWND wnd, LPARAM lInitParam)
	{
		FindChildByName(L"txt_msg")->SetWindowText(m_strMsg);
		return FALSE;
	}
	//HostWnd��ʵ������Ϣ����
	BEGIN_MSG_MAP_EX(SMsgDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
		CHAIN_MSG_MAP(SHostDialog)
		END_MSG_MAP()

		SStringT m_strMsg;
};

UINT SMsgBox(LPCTSTR pszMsg)
{
	SMsgDlg msgDlg(pszMsg);
	return msgDlg.DoModal();
}

/*

��ϸ˵����
HWND hwndParent ������������ڵľ����ÿ��������һ��ҳ���ʱ�� NSIS �����������ﴴ��һ���Ӵ��ڣ�
����Ӵ��ڲ������Ƕ����ҳ����ʾ�Ĵ��ڣ�������Щ���Э�顢���ѡ��Ŀ¼ѡ��ҳ�洰�ڡ�
�������ǹᴩ������װ���̵ģ�
���Ӵ��ڵľ�������ڶ���� Show ��������FindWindow $0 "#32770" "" $HWNDPARENT ;$0 �����Ӵ��ھ������ȡ��
(��Ȼ������仰�ڱ��������ÿ�������
int string_size �൱��main�����е� int argc��
int  char *variables �൱��main�����е� char* argv��
stack_t **stacktop ����Ĳ�����ͨ���������ݵġ�
extra_parameters ��������ýű��Ϳ����ˡ�
*/
NSISAPI  InitWindow(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    EXDLL_INIT();//ÿ���������������ϣ��󶨶�ջָ��ģ��������ݻ�ȡ������

    g_hwndParent = hwndParent;
    g_pluginParms = extra;

    //NSISע��ص���
    extra->RegisterPluginCallback(g_hInstance, PluginCallback);
    {

        //����װ������Ҫ�����Դ·��
        std::wstring tmpPath = _T("");
        PopString(tmpPath);

        HRESULT hRes = OleInitialize(NULL);
        SASSERT(SUCCEEDED(hRes));

        int nRet = 0;

        pComMgr = new SComMgr2(_T("imgdecoder-gdip"));

        wstring t = tmpPath + _T("\\");
        SetCurrentDirectory(t.c_str());
        SetDllDirectory(t.c_str());
        {
            BOOL bLoaded = FALSE;
            CAutoRefPtr<SOUI::IImgDecoderFactory> pImgDecoderFactory;
            CAutoRefPtr<SOUI::IRenderFactory> pRenderFactory;
            bLoaded = pComMgr->CreateRender_GDI((IObjRef**)&pRenderFactory);
            SASSERT_FMT(bLoaded, _T("load interface [render] failed!"));
            bLoaded = pComMgr->CreateImgDecoder((IObjRef**)&pImgDecoderFactory);
            SASSERT_FMT(bLoaded, _T("load interface [%s] failed!"), _T("imgdecoder"));

            pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);
            theApp = new SApplication(pRenderFactory, g_hInstance);

			SouiFactory souiFac;
            //����ϵͳ��Դ
            {
                CAutoRefPtr<IResProvider> sysSesProvider;
                sysSesProvider.Attach(souiFac.CreateResProvider(RES_PE));
                sysSesProvider->Init((WPARAM)g_hInstance, 0);
                theApp->LoadSystemNamedResource(sysSesProvider);
            }
			//���س�����Դ
#ifdef _DEBUG
			{
				CAutoRefPtr<IResProvider>   pResProvider;
				pResProvider.Attach(souiFac.CreateResProvider(RES_FILE));
				pResProvider->Init((LPARAM)RES_PATH, 0);
				theApp->AddResProvider(pResProvider);
			}
#else
			{
				CAutoRefPtr<IResProvider>   pResProvider;
				pResProvider.Attach(souiFac.CreateResProvider(RES_PE));
				pResProvider->Init((WPARAM)g_hInstance, 0);
				theApp->AddResProvider(pResProvider);
			}
#endif
            //���ض����Է���ģ�顣
            CAutoRefPtr<ITranslatorMgr> trans;
            bLoaded = pComMgr->CreateTranslator((IObjRef**)&trans);
            SASSERT_FMT(bLoaded, _T("load interface [%s] failed!"), _T("translator"));
            if (trans)
            {//�������Է����
                theApp->SetTranslator(trans);
                SXmlDoc xmlLang;
				if (theApp->LoadXmlDocment(xmlLang, _T("lang_cn:translator")))
                {
                    CAutoRefPtr<ITranslator> langCN;
                    trans->CreateTranslator(&langCN);
                    langCN->Load(&xmlLang.root().child(L"language"), 1);//1=LD_XML
                    trans->InstallTranslator(langCN);
                }
            }

			//�ı�msgbox��ʽ byJoe
			theApp->SetMessageBoxTemplateResId(_T("XML_MSGBOX:LAYOUT"));

            // BLOCK: Run application
            {
                CMainDlg* dlgMain = new CMainDlg;
                dlgMain->Create(GetActiveWindow());
                dlgMain->SendMessage(WM_INITDIALOG);
                dlgMain->CenterWindow();
                dlgMain->ShowWindow(SW_HIDE);
                pMainDlg = dlgMain;

                pushint(int(dlgMain->m_hWnd));
            }

        }
    }
}
//��ʾ���д���
NSISAPI  ShowPage(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    EXDLL_INIT();
    pMainDlg->ShowWindow(SW_SHOWNORMAL);
    int nRet = theApp->Run(NULL);
}

//�رմ���
NSISAPI  ClosePage(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    EXDLL_INIT();
    pMainDlg->OnClose();
}

//�������Ƽ���uires.idx��string�Ķ���
// ��������Եõ� %title% %ver% ʹ������ƺͰ汾����ͳһ���壨�����ű��еĶ���)
// ���������г���ͳһ���嵽uires.idx������ά��
NSISAPI FindStringByName(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    //����ʵ��
    //SStringPool::getSingleton().Get(_T("title"));
}

//���ݿؼ����Ʋ����ӿؼ�
NSISAPI FindChildByName(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    EXDLL_INIT();
    wstring childName = _T("");
    PopString(childName);
    SWindow *child = pMainDlg->FindChildByName(childName.c_str());
    if (child == NULL) {
        pushint(-1);
    }
    else
        pushint(0);
}

//�󶨿ؼ���ű���ϵ
NSISAPI BindControlAndNSISScript(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    EXDLL_INIT();
    wstring childName = _T("");
    PopString(childName);//����
    int callbackID = popint();//�ص�������ַ
    pMainDlg->AddToControlCallbackMap(childName, callbackID);
}


//�����ؼ����ԣ�ע�⣺ֻ���ڲ�Ӱ��NSIS���������е�����²ſ���ʹ�ã�
NSISAPI  SetControlProperties(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    EXDLL_INIT();

    wstring type = _T("");
    PopString(type);//�ؼ�����

    wstring childName = _T("");
    PopString(childName);//�ؼ�����

    wstring property = _T("");
    PopString(property);//�����Ŀؼ�����

    wstring data = _T("");
    PopString(data);//����


    SWindow *child = NULL;
    if (_tcsicmp(type.c_str(), _T("SImageButton")) == 0) {//��ť
        child = pMainDlg->FindChildByName2<SImageButton>(childName.c_str());
        if (child == NULL) {
            return;
        }
        //��������
    }
    else if (_tcsicmp(type.c_str(), _T("STabCtrl")) == 0) {//tab��ǩ
        child = pMainDlg->FindChildByName2<STabCtrl>(childName.c_str());
        if (child == NULL) {
            return;
        }
        //��������
        if (_tcsicmp(property.c_str(), _T("curSel")) == 0) {//ѡ��page
            wchar_t* wcTarget = new wchar_t[data.size() + 1];
            swprintf(wcTarget, data.size() + 1, L"%lS", data.c_str());
            string tmp;
            Wchar_tToString(tmp, wcTarget);
            ((STabCtrl*)child)->SetCurSel(myatoi(tmp.c_str()));
            return;
        }

    }//�ɸ�����Ҫ��������Ӷ������ӿؼ��������á�
    else {
        child = pMainDlg->FindChildByName2<SWindow>(childName.c_str());
        if (child == NULL) {
            return;
        }
    }


    //////ͨ������/////

    //�ؼ��Ƿ����
    if (_tcsicmp(property.c_str(), _T("enable")) == 0) {
        if (_tcsicmp(data.c_str(), _T("true")) == 0)
            child->EnableWindow(TRUE, TRUE);
        else child->EnableWindow(FALSE, TRUE);
    }
    //�ؼ��Ƿ�ɼ�
    else if (_tcsicmp(property.c_str(), _T("visible")) == 0) {
        if (_tcsicmp(data.c_str(), _T("true")) == 0)
            child->SetVisible(TRUE, TRUE);
        else child->EnableWindow(FALSE, TRUE);
    }
    //���ÿؼ��ı�
    else if (_tcsicmp(property.c_str(), _T("text")) == 0) {
        SStringT  transData = data.c_str();
        //�����Է���
        SStringW t = TR(transData, _T("mainWindow"));//����
        //����uires.idx string %x%�����ڰ汾�ɴ����⣬��ʱ���)
        GETSTRING(t);
        child->SetWindowTextW(t);
    }
    //�����Ƿ�ѡ��
    else if (_tcsicmp(property.c_str(), _T("checked")) == 0) {
        if (_tcsicmp(data.c_str(), _T("true")) == 0)
            child->SetCheck(TRUE);
        else child->SetCheck(FALSE);
    }

}

//��ȡ�ؼ�����������ݣ�ע�⣺ֻ���ڲ�Ӱ��NSIS���������е�����²ſ���ʹ�ã�
NSISAPI  GetControlProperties(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    EXDLL_INIT();

    wstring type = _T("");
    PopString(type);//�ؼ�����

    wstring childName = _T("");
    PopString(childName);//�ؼ�����

    wstring property = _T("");
    PopString(property);//�����Ŀؼ�����


    SWindow *child = NULL;
    if (_tcsicmp(type.c_str(), _T("SImageButton")) == 0) {//��ť
        child = pMainDlg->FindChildByName2<SImageButton>(childName.c_str());
        if (child == NULL) {
            return;
        }
        //��������
    }
    else if (_tcsicmp(type.c_str(), _T("STabCtrl")) == 0) {//tab��ǩ
        child = pMainDlg->FindChildByName2<STabCtrl>(childName.c_str());
        if (child == NULL) {
            return;
        }
        //��������
        if (_tcsicmp(property.c_str(), _T("curSel")) == 0) {//ѡ��page

        }

    }//�ɸ�����Ҫ��������Ӷ������ӿؼ��������á�
    else {
        child = pMainDlg->FindChildByName2<SWindow>(childName.c_str());
        if (child == NULL) {
            return;
        }
    }

    //�ؼ��Ƿ����
    if (_tcsicmp(property.c_str(), _T("enable")) == 0) {
        if (!child->IsDisabled())
            pushint(1);
        else 	pushint(0);
    }
    //�ؼ��Ƿ�ɼ�
    else if (_tcsicmp(property.c_str(), _T("visible")) == 0) {
        if (child->IsVisible())
            pushint(1);
        else 	pushint(0);
    }
    //��ȡ�ؼ��ı�
    else if (_tcsicmp(property.c_str(), _T("text")) == 0) {
        SStringT str = child->GetWindowTextW();
        wstring s = str.GetBuffer(str.GetLength());
        //string tmp;
        //Wchar_tToString(tmp, str.GetBuffer(str.GetLength()));
        PushString(s);
    }
    //��ȡ�Ƿ�ѡ��
    else if (_tcsicmp(property.c_str(), _T("checked")) == 0) {
        //MessageBox(NULL, _T(""), _T(""), MB_OK);
        if (child->IsChecked()) pushint(1);
        else pushint(0);
    }

}

//�ɽű������¼�֪ͨ
NSISAPI NSISScriptSendMessage(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    EXDLL_INIT();
    //��ȡҪ���ʹ�����Ϣ�ľ��
    HWND hwnd = (HWND)popint();
    //��ȡ��ϢID
    wstring MsgID = _T("");
    PopString(MsgID);
    //
    wstring wParam = _T("");
    PopString(wParam);
    //
    wstring lParam = _T("");
    PopString(lParam);

    if (_tcsicmp(MsgID.c_str(), _T("WM_SJONECHECKSTATUS")) == 0) {
        ::SendMessage(hwnd, WM_SJONECHECKSTATUS, (WPARAM)wParam.c_str(), (LPARAM)lParam.c_str());
    }
    else if (_tcsicmp(MsgID.c_str(), _T("WM_SJONELINKAGREEMENT")) == 0) {
        ::SendMessage(hwnd, WM_SJONELINKAGREEMENT, (WPARAM)wParam.c_str(), (LPARAM)lParam.c_str());
    }
    else if (_tcsicmp(MsgID.c_str(), _T("WM_SJONECUSTOMINSTALL")) == 0) {
        ::SendMessage(hwnd, WM_SJONECUSTOMINSTALL, (WPARAM)wParam.c_str(), (LPARAM)lParam.c_str());
    }
    else if (_tcsicmp(MsgID.c_str(), _T("WM_SJONEINSTALL")) == 0) {
        ::SendMessage(hwnd, WM_SJONEINSTALL, (WPARAM)wParam.c_str(), (LPARAM)lParam.c_str());
    }
    else if (_tcsicmp(MsgID.c_str(), _T("WM_SJTWOSURE")) == 0) {
        ::SendMessage(hwnd, WM_SJTWOSURE, (WPARAM)wParam.c_str(), (LPARAM)lParam.c_str());
    }
    else if (_tcsicmp(MsgID.c_str(), _T("WM_SJTHREEPREVIOUS")) == 0) {
        ::SendMessage(hwnd, WM_SJTHREEPREVIOUS, (WPARAM)wParam.c_str(), (LPARAM)lParam.c_str());
    }
    else if (_tcsicmp(MsgID.c_str(), _T("WM_SJTHREEUPDATEFREESPACE")) == 0) {
        ::SendMessage(hwnd, WM_SJTHREEUPDATEFREESPACE, (WPARAM)wParam.c_str(), (LPARAM)lParam.c_str());
    }
    else if (_tcsicmp(MsgID.c_str(), _T("WM_SJTHREEUPDATEREQUIREDSPACE")) == 0) {
        ::SendMessage(hwnd, WM_SJTHREEUPDATEREQUIREDSPACE, (WPARAM)wParam.c_str(), (LPARAM)lParam.c_str());
    }
    else if (_tcsicmp(MsgID.c_str(), _T("WM_SJMSGLOOPSTOP")) == 0) {
        ::SendMessage(hwnd, WM_SJMSGLOOPSTOP, (WPARAM)wParam.c_str(), (LPARAM)lParam.c_str());
    }
    else if (_tcsicmp(MsgID.c_str(), _T("WM_SJUNSTALLSTARTNEXT")) == 0) {
        ::SendMessage(hwnd, WM_SJUNSTALLSTARTNEXT, (WPARAM)wParam.c_str(), (LPARAM)lParam.c_str());
    }
    else if (_tcsicmp(MsgID.c_str(), _T("WM_SJTABSETSEL")) == 0) {
        ::SendMessage(hwnd, WM_SJTABSETSEL, (WPARAM)wParam.c_str(), (LPARAM)lParam.c_str());
    }
}

//���ļ��Ի���ص�
int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
    if (uMsg == BFFM_INITIALIZED)
        SendMessage(hwnd, BFFM_SETSELECTION, TRUE, pData);

    return 0;
}
//��ѡ��Ŀ¼�Ի���
NSISAPI NSISOpenFolderDialog(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    EXDLL_INIT();

    BROWSEINFO bi;
    TCHAR result[MAX_PATH];
    TCHAR title[MAX_PATH];
    LPITEMIDLIST resultPIDL;
    ZeroMemory(result, MAX_PATH);
    ZeroMemory(title, MAX_PATH);

    wstring p1;
    PopString(p1);
    lstrcpyW(title, p1.c_str());

    bi.hwndOwner = pMainDlg->m_hWnd;
    bi.pidlRoot = NULL;
    bi.pszDisplayName = result;
    bi.lpszTitle = title;
#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE 0x0040
#endif
    bi.ulFlags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_NONEWFOLDERBUTTON;
    bi.lpfn = BrowseCallbackProc;
    bi.lParam = NULL;
    bi.iImage = 0;

    resultPIDL = SHBrowseForFolder(&bi);
    if (!resultPIDL)
    {
        pushint(-1);
        return;
    }

    if (SHGetPathFromIDList(resultPIDL, result))
    {
        if (result[_tcslen(result) - 1] == _T('\\'))
            result[_tcslen(result) - 1] = NULL;
        wstring r1 = result;
        PushString(r1);
    }
    else
        pushint(-1);

    CoTaskMemFree(resultPIDL);
}


//�ص�
BOOL CALLBACK NSISWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //������ӣ�������ʾ#32770
    ShowWindow(hwnd, SW_HIDE);

    BOOL res = 0;
    std::map<HWND, WNDPROC>::iterator iter = g_windowInfoMap.find(hwnd);
    if (iter != g_windowInfoMap.end())
    {
        if (message == WM_PAINT)
        {
            ShowWindow(hwnd, SW_HIDE);
        }
        else if (message == LVM_INSERTITEM)
        {
            const LV_ITEM * item = (const LV_ITEM *)lParam;
            MessageBox(NULL, item->pszText, _T(""), MB_OK);
        }
        else if (message == PBM_SETPOS)
        {
            SProgress* pg = pMainDlg->FindChildByName2<SProgress>(progressName.c_str());
            if (pg == NULL) {
                MessageBox(NULL, _T("�������쳣"), _T(""), MB_OK);
                return -1;
            }
            pg->SetValue(((int)wParam) / 300);

			if (pg->GetValue() == 100) {
                STabCtrl* tab = pMainDlg->FindChildByName2<STabCtrl>(L"tab_main");
                if (tab == NULL) {
                    MessageBox(NULL, _T("Tab�����쳣"), _T(""), MB_OK);
                    return -1;
                }
                if (_tcsicmp(progressName.c_str(), _T("pg_unstalling")) == 0)
                    tab->SetCurSel(6);
                else 
					tab->SetCurSel(3);
            }
        }
        else
        {
            res = CallWindowProc(iter->second, hwnd, message, wParam, lParam);
        }
    }
    return res;
}

//���������Ƽ���װ������Ϣ��ȡ
NSISAPI BindingProgress(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    EXDLL_INIT();

    PopString(progressName);

    // �ӹ�page instfiles����Ϣ
    g_windowInfoMap[hwndParent] = (WNDPROC)SetWindowLong(hwndParent, GWL_WNDPROC, (long)NSISWindowProc);
    HWND hProgressHWND = FindWindowEx(FindWindowEx(hwndParent, NULL, _T("#32770"), NULL), NULL, _T("msctls_progress32"), NULL);
    g_windowInfoMap[hProgressHWND] = (WNDPROC)SetWindowLong(hProgressHWND, GWL_WNDPROC, (long)NSISWindowProc);

    //NSISԴ�������ҵ� g_hwndList = FindWindowEx(FindWindowEx(g_hwndParent,NULL,"#32770",NULL),NULL,"SysListView32",NULL);
    HWND g_hwndList = FindWindowEx(FindWindowEx(hwndParent, NULL, _T("#32770"), NULL), NULL, _T("SysListView32"), NULL);
    if (g_hwndList)
        g_windowInfoMap[g_hwndList] = (WNDPROC)SetWindowLong(g_hwndList, GWL_WNDPROC, (long)NSISWindowProc);

}

//��Ϣ�Ի���
NSISAPI NSISMessageBox(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    EXDLL_INIT();
    //����
    wstring content = _T("");
    PopString(content);
    int ret = SMsgBox(content.c_str());
    pushint(ret);
}




