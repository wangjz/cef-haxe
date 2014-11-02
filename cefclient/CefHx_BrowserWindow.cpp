#include <windows.h>
#include <commdlg.h>
#include <shellapi.h>
#include <direct.h>
#include <sstream>
#include <string>
#include <stddef.h>
#include <stdio.h>
#include <tlhelp32.h>
#include "cefclient/CefHx_BrowserWindow.h"
#include "include/cef_app.h"
#include "include/cef_base.h"
#include "include/cef_browser.h"
#include "include/cef_client.h"
#include "include/cef_command_line.h"
#include "include/cef_frame.h"
#include "include/cef_runnable.h"
#include "include/cef_web_plugin.h"
#include "include/cef_web_urlrequest.h"
#include "cefclient/ExternalInterface.h"
#include "cefclient/client_handler.h"


//window_list *windows = NULL;
static LRESULT CALLBACK WndProcStub( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );
static LRESULT CALLBACK InitialWndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );
static void fullscreen(window *w, bool enter);
//static LRESULT sendEvent( window *w, UINT msg, WPARAM wParam, LPARAM lParam );

#define CLASS_NAME TEXT("CEFCLIENT")

//#define AC_SRC_ALPHA 0x01
//#define WS_EX_LAYERED 0x00080000
//#define ULW_ALPHA 0x00000002

//typedef BOOL WINAPI UpdateLayeredWindowProc(HWND,HDC,POINT *,SIZE *,HDC,POINT *,COLORREF,BLENDFUNCTION *,DWORD);
//static UpdateLayeredWindowProc* pUpdateLayeredWindow = NULL;
//static HMODULE user32 = NULL;

int system_init() {
	WNDCLASSEX wcl;
	HINSTANCE hinst = GetModuleHandle(NULL);
	memset(&wcl,0,sizeof(wcl));
	wcl.cbSize			= sizeof(WNDCLASSEX);
	wcl.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcl.lpfnWndProc		= InitialWndProc;
	wcl.cbClsExtra		= 0;
	wcl.cbWndExtra		= sizeof(window*);
	wcl.hInstance		= hinst;
	wcl.hIcon			= LoadIcon(hinst,MAKEINTRESOURCE(104));
	wcl.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wcl.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);
	wcl.lpszMenuName	= 0;
	wcl.lpszClassName	= CLASS_NAME;
	wcl.hIconSm			= LoadIcon(hinst,MAKEINTRESOURCE(104));;
	if( RegisterClassEx(&wcl) == 0 )
		return -1;
	// this will create our main message queue
	/*user32 = LoadLibrary("user32.dll");
	if (user32) {
		pUpdateLayeredWindow = (UpdateLayeredWindowProc*) GetProcAddress(user32,"UpdateLayeredWindow");
		if (!pUpdateLayeredWindow)
			FreeLibrary(user32);
	}*/

	return 0;
}

// -------------------------------------- WINDOW -----------------------------------------

//typedef void SetBufferProc(void*);
//typedef void PaintBufferProc(void*,void*);


window *system_window_create(int width, int height, enum WindowFlags flags,value window_hxobj) {
	RECT rc = { 0, 0, width, height };
	HWND hwnd;
	int style;
	DWORD exstyle = 0;
	window *w =new window();
	w->flags = flags;
	w->window_hxobj=new AutoGCRoot(window_hxobj);
	//w->browser_hxobj=browser_hxobj;
	if ( flags & WF_ALWAYS_ONTOP )
		exstyle |= WS_EX_TOPMOST;
	if ( flags & WF_NO_TASKBAR )
		exstyle |= WS_EX_TOOLWINDOW;
	style = WS_OVERLAPPEDWINDOW;
	AdjustWindowRectEx( &rc, style, FALSE, exstyle);

	if (flags & WF_TRANSPARENT ) { //&& pUpdateLayeredWindow
		//exstyle |=  0x80000; //== WS_EX_LAYERED;
		style = WS_SYSMENU | WS_POPUP;
		//w->bbuffer_set = setBackBufferTrans;
		//w->bbuffer_paint = paintBackBufferTrans;
	} else {
		if (flags & WF_PLAIN) {
			style = WS_POPUP | WS_BORDER | WS_SYSMENU;
			exstyle |= WS_EX_APPWINDOW;
		} else {
			style= WS_OVERLAPPEDWINDOW;
		}
		AdjustWindowRectEx( &rc, style, FALSE, exstyle);
		//w->bbuffer_set = setBackBufferStd;
		//w->bbuffer_paint = paintBackBufferStd;
	}


	hwnd = CreateWindowEx(exstyle, CLASS_NAME,NULL,style|WS_CLIPCHILDREN,CW_USEDEFAULT,CW_USEDEFAULT,rc.right-rc.left,rc.bottom-rc.top,NULL,NULL,GetModuleHandle(NULL),w);

	if(flags&WF_NO_CAPTION){
		HINSTANCE hInst = LoadLibrary(TEXT("UxTheme.dll"));
        if (hInst)
        {
            typedef HRESULT (WINAPI *PFUN_SetWindowTheme)(HWND, LPCWSTR, LPCWSTR);
            
            PFUN_SetWindowTheme pFun = (PFUN_SetWindowTheme)GetProcAddress(hInst, "SetWindowTheme");
    
            if (pFun)
                pFun(hwnd, TEXT(""), TEXT(""));        //去掉xp主题
    
            FreeLibrary(hInst);
        }
    
        hInst = LoadLibrary(TEXT("dwmapi.dll"));
        if (hInst)
        {
            typedef HRESULT (WINAPI * TmpFun)(HWND,DWORD,LPCVOID,DWORD);
            TmpFun DwmSetWindowAttributeEX = (TmpFun)::GetProcAddress(hInst, "DwmSetWindowAttribute");
            
            if (DwmSetWindowAttributeEX)
            {
                DWORD dwAttr = 1;
                DwmSetWindowAttributeEX(hwnd, 2, &dwAttr, 4);    //去掉vista特效
            }
    
            FreeLibrary(hInst);
        }
	}
	
	if( hwnd == NULL ) {
		delete w;
		return NULL;
	}
	return w;
}

