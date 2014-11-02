#ifndef IPHONE
#define IMPLEMENT_API
#endif

//#if defined(HX_WINDOWS) || defined(HX_MACOS) || defined(HX_LINUX)
//// Include neko glue....
//#define NEKO_COMPATIBLE
//#endif

#include <hx/CFFI.h>
#include "cefclient/ExternalInterface.h"
#include <windows.h>
#include <commdlg.h>
#include <shellapi.h>
#include <direct.h>
#include <sstream>
#include <string>
#include "include/cef_app.h"
#include "include/cef_base.h"
#include "include/cef_browser.h"
#include "include/cef_client.h"
#include "include/cef_command_line.h"
#include "include/cef_frame.h"
#include "include/cef_runnable.h"
#include "include/cef_web_plugin.h"
#include "include/cef_web_urlrequest.h"
#include "cefclient/CefHx_BrowserWindow.h"
#include "cefclient/plugin_client.h"
#if defined(OS_WIN)
 //Add Common Controls to the application manifest because it's required to
// support the default tooltip implementation.
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")  // NOLINT(whitespace/line_length)
#endif

int sgIDsInit=false;
vkind gObjectKind=0;
value gHaxeStaticCefApp=0;
//int Global_Atom_KeyId=-1;
int _id_url=0;
int _id_minsize=0;
int _id_windowFlag;
int _id_cef_browserset=0;
int _id_onjscall=0;
int _id_on_create;
int _id_on_close;
int _id_on_destroy;
int _id_on_size;
int _id_on_maximize;
int _id_on_minimize;
int _id_on_restore;

int _id_on_sysmsg;
//int _id_on_messagehook;
//int _id_on_rightclick;

int _id_width;
int _id_height;
int _id_x;
int _id_y;
int _id_frameUrl;
int _id_imageUrl;
int _id_linkUrl;
int _id_selectionText;
int _id_misspelledWord;
int _id_securityInfo;
int _id_typeFlags;
int _id_editFlags;

int _id_onBrowserEvent;
int _id_on_io;

int _id_accelerated2DCanvasDisabled;
int _id_acceleratedCompositingEnabled;
int _id_acceleratedPaintingDisabled;
int _id_acceleratedLayersDisabled;
int _id_acceleratedFiltersDisabled;
int _id_acceleratedPluginsDisabled;
int _id_acceleratedVideoDisabled;
int _id_applicationCacheDisabled;
int _id_authorAndUserStylesDisabled;
int _id_caretBrowsingEnabled;
int _id_databasesDisabled;
int _id_defaultEncoding;
int _id_developerToolsDisabled;
int _id_domPasteDisabled;
int _id_dragDropDisabled;
int _id_encodingDetectorEnabled;
int _id_fileAccessFromFileUrlsAllowed;
int _id_universalAccessFromFileUrlsAllowed;
int _id_hyperlinkAuditingDisabled;
int _id_imageLoadDisabled;
int _id_javaDisabled;
int _id_javaScriptAccessClipboardDisallowed;
int _id_javaScriptCloseWindowsDisallowed;
int _id_javaScriptDisabled;
int _id_javaScriptOpenWindowsDisallowed;
int _id_localStorageDisabled;
int _id_pageCacheDisabled;
int _id_pluginsDisabled;
int _id_remoteFontsDisabled;
int _id_shrinkStandaloneImagesToFit;
int _id_siteSpecificQuirksDisabled;
int _id_tabToLinksDisabled;
int _id_textAreaResizeDisabled;
int _id_userStyleSheetEnabled;
int _id_userStyleSheetLocation;
int _id_webGLDisabled;
int _id_webSecurityDisabled;
int _id_xssAuditorEnabled;
int _id_loadDropsDisabled;
int _id_historyDisabled;
int _id_animationFrameRate;
int _id_fullscreenEnabled;
int _id_standardFontFamily;
int _id_fixedFontFamily;
int _id_sansSerifFontFamily;
int _id_serifFontFamily;
int _id_cursiveFontFamily;
int _id_fantasyFontFamily;
int _id_defaultFixedFontSize;
int _id_defaultFontSize;
int _id_minimumFontSize;
int _id_minimumLogicalFontSize;

char ** pFilterResPaths;//(char*)malloc(sizeof(char)*);
int filterPathsLen;

/*
int loadHaxePlug(const char* plugPath,const char* plugArgs){
	if(gHaxeStaticCefApp!=NULL){
		value pv=val_ocall2(gHaxeStaticCefApp,val_id("loadPlug"),alloc_string(plugPath),alloc_string(plugArgs));
		return val_int(pv);
	}
	return -1;
}
*/
bool closeProcessById(int plugid){
	if(gHaxeStaticCefApp!=NULL){
		value pv=val_ocall1(gHaxeStaticCefApp,val_id("closeProcessById"),alloc_int(plugid));
		return val_bool(pv);
	}
	return false;
}
wchar_t* c2w(const char *str)
{
	int length = strlen(str)+1;
	wchar_t *t = (wchar_t*)malloc(sizeof(wchar_t)*length);
	memset(t,0,length*sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP,0,str,strlen(str),t,length);
	return t;
}

 static void RunTask(window* pw,int delay) {
	 if(pw&&pw->hwnd){
		 ::PostMessage(pw->hwnd,0x2325,0,delay);
	 }
}

////将 单字节char* 转换为 宽字节 wchar*
//inline wchar_t* AnsiToUnicode( const char* szStr )
//{
//int nLen = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0 );
//if (nLen == 0)
//{
//   return NULL;
//}
//wchar_t* pResult = new wchar_t[nLen];
//MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen );
//return pResult;
//}
//———————————————————————————-
// 将 宽字节wchar_t* 转换 单字节char*
const char* w2c(const wchar_t* szStr )
{
	int nLen = WideCharToMultiByte( CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL );
	if (nLen == 0)
	{
	   return NULL;
	}
	char* pResult = new char[nLen];
	WideCharToMultiByte( CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL );
	return pResult;
}

//WString val2stdwstr(value inVal)
//{
//   const wchar_t *val = val_wstring(inVal);
//   int len=0;
//   while(val[len]) len++;
//   return WString(val,len);
//}

value executeJsCallBack(void* pWindow,const char * funcName,value args)
{
	window* p_w=(window*)pWindow;
	if(p_w!=NULL){
		value retHxVal=val_ocall2(p_w->window_hxobj->get(),_id_onjscall,alloc_string(funcName),args);
		return retHxVal;
	}
	return alloc_null();
}

