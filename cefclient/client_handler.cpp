// Copyright (c) 2011 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "cefclient/client_handler.h"
#include <stdio.h>
#include <sstream>
#include <string>
#include "include/cef_browser.h"
#include "include/cef_frame.h"

//m_bFormElementHasFocus(false)
//isBrowser_OnAfterCreated(false),
ClientHandler::ClientHandler()
  : m_MainHwnd(NULL),
    m_BrowserHwnd(NULL),
	pWindow(NULL),
	isBrowser_OnBeforeClose(false),
	isBrowser_OnLoadStart(false),
	isBrowser_OnLoadEnd(false),
	isBrowser_OnLoadError(false),
	isBrowser_OnNavStateChange(false),
	isBrowser_OnContextCreated(false),
	isBrowser_OnBeforePopup(false),
	isBrowser_OnBeforeResourceLoad(false),
	isBrowser_OnAddressChange(false),
	isBrowser_OnTitleChange(false),
	isBrowser_OnBeforeBrowse(false),
	isBrowser_OnContentsSizeChange(false),
	isBrowser_OnBeforeMenu(false),
	isBrowser_OnJSAlert(false),
	isBrowser_OnJSConfirm(false)
	//isBrowser_OnJSPrompt(false)
	{}

ClientHandler::~ClientHandler() {
	m_MainHwnd=NULL;
	m_BrowserHwnd=NULL;
	pWindow=NULL;
	isBrowser_OnBeforeClose=false;
	isBrowser_OnLoadStart=false;
	isBrowser_OnLoadEnd=false;
	isBrowser_OnLoadError=false;
	isBrowser_OnNavStateChange=false;
	isBrowser_OnContextCreated=false;
	isBrowser_OnBeforePopup=false;
	isBrowser_OnBeforeResourceLoad=false;
	isBrowser_OnAddressChange=false;
	isBrowser_OnTitleChange=false;
	isBrowser_OnBeforeBrowse=false;
	isBrowser_OnContentsSizeChange=false;
	isBrowser_OnBeforeMenu=false;
	isBrowser_OnJSAlert=false;
	isBrowser_OnJSConfirm=false;
	//isBrowser_OnJSPrompt=false;
	//m_DOMVisitors.clear();
	//printf("----client handle release\n");
}


void ClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  REQUIRE_UI_THREAD();

  AutoLock lock_scope(this);
  if (!m_Browser.get())   {
    // We need to keep the main child window, but not popup windows
    m_Browser = browser;
    m_BrowserHwnd = browser->GetWindowHandle();
	onBrowserEvent(this->pWindow,"onAfterCreated",alloc_null());
  }
}

bool ClientHandler::DoClose(CefRefPtr<CefBrowser> browser) {
  REQUIRE_UI_THREAD();

  if (m_BrowserHwnd == browser->GetWindowHandle()) {
    // Since the main window contains the browser window, we need to close
    // the parent window instead of the browser window.
    CloseMainWindow();

    // Return true here so that we can skip closing the browser window
    // in this pass. (It will be destroyed due to the call to close
    // the parent above.)
    return true;
  }

  // A popup browser window is not contained in another window, so we can let
  // these windows close by themselves.
  return false;
}

void ClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  REQUIRE_UI_THREAD();
  if (m_BrowserHwnd == browser->GetWindowHandle()) {
    // Free the browser pointer so that the browser can be destroyed
    //printf("----browser handle release\n");
	if(isBrowser_OnBeforeClose&&this->pWindow){
		 onBrowserEvent(this->pWindow,"onBeforeClose",alloc_null());
	}
    m_Browser = NULL;
	this->pWindow=NULL;
  }
}

void ClientHandler::OnLoadStart(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame) {
  REQUIRE_UI_THREAD();
  if(isBrowser_OnLoadStart==false)return;
  if(this->pWindow&&frame.get()){
		value eventArgs=alloc_empty_object();
		alloc_field(eventArgs,_id_x,alloc_bool(frame->IsMain()));
		if(frame->IsMain()){
			alloc_field(eventArgs,_id_y,alloc_string(""));
		}else{
			alloc_field(eventArgs,_id_y,alloc_string(std::string(frame->GetName()).c_str()));
		}
		onBrowserEvent(this->pWindow,"onLoadStart",eventArgs);
	}
}

void ClientHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame,
                              int httpStatusCode) {
  REQUIRE_UI_THREAD();
  if(isBrowser_OnLoadEnd==false)return;
   if(this->pWindow&&frame.get()){
		value eventArgs=alloc_empty_object();
		alloc_field(eventArgs,_id_x,alloc_bool(frame->IsMain()));
		if(frame->IsMain()){
			alloc_field(eventArgs,_id_y,alloc_string(""));
		}else{
			alloc_field(eventArgs,_id_y,alloc_string(std::string(frame->GetName()).c_str()));
		}
		alloc_field(eventArgs,_id_url,alloc_int(httpStatusCode));
		onBrowserEvent(this->pWindow,"onLoadEnd",eventArgs);
	}
}

bool ClientHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                ErrorCode errorCode,
                                const CefString& failedUrl,
                                CefString& errorText) {
  REQUIRE_UI_THREAD();
  if(isBrowser_OnLoadError==false)return false;
  if(this->pWindow&&frame.get()){
	  value eventArgs=alloc_empty_object();
	  alloc_field(eventArgs,_id_x,alloc_bool(frame->IsMain()));
	  if(frame->IsMain()){
			alloc_field(eventArgs,_id_y,alloc_string(""));
		}else{
			alloc_field(eventArgs,_id_y,alloc_string(std::string(frame->GetName()).c_str()));
		}
	  alloc_field(eventArgs,_id_width,alloc_int(int(errorCode)));
	  alloc_field(eventArgs,_id_url,alloc_string(std::string(failedUrl).c_str()));
	  value ret=onBrowserEvent(this->pWindow,"onLoadError",eventArgs);
	  if(!val_is_null(ret)){
	    errorText=val_string(ret);
		return true;
	  }
  }
  return false;
}

//bool ClientHandler::GetDownloadHandler(CefRefPtr<CefBrowser> browser,
//                                       const CefString& mimeType,
//                                       const CefString& fileName,
//                                       int64 contentLength,
//                                       CefRefPtr<CefDownloadHandler>& handler) {
//  REQUIRE_UI_THREAD();
//
//  // Create the handler for the file download.
//  handler = CreateDownloadHandler(this, fileName);
//
//  // Close the browser window if it is a popup with no other document contents.
//  if (browser->IsPopup() && !browser->HasDocument())
//    browser->CloseBrowser();
//
//  return true;
//}

void ClientHandler::OnNavStateChange(CefRefPtr<CefBrowser> browser,
                                     bool canGoBack,
                                     bool canGoForward) {
  REQUIRE_UI_THREAD();
  if(isBrowser_OnNavStateChange==false)return;
  if(this->pWindow){
	  value eventArgs=alloc_empty_object();
	  alloc_field(eventArgs,_id_x,alloc_bool(canGoBack));
	  alloc_field(eventArgs,_id_y,alloc_bool(canGoForward));
	  onBrowserEvent(this->pWindow,"onNavStateChange",eventArgs);
  }
}


void ClientHandler::OnContentsSizeChange(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    int width,
									int height){
	  REQUIRE_UI_THREAD();
	  if(isBrowser_OnContentsSizeChange==false)return;
	  if(this->pWindow){
		  value eventArgs=alloc_empty_object();
		  alloc_field(eventArgs,_id_x,alloc_bool(frame->IsMain()));
	      if(frame->IsMain()){
			  alloc_field(eventArgs,_id_y,alloc_string(""));
		  }else{
			  alloc_field(eventArgs,_id_y,alloc_string(std::string(frame->GetName()).c_str()));
		  }
		  alloc_field(eventArgs,_id_width,alloc_int(width));
		  alloc_field(eventArgs,_id_height,alloc_int(height));
		  onBrowserEvent(this->pWindow,"onContentsSizeChange",eventArgs);
	  }
}

