; 该脚本使用 HM VNISEdit 脚本编辑器向导产生

; ############################ 初始化定义变量 ####################################
;主窗口句柄
Var Dialog

SetCompressor lzma

RequestExecutionLevel admin

!AddPluginDir ".\plugins"
;############################# 引入nsh脚本 ######################################

!include "MUI.nsh"
!include "WinCore.nsh"
!include "nsWindows.nsh"
!include "nsDialogs.nsh"
!include "FileFunc.nsh"
!include  "LogicLib.nsh"
!include  "WinMessages.nsh"
!include 	"MUI2.nsh"
!include 	"WordFunc.nsh"
!include 	"Library.nsh"


; ########################### 安装程序初始定义常量 ###############################
!define PRODUCT_NAME "soui4_setup"
!define PRODUCT_VERSION "1.0"
!define PRODUCT_PUBLISHER "soui4_setup"
!define PRODUCT_WEB_SITE "http://soui4js.com"
!define PRODUCT_MAIN_EXE  "RegWorkshopX64.exe"

; 图标
!define MUI_ICON	".\logo.ico"
;卸载图标
!define MUI_UNICON	".\logo.ico"
;英文产品名称
!define PRODUCT_NAME_EN  "soui4js"
;提示语
!define ERROR_MESSAGE           "安装包不正确，请重新下载"
;预计软件所需空间大小(M)，注册表中为真实大小，仅用于判断空间是否够用。
!define REQUIRED_SPACE  "15"
;定义互斥名称
!define MyMutex_Install     "MyMutex_Install_soui4js"
!define MyMutex_UnInstall   "MyMutex_UnInstall_soui4js"

!define Unstall_KeyPath "Software\Microsoft\Windows\CurrentVersion\Uninstall\"

;Languages 如果不使用MUI，自定义setup图标就失效了，简简单单用上一条就OK了。
!insertmacro MUI_LANGUAGE "SimpChinese"


;############################ 安装文件定义 ############################
Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Setup.exe"
InstallDir "$PROGRAMFILES\${PRODUCT_NAME_EN}"
InstallDirRegKey HKLM "${PRODUCT_UNINST_KEY}" "UninstallString"
;
ShowInstDetails show
ShowUnInstDetails show
BrandingText " "

;############################ 安装界面 ################################
;自定义页面
Page custom MyPage
;安装进度界面
Page instfiles "" MyInstallNow
;自定义卸载页面
UninstPage   custom     un.MyUninstallPage
;卸载进度界面
UninstPage   instfiles	""	un.MyUninstallNow

