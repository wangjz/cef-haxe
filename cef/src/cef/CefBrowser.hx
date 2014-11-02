package cef;
import cpp.Lib;
import cef.CefWindow;
//import cpp.rtti.FieldIntegerLookup;
import haxe.io.Bytes;
/**
 * ...
 * @author wangjian
 */

 
class CefBrowser //implements FieldIntegerLookup
{
	public var onAfterCreated:Void->Void;
	
	private var _onBeforeClose:Void->Void;
	private function get_onBeforeClose():Void->Void {
		return _onBeforeClose;
	}
	private function set_onBeforeClose(v:Void->Void):Void->Void {
		_onBeforeClose = v;
		browser_seteventhandle(untyped _cefWindow._handle, 1, v != null);
		return v;
	}
	public var onBeforeClose(get_onBeforeClose, set_onBeforeClose):Void->Void;
	
	private var _onLoadStart:Bool->String->Void;
	private function get_onLoadStart():Bool->String->Void {
		return _onLoadStart;
	}
	private function set_onLoadStart(v:Bool->String->Void):Bool->String->Void {
		_onLoadStart = v;
		browser_seteventhandle(untyped _cefWindow._handle, 2, v != null);
		return v;
	}
	/**
	 * onLoadStart(isMainFrame,frameName):Void;
	 */
	public var onLoadStart(get_onLoadStart, set_onLoadStart):Bool->String->Void;
	
	
	private var _onLoadEnd:Bool->String->Int->Void;
	private function get_onLoadEnd():Bool->String->Int->Void {
		return _onLoadEnd;
	}
	private function set_onLoadEnd(v:Bool->String->Int->Void):Bool->String->Int->Void {
		_onLoadEnd = v;
		browser_seteventhandle(untyped _cefWindow._handle, 3, v != null);
		return v;
	}
	/**
	 * onLoadEnd(isMainFrame,frameName,httpStatusCode):Void;
	 */
	public var onLoadEnd(get_onLoadEnd, set_onLoadEnd):Bool->String->Int->Void;
	
	
	private var _onLoadError:Bool->String->Int->String->String;
	private function get_onLoadError():Bool->String->Int->String->String {
		return _onLoadError;
	}
	private function set_onLoadError(v:Bool->String->Int->String->String):Bool->String->Int->String->String {
		_onLoadError = v;
		browser_seteventhandle(untyped _cefWindow._handle, 4, v != null);
		return v;
	}
	/**
	 * onLoadError(isMainFrame,frameName,errorCode,failedUrl):String;
	 * 返回null显示默认错误信息
	 */
	public var onLoadError(get_onLoadError,set_onLoadError):Bool->String->Int->String->String;
	
	
	
	private var _onNavStateChange:Bool->Bool->Void;
	private function get_onNavStateChange():Bool->Bool->Void {
		return _onNavStateChange;
	}
	private function set_onNavStateChange(v:Bool->Bool->Void):Bool->Bool->Void {
		_onNavStateChange = v;
		browser_seteventhandle(untyped _cefWindow._handle, 5, v != null);
		return v;
	}
	/**
	 * onNavStateChange(canGoBack,canGoForward):Void;
	 */
	public var onNavStateChange(get_onNavStateChange, set_onNavStateChange):Bool->Bool->Void;
	
	
	
	private var _onContextCreated:Bool->String->Void;
	private function get_onContextCreated():Bool->String->Void{
		return _onContextCreated;
	}
	private function set_onContextCreated(v:Bool->String->Void):Bool->String->Void {
		_onContextCreated = v;
		browser_seteventhandle(untyped _cefWindow._handle, 6, v != null);
		return v;
	}
	/**
	 * onContextCreated(isMainFrame,frameName):Void;
	 */
	public var onContextCreated(get_onContextCreated, set_onContextCreated):Bool->String->Void;
	
	
	