value onBrowserEvent(void* pWindow,const char* eventName, value args)
{
	window* p_w=(window*)pWindow;
	if(p_w){
		value result=val_ocall2(p_w->window_hxobj->get(),_id_onBrowserEvent,alloc_string(eventName),args);
		return result;
	}
	return alloc_null();
	
}
bool getIsFilterPath(const char* str){
	if(pFilterResPaths==NULL)return false;
	size_t pos;
	int i=0;
	for(i=0;i<filterPathsLen;++i){
		//printf("%s:\n",(char*)pFilterResPaths[i]);
		pos=std::string(str).find((char*)pFilterResPaths[i]);
		if (pos != std::string::npos){
			return true;
		}
		
	}
	return false;
}
int OnBeforeResourceLoad(void* pWindow,const char * url,int type){
	window* p_w=(window*)pWindow;
	if(p_w&&p_w->hwnd){
		if(type==0){
			return ::SendMessage(p_w->hwnd,0x2326,0,(LPARAM)url);
		}else{
			printf("extern OnBeforeResourceLoad post send...:%s\n",url);
			return ::PostMessage(p_w->hwnd,0x2326,1,(LPARAM)url);
		}
	}
	return 0;
}
void releaseHxWindow(void* pWindow){
	 window* w=(window*)pWindow;
	 int result=val_int(val_ocall0(w->window_hxobj->get(),_id_on_destroy));
	 if(w->hdc) ReleaseDC(w->hwnd,w->hdc);
	 w->g_handler=NULL;
	 if(w->hrgn)DeleteObject(w->hrgn);
	 w->hrgn=NULL;
	 w->hdc=NULL;
	 w->hwnd=NULL;
	 delete w->window_hxobj;
	 w->window_hxobj=NULL;
	 SetWindowLongPtr(w->hwnd,GWLP_USERDATA,0);
	 SetWindowLongPtr(w->hwnd, GWLP_WNDPROC,0);
	 delete w;
	 w=NULL;
	 if(result==0)system_quit();
	 //printf("-----haxe window release\n");
}
namespace CefHx
{
extern "C" void InitIDs()
{
   sgIDsInit = true;
   _id_url=val_id("_url");
   _id_minsize=val_id("minSize");
   _id_windowFlag=val_id("_windowFlag");
   _id_cef_browserset=val_id("cefBrowserSet");

   _id_width=val_id("width");
   _id_height=val_id("height");
   _id_x=val_id("x");
   _id_y=val_id("y");

	_id_frameUrl=val_id("frameUrl");
	_id_imageUrl=val_id("imageUrl");
	_id_linkUrl=val_id("linkUrl");
	_id_selectionText=val_id("selectionText");
	_id_misspelledWord=val_id("misspelledWord");
	_id_securityInfo=val_id("securityInfo");
	_id_typeFlags=val_id("typeFlags");
	_id_editFlags=val_id("editFlags");

   _id_onBrowserEvent=val_id("onBrowserEvent");

   _id_on_create=val_id("on_create");
   _id_on_close=val_id("on_close");
   _id_onjscall=val_id("on_jscall");
   _id_on_destroy=val_id("on_destroy");
   _id_on_size=val_id("on_size");
   _id_on_maximize=val_id("on_maximize");
   _id_on_minimize=val_id("on_minimize");
   _id_on_restore=val_id("on_restore");

   _id_on_sysmsg=val_id("onSystemMessage");
  // _id_on_rightclick=val_id("on_rightclick");
   //_id_on_resload=val_id("onResLoad");

   _id_accelerated2DCanvasDisabled=val_id("accelerated2DCanvasDisabled");
   _id_acceleratedCompositingEnabled=val_id("acceleratedCompositingEnabled");
   _id_acceleratedPaintingDisabled=val_id("acceleratedPaintingDisabled");
	_id_acceleratedLayersDisabled=val_id("acceleratedLayersDisabled");
	 _id_acceleratedFiltersDisabled=val_id("acceleratedFiltersDisabled");
	 _id_acceleratedPluginsDisabled=val_id("acceleratedPluginsDisabled");
	 _id_acceleratedVideoDisabled=val_id("acceleratedVideoDisabled");
	 _id_applicationCacheDisabled=val_id("applicationCacheDisabled");
	 _id_authorAndUserStylesDisabled=val_id("authorAndUserStylesDisabled");
	 _id_caretBrowsingEnabled=val_id("caretBrowsingEnabled");
	 _id_databasesDisabled=val_id("databasesDisabled");
	 _id_defaultEncoding=val_id("defaultEncoding");
	 _id_developerToolsDisabled=val_id("developerToolsDisabled");
	 _id_domPasteDisabled=val_id("domPasteDisabled");
	 _id_dragDropDisabled=val_id("dragDropDisabled");
	 _id_encodingDetectorEnabled=val_id("encodingDetectorEnabled");
	 _id_fileAccessFromFileUrlsAllowed=val_id("fileAccessFromFileUrlsAllowed");
	 _id_universalAccessFromFileUrlsAllowed=val_id("universalAccessFromFileUrlsAllowed");
	 _id_hyperlinkAuditingDisabled=val_id("hyperlinkAuditingDisabled");
	 _id_imageLoadDisabled=val_id("imageLoadDisabled");
	 _id_javaDisabled=val_id("javaDisabled");
	 _id_javaScriptAccessClipboardDisallowed=val_id("javaScriptAccessClipboardDisallowed");
	 _id_javaScriptCloseWindowsDisallowed=val_id("javaScriptCloseWindowsDisallowed");
	 _id_javaScriptDisabled=val_id("javaScriptDisabled");
	 _id_javaScriptOpenWindowsDisallowed=val_id("javaScriptOpenWindowsDisallowed");
	 _id_localStorageDisabled=val_id("localStorageDisabled");
	 _id_pageCacheDisabled=val_id("pageCacheDisabled");
	 _id_pluginsDisabled=val_id("pluginsDisabled");
	 _id_remoteFontsDisabled=val_id("remoteFontsDisabled");
	 _id_shrinkStandaloneImagesToFit=val_id("shrinkStandaloneImagesToFit");
	 _id_siteSpecificQuirksDisabled=val_id("siteSpecificQuirksDisabled");
	 _id_tabToLinksDisabled=val_id("tabToLinksDisabled");
	 _id_textAreaResizeDisabled=val_id("textAreaResizeDisabled");
	 _id_userStyleSheetEnabled=val_id("userStyleSheetEnabled");
	 _id_userStyleSheetLocation=val_id("userStyleSheetLocation");
	 _id_webGLDisabled=val_id("webGLDisabled");
	 _id_webSecurityDisabled=val_id("webSecurityDisabled");
	 _id_xssAuditorEnabled=val_id("xssAuditorEnabled");
	 _id_loadDropsDisabled=val_id("loadDropsDisabled");
	 _id_historyDisabled=val_id("historyDisabled");
	 _id_animationFrameRate=val_id("animationFrameRate");
	 _id_fullscreenEnabled=val_id("fullscreenEnabled");
	 _id_standardFontFamily=val_id("standardFontFamily");
	 _id_fixedFontFamily=val_id("fixedFontFamily");
	 _id_sansSerifFontFamily=val_id("sansSerifFontFamily");
	 _id_serifFontFamily=val_id("serifFontFamily");
	 _id_cursiveFontFamily=val_id("cursiveFontFamily");
	 _id_fantasyFontFamily=val_id("fantasyFontFamily");
	 _id_defaultFixedFontSize=val_id("defaultFixedFontSize");
	 _id_defaultFontSize=val_id("defaultFontSize");
	 _id_minimumFontSize=val_id("minimumFontSize");
	_id_minimumLogicalFontSize=val_id("minimumLogicalFontSize");

	pFilterResPaths=NULL;
	filterPathsLen=0;
   gObjectKind = alloc_kind();
}

DEFINE_ENTRY_POINT(InitIDs)
}
using namespace CefHx;

/*----------------------------------------入口函数----------------------------------------------------*/
AutoGCRoot *sOnCreateCallback = 0;

