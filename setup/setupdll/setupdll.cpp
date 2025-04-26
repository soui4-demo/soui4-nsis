/**
* Copyright (C) 2014-2050 SongJia
* All rights reserved.
*
* @file       setupdll.cpp
* @brief	  定义 DLL 应用程序的导出函数。
* @version    v1.0
* @author     SongJia
* @date       2015-04-23
*
* Describe    将生成的setupdll.DLL放入NSIS的plugins目录下，在脚本中调用DLL名::函数名既可。
*			  （脚本调用DLL不要忘记加上参数：/NOUNLOAD）
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

/////全局变量/////
HINSTANCE g_hInstance;
HWND g_hwndParent;//NSIS主窗口句柄
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
	//HostWnd真实窗口消息处理
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

详细说明：
HWND hwndParent 保存的是主窗口的句柄，每当进入下一个页面的时候 NSIS 会在主窗口里创建一个子窗口，
这个子窗口才是我们定义的页面显示的窗口，就是那些许可协议、组件选择、目录选择页面窗口。
主窗口是贯穿整个安装过程的，
而子窗口的句柄必须在定义的 Show 函数里用FindWindow $0 "#32770" "" $HWNDPARENT ;$0 保存子窗口句柄来获取。
(当然上面这句话在本场景不用考虑啦）
int string_size 相当于main函数中的 int argc。
int  char *variables 相当于main函数中的 char* argv。
stack_t **stacktop 具体的参数是通过它来传递的。
extra_parameters 程序反向调用脚本就靠他了。
*/
NSISAPI  InitWindow(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    EXDLL_INIT();//每个导出函数都加上，绑定堆栈指针的，否则数据会取不到。

    g_hwndParent = hwndParent;
    g_pluginParms = extra;

    //NSIS注册回调。
    extra->RegisterPluginCallback(g_hInstance, PluginCallback);
    {

        //本安装程序需要相关资源路径
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
            //加载系统资源
            {
                CAutoRefPtr<IResProvider> sysSesProvider;
                sysSesProvider.Attach(souiFac.CreateResProvider(RES_PE));
                sysSesProvider->Init((WPARAM)g_hInstance, 0);
                theApp->LoadSystemNamedResource(sysSesProvider);
            }
			//加载程序资源
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
            //加载多语言翻译模块。
            CAutoRefPtr<ITranslatorMgr> trans;
            bLoaded = pComMgr->CreateTranslator((IObjRef**)&trans);
            SASSERT_FMT(bLoaded, _T("load interface [%s] failed!"), _T("translator"));
            if (trans)
            {//加载语言翻译包
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

			//改变msgbox样式 byJoe
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
//显示运行窗口
NSISAPI  ShowPage(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    EXDLL_INIT();
    pMainDlg->ShowWindow(SW_SHOWNORMAL);
    int nRet = theApp->Run(NULL);
}

//关闭窗口
NSISAPI  ClosePage(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    EXDLL_INIT();
    pMainDlg->OnClose();
}

//根据名称检索uires.idx中string的定义
// 例如你可以得到 %title% %ver% 使软件名称和版本进行统一定义（包括脚本中的定义)
// 后续将所有常量统一定义到uires.idx。方便维护
NSISAPI FindStringByName(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    //后续实现
    //SStringPool::getSingleton().Get(_T("title"));
}

//根据控件名称查找子控件
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

//绑定控件与脚本关系
NSISAPI BindControlAndNSISScript(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    EXDLL_INIT();
    wstring childName = _T("");
    PopString(childName);//名字
    int callbackID = popint();//回调函数地址
    pMainDlg->AddToControlCallbackMap(childName, callbackID);
}


//操作控件属性（注意：只能在不影响NSIS主程序运行的情况下才可以使用）
NSISAPI  SetControlProperties(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    EXDLL_INIT();

    wstring type = _T("");
    PopString(type);//控件类型

    wstring childName = _T("");
    PopString(childName);//控件名字

    wstring property = _T("");
    PopString(property);//操作的控件属性

    wstring data = _T("");
    PopString(data);//数据


    SWindow *child = NULL;
    if (_tcsicmp(type.c_str(), _T("SImageButton")) == 0) {//按钮
        child = pMainDlg->FindChildByName2<SImageButton>(childName.c_str());
        if (child == NULL) {
            return;
        }
        //特有属性
    }
    else if (_tcsicmp(type.c_str(), _T("STabCtrl")) == 0) {//tab标签
        child = pMainDlg->FindChildByName2<STabCtrl>(childName.c_str());
        if (child == NULL) {
            return;
        }
        //特有属性
        if (_tcsicmp(property.c_str(), _T("curSel")) == 0) {//选择page
            wchar_t* wcTarget = new wchar_t[data.size() + 1];
            swprintf(wcTarget, data.size() + 1, L"%lS", data.c_str());
            string tmp;
            Wchar_tToString(tmp, wcTarget);
            ((STabCtrl*)child)->SetCurSel(myatoi(tmp.c_str()));
            return;
        }

    }//可根据需要，自行添加对其他子控件属性设置。
    else {
        child = pMainDlg->FindChildByName2<SWindow>(childName.c_str());
        if (child == NULL) {
            return;
        }
    }


    //////通用属性/////

    //控件是否可用
    if (_tcsicmp(property.c_str(), _T("enable")) == 0) {
        if (_tcsicmp(data.c_str(), _T("true")) == 0)
            child->EnableWindow(TRUE, TRUE);
        else child->EnableWindow(FALSE, TRUE);
    }
    //控件是否可见
    else if (_tcsicmp(property.c_str(), _T("visible")) == 0) {
        if (_tcsicmp(data.c_str(), _T("true")) == 0)
            child->SetVisible(TRUE, TRUE);
        else child->EnableWindow(FALSE, TRUE);
    }
    //设置控件文本
    else if (_tcsicmp(property.c_str(), _T("text")) == 0) {
        SStringT  transData = data.c_str();
        //多语言翻译
        SStringW t = TR(transData, _T("mainWindow"));//翻译
        //翻译uires.idx string %x%（现在版本由此问题，临时解决)
        GETSTRING(t);
        child->SetWindowTextW(t);
    }
    //设置是否选中
    else if (_tcsicmp(property.c_str(), _T("checked")) == 0) {
        if (_tcsicmp(data.c_str(), _T("true")) == 0)
            child->SetCheck(TRUE);
        else child->SetCheck(FALSE);
    }

}

//获取控件相关属性数据（注意：只能在不影响NSIS主程序运行的情况下才可以使用）
NSISAPI  GetControlProperties(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    EXDLL_INIT();

    wstring type = _T("");
    PopString(type);//控件类型

    wstring childName = _T("");
    PopString(childName);//控件名字

    wstring property = _T("");
    PopString(property);//操作的控件属性


    SWindow *child = NULL;
    if (_tcsicmp(type.c_str(), _T("SImageButton")) == 0) {//按钮
        child = pMainDlg->FindChildByName2<SImageButton>(childName.c_str());
        if (child == NULL) {
            return;
        }
        //特有属性
    }
    else if (_tcsicmp(type.c_str(), _T("STabCtrl")) == 0) {//tab标签
        child = pMainDlg->FindChildByName2<STabCtrl>(childName.c_str());
        if (child == NULL) {
            return;
        }
        //特有属性
        if (_tcsicmp(property.c_str(), _T("curSel")) == 0) {//选择page

        }

    }//可根据需要，自行添加对其他子控件属性设置。
    else {
        child = pMainDlg->FindChildByName2<SWindow>(childName.c_str());
        if (child == NULL) {
            return;
        }
    }

    //控件是否可用
    if (_tcsicmp(property.c_str(), _T("enable")) == 0) {
        if (!child->IsDisabled())
            pushint(1);
        else 	pushint(0);
    }
    //控件是否可见
    else if (_tcsicmp(property.c_str(), _T("visible")) == 0) {
        if (child->IsVisible())
            pushint(1);
        else 	pushint(0);
    }
    //获取控件文本
    else if (_tcsicmp(property.c_str(), _T("text")) == 0) {
        SStringT str = child->GetWindowTextW();
        wstring s = str.GetBuffer(str.GetLength());
        //string tmp;
        //Wchar_tToString(tmp, str.GetBuffer(str.GetLength()));
        PushString(s);
    }
    //获取是否选中
    else if (_tcsicmp(property.c_str(), _T("checked")) == 0) {
        //MessageBox(NULL, _T(""), _T(""), MB_OK);
        if (child->IsChecked()) pushint(1);
        else pushint(0);
    }

}

//由脚本发起事件通知
NSISAPI NSISScriptSendMessage(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    EXDLL_INIT();
    //获取要发送窗口消息的句柄
    HWND hwnd = (HWND)popint();
    //获取消息ID
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

//打开文件对话框回调
int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
    if (uMsg == BFFM_INITIALIZED)
        SendMessage(hwnd, BFFM_SETSELECTION, TRUE, pData);

    return 0;
}
//打开选择目录对话框
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


//回调
BOOL CALLBACK NSISWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //必须添加，否则显示#32770
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
                MessageBox(NULL, _T("进度条异常"), _T(""), MB_OK);
                return -1;
            }
            pg->SetValue(((int)wParam) / 300);

			if (pg->GetValue() == 100) {
                STabCtrl* tab = pMainDlg->FindChildByName2<STabCtrl>(L"tab_main");
                if (tab == NULL) {
                    MessageBox(NULL, _T("Tab界面异常"), _T(""), MB_OK);
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

//进度条控制及安装过程信息获取
NSISAPI BindingProgress(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    EXDLL_INIT();

    PopString(progressName);

    // 接管page instfiles的消息
    g_windowInfoMap[hwndParent] = (WNDPROC)SetWindowLong(hwndParent, GWL_WNDPROC, (long)NSISWindowProc);
    HWND hProgressHWND = FindWindowEx(FindWindowEx(hwndParent, NULL, _T("#32770"), NULL), NULL, _T("msctls_progress32"), NULL);
    g_windowInfoMap[hProgressHWND] = (WNDPROC)SetWindowLong(hProgressHWND, GWL_WNDPROC, (long)NSISWindowProc);

    //NSIS源代码中找到 g_hwndList = FindWindowEx(FindWindowEx(g_hwndParent,NULL,"#32770",NULL),NULL,"SysListView32",NULL);
    HWND g_hwndList = FindWindowEx(FindWindowEx(hwndParent, NULL, _T("#32770"), NULL), NULL, _T("SysListView32"), NULL);
    if (g_hwndList)
        g_windowInfoMap[g_hwndList] = (WNDPROC)SetWindowLong(g_hwndList, GWL_WNDPROC, (long)NSISWindowProc);

}

//消息对话框
NSISAPI NSISMessageBox(HWND hwndParent, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
    EXDLL_INIT();
    //内容
    wstring content = _T("");
    PopString(content);
    int ret = SMsgBox(content.c_str());
    pushint(ret);
}




