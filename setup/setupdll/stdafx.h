// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息

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



// TODO:  在此处引用程序需要的其他头文件