value cef_app_main(value callBack,value cefSetting)
{
   if (!sgIDsInit)
      InitIDs();
	
     system_init();
	 CefSettings appSettings;
	 CefRefPtr<CefApp> cefApplication;
	 if(!val_is_null(cefSetting))
	 {
		int cachePath_id=val_id("cachePath");
		int userAgent_id=val_id("userAgent");
		int locale_id=val_id("locale");
		int extraPluginPaths_id=val_id("extraPluginPaths");
		int logFile_id=val_id("logFile");
		int logSeverity_id=val_id("logSeverity");
		int releaseDcheckEnabled_id=val_id("releaseDcheckEnabled");
		int graphicsImplementation_id=val_id("graphicsImplementation");
		int localStorageQuota_id=val_id("localStorageQuota");
		int sessionStorageQuota_id=val_id("sessionStorageQuota");
		int javascriptFlags_id=val_id("javascriptFlags");
		int resourcesDirPath_id=val_id("resourcesDirPath");
		int localesDirPath_id=val_id("localesDirPath");
		int packLoadingDisabled_id=val_id("packLoadingDisabled");
		int uncaughtExceptionStackSize_id=val_id("uncaughtExceptionStackSize");
		int contextSafetyImplementation_id=val_id("contextSafetyImplementation");
		
		appSettings.multi_threaded_message_loop=false;
		#if defined(OS_WIN)
		appSettings.auto_detect_proxy_settings_enabled=false;
		#endif
		const char* cachePath_str=val_string(val_field(cefSetting,cachePath_id));
		if(cachePath_str!=NULL&&cachePath_str!=""){
			wchar_t* w_str=c2w(cachePath_str);
			CefString(&appSettings.cache_path)=w_str;
			delete []w_str; //not sure
			w_str=NULL;
		}

		const char* userAgent_str=val_string(val_field(cefSetting,userAgent_id));
		if(userAgent_str!=NULL&&userAgent_str!="")CefString(&appSettings.user_agent)=userAgent_str;

		const char *locale_str=val_string(val_field(cefSetting,locale_id));
		if(locale_str!=NULL&&locale_str!="")CefString(&appSettings.locale)=locale_str;

		const char* logFile_str=val_string(val_field(cefSetting,logFile_id));
		if(logFile_str!=NULL&&logFile_str!="")CefString(&appSettings.log_file)=logFile_str;
		
		int logSeverity=val_int(val_field(cefSetting,logSeverity_id));
		appSettings.log_severity=(cef_log_severity_t)logSeverity;

		bool releaseDcheckEnabled=val_bool(val_field(cefSetting,releaseDcheckEnabled_id));
		appSettings.release_dcheck_enabled=releaseDcheckEnabled;
		
		int graphicsImplementation=val_int(val_field(cefSetting,graphicsImplementation_id));
		appSettings.graphics_implementation=(cef_graphics_implementation_t)graphicsImplementation;

		int localStorageQuota=val_int(val_field(cefSetting,localStorageQuota_id));
		if(localStorageQuota!=-1)appSettings.local_storage_quota=localStorageQuota;

		int sessionStorageQuota=val_int(val_field(cefSetting,sessionStorageQuota_id));
		if(sessionStorageQuota!=-1)appSettings.session_storage_quota=sessionStorageQuota;

		const char* javascriptFlags_str=val_string(val_field(cefSetting,javascriptFlags_id));
		if(javascriptFlags_str!=NULL&&javascriptFlags_str!="")CefString(&appSettings.javascript_flags)=javascriptFlags_str;

		const char* resourcesDirPath_str=val_string(val_field(cefSetting,resourcesDirPath_id));
		if(resourcesDirPath_str!=NULL&&resourcesDirPath_str!=""){
			wchar_t* w_str=c2w(resourcesDirPath_str);
			CefString(&appSettings.resources_dir_path)=w_str;
			delete []w_str; //not sure
			w_str=NULL;
		}

		const char* localesDirPath_str=val_string(val_field(cefSetting,localesDirPath_id));
		if(localesDirPath_str!=NULL&&localesDirPath_str!=""){
			wchar_t* w_str=c2w(localesDirPath_str);
			CefString(&appSettings.locales_dir_path)=w_str;
			delete []w_str; //not sure
			w_str=NULL;
		}
		bool packLoadingDisabled=val_bool(val_field(cefSetting,packLoadingDisabled_id));
		appSettings.pack_loading_disabled=packLoadingDisabled;

		int uncaughtExceptionStackSize=val_int(val_field(cefSetting,uncaughtExceptionStackSize_id));
		appSettings.uncaught_exception_stack_size=uncaughtExceptionStackSize;
		
		int contextSafetyImplementation=val_int(val_field(cefSetting,contextSafetyImplementation_id));
		appSettings.context_safety_implementation=contextSafetyImplementation;
		
		value extraPluginPaths_value=val_field(cefSetting,extraPluginPaths_id);
		if(val_is_null(extraPluginPaths_value)!=true)
		{
			
			int arr_len=val_array_size(extraPluginPaths_value);
			if(arr_len>0)
			{
				appSettings.extra_plugin_paths = cef_string_list_alloc();
				for(int i=0;i<arr_len;++i)
				{
					//gc_safe_point();
					value v_path=val_array_i(extraPluginPaths_value,i);
					wchar_t *p=c2w(val_string(v_path));
					CefString pathStr;
					pathStr.FromWString(p);
					cef_string_list_append(appSettings.extra_plugin_paths,pathStr.GetStruct());
					delete []p; //not sure
				}
			}
		}
	 }

	 CefInitialize(appSettings, cefApplication);
	 InitPluginClient();
	 sOnCreateCallback = new AutoGCRoot(callBack);
	 val_call0( sOnCreateCallback->get());
     delete sOnCreateCallback;

	 CefRunMessageLoop();
	 CefShutdown();
     return alloc_null();
}

DEFINE_PRIM(cef_app_main,2);

value cef_app_setglobalapp(value _hxStaticCefApp){
	gHaxeStaticCefApp=_hxStaticCefApp;
	return alloc_null();
}
DEFINE_PRIM(cef_app_setglobalapp,1);

value cef_unicode2ansi(value wide_str){
	const wchar_t *val = val_wstring(wide_str);
	if(val){
		const char* c_str=w2c(val);
		if(c_str){
			value char_str=alloc_string(c_str);
			delete []c_str; //not sure
			c_str=NULL;
			return char_str;
		}
	}
	return alloc_null();
}
DEFINE_PRIM(cef_unicode2ansi,1);

value cef_ansi2unicode(value _str){
	const char* c_str=val_string(_str);
	if(c_str){
		const wchar_t *w_str=c2w(c_str);
		if(w_str){
			value wide_str=alloc_wstring(w_str);
			delete []w_str; //not sure
			w_str=NULL;
			return wide_str;
		}
	}
	return alloc_null();
}
DEFINE_PRIM(cef_ansi2unicode,1);

value cef_getsystemmetrics(value inx){
	return alloc_int(system_getmetrics(val_int(inx)));
}
DEFINE_PRIM(cef_getsystemmetrics,1);

value cef_setosmodalloop(value v){
	CefSetOSModalLoop(val_bool(v));
	return alloc_null();
}
DEFINE_PRIM(cef_setosmodalloop,1);

value cef_window_create(value width, value height, value flags,value window_hxobj) {
	window *w= system_window_create(val_int(width),val_int(height),(WindowFlags)val_int(flags),window_hxobj);
	if( w == NULL )return alloc_bool(false);
	return alloc_bool(true);
}
DEFINE_PRIM(cef_window_create,5);

value cef_window_show( value w, value b,value front,value isAsync,value isActive) {
	system_window_show((window*)val_data(w),val_bool(b),val_bool(front),val_bool(isAsync),val_bool(isActive));
	return val_null;
}
DEFINE_PRIM(cef_window_show,5);

value cef_window_close(value w){
	system_window_close((window*)val_data(w));
	return alloc_null();
}
DEFINE_PRIM(cef_window_close,1);
/*
value window_set_message_hook(value w, value b){
	window* pw=(window*)val_data(w);
	pw->isMessageHook=val_bool(b);
	return alloc_null();
}
DEFINE_PRIM(window_set_message_hook,2);
*/
value cef_app_registerhotkey(value w,value hotKeyName,value fsModifiers,value vk){
	const char* hkName_str=val_string(hotKeyName);
	ATOM key_id=GlobalFindAtomA(hkName_str);
	if(key_id!=0){
		GlobalDeleteAtom(key_id);
	}
	key_id=GlobalAddAtomA(hkName_str);
	if(key_id==0)return alloc_int(-1);
	window* pw=(window*)val_data(w);
	BOOL result=RegisterHotKey(pw->hwnd,key_id-0xc000,val_int(fsModifiers),val_int(vk));
	if(result==1)return alloc_int(key_id);
	return alloc_int(-1);
}
DEFINE_PRIM(cef_app_registerhotkey,4);

