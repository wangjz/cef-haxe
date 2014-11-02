// Copyright (c) 2011 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_CLIENT_HANDLER_H_
#define CEF_TESTS_CEFCLIENT_CLIENT_HANDLER_H_
#pragma once

#include <map>
#include <string>
#include "include/cef_client.h"
#include "cefclient/util.h"
#include "cefclient/ExternalInterface.h"
#include "cefclient/MyQueue.h"
// Define this value to redirect all popup URLs to the main application browser
// window.
// #define TEST_REDIRECT_POPUP_URLS

// ClientHandler implementation.
static value V8ValueToHxValue(CefRefPtr<CefV8Value> v8Value);
static CefRefPtr<CefV8Value> hxValueToV8Value(value hxValue);
static void hx_field_iter(value v,int f,void* inValue);

static bool globalCookieInit=false;
static value V8ValueToHxValue(CefRefPtr<CefV8Value> v8Value) {
    if (v8Value->IsUndefined())
		return alloc_null();
    else if (v8Value->IsNull())
		return alloc_null();
	else if(v8Value->IsUInt()){
		return alloc_float(v8Value->GetUIntValue());
	}
	else if (v8Value->IsInt()){
		return alloc_int(v8Value->GetIntValue());
	}
	else if (v8Value->IsDouble())
	{
		return alloc_float(v8Value->GetDoubleValue());
	}
    else if (v8Value->IsBool())
		return alloc_bool(v8Value->GetBoolValue());
    else if (v8Value->IsString())
		return alloc_string(std::string(v8Value->GetStringValue()).c_str());
    else if (v8Value->IsArray()) {
		int len = v8Value->GetArrayLength();
		value hx_arr=alloc_array(len);
		for (int i = 0; i < len; ++i) {
			//::gc_safe_point();
			val_array_set_i(hx_arr,i,V8ValueToHxValue(v8Value->GetValue(i)));
		}
		return hx_arr;
    } else if (v8Value->IsObject()) {
		value hx_obj=alloc_empty_object();
		std::vector<CefString> keys;
		if (v8Value->GetKeys(keys)) {
			for (size_t i = 0; i < keys.size(); ++i) {
				//::gc_safe_point();
				alloc_field(hx_obj,val_id(keys[i].ToString().c_str()),V8ValueToHxValue(v8Value->GetValue(keys[i])));
			}
		}
		return hx_obj;
    }
	return alloc_null();
  }



static void hx_field_iter(value v,int f,void* inValue)
{
	value hxVal=val_field(v,f);
	value fNameHx=val_field_name(f);
	CefRefPtr<CefV8Value> v8_obj=(CefV8Value*)inValue;
	v8_obj->SetValue(val_string(fNameHx),hxValueToV8Value(hxVal),V8_PROPERTY_ATTRIBUTE_NONE);
}



