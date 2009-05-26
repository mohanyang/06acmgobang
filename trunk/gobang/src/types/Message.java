package types;

public class Message {
	public static String MESSAGENAMES[] = { "COMM_MSG_DONOTHING",
			"COMM_MSG_REJECTED", "COMM_MSG_ACCEPT", "COMM_MSG_FIRST",
			"COMM_MSG_SECOND", "COMM_MSG_GAME_REQUIRE_START",
			"COMM_MSG_GAME_START", "COMM_MSG_CHESS", "COMM_MSG_TIMEOUT",
			"COMM_MSG_WIN", "COMM_MSG_LOSE", "COMM_MSG_DRAW" };

	// do nothing, ignore it!
	public static final byte COMM_MSG_DONOTHING = 0;

	// last request is rejected
	public static final byte COMM_MSG_REJECTED = 1;

	// last request is accepted
	public static final byte COMM_MSG_ACCEPTED = 2;

	// go first when the game starts
	public static final byte COMM_MSG_FIRST = 3;

	// go second when the game starts
	public static final byte COMM_MSG_SECOND = 4;

	// request for game start
	public static final byte COMM_MSG_GAME_REQUIRE_START = 5;

	// game start
	public static final byte COMM_MSG_GAME_START = 6;

	// information for chess
	public static final byte COMM_MSG_CHESS = 7;

	// time out when one peer takes too many time in a bout
	public static final byte COMM_MSG_TIMEOUT = 8;

	// win
	public static final byte COMM_MSG_WIN = 9;

	// lose
	public static final byte COMM_MSG_LOSE = 10;

	// draw
	public static final byte COMM_MSG_DRAW = 11;
}