value cef_app_unregisterhotkey(value w,value hotKeyName){
	const char* hkName_str=val_string(hotKeyName);
	ATOM key_id=GlobalFindAtomA(hkName_str);
	if(key_id==0){
		return alloc_bool(false);
	}
	GlobalDeleteAtom(key_id);
	window* pw=(window*)val_data(w);
	BOOL result=UnregisterHotKey(pw->hwnd,key_id-0xc000);
	return alloc_bool((result==1?true:false));
}
DEFINE_PRIM(cef_app_unregisterhotkey,2);

value cef_app_closeprocess(value inName){
	const wchar_t * process_name=val_wstring(inName);
	bool ret=system_process_close(process_name);
	return alloc_bool(ret);
}
DEFINE_PRIM(cef_app_closeprocess,1);

value cef_app_closeprocessbyid(value pid){
	int p_id=val_int(pid);
	bool ret=system_process_closebyid(p_id);
	return alloc_bool(ret);
}
DEFINE_PRIM(cef_app_closeprocessbyid,1);

value cef_app_registeinstance(value inName){
	const wchar_t * instance_name=val_wstring(inName);
	bool ret=system_registeinstance(instance_name);
	return alloc_bool(ret);
}
DEFINE_PRIM(cef_app_registeinstance,1);

value cef_app_sendmessage(value inHwnd,value msg,value msgType){
	char* szSendBuf=(char*)val_string(msg);
	COPYDATASTRUCT CopyData;
	CopyData.dwData =val_int(msgType);
    CopyData.cbData = strlen(szSendBuf);
    CopyData.lpData = szSendBuf;
	HWND p_hwnd=(HWND)val_data(inHwnd);
    SendMessage(p_hwnd, WM_COPYDATA, (WPARAM)p_hwnd , (LPARAM)&CopyData);
	return alloc_null();
}
DEFINE_PRIM(cef_app_sendmessage,3);

value cef_app_gethwndstr(value w){
	window* pw=(window*)val_data(w);
	char hwnd_var[30];
	sprintf_s(hwnd_var, "%I64d",(__int64)pw->hwnd);
	return alloc_string(hwnd_var);
}
DEFINE_PRIM(cef_app_gethwndstr,1);
//value cef_window_destroy(value w){
//	system_window_destroy((window*)val_data(w));
//	return alloc_null();
//}
//DEFINE_PRIM(cef_window_destroy,1);

value cef_app_quit(){
	system_quit();
	return val_null;
}
DEFINE_PRIM(cef_app_quit,0);

value cef_app_currentlyon(value threadId){
	return alloc_bool(CefCurrentlyOn((CefThreadId)(val_int(threadId))));
}
DEFINE_PRIM(cef_app_currentlyon,1);

value cef_app_posttask(value whandle,value delay){ //value threadId,
	int delay_=val_int(delay);
	bool isPost=false;
	window* pw=(window*)val_data(whandle);
	if(delay_>0){
		isPost=CefPostDelayedTask(TID_UI,NewCefRunnableFunction(&RunTask,pw,delay_),delay_);
	}else{
		isPost=CefPostTask(TID_UI,NewCefRunnableFunction(&RunTask,pw,0));
	}
	return alloc_bool(isPost);
	
}
DEFINE_PRIM(cef_app_posttask,2);

value cef_app_setfilterpath(value inArr){
	if(pFilterResPaths!=NULL){
		int i=0;
		for(i=0;i<filterPathsLen;++i)free(pFilterResPaths[i]);
		free(pFilterResPaths);
		pFilterResPaths=NULL;
		filterPathsLen=0;
	}
	if(!val_is_null(inArr)){
		filterPathsLen=val_array_size(inArr);
		pFilterResPaths= (char **) malloc(filterPathsLen * sizeof(char * ));
		int i=0;
		for(int i=0;i<filterPathsLen;++i){
			pFilterResPaths[i] = (char * )malloc(255 * sizeof(char));
			value v_str=val_array_i(inArr,i);
			const char* in_str=val_string(v_str);
			//char * _path= pFilterResPaths[i];
			::strcpy(pFilterResPaths[i],in_str);
			//char path_var[255];
			//sprintf_s(_path, "%s", in_str);
			//pFilterResPaths[i]=path_var;
		}
	}
	return alloc_null();
}
DEFINE_PRIM(cef_app_setfilterpath,1);

value window_set_title( value w, value t ) {
	const wchar_t * title_str=val_wstring(t);
	system_window_set_title((window*)val_data(w),title_str);
	return val_null;
}
DEFINE_PRIM(window_set_title,2);

value window_set_prop( value w, value p, value b ) {
	system_window_set_prop((window*)val_data(w),(WindowProperty)val_int(p),val_int(b));
	return val_null;
}
DEFINE_PRIM(window_set_prop,3);

value window_get_prop( value w, value p ) {
	return alloc_int(system_window_get_prop((window*)val_data(w),(WindowProperty)val_int(p)));
}
DEFINE_PRIM(window_get_prop,2);



value window_drag( value w ) {
	system_window_drag((window*)val_data(w));
	return val_null;
}
DEFINE_PRIM(window_drag,1);


value window_resize( value w, value o ) {
	system_window_resize((window*)val_data(w),val_int(o));
	return val_null;
}
DEFINE_PRIM(window_resize,2);

value window_setround(value w,value rect,value size){
	window* pw=(window*)val_data(w);
	int x=val_int(val_field(rect,_id_x));
	int y=val_int(val_field(rect,_id_y));
	int width=val_int(val_field(rect,_id_width));
	int height=val_int(val_field(rect,_id_height));
	int szW=val_int(val_field(size,_id_width));
	int szH=val_int(val_field(size,_id_height));
	system_window_round(pw,x,y,width,height,szW,szH);
	return alloc_null();
}
DEFINE_PRIM(window_setround,3);

value window_onresloaded(value w,value v){
	window* pw=(window*)val_data(w);
	if(val_is_null(v)){
		system_window_onresload(pw,NULL,NULL,NULL,0,false,NULL,0);
		return alloc_null();
	}
	const char* mime=NULL;
	const char* url=NULL;
	const char* redirectUrl=NULL;
	int status=0;
	unsigned char * bytes=NULL;
	bool isCancel=false;
	int bytesLen=0;
	//{x:r_bytes,y:cefResponse.end, _url:cefResponse.redirectUrl,width:cefResponse.mimeType,height:bytesLen,linkUrl:evnetValue,minSize:cefResponse.status}
	value mime_v=val_field(v,_id_width);
	value url_v=val_field(v,_id_linkUrl);
	value redicturl_v=val_field(v,_id_url);
	value status_v=val_field(v,_id_minsize);
	value bytes_v=val_field(v,_id_x);
	value cancel_v=val_field(v,_id_y);
	value len_v=val_field(v,_id_height);
	if(!val_is_null(mime_v))mime=val_string(mime_v);
	if(!val_is_null(url_v))url=val_string(url_v);
	if(!val_is_null(redicturl_v))redirectUrl=val_string(redicturl_v);
	if(!val_is_null(status_v))status=val_int(status_v);
	if(!val_is_null(bytes_v)){
		 buffer buf = val_to_buffer(bytes_v);
		if (buf!=0)bytes=(unsigned char *)buffer_data(buf);
	}
	if(!val_is_null(cancel_v))isCancel=val_bool(cancel_v);
	if(!val_is_null(len_v))bytesLen=val_int(len_v);
	system_window_onresload(pw,mime,url,redirectUrl,status,bytes,isCancel,bytesLen);
	return alloc_null();
}
DEFINE_PRIM(window_onresloaded,2);
/************************************************************************/
//--------- CefBrowser--------------------------------------------//

value browser_getframeobj(value h_window,value name){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	const char* f_name=val_string(name);
	if(f_name==NULL)return alloc_null();
	if(pw->g_handler->m_Browser.get()->GetFrame(f_name).get()){
		value hx_objs=alloc_empty_object();
		alloc_field(hx_objs,_id_x,alloc_bool(pw->g_handler->m_Browser.get()->GetFrame(f_name)->IsMain()));
		alloc_field(hx_objs,_id_y,name);
		alloc_field(hx_objs,_id_url,alloc_int(pw->g_handler->m_Browser.get()->GetFrame(f_name)->GetIdentifier()));
		return hx_objs;
	}
	return alloc_null();
}
DEFINE_PRIM(browser_getframeobj,2);


