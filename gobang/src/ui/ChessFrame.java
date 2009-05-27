package ui;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Toolkit;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.UIManager;

public class ChessFrame extends JFrame {
	static public final long serialVersionUID = 0;

	private static final int defaultWidth = 400, defaultHeight = 420;
	ChessBoardPanel chessBoardPanel = null;
	JPanel infoPanel = null;
	JLabel side = null;
	JList list = null;
	JTextArea text = null;

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
		chessBoardPanel = new ChessBoardPanel(15, 15, auto);

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

	public synchronized void drawChess(int x, int y, byte color,
			boolean internal) {
		chessBoardPanel.drawChess(x, y, color, internal);
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
