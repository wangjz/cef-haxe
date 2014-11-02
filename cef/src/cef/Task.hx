package cef;
import cpp.vm.Mutex;
/**
 * ...
 * @author wangjian QQ:421924396
 */
class Task {
	static var mutex:Mutex = new Mutex();
	static var tasks:Array<Task> = [];
	var call:Dynamic->Void;
	var args:Dynamic;
	var window:CefWindow;
	var delay:Int;
	public function new(taskCall:Dynamic->Void,?_args:Dynamic=null) {
		call = taskCall;
		args = _args;
		delay = 0;
		window = null;
		mutex.acquire();
		tasks.push(this);
		mutex.release();
	}
	static function onTask(_window:CefWindow, ?delay:Int = 0) {
		if (_window == null) return;
		var _task:Task = null;
		mutex.acquire();
		for (i in 0...tasks.length) {
			var t_task:Task = tasks[i];
			if (_window!=null&&t_task.window!=null&&t_task.window == _window && t_task.delay == delay) {
				_task = t_task;
				break;
			}
		}
		if (_task != null) tasks.remove(_task);
		mutex.release();
		if (_task!=null&&_task.call != null)_task.call(_task.args);
	}
}