value browser_getfocusedframeobj(value h_window){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	if(pw->g_handler->m_Browser.get()->GetFocusedFrame().get()){
		value hx_objs=alloc_empty_object();
		alloc_field(hx_objs,_id_x,alloc_bool(pw->g_handler->m_Browser.get()->GetFocusedFrame()->IsMain()));
		alloc_field(hx_objs,_id_y,alloc_string(std::string(pw->g_handler.get()->m_Browser->GetFocusedFrame()->GetName()).c_str()));
		alloc_field(hx_objs,_id_url,alloc_int(pw->g_handler->m_Browser.get()->GetFocusedFrame()->GetIdentifier()));
		return hx_objs;
	}
	return alloc_null();
}
DEFINE_PRIM(browser_getfocusedframeobj,1);


value browser_getframenames(value h_window){
	return alloc_null();
}
DEFINE_PRIM(browser_getframenames,1);


value browser_getmainframeobj(value h_window){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	if(pw->g_handler->m_Browser.get()->GetMainFrame().get()==NULL){
		return alloc_null();
	}
	value hx_objs=alloc_empty_object();
	alloc_field(hx_objs,_id_x,alloc_bool(true));
	alloc_field(hx_objs,_id_y,alloc_string(std::string(pw->g_handler->m_Browser.get()->GetMainFrame()->GetName()).c_str()));
	alloc_field(hx_objs,_id_url,alloc_int(pw->g_handler->m_Browser.get()->GetMainFrame()->GetIdentifier()));
	return hx_objs;
}
DEFINE_PRIM(browser_getmainframeobj,1);

value browser_getmainframeid(value h_window){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	if(pw->g_handler->m_Browser.get()->GetMainFrame().get()==NULL){
		return alloc_int(-1);
	}
	return alloc_int(pw->g_handler->m_Browser.get()->GetMainFrame()->GetIdentifier());
}
DEFINE_PRIM(browser_getmainframeid,1);

value browser_goback(value h_window){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	pw->g_handler->m_Browser.get()->GoBack();
	return alloc_null();
}
DEFINE_PRIM(browser_goback,1);

value browser_goforward(value h_window){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	pw->g_handler->m_Browser.get()->GoForward();
	return alloc_null();
}
DEFINE_PRIM(browser_goforward,1);

value browser_hidepopup(value h_window){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	pw->g_handler->m_Browser.get()->HidePopup();
	return alloc_null();
}
DEFINE_PRIM(browser_hidepopup,1);

value browser_reload(value h_window,value isIgnoreCache){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	if(val_bool(isIgnoreCache)){
		pw->g_handler->m_Browser.get()->ReloadIgnoreCache();
	}else{
		pw->g_handler->m_Browser.get()->Reload();
	}
	
	return alloc_null();
}
DEFINE_PRIM(browser_reload,2);

value browser_ispopup(value h_window){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	return alloc_bool(pw->g_handler->m_Browser.get()->IsPopup());
}
DEFINE_PRIM(browser_ispopup,1);

value browser_hasdocument(value h_window){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	return alloc_bool(pw->g_handler->m_Browser.get()->HasDocument());

}
DEFINE_PRIM(browser_hasdocument,1);

value browser_getzoomlevel(value h_window){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	return alloc_float(pw->g_handler->m_Browser.get()->GetZoomLevel());
}
DEFINE_PRIM(browser_getzoomlevel,1);

value browser_setzoomlevel(value h_window,value level){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	pw->g_handler->m_Browser.get()->SetZoomLevel(val_float(level));
	return alloc_null();
}
DEFINE_PRIM(browser_setzoomlevel,2);

value browser_iswindowrenderingdisabled(value h_window){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	return alloc_bool(pw->g_handler->m_Browser.get()->IsWindowRenderingDisabled());
}
DEFINE_PRIM(browser_iswindowrenderingdisabled,1);

value browser_getispopupvisible(value h_window){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	return alloc_bool(pw->g_handler->m_Browser.get()->IsPopupVisible());
}
DEFINE_PRIM(browser_getispopupvisible,1);

value browser_getcangoback(value h_window){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	return alloc_bool(pw->g_handler->m_Browser.get()->CanGoBack());
}
DEFINE_PRIM(browser_getcangoback,1);

value browser_getcangoforward(value h_window){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	return alloc_bool(pw->g_handler->m_Browser.get()->CanGoForward());
}
DEFINE_PRIM(browser_getcangoforward,1);

value browser_setfocus(value h_window,value v){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	pw->g_handler->m_Browser.get()->SetFocus(val_bool(v));
	return alloc_null();	
}
DEFINE_PRIM(browser_setfocus,2);

value browser_stopfind(value h_window,value clearSelection){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	pw->g_handler->m_Browser.get()->StopFinding(val_bool(clearSelection));
	return alloc_null();	
}
DEFINE_PRIM(browser_stopfind,1);

//value matchCase,value findNext
value browser_find(value h_window,value identifier,value searchText,value forward,value mulitVal){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	bool matchCase=val_bool(val_field(mulitVal,_id_x));
	bool findNext=val_bool(val_field(mulitVal,_id_y));
	pw->g_handler->m_Browser.get()->Find(val_int(identifier),CefString(val_string(searchText)),val_bool(forward),matchCase,findNext);
	return alloc_null();
}
DEFINE_PRIM(browser_find,5);

value browser_closedevtools(value h_window){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	pw->g_handler->m_Browser.get()->CloseDevTools();
	return alloc_null();
}
DEFINE_PRIM(browser_closedevtools,1);

value browser_showdevtools(value h_window){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	pw->g_handler->m_Browser.get()->ShowDevTools();
	return alloc_null();
}
DEFINE_PRIM(browser_showdevtools,1);

value browser_clearhistory(value h_window){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	pw->g_handler->m_Browser.get()->ClearHistory();
	return alloc_null();
}
DEFINE_PRIM(browser_clearhistory,1);

value browser_stopload(value h_window){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	pw->g_handler->m_Browser.get()->StopLoad();
	return alloc_null();
}
DEFINE_PRIM(browser_stopload,1);

value browser_invalidate(value h_window,value x,value y,value width,value height){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	CefRect rect;
	rect.Set(val_int(x),val_int(y),val_int(width),val_int(height));
	pw->g_handler->m_Browser.get()->Invalidate(rect);
	return alloc_null();
}
DEFINE_PRIM(browser_invalidate,5);

value browser_setsize(value h_window,value type,value width,value height){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	pw->g_handler->m_Browser.get()->SetSize(CefBrowser::PaintElementType(val_int(type)),val_int(width),val_int(height));
	return alloc_null();
}
DEFINE_PRIM(browser_setsize,4);

value browser_getsize(value h_window,value type){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	int width,height;
	pw->g_handler->m_Browser.get()->GetSize(CefBrowser::PaintElementType(val_int(type)),width,height);
	value size_obj=alloc_empty_object();
	alloc_field(size_obj,_id_width,alloc_int(width));
	alloc_field(size_obj,_id_height,alloc_int(height));
	return size_obj;
}
DEFINE_PRIM(browser_getsize,2);

value browser_sendkeyevent(value h_window,value keyType,value keyInfo,value modifiers){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	CefKeyInfo cefKeyInfo;
	cefKeyInfo.key=val_int(val_field(keyInfo,_id_x));
	cefKeyInfo.sysChar=val_bool(val_field(keyInfo,_id_y));
	cefKeyInfo.imeChar=val_bool(val_field(keyInfo,_id_width));
	pw->g_handler->m_Browser.get()->SendKeyEvent(CefBrowser::KeyType(val_int(keyType)),cefKeyInfo,val_int(modifiers));
	cefKeyInfo.~CefStructBase();
	return alloc_null();
}
DEFINE_PRIM(browser_sendkeyevent,4);

