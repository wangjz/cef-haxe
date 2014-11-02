package cef;
import cpp.Lib;
//import cpp.rtti.FieldIntegerLookup;
/**
 * ...
 * @author wangjian
 */
//enum cef_thread_id_t {
  //TID_UI      = 0,
  //TID_IO      = 1,
  //TID_FILE    = 2,
//};

class CefApp //implements FieldIntegerLookup
{
	public static var naviteWindows:Array<CefWindow>;
	private static var sIsInit = false;
	private static var isQuit:Bool = false;
	public static var onQuit:Void->Void;
	static var filterResourcePaths:Array<String>;
	//public static inline var Thread_UI:Int = 0;
	//public static inline var Thread_IO:Int = 1;
	//public static inline var Thread_FILE:Int = 2;
	//private static var schemesList:Array<Dynamic>;
	//GetResourceBundleHandler
	//GetProxyHandler
	//CefGetWebPluginInfo
	//CefGetWebPluginCount
	//CefCurrentlyOn
	//CefAddCrossOriginWhitelistEntry
	//CefGetGeolocation
	//CefRemoveCrossOriginWhitelistEntry
	//CefClearCrossOriginWhitelist
	//CefClearSchemeHandlerFactories
	//
	public static function main(initFunc:Void->Void,?cefSetting:CefSettings=null):Void
	{
		if (sIsInit)
		{
			throw("cef.App.main called multiple times. This function is automatically called by the project code.");
		}
		sIsInit = true;
		naviteWindows = [];
		cef_app_setglobalapp(CefApp);
		cef_app_main(
			function() {
				initFunc();
			},cefSetting
		);
	}
	
	public static function quit():Void
	{
		if (isQuit) return;
		isQuit = true;
		if (onQuit != null) onQuit();
		cef_app_quit();
	}
	public static function getSystemMetrics(inx:Int):Int {
		return cef_getsystemmetrics(inx);
	}
	public static function setOSModalLoop(v:Bool):Void
	{
		cef_setosmodalloop(v);
	}
	public static function unicode2Ansi(_str):String {
		return cef_unicode2ansi(_str);
	}
	public static function ansi2Unicode(_str):String {
		return cef_ansi2unicode(_str);
	}
	//public static function getWebPluginInfo(pluginName):{name:String,path:String,version:String,description:String} {
		//return null;
	//}
	//public static function getWebPluginInfoByInx(inx:Int):{name:String,path:String,version:String,description:String} {
		//return null;
	//}
	//public static function getWebPluginCount():Int {
		//return 0;
	//}
	/**
	 * 注册实例名称，用于只运行一次,返回false 表示已经有实例在运行
	 * @param	instanceName
	 * @return
	 */
	public static function registeAppInstance(instanceName:String):Bool{
		return cef_app_registeinstance(instanceName);
	}
	
	public static function closeProcess(processName:String):Bool {
		return cef_app_closeprocess(processName);
	}
	public static function closeProcessById(processId:Int):Bool {
		return cef_app_closeprocessbyid(processId);
	}
	/**
	 * 注册系统热键
	 * @param	window 响应该热键的窗口
	 * @param	hotKeyName 该热键的唯一标识 
	 * @param	fsModifiers 该热键的辅助按键 
	 * @param	vk 该热键的键值 
	 * @return -1注册失败
	 */
	public static function registerHotKey(window:CefWindow, hotKeyName:String, fsModifiers:Int, vk:Int):Int {
		if (window == null) return -1;
		if (hotKeyName == null) return -1;
		return cef_app_registerhotkey(untyped window._handle, hotKeyName, fsModifiers, vk);
	}
	
	public static function unRegisterHotKey(window:CefWindow, hotKeyName:String):Bool {
		if (window == null) return false;
		return cef_app_unregisterhotkey(untyped window._handle, hotKeyName);
	}
	/**
	 * 进程间通信
	 */ 
	public static function sendMessage(hwnd:Dynamic, msg:String,msgType:Int=0):Void {
		cef_app_sendmessage(hwnd, msg,msgType);
	}
	
	public static function getHwndStr(window:CefWindow):String {
		return cef_app_gethwndstr(untyped window._handle);
	}
	//
	public static function isCurrentThreadOn(threadId:Int):Bool {
		return cef_app_currentlyon(threadId);
	}
	