//bool ClientHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser,
//                                     const CefString& message,
//                                     const CefString& source,
//                                     int line) {
//  REQUIRE_UI_THREAD();
//
//  return false;
//}

//void ClientHandler::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser,
//                                         CefRefPtr<CefFrame> frame,
//                                         CefRefPtr<CefDOMNode> node) {
//  REQUIRE_UI_THREAD();
//
//  // Set to true if a form element has focus.
//  m_bFormElementHasFocus = (node.get() && node->IsFormControlElement());
//}

//bool ClientHandler::OnKeyEvent(CefRefPtr<CefBrowser> browser,
//                               KeyEventType type,
//                               int code,
//                               int modifiers,
//                               bool isSystemKey,
//                               bool isAfterJavaScript) {
//  REQUIRE_UI_THREAD();
//  return false;
//}

//bool ClientHandler::GetPrintHeaderFooter(CefRefPtr<CefBrowser> browser,
//                                         CefRefPtr<CefFrame> frame,
//                                         const CefPrintInfo& printInfo,
//                                         const CefString& url,
//                                         const CefString& title,
//                                         int currentPage,
//                                         int maxPages,
//                                         CefString& topLeft,
//                                         CefString& topCenter,
//                                         CefString& topRight,
//                                         CefString& bottomLeft,
//                                         CefString& bottomCenter,
//                                         CefString& bottomRight) {
//  REQUIRE_UI_THREAD();
//
//  // Place the page title at top left
//  topLeft = title;
//  // Place the page URL at top right
//  topRight = url;
//
//  // Place "Page X of Y" at bottom center
//  std::stringstream strstream;
//  strstream << "Page " << currentPage << " of " << maxPages;
//  bottomCenter = strstream.str();
//
//  return false;
//}

void ClientHandler::OnContextCreated(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     CefRefPtr<CefV8Context> context) {
    REQUIRE_UI_THREAD();
	// Retrieve the context's window object.
	CefRefPtr<CefV8Value> window = context->GetGlobal();

	// Create an instance of my CefV8Handler object.
	// In this case it's this object, and content will be executed in bool ClientHandler::Execute(...)
	CefRefPtr<CefV8Handler> handler = this;

	// Create a function.
	CefRefPtr<CefV8Value> function = CefV8Value::CreateFunction("call", handler);

	// Create a new object
	CefRefPtr<CefV8Value> extern_obj = CefV8Value::CreateObject(NULL);

	// Add the object to windows JS: window.interface
	window->SetValue("extern", extern_obj, V8_PROPERTY_ATTRIBUTE_NONE);

	// Add the function to the object
	extern_obj->SetValue("call", function, V8_PROPERTY_ATTRIBUTE_NONE);
	if(isBrowser_OnContextCreated==false)return;
	if (this->pWindow&&frame.get()) {

		value eventArgs=alloc_empty_object();
		alloc_field(eventArgs,_id_x,alloc_bool(frame->IsMain()));
		if(frame->IsMain()){
			alloc_field(eventArgs,_id_y,alloc_string(""));
		}else{
			alloc_field(eventArgs,_id_y,alloc_string(std::string(frame->GetName()).c_str()));
		}
		onBrowserEvent(this->pWindow,"onContextCreated",eventArgs);
	}
}

// Execute with the specified argument list and return value.  Return true if
  // the method was handled.
bool ClientHandler::Execute(const CefString& name,
                       CefRefPtr<CefV8Value> object,
                       const CefV8ValueList& arguments,
                       CefRefPtr<CefV8Value>& retval,
                       CefString& exception) {
		REQUIRE_UI_THREAD();
	   if(pWindow==NULL)return false;
	   if(name=="call"){
			int argSize = arguments.size();
			if(argSize<1|| !arguments[0]->IsString())return false;
			std::string func_name=std::string(arguments[0]->GetStringValue());
			value retHxValue;
			if(argSize==1)
			{
				retHxValue=executeJsCallBack(pWindow,func_name.c_str(),alloc_null());
			}else
			{
				//::gc_safe_point();
				value args_arr=alloc_array(argSize-1);
				for(int i=1;i<argSize;++i){
					//::gc_safe_point();
					val_array_set_i(args_arr,i-1,V8ValueToHxValue(arguments[i]));
				}
				retHxValue=executeJsCallBack(pWindow,func_name.c_str(),args_arr);
			}
			if(val_is_null(retHxValue))
			{
				retval=CefV8Value::CreateNull();
			}else
			{
				retval=(CefV8Value*)hxValueToV8Value(retHxValue);
			}
			return true;
	   }
    return false;
  }


