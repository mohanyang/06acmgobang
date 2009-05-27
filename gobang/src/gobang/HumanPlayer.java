package gobang;

import javax.swing.JOptionPane;

import types.ChessInfo;
import types.Color;
import ui.BoardListener;
import ui.ChessFrame;

public class HumanPlayer implements BoardListener, Runnable {

	private byte color = Color.BLACK;

	private ChessInfo lastMove;

	private ChessFrame cf = null;
	private JNIAdapter jni = null;
	private boolean isOver = false;
	private byte wins = Color.NONE;

	private int mode = 0;

	public HumanPlayer(int mode) {
		this.mode = mode;
		if (mode > 0)
			jni = new JNIAdapter();
		cf = new ChessFrame(false, "human player");
		cf.addBoardListener(this);
	}

	public void run() {
		playerReset();
		while (true) {
			switch (mode) {
			case 0:
				humanPlay();
				if (isOver) {
					judgeOver();
					continue;
				}
				humanPlay();
				if (isOver) {
					judgeOver();
					continue;
				}
				break;
			case 1:
				humanPlay();
				if (isOver) {
					judgeOver();
					continue;
				}
				AIPlay();
				if (isOver) {
					judgeOver();
					continue;
				}
				break;
			case 2:
				AIPlay();
				if (isOver) {
					judgeOver();
					continue;
				}
				humanPlay();
				if (isOver) {
					judgeOver();
					continue;
				}
				break;
			default:
				return;
			}
		}
	}

	synchronized void judgeOver() {
		if (isOver) {
			int n = JOptionPane.showConfirmDialog(null,
					(wins == Color.WHITE ? "Whie" : "Black")
							+ " wins! Click y to restart, n to exit.",
					"Gobang", JOptionPane.YES_NO_CANCEL_OPTION);
			if (n == JOptionPane.OK_OPTION) {
				playerReset();
			} else if (n == JOptionPane.NO_OPTION) {
				System.exit(0);
			}
		}
	}

	synchronized void playerReset() {
		color = Color.BLACK;
		if (jni != null)
			jni.reset();
		cf.reset();
		isOver = false;
	}

	synchronized void AIPlay() {
		cf.waitResponse();
		lastMove = new ChessInfo(color, jni.GenerateChessInfo());
		System.out.println(lastMove);
		jni.playChess(lastMove.toInt());
		cf.drawChess(lastMove.col, lastMove.row, lastMove.color, false);
		color = (byte) (Color.SUM - lastMove.color);
	}

	synchronized void humanPlay() {
		try {
			waitResult = true;
			wait();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		System.out.println(lastMove);
		if (jni != null)
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

	@Override
	public synchronized void reset(byte wins) {
		this.wins = wins;
		isOver = true;
		notifyAll();
	}
}