void system_window_show( window *w, int show,bool isFront,bool isAsync,bool isActive ) {
	if(isAsync){
		if(isActive){
			ShowWindowAsync( w->hwnd, show?SW_SHOW:SW_HIDE );
		}else{
			ShowWindowAsync( w->hwnd, show?SW_SHOWNA:SW_HIDE );
		}
		
	}else{
		if(isActive){
			ShowWindow( w->hwnd, show?SW_SHOW:SW_HIDE );
		}else{
			ShowWindow( w->hwnd, show?SW_SHOWNA:SW_HIDE );
		}
	}
	if(show&&isFront){
		if(isActive){
			HWND hFrgWnd = ::GetForegroundWindow();
			AttachThreadInput( GetWindowThreadProcessId(hFrgWnd, NULL), GetCurrentThreadId(), TRUE );
			SetForegroundWindow(w->hwnd);
            BringWindowToTop(w->hwnd);
			SwitchToThisWindow(w->hwnd, TRUE);
   
			AttachThreadInput(GetWindowThreadProcessId(hFrgWnd, NULL), GetCurrentThreadId(), FALSE);
		}else{
			SetForegroundWindow(w->hwnd);
		}
	}
}

void system_window_close( window *w ) {
	if( w->hwnd )PostMessage(w->hwnd,WM_CLOSE,NULL,NULL);
}

void system_window_destroy( window *w ) {
	if( w->hwnd )
		DestroyWindow(w->hwnd);
}

void system_quit(){
	::PostQuitMessage(0);
	if(hInstanceMutex){
		ReleaseMutex(hInstanceMutex); 
        CloseHandle(hInstanceMutex); 
	}
}


void *system_window_get_handle( window *w ) {
	return w->hwnd;
}


#define SET(v,flags,toggle) v = toggle ?(v | flags) :(v & ~flags)
#define IS_SET(v,flags)	(((v) & flags) == flags)

void system_window_set_prop( window *w, enum WindowProperty prop, int value ) {
	DWORD style = GetWindowLong(w->hwnd,GWL_STYLE);
	switch( prop ) {
		case WP_RESIZABLE:
			if (!(w->flags & (WF_PLAIN | WF_TRANSPARENT))) {
				SET(style,WS_THICKFRAME,value);
				SetWindowLong(w->hwnd,GWL_STYLE,style);
			}
			break;
		case WP_MAXIMIZE_ICON:
			SET(style,WS_MAXIMIZEBOX ,value);
			SetWindowLong(w->hwnd,GWL_STYLE,style);
			break;
		case WP_MINIMIZE_ICON:
			SET(style,WS_MINIMIZEBOX ,value);
			SetWindowLong(w->hwnd,GWL_STYLE,style);
			break;
		case WP_WIDTH:
		case WP_HEIGHT:	{
			RECT rc;
			GetClientRect(w->hwnd,&rc);
			if( prop == WP_WIDTH )
				rc.right = value;
			else
				rc.bottom = value;
			AdjustWindowRectEx( &rc, style, FALSE, 0);
			SetWindowPos(w->hwnd,NULL,0,0,rc.right - rc.left,rc.bottom - rc.top, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER );
			break;
		}
		case WP_TOP:
		case WP_LEFT: {
			RECT rc;
			GetWindowRect(w->hwnd,&rc);
			if( prop == WP_LEFT )
				rc.left = value;
			else
				rc.top = value;
			SetWindowPos(w->hwnd,NULL,rc.left,rc.top,0,0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER );
			break;
		}
		case WP_FULLSCREEN:
			if( w->flags & WF_FULLSCREEN ) {
				if( !value ) {
					fullscreen(w, 0);
					int flags=w->flags;
					flags=(flags^=int(WF_FULLSCREEN));
					w->flags=(WindowFlags)flags;
				}
			} else if( value ) {
				fullscreen(w, 1);
				int flags=w->flags;
				flags=(flags|=int(WF_FULLSCREEN));
				w->flags=(WindowFlags)flags;
			}
			break;
		case WP_DROPTARGET:
			/*DragAcceptFiles(w->hwnd,value!=0);
			if (value)
				w->flags &= ~WF_DROPTARGET;
			else
				w->flags |= WF_DROPTARGET;*/
			break;
		case WP_MINIMIZED:
			if( value ) {
				if( !IsIconic(w->hwnd))SendMessage(w->hwnd,WM_SYSCOMMAND,SC_MINIMIZE,0);
					//ShowWindow(w->hwnd,SW_MINIMIZE);
			} else {
				if( IsIconic(w->hwnd))SendMessage(w->hwnd,WM_SYSCOMMAND,SC_RESTORE,0);
					//ShowWindow(w->hwnd,SW_RESTORE);
			}
			break;
		case WP_MAXIMIZED: {
			if( value ) {
				if( !IsZoomed(w->hwnd))SendMessage(w->hwnd,WM_SYSCOMMAND,SC_MAXIMIZE,0);
					//ShowWindow(w->hwnd,SW_MAXIMIZE);
			} else {
				if( IsZoomed(w->hwnd))SendMessage(w->hwnd,WM_SYSCOMMAND,SC_RESTORE,0);
					//ShowWindow(w->hwnd,SW_RESTORE);
			}
			break;
		case WP_CAPTION:{
				SET(style,(WS_CAPTION|WS_BORDER) ,value);
				SetWindowLong(w->hwnd,GWL_STYLE,style);
			}
			break;
		case WP_ENABLED:{
				EnableWindow(w->hwnd,value);
			}
		}
	}
}