//bool ClientHandler::OnDragStart(CefRefPtr<CefBrowser> browser,
//                                CefRefPtr<CefDragData> dragData,
//                                DragOperationsMask mask) {
//  REQUIRE_UI_THREAD();
//  return false;
//}
//
//bool ClientHandler::OnDragEnter(CefRefPtr<CefBrowser> browser,
//                                CefRefPtr<CefDragData> dragData,
//                                DragOperationsMask mask) {
//  REQUIRE_UI_THREAD();
//
//  // Forbid dragging of link URLs.
//  if (dragData->IsLink())
//    return true;
//
//  return false;
//}

//bool ClientHandler::OnBeforeScriptExtensionLoad(
//    CefRefPtr<CefBrowser> browser,
//    CefRefPtr<CefFrame> frame,
//    const CefString& extensionName) {
//  return false;
//}

void ClientHandler::OnRequestGeolocationPermission(
      CefRefPtr<CefBrowser> browser,
      const CefString& requesting_url,
      int request_id,
      CefRefPtr<CefGeolocationCallback> callback) {
  // Allow geolocation access from all websites.
  callback->Continue(true);
}

//void ClientHandler::NotifyDownloadComplete(const CefString& fileName) {
//  SetLastDownloadFile(fileName);
//  SendNotification(NOTIFY_DOWNLOAD_COMPLETE);
//}

//void ClientHandler::NotifyDownloadError(const CefString& fileName) {
//  SetLastDownloadFile(fileName);
//  SendNotification(NOTIFY_DOWNLOAD_ERROR);
//}

void ClientHandler::SetMainWindow(void* w) {
  AutoLock lock_scope(this);
  pWindow = w;
}

void ClientHandler::SetMainHwnd(CefWindowHandle hwnd){
	AutoLock lock_scope(this);
	m_MainHwnd=hwnd;
}

//void ClientHandler::AddDOMVisitor(const std::string& path,
//                                  CefRefPtr<CefDOMVisitor> visitor) {
//  AutoLock lock_scope(this);
//  DOMVisitorMap::iterator it = m_DOMVisitors.find(path);
//  if (it == m_DOMVisitors.end())
//    m_DOMVisitors.insert(std::make_pair(path, visitor));
//  else
//    it->second = visitor;
//}
//
//CefRefPtr<CefDOMVisitor> ClientHandler::GetDOMVisitor(const std::string& path) {
//  AutoLock lock_scope(this);
//  DOMVisitorMap::iterator it = m_DOMVisitors.find(path);
//  if (it != m_DOMVisitors.end())
//    return it->second;
//  return NULL;
//}


bool ClientHandler::OnBeforePopup(CefRefPtr<CefBrowser> parentBrowser,
                                  const CefPopupFeatures& popupFeatures,
                                  CefWindowInfo& windowInfo,
                                  const CefString& url,
                                  CefRefPtr<CefClient>& client,
                                  CefBrowserSettings& settings) {
  REQUIRE_UI_THREAD();
  if(isBrowser_OnBeforePopup==false)return false;
 if(this->pWindow){
	 value result=onBrowserEvent(this->pWindow,"onBeforePopup",alloc_string(std::string(url).c_str()));
	 bool b_ret=val_bool(result);
	 return !b_ret;
 }
  return false;
}

