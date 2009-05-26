package gobang;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

import types.ChessInfo;
import types.Color;
import types.Message;
import types.Status;
import ui.ChessFrame;
import basis.Config;
import basis.Debug;

public class SocketPlayer implements Runnable {
	private Socket socket = null;
	private InputStream sin;
	private OutputStream sout;

	private byte color = Color.NONE;
	private byte curColor = Color.BLACK;

	private ChessInfo lastMove;

	private int status;
	private ChessFrame cf = null;
	private JNIAdapter jni = null;

	public SocketPlayer() {
		try {
			socket = new Socket(Config.getString("ip"), Config
					.getInteger("port"));
			Debug.assertTrue(socket != null);
			sin = socket.getInputStream();
			sout = socket.getOutputStream();
			jni = new JNIAdapter();
			cf = new ChessFrame(true);
		} catch (IOException e) {
			e.printStackTrace();
		}
		System.out.println("socket player " + this + " connected");
	}

	public void run() {
		while (true) {
			System.out.println(status);
			switch (status) {
			case Status.UNSTART:
				handleUnstart();
				break;
			case Status.STARTED:
				handleStarted();
				break;
			case Status.WAITRES:
				handleWaitRes();
				break;
			case Status.SOLPROB:
				handleSolProb();
				break;
			case Status.TIMEOUT:
				handleTimeout();
				break;
			default:
				Debug.assertNotReached();
			}
		}
	}

	void handleUnstart() {
		byte msg = readByte();
		switch (msg) {
		case Message.COMM_MSG_GAME_START:
			Debug.assertTrue(color != Color.NONE);
			status = Status.STARTED;
			curColor = Color.BLACK;
			jni.reset();
			cf.reset();
			break;
		case Message.COMM_MSG_FIRST:
			color = Color.BLACK;
			System.out.println("player color " + color);
			writeByte(Message.COMM_MSG_GAME_REQUIRE_START);
			break;
		case Message.COMM_MSG_SECOND:
			color = Color.WHITE;
			System.out.println("player color " + color);
			writeByte(Message.COMM_MSG_GAME_REQUIRE_START);
			break;
		}
	}

	void handleStarted() {
		System.err.println(curColor == color);
		if (curColor == color) {
			ChessInfo info = new ChessInfo(color, jni.GenerateChessInfo());
			status = Status.WAITRES;
			writeChessInfo(info);
		} else {
			status = Status.WAITRES;
		}
	}

	void handleWaitRes() {
		byte msg = readByte();
		switch (msg) {
		case Message.COMM_MSG_REJECTED:
			handleSolProb();
			break;
		case Message.COMM_MSG_ACCEPTED:
			status = Status.STARTED;
			break;
		case Message.COMM_MSG_CHESS:
			handleChessmove();
			break;
		case Message.COMM_MSG_TIMEOUT:
			handleTimeout();
			break;
		case Message.COMM_MSG_WIN:
		case Message.COMM_MSG_DRAW:
		case Message.COMM_MSG_LOSE:
			handleOver();
			break;
		}
	}

	void handleOver() {
		jni.reset();
		cf.reset();
		status = Status.UNSTART;
		curColor = Color.BLACK;
	}

	void handleSolProb() {
		// TODO: problem unhandled
	}

	void handleChessmove() {
		readChessInfo();
		System.out.println("chess move " + lastMove.col + " " + lastMove.row
				+ " color " + lastMove.color);
		cf.drawChess(lastMove.col, lastMove.row, lastMove.color);
		jni.playChess(lastMove.toInt());
		curColor = (byte) (Color.SUM - lastMove.color);
	}

	void handleTimeout() {
		if (readByte() == Message.COMM_MSG_LOSE)
			handleOver();
	}

	byte readByte() {
		byte res = 0;
		try {
			res = (byte) sin.read();
			System.out.println("read " + res);
		} catch (IOException ioe) {
			ioe.printStackTrace();
		}
		return res;
	}

	void writeByte(byte b) {
		try {
			sout.write(b);
		} catch (IOException ioe) {
			ioe.printStackTrace();
		}
	}

	ChessInfo readChessInfo() {
		try {
			byte[] buf = new byte[8];
			sin.read(buf);
			return lastMove = new ChessInfo(buf[1], buf[2], buf[3]);
		} catch (IOException e) {
			return null;
		}
	}

	void writeChessInfo(ChessInfo info) {
		writeByte(Message.COMM_MSG_CHESS);
		try {
			sout.write(info.toBytes());
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	protected void finalize() throws Throwable {
		super.finalize();
		try {
			sout.close();
			sin.close();
			socket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
