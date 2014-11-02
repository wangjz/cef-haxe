package cef;
import cpp.Lib;
//import cpp.rtti.FieldIntegerLookup;

/**
 * ...
 * @author wangjian
 */

class CefFrame //implements FieldIntegerLookup
{
	private var cefWindow:CefWindow;
	private var cefBrowser:CefBrowser;
	public var identifier(default, null):Int;
	public var name(default, null):String;
	public var isMain(default, null):Bool;
	public function new(_cefBrowser:CefBrowser,_cefWindow:CefWindow,_isMain:Bool,_name:String,_identifier:Int) 
	{
		cefBrowser = _cefBrowser;
		cefWindow = _cefWindow;
		isMain = _isMain;
		name = _name;
		identifier = _identifier;
	}
	
	
	
	/**
	 *  Execute undo in this frame.
	 */
	public function undo():Void
	{
		if (cefBrowser.isInit) frame_undo(untyped cefWindow._handle, isMain, name);
	}
	/**
	 *  Execute redo in this frame.
	 */
	public function redo():Void
	{
		if (cefBrowser.isInit) frame_redo(untyped  cefWindow._handle,isMain, name);
	}
	/**
	 * Execute cut in this frame.
	 */
	public function cut():Void
	{
		if (cefBrowser.isInit) frame_cut(untyped  cefWindow._handle,isMain, name);
	}
	/**
	 * Execute copy in this frame.
	 */
	public function copy():Void
	{
		if (cefBrowser.isInit)frame_copy(untyped  cefWindow._handle,isMain, name);
	}
	/**
	 * Execute paste in this frame.
	 */
	public function paste():Void
	{
		if (cefBrowser.isInit)frame_paste(untyped  cefWindow._handle,isMain, name);
	}
	/**
	 * Execute delete in this frame.
	 */
	public function delete():Void
	{
		if (cefBrowser.isInit)frame_delete(untyped  cefWindow._handle,isMain, name);
	}
	
	/**
	 * Execute select all in this frame.
	 */
	public function selectAll():Void
	{
		if (cefBrowser.isInit)frame_selectall(untyped  cefWindow._handle,isMain, name);
	}
	
	/**
	 * Execute printing in the this frame.  The user will be prompted with the
	 * print dialog appropriate to the operating system.
	 */
	public function print():Void
	{
		if (cefBrowser.isInit)frame_print(untyped  cefWindow._handle,isMain, name);
	}
	
	/**
	 * Save this frame's HTML source to a temporary file and open it in the
	 * default text viewing application.
	 */
	public function viewSource():Void
	{
		if (cefBrowser.isInit)frame_viewsource(untyped  cefWindow._handle,isMain, name);
	}
	
	/**
	 * Returns this frame's HTML source as a string. This method should only be
	 * called on the UI thread.
	 * @return
	 */
	public function getSource():String
	{
		if (cefBrowser.isInit) return frame_getsource(untyped  cefWindow._handle, isMain, name);
		return null;
	}
	/**
	 * Returns this frame's display text as a string. This method should only be
	 * called on the UI thread.
	 * @return
	 */
	public function getText():String
	{
		if (cefBrowser.isInit) return frame_gettext(untyped  cefWindow._handle, isMain, name);
		return null;
	}
	
	/**
	 * Load the request represented by the |request| object.
	 */
	//public function loadRequest(request:Dynamic):Void
	//{
		//
	//}
	/**
	 * Load the specified |url|.
	 */
	public function loadURL(url:String):Void
	{
		if (cefBrowser.isInit)frame_loadurl(untyped  cefWindow._handle, isMain, name,url);
	}
	
	/**
	 * Load the contents of |string_val| with the optional dummy target |url|.
	 * @param	string_val
	 * @param	url
	 */
	public function loadString(string_val:String, url:String):Void
	{
		if (cefBrowser.isInit)frame_loadstring(untyped  cefWindow._handle, isMain,name,string_val,url);
	}
	/**
	 * Load the contents of |stream| with the optional dummy target |url|.
	 * @param	stream
	 * @param	url
	 */
	//public function loadStream(stream:Dynamic, url:String):Void
	//{
		//
	//}
	
	/**
	 * / Execute a string of JavaScript code in this frame. The |script_url|
	 * parameter is the URL where the script in question can be found, if any.
	 * The renderer may request this URL to show the developer the source of the
	 * error.  The |start_line| parameter is the base line number to use for error
	 * reporting.
	 * @param	jsCode
	 * @param	scriptUrl
	 * @param	startLine
	 */
	public function executeJavaScript(jsCode:String, ?scriptUrl:String="", ?startLine:Int = 0):Void
	{
		if (cefBrowser.isInit)frame_executejavascript(untyped  cefWindow._handle, isMain,name, jsCode, {_url:scriptUrl, x:startLine});
	}
	
	public function evalJavaScript(jsCode:String):Dynamic
	{
		if (cefBrowser.isInit) return frame_evaljavascript(untyped  cefWindow._handle, isMain, name, jsCode);
		return null;
	}
	