	private var _onAddressChange:Bool->String->String -> Void;
	private function get_onAddressChange():Bool->String->String -> Void{
		return _onAddressChange;
	}
	private function set_onAddressChange(v:Bool->String->String -> Void):Bool->String->String -> Void {
		_onAddressChange = v;
		browser_seteventhandle(untyped _cefWindow._handle, 9, v != null);
		return v;
	}
	/**
	 * onAddressChange(isMainFrame,frameName,url):Void;
	 */
	public var onAddressChange(get_onAddressChange, set_onAddressChange):Bool->String->String -> Void;
	
	
	private var _onTitleChange:String->String;
	private function get_onTitleChange():String->String{
		return _onTitleChange;
	}
	private function set_onTitleChange(v:String->String):String->String {
		_onTitleChange = v;
		browser_seteventhandle(untyped _cefWindow._handle, 10, v != null);
		return v;
	}
	/**
	 * onTitleChange(title):String;
	 * 返回更改标题内容,返回null显示原来标题
	 */
	public var onTitleChange(get_onTitleChange, set_onTitleChange):String->String;
	
	
	private var _onBeforePopup:String->Bool;
	private function get_onBeforePopup():String->Bool{
		return _onBeforePopup;
	}
	private function set_onBeforePopup(v:String->Bool):String->Bool {
		_onBeforePopup = v;
		browser_seteventhandle(untyped _cefWindow._handle, 7, v != null);
		return v;
	}
	/**
	 * onBeforePopup(url):Bool;
	 * 返回false,取消打开窗口
	 */
	public var onBeforePopup(get_onBeforePopup,set_onBeforePopup):String->Bool;
	
	
	
	private var _onBeforeResourceLoad:String->CefResponse-> Bytes;
	private function get_onBeforeResourceLoad():String->CefResponse-> Bytes{
		return _onBeforeResourceLoad;
	}
	private function set_onBeforeResourceLoad(v:String->CefResponse-> Bytes):String->CefResponse-> Bytes {
		_onBeforeResourceLoad = v;
		browser_seteventhandle(untyped _cefWindow._handle, 8, v != null);
		return v;
	}
	/**
	 * onBeforeResourceLoad(url:String,response:CefResponse):BytesData;
	 */
	public var onBeforeResourceLoad(get_onBeforeResourceLoad, set_onBeforeResourceLoad):String->CefResponse-> Bytes;
	
	
	
	private var _onBeforeBrowse:Bool->String->String->Int->CefRequest->Bool;
	private function get_onBeforeBrowse():Bool->String->String->Int->CefRequest->Bool {
		return _onBeforeBrowse;
	}
	private function set_onBeforeBrowse(v:Bool->String->String->Int->CefRequest->Bool):Bool->String->String->Int->CefRequest->Bool {
		_onBeforeBrowse = v;
		browser_seteventhandle(untyped _cefWindow._handle, 11, v != null);
		return v;
	}
	/**
	 * onBeforeBrowser(isMainFrame:Bool,frameName:String,url:String,navType:Int):Bool;
	 * 返回false,取消跳转
	 */
	public var onBeforeBrowse(get_onBeforeBrowse, set_onBeforeBrowse):Bool->String->String->Int->CefRequest->Bool;
	
	
	
	private var _onContentsSizeChange:Bool->String->Int->Int->Void;
	private function get_onContentsSizeChange():Bool->String->Int->Int->Void {
		return _onContentsSizeChange;
	}
	private function set_onContentsSizeChange(v:Bool->String->Int->Int->Void):Bool->String->Int->Int->Void {
		_onContentsSizeChange = v;
		browser_seteventhandle(untyped _cefWindow._handle, 12, v != null);
		return v;
	}
	/**
	 * onContentsSizeChange(isMainFrame:Bool,frameName:String,width:Int,height:Int):Void;
	 *
	 */
	public var onContentsSizeChange(get_onContentsSizeChange, set_onContentsSizeChange):Bool->String->Int->Int->Void;
	
	
	