bool ClientHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame,
                              CefRefPtr<CefRequest> request,
                              NavType navType,
							  bool isRedirect){
	REQUIRE_UI_THREAD();
	if(isBrowser_OnBeforeBrowse==false)return false;
	if(this->pWindow){
		value eventArgs=alloc_empty_object();
	   alloc_field(eventArgs,_id_x,alloc_bool(frame->IsMain()));
	   if(frame->IsMain()){
			alloc_field(eventArgs,_id_y,alloc_string(""));
		}else{
			alloc_field(eventArgs,_id_y,alloc_string(std::string(frame->GetName()).c_str()));
		}
	   alloc_field(eventArgs,_id_url,alloc_string(std::string(request->GetURL()).c_str()));
	   alloc_field(eventArgs,_id_width,alloc_int(int(navType)));
	   //alloc_field(eventArgs,_id_linkUrl,alloc_abstract(gObjectKind,request.get()));
	   value result=onBrowserEvent(this->pWindow,"onBeforeBrowse",eventArgs);
	   bool b_ret=val_bool(result);
	   return !b_ret;
	}
	return false;
}
void ClientHandler::OnResourceLoaded(const char* mime,const char* url,const char* redirectUrl,int status,unsigned char * bytes,bool isCancel,int bytesLen){
	//printf("ClientHandler::OnResourceLoaded\n");
	IOData* data=NULL;
	if(mime!=NULL||redirectUrl!=NULL||status!=0||bytes!=NULL||isCancel){
		data=new IOData();
		data->bytes=bytes;
		data->mime=mime;
		data->redirectUrl=redirectUrl;
		data->status=status;
		data->isCancel=isCancel;
		data->bytesLen=bytesLen;
	}
	QueueData* queueData=new QueueData();
	queueData->url=url;
	queueData->data=data;
	//printf("to push: %s\n",url);
	int r=resQueue.Push(queueData);
	//printf("resQueue.Push:%d\n",r );
	if(r==1){
		delete queueData;
		/*while(resQueue.Push(data)==1){
			printf("no push\n");
			Sleep(5);
		}*/
		if(this->pWindow){
			//printf("post message...\n");
			 char url_var[255];
			 sprintf_s(url_var, "%s", url);
			 ::OnBeforeResourceLoad(pWindow,url_var,1);
		}
	}
	//printf("push end:%d...\n",r);
}
bool ClientHandler::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefRequest> request,
                                     CefString& redirectUrl,
                                     CefRefPtr<CefStreamReader>& resourceStream,
                                     CefRefPtr<CefResponse> response,
                                     int loadFlags) {
  //printf(" cur io thread... \n");
  REQUIRE_IO_THREAD();
  if(isBrowser_OnBeforeResourceLoad==false)return false;
  if(this->pWindow){
	 // printf("url:%s:\n",std::string(request->GetURL()).c_str());
	  bool pos=::getIsFilterPath(std::string(request->GetURL()).c_str());
	  //printf("pos:%d\n",pos);
	  if(pos==false){
		 return false;
	  }
	  ::OnBeforeResourceLoad(pWindow,std::string(request->GetURL()).c_str(),0);
	  QueueData* resData=resQueue.Find(std::string(request->GetURL()).c_str());
	  //printf("finding...\n");
	  if(resData==NULL)return false;
	 // printf("find is null \n");
	  while(resData!=NULL&&resData->url==NULL){
		 // printf("resData->data is null \n");
		  delete resData;
		  Sleep(5);
		  resData=resQueue.Find(std::string(request->GetURL()).c_str());
		  if(resData==NULL){
			  return false;
		  }
	  } 
	  if(resData==NULL){
		  return false;
	   }
	  if(resData->data==NULL){
		  delete resData;
		  return false;
	  }
	  if(resData->data->redirectUrl!=NULL){
		  redirectUrl=CefString(redirectUrl);
		  delete resData;
		  return false;
	  }
	  if(resData->data->isCancel){
		  delete resData;
		  return true;
	  }
	  
	  if(resData->data->bytes!=NULL){
		  resourceStream=CefStreamReader::CreateForData(resData->data->bytes,resData->data->bytesLen);
		 // printf("%s\n",resData->data->mime);
		  if(resData->data->mime!=NULL)response->SetMimeType(CefString(resData->data->mime));
		  int status=resData->data->status;
		  if(status==0)status=200;
		  response->SetStatus(status);
	  }
	  // printf("resData->data is not null \n");
	  delete resData;
	  return false;
  }
  return false;
}

