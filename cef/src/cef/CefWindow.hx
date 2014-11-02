package cef;
import cpp.Lib;
//import cpp.rtti.FieldIntegerLookup;
import haxe.Int32;

/**
 * ...
 * @author wangjian QQ:421924396
 */
class CefWindow //implements FieldIntegerLookup
{
	
		public static var WF_FULLSCREEN	= 1;
		public static var WF_TRANSPARENT	= 1 << 1;
		public static var WF_PLAIN			= 1 << 2;
		public static var WF_ALWAYS_ONTOP	= 1 << 3;
		public static var WF_NO_TASKBAR	= 1 << 4;
		
		public static var WF_DROPTARGET = 1 << 5;
		
		public static var WF_BROWSER_RENDERING = 1 << 6; //离屏
		public static var WF_BROWSER_CHILD = 1 <<7;
		public static var WF_BROWSER_POPUP = 1 << 8;
		
		public static var WF_NO_CAPTION = 1 << 9;
		
		private var cefBrowserSet:CefBrowserSettings;
		private var _url:String;
		public var url(get_url, set_url):String;
		private var _windowFlag:Int;
		public var windowFlag(get_windowFlag, null):Int;
		private var _cefBrowser:CefBrowser;
		public var cefBrowser(get_cefBrowser,null):CefBrowser;
		private var _handle:Dynamic;
		
		private var isToDestroy:Bool;
		public var minSize:Size;
		
		//private var _sysMessageHook:Bool;
		/**
		 * sysMessageHook 是否侦听系统消息 拖盘 热键
		 * 
		 */
		//public var sysMessageHook(get_sysMessageHook, set_sysMessageHook):Bool;
		private function set_url(in_url:String):String
		{
			_url = in_url;
			return _url;
		}
		/*
		private function set_sysMessageHook(v:Bool):Bool {
			_window_set_message_hook(_handle, v);
			_sysMessageHook = v;
			return v;
		}
		private function get_sysMessageHook():Bool {
			return _sysMessageHook;
		}
		*/
		private function get_url():String
		{
			return _url;
		}
		private function get_windowFlag():Int
		{
			return _windowFlag;
		}	
		
		private function get_cefBrowser():CefBrowser
		{
			return _cefBrowser;
		}
		/**Get/Set allowing operating system default chrome window resizing.**/
		public var resizable(get_resizable,set_resizable) : Bool;

		/**Get/Set operating system default chrome maximize button presence/enablement.**/
		public var maximizeIcon(get_maximizeIcon,set_maximizeIcon) : Bool;

		/**Get/Set operating system default chrome minimize button presence/enablement.**/
		public var minimizeIcon(get_minimizeIcon,set_minimizeIcon) : Bool;

		/**Get/Set the width of the window's client area.**/
		public var width(get_width,set_width) : Int;

		/**Get/Set the height of the window's client area.**/
		public var height(get_height,set_height) : Int;

		/**Get/Set the window's vertical position on the desktop.**/
		public var left(get_left,set_left) : Int;

		/**Get/Set the window's horizontal position on the desktop.**/
		public var top(get_top,set_top) : Int;

		/**
		Get/Set window full-screen mode.
		On OS-X, full-screen mode will disable the menu bar, task-switching and force quit.
		**/
		public var fullScreen(get_fullScreen,set_fullScreen) : Bool;

		/**Not implemented.**/
		public var transparent(get_transparent,null) : Bool;

		/**Get/Set window file-drop acceptance.**/
		public var dropTarget(get_dropTarget,set_dropTarget) : Bool;

		/**Get/Set window title.**/
		public var title(default,set_title) : String;

		/**''true'' if the window was created with the WF_PLAIN flag set.**/
		public var plain(get_plain,null) : Bool;

		/**Get/Set window minimized state.**/
		public var minimized(get_minimized,set_minimized) : Bool;

		/**Get/Set window maximized state.**/
		public var maximized(get_maximized,set_maximized) : Bool;
	//
		///**Get/Set window visibility.**/
		private var _visible:Bool;
		public var visible(get_visible, set_visible):Bool;
		
		public var caption(get_caption, set_caption):Bool;
		public var enabled(get_enabled, set_enabled):Bool;
		//public var border(get_border, set_border):Bool;
		public function new(in_url:String,  width : Int, height : Int, ?window_flags : Int,?_browserSet:CefBrowserSettings) {
			isToDestroy = false;
			//_sysMessageHook = false;
			_url = in_url;
			cefBrowserSet = _browserSet;
			if (window_flags == null) window_flags = 0;
			_windowFlag = window_flags;
			_visible = false;
			minSize = new Size(0, 0);
			_cefBrowser = new CefBrowser(this);
			var isCreate = _window_create(width, height, window_flags, this,_cefBrowser);
		}
		
