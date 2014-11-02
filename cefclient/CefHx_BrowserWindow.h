#ifndef CEFHX_BROWSERWINDOW_H_
#define CEFHX_BROWSERWINDOW_H_
#pragma once
#include <string>
#include <windows.h>
#include <hx/CFFI.h>
#include "include/cef_app.h"
#include "include/cef_base.h"
#include "include/cef_client.h"

#include "cefclient/client_handler.h"


//enum WindowEvent {
//	WE_DESTROY,
//	WE_CLOSE,
//	WE_MINIMIZE,
//	WE_MAXIMIZE,
//	WE_RIGHTCLICK,
//	WE_RESTORE
//};

enum WindowProperty {
	WP_RESIZABLE,
	WP_MAXIMIZE_ICON,
	WP_MINIMIZE_ICON,
	WP_WIDTH,
	WP_HEIGHT,
	WP_LEFT,
	WP_TOP,
	WP_FULLSCREEN,
	WP_TRANSPARENT,
	WP_DROPTARGET,
	WP_PLAIN,
	WP_MINIMIZED,
	WP_MAXIMIZED,
	WP_CAPTION,
	WP_ENABLED
};

enum WindowFlags {
	WF_FULLSCREEN		= 1,
	WF_TRANSPARENT		= 1 << 1,
	
	WF_PLAIN			= 1 << 2,
	WF_ALWAYS_ONTOP		= 1 << 3,
	WF_NO_TASKBAR		= 1 << 4,

	WF_DROPTARGET		= 1 << 5,

	WBF_RENDERING       = 1 << 6,
	WBF_CHILD			= 1 << 7,
	WBF_POPUP			= 1 << 8,
	WF_NO_CAPTION		= 1 << 9,
};

class window {
public:
	window::window():hwnd(NULL),hdc(NULL),hrgn(NULL),window_hxobj(NULL){} //,isMessageHook(false)
	HWND hwnd;
	HDC hdc;
	enum WindowFlags flags;
	// full screen data:
	WINDOWINFO fullscreen_org_wi;
	CefRefPtr<ClientHandler> g_handler;
	AutoGCRoot* window_hxobj;
	HRGN hrgn;
	//bool isMessageHook;
};


// MAIN functions
int system_init();
void system_cleanup();


// WINDOW functions
window *system_window_create(int width, int height, enum WindowFlags flags,value window_hxobj);
void system_window_show( window *w, int show,bool front,bool isAsync,bool isActive  );
void system_window_close(window* w);

void system_window_destroy( window *w );

void *system_window_get_handle( window *w );

void system_window_set_title( window *w,const wchar_t * title );
void system_window_drag( window *w );
void system_window_resize( window *w, int o );
void system_window_round(window* w,int x,int y,int width,int height,int szWidth,int szHeight);
void system_window_onresload(window* w,const char* mime,const char* url,const char* redirectUrl,int status,unsigned char * bytes,bool isCancel,int bytesLen);
void system_window_set_prop( window *w, enum WindowProperty prop, int value );
int system_window_get_prop( window *w, enum WindowProperty prop );
void system_quit();
int system_getmetrics(int inx);
unsigned long  system_process_findid(const wchar_t *processName); ////根据进程名称查找进程编号
bool system_process_close(const wchar_t *processName);
bool system_process_closebyid(unsigned long id);
bool  system_registeinstance(const wchar_t *instanceName);
static HANDLE  hInstanceMutex=0;
#endif