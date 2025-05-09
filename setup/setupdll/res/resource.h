﻿//stamp:1470d59c952528be
/*<------------------------------------------------------------------------------------------------->*/
/*该文件由uiresbuilder生成，请不要手动修改*/
/*<------------------------------------------------------------------------------------------------->*/
#ifndef _UIRES_H_
#define _UIRES_H_
	struct _UIRES{
		struct _UIDEF{
			const TCHAR * XML_INIT;
			}UIDEF;
		struct _LAYOUT{
			const TCHAR * XML_MAINWND;
			const TCHAR * div_header;
			const TCHAR * XML_WELCOME;
			const TCHAR * XML_AGREEMENT;
			const TCHAR * XML_INSTALLING;
			const TCHAR * XML_FINISH;
			const TCHAR * XML_UNSTALLCHOOSE;
			const TCHAR * XML_UNSTALLING;
			const TCHAR * XML_UNSTALLFINISH;
			const TCHAR * dlg_msgbox;
			}LAYOUT;
		struct _img{
			const TCHAR * png_logo;
			const TCHAR * png_logo2;
			const TCHAR * png_bg;
			const TCHAR * png_welcome;
			const TCHAR * png_bg_dir;
			const TCHAR * png_scroll;
			const TCHAR * png_check_agree;
			const TCHAR * png_check_custom;
			const TCHAR * png_btn_pick_folder;
			const TCHAR * png_btn_close;
			const TCHAR * png_btn_minimize;
			const TCHAR * png_btn_normal;
			const TCHAR * png_btn_hollow;
			const TCHAR * prog_bar;
			const TCHAR * prog_bkgnd;
			const TCHAR * png_uninstall;
			const TCHAR * png_uninstalling;
			const TCHAR * png_uninstalled;
			const TCHAR * setup1;
			const TCHAR * setup2;
			const TCHAR * setup3;
			const TCHAR * setup4;
			const TCHAR * setup_tab;
			}img;
		struct _ICON{
			const TCHAR * LOGO;
			}ICON;
		struct _translator{
			const TCHAR * lang_cn;
			}translator;
		struct _rtf{
			const TCHAR * rtf_agreement;
			}rtf;
	};
#endif//_UIRES_H_
#ifdef INIT_R_DATA
struct _UIRES UIRES={
		{
			_T("UIDEF:XML_INIT"),
		},
		{
			_T("LAYOUT:XML_MAINWND"),
			_T("LAYOUT:div_header"),
			_T("LAYOUT:XML_WELCOME"),
			_T("LAYOUT:XML_AGREEMENT"),
			_T("LAYOUT:XML_INSTALLING"),
			_T("LAYOUT:XML_FINISH"),
			_T("LAYOUT:XML_UNSTALLCHOOSE"),
			_T("LAYOUT:XML_UNSTALLING"),
			_T("LAYOUT:XML_UNSTALLFINISH"),
			_T("LAYOUT:dlg_msgbox"),
		},
		{
			_T("img:png_logo"),
			_T("img:png_logo2"),
			_T("img:png_bg"),
			_T("img:png_welcome"),
			_T("img:png_bg_dir"),
			_T("img:png_scroll"),
			_T("img:png_check_agree"),
			_T("img:png_check_custom"),
			_T("img:png_btn_pick_folder"),
			_T("img:png_btn_close"),
			_T("img:png_btn_minimize"),
			_T("img:png_btn_normal"),
			_T("img:png_btn_hollow"),
			_T("img:prog_bar"),
			_T("img:prog_bkgnd"),
			_T("img:png_uninstall"),
			_T("img:png_uninstalling"),
			_T("img:png_uninstalled"),
			_T("img:setup1"),
			_T("img:setup2"),
			_T("img:setup3"),
			_T("img:setup4"),
			_T("img:setup_tab"),
		},
		{
			_T("ICON:LOGO"),
		},
		{
			_T("translator:lang_cn"),
		},
		{
			_T("rtf:rtf_agreement"),
		},
	};
#else
extern struct _UIRES UIRES;
#endif//INIT_R_DATA