value browser_sendmouseclickevent(value h_window,value point,value type,value isUp,value clickCount){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	int x=val_int(val_field(point,_id_x));
	int y=val_int(val_field(point,_id_y));
	pw->g_handler->m_Browser.get()->SendMouseClickEvent(x,y,CefBrowser::MouseButtonType(val_int(type)),val_bool(isUp),val_int(clickCount));
	return alloc_null();
}
DEFINE_PRIM(browser_sendmouseclickevent,5);

value browser_sendmousemoveevent(value h_window,value x,value y,value mouseLeave){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	pw->g_handler->m_Browser.get()->SendMouseMoveEvent(val_int(x),val_int(y),val_bool(mouseLeave));
	return alloc_null();
}
DEFINE_PRIM(browser_sendmousemoveevent,4);

value browser_sendmousewheelevent(value h_window,value x,value y,value deltaX,value deltaY){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	pw->g_handler->m_Browser.get()->SendMouseWheelEvent(val_int(x),val_int(y),val_int(deltaX),val_int(deltaY));
	return alloc_null();
}
DEFINE_PRIM(browser_sendmousewheelevent,5);

value browser_sendcapturelostevent(value h_window){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	pw->g_handler->m_Browser.get()->SendCaptureLostEvent();
	return alloc_null();
}
DEFINE_PRIM(browser_sendcapturelostevent,1);

value browser_sendfocusevent(value h_window,value v){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	pw->g_handler->m_Browser.get()->SendFocusEvent(val_bool(v));
	return alloc_null();
}
DEFINE_PRIM(browser_sendfocusevent,2);

value browser_seteventhandle(value h_window,value eventId,value v){
	window* pw=(window*)val_data(h_window);
	if(pw&&pw->g_handler&&pw->g_handler.get()){
		int browser_event_id=val_int(eventId);
		bool event_val=val_bool(v);
		switch(browser_event_id){
			case 1:
				pw->g_handler.get()->isBrowser_OnBeforeClose=event_val;
				break;
			case 2:
				pw->g_handler.get()->isBrowser_OnLoadStart=event_val;
				break;
			case 3:
				pw->g_handler.get()->isBrowser_OnLoadEnd=event_val;
				break;
			case 4:
				pw->g_handler.get()->isBrowser_OnLoadError=event_val;
				break;
			case 5:
				pw->g_handler.get()->isBrowser_OnNavStateChange=event_val;
				break;
			case 6:
				pw->g_handler.get()->isBrowser_OnContextCreated=event_val;
				break;
			case 7:
				pw->g_handler.get()->isBrowser_OnBeforePopup=event_val;
				break;
			case 8:
				pw->g_handler.get()->isBrowser_OnBeforeResourceLoad=event_val;
				break;
			case 9:
				pw->g_handler.get()->isBrowser_OnAddressChange=event_val;
				break;
			case 10:
				pw->g_handler.get()->isBrowser_OnTitleChange=event_val;
				break;
			case 11:
				pw->g_handler.get()->isBrowser_OnBeforeBrowse=event_val;
				break;
			case 12:
				pw->g_handler.get()->isBrowser_OnContentsSizeChange=event_val;
				break;
			case 13:
				pw->g_handler.get()->isBrowser_OnBeforeMenu=event_val;
				break;
			case 14:
				pw->g_handler.get()->isBrowser_OnJSAlert=event_val;
				break;
			case 15:
				pw->g_handler.get()->isBrowser_OnJSConfirm=event_val;
				break;
		}
	}
	return alloc_null();
}
DEFINE_PRIM(browser_seteventhandle,3);
/*************************************************************************/


/************************************************************************/
//--------- CefFrame--------------------------------------------//
value frame_undo(value h_window,value isMain,value name){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	if(val_bool(isMain)){
		if(pw->g_handler->m_Browser.get()->GetMainFrame().get())pw->g_handler->m_Browser.get()->GetMainFrame()->Undo();
	}else{
		const char* f_name=val_string(name);
		if(pw->g_handler->m_Browser.get()->GetFrame(f_name).get())pw->g_handler->m_Browser.get()->GetFrame(f_name)->Undo();
		//delete []f_name;
	}
	return alloc_null();
}
DEFINE_PRIM(frame_undo,3);

value frame_redo(value h_window,value isMain,value name){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	if(val_bool(isMain)){
		if(pw->g_handler->m_Browser.get()->GetMainFrame().get())pw->g_handler->m_Browser.get()->GetMainFrame()->Redo();
	}else{
		const char* f_name=val_string(name);
		if(pw->g_handler->m_Browser.get()->GetFrame(f_name).get())pw->g_handler->m_Browser.get()->GetFrame(f_name)->Redo();
		//delete []f_name;
	}
	return alloc_null();
}
DEFINE_PRIM(frame_redo,3);

value frame_cut(value h_window,value isMain,value name){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	if(val_bool(isMain)){
		if(pw->g_handler->m_Browser.get()->GetMainFrame().get())pw->g_handler->m_Browser.get()->GetMainFrame()->Cut();
	}else{
		const char* f_name=val_string(name);
		if(pw->g_handler->m_Browser.get()->GetFrame(f_name).get())pw->g_handler->m_Browser.get()->GetFrame(f_name)->Cut();
		//delete []f_name;
	}
	return alloc_null();
}
DEFINE_PRIM(frame_cut,3);

value frame_copy(value h_window,value isMain,value name){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	if(val_bool(isMain)){
		if(pw->g_handler->m_Browser.get()->GetMainFrame().get())pw->g_handler->m_Browser.get()->GetMainFrame()->Copy();
	}else{
		const char* f_name=val_string(name);
		if(pw->g_handler->m_Browser.get()->GetFrame(f_name).get())pw->g_handler->m_Browser.get()->GetFrame(f_name)->Copy();
		//delete []f_name;
	}
	return alloc_null();
}
DEFINE_PRIM(frame_copy,3);

value frame_paste(value h_window,value isMain,value name){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	if(val_bool(isMain)){
		if(pw->g_handler->m_Browser.get()->GetMainFrame().get())pw->g_handler->m_Browser.get()->GetMainFrame()->Paste();
	}else{
		const char* f_name=val_string(name);
		if(pw->g_handler->m_Browser.get()->GetFrame(f_name).get())pw->g_handler->m_Browser.get()->GetFrame(f_name)->Paste();
		//delete []f_name;
	}
	return alloc_null();
}
DEFINE_PRIM(frame_paste,3);

value frame_delete(value h_window,value isMain,value name){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	if(val_bool(isMain)){
		if(pw->g_handler->m_Browser.get()->GetMainFrame().get())pw->g_handler->m_Browser.get()->GetMainFrame()->Delete();
	}else{
		const char* f_name=val_string(name);
		if(pw->g_handler->m_Browser.get()->GetFrame(f_name).get())pw->g_handler->m_Browser.get()->GetFrame(f_name)->Delete();
		//delete []f_name;
	}
	return alloc_null();
}
DEFINE_PRIM(frame_delete,3);

value frame_selectall(value h_window,value isMain,value name){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	if(val_bool(isMain)){
		if(pw->g_handler->m_Browser.get()->GetMainFrame().get())pw->g_handler->m_Browser.get()->GetMainFrame()->SelectAll();
	}else{
		const char* f_name=val_string(name);
		if(pw->g_handler->m_Browser.get()->GetFrame(f_name).get())pw->g_handler->m_Browser.get()->GetFrame(f_name)->SelectAll();
		//delete []f_name;
	}
	return alloc_null();
}
DEFINE_PRIM(frame_selectall,3);

