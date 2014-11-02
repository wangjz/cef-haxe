#ifndef EXTERNAL_INTERFACE
#define EXTERNAL_INTERFACE
#include <hx/CFFI.h>
extern vkind gObjectKind;
extern value gHaxeStaticCefApp;
//extern int Global_Atom_KeyId; //系统热键全局标识
extern int sgIDsInit;
extern int _id_url;
extern int _id_minsize;
extern int _id_onjscall;
extern int _id_windowFlag;
extern int _id_cef_browserset;
extern int _id_on_create;
extern int _id_on_close;
extern int _id_on_destroy;
extern int _id_on_size;
extern int _id_on_maximize;
extern int _id_on_minimize;
extern int _id_on_restore;

extern int _id_on_sysmsg;
//extern int _id_on_messagehook;
//extern int _id_on_rightclick;

extern int _id_width;
extern int _id_height;
extern int _id_x;
extern int _id_y;
extern int _id_frameUrl;
extern int _id_imageUrl;
extern int _id_linkUrl;
extern int _id_selectionText;
extern int _id_misspelledWord;
extern int _id_securityInfo;
extern int _id_typeFlags;
extern int _id_editFlags;

extern int _id_onBrowserEvent;
//extern int _id_on_resload;

extern int _id_accelerated2DCanvasDisabled;
extern int _id_acceleratedCompositingEnabled;
extern int _id_acceleratedPaintingDisabled;
extern int _id_acceleratedLayersDisabled;
extern int _id_acceleratedFiltersDisabled;
extern int _id_acceleratedPluginsDisabled;
extern int _id_acceleratedVideoDisabled;
extern int _id_applicationCacheDisabled;
extern int _id_authorAndUserStylesDisabled;
extern int _id_caretBrowsingEnabled;
extern int _id_databasesDisabled;
extern int _id_defaultEncoding;
extern int _id_developerToolsDisabled;
extern int _id_domPasteDisabled;
extern int _id_dragDropDisabled;
extern int _id_encodingDetectorEnabled;
extern int _id_fileAccessFromFileUrlsAllowed;
extern int _id_universalAccessFromFileUrlsAllowed;
extern int _id_hyperlinkAuditingDisabled;
extern int _id_imageLoadDisabled;
extern int _id_javaDisabled;
extern int _id_javaScriptAccessClipboardDisallowed;
extern int _id_javaScriptCloseWindowsDisallowed;
extern int _id_javaScriptDisabled;
extern int _id_javaScriptOpenWindowsDisallowed;
extern int _id_localStorageDisabled;
extern int _id_pageCacheDisabled;
extern int _id_pluginsDisabled;
extern int _id_remoteFontsDisabled;
extern int _id_shrinkStandaloneImagesToFit;
extern int _id_siteSpecificQuirksDisabled;
extern int _id_tabToLinksDisabled;
extern int _id_textAreaResizeDisabled;
extern int _id_userStyleSheetEnabled;
extern int _id_userStyleSheetLocation;
extern int _id_webGLDisabled;
extern int _id_webSecurityDisabled;
extern int _id_xssAuditorEnabled;
extern int _id_loadDropsDisabled;
extern int _id_historyDisabled;
extern int _id_animationFrameRate;
extern int _id_fullscreenEnabled;
extern int _id_standardFontFamily;
extern int _id_fixedFontFamily;
extern int _id_sansSerifFontFamily;
extern int _id_serifFontFamily;
extern int _id_cursiveFontFamily;
extern int _id_fantasyFontFamily;
extern int _id_defaultFixedFontSize;
extern int _id_defaultFontSize;
extern int _id_minimumFontSize;
extern int _id_minimumLogicalFontSize;


extern wchar_t* c2w(const char* str);
extern const char* w2c(const wchar_t* wstr);
extern value executeJsCallBack(void* window,const char * funcName,value args);
extern value onBrowserEvent(void* window,const char* eventName, value args);
extern void releaseHxWindow(void* window);
extern int OnBeforeResourceLoad(void* window,const char * url,int type);
extern bool getIsFilterPath(const char* str);
//extern int loadHaxePlug(const char* plugPath,const char* plugHwnd);
extern bool closeProcessById(int plugid);
#endif