package gobang;

import ui.ImageLoader;
import basis.Config;

public class GoBang {

	public static void main(String[] args) {
		Config.load("gobang.conf");
		ImageLoader.getInstance();
		int mode = Config.getInteger("mode");
		if (mode == 3) {
			new Thread(new SocketPlayer()).run();
		} else {
			new Thread(new HumanPlayer(mode)).run();
		}
	}
}
