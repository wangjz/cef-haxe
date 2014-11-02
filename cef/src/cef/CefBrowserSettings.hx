package cef;
//import cpp.rtti.FieldIntegerLookup;

/**
 * ...
 * @author wangjian
 */
class CefBrowserSettings //implements FieldIntegerLookup
{
	/**
	 * Set to true to disable accelerated 2d canvas.
	 * 是否禁止canvas true 禁止
	 */
	public var accelerated2DCanvasDisabled:Bool; //==
	/**
	 * 是否启用加速合成 true 禁止
	 * Set to true (1) to enable accelerated compositing. This is turned off by
       default because the current in-process GPU implementation does not support it correctly.
	 */
	public var acceleratedCompositingEnabled:Bool;	 //==
	
	/**
	 * Set to true (1) to disable accelerated painting.
	 * 是否禁止加速渲染 true 禁止
	 */
	public var acceleratedPaintingDisabled:Bool; //==
	/**
	 * Set to true to disable accelerated layers. This affects features like 3D CSS transforms.
	 * 是否禁止加速层 true 禁止
	 */
	public var acceleratedLayersDisabled:Bool;	//==
	
	/**
	 *  Set to true (1) to disable accelerated filters.
	 */
	public var acceleratedFiltersDisabled:Bool; //==
	
	/**
	 * Set to true to disable accelerated plugins.
	 * 是否禁止加速插件?true 禁止
	 */
	public var acceleratedPluginsDisabled:Bool; //==
	/**
	 * Set to true to disable accelerated video.
	 * 是否禁止加速视频 true 禁止
	 */
	public var acceleratedVideoDisabled:Bool;	//==
	
	/**
	 * Set to true to disable application cache.
	 * 是否禁止程序缓存 true 禁止
	 */
	public var applicationCacheDisabled:Bool; //==
	/**
	 * Set to true to disable style sheets.
	 * 是否禁止样式表 true 禁止
	 */
	public var authorAndUserStylesDisabled:Bool;//==
	/**
	 * Set to true to enable drawing of the caret position.
	 * 启用绘制光标位置 true 启用
	 */
	public var caretBrowsingEnabled:Bool; //==
	
	
	/**
	 * Set to true to disable databases.
	 * 禁用数据库 true 禁止
	 */
	public var databasesDisabled:Bool; //==
	/**
	 *  Default encoding for Web content. If empty "ISO-8859-1" will be used.
	 */
	public var defaultEncoding:String; //==
	
	
	/**
	 * Set to true to disable developer tools (WebKit inspector).
	 * 禁用开发者工工具?
	 */
	public var developerToolsDisabled:Bool;	 //==
	/**
	 * Set to true to disable DOM pasting in the editor. DOM pasting also depends on |javascript_cannot_access_clipboard| being false (0).
	 */
	public var domPasteDisabled:Bool; //==
	
	/**
	 * Disable drag & drop of URLs from other windows.
	 */
	public var dragDropDisabled:Bool; //==
	/**
	 * Set to true to attempt automatic detection of content encoding.
	 * 自动检测内容编码?
	 */
	public var encodingDetectorEnabled:Bool; //==
	
	
	
	/**
	 * Set to true to allow access to file URLs from other file URLs.
	 * 
	 */
	public var fileAccessFromFileUrlsAllowed:Bool; //==
	
	/**
	 * Set to true to allow access to all URLs from file URLs.
	 */
	public var universalAccessFromFileUrlsAllowed:Bool; //==
	
	/**
	 * Set to true to disable hyperlink pings (<a ping> and window.sendPing).
	 */
	public var hyperlinkAuditingDisabled:Bool; //==
	
	/**
	 * Set to true to suppress the network load of image URLs. A cached image will still be rendered if requested.
	 */
	public var imageLoadDisabled:Bool; //==
	
	/**
	 * Set to true to disable Java.
	 */
	public var javaDisabled:Bool; //==
	
	/**
	 * Set to true to disallow JavaScript from accessing the clipboard.
	 */
	public var javaScriptAccessClipboardDisallowed:Bool; //==
	
	/**
	 * Set to true to disallow JavaScript from closing windows.
	 */
	public var javaScriptCloseWindowsDisallowed:Bool; //==
	
	/**
	 * Set to true to disable JavaScript.
	 */
	public var javaScriptDisabled:Bool; //==
	
	/**
	 * Set to true to disallow JavaScript from opening windows.
	 */
	public var javaScriptOpenWindowsDisallowed:Bool; //==
	
	/**
	 * Set to true to disable local storage.
	 * 禁用本地存储设置
	 */
	public var localStorageDisabled:Bool; //==
	
	
	
	
	/**
	 * Set to true to disable use of the page cache.
	 * 禁用页面缓存
	 */
	public var pageCacheDisabled:Bool;	 //==
	
	/**
	 * Set to true to disable plugins.
	 */
	public var pluginsDisabled:Bool; //==
	
