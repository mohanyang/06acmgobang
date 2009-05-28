package gobang;

import ui.ImageLoader;
import basis.Config;

public class GoBang {

	public static void main(String[] args) {
		Config.load("gobang.conf");
		ImageLoader.getInstance();
		int mode = Config.getInteger("mode");
		if (mode == 3) {
			new SocketPlayer().run();
		} else {
			new HumanPlayer(mode).run();
		}
	}
}
