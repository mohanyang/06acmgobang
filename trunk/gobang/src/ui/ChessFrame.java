package ui;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Toolkit;

import javax.swing.JFrame;
import javax.swing.UIManager;

public class ChessFrame extends JFrame {
	static public final long serialVersionUID = 0;

	private static final int defaultWidth = 500, defaultHeight = 400;
	ChessBoardPanel chessBoardPanel = null;

	public ChessFrame(boolean auto) {
		try {
			UIManager.setLookAndFeel(UIManager
					.getCrossPlatformLookAndFeelClassName());
		} catch (Exception e) {
		}
		setIconImage(ImageLoader.systemIcon);
		setTitle("Gobang");
		setResizable(false);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		chessBoardPanel = new ChessBoardPanel(19, 19, auto);

		getContentPane().setLayout(new BorderLayout());
		getContentPane().add(chessBoardPanel, BorderLayout.CENTER);
		setFocusable(true);
		pack();

		setSize(defaultWidth, defaultHeight);
		centerize();
		setVisible(true);
	}

	private void centerize() {
		Toolkit kit = Toolkit.getDefaultToolkit();
		Dimension screenSize = kit.getScreenSize();
		int screenWidth = screenSize.width / 2;
		int width = getWidth();
		int screenHeight = screenSize.height / 2;
		int height = getHeight();
		setLocation(screenWidth - width / 2, screenHeight - height / 2);
	}

	public synchronized void drawChess(int x, int y, byte color) {
		chessBoardPanel.drawChess(x, y, color);
	}

	public synchronized void reset() {
		chessBoardPanel.reset();
	}

	public synchronized void addBoardListener(BoardListener listener) {
		chessBoardPanel.addListener(listener);
	}

	public synchronized void waitResponse() {
		chessBoardPanel.waitResponse();
	}
}