	private var _onBeforeMenu:MenuInfo->Bool;
	private function get_onBeforeMenu():MenuInfo->Bool {
		return _onBeforeMenu;
	}
	private function set_onBeforeMenu(v:MenuInfo->Bool):MenuInfo->Bool {
		_onBeforeMenu = v;
		browser_seteventhandle(untyped _cefWindow._handle, 13, v != null);
		return v;
	}
	/**
	 * onBeforeMenu(menuInfo:MenuInfo):Bool
	 * 返回false,取消弹出默认菜单
	 */
	public var  onBeforeMenu(get_onBeforeMenu, set_onBeforeMenu):MenuInfo->Bool;
	
	//onJSAlert
	private var _onJSAlert:Bool->String->String->Bool;
	private function get_onJSAlert():Bool->String->String->Bool {
		return _onJSAlert;
	}
	private function set_onJSAlert(v:Bool->String->String->Bool):Bool->String->String->Bool {
		_onJSAlert = v;
		browser_seteventhandle(untyped _cefWindow._handle, 14, v != null);
		return v;
	}
	/**
	 * onJSAlert(isMainFrame:Bool,frameName:String,message:String):Void;
	 * 返回false,取消弹出默认对话框
	 */
	public var onJSAlert(get_onJSAlert, set_onJSAlert):Bool->String->String->Bool;
	
	
	//onJSConfirm
	private var _onJSConfirm:Bool->String->String->Bool;
	private function get_onJSConfirm():Bool->String->String->Bool {
		return _onJSConfirm;
	}
	private function set_onJSConfirm(v:Bool->String->String->Bool):Bool->String->String->Bool {
		_onJSConfirm = v;
		browser_seteventhandle(untyped _cefWindow._handle, 15, v != null);
		return v;
	}
	/**
	 * onJSConfirm(isMainFrame:Bool,frameName:String,message:String):Bool; 返回确认结果
	 */
	public var onJSConfirm(get_onJSConfirm, set_onJSConfirm):Bool->String->String->Bool;
	
	
	//onConsoleMessage
	//public var OnFocusedNodeChanged;
	//public var onKeyEvent;
	//public var GetPrintHeaderFooter;
	//public var OnDragStart;
	//public var OnBeforeScriptExtensionLoad;
	//public var OnRequestGeolocationPermission;
	//public var NotifyDownloadComplete;
	//public var NotifyDownloadError;
	//public var ClientDownloadHandler;
	//	
	private var _cefWindow:CefWindow;
	private var frames:Array<CefFrame>;
	public var isPopup(get_isPopup,null):Bool;
	public var hasDocument(get_hasDocument,null):Bool;
	public var zoomLevel(get_zoomLevel,set_zoomLevel):Float;
	public var isWindowRenderingDisabled(get_isWindowRenderingDisabled,null):Bool;
	public var isPopupVisible(get_isPopupVisible,null):Bool;
	public var canGoBack(get_canGoBack,null):Bool;
	public var canGoForward(get_canGoForward,null):Bool;
	private var mainFrame:CefFrame;
	public var isInit:Bool;
	public function new(atWindow:CefWindow) 
	{
		isInit = false;
		_cefWindow = atWindow;
		frames = [];
	}
	
	
	private function destroy():Void
	{
		onAfterCreated = null;
		_onBeforeClose = null;
		_onLoadStart = null;
		_onLoadEnd = null;
		_onLoadError = null;
		_onNavStateChange = null;
		_onContextCreated = null;
		_onAddressChange = null;
		_onTitleChange = null;
		_onBeforePopup = null;
		for (i in 0...frames.length) {
			untyped frames[i].destroy();
		}
		frames = null;
		_cefWindow = null;
		untyped if (mainFrame!=null) mainFrame.destroy();
		mainFrame = null;
	}
	private function get_isPopup():Bool
	{
		return browser_ispopup(untyped  _cefWindow._handle);
	}
	private function get_hasDocument():Bool
	{
		return browser_hasdocument(untyped  _cefWindow._handle);
	}
	private function get_zoomLevel():Float
	{
		return browser_getzoomlevel(untyped  _cefWindow._handle);
	}
	private function set_zoomLevel(_v:Float):Float
	{
		browser_setzoomlevel(untyped  _cefWindow._handle,_v);
		return _v;
	}
	private function get_isWindowRenderingDisabled():Bool
	{
		return browser_iswindowrenderingdisabled(untyped  _cefWindow._handle);
	}
	private function get_isPopupVisible():Bool
	{
		return browser_getispopupvisible(untyped  _cefWindow._handle);
	}
	private function get_canGoBack():Bool
	{
		return browser_getcangoback(untyped  _cefWindow._handle);
	}
	private function get_canGoForward():Bool
	{
		return browser_getcangoforward(untyped  _cefWindow._handle);
	}

