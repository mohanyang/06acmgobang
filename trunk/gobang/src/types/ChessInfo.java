package types;

public class ChessInfo {
	// color of the chess
	public byte color;

	// row of the chess
	public byte row;

	// col of the chess
	public byte col;

	public ChessInfo(byte color, int info) {
		this.color = color;
		col = (byte) (info >> 8);
		row = (byte) (info);
	}

	public ChessInfo(byte color, byte row, byte col) {
		this.color = color;
		this.row = row;
		this.col = col;
	}

	public byte[] toBytes() {
		byte[] ret = new byte[8];
		ret[1] = (byte) color;
		ret[2] = (byte) row;
		ret[3] = (byte) col;
		return ret;
	}

	public int toInt() {
		return (((int) col) << 8) | ((int) row);
	}

	public String toString() {
		if (color == Color.BLACK)
			return "BLACK Chess@[" + col + ", " + row + "]";
		else if (color == Color.WHITE)
			return "WHITE Chess@[" + col + ", " + row + "]";
		else
			return "NONE  Chess@[" + col + ", " + row + "]";
	}
}
