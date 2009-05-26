package test;

import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

import types.Message;

public class TestSocket {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		try {
			Socket s = new Socket("127.0.0.1", 9527);
			InputStream in = s.getInputStream();
			OutputStream out = s.getOutputStream();

			out.write(Message.COMM_MSG_GAME_REQUIRE_START);

			// s.close();
			while (true) {
				int r = in.read();
				System.out.println(r & 0xff);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

	}

}
