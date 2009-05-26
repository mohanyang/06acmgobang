package gobang;

import types.ChessInfo;

public interface IPlayer {

	public void start(byte color);

	public void accept();

	public void reject();

	public void win();

	public void lose();

	public void draw();

	public ChessInfo getNextMove();

	public void validateNextMove(ChessInfo info);
}