int system_window_get_prop( window *w, enum WindowProperty prop ) {
	DWORD style = GetWindowLong(w->hwnd,GWL_STYLE);
	RECT rc;
	switch( prop ) {
		case WP_RESIZABLE:
			return IS_SET(style,WS_THICKFRAME);
		case WP_MAXIMIZE_ICON:
			return IS_SET(style,WS_MAXIMIZEBOX);
		case WP_MINIMIZE_ICON:
			return IS_SET(style,WS_MINIMIZEBOX);
		case WP_WIDTH:
			GetClientRect(w->hwnd,&rc);
			return rc.right;
		case WP_HEIGHT:
			GetClientRect(w->hwnd,&rc);
			return rc.bottom;
		case WP_TOP:
			GetWindowRect(w->hwnd,&rc);
			return rc.top;
		case WP_LEFT:
			GetWindowRect(w->hwnd,&rc);
			return rc.left;
		case WP_FULLSCREEN:
			return (w->flags & WF_FULLSCREEN);
		case WP_TRANSPARENT:
			return (w->flags & WF_TRANSPARENT);
		case WP_DROPTARGET:
			return (w->flags & WF_DROPTARGET);
		case WP_MINIMIZED:
			return IsIconic(w->hwnd);
		case WP_MAXIMIZED:
			return IsZoomed(w->hwnd);
		case WP_CAPTION:
			return IS_SET(style,(WS_CAPTION|WS_BORDER));
		case WP_ENABLED:
			return IsWindowEnabled(w->hwnd);			
	}
	return 0;
}


void system_window_set_title( window *w, const wchar_t * title ) {
	SetWindowText(w->hwnd, (LPCWSTR)title);
}

void system_window_drag( window *w ) {
	ReleaseCapture();
	SendMessage(w->hwnd,WM_SYSCOMMAND,SC_MOVE+1,0);
}

void system_window_resize( window *w, int o ) {
	ReleaseCapture();
	SendMessage(w->hwnd,WM_SYSCOMMAND,SC_SIZE+o,0);
}

void system_window_round(window* w,int x,int y,int width,int height,int szWidth,int szHeight){
	if(w->hrgn){
		DeleteObject(w->hrgn);
	}
	w->hrgn=NULL;
	w->hrgn = CreateRoundRectRgn(x,y,width,height,szWidth,szHeight);
	SetWindowRgn(w->hwnd,w->hrgn,TRUE);
}

void system_window_onresload(window* w,const char* mime,const char* url,const char* redirectUrl,int status,unsigned char * bytes,bool isCancel,int bytesLen){
	//printf("%d",(w&&w->g_handler));
	if(w&&w->g_handler)w->g_handler->OnResourceLoaded(mime,url,redirectUrl,status,bytes,isCancel,bytesLen);
}
int system_getmetrics(int inx){
	return GetSystemMetrics(inx);
}

unsigned long  system_process_findid(const wchar_t *processName){
	HANDLE handle=::CreateToolhelp32Snapshot(TH32CS_SNAPALL,0);
	PROCESSENTRY32 Info;
	Info.dwSize = sizeof(PROCESSENTRY32);
	if(::Process32First(handle,&Info))
	{
		do{			
			if(lstrcmpi(processName,Info.szExeFile)==0)
			{
				::CloseHandle(handle);
				return Info.th32ProcessID;
			}
		}
		while(::Process32Next(handle,&Info));	
		::CloseHandle(handle);
	}
	return -1;
}

bool system_process_close(const wchar_t *processName){
	unsigned long  id=system_process_findid(processName);
	if(id!=-1){
		HANDLE ProcessHandle= ::OpenProcess( PROCESS_TERMINATE, FALSE,id );
		if(ProcessHandle)return TerminateProcess(ProcessHandle,4)==1;
	}
	return false;
}

bool system_process_closebyid(unsigned long id){
	if(id!=-1){
		HANDLE ProcessHandle= ::OpenProcess( PROCESS_TERMINATE, FALSE,id );
		if(ProcessHandle)return TerminateProcess(ProcessHandle,4)==1;
	}
	return false;
}