value frame_print(value h_window,value isMain,value name){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	if(val_bool(isMain)){
		if(pw->g_handler->m_Browser.get()->GetMainFrame().get())pw->g_handler->m_Browser.get()->GetMainFrame()->Print();
	}else{
		const char* f_name=val_string(name);
		if(pw->g_handler->m_Browser.get()->GetFrame(f_name).get())pw->g_handler->m_Browser.get()->GetFrame(f_name)->Print();
		//delete []f_name;
	}
	return alloc_null();
}
DEFINE_PRIM(frame_print,3);

value frame_viewsource(value h_window,value isMain,value name){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	if(val_bool(isMain)){
		if(pw->g_handler->m_Browser.get()->GetMainFrame().get())pw->g_handler->m_Browser.get()->GetMainFrame()->ViewSource();
	}else{
		const char* f_name=val_string(name);
		if(pw->g_handler->m_Browser.get()->GetFrame(f_name).get())pw->g_handler->m_Browser.get()->GetFrame(f_name)->ViewSource();
		//delete []f_name;
	}
	return alloc_null();
}
DEFINE_PRIM(frame_viewsource,3);

value frame_getsource(value h_window,value isMain,value name){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	if(val_bool(isMain)){
		if(pw->g_handler->m_Browser.get()->GetMainFrame().get())return alloc_string(std::string(pw->g_handler->m_Browser.get()->GetMainFrame()->GetSource()).c_str());
	}else{
		const char* f_name=val_string(name);
		if(pw->g_handler->m_Browser.get()->GetFrame(f_name).get())return alloc_string( std::string(pw->g_handler->m_Browser.get()->GetFrame(f_name)->GetSource()).c_str());
		//delete []f_name;
	}
	return alloc_null();
}
DEFINE_PRIM(frame_getsource,3);

value frame_gettext(value h_window,value isMain,value name){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	if(val_bool(isMain)){
		if(pw->g_handler->m_Browser.get()->GetMainFrame().get())return alloc_string(std::string(pw->g_handler->m_Browser.get()->GetMainFrame()->GetText()).c_str());
	}else{
		const char* f_name=val_string(name);
		if(pw->g_handler->m_Browser.get()->GetFrame(f_name).get())return alloc_string( std::string(pw->g_handler->m_Browser.get()->GetFrame(f_name)->GetText()).c_str());
	}
	return alloc_null();
}
DEFINE_PRIM(frame_gettext,3);

//value frame_loadrequest(value h_frame){
//	
//}

value frame_loadurl(value h_window,value isMain,value name,value url){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	if(val_bool(isMain)){
		wchar_t* w_str=c2w(val_string(url));
		if(pw->g_handler->m_Browser.get()->GetMainFrame().get())pw->g_handler->m_Browser.get()->GetMainFrame()->LoadURL(w_str);
		delete []w_str; //not sure
		w_str=NULL;
	}else{
		const char* f_name=val_string(name);
		wchar_t* w_str=c2w(val_string(url));
		if(pw->g_handler->m_Browser.get()->GetFrame(f_name).get())pw->g_handler->m_Browser.get()->GetFrame(f_name)->LoadURL(w_str);
		delete []w_str; //not sure
		w_str=NULL;
	}
	return alloc_null();
}
DEFINE_PRIM(frame_loadurl,4);

value frame_loadstring(value h_window,value isMain,value name,value str,value url){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	if(val_bool(isMain)){
		wchar_t* w_str=c2w(val_string(url));
		if(pw->g_handler->m_Browser.get()->GetMainFrame().get())pw->g_handler->m_Browser.get()->GetMainFrame()->LoadStringW(val_string(str),w_str);
		delete []w_str; //not sure
		w_str=NULL;
	}else{
		const char* f_name=val_string(name);
		wchar_t* w_str=c2w(val_string(url));
		if(pw->g_handler->m_Browser.get()->GetFrame(f_name).get())pw->g_handler->m_Browser.get()->GetFrame(f_name)->LoadStringW(val_string(str),w_str);
		delete []w_str; //not sure
		w_str=NULL;
	}
	return alloc_null();
}
DEFINE_PRIM(frame_loadstring,5);

value frame_executejavascript(value h_window,value isMain,value name,value jsCode,value externObj){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	const char* jsCode_str=val_string(jsCode);
	const char* url_str=val_string(val_field(externObj,_id_url));
	int startLine=val_int(val_field(externObj,_id_x));
	if(val_bool(isMain)){
		if(pw->g_handler->m_Browser.get()->GetMainFrame().get())pw->g_handler->m_Browser.get()->GetMainFrame()->ExecuteJavaScript(jsCode_str,url_str,startLine);
	}else{
		const char* f_name=val_string(name);
		if(pw->g_handler->m_Browser.get()->GetFrame(f_name).get())pw->g_handler->m_Browser.get()->GetFrame(f_name)->ExecuteJavaScript(jsCode_str,url_str,startLine);
		//delete []f_name;
	}
	//if(jsCode_str)delete []jsCode_str;
	//if(url_str)delete []url_str;
	return alloc_null();
}
DEFINE_PRIM(frame_executejavascript,5);


value frame_isfocused(value h_window,value isMain,value name){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	if(val_bool(isMain)){
		if(pw->g_handler->m_Browser.get()->GetMainFrame().get())return alloc_bool(pw->g_handler->m_Browser.get()->GetMainFrame()->IsFocused());
	}else{
		const char* f_name=val_string(name);
		if(pw->g_handler->m_Browser.get()->GetFrame(f_name).get())return alloc_bool(pw->g_handler->m_Browser.get()->GetFrame(f_name)->IsFocused());
		//delete []f_name;
	}
	return alloc_bool(false);
}
DEFINE_PRIM(frame_isfocused,3);


value frame_getidentifier(value h_window,value isMain,value name){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	if(val_bool(isMain)){
		if(pw->g_handler->m_Browser.get()->GetMainFrame().get())return alloc_int(pw->g_handler->m_Browser.get()->GetMainFrame()->GetIdentifier());
	}else{
		const char* f_name=val_string(name);
		if(pw->g_handler->m_Browser.get()->GetFrame(f_name).get())return alloc_int(pw->g_handler->m_Browser.get()->GetFrame(f_name)->GetIdentifier());
		//delete []f_name;
	}
	return alloc_int(-1);
}
DEFINE_PRIM(frame_getidentifier,3);


value frame_getparentid(value h_window,value ismain,value fname){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	if(val_bool(ismain)){
		return alloc_int(-1);
	}
	const char* f_namestr=val_string(fname);
	if(pw->g_handler->m_Browser.get()->GetFrame(f_namestr).get()!=NULL&&pw->g_handler->m_Browser.get()->GetFrame(f_namestr)->GetParent().get()!=NULL){
		value id=alloc_int(pw->g_handler->m_Browser.get()->GetFrame(f_namestr)->GetParent()->GetIdentifier());
		//delete []f_namestr;
		return id;
	}
	return alloc_int(-1);
}
DEFINE_PRIM(frame_getparentid,3);


value frame_getparentobj(value h_window,value ismain,value fname){
	if(val_bool(ismain)){
		return alloc_null();
	}
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	const char* f_namestr=val_string(fname);
	if(pw->g_handler->m_Browser.get()->GetFrame(f_namestr).get()!=NULL&&pw->g_handler->m_Browser.get()->GetFrame(f_namestr)->GetParent().get()!=NULL){
		value hx_objs=alloc_empty_object();
		alloc_field(hx_objs,_id_x,alloc_bool(pw->g_handler->m_Browser.get()->GetFrame(f_namestr)->GetParent()->IsMain()));
		alloc_field(hx_objs,_id_y,alloc_string(std::string(pw->g_handler->m_Browser.get()->GetFrame(f_namestr)->GetParent()->GetName()).c_str()));
		alloc_field(hx_objs,_id_url,alloc_int(pw->g_handler->m_Browser.get()->GetFrame(f_namestr)->GetParent()->GetIdentifier()));
		//delete []f_namestr;
		return hx_objs;
	}
	return alloc_null();
}
DEFINE_PRIM(frame_getparentobj,3);