/*
void ClientHandler::OnResourceResponse(CefRefPtr<CefBrowser> browser,
                                  const CefString& url,
                                  CefRefPtr<CefResponse> response,
                                  CefRefPtr<CefContentFilter>& filter) {
    CefResponse::HeaderMap headers;
    response->GetHeaderMap(headers);
	CefResponse::HeaderMap::const_iterator it1;
	
	for(it1 = headers.begin(); it1 != headers.end(); ++it1) {
		printf("first:%s\n",std::string(it1->first).c_str());
		printf("second:%s\n",std::string(it1->second).c_str());
	}
}
*/
void ClientHandler::OnAddressChange(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    const CefString& url) {
  REQUIRE_UI_THREAD();
  if(isBrowser_OnAddressChange==false)return;
  if(this->pWindow&&frame.get()){
	   value eventArgs=alloc_empty_object();
	   alloc_field(eventArgs,_id_x,alloc_bool(frame->IsMain()));
	   if(frame->IsMain()){
			alloc_field(eventArgs,_id_y,alloc_string(""));
		}else{
			alloc_field(eventArgs,_id_y,alloc_string(std::string(frame->GetName()).c_str()));
		}
	   alloc_field(eventArgs,_id_url,alloc_string(std::string(url).c_str()));
	   onBrowserEvent(this->pWindow,"onAddressChange",eventArgs);
  }
}

void ClientHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                  const CefString& title) {
  REQUIRE_UI_THREAD();
  CefWindowHandle hwnd = browser->GetWindowHandle();
  if (m_BrowserHwnd!=NULL&&m_BrowserHwnd == hwnd){
	  hwnd = GetParent(hwnd);
	  if(isBrowser_OnTitleChange&&this->pWindow){
		  value title_str=onBrowserEvent(this->pWindow,"onTitleChange",alloc_string(std::string(title).c_str()));
		  if(val_is_null(title_str)){
			  SetWindowText(hwnd, std::wstring(title).c_str());
		  }else{
			SetWindowText(hwnd, val_wstring(title_str));
		  }
	  }else{
		  SetWindowText(hwnd, std::wstring(title).c_str());
	  }
		
  }else{
		SetWindowText(hwnd,std::wstring(title).c_str());
  }
}