	/**
	 * Set to true to disable loading of fonts from remote sources.
	 */
	public var remoteFontsDisabled:Bool;  //==
		
	
	/**
	 * Set to true to shrink standalone images to fit the page.
	 */
	public var shrinkStandaloneImagesToFit:Bool; //==
	
	/**
	 * Set to true to disable browser backwards compatibility features.
	 * 禁用浏览器向后兼容
	 */
	public var siteSpecificQuirksDisabled:Bool; //==
	
	
	/**
	 * Set to true to not have the tab key advance focus to links.
	 */
	public var tabToLinksDisabled:Bool; //==
	
	
	/**
	 * Set to true to disable resize of text areas.
	 */
	public var textAreaResizeDisabled:Bool; //==
	
	
	
	
	/**
	 * Set to true (1) to enable the user style sheet for all pages.
	 */
	public var userStyleSheetEnabled:Bool; //==
	
	/**
	 * Location of the user style sheet. This must be a data URL of the form "data:text/css;charset=utf-8;base64,csscontent" where "csscontent" is the base64 encoded contents of the CSS file.
	 *用户样式表的位置
	 */
	public var userStyleSheetLocation:String; //==
	
	/**
	 * Set to true to disable WebGL.
	 * 禁用webgl
	 */
	public var webGLDisabled:Bool; //==
	
	/**
	 * Set to true to allow risky security behavior such as cross-site scripting (XSS). Use with extreme care.
	 */
	public var webSecurityDisabled:Bool; //==
	/**
	 * Set to true to enable console warnings about XSS attempts.
	 * 
	 */
	public var xssAuditorEnabled:Bool; //==
	
	/**
	 * Disable default navigation resulting from drag & drop of URLs.
	 */
	public var loadDropsDisabled:Bool; //==
	
	/**
	 * Disable history back/forward navigation.
	 */
	public var historyDisabled:Bool; //==
	
	/**
	 * The number of frames per second (fps) for animation and windowless
	   rendering. When window rendering is enabled and the JavaScript
       requestAnimationFrame method is used the browser client area will be
       invalidated at the rate specified. When window rendering is disabled the
       CefRenderHandler::OnPaint() method will be called at the rate specified.
       This value must be between 0 and 90. Specify a value of zero for the
       default frame rate of 30 fps. Changing this value may affect display
       performance and/or CPU usage.
	 */
	public var animationFrameRate:Int; //==
	
	/**
	 *  Set to true (1) to enable fullscreen mode.
	 */
	public var fullscreenEnabled:Bool; //==
	
	/*字体设置*/
	public var standardFontFamily:String;
	public var fixedFontFamily:String;
	public var sansSerifFontFamily:String;
	public var serifFontFamily:String;
	public var cursiveFontFamily:String;
	public var fantasyFontFamily:String;	
	public var defaultFixedFontSize:Int;
	public var defaultFontSize:Int;	
	public var minimumFontSize:Int;
	public var minimumLogicalFontSize:Int;	
	
	public function new()
	{
		accelerated2DCanvasDisabled = false; //
		acceleratedCompositingEnabled = false;
		acceleratedPaintingDisabled = false;
		acceleratedFiltersDisabled = false;
		acceleratedLayersDisabled = false;
		acceleratedPluginsDisabled = false;
		acceleratedVideoDisabled = false;
		applicationCacheDisabled = false;
		authorAndUserStylesDisabled = false;
		caretBrowsingEnabled = false;
		databasesDisabled = false;
		defaultEncoding = "";
		developerToolsDisabled = true;
		domPasteDisabled = false;
		dragDropDisabled = true;
		encodingDetectorEnabled = false;
		fileAccessFromFileUrlsAllowed = false;
		hyperlinkAuditingDisabled = false;
		imageLoadDisabled = false;
		javaDisabled = true;
		javaScriptAccessClipboardDisallowed = false;
		javaScriptCloseWindowsDisallowed = false;
		javaScriptDisabled = false;
		javaScriptOpenWindowsDisallowed = false;
		
		localStorageDisabled = false;
		pageCacheDisabled = false;
		pluginsDisabled = false;
		remoteFontsDisabled = false;
		shrinkStandaloneImagesToFit = false;
		siteSpecificQuirksDisabled = false;
		tabToLinksDisabled = false;
		textAreaResizeDisabled = false;
		universalAccessFromFileUrlsAllowed = false;
		
		userStyleSheetEnabled = false;
		webGLDisabled = true;
		webSecurityDisabled = false;
		xssAuditorEnabled = false;
		
		loadDropsDisabled = false;
		historyDisabled = false;
		
		animationFrameRate = 25;
		
		fullscreenEnabled = false;
		
		defaultFixedFontSize = -1;
		defaultFontSize=-1;	
		minimumFontSize=-1;
		minimumLogicalFontSize=-1;	
	}
}