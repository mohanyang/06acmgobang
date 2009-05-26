package gobang;

import ui.ImageLoader;
import basis.Config;

public class GoBang {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Config.load("gobang.conf");
		ImageLoader.getInstance();
//		new Thread(new SocketPlayer()).run();
		 new Thread(new HumanPlayer()).run();

		// ChessFrame cf = new ChessFrame();
		// for (int i = 0; i < 15; ++i)
		// for (int j = 0; j < 15; ++j)
		// if ((i + j) % 7 == 0) {
		// cf.drawChess(i, j, (i + j) % 2);
		// try {
		// Thread.sleep(500);
		// } catch (InterruptedException e) {
		// e.printStackTrace();
		// }
		// }
	}
}