		/** Show or hide the window [DEPRECATED] uses .visible instead **/
		public function show( b : Bool,?isAvtive:Bool=true,?isFront:Bool=true,?isAsync:Bool=false ) {
			if ( _handle != null ) {
				_window_show(_handle,b,isFront,isAsync,isAvtive);
			}
			_visible = b;
		}
		
		/**Close the window.**/
		public function close() {
			untyped if(CefApp.isQuit==true)isToDestroy = true;
			_window_close(_handle);
		}
//
		///**Destroy the window.**/
		public function destroy() {
			isToDestroy = true;
			_window_close(_handle);
			//_window_destroy(_handle);
		}
		
		public function flash(state:Bool):Void {
			
		}
		/**
		<p>
		Initiate user window resizing.
		<p>
		Specify either "L","R","T","TL","TR","B","BL" or "BR" to set the resize direction.
		Windows on OSX ignore this argument and always resizes from the bottom-right ("BR") corner of the window.
		**/
		public function resize(?direction: String) {
			var i;
			switch (direction) {
				case "L": i=1;
				case "R": i=2;
				case "T": i=3;
				case "TL": i=4;
				case "TR": i=5;
				case "B": i=6;
				case "BL": i=7;
				case "BR": i=8;
				default: i=8;
			}
			_window_resize(_handle,i);
		}

		/**Initiate user window dragging.**/
		public function drag() {
			return _window_drag(_handle);
		}

		public function setRound(rect:Rect, ellipseSize:Size):Void {
			if (rect == null || ellipseSize == null) return;
			_window_setround(_handle,rect,ellipseSize);
		}
		
		

		/**
		Event invoked when the window is minimized from the operating system window chrome.
		Returning ''false'' will cancel this action.
		**/
		public function onMinimize() {
			return true;
		}

		/**
		Event invoked when the window is maximized from the operating system window chrome.
		Returning ''false'' will cancel this action.
		**/
		public function onMaximize() {
			return true;
		}

		/**
		Event invoked when the user right-clicks in the window's client area.
		Returning ''false'' from the event handler will prevent the event being forwarded to the Flash player.
		**/
		//public function onRightClick() {
			//return true;
		//}

		/**Event invoked when the user drops files on the window's client area while Window.dropTarget is enabled.**/
		//public function onFilesDropped( files : Array<String> ) {
			//return true;
		//}

		/**
		Event invoked when the window is restored by the user.
		Returning ''false'' will cancel this action.
		**/
		public function onCreate():Void
		{
			
		}
		/**Event invoked on window closure. Returning ''false'' from the event handler will cancel window closure.**/
		public function onClose():Bool
		{
			return true;
		}
		public function onRestore():Bool {
			return true;
		}
		
		public function onDestroy():Void
		{
			
		}
		public function onSize():Bool
		{
			return true;
		}
		function get_resizable() {
			return _window_get_prop(_handle,0) != 0;
		}

		function set_resizable( b:Bool ) {
			_window_set_prop(_handle,0,if( b ) 1 else 0);
			return b;
		}
		
		function get_maximizeIcon() {
			return _window_get_prop(_handle,1) != 0;
		}

		function set_maximizeIcon(b:Bool) {
			_window_set_prop(_handle,1,if( b ) 1 else 0);
			return b;
		}

		function get_minimizeIcon() {
			return _window_get_prop(_handle,2) != 0;
		}

		function set_minimizeIcon(b:Bool) {
			_window_set_prop(_handle,2,if( b ) 1 else 0);
			return b;
		}

		function get_width() {
			return _window_get_prop(_handle,3);
		}

		function get_height() {
			return _window_get_prop(_handle,4);
		}

		function get_left() {
			return _window_get_prop(_handle,5);
		}

		function get_top() {
			return _window_get_prop(_handle,6);
		}

		function set_width(i:Int) {
			_window_set_prop(_handle,3,i);
			return i;
		}

		function set_height(i:Int) {
			_window_set_prop(_handle,4,i);
			return i;
		}

		function set_left(i:Int) {
			_window_set_prop(_handle,5,i);
			return i;
		}

		function set_top(i:Int) {
			_window_set_prop(_handle,6,i);
			return i;
		}

		function get_fullScreen() {
			return _window_get_prop(_handle,7) != 0;
		}

		function set_fullScreen( b ) {
			_window_set_prop(_handle,7,if( b ) 1 else 0);
			return b;
		}

		function get_transparent() {
			return _window_get_prop(_handle,8) != 0;
		}

		function get_dropTarget() {
			return _window_get_prop(_handle,13) != 0;
		}

		function set_dropTarget( b ) {
			_window_set_prop(_handle,9,if( b ) 1 else 0);
			return b;
		}

