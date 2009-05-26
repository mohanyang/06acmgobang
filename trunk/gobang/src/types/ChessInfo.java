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
		row = (byte) (info >> 8);
		col = (byte) (info);
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
		return (((int) color) << 16) | (((int) row) << 8) | ((int) col);
	}
}
