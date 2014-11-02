package cef;
//import cpp.rtti.FieldIntegerLookup;

/**
 * ...
 * @author wangjian QQ:421924396
 */
class CefSettings //implements FieldIntegerLookup
{
	
	public static var multiThreadedMessageLoop:Bool;
	/**
	 * The location where cache data will be stored on disk. If empty an
	 * in-memory cache will be used. HTML5 databases such as localStorage will
	 * only persist across sessions if a cache path is specified.
	 */
	public var cachePath:String;
	/**
	 * Value that will be returned as the User-Agent HTTP header. If empty the
	 * default User-Agent string will be used.
	 */
	public var userAgent:String;
	/**
	 * The locale string that will be passed to WebKit. If empty the default
	 * locale of "en-US" will be used. This value is ignored on Linux where locale
	 * is determined using environment variable parsing with the precedence order:
	 * LANGUAGE, LC_ALL, LC_MESSAGES and LANG.
	 */
	public var locale:String;
	/**
	 * List of fully qualified paths to plugins (including plugin name) that will
	 * be loaded in addition to any plugins found in the default search paths.
	 */
	public var extraPluginPaths:Array<String>;
	/**
	 * The directory and file name to use for the debug log. If empty, the
	 * default name of "debug.log" will be used and the file will be written
	 * to the application directory.
	 */
	public var logFile:String;
	/**
	 * LOGSEVERITY_VERBOSE -1
	 * LOGSEVERITY_INFO 0
	 * LOGSEVERITY_WARNING 1
	 * LOGSEVERITY_ERROR 2
	 * LOGSEVERITY_ERROR_REPORT 3
	 * LOGSEVERITY_DISABLE 99
	 */
	public var logSeverity:Int; //
	/**
	 * Enable DCHECK in release mode to ease debugging.
	 */
	public var releaseDcheckEnabled:Bool;
	
	/**
	 * The graphics implementation that CEF will use for rendering GPU accelerated content like WebGL, accelerated layers and 3D CSS.
	 * ANGLE_IN_PROCESS 0
	 * ANGLE_IN_PROCESS_COMMAND_BUFFER 1
	 * DESKTOP_IN_PROCESS 2
	 * DESKTOP_IN_PROCESS_COMMAND_BUFFER 3
	 */
	public var graphicsImplementation:Int;
	/**
	 * Quota limit for localStorage data across all origins. Default size is 5MB.
	 */
	public var localStorageQuota:Int;
	/**
	 * Quota limit for sessionStorage data per namespace. Default size is 5MB.
	 */
	public var sessionStorageQuota:Int;
	/**
	 *  Custom flags that will be used when initializing the V8 JavaScript engine. The consequences of using custom flags may not be well tested.
	 */
	public var javascriptFlags:String;
	/**
	 * The fully qualified path for the resources directory. If this value is
	 * empty the chrome.pak and/or devtools_resources.pak files must be located in
	 * the module directory on Windows/Linux or the app bundle Resources directory
	 * on Mac OS X.
	 */
	public var resourcesDirPath:String;
	/**
	 * The fully qualified path for the locales directory. If this value is empty
	 * the locales directory must be located in the module directory. This value
	 * is ignored on Mac OS X where pack files are always loaded from the app
	 * bundle Resources directory.
	 */
	public var localesDirPath:String;
	/**
	 * Set to true (1) to disable loading of pack files for resources and locales.
	 * A resource bundle handler must be provided for the browser and renderer
	 * processes via CefApp::GetResourceBundleHandler() if loading of pack files
	 * is disabled.
	 */
	public var packLoadingDisabled:Bool;
	/**
	 * The number of stack trace frames to capture for uncaught exceptions.
	 * Specify a positive value to enable the CefV8ContextHandler::
	 * OnUncaughtException() callback. Specify 0 (default value) and
	 * OnUncaughtException() will not be called
	 */
	public var uncaughtExceptionStackSize:Int;
	/**
	 *  By default CEF V8 references will be invalidated (the IsValid() method will
	 * return false) after the owning context has been released. This reduces the
	 * need for external record keeping and avoids crashes due to the use of V8
	 * references after the associated context has been released.
	 * 
	 * CEF currently offers two context safety implementations with different
	 * performance characteristics. The default implementation (value of 0) uses a
	 * map of hash values and should provide better performance in situations with
	 * a small number contexts. The alternate implementation (value of 1) uses a
	 * hidden value attached to each context and should provide better performance
	 * in situations with a large number of contexts.
	 * 
	 * If you need better performance in the creation of V8 references and you
	 * plan to manually track context lifespan you can disable context safety by
	 * specifying a value of -1.
	 */
	public var contextSafetyImplementation:Int;
	public function new() 
	{
		cachePath = "";
		userAgent = "";
		locale = "zh-CN";
		logFile = "";
		javascriptFlags = "";
		resourcesDirPath = "";
		localesDirPath = "";
		
		logSeverity = 0;
		multiThreadedMessageLoop = false;
		releaseDcheckEnabled = false;
		packLoadingDisabled = false;
		graphicsImplementation = 0;
		localStorageQuota = -1;
		sessionStorageQuota = -1;
		uncaughtExceptionStackSize = 0;
		contextSafetyImplementation = 0;
	}
}