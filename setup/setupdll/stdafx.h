// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ

#define  _CRT_SECURE_NO_WARNINGS

#define	 DLL_SOUI

//for link release lib
#define DISABLE_SWNDSPY 1
#define ENABLE_SOBJ_XML 0
//#define 
#include <souistd.h>
#include <core/SHostDialog.h>
#include <control/SMessageBox.h>
#include <control/souictrls.h>
#include <res.mgr/sobjdefattr.h>

#include "res/resource.h"

#include <commask.h>
#define SCOM_MASK (scom_mask_render_gdi|scom_mask_imgdecoder_gdip|scom_mask_translator)
#include <commgr2.h>

using namespace SOUI;



// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
