package fs.ane
{
	import flash.events.EventDispatcher;
	import flash.events.IEventDispatcher;
	import flash.external.ExtensionContext;

	public class NativeMIDI extends EventDispatcher
	{
		private var _ctx: ExtensionContext = null;

		public function NativeMIDI(target:IEventDispatcher=null)
		{
			super(target);
			_ctx = ExtensionContext.createExtensionContext("fs.ane.NativeMIDI", null);
		}

		public function dispose(): void {
			_ctx.dispose();
		}

		public function hello(): String {
			return String(_ctx.call("hello"));
		}
	}
}