bool  system_registeinstance(const wchar_t *instanceName){
	 hInstanceMutex = ::CreateMutex(NULL,TRUE, instanceName); 
     if(GetLastError()   ==   ERROR_ALREADY_EXISTS)   { 
        if(hInstanceMutex) CloseHandle(hInstanceMutex); 
		hInstanceMutex=NULL;
		HWND   hPrevApp   = ::FindWindow(NULL,instanceName); 
        if(hPrevApp)   { 

             PostMessage(hPrevApp,0x2328,0,0); 
        } 
	 	return false;
     } 
	 return true;
}
static LRESULT WndProc( window *w, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch(msg)
	{
		case WM_COPYDATA:
			{
				//if(w->isMessageHook){
					COPYDATASTRUCT *pCopyData = (COPYDATASTRUCT*)lparam;
					if(wparam==0){
						val_call3(val_field(w->window_hxobj->get(),_id_on_sysmsg),alloc_string("process"),alloc_null(),alloc_string((char*)pCopyData->lpData));
					}else{
						val_call3(val_field(w->window_hxobj->get(),_id_on_sysmsg),alloc_string("process"),alloc_abstract(gObjectKind,(HWND)wparam),alloc_abstract(gObjectKind,pCopyData));
					}
				//}
			}
			return 1;
		case 0x2325://task
			val_ocall2(w->window_hxobj->get(),_id_onBrowserEvent,alloc_string("on_task"),alloc_int(lparam));
			return 1;
		case 0x2326:
			{
				char* resUrl=(char*)lparam;
				val_ocall2(w->window_hxobj->get(),_id_onBrowserEvent,alloc_string("onBeforeResourceLoad"),alloc_string(resUrl));
				if(int(wparam)==1){
					delete []resUrl;
				}
			}

			return 1;
		case 0x2327:
			//if(w->isMessageHook){
				val_call3(val_field(w->window_hxobj->get(),_id_on_sysmsg),alloc_string("plug"),alloc_int(wparam),alloc_string((char*)lparam));
			//}
			return 1;
		case 0x2328: //其它实例运行消息
			//if(w->isMessageHook){
				val_call3(val_field(w->window_hxobj->get(),_id_on_sysmsg),alloc_string("newapp"),alloc_int(wparam),alloc_int(lparam));
			//}
			return 0;
		case 0x2329: //拖盘事件
		{
			//if(w->isMessageHook){
				val_call3(val_field(w->window_hxobj->get(),_id_on_sysmsg),alloc_string("tray"),alloc_int(wparam),alloc_int(lparam));
			//}
		}
			return 0;
		case WM_HOTKEY:
		{
			//if(w->isMessageHook){
				val_call3(val_field(w->window_hxobj->get(),_id_on_sysmsg),alloc_string("hotkey"),alloc_int(wparam),alloc_int(lparam));
			//}
		}
			break;
		case WM_ENTERMENULOOP:
		{
		  if (!wparam) {
			// Entering the menu loop for the application menu.
			CefSetOSModalLoop(true);
		  }
		}
		  break;

		case WM_EXITMENULOOP:
		{
		  if (!wparam) {
			// Exiting the menu loop for the application menu.
			CefSetOSModalLoop(false);
		  }
		}
		  break;

		case WM_CREATE:
		{
			
			CefBrowserSettings browserSettings;
			value browserSet_hxobj=val_field(w->window_hxobj->get(),_id_cef_browserset);
			if(!val_is_null(browserSet_hxobj)) //to init browserset
			{
				value v_accelerated2DCanvasDisabled=val_field(browserSet_hxobj,_id_accelerated2DCanvasDisabled);
				bool _accelerated2DCanvasDisabled=val_bool(v_accelerated2DCanvasDisabled);
				browserSettings.accelerated_2d_canvas_disabled=_accelerated2DCanvasDisabled;

				value v_acceleratedCompositingEnabled=val_field(browserSet_hxobj,_id_acceleratedCompositingEnabled);
				bool _acceleratedCompositingEnabled=val_bool(v_acceleratedCompositingEnabled);
				browserSettings.accelerated_compositing_enabled=_acceleratedCompositingEnabled;

				value v_acceleratedPaintingDisabled=val_field(browserSet_hxobj,_id_acceleratedPaintingDisabled);
				bool _acceleratedPaintingDisabled=val_bool(v_acceleratedPaintingDisabled);
				browserSettings.accelerated_painting_disabled=_acceleratedPaintingDisabled;

				value v_acceleratedLayersDisabled=val_field(browserSet_hxobj,_id_acceleratedLayersDisabled);
				bool _acceleratedLayersDisabled=val_bool(v_acceleratedLayersDisabled);
				browserSettings.accelerated_layers_disabled=_acceleratedLayersDisabled;

				value v_acceleratedFiltersDisabled=val_field(browserSet_hxobj,_id_acceleratedFiltersDisabled);
				bool _acceleratedFiltersDisabled=val_bool(v_acceleratedFiltersDisabled);
				browserSettings.accelerated_filters_disabled=_acceleratedFiltersDisabled;

				value v_acceleratedPluginsDisabled=val_field(browserSet_hxobj,_id_acceleratedPluginsDisabled);
				bool _acceleratedPluginsDisabled=val_bool(v_acceleratedPluginsDisabled);
				browserSettings.accelerated_plugins_disabled=_acceleratedPluginsDisabled;

				value v_acceleratedVideoDisabled=val_field(browserSet_hxobj,_id_acceleratedVideoDisabled);
				bool _acceleratedVideoDisabled=val_bool(v_acceleratedVideoDisabled);
				browserSettings.accelerated_video_disabled=_acceleratedVideoDisabled;

				value v_applicationCacheDisabled=val_field(browserSet_hxobj,_id_applicationCacheDisabled);
				bool _applicationCacheDisabled=val_bool(v_applicationCacheDisabled);
				browserSettings.application_cache_disabled=_applicationCacheDisabled;

				value v_authorAndUserStylesDisabled=val_field(browserSet_hxobj,_id_authorAndUserStylesDisabled);
				bool _authorAndUserStylesDisabled=val_bool(v_authorAndUserStylesDisabled);
				browserSettings.author_and_user_styles_disabled=_authorAndUserStylesDisabled;

				value v_caretBrowsingEnabled=val_field(browserSet_hxobj,_id_caretBrowsingEnabled);
				bool _caretBrowsingEnabled=val_bool(v_caretBrowsingEnabled);
				browserSettings.caret_browsing_enabled=_caretBrowsingEnabled;

				value v_databasesDisabled=val_field(browserSet_hxobj,_id_databasesDisabled);
				bool _databasesDisabled=val_bool(v_databasesDisabled);
				browserSettings.databases_disabled=_databasesDisabled;

				value v_defaultEncoding=val_field(browserSet_hxobj,_id_defaultEncoding);
				if(val_is_null(v_defaultEncoding)==false)
				{
					
					const char* _defaultEncoding=val_string(v_defaultEncoding);
					CefString(&browserSettings.default_encoding)=_defaultEncoding;
				}

				value v_developerToolsDisabled=val_field(browserSet_hxobj,_id_developerToolsDisabled);
				bool _developerToolsDisabled=val_bool(v_developerToolsDisabled);
				browserSettings.developer_tools_disabled=_developerToolsDisabled;

				value v_domPasteDisabled=val_field(browserSet_hxobj,_id_domPasteDisabled);
				bool _domPasteDisabled=val_bool(v_domPasteDisabled);
				browserSettings.dom_paste_disabled=_domPasteDisabled;

				value v_dragDropDisabled=val_field(browserSet_hxobj,_id_dragDropDisabled);
				bool _dragDropDisabled=val_bool(v_dragDropDisabled);
				browserSettings.drag_drop_disabled=_dragDropDisabled;

				value v_encodingDetectorEnabled=val_field(browserSet_hxobj,_id_encodingDetectorEnabled);
				bool _encodingDetectorEnabled=val_bool(v_encodingDetectorEnabled);
				browserSettings.encoding_detector_enabled=_encodingDetectorEnabled;


				value v_fileAccessFromFileUrlsAllowed=val_field(browserSet_hxobj,_id_fileAccessFromFileUrlsAllowed);
				bool _fileAccessFromFileUrlsAllowed=val_bool(v_fileAccessFromFileUrlsAllowed);
				browserSettings.file_access_from_file_urls_allowed=_fileAccessFromFileUrlsAllowed;

				value v_universalAccessFromFileUrlsAllowed=val_field(browserSet_hxobj,_id_universalAccessFromFileUrlsAllowed);
				bool _universalAccessFromFileUrlsAllowed=val_bool(v_universalAccessFromFileUrlsAllowed);
				browserSettings.universal_access_from_file_urls_allowed=_universalAccessFromFileUrlsAllowed;

				value v_hyperlinkAuditingDisabled=val_field(browserSet_hxobj,_id_hyperlinkAuditingDisabled);
				bool _hyperlinkAuditingDisabled=val_bool(v_hyperlinkAuditingDisabled);
				browserSettings.hyperlink_auditing_disabled=_hyperlinkAuditingDisabled;

				value v_imageLoadDisabled=val_field(browserSet_hxobj,_id_imageLoadDisabled);
				bool _imageLoadDisabled=val_bool(v_imageLoadDisabled);
				browserSettings.image_load_disabled=_imageLoadDisabled;

				value v_javaDisabled=val_field(browserSet_hxobj,_id_javaDisabled);
				bool _javaDisabled=val_bool(v_javaDisabled);
				browserSettings.java_disabled=_javaDisabled;

				value v_javaScriptAccessClipboardDisallowed=val_field(browserSet_hxobj,_id_javaScriptAccessClipboardDisallowed);
				bool _javaScriptAccessClipboardDisallowed=val_bool(v_javaScriptAccessClipboardDisallowed);
				browserSettings.javascript_access_clipboard_disallowed=_javaScriptAccessClipboardDisallowed;

				value v_javaScriptCloseWindowsDisallowed=val_field(browserSet_hxobj,_id_javaScriptCloseWindowsDisallowed);
				bool _javaScriptCloseWindowsDisallowed=val_bool(v_javaScriptCloseWindowsDisallowed);
				browserSettings.javascript_close_windows_disallowed=_javaScriptCloseWindowsDisallowed;

				value v_javaScriptDisabled=val_field(browserSet_hxobj,_id_javaScriptDisabled);
				bool _javaScriptDisabled=val_bool(v_javaScriptDisabled);
				browserSettings.javascript_disabled=_javaScriptDisabled;

				value v_javaScriptOpenWindowsDisallowed=val_field(browserSet_hxobj,_id_javaScriptOpenWindowsDisallowed);
				bool _javaScriptOpenWindowsDisallowed=val_bool(v_javaScriptOpenWindowsDisallowed);
				browserSettings.javascript_open_windows_disallowed=_javaScriptOpenWindowsDisallowed;

				value v_localStorageDisabled=val_field(browserSet_hxobj,_id_localStorageDisabled);
				bool _localStorageDisabled=val_bool(v_localStorageDisabled);
				browserSettings.local_storage_disabled=_localStorageDisabled;


				value v_pageCacheDisabled=val_field(browserSet_hxobj,_id_pageCacheDisabled);
				bool _pageCacheDisabled=val_bool(v_pageCacheDisabled);
				browserSettings.page_cache_disabled=_pageCacheDisabled;


				value v_pluginsDisabled=val_field(browserSet_hxobj,_id_pluginsDisabled);
				bool _pluginsDisabled=val_bool(v_pluginsDisabled);
				browserSettings.plugins_disabled=_pluginsDisabled;

				value v_remoteFontsDisabled=val_field(browserSet_hxobj,_id_remoteFontsDisabled);
				bool _remoteFontsDisabled=val_bool(v_remoteFontsDisabled);
				browserSettings.remote_fonts_disabled=_remoteFontsDisabled;

				value v_shrinkStandaloneImagesToFit=val_field(browserSet_hxobj,_id_shrinkStandaloneImagesToFit);
				bool _shrinkStandaloneImagesToFit=val_bool(v_shrinkStandaloneImagesToFit);
				browserSettings.shrink_standalone_images_to_fit=_shrinkStandaloneImagesToFit;

				value v_siteSpecificQuirksDisabled=val_field(browserSet_hxobj,_id_siteSpecificQuirksDisabled);
				bool _siteSpecificQuirksDisabled=val_bool(v_siteSpecificQuirksDisabled);
				browserSettings.site_specific_quirks_disabled=_siteSpecificQuirksDisabled;

				value v_tabToLinksDisabled=val_field(browserSet_hxobj,_id_tabToLinksDisabled);
				bool _tabToLinksDisabled=val_bool(v_tabToLinksDisabled);
				browserSettings.tab_to_links_disabled=_tabToLinksDisabled;

				value v_textAreaResizeDisabled=val_field(browserSet_hxobj,_id_textAreaResizeDisabled);
				bool _textAreaResizeDisabled=val_bool(v_textAreaResizeDisabled);
				browserSettings.text_area_resize_disabled=_textAreaResizeDisabled;

				value v_userStyleSheetEnabled=val_field(browserSet_hxobj,_id_userStyleSheetEnabled);
				bool _userStyleSheetEnabled=val_bool(v_userStyleSheetEnabled);
				browserSettings.user_style_sheet_enabled=_userStyleSheetEnabled;

				value v_userStyleSheetLocation=val_field(browserSet_hxobj,_id_userStyleSheetLocation);
				if(val_is_null(v_userStyleSheetLocation)==false)
				{
					const char* _userStyleSheetLocation=val_string(v_userStyleSheetLocation);
					CefString(&browserSettings.user_style_sheet_location)=_userStyleSheetLocation;
				}
				
				value v_webGLDisabled=val_field(browserSet_hxobj,_id_webGLDisabled);
				bool _webGLDisabled=val_bool(v_webGLDisabled);
				browserSettings.webgl_disabled=_webGLDisabled;

				value v_webSecurityDisabled=val_field(browserSet_hxobj,_id_webSecurityDisabled);
				bool _webSecurityDisabled=val_bool(v_webSecurityDisabled);
				browserSettings.web_security_disabled=_webSecurityDisabled;

				value v_xssAuditorEnabled=val_field(browserSet_hxobj,_id_xssAuditorEnabled);
				bool _xssAuditorEnabled=val_bool(v_xssAuditorEnabled);
				browserSettings.xss_auditor_enabled=_xssAuditorEnabled;

				value v_loadDropsDisabled=val_field(browserSet_hxobj,_id_loadDropsDisabled);
				bool _loadDropsDisabled=val_bool(v_loadDropsDisabled);
				browserSettings.load_drops_disabled=_loadDropsDisabled;

				value v_historyDisabled=val_field(browserSet_hxobj,_id_historyDisabled);
				bool _historyDisabled=val_bool(v_historyDisabled);
				browserSettings.history_disabled=_historyDisabled;

				value v_animationFrameRate=val_field(browserSet_hxobj,_id_animationFrameRate);
				int _animationFrameRate=val_int(v_animationFrameRate);
				browserSettings.animation_frame_rate=_animationFrameRate;

				value v_fullscreenEnabled=val_field(browserSet_hxobj,_id_fullscreenEnabled);
				bool _fullscreenEnabled=val_bool(v_fullscreenEnabled);
				browserSettings.fullscreen_enabled=_fullscreenEnabled;

				value v_standardFontFamily=val_field(browserSet_hxobj,_id_standardFontFamily);
				if(val_is_null(v_standardFontFamily)==false)
				{
					const char* _standardFontFamily=val_string(v_standardFontFamily);
					wchar_t* w_str=c2w(_standardFontFamily);
					CefString(&browserSettings.standard_font_family)=w_str;
					delete []w_str; //not sure
					w_str=NULL;
				}
				//
				
				value v_fixedFontFamily=val_field(browserSet_hxobj,_id_fixedFontFamily);
				if(val_is_null(v_fixedFontFamily)==false)
				{
					const char* _fixedFontFamily=val_string(v_fixedFontFamily);
					wchar_t* w_str=c2w(_fixedFontFamily);
					CefString(&browserSettings.fixed_font_family)=w_str;
					delete []w_str; //not sure
					w_str=NULL;
				}

				value v_sansSerifFontFamily=val_field(browserSet_hxobj,_id_sansSerifFontFamily);
				if(val_is_null(v_sansSerifFontFamily)==false)
				{
					const char* _sansSerifFontFamily=val_string(v_sansSerifFontFamily);
					wchar_t* w_str=c2w(_sansSerifFontFamily);
					CefString(&browserSettings.sans_serif_font_family)=w_str;
					delete []w_str; //not sure
					w_str=NULL;
				}

				value v_serifFontFamily=val_field(browserSet_hxobj,_id_serifFontFamily);
				if(val_is_null(v_serifFontFamily)==false)
				{
					const char* _serifFontFamily=val_string(v_serifFontFamily);
					wchar_t* w_str=c2w(_serifFontFamily);
					CefString(&browserSettings.serif_font_family)=w_str;
					delete []w_str; //not sure
					w_str=NULL;
				}


				value v_cursiveFontFamily=val_field(browserSet_hxobj,_id_cursiveFontFamily);
				if(val_is_null(v_cursiveFontFamily)==false)
				{
					const char* _cursiveFontFamily=val_string(v_cursiveFontFamily);
					wchar_t* w_str=c2w(_cursiveFontFamily);
					CefString(&browserSettings.cursive_font_family)=w_str;
					delete []w_str; //not sure
					w_str=NULL;
				}

				value v_fantasyFontFamily=val_field(browserSet_hxobj,_id_fantasyFontFamily);
				if(val_is_null(v_fantasyFontFamily)==false)
				{
					const char* _fantasyFontFamily=val_string(v_fantasyFontFamily);
					wchar_t* w_str=c2w(_fantasyFontFamily);
					CefString(&browserSettings.fantasy_font_family)=w_str;
					delete []w_str; //not sure
					w_str=NULL;
				}

				value v_defaultFixedFontSize=val_field(browserSet_hxobj,_id_defaultFixedFontSize);
				int _defaultFixedFontSize=val_int(v_defaultFixedFontSize);
				if(_defaultFixedFontSize!=-1)browserSettings.default_fixed_font_size=_defaultFixedFontSize;

				value v_defaultFontSize=val_field(browserSet_hxobj,_id_defaultFontSize);
				int _defaultFontSize=val_int(v_defaultFontSize);
				if(_defaultFontSize!=-1)browserSettings.default_font_size=_defaultFontSize;

				value v_minimumFontSize=val_field(browserSet_hxobj,_id_minimumFontSize);
				int _minimumFontSize=val_int(v_minimumFontSize);
				if(_minimumFontSize!=-1)browserSettings.minimum_font_size=_minimumFontSize;

				value v_minimumLogicalFontSize=val_field(browserSet_hxobj,_id_minimumLogicalFontSize);
				int _minimumLogicalFontSize=val_int(v_minimumLogicalFontSize);
				if(_minimumLogicalFontSize!=-1)browserSettings.minimum_logical_font_size=_minimumLogicalFontSize;
			}
			RECT rect;
			GetClientRect(w->hwnd, &rect);
			w->g_handler=new ClientHandler();
			w->g_handler->SetMainWindow(w);
			w->g_handler->SetMainHwnd(w->hwnd);
			CefWindowInfo info;
			int wininfo_flag=val_int(val_field(w->window_hxobj->get(),_id_windowFlag));
			if(wininfo_flag&WBF_POPUP)
			{
				info.SetAsPopup(w->hwnd,"");
			}else
			{
				info.SetAsChild(w->hwnd, rect);
			}
			if(wininfo_flag&WBF_RENDERING)
			{
				info.SetAsOffScreen(w->hwnd);
			}
			if(wininfo_flag&WF_TRANSPARENT)
			{
				info.SetTransparentPainting(true);
			}
			value url_obj=val_field(w->window_hxobj->get(),_id_url);
			wchar_t* path_str=c2w(val_string(url_obj));
			CefBrowser::CreateBrowser(info,
           static_cast<CefRefPtr<CefClient> >(w->g_handler),
		   path_str, browserSettings);
		   delete []path_str; //not sure
		   path_str=NULL;
		   value window_obj=alloc_abstract(gObjectKind,w);
		   //val_gc(window_obj,release_window);
		   val_ocall1(w->window_hxobj->get(),_id_on_create,window_obj);
			return 0;
		}
		case WM_SIZE:
		{
			value size_result=val_ocall0(w->window_hxobj->get(),_id_on_size);
			if(!val_bool(size_result))return 0;
			if (w->g_handler) {
				// Resize the browser window and address bar to match the new frame
				// window size
				RECT rect;
				GetClientRect(w->hwnd, &rect);

				HDWP hdwp = BeginDeferWindowPos(1);
				hdwp = DeferWindowPos(hdwp, w->g_handler->GetBrowserHwnd(), NULL,
					rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
					SWP_NOZORDER);
				EndDeferWindowPos(hdwp);
			}
		}
		break;
	case WM_SETFOCUS:
      if (w->g_handler && w->g_handler->GetBrowserHwnd()) {
        // Pass focus to the browser window
        PostMessage(w->g_handler->GetBrowserHwnd(), WM_SETFOCUS, wparam, NULL);
      }
      return 0;
	case WM_ERASEBKGND:
		if (w->g_handler) {
			// Dont erase the background if the browser window has been loaded
			// (this avoids flashing)
			return 0;
		}
		break;
	case WM_CLOSE:
	{
		value close_result=val_ocall0(w->window_hxobj->get(),_id_on_close);
		if(!val_bool(close_result))return 0;
		if (w->g_handler) {
			CefRefPtr<CefBrowser> browser = w->g_handler->GetBrowser();
			if (browser.get()) {
				// Let the browser window know we are about to destroy it.
				browser->ParentWindowWillClose();
			}
		}
	}
		break;
	case WM_GETMINMAXINFO:
		{
			MINMAXINFO *lpMMI = (MINMAXINFO*)lparam;
			DWORD style = GetWindowLong(w->hwnd,GWL_STYLE);
			bool isCaption=IS_SET(style,(WS_CAPTION|WS_BORDER));
			if(!isCaption){
				HMONITOR hMonitor=MonitorFromWindow(w->hwnd,MONITOR_DEFAULTTONEAREST);
				if(hMonitor!=NULL){
					MONITORINFO monitorInfo = {sizeof(MONITORINFO)}; 
					::GetMonitorInfo(hMonitor, &monitorInfo);
					RECT rcWorkArea = monitorInfo.rcWork;
					RECT rcMonitorArea = monitorInfo.rcMonitor;
					lpMMI->ptMaxPosition.x =abs(rcWorkArea.left - rcMonitorArea.left);//-1;
					lpMMI->ptMaxPosition.y =abs(rcWorkArea.top - rcMonitorArea.top);//-1;
					lpMMI->ptMaxSize.x = abs(rcWorkArea.right - rcWorkArea.left);//+2;
					lpMMI->ptMaxSize.y = abs(rcWorkArea.bottom - rcWorkArea.top);//+2;
					bool isNoSizer=IS_SET(style,WS_THICKFRAME);
					if(isNoSizer){
						lpMMI->ptMaxPosition.x=lpMMI->ptMaxPosition.x-GetSystemMetrics(SM_CXSIZEFRAME);
						lpMMI->ptMaxPosition.y=lpMMI->ptMaxPosition.y-GetSystemMetrics(SM_CYSIZEFRAME);
						lpMMI->ptMaxSize.x=lpMMI->ptMaxSize.x+GetSystemMetrics(SM_CYSIZEFRAME)*2;
						lpMMI->ptMaxSize.y=lpMMI->ptMaxSize.y+GetSystemMetrics(SM_CYSIZEFRAME)*2;
					}
					//lpMMI->ptMaxTrackSize.x = abs(rcWorkArea.right-rcWorkArea.left) + 2; 
					//lpMMI->ptMaxTrackSize.y = abs(rcWorkArea.bottom-rcWorkArea.top) + 2; 

					/*RECT rcWork = mi.rcWork, rcMonitor = mi.rcMonitor;
					lpMMI->ptMaxPosition.x = abs(rcWork.left - rcMonitor.left) - 1; 
					lpMMI->ptMaxPosition.y = abs(rcWork.top - rcMonitor.top) - 1; 
					lpMMI->ptMaxSize.x = abs(rcWork.Width()) + 2; 
					lpMMI->ptMaxSize.y = abs(rcWork.Height()) + 2; 
					lpMMI->ptMaxTrackSize.x = abs(rcWork.Width()) + 2; 
					lpMMI->ptMaxTrackSize.y = abs(rcWork.Height()) + 2; */
				}
			}
			
			/*
			DWORD style = GetWindowLong(w->hwnd,GWL_STYLE);
			bool isCaption=IS_SET(style,(WS_CAPTION|WS_BORDER));
			if(!isCaption){
				//lpMMI->ptMaxPosition.x=-GetSystemMetrics(SM_CXSIZEFRAME);
				//lpMMI->ptMaxPosition.y=-GetSystemMetrics(SM_CYSIZEFRAME);
				bool isNoSizer=IS_SET(style,WS_THICKFRAME);
				if(isNoSizer){
					lpMMI->ptMaxSize.x=lpMMI->ptMaxSize.x;//+GetSystemMetrics(SM_CXSIZEFRAME);
					lpMMI->ptMaxSize.y=lpMMI->ptMaxSize.y+GetSystemMetrics(SM_CYCAPTION)-GetSystemMetrics(SM_CYSIZEFRAME)*2;
					//lpMMI->ptMaxSize.x=GetSystemMetrics(SM_CXFULLSCREEN)+GetSystemMetrics(SM_CXSIZEFRAME)*2;
					//lpMMI->ptMaxSize.y=GetSystemMetrics(SM_CYFULLSCREEN)+GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYSIZEFRAME)*2;
				}else{
					lpMMI->ptMaxSize.x=lpMMI->ptMaxSize.x;//+GetSystemMetrics(SM_CXFIXEDFRAME);
					lpMMI->ptMaxSize.y=lpMMI->ptMaxSize.y+GetSystemMetrics(SM_CYCAPTION)-GetSystemMetrics(SM_CYFIXEDFRAME)*2;
					//lpMMI->ptMaxSize.x=GetSystemMetrics(SM_CXFULLSCREEN)+GetSystemMetrics(SM_CXFIXEDFRAME)*2;
					//lpMMI->ptMaxSize.y=GetSystemMetrics(SM_CYFULLSCREEN)+GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYFIXEDFRAME )*2;
				}
			}
			*/
			//得到最小尺寸
			value minSize=val_field(w->window_hxobj->get(),_id_minsize);
			int min_width=val_int(val_field(minSize,_id_width));
			int min_height=val_int(val_field(minSize,_id_height));
			lpMMI->ptMinTrackSize.x=min_width;
			lpMMI->ptMinTrackSize.y=min_height;
		}
		break;
	case WM_SYSCOMMAND: 
		{
			switch(wparam & 0xFFF0) {
				case SC_MAXIMIZE:
				{
					value cmd_result=val_ocall0(w->window_hxobj->get(),_id_on_maximize);
					if(!val_bool(cmd_result))return 0;
				}
					break;
				case SC_MINIMIZE:
				{
					value cmd_result=val_ocall0(w->window_hxobj->get(),_id_on_minimize);
					if(!val_bool(cmd_result))return 0;
				}
					break;
				case SC_RESTORE:
				{
					value cmd_result=val_ocall0(w->window_hxobj->get(),_id_on_restore);
					if(!val_bool(cmd_result))return 0;
				}
					break;
				case SC_CLOSE:
				{
					value cmd_result=val_ocall0(w->window_hxobj->get(),_id_on_close);
					if(!val_bool(cmd_result))return 0;
				}
					break;
			}
		}
		break;
	case WM_NCDESTROY:
		{
			LRESULT result = DefWindowProc( w->hwnd, msg, wparam, lparam );
			releaseHxWindow(w);
			//printf("wm_ncdestroy");
			return result;
		}
	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(w->hwnd, &ps);
		EndPaint(w->hwnd, &ps);
		return 0;
	}
	return DefWindowProc( w->hwnd, msg, wparam, lparam );
}