static CefRefPtr<CefV8Value> hxValueToV8Value(value hxValue)
{
	if(val_is_null(hxValue))
		return CefV8Value::CreateNull();
	else if(val_is_bool(hxValue))
		return CefV8Value::CreateBool(val_bool(hxValue));
	else if(val_is_int(hxValue))
		return CefV8Value::CreateInt(val_int(hxValue));
	else if(val_is_float(hxValue))
		return CefV8Value::CreateDouble(val_float(hxValue));
	else if(val_is_string(hxValue))
		return CefV8Value::CreateString(val_string(hxValue));
	else if(val_is_array(hxValue)){
		int arr_len=val_array_size(hxValue);
		CefRefPtr<CefV8Value> v8_arr=CefV8Value::CreateArray(arr_len);
		for(int i=0;i<arr_len;++i){
			v8_arr->SetValue(i,hxValueToV8Value(val_array_i(hxValue,i)));
		}
		return v8_arr;
	}else if(val_is_object(hxValue)){
		CefRefPtr<CefV8Value> v8_obj=CefV8Value::CreateObject(NULL);
		val_iter_fields(hxValue,hx_field_iter,v8_obj);
		return v8_obj;
	}
	return CefV8Value::CreateNull();
}
/*
namespace {
class ClientCookieVisitor : public CefCookieVisitor {
 public:
	 ClientCookieVisitor() {
  }
  virtual bool Visit(const CefCookie& cookie, int count, int total,bool& deleteCookie) OVERRIDE{
	  printf("count:%s\n",count);
	  printf("total:%s\n",total);
	  //printf("Cookie name:%s value:%s\n", std::string(CefString(cookie.name.str)).c_str(),std::string(CefString(cookie.value.str)).c_str());
	  return true;
  }
 private:
  IMPLEMENT_REFCOUNTING(ClientCookieVisitor);
};
}
*/
class ClientHandler : public CefClient,
                      public CefLifeSpanHandler,
                      public CefLoadHandler,
                      public CefRequestHandler,
                      public CefDisplayHandler,
                      public CefFocusHandler,
                      public CefKeyboardHandler,
                      //public CefPrintHandler,
                      public CefV8ContextHandler,
					  public CefV8Handler,
                      public CefDragHandler,
                      public CefPermissionHandler,
                      public CefGeolocationHandler,
					  public CefMenuHandler,
					  public CefJSDialogHandler{
 public:
  ClientHandler();
  virtual ~ClientHandler();

  // CefClient methods
  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefFocusHandler> GetFocusHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE {
    return this;
  }
 /* virtual CefRefPtr<CefPrintHandler> GetPrintHandler() OVERRIDE {
    return this;
  }*/
  virtual CefRefPtr<CefV8ContextHandler> GetV8ContextHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefDragHandler> GetDragHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefPermissionHandler> GetPermissionHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefGeolocationHandler> GetGeolocationHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefMenuHandler> GetMenuHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() OVERRIDE {
    return this;
  }
  /*
  virtual CefRefPtr<CefCookieManager> GetCookieManager(
      CefRefPtr<CefBrowser> browser,
      const CefString& main_url) OVERRIDE{ 
		  CefRefPtr<CefCookieManager>  cookieManager=CefCookieManager::CreateManager(CefString("c:\\cookie"));
		 // CefRefPtr<CefCookieVisitor> cookieVisitor = new ClientCookieVisitor();
		//  cookieManager->VisitAllCookies(cookieVisitor);
		  return cookieManager;
  
  }
  */
 
  virtual void OnContentsSizeChange(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    int width,
                                    int height) OVERRIDE;
  virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame,
                              CefRefPtr<CefRequest> request,
                              NavType navType,
                              bool isRedirect) OVERRIDE;

  // CefLifeSpanHandler methods
  virtual bool OnBeforePopup(CefRefPtr<CefBrowser> parentBrowser,
                             const CefPopupFeatures& popupFeatures,
                             CefWindowInfo& windowInfo,
                             const CefString& url,
                             CefRefPtr<CefClient>& client,
                             CefBrowserSettings& settings) OVERRIDE;
  virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

  // CefLoadHandler methods
  virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame) OVERRIDE;
  virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
                         CefRefPtr<CefFrame> frame,
                         int httpStatusCode) OVERRIDE;
  virtual bool OnLoadError(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           ErrorCode errorCode,
                           const CefString& failedUrl,
                           CefString& errorText) OVERRIDE;

  // CefRequestHandler methods
  virtual bool OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefRequest> request,
                                   CefString& redirectUrl,
                                   CefRefPtr<CefStreamReader>& resourceStream,
                                   CefRefPtr<CefResponse> response,
                                   int loadFlags) OVERRIDE;