	public function invokeJsFunc(jsFuncName:String,?args:Array<Dynamic>=null):Dynamic
	{
		if (cefBrowser.isInit) return frame_invokejsfunc( untyped  cefWindow._handle, isMain, name, jsFuncName, args);
		return null;
	}
	/**
	 * Returns true if this is the main (top-level) frame.
	 * @return
	 */
	//public function isMain():Bool
	//{
		//return isMain;
	//}
	
	 /**
	  * Returns true if this is the focused frame. This method should only be
	  * called on the UI thread.
	  * @return
	  */
	public function isFocused():Bool
	{
		return frame_isfocused(untyped  cefWindow._handle, isMain, name);
	}
  
  
	/**
	 * Returns the name for this frame. If the frame has an assigned name (for
	 * example, set via the iframe "name" attribute) then that value will be
	 * returned. Otherwise a unique name will be constructed based on the frame
	 * parent hierarchy. The main (top-level) frame will always have an empty name
	 * value.
	 * @return
	 */
	//private function  get_name():String
	//{
		//return name;
		//var _name:String = frame_getname(_handle);
		//return _name;
	//}
	/**
	 * Returns the globally unique identifier for this frame.
	 * @return
	 */
	public function getIdentifier():Int
	{
		if (identifier != -1) return identifier;
		return frame_getidentifier(untyped  cefWindow._handle, isMain, name);
	}
	/**
	 * Returns the parent of this frame or NULL if this is the main (top-level)
	 * frame. This method should only be called on the UI thread.
	 * @return
	 */
	public function getParent():CefFrame
	{
		if (cefBrowser.isInit == false) return null;
		var parent_id = frame_getparentid(untyped  cefWindow._handle, isMain, name);
		if (parent_id == -1) return null;
		untyped for (i in 0...cefBrowser.frames.length) {
			untyped if (cefBrowser.frames[i].identifier == parent_id) return cefBrowser.frames[i];
		}
		var obj = frame_getparentobj(untyped  cefWindow._handle, isMain, name);
		if (obj == null) return null;
		return untyped cefBrowser.initFrame(obj);
	}
	
	//public function getParentIdentifier():Int
	//{
		//return frame_getparentid(_handle);
	//}
	/**
	 * Returns the URL currently loaded in this frame.
	 * @return
	 */
	public function getUrl():String
	{
		return frame_geturl(untyped  cefWindow._handle, isMain, name);
	}
	/**
	 * Returns the browser that this frame belongs to.
	 * @return
	 */
	public function getBrowser():CefBrowser
	{
		return cefBrowser;
	}
	public function getCefWindow():CefWindow
	{
		return cefWindow;
	}
	
	private function destroy():Void
	{
		cefWindow = null;
		cefBrowser = null;
	}
	static var frame_undo:Dynamic = Lib.load("cefhx", "frame_undo", 3);
	static var frame_redo:Dynamic = Lib.load("cefhx", "frame_redo", 3);
	static var frame_cut:Dynamic = Lib.load("cefhx", "frame_cut", 3);
	static var frame_copy:Dynamic = Lib.load("cefhx", "frame_copy", 3);
	static var frame_paste:Dynamic= Lib.load("cefhx", "frame_paste", 3);
	static var frame_delete:Dynamic= Lib.load("cefhx", "frame_delete", 3);
	static var frame_selectall:Dynamic = Lib.load("cefhx", "frame_selectall", 3);
	static var frame_print:Dynamic = Lib.load("cefhx", "frame_print", 3);
	static var frame_viewsource:Dynamic = Lib.load("cefhx", "frame_viewsource", 3);
	static var frame_getsource:Dynamic = Lib.load("cefhx", "frame_getsource",3);
	static var frame_gettext:Dynamic = Lib.load("cefhx", "frame_gettext", 3);
	//static var frame_loadrequest:Dynamic = Lib.load("cefhx", "frame_loadrequest", 2);
	static var frame_loadurl:Dynamic = Lib.load("cefhx", "frame_loadurl", 4);
	static var frame_loadstring:Dynamic = Lib.load("cefhx", "frame_loadstring", 5);
	static var frame_executejavascript:Dynamic = Lib.load("cefhx", "frame_executejavascript", 5);
	static var frame_isfocused:Dynamic = Lib.load("cefhx", "frame_isfocused", 3);
	static var frame_getidentifier:Dynamic = Lib.load("cefhx", "frame_getidentifier", 3);
	static var frame_getparentobj:Dynamic = Lib.load("cefhx", "frame_getparentobj", 3);
	static var frame_getparentid:Dynamic = Lib.load("cefhx", "frame_getparentid", 3);
	static var frame_geturl:Dynamic= Lib.load("cefhx", "frame_geturl", 3);
	static var frame_evaljavascript:Dynamic = Lib.load("cefhx", "frame_evaljavascript", 4);
	static var frame_invokejsfunc:Dynamic = Lib.load("cefhx", "frame_invokejsfunc", 5);
  ///
  // Visit the DOM document.
  ///
  /*--cef()--*/
 // virtual void VisitDOM(CefRefPtr<CefDOMVisitor> visitor) =0;
	
}

