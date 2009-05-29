package gobang;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintStream;

import javax.swing.JOptionPane;

import types.ChessInfo;
import types.Color;
import ui.BoardListener;
import ui.ChessFrame;

public class HumanPlayer implements BoardListener {

	private byte color = Color.BLACK;

	private ChessInfo lastMove;

	private ChessFrame cf = null;
	private JNIAdapter jni = null;
	private boolean isOver = false;
	private byte wins = Color.NONE;

	private int mode = 0;
	private PrintStream out = null;

	public HumanPlayer(int mode) {
		this.mode = mode;
		if (mode > 0)
			jni = new JNIAdapter();
		cf = new ChessFrame(false, "human player");
		cf.addBoardListener(this);
		try {
			out = new PrintStream(new File("log.txt"));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}

	public void run() {
		playerReset();
		while (true) {
			switch (mode) {
			case 0:
				humanPlay();
				if (isOver) {
					judgeOver();
					break;
				}
				humanPlay();
				if (isOver) {
					judgeOver();
				}
				break;
			case 1:
				humanPlay();
				if (isOver) {
					judgeOver();
					break;
				}
				AIPlay();
				if (isOver) {
					judgeOver();
				}
				break;
			case 2:
				AIPlay();
				if (isOver) {
					judgeOver();
					break;
				}
				humanPlay();
				if (isOver) {
					judgeOver();
				}
				break;
			default:
				return;
			}
		}
	}

	synchronized void judgeOver() {
		if (isOver) {
			out.println((wins == Color.WHITE ? "WHITE" : "BLACK") + " wins");
			int n = JOptionPane.showConfirmDialog(null,
					(wins == Color.WHITE ? "WHITE" : "BLACK")
							+ " wins!\nClick Y to restart, N to exit.",
					"Gobang", JOptionPane.YES_NO_OPTION);
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
		lastMove = new ChessInfo(color, jni.generateChessInfo());
		out.println(lastMove);
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
		if (!isOver) {
			out.println(lastMove);
			System.out.println(lastMove);
			if (jni != null)
				jni.playChess(lastMove.toInt());
			color = (byte) (Color.SUM - lastMove.color);
		}
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
