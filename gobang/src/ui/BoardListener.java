package ui;

public interface BoardListener {
	public void onClick(byte color, byte row, byte col);

	public void reset(byte wins);
}