	public function setFocus(_v:Bool):Bool
	{
		browser_setfocus(untyped  _cefWindow._handle,_v);
		return _v;
	}
	/**
	 * Returns the focused frame for the browser window. This method should only be called on the UI thread.
	 * @return
	 */
	public function getFocusedFrame():CefFrame
	{
		var frame_obj:Dynamic = browser_getfocusedframeobj(untyped  _cefWindow._handle);
		if (frame_obj != null) {
			return initFrame(frame_obj);
		}
		return null;
	}
	/**
	 * Returns the frame with the specified name, or NULL if not found. This method should only be called on the UI thread.
	 * @return
	 */
	public function getFrame(name:String):CefFrame
	{
		var frame_obj:Dynamic = browser_getframeobj(untyped  _cefWindow._handle,name);
		if (frame_obj != null) {
			return initFrame(frame_obj);
		}
		return null;
	}
	
	private function initFrame(frameobj:Dynamic):CefFrame
	{
		if (frameobj == null) return null;
		if (this.isInit == false) return null;
		for (i in 0...frames.length) {
			if (frames[i].identifier == frameobj._url) {
				return frames[i];
			}
		}
		var frame:CefFrame = new CefFrame(this,_cefWindow,frameobj.x,frameobj.y,frameobj._url);
		this.frames.push(frame);
		return frame;
	}
	/**
	 * Returns the names of all existing frames. This method should only be called on the UI thread.
	 * @return
	 */
	public function getFrameNames():Array<String>
	{
		return browser_getframenames(untyped  _cefWindow._handle);
	}
	
	//public function getImage()
	//{
		//
	//}
	
	public function getMainFrame():CefFrame
	{
		if (this.isInit == false) return null;
		if (this.mainFrame != null) return this.mainFrame;
		var frame_id = browser_getmainframeid(untyped  _cefWindow._handle);
		if (frame_id == -1) return null;
		mainFrame=new CefFrame(this, _cefWindow,true,"",frame_id);
		return mainFrame;
	}
	
	
	public function goBack():Void {
		browser_goback(untyped  _cefWindow._handle);
	}
	
	public function goForward():Void
	{
		browser_goforward(untyped  _cefWindow._handle);
	}
	
	
	public function hidePopup():Void
	{
		browser_hidepopup(untyped  _cefWindow._handle);
	}
	
	public function reload(isIgnoreCache:Bool):Void
	{
		browser_reload(untyped  _cefWindow._handle);
	}
	
	
	public function stopLoad():Void
	{
		browser_stopload(untyped  _cefWindow._handle);
	}

	public function find(identifier:Int,searchText:String,forward:Bool,matchCase:Bool,findNext:Bool):Void
	{
		browser_find(untyped  _cefWindow._handle,identifier,searchText,forward,{x:matchCase,y:findNext});
	}
	
	public function stopFind():Void
	{
		browser_stopfind(untyped  _cefWindow._handle);
	}
	public function clearHistory():Void
	{
		browser_clearhistory(untyped  _cefWindow._handle);
	}
	