;############################ 关键Section代码段 #######################
/*程序文件、创建快捷方式*/
Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File /r "file\*.*"

	;创建开始菜单内容
 	CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\${PRODUCT_MAIN_EXE}"
	;创建桌面快捷方式
	CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\${PRODUCT_MAIN_EXE}"
	
	;计算程序大小，并写入注册表
	${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
 	IntFmt $0 "0x%08X" $0
  WriteRegDWORD HKLM "${Unstall_KeyPath}${PRODUCT_NAME_EN}" "EstimatedSize" "$0"

   
SectionEnd

Section -AdditionalIcons
	;公司官网链接
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  ;开始菜单-公司官网链接
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  ;开始菜单-卸载程序快捷方式
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
	;创建退出程序
  WriteUninstaller "$INSTDIR\uninst.exe"
  
  ;卸载信息注册表
  WriteRegStr HKLM "${Unstall_KeyPath}${PRODUCT_NAME_EN}" "DisplayName" "${PRODUCT_NAME}"
  WriteRegStr HKLM "${Unstall_KeyPath}${PRODUCT_NAME_EN}" "UninstallString" '"$INSTDIR\uninst.exe"'
  WriteRegStr HKLM "${Unstall_KeyPath}${PRODUCT_NAME_EN}" "DisplayIcon" '"$INSTDIR\${PRODUCT_MAIN_EXE}"'
  WriteRegStr HKLM "${Unstall_KeyPath}${PRODUCT_NAME_EN}" "InstallLocation" "$INSTDIR"
  WriteRegStr HKLM "${Unstall_KeyPath}${PRODUCT_NAME_EN}" "Publisher" "${PRODUCT_PUBLISHER}"
  WriteRegStr HKLM "${Unstall_KeyPath}${PRODUCT_NAME_EN}" "HelpLink" "${PRODUCT_WEB_SITE}"
  WriteRegStr HKLM "${Unstall_KeyPath}${PRODUCT_NAME_EN}" "DisplayVersion" "${PRODUCT_VERSION}"
		/*
  	  卸载注册表-可选参数
	  InstallLocation (string) - Installation directory ($INSTDIR)
		DisplayIcon (string) - Path, filename and index of of the icon that will be displayed next to your application name
		Publisher (string) - (Company) name of the publisher
		ModifyPath (string) - Path and filename of the application modify program
		InstallSource (string) - Location where the application was installed from
		ProductID (string) - Product ID of the application
		Readme (string) - Path (File or URL) to readme information
		RegOwner (string) - Registered owner of the application
		RegCompany (string) - Registered company of the application
		HelpLink (string) - Link to the support website
		HelpTelephone (string) - Telephone number for support
		URLUpdateInfo (string) - Link to the website for application updates
		URLInfoAbout (string) - Link to the application home page
		DisplayVersion (string) - Displayed version of the application
		VersionMajor (DWORD) - Major version number of the application
		VersionMinor (DWORD) - Minor version number of the application
		NoModify (DWORD) - 1 if uninstaller has no option to modify the installed application
		NoRepair (DWORD) - 1 if the uninstaller has no option to repair the installation
		SystemComponent (DWORD) - Set 1 to prevents display of the application in the Programs List of the Add/Remove Programs in the Control Panel.
		EstimatedSize (DWORD) - The size of the installed files (in KB)
		Comments (string) - A comment describing the installer package
	*/

SectionEnd



;Uninstaller Section

Section "Uninstall"
  ;执行uninstall.exe
  Delete "$SMSTARTUP\${PRODUCT_NAME}.lnk"
  Delete "$DESKTOP\${PRODUCT_NAME}.lnk"
  Delete "$QUICKLAUNCH\${PRODUCT_NAME}.lnk"

  SetShellVarContext all
  RMDir /r /REBOOTOK "$SMPROGRAMS\${PRODUCT_NAME}"

  SetShellVarContext current
  RMDir /r /REBOOTOK "$SMPROGRAMS\${PRODUCT_NAME}"
  
  SetRebootFlag false
  RMDir /r /REBOOTOK "$INSTDIR"
  ;DeleteRegKey ${PRODUCT_ROOT_KEY} "${PRODUCT_SUB_KEY}"

  SetRebootFlag false
  DeleteRegKey HKLM "${Unstall_KeyPath}${PRODUCT_NAME_EN}"

  Delete "$INSTDIR\Uninstall.exe"
  ;RMDir "$INSTDIR"
SectionEnd

;############################ 安装过程函数定义 ############################
; 快速安装按钮点击处理函数
Function OnBtnInstallClick
 	;停止消息循环
	setupdll::NSISScriptSendMessage /NOUNLOAD $Dialog "WM_SJMSGLOOPSTOP" "install"
FunctionEnd

;变更路径
Function OnChangePathClick
		;打开文件对话框
		setupdll::NSISOpenFolderDialog /NOUNLOAD "请选择安装目录"
		Pop $0
		
		${If} $0 == "-1"
		${Else}
      StrCpy $INSTDIR "$0\${PRODUCT_NAME_EN}"
      ;可用磁盘空间
			${GetRoot} $INSTDIR $1
			${DriveSpace} $1 "/D=F /S=M" $2
			setupdll::NSISScriptSendMessage /NOUNLOAD $Dialog "WM_SJTHREEUPDATEFREESPACE" "$2"
			;软件所需空间大小
			setupdll::NSISScriptSendMessage /NOUNLOAD $Dialog "WM_SJTHREEUPDATEREQUIREDSPACE" "${REQUIRED_SPACE}"
			;默认安装路径
			setupdll::SetControlProperties /NOUNLOAD "SEdit" "edit_folder" "text" $INSTDIR
   ${EndIf}
FunctionEnd

;点击“立即体验",完成并开始运行程序(消息循环已经停止，不会阻塞，可直接操作界面)
Function OnFinishRun
	;打开程序运行
	Exec '"$INSTDIR\${PRODUCT_MAIN_EXE}"'
  setupdll::ClosePage /NOUNLOAD
FunctionEnd

Function OnFinish
  setupdll::ClosePage /NOUNLOAD
FunctionEnd

;关闭程序
Function OnClose
  setupdll::NSISMessageBox /NOUNLOAD "quit install?"
  Pop $0
  ${If} $0 == "1"
    ;关闭程序
	  setupdll::ClosePage /NOUNLOAD
	${EndIf}
FunctionEnd


Function MyPage

	;解决闪屏
 	;ShowWindow $HWNDPARENT ${SW_HIDE}
	${NSW_SetWindowSize} $HWNDPARENT 0 0 ;改变自定义窗体大小


	;/////初始化窗口/////
	setupdll::InitWindow /NOUNLOAD "$PLUGINSDIR"
	Pop $Dialog
	;/////绑定控件与脚本函数对应关系//////
	

	setupdll::FindChildByName /NOUNLOAD "btn_install"
	Pop $0
	${If} $0 == "-1"
		MessageBox MB_OK ERROR_MESSAGE
	${Else}
		GetFunctionAddress $0 OnBtnInstallClick
		setupdll::BindControlAndNSISScript /NOUNLOAD "btn_install" $0
	${EndIf}
	
  setupdll::FindChildByName /NOUNLOAD "btn_change_folder"
	Pop $0
	${If} $0 == "-1"
		MessageBox MB_OK ERROR_MESSAGE
	${Else}
		GetFunctionAddress $0 OnChangePathClick
		setupdll::BindControlAndNSISScript /NOUNLOAD "btn_change_folder" $0
	${EndIf}
	
	setupdll::FindChildByName /NOUNLOAD "btn_close"
	Pop $0
	${If} $0 == "-1"
		MessageBox MB_OK ERROR_MESSAGE
	${Else}
		GetFunctionAddress $0 OnClose
		setupdll::BindControlAndNSISScript /NOUNLOAD "btn_close" $0
	${EndIf}

	setupdll::FindChildByName /NOUNLOAD "btn_open"
	Pop $0
	${If} $0 == "-1"
		MessageBox MB_OK ERROR_MESSAGE
	${Else}
		GetFunctionAddress $0 OnFinishRun
		setupdll::BindControlAndNSISScript /NOUNLOAD "btn_open" $0
	${EndIf}

	setupdll::FindChildByName /NOUNLOAD "btn_finish"
	Pop $0
	${If} $0 == "-1"
		MessageBox MB_OK ERROR_MESSAGE
	${Else}
		GetFunctionAddress $0 OnFinish
		setupdll::BindControlAndNSISScript /NOUNLOAD "btn_finish" $0
	${EndIf}

	;/////初始化数据/////

	;可用磁盘空间
	${GetRoot} $INSTDIR $0
	${DriveSpace} $0 "/D=F /S=M" $2
	setupdll::NSISScriptSendMessage /NOUNLOAD $Dialog "WM_SJTHREEUPDATEFREESPACE" "$2"
	
	;软件所需空间大小
	setupdll::NSISScriptSendMessage /NOUNLOAD $Dialog "WM_SJTHREEUPDATEREQUIREDSPACE" "${REQUIRED_SPACE}"
	
	;默认安装路径
	setupdll::SetControlProperties /NOUNLOAD "SEdit" "edit_folder" "text" $INSTDIR
	
	;/////显示窗口/////
	setupdll::ShowPage /NOUNLOAD
FunctionEnd

Function MyInstallNow
	;解决闪屏
  ShowWindow $HWNDPARENT ${SW_HIDE}
	${NSW_SetWindowSize} $HWNDPARENT 0 0 ;改变自定义窗体大小

	;切换视图
	setupdll::SetControlProperties /NOUNLOAD "STabCtrl" "tab_main" "curSel" "2"

   ;绑定进度
  setupdll::BindingProgress  /NOUNLOAD "pg_installing"
FunctionEnd

Function CreateMutex
	;检查安装互斥
	ReCheck:
	System::Call 'kernel32::CreateMutexA(i 0, i 0, t "${MyMutex_Install}") i .R1 ?e'
	Pop $R0
	System::Call 'kernel32::CloseHandle(i R1) i.s'
	;判断安装互斥的存在
	${If} $R0 != 0
		MessageBox MB_RetryCancel|MB_ICONEXCLAMATION "安装程序已经运行！" IdRetry ReCheck
		Quit
	${Else}
		;创建安装互斥：
		System::Call 'kernel32::CreateMutexA(i 0, i 0, t "${MyMutex_Install}") i .R1 ?e'
		Pop $R0
		StrCmp $R0 0 +2
		Quit
	${EndIf}
FunctionEnd

Function un.CreateMutex
	;检查卸载互斥：
	ReCheck:
	System::Call 'kernel32::CreateMutexA(i 0, i 0, t "${MyMutex_UnInstall}") i .R1 ?e'
	Pop $R0
	System::Call 'kernel32::CloseHandle(i R1) i.s'
	;判断卸载互斥的存在
	${If} $R0 != 0
		MessageBox MB_RetryCancel|MB_ICONEXCLAMATION "卸载程序已经运行！" IdRetry ReCheck
		Quit
	${Else}
		;创建安装互斥：
		System::Call 'kernel32::CreateMutexA(i 0, i 0, t "${MyMutex_UnInstall}") i .R1 ?e'
		Pop $R0
		StrCmp $R0 0 +2
		Quit
	${EndIf}
FunctionEnd

Function .onInit

	;加载UI界面需要的资源
	InitPluginsDir
  SetOutPath $PLUGINSDIR
  File /r `.\plugins\*.*`
	;检查程序互斥
	Call CreateMutex

FunctionEnd

;############################################卸载############################################
Function un.OnClose
  setupdll::NSISMessageBox /NOUNLOAD "quit uninstall?"
  Pop $0
  ${If} $0 == "1"
    ;关闭程序
	  setupdll::ClosePage /NOUNLOAD
	${EndIf}
FunctionEnd
;卸载第一页进入下一页
Function un.OnBtnNextClick
	setupdll::NSISScriptSendMessage /NOUNLOAD $Dialog "WM_SJUNSTALLSTARTNEXT"
FunctionEnd

Function un.OnBtnCancellClick
	setupdll::NSISMessageBox /NOUNLOAD "quit uninstall?"
  Pop $0
  ${If} $0 == "1"
    ;关闭程序
	  setupdll::ClosePage /NOUNLOAD
	${EndIf}
FunctionEnd

Function un.OnUnstallClick
		;停止消息循环
		setupdll::NSISScriptSendMessage /NOUNLOAD $Dialog "WM_SJMSGLOOPSTOP" "uninstall"
FunctionEnd

Function un.OnFinishClick
  setupdll::ClosePage /NOUNLOAD
FunctionEnd

Function un.MyUninstallPage
  ;解决闪屏
 	ShowWindow $HWNDPARENT ${SW_HIDE}
	${NSW_SetWindowSize} $HWNDPARENT 0 0 ;改变自定义窗体大小

	;/////初始化窗口/////
	setupdll::InitWindow /NOUNLOAD "$PLUGINSDIR"
	Pop $Dialog

	;切换到卸载界面
	setupdll::SetControlProperties /NOUNLOAD "STabCtrl" "tab_main" "curSel" "4"
	;隐藏最小化按钮
	setupdll::SetControlProperties /NOUNLOAD "SImageButton" "btn_min" "visible" "false"
	;显示卸载标题
	setupdll::SetControlProperties /NOUNLOAD "SStatic" "txt_titlename" "text" "Sha Monk MM UnInstall Guide"

	;绑定关闭按钮
	setupdll::FindChildByName /NOUNLOAD "btn_close"
	Pop $0
	${If} $0 == "-1"
		MessageBox MB_OK ERROR_MESSAGE
	${Else}
		GetFunctionAddress $0 un.OnClose
		setupdll::BindControlAndNSISScript /NOUNLOAD "btn_close" $0
	${EndIf}
	;/////第一页面/////
	;取消按钮
	setupdll::FindChildByName /NOUNLOAD "btn_unstallchoose_previous"
	Pop $0
	${If} $0 == "-1"
		MessageBox MB_OK ERROR_MESSAGE
	${Else}
		GetFunctionAddress $0 un.OnBtnCancellClick
		setupdll::BindControlAndNSISScript /NOUNLOAD "btn_unstallchoose_previous" $0
	${EndIf}
	;卸载按钮
		setupdll::FindChildByName /NOUNLOAD "btn_unstallchoose_unnstall"
	Pop $0
	${If} $0 == "-1"
		MessageBox MB_OK ERROR_MESSAGE
	${Else}
		GetFunctionAddress $0 un.OnUnstallClick
		setupdll::BindControlAndNSISScript /NOUNLOAD "btn_unstallchoose_unnstall" $0
	${EndIf}
	;////最后一个页面/////
	;完成按钮
	setupdll::FindChildByName /NOUNLOAD "btn_unstallfinish"
	Pop $0
	${If} $0 == "-1"
		MessageBox MB_OK ERROR_MESSAGE
	${Else}
		GetFunctionAddress $0 un.OnFinishClick
		setupdll::BindControlAndNSISScript /NOUNLOAD "btn_unstallfinish" $0
	${EndIf}
	
	;获取要卸载的目录
	ReadRegStr $0 HKLM "${Unstall_KeyPath}${PRODUCT_NAME_EN}" "InstallLocation"
  ;显示卸载的目录
	setupdll::SetControlProperties /NOUNLOAD "SEdit" "edit_unstallchoose_url" "text" "$0"

	;/////显示窗口/////
	setupdll::ShowPage /NOUNLOAD
FunctionEnd

Function un.MyUninstallNow
	;解决闪屏
 	ShowWindow $HWNDPARENT ${SW_HIDE}
	${NSW_SetWindowSize} $HWNDPARENT 0 0 ;改变自定义窗体大小

  setupdll::SetControlProperties /NOUNLOAD "STabCtrl" "tab_main" "curSel" "5"
  setupdll::BindingProgress  /NOUNLOAD "pg_unstalling"
  
FunctionEnd

Function un.onInit

  ;加载UI界面需要的资源
	InitPluginsDir
	SetOutPath $PLUGINSDIR
  File /r `plugins\*.*`
	;检查程序互斥
	Call un.CreateMutex
FunctionEnd
