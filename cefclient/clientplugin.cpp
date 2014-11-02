// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "cefclient/clientplugin.h"
//#include "cefclient/ExternalInterface.h"
#if defined(OS_WIN)

#include <windows.h>
namespace {

// Client plugin window implementation.
class ClientPlugin {
 public:
  ClientPlugin(int16 _argc, char* _argn[], char* _argv[] )
      :_parent_hwnd(NULL),plug_hwnd(NULL),host_hwnd(NULL) { //hwnd_(NULL), plugId(NULL)
	//argc=_argc;
	//for (int i = 0; i < _argc; ++i) {
	//	if (strcmp(_argn[i], "src") == 0) {
	//		//plugId = _argv[i];
	//		//break;
	//	}
	// }
  }

  ~ClientPlugin() {
	  DWORD pid;
	  ::GetWindowThreadProcessId(plug_hwnd,&pid);
	  //printf("pid:%d",pid);
	  HANDLE p_hand;
	  p_hand=OpenProcess(PROCESS_TERMINATE,false,pid);//|SYNCHRONIZE
	  if(p_hand){
		  if(TerminateProcess(p_hand,0)){
			//printf("closed");
		  }else{
			//printf("no close");
		  }
	  }else{
		 // printf("open err:%d", GetLastError());
	  }
	_parent_hwnd=NULL;
	plug_hwnd=NULL;
	host_hwnd=NULL;
  }
  //int16 argc;
  //char **argn; 
  //char** argv;
  HWND _parent_hwnd;
  HWND plug_hwnd;
  HWND host_hwnd;
 // int pid;

  void Initialize(HWND parent_hwnd) {
    if (_parent_hwnd != NULL)
      return;
	_parent_hwnd=parent_hwnd;
	HWND t_hwnd=::GetParent(_parent_hwnd);
	HWND tp_hwnd=::GetParent(t_hwnd);
	host_hwnd=::GetParent(tp_hwnd);
	SetWindowLongPtr(_parent_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	SetWindowLongPtr(_parent_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&PluginWndProc));
	char sdl_var[30];
	sprintf_s(sdl_var, "%I64d", (__int64)_parent_hwnd);//Ö÷´°¿Ú¾ä±ú
	try {
		::SendMessage( host_hwnd,0x2327,-1,(LPARAM)sdl_var);
	}catch (std::exception & e) {
		printf(e.what());
	} catch (...) {
		printf("plugin:Unknown unhandled exception thrown");
	}
  }

  // Plugin window procedure.
  static LRESULT CALLBACK PluginWndProc(HWND hwnd_, UINT message, WPARAM wParam,
                                LPARAM lParam) {
    ClientPlugin* plugin =
        reinterpret_cast<ClientPlugin*>(GetWindowLongPtr(hwnd_, GWLP_USERDATA));
	if(plugin==NULL)return DefWindowProc(hwnd_, message, wParam, lParam);
	
    switch (message) {
	  case WM_SIZE:
		  {
			LRESULT ret=DefWindowProc(hwnd_, message, wParam, lParam);
			if(plugin->plug_hwnd!=NULL)::SendMessage(plugin->plug_hwnd,WM_SIZE,wParam,lParam);
			return ret;
		  }
	  case WM_COPYDATA:
		  {
			COPYDATASTRUCT *pCopyData = (COPYDATASTRUCT*)lParam;
			if((int)(pCopyData->dwData)==-9999){
				 plugin->plug_hwnd=(HWND)_atoi64((char*)pCopyData->lpData);
			}
			::SendMessage( plugin->host_hwnd,0x2327,pCopyData->dwData,(LPARAM)pCopyData->lpData);
		}
		return 1;
    }

    return DefWindowProc(hwnd_, message, wParam, lParam);
  }
};

NPError NPP_NewImpl(NPMIMEType plugin_type, NPP instance, uint16 mode,
                    int16 argc, char* argn[], char* argv[],
                    NPSavedData* saved) {
  if (instance == NULL)
    return NPERR_INVALID_INSTANCE_ERROR;
   
  ClientPlugin* plugin = new ClientPlugin(argc,argn,argv);
  instance->pdata = reinterpret_cast<void*>(plugin);

  return NPERR_NO_ERROR;
}

NPError NPP_DestroyImpl(NPP instance, NPSavedData** save) {
  ClientPlugin* plugin = reinterpret_cast<ClientPlugin*>(instance->pdata);
  if (plugin)
    delete plugin;

  return NPERR_NO_ERROR;
}

NPError NPP_SetWindowImpl(NPP instance, NPWindow* window_info) {
  if (instance == NULL)
    return NPERR_INVALID_INSTANCE_ERROR;

  if (window_info == NULL)
    return NPERR_GENERIC_ERROR;

  ClientPlugin* plugin = reinterpret_cast<ClientPlugin*>(instance->pdata);
  HWND parent_hwnd = reinterpret_cast<HWND>(window_info->window);
  plugin->Initialize(parent_hwnd);
  return NPERR_NO_ERROR;
}

}  // namespace

NPError API_CALL NP_ClientGetEntryPoints(NPPluginFuncs* pFuncs) {
  pFuncs->newp = NPP_NewImpl;
  pFuncs->destroy = NPP_DestroyImpl;
  pFuncs->setwindow = NPP_SetWindowImpl;
  return NPERR_NO_ERROR;
}

NPError API_CALL NP_ClientInitialize(NPNetscapeFuncs* pFuncs) {
  return NPERR_NO_ERROR;
}

NPError API_CALL NP_ClientShutdown(void) {
  return NPERR_NO_ERROR;
}
#endif  // OS_WIN
