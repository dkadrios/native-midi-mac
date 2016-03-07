package  {
	import flash.display.MovieClip;
	import fs.ane.NativeMIDI;
	import flash.text.TextField;

	public class NativeMIDITest extends MovieClip {

		private var _ane = new NativeMIDI();
		public var tx: TextField;

		public function NativeMIDITest() {
			tx.text = _ane.hello();
		}
	}

}