		function set_title(t: String) : String {
			title = t;
			if( _handle != null ) _window_set_title(_handle,t);
			return t;
		}

		function get_plain() {
			return _window_get_prop(_handle,10) != 0;
		}

		function get_minimized() {
			return _window_get_prop(_handle,11) != 0;
		}

		function set_minimized( b:Bool ) {
			_window_set_prop(_handle,11,if( b ) 1 else 0);
			return b;
		}

		function get_maximized():Bool {
			return _window_get_prop(_handle,12) != 0;
		}

		function set_maximized( b:Bool ) {
			_window_set_prop(_handle,12,if( b ) 1 else 0);
			return b;
		}

		private function get_visible() {
			return _visible;
		}

		private function set_visible(b:Bool) {
			if ( _handle != null ) {
				_window_show(_handle,b,true,false,true);
			}
			_visible = b;
			return b;
		}
		
		
		private function get_caption() {
			return _window_get_prop(_handle,13) != 0;
		}

		private function set_caption(b:Bool) {
				_window_set_prop(_handle,13,if( b ) 1 else 0);
			return b;
		}
		
		private function get_enabled() {
			return _window_get_prop(_handle,14) != 0;
		}

		private function set_enabled(b:Bool) {
			_window_set_prop(_handle,14,if( b ) 1 else 0);
			return b;
		}
		public function runTask(task:Task, ?delay:Int = 0):Void {
			untyped if (CefApp.isQuit == true) return;
			if (isToDestroy) return;
			untyped task.window = this;
			untyped task.delay = delay;
			cef_app_posttask(_handle, delay);
		}
		private function on_jscall(funcName:String, args:Array<Dynamic>):Dynamic {
			untyped if (CefApp.isQuit == true) return null;
			if (isToDestroy) return null;
			return onJsCallBack(funcName, args);
		}
		public function onJsCallBack(funcName:String, args:Array<Dynamic>):Dynamic
		{
			var field = Reflect.field(this, funcName);
			if (field == null) return null;
			if (Reflect.isFunction(field)) {
				return Reflect.callMethod(this, field, args);
			}else {
				if (args == null || args.length == 0) {
					return field;
				}else {
					Reflect.setField(this, funcName, args[0]);
				}
			}
			return null;
		}
		

		private function on_create(w_handle):Void
		{
			this._handle = w_handle;
			CefApp.naviteWindows.push(this);
			onCreate();
		}
		private function on_close():Bool
		{
			untyped if (CefApp.isQuit == true) return true;
			if (isToDestroy) return true;
			return onClose();
		}

		private function on_minimize():Bool
		{
			return onMinimize();			
		}
		private function on_restore():Bool
		{
			return onRestore();
		}
		private function on_maximize():Bool
		{
			return onMaximize();
		}
		private function on_fullscreen():Bool
		{
			return true;
		}
		private function on_destroy():Int
		{
			onDestroy();
			untyped cefBrowser.destroy();
			cefBrowser = null;
			cefBrowserSet = null;
			_handle = null;
			CefApp.naviteWindows.remove(this);
			return CefApp.naviteWindows.length;
		}
		private function on_size():Bool
		{
			return onSize();
		}
		public function onSystemMessage(msg:String, wparam:Dynamic, lparam:Dynamic):Bool {
			return true;
		}
		