	public static function setFilterResourcePath(paths:Array<String>):Void {
		if (paths == null || paths.length == 0) return;
		var in_paths:Array<String> = [];
		for (i in 0...paths.length) {
			var t_path = paths[i];
			if (t_path != null && t_path.length > 0) {
				var url_path = StringTools.urlEncode(t_path);
				url_path=StringTools.replace(url_path, "%3A", ":");
				url_path = StringTools.replace(url_path, "%2F", "/");
				url_path = StringTools.replace(url_path, "%28", "(");
				url_path = StringTools.replace(url_path, "%29", ")");
				in_paths.push(url_path);
			}
		}
		filterResourcePaths = in_paths;
		cef_app_setfilterpath(in_paths);
	}
	/*
	#if plug
	public static function runNdllMethod(dllPath:String, _method:String, ?args:Array<Dynamic>=null):Dynamic {
		var argsCount:Int = (args == null?0:args.length);
		var ndll_method:Dynamic = Lib.load(dllPath, _method, argsCount);
		if (ndll_method != null) {
			if (argsCount < 6) {
				switch(argsCount) {
					case 0:
						return ndll_method();
					case 1:
						return ndll_method(args[0]);
					case 2:
						return ndll_method(args[0],args[1]);
					case 3:
						return ndll_method(args[0],args[1],args[2]);
					case 4:
						return ndll_method(args[0],args[1],args[2],args[3]);
					case 5:
						return ndll_method(args[0],args[1],args[2],args[3],args[4]);
				}
			}else {
				return ndll_method(args);
			}
		}
		return null;
	}
	public static function loadPlug(plugPath:String, plugArgs:String):Int {
		return syslite.win.Tools.createProcess(plugPath, plugArgs, "", false, false);
	}
	#end
	*/
	static var cef_app_main = cpp.Lib.load("cefhx", "cef_app_main", 2);
	static var cef_app_quit = cpp.Lib.load("cefhx", "cef_app_quit", 0);
	static var cef_getsystemmetrics = cpp.Lib.load("cefhx", "cef_getsystemmetrics", 1);
	static var cef_unicode2ansi = cpp.Lib.load("cefhx", "cef_unicode2ansi", 1);
	static var cef_ansi2unicode = cpp.Lib.load("cefhx", "cef_ansi2unicode", 1);
	static var cef_setosmodalloop = cpp.Lib.load("cefhx", "cef_setosmodalloop", 1);
	static var cef_app_registerhotkey = cpp.Lib.load("cefhx", "cef_app_registerhotkey", 4);
	static var cef_app_unregisterhotkey = cpp.Lib.load("cefhx", "cef_app_unregisterhotkey", 2);
	static var cef_app_closeprocess = Lib.load("cefhx", "cef_app_closeprocess", 1);
	static var cef_app_registeinstance = Lib.load("cefhx", "cef_app_registeinstance", 1);
	static var cef_app_sendmessage = Lib.load("cefhx", "cef_app_sendmessage", 3);
	static var cef_app_closeprocessbyid = Lib.load("cefhx", "cef_app_closeprocessbyid", 1);
	static var cef_app_gethwndstr = Lib.load("cefhx", "cef_app_gethwndstr", 1);
	static var cef_app_currentlyon = Lib.load("cefhx", "cef_app_currentlyon", 1);
	static var cef_app_setfilterpath = Lib.load("cefhx", "cef_app_setfilterpath", 1);
	static var cef_app_setglobalapp = Lib.load("cefhx", "cef_app_setglobalapp", 1);
}

//
  ///
  // Return the handler for resource bundle events. If
  // CefSettings.pack_loading_disabled is true a handler must be returned. If no
  // handler is returned resources will be loaded from pack files. This method
  // is called on multiple threads.
  ///
  //virtual CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler() {
    //return NULL;
  //}
//
  ///
  // Return the handler for proxy events. If not handler is returned the default
  // system handler will be used. This method is called on the IO thread.
  ///
  //virtual CefRefPtr<CefProxyHandler> GetProxyHandler() {
    //return NULL;
  //}
  
  
  
  
  //enum cef_handler_errorcode_t {
  //ERR_FAILED = -2,
  //ERR_ABORTED = -3,
  //ERR_INVALID_ARGUMENT = -4,
  //ERR_INVALID_HANDLE = -5,
  //ERR_FILE_NOT_FOUND = -6,
  //ERR_TIMED_OUT = -7,
  //ERR_FILE_TOO_BIG = -8,
  //ERR_UNEXPECTED = -9,
  //ERR_ACCESS_DENIED = -10,
  //ERR_NOT_IMPLEMENTED = -11,
  //ERR_CONNECTION_CLOSED = -100,
  //ERR_CONNECTION_RESET = -101,
  //ERR_CONNECTION_REFUSED = -102,
  //ERR_CONNECTION_ABORTED = -103,
  //ERR_CONNECTION_FAILED = -104,
  //ERR_NAME_NOT_RESOLVED = -105,
  //ERR_INTERNET_DISCONNECTED = -106,
  //ERR_SSL_PROTOCOL_ERROR = -107,
  //ERR_ADDRESS_INVALID = -108,
  //ERR_ADDRESS_UNREACHABLE = -109,
  //ERR_SSL_CLIENT_AUTH_CERT_NEEDED = -110,
  //ERR_TUNNEL_CONNECTION_FAILED = -111,
  //ERR_NO_SSL_VERSIONS_ENABLED = -112,
  //ERR_SSL_VERSION_OR_CIPHER_MISMATCH = -113,
  //ERR_SSL_RENEGOTIATION_REQUESTED = -114,
  //ERR_CERT_COMMON_NAME_INVALID = -200,
  //ERR_CERT_DATE_INVALID = -201,
  //ERR_CERT_AUTHORITY_INVALID = -202,
  //ERR_CERT_CONTAINS_ERRORS = -203,
  //ERR_CERT_NO_REVOCATION_MECHANISM = -204,
  //ERR_CERT_UNABLE_TO_CHECK_REVOCATION = -205,
  //ERR_CERT_REVOKED = -206,
  //ERR_CERT_INVALID = -207,
  //ERR_CERT_END = -208,
  //ERR_INVALID_URL = -300,
  //ERR_DISALLOWED_URL_SCHEME = -301,
  //ERR_UNKNOWN_URL_SCHEME = -302,
  //ERR_TOO_MANY_REDIRECTS = -310,
  //ERR_UNSAFE_REDIRECT = -311,
  //ERR_UNSAFE_PORT = -312,
  //ERR_INVALID_RESPONSE = -320,
  //ERR_INVALID_CHUNKED_ENCODING = -321,
  //ERR_METHOD_NOT_SUPPORTED = -322,
  //ERR_UNEXPECTED_PROXY_AUTH = -323,
  //ERR_EMPTY_RESPONSE = -324,
  //ERR_RESPONSE_HEADERS_TOO_BIG = -325,
  //ERR_CACHE_MISS = -400,
  //ERR_INSECURE_RESPONSE = -501,
//};