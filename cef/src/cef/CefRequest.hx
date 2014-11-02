package cef;
import cpp.Lib;

/**
 * ...
 * @author wangjian QQ:421924396
 */
//enum cef_weburlrequest_flags_t {
  //WUR_FLAG_NONE = 0,
  //WUR_FLAG_SKIP_CACHE = 0x1,
  //WUR_FLAG_ALLOW_CACHED_CREDENTIALS = 0x2,
  //WUR_FLAG_ALLOW_COOKIES = 0x4,
  //WUR_FLAG_REPORT_UPLOAD_PROGRESS = 0x8,
  //WUR_FLAG_REPORT_LOAD_TIMING = 0x10,
  //WUR_FLAG_REPORT_RAW_HEADERS = 0x20
//};
class CefRequest
{
	//public var headerMap:Hash<String>;
	//public var requestFlags:Int; //
	public var url(get_url,set_url):String;
	public var method(get_method,set_method):String;
	private var _handle:Dynamic;
	function new(t_handle) 
	{
		_handle = t_handle;
	}
	function get_url():String {
		return request_geturl(_handle);
	}
	function set_url(v:String):String {
		request_seturl(_handle, v);
		return v;
	}
	function get_method():String {
		return request_getmethod(_handle);
	}
	function set_method(v:String):String {
		request_setmethod(_handle, v);
		return v;
	}
	public function getHeader(name:String):String {
		return request_getheader(_handle,name);
	}
	public function setHeader(name:String, v:String):Void {
		request_setheader(_handle, name, v);
	}
	public function getFirstPartyForCookies():String {
		return request_getfirstcookie(_handle);
	}
	//PostData
	//firstPartyForCookies
	static var request_getheader = Lib.load("cefhx", "request_getheader", 2);
	static var request_setheader = Lib.load("cefhx", "request_setheader", 3);
	static var request_getfirstcookie = Lib.load("cefhx", "request_getfirstcookie", 1);
	static var request_geturl = Lib.load("cefhx", "request_geturl", 1);
	static var request_seturl = Lib.load("cefhx", "request_seturl", 2);
	static var request_getmethod = Lib.load("cefhx", "request_getmethod", 1);
	static var request_setmethod= Lib.load("cefhx", "request_setmethod", 2);
}