		private function onBrowserEvent(eventName:String,evnetValue:Dynamic):Dynamic
		{
			untyped if (CefApp.isQuit == true) return null;
			if (isToDestroy) return null;
			if (eventName == "onAfterCreated") {
				cefBrowser.isInit = true;
				if (cefBrowser.onAfterCreated != null) cefBrowser.onAfterCreated();
			}else if (eventName == "onContextCreated") {
				if (cefBrowser.onContextCreated != null) cefBrowser.onContextCreated(evnetValue.x, evnetValue.y);
			}else if (eventName == "onBeforeClose") {
				if (cefBrowser.onBeforeClose != null) cefBrowser.onBeforeClose();
			}else if (eventName == "onLoadStart") {
				if (cefBrowser.onLoadStart != null) cefBrowser.onLoadStart(evnetValue.x, evnetValue.y);
			}else if (eventName == "onLoadEnd") {
				if (cefBrowser.onLoadEnd != null) cefBrowser.onLoadEnd(evnetValue.x, evnetValue.y, evnetValue._url);
			}else if (eventName == "onLoadError") {
				if (cefBrowser.onLoadError != null) return cefBrowser.onLoadError(evnetValue.x, evnetValue.y, evnetValue.width, evnetValue._url);
			}else if (eventName == "onAddressChange") {
				if (cefBrowser.onAddressChange != null) cefBrowser.onAddressChange(evnetValue.x, evnetValue.y, evnetValue._url);
			}else if (eventName == "onTitleChange") {
				if (cefBrowser.onTitleChange != null)return cefBrowser.onTitleChange(Std.string(evnetValue));
			}else if (eventName == "onBeforePopup") {
				if (cefBrowser.onBeforePopup != null) return cefBrowser.onBeforePopup(Std.string(evnetValue));
				return true;
			}else if (eventName == "onBeforeResourceLoad") {
				if (cefBrowser.onBeforeResourceLoad != null) {
					var cefResponse:CefResponse = new CefResponse();
					var retVal = cefBrowser.onBeforeResourceLoad(evnetValue, cefResponse);
					if (retVal == null)  {
						_window_onresloaded(_handle, {x:null,y:false, _url:null,width:null,height:0,linkUrl:evnetValue,minSize:0});
						return true;
					}
					var bytesLen = 0;
					var r_bytes = null;
					if (retVal != null) {
						r_bytes = retVal.getData();
						bytesLen = retVal.length;
					}
					_window_onresloaded(_handle,{x:r_bytes,y:cefResponse.end, _url:cefResponse.redirectUrl,width:cefResponse.mimeType,height:bytesLen,linkUrl:evnetValue,minSize:cefResponse.status});
				}
				return true;
			}else if (eventName == "onBeforeBrowse") {
				if (cefBrowser.onBeforeBrowse != null) {
					//var _request:CefRequest =untyped new CefRequest(evnetValue.linkUrl); _request
					return cefBrowser.onBeforeBrowse(evnetValue.x, evnetValue.y, evnetValue._url, evnetValue.width,null);
				}
				return true;
			}else if (eventName=="onNavStateChange") {
				if (cefBrowser.onNavStateChange != null) cefBrowser.onNavStateChange(evnetValue.x, evnetValue.y);
			}else if (eventName=="onContentsSizeChange") {
				if (cefBrowser.onContentsSizeChange != null) cefBrowser.onContentsSizeChange(evnetValue.x, evnetValue.y,evnetValue.width,evnetValue.height);
			}else if (eventName == "onBeforeMenu") {
				if (cefBrowser.onBeforeMenu != null) return cefBrowser.onBeforeMenu(evnetValue);
			}
			else if (eventName=="onJSAlert") {
				if (cefBrowser.onJSAlert != null)return cefBrowser.onJSAlert(evnetValue.x, evnetValue.y, evnetValue._url);
			}else if (eventName == "onJSConfirm") {
				if (cefBrowser.onJSConfirm != null)return cefBrowser.onJSConfirm(evnetValue.x, evnetValue.y, evnetValue._url);
			}else if (eventName == "on_task") {
				untyped Task.onTask(this,evnetValue);
			}
			return null;
		}
		static var _window_create = Lib.load("cefhx", "cef_window_create", 5);
		static var _window_close= Lib.load("cefhx", "cef_window_close", 1);
		static var _window_show = Lib.load("cefhx","cef_window_show",5);
		//static var _window_destroy = Lib.load("cefhx","cef_window_destroy",1);
		static var _window_set_prop = Lib.load("cefhx","window_set_prop",3);
		static var _window_get_prop = Lib.load("cefhx","window_get_prop",2);
		static var _window_set_title = Lib.load("cefhx","window_set_title",2);
		static var _window_drag = Lib.load("cefhx","window_drag",1);
		static var _window_resize = Lib.load("cefhx", "window_resize", 2);
		static var _window_setround = Lib.load("cefhx", "window_setround", 3);
		
		static var _window_onresloaded = Lib.load("cefhx", "window_onresloaded", 2);
		//static var _window_set_message_hook = Lib.load("cefhx", "window_set_message_hook", 2);
		static var cef_app_posttask = Lib.load("cefhx", "cef_app_posttask", 2);
}

class Rect {
	public function new(_x:Int, _y:Int, _width:Int, _height:Int)
	{
		x = _x;
		y = _y;
		width = _width;
		height = _height;
	}
	
	public var x:Int;
	public var y:Int;
	public var width:Int;
	public var height:Int;
}

class Size {
	public function new(_width:Float, _height:Float) 
	{
		width = _width;
		height = _height;
	}
	public var width:Float;
	public var height:Float;
}

class KeyInfo {
	public function new(_key:Int, _sysChar:Bool, _imeChar:Bool)
	{
		key = _key;
		sysChar = _sysChar;
		imeChar = _imeChar;
	}
	public var key:Int;
	public var sysChar:Bool;
	public var imeChar:Bool;
}
enum KeyType {
	KEY_UP;
	KEY_DOWN ;
	KEY_CHAR;
	
}

enum MouseButtonType {
  MOUSE_LEFT ;
  MOUSE_MIDDLE;
  MOUSE_RIGHT;
}

enum PaintElementType {
  PET_VIEW ;
  PET_POPUP;
}