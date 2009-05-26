package gobang;

import types.ChessInfo;
import types.Color;
import ui.BoardListener;
import ui.ChessFrame;

public class HumanPlayer implements BoardListener, Runnable {

	private byte color = Color.BLACK;

	private ChessInfo lastMove;

	private ChessFrame cf = null;
	private JNIAdapter jni = null;

	public HumanPlayer() {
		jni = new JNIAdapter();
		cf = new ChessFrame(false);
		cf.addBoardListener(this);
	}

	public void run() {
		color = Color.BLACK;
		jni.reset();
		cf.reset();
		while (true) {
			humanPlay();
			AIPlay();
		}
	}

	synchronized void AIPlay() {
		cf.waitResponse();
		lastMove = new ChessInfo(color, jni.GenerateChessInfo());
		jni.playChess(lastMove.toInt());
		cf.drawChess(lastMove.col, lastMove.row, lastMove.color);
		color = (byte) (Color.SUM - lastMove.color);
	}

	synchronized void humanPlay() {
		try {
			waitResult = true;
			wait();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		jni.playChess(lastMove.toInt());
		color = (byte) (Color.SUM - lastMove.color);
	}

	private boolean waitResult = false;

	@Override
	public synchronized void onClick(byte color, byte row, byte col) {
		if (waitResult) {
			waitResult = false;
			lastMove = new ChessInfo(color, row, col);
			notify();
		} else
			lastMove = null;
	}
}