#ifndef _R_H_
#define _R_H_
struct _R{
	struct _name{
		 const wchar_t * agreement_edit;
		 const wchar_t * btn_agreement_sure;
		 const wchar_t * btn_change_folder;
		 const wchar_t * btn_close;
		 const wchar_t * btn_finish;
		 const wchar_t * btn_install;
		 const wchar_t * btn_min;
		 const wchar_t * btn_open;
		 const wchar_t * btn_unstallchoose_previous;
		 const wchar_t * btn_unstallchoose_unnstall;
		 const wchar_t * btn_unstallfinish;
		 const wchar_t * chk_agreement;
		 const wchar_t * chk_custom;
		 const wchar_t * chk_unstallchoose_delete;
		 const wchar_t * div_change_folder;
		 const wchar_t * edit_folder;
		 const wchar_t * edit_unstallchoose_url;
		 const wchar_t * img_setup1;
		 const wchar_t * img_setup2;
		 const wchar_t * img_setup3;
		 const wchar_t * img_setup4;
		 const wchar_t * link_agreement;
		 const wchar_t * pg_installing;
		 const wchar_t * pg_unstalling;
		 const wchar_t * tab_installing;
		 const wchar_t * tab_main;
		 const wchar_t * txt_msg;
	}name;
	struct _id{
		int agreement_edit;
		int btn_agreement_sure;
		int btn_change_folder;
		int btn_close;
		int btn_finish;
		int btn_install;
		int btn_min;
		int btn_open;
		int btn_unstallchoose_previous;
		int btn_unstallchoose_unnstall;
		int btn_unstallfinish;
		int chk_agreement;
		int chk_custom;
		int chk_unstallchoose_delete;
		int div_change_folder;
		int edit_folder;
		int edit_unstallchoose_url;
		int img_setup1;
		int img_setup2;
		int img_setup3;
		int img_setup4;
		int link_agreement;
		int pg_installing;
		int pg_unstalling;
		int tab_installing;
		int tab_main;
		int txt_msg;
	}id;
	struct _color{
		int gray_title;
		int theme;
		int white;
	}color;
	struct _string{
		int agreement;
		int agreement_check;
		int cancel;
		int change;
		int custom_install;
		int finish;
		int install;
		int installing;
		int keep_data;
		int ok;
		int open;
		int quit_install;
		int quit_uninstall;
		int slogon;
		int sure;
		int title;
		int uninstall;
		int uninstall_finish;
		int uninstall_title;
		int uninstalling;
		int ver;
	}string;

};
#endif//_R_H_
#ifdef INIT_R_DATA
struct _R R={
	{
		L"agreement_edit",
		L"btn_agreement_sure",
		L"btn_change_folder",
		L"btn_close",
		L"btn_finish",
		L"btn_install",
		L"btn_min",
		L"btn_open",
		L"btn_unstallchoose_previous",
		L"btn_unstallchoose_unnstall",
		L"btn_unstallfinish",
		L"chk_agreement",
		L"chk_custom",
		L"chk_unstallchoose_delete",
		L"div_change_folder",
		L"edit_folder",
		L"edit_unstallchoose_url",
		L"img_setup1",
		L"img_setup2",
		L"img_setup3",
		L"img_setup4",
		L"link_agreement",
		L"pg_installing",
		L"pg_unstalling",
		L"tab_installing",
		L"tab_main",
		L"txt_msg"
	}
	,
	{
		65546,
		65547,
		65542,
		65537,
		65555,
		65539,
		65536,
		65554,
		65559,
		65558,
		65561,
		65543,
		65545,
		65557,
		65540,
		65541,
		65556,
		65549,
		65550,
		65551,
		65552,
		65544,
		65553,
		65560,
		65548,
		65538,
		65562
	}
	,
	{
		0,
		1,
		2
	}
	,
	{
		0,
		1,
		2,
		3,
		4,
		5,
		6,
		7,
		8,
		9,
		10,
		11,
		12,
		13,
		14,
		15,
		16,
		17,
		18,
		19,
		20
	}
	
};
#else
extern struct _R R;
#endif//INIT_R_DATA