static LRESULT CALLBACK WndProcStub( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	LONG_PTR plptrWin= GetWindowLongPtr(hwnd,GWLP_USERDATA);
	window *w=reinterpret_cast<window*>(plptrWin);
	if( w != NULL&&w->hwnd==hwnd )	return WndProc( w, msg, wparam, lparam );
	return DefWindowProc( hwnd, msg, wparam, lparam );
}

static LRESULT CALLBACK InitialWndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	 if (msg == WM_NCCREATE) {
        LPCREATESTRUCT create_struct = reinterpret_cast<LPCREATESTRUCT>(lparam);
        void * lpCreateParam = create_struct->lpCreateParams;
        window * this_window = reinterpret_cast<window *>(lpCreateParam);
		//assert(this_window->hwnd == 0); // this should be the first (and only) time
                                         // WM_NCCREATE is processed
        this_window->hwnd = hwnd;
		this_window->hdc = GetWindowDC(hwnd);
        SetWindowLongPtr(hwnd, 
                         GWLP_USERDATA, 
                         reinterpret_cast<LONG_PTR>(this_window));
        SetWindowLongPtr(hwnd, 
                         GWLP_WNDPROC,
                         reinterpret_cast<LONG_PTR>(&WndProcStub));
        return WndProc(this_window, msg, wparam, lparam );
      }                               
      return DefWindowProc(hwnd, msg, wparam, lparam);
}

static void fullscreen(window *w, bool enter) {
	RECT rc;
	if (enter) {
		w->fullscreen_org_wi.cbSize = sizeof(WINDOWINFO);
		GetWindowInfo(w->hwnd,&w->fullscreen_org_wi);
		SetWindowLong(w->hwnd, GWL_STYLE, WS_VISIBLE | WS_SYSMENU );
		GetWindowRect(GetDesktopWindow(), &rc);
		SetWindowPos( w->hwnd, HWND_TOPMOST, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 0);
	}
	else {
		RECT *rc = &w->fullscreen_org_wi.rcWindow;
		SetWindowLong( w->hwnd, GWL_STYLE, w->fullscreen_org_wi.dwStyle );
		SetWindowLong( w->hwnd, GWL_EXSTYLE, w->fullscreen_org_wi.dwExStyle );
		SetWindowPos
			( w->hwnd, HWND_NOTOPMOST
			, rc->left, rc->top
			, rc->right-rc->left, rc->bottom-rc->top
			, 0 );
	}
}