bool ClientHandler::OnBeforeMenu(CefRefPtr<CefBrowser> browser,
								 const CefMenuInfo& menuInfo){
	 REQUIRE_UI_THREAD();
	 if(isBrowser_OnBeforeMenu==false){
		if(menuInfo.typeFlags&0x20){
			return false;
		}
		return true;
	 }else if(this->pWindow){
	    value eventArgs=alloc_empty_object();
		alloc_field(eventArgs,_id_x,alloc_int(menuInfo.x));
		alloc_field(eventArgs,_id_y,alloc_int(menuInfo.y));
		
		alloc_field(eventArgs,_id_frameUrl,alloc_string(std::string(CefString(&menuInfo.frameUrl)).c_str())); //frameUrl
		alloc_field(eventArgs,_id_imageUrl,alloc_string(std::string(CefString(&menuInfo.imageUrl)).c_str()));//imageUrl
		alloc_field(eventArgs,_id_linkUrl,alloc_string(std::string(CefString(&menuInfo.linkUrl)).c_str()));//linkUrl
		alloc_field(eventArgs,_id_selectionText,alloc_string(std::string(CefString(&menuInfo.selectionText)).c_str()));//selectionText
		alloc_field(eventArgs,_id_misspelledWord,alloc_string(std::string(CefString(&menuInfo.misspelledWord)).c_str()));//misspelledWord
		alloc_field(eventArgs,_id_securityInfo,alloc_string(std::string(CefString(&menuInfo.securityInfo)).c_str()));//securityInfo
		alloc_field(eventArgs,_id_typeFlags,alloc_int(menuInfo.typeFlags));//typeFlags
		alloc_field(eventArgs,_id_editFlags,alloc_int(menuInfo.editFlags));//editFlags
		value result=onBrowserEvent(this->pWindow,"onBeforeMenu",eventArgs);
		bool b_ret=val_bool(result);
	    return !b_ret;
	 }
	 return false;
}
void ClientHandler::GetMenuLabel(CefRefPtr<CefBrowser> browser,
                            MenuId menuId,
							CefString& label){
	REQUIRE_UI_THREAD();
	if(label=="Undo"){
		label=c2w("撤销");
	}else if(label=="Redo"){
		label=c2w("重做");
	}else if(label=="Cut"){
		label=c2w("剪切");
	}else if(label=="Copy"){
		label=c2w("复制");
	}else if(label=="Paste"){
		label=c2w("粘贴");
	}else if(label=="Delete"){
		label=c2w("删除");
	}else if(label=="Select All"){
		label=c2w("全选");
	}else if(label=="Print"){
		label=c2w("打印");
	}
}

 bool ClientHandler::OnJSAlert(CefRefPtr<CefBrowser> browser,
                         CefRefPtr<CefFrame> frame,
						 const CefString& message) {
	REQUIRE_UI_THREAD();
	if(isBrowser_OnJSAlert==false)return false;
	if(this->pWindow){
	   value eventArgs=alloc_empty_object();
	   alloc_field(eventArgs,_id_x,alloc_bool(frame->IsMain()));
	   if(frame->IsMain()){
			alloc_field(eventArgs,_id_y,alloc_string(""));
		}else{
			alloc_field(eventArgs,_id_y,alloc_string(std::string(frame->GetName()).c_str()));
		}
	   alloc_field(eventArgs,_id_url,alloc_string(std::string(message).c_str()));
	   value result=onBrowserEvent(this->pWindow,"onJSAlert",eventArgs);
	   return !val_bool(result);
	}
	return false;
 }
 bool ClientHandler::OnJSConfirm(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           const CefString& message,
						   bool& retval){
	REQUIRE_UI_THREAD();
	if(isBrowser_OnJSConfirm==false)return false;
	if(this->pWindow){
		value eventArgs=alloc_empty_object();
	   alloc_field(eventArgs,_id_x,alloc_bool(frame->IsMain()));
	   if(frame->IsMain()){
			alloc_field(eventArgs,_id_y,alloc_string(""));
		}else{
			alloc_field(eventArgs,_id_y,alloc_string(std::string(frame->GetName()).c_str()));
		}
	   alloc_field(eventArgs,_id_url,alloc_string(std::string(message).c_str()));
	   value result=onBrowserEvent(this->pWindow,"onJSConfirm",eventArgs);
	   bool b_ret=val_bool(result);
	   retval=b_ret;
	   return true;
	}
	return false;
 }
 /*bool ClientHandler::OnJSPrompt(CefRefPtr<CefBrowser> browser,
                          CefRefPtr<CefFrame> frame,
                          const CefString& message,
                          const CefString& defaultValue,
                          bool& retval,
						  CefString& result) {
	REQUIRE_UI_THREAD();
	if(isBrowser_OnJSPrompt==false)return false;
	if(this->pWindow){
		value eventArgs=alloc_empty_object();
	   alloc_field(eventArgs,_id_x,alloc_bool(frame->IsMain()));
	   if(frame->IsMain()){
			alloc_field(eventArgs,_id_y,alloc_string(""));
		}else{
			alloc_field(eventArgs,_id_y,alloc_string(std::string(frame->GetName()).c_str()));
		}
	   alloc_field(eventArgs,_id_url,alloc_string(std::string(message).c_str()));
	   alloc_field(eventArgs,_id_width,alloc_string(std::string(defaultValue).c_str()));
	   value result=onBrowserEvent(this->pWindow,"onJSPrompt",eventArgs);
	   bool b_ret=val_bool(result);
	   retval=b_ret;
	   return true;
	}
	return false;
 }*/
void ClientHandler::CloseMainWindow() {
  ::PostMessage(m_MainHwnd, WM_CLOSE, 0, 0);
}