/*
  virtual void OnResourceResponse(CefRefPtr<CefBrowser> browser,
                                  const CefString& url,
                                  CefRefPtr<CefResponse> response,
                                  CefRefPtr<CefContentFilter>& filter) OVERRIDE;
								  */
 /* virtual bool GetDownloadHandler(CefRefPtr<CefBrowser> browser,
                                  const CefString& mimeType,
                                  const CefString& fileName,
                                  int64 contentLength,
                                  CefRefPtr<CefDownloadHandler>& handler)
                                  OVERRIDE;*/

  // CefDisplayHandler methods
  virtual void OnNavStateChange(CefRefPtr<CefBrowser> browser,
                                bool canGoBack,
                                bool canGoForward) OVERRIDE;
  virtual void OnAddressChange(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               const CefString& url) OVERRIDE;
  virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
                             const CefString& title) OVERRIDE;

  virtual bool OnBeforeMenu(CefRefPtr<CefBrowser> browser,
                            const CefMenuInfo& menuInfo) OVERRIDE;
  virtual void GetMenuLabel(CefRefPtr<CefBrowser> browser,
                            MenuId menuId,
                            CefString& label) OVERRIDE;
  virtual bool OnJSAlert(CefRefPtr<CefBrowser> browser,
                         CefRefPtr<CefFrame> frame,
                         const CefString& message) OVERRIDE;
  virtual bool OnJSConfirm(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           const CefString& message,
                           bool& retval) OVERRIDE;
 /* virtual bool OnJSPrompt(CefRefPtr<CefBrowser> browser,
                          CefRefPtr<CefFrame> frame,
                          const CefString& message,
                          const CefString& defaultValue,
                          bool& retval,
                          CefString& result) OVERRIDE;*/
 /* virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                const CefString& message,
                                const CefString& source,
                                int line) OVERRIDE;*/

  // CefFocusHandler methods.
 /* virtual void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefDOMNode> node) OVERRIDE;*/

  // CefKeyboardHandler methods.
 /* virtual bool OnKeyEvent(CefRefPtr<CefBrowser> browser,
                          KeyEventType type,
                          int code,
                          int modifiers,
                          bool isSystemKey,
                          bool isAfterJavaScript) OVERRIDE;*/

  // CefPrintHandler methods.
 /* virtual bool GetPrintHeaderFooter(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    const CefPrintInfo& printInfo,
                                    const CefString& url,
                                    const CefString& title,
                                    int currentPage,
                                    int maxPages,
                                    CefString& topLeft,
                                    CefString& topCenter,
                                    CefString& topRight,
                                    CefString& bottomLeft,
                                    CefString& bottomCenter,
                                    CefString& bottomRight) OVERRIDE;*/

  // CefV8ContextHandler methods
  virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                CefRefPtr<CefV8Context> context) OVERRIDE;

  // CefDragHandler methods.
 /* virtual bool OnDragStart(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefDragData> dragData,
                           DragOperationsMask mask) OVERRIDE;
  virtual bool OnDragEnter(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefDragData> dragData,
                           DragOperationsMask mask) OVERRIDE;*/
  //virtual bool OnTooltip(CefRefPtr<CefBrowser> browser,
  //                       CefString& text) OVERRIDE;
  // CefPermissionHandler methods.
 /* virtual bool OnBeforeScriptExtensionLoad(CefRefPtr<CefBrowser> browser,
                                       CefRefPtr<CefFrame> frame,
                                       const CefString& extensionName) OVERRIDE;*/

  // CefGeolocationHandler methods.
  virtual void OnRequestGeolocationPermission(
      CefRefPtr<CefBrowser> browser,
      const CefString& requesting_url,
      int request_id,
      CefRefPtr<CefGeolocationCallback> callback) OVERRIDE;

  // DownloadListener methods
 // virtual void NotifyDownloadComplete(const CefString& fileName) OVERRIDE;
 // virtual void NotifyDownloadError(const CefString& fileName) OVERRIDE;

   // Virutal on CefV8Handler
  virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) OVERRIDE;

  void SetMainWindow(void* pWindow);
  void* GetMainWindow() { return pWindow; }
  void SetMainHwnd(CefWindowHandle hwnd);
  CefWindowHandle GetMainHwnd() { return m_MainHwnd; }

  CefRefPtr<CefBrowser> GetBrowser() { return m_Browser; }
  CefWindowHandle GetBrowserHwnd() { return m_BrowserHwnd; }

  //std::string GetLogFile();

  //void SetLastDownloadFile(const std::string& fileName);
  //std::string GetLastDownloadFile();

  // DOM visitors will be called after the associated path is loaded.
 /* void AddDOMVisitor(const std::string& path, CefRefPtr<CefDOMVisitor> visitor);
  CefRefPtr<CefDOMVisitor> GetDOMVisitor(const std::string& path);*/

  void OnResourceLoaded(const char* mime,const char* url,const char* redirectUrl,int status,unsigned char * bytes,bool isCancel,int bytesLen);
  // Send a notification to the application. Notifications should not block the
  // caller.
  enum NotificationType {
    NOTIFY_CONSOLE_MESSAGE,
    NOTIFY_DOWNLOAD_COMPLETE,
    NOTIFY_DOWNLOAD_ERROR,
  };
  void SendNotification(NotificationType type);
  void CloseMainWindow();
  // The child browser window
  CefRefPtr<CefBrowser> m_Browser;

  //bool isBrowser_OnAfterCreated; //0
  bool isBrowser_OnBeforeClose;  //1
  bool isBrowser_OnLoadStart; //2
  bool isBrowser_OnLoadEnd; //3
  bool isBrowser_OnLoadError; //4
  bool isBrowser_OnNavStateChange; //5
  bool isBrowser_OnContextCreated; //6
  bool isBrowser_OnBeforePopup; //7
  bool isBrowser_OnBeforeResourceLoad; //8
  bool isBrowser_OnAddressChange; //9
  bool isBrowser_OnTitleChange; //10
  bool isBrowser_OnBeforeBrowse; //11
  bool isBrowser_OnContentsSizeChange; //12
  bool isBrowser_OnBeforeMenu;//13
  bool isBrowser_OnJSAlert;//14
  bool isBrowser_OnJSConfirm; //15
 // bool isBrowser_OnJSPrompt;//16

 protected:
  // The main frame window handle
  CefWindowHandle m_MainHwnd;

  // The child browser window handle
  CefWindowHandle m_BrowserHwnd;

  // Support for DOM visitors.
  //typedef std::map<std::string, CefRefPtr<CefDOMVisitor> > DOMVisitorMap;
  //DOMVisitorMap m_DOMVisitors;

  // True if a form element currently has focus
 // bool m_bFormElementHasFocus;

  void* pWindow;

  MyQueue resQueue;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(ClientHandler);
  // Include the default locking implementation.
  IMPLEMENT_LOCKING(ClientHandler);
};

#endif  // CEF_TESTS_CEFCLIENT_CLIENT_HANDLER_H_
