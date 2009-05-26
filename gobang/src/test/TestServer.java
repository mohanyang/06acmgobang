package test;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;

public class TestServer {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		try {
			ServerSocket s = new ServerSocket(9527);
			Integer i = 1;

			while (true) {
				final Socket incoming = s.accept();
				System.out.println("spanning " + i);
				Runnable r = new Runnable() {
					public void run() {
						try {
							InputStream inStream = incoming.getInputStream();
							OutputStream outStream = incoming.getOutputStream();
							Scanner in = new Scanner(inStream);
							PrintWriter out = new PrintWriter(outStream, true);
							out.println("Hello! Enter byte to exit!");

							boolean done = false;
							while (!done && in.hasNextLine()) {
								String line = in.nextLine();
								out.println("Echo " + line);
							}
						} catch (IOException e) {
							e.printStackTrace();
						}
					}
				};
				Thread t = new Thread(r);
				t.start();
				++i;
			}

		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