	public function showDevTools():Void
	{
		browser_showdevtools(untyped  _cefWindow._handle);
	}
	public function closeDevTools():Void
	{
		browser_closedevtools(untyped  _cefWindow._handle);
	}
	
	
	public function getSize(paintElementType:PaintElementType):Size {
		var size:Dynamic = browser_getsize(untyped  _cefWindow._handle, Type.enumIndex(paintElementType));
		return new Size(size.width, size.height);
	}
	public function setSize(paintElementType:PaintElementType,width:Int,height:Int):Void {
		browser_setsize(untyped  _cefWindow._handle,Type.enumIndex(paintElementType), width, height);
	}
	public function Invalidate(rect:Rect):Void {
		browser_invalidate(untyped  _cefWindow._handle,rect.x,rect.y,rect.width,rect.height);
	}
	
	/**
	 * Send a key event to the browser.
	 * @param	type
	 * @param	keyInfo
	 * @param	modifiers
	 */
	public function sendKeyEvent(type:KeyType,keyInfo:KeyInfo ,modifiers:Int):Void
	{
		browser_sendkeyevent(untyped  _cefWindow._handle,Type.enumIndex(type), {x:keyInfo.key,y:keyInfo.sysChar,width:keyInfo.imeChar}, modifiers);
	}
	/**
	 * Send a mouse click event to the browser. The |x| and |y| coordinates are relative to the upper-left corner of the view.
	 * @param	x
	 * @param	y
	 * @param	type
	 * @param	mouseUp
	 * @param	clickCount
	 */
	public function sendMouseClickEvent(x:Int, y:Int, type:MouseButtonType,mouseUp:Bool,clickCount:Int):Void
	{
		browser_sendmouseclickevent(untyped  _cefWindow._handle, {x:x, y:y},Type.enumIndex(type), mouseUp, clickCount);
	}
	/**
	 * Send a mouse move event to the browser. The |x| and |y| coordinates are relative to the upper-left corner of the view.
	 * @param	x
	 * @param	y
	 * @param	mouseLeave
	 */
	public function sendMouseMoveEvent(x:Int,y:Int,mouseLeave:Bool):Void
	{
		browser_sendmousemoveevent(untyped  _cefWindow._handle,x,y,mouseLeave);
	}
	/**
	 * Send a mouse wheel event to the browser. The |x| and |y| coordinates are
	 * relative to the upper-left corner of the view. The |deltaX| and |deltaY|
	 * values represent the movement delta in the X and Y directions respectively.
	 * @param	x
	 * @param	y
	 * @param	deltaX
	 * @param	deltaY
	 */
	public function sendMouseWheelEvent(x:Int,y:Int,deltaX:Int,deltaY:Int):Void
	{
		browser_sendmousewheelevent(untyped  _cefWindow._handle, x, y, deltaX, deltaY);
	}
	/**
	 * Send a capture lost event to the browser.
	 */
	public function sendCaptureLostEvent():Void
	{
		browser_sendcapturelostevent(untyped  _cefWindow._handle);
	}
	/**
	 * Send a focus event to the browser.
	 * @param	v
	 */
	public function sendFocusEvent(v:Bool):Void
	{
		browser_sendfocusevent(untyped  _cefWindow._handle, v);
	}
	static var browser_getfocusedframeobj = Lib.load("cefhx", "browser_getfocusedframeobj", 1);
	//static var browser_getfocusedframeid = Lib.load("cefhx", "browser_getfocusedframeid", 1);
	
	static var browser_getframeobj		   = Lib.load("cefhx", "browser_getframeobj", 2);
	//static var browser_getframeid=		Lib.load("cefhx", "browser_getframeid", 2);
	static var browser_getframenames   = Lib.load("cefhx", "browser_getframenames", 1);
	static var browser_getmainframeid	   = Lib.load("cefhx", "browser_getmainframeid", 1);
	
	static var browser_goback = Lib.load("cefhx", "browser_goback", 1);
	static var browser_goforward = Lib.load("cefhx", "browser_goforward", 1);
	static var browser_hidepopup = Lib.load("cefhx", "browser_hidepopup", 1);
	static var browser_reload = Lib.load("cefhx", "browser_reload", 2);
	