value frame_geturl(value h_window,value ismain,value fname){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	if(val_bool(ismain)){
		return alloc_string(std::string(pw->g_handler->m_Browser.get()->GetMainFrame()->GetURL()).c_str());
	}
	const char* f_namestr=val_string(fname);
	if(pw->g_handler->m_Browser.get()->GetFrame(f_namestr).get()){
		return alloc_string(std::string(pw->g_handler->m_Browser.get()->GetFrame(f_namestr)->GetURL()).c_str());
	}
	return alloc_null();
}
DEFINE_PRIM(frame_geturl,3);

value frame_evaljavascript(value h_window,value ismain,value fname,value jsCode){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	bool isMain=val_bool(ismain);
	CefRefPtr<CefV8Context> v8Context;
	if(isMain){
		v8Context=pw->g_handler->m_Browser.get()->GetMainFrame()->GetV8Context();
	}else{
		v8Context=pw->g_handler->m_Browser.get()->GetFrame(val_string(fname))->GetV8Context();
	}
	if(v8Context.get()==NULL)return alloc_null();
	CefRefPtr<CefV8Value> cefV8Value;
	CefRefPtr<CefV8Exception> cefV8Exception;
	bool isResult=v8Context->Eval(val_string(jsCode),cefV8Value,cefV8Exception);
	if(isResult&&cefV8Value!=NULL){
		value resultValue=V8ValueToHxValue(cefV8Value);
		return resultValue;
	}
	return alloc_null();
}
DEFINE_PRIM(frame_evaljavascript,4);

value frame_invokejsfunc(value h_window,value ismain,value fname,value jsName,value hx_args){
	window* pw=(window*)val_data(h_window);
	if(pw->g_handler==NULL||pw->g_handler.get()==NULL||pw->g_handler->m_Browser==NULL||pw->g_handler->m_Browser.get()==NULL)return alloc_null();
	bool isMain=val_bool(ismain);
	CefFrame* p_frame;
	const char * f_str=val_string(fname);
	if(isMain){
		p_frame=pw->g_handler->m_Browser.get()->GetMainFrame().get();
	}else{
		p_frame=pw->g_handler->m_Browser.get()->GetFrame(f_str).get();
	}
	if(p_frame!=NULL){
		CefRefPtr<CefV8Context> v8Context;
		if(isMain){
			v8Context=pw->g_handler->m_Browser.get()->GetMainFrame()->GetV8Context();
		}else{
			v8Context=pw->g_handler->m_Browser.get()->GetFrame(f_str)->GetV8Context();
		}
		 if (!v8Context.get()) {
			 //printf("--------------------Failed to get V8 context!");
			return alloc_null();
		} 
		v8Context.get()->Enter();
		CefRefPtr<CefV8Value> globalObj = v8Context->GetGlobal();
		CefRefPtr<CefV8Value> evalFunc = globalObj->GetValue(val_string(jsName));
		if(evalFunc.get()==NULL){
			v8Context.get()->Exit();
			return alloc_null();
		}
		CefV8ValueList v8_args;
		if(!val_is_null(hx_args)){
			int arr_len=val_array_size(hx_args);
			for(int i=0;i<arr_len;++i){
				v8_args.push_back(hxValueToV8Value(val_array_i(hx_args,i)));
			}
		}
		//v8Context.get()->Enter();
		//CefRefPtr<CefV8Value> cefV8Value=evalFunc->ExecuteFunction(globalObj,v8_args);
		CefRefPtr<CefV8Value> cefV8Value=evalFunc->ExecuteFunctionWithContext(v8Context,globalObj,v8_args);
		if(cefV8Value.get()==NULL||evalFunc->HasException()){
			v8Context.get()->Exit();
			return alloc_null();
		}
		v8Context.get()->Exit();
		//CefRefPtr<CefV8Value> cefV8Value=v8Context->GetGlobal()->ExecuteFunctionWithContext(v8Context,globalObj,v8_args);
		value resultValue=V8ValueToHxValue(cefV8Value);
		return resultValue;
	}
	return alloc_null();
}
DEFINE_PRIM(frame_invokejsfunc,5);
/************************************************************************/

value request_getheader(value inHandle,value v)
{
	if(!val_is_null(inHandle))
	{
		CefRequest* request=(CefRequest*)val_data(inHandle); 
		if(request!=NULL)
		{
			const char* hv=val_string(v);
			CefRequest::HeaderMap headers;
			request->GetHeaderMap(headers);
			CefRequest::HeaderMap::const_iterator it1, it2;
			for(it1 = headers.begin(); it1 != headers.end(); ++it1) {
				//printf("first:%s\n",std::string(it1->first).c_str());
				//printf("second:%s\n",std::string(it1->second).c_str());
				if(it1->first==hv){
					return alloc_string(std::string(it1->second).c_str());
				}
			}
		}
	}
	return alloc_null();
}
DEFINE_PRIM(request_getheader,2);

value request_setheader(value inHandle,value v1,value v2)
{
	if(!val_is_null(inHandle))
	{
		CefRequest* request=(CefRequest*)val_data(inHandle); 
		if(request!=NULL){
			CefRequest::HeaderMap headers;
			request->GetHeaderMap(headers);
			headers.insert(std::make_pair(CefString(val_string(v1)),CefString(val_string(v2))));
		}
	}
	return alloc_null();
}
DEFINE_PRIM(request_setheader,3);

value request_getfirstcookie(value inHandle)
{
	if(!val_is_null(inHandle))
	{
		CefRequest* request=(CefRequest*)val_data(inHandle); 
		if(request!=NULL){
			return alloc_string(std::string(request->GetFirstPartyForCookies()).c_str());
		}
	}
	return alloc_null();
}
DEFINE_PRIM(request_getfirstcookie,1);

value request_geturl(value inHandle)
{
	if(!val_is_null(inHandle))
	{
		CefRequest* request=(CefRequest*)val_data(inHandle); 
		if(request!=NULL){
			return alloc_string(std::string(request->GetURL()).c_str());
		}
	}
	return alloc_null();
}
DEFINE_PRIM(request_geturl,1);

value request_seturl(value inHandle,value v)
{
	if(!val_is_null(inHandle))
	{
		CefRequest* request=(CefRequest*)val_data(inHandle); 
		if(request!=NULL){
			request->SetURL(CefString(val_string(v)));
		}
	}
	return alloc_null();
}
DEFINE_PRIM(request_seturl,2);

value request_getmethod(value inHandle)
{
	if(!val_is_null(inHandle))
	{
		CefRequest* request=(CefRequest*)val_data(inHandle); 
		if(request!=NULL){
			return alloc_string(std::string(request->GetMethod()).c_str());
		}
	}
	return alloc_null();
}
DEFINE_PRIM(request_getmethod,1);

value request_setmethod(value inHandle,value v)
{
	if(!val_is_null(inHandle))
	{
		CefRequest* request=(CefRequest*)val_data(inHandle); 
		if(request!=NULL){
			request->SetMethod(CefString(val_string(v)));
		}
	}
	return alloc_null();
}
DEFINE_PRIM(request_setmethod,2);

 //CefRequest::HeaderMap::const_iterator it1, it2;

 // for(it1 = map1.begin(); it1 != map1.end(); ++it1) {
 //   it2 = map2.find(it1->first);
 //   ASSERT_TRUE(it2 != map2.end());
 //   ASSERT_EQ(it1->second, it2->second);
 // }

//CefRequest::HeaderMap headers1, headers2;
//  request1->GetHeaderMap(headers1);
//  request2->GetHeaderMap(headers2);

//CefRequest::HeaderMap headers;
//  headers.insert(std::make_pair("HeaderA", "ValueA"));
//  headers.insert(std::make_pair("HeaderB", "ValueB"));
//  request->SetHeaderMap(headers);