	static var browser_ispopup = Lib.load("cefhx", "browser_ispopup", 1);
	static var browser_hasdocument = Lib.load("cefhx", "browser_hasdocument", 1);
	static var browser_getzoomlevel = Lib.load("cefhx", "browser_getzoomlevel", 1);
	static var browser_setzoomlevel = Lib.load("cefhx", "browser_setzoomlevel", 2);
	static var browser_iswindowrenderingdisabled = Lib.load("cefhx", "browser_iswindowrenderingdisabled", 1);
	static var browser_getispopupvisible = Lib.load("cefhx", "browser_getispopupvisible", 1);
	static var browser_getcangoback = Lib.load("cefhx", "browser_getcangoback", 1);
	static var browser_getcangoforward = Lib.load("cefhx", "browser_getcangoforward", 1);
	static var browser_setfocus = Lib.load("cefhx", "browser_setfocus", 2);
	
	static var browser_sendfocusevent = Lib.load("cefhx", "browser_sendfocusevent", 2);
	static var browser_sendcapturelostevent = Lib.load("cefhx", "browser_sendcapturelostevent", 1);
	static var browser_sendmousewheelevent = Lib.load("cefhx", "browser_sendmousewheelevent", 5);
	static var browser_sendmousemoveevent = Lib.load("cefhx", "browser_sendmousemoveevent", 4);
	static var browser_sendmouseclickevent = Lib.load("cefhx", "browser_sendmouseclickevent", 5);
	static var browser_sendkeyevent = Lib.load("cefhx", "browser_sendkeyevent", 4);
	
	static var browser_find = Lib.load("cefhx", "browser_find", 5);
	static var browser_setsize = Lib.load("cefhx", "browser_setsize", 4);
	static var browser_getsize = Lib.load("cefhx", "browser_getsize", 2);
	static var browser_invalidate = Lib.load("cefhx", "browser_invalidate", 5);
	static var browser_stopload = Lib.load("cefhx", "browser_stopload", 1);
	static var browser_stopfind = Lib.load("cefhx", "browser_stopfind", 1);
	static var browser_clearhistory = Lib.load("cefhx", "browser_clearhistory", 1);
	static var browser_showdevtools = Lib.load("cefhx", "browser_showdevtools", 1);
	static var browser_closedevtools = Lib.load("cefhx", "browser_closedevtools", 1);
	static var browser_seteventhandle=Lib.load("cefhx", "browser_seteventhandle", 3);
	
}
typedef MenuInfo = {
	var x:Int;
	var y:Int;
	var frameUrl:String;
	var imageUrl:String;
	var linkUrl:String;
	var selectionText:String;
	var misspelledWord:String;
	var securityInfo:String;
	/**
	 * 
  // No node is selected MENUTYPE_NONE = 0x0,
	The top page is selected MENUTYPE_PAGE = 0x1,//A subframe page is selected MENUTYPE_FRAME = 0x2,// A link is selected MENUTYPE_LINK = 0x4,// An image is selected  MENUTYPE_IMAGE = 0x8,// There is a textual or mixed selection that is selected MENUTYPE_SELECTION = 0x10,//An editable element is selected MENUTYPE_EDITABLE = 0x20,// A misspelled word is selected MENUTYPE_MISSPELLED_WORD = 0x40,// A video node is selected MENUTYPE_VIDEO = 0x80,// A video node is selected MENUTYPE_AUDIO = 0x100,
	 */
	var typeFlags:Int;
	/**
	 * MENU_CAN_DO_NONE = 0x0, MENU_CAN_UNDO = 0x1,MENU_CAN_REDO = 0x2,MENU_CAN_CUT = 0x4,MENU_CAN_COPY = 0x8,MENU_CAN_PASTE = 0x10,MENU_CAN_DELETE = 0x20,MENU_CAN_SELECT_ALL = 0x40,MENU_CAN_TRANSLATE = 0x80,Values unique to CEF MENU_CAN_GO_FORWARD = 0x10000000, MENU_CAN_GO_BACK = 0x20000000,
	 */
	var editFlags:Int;
}