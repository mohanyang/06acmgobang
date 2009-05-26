package ui;

import java.awt.Cursor;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.image.BufferedImage;
import java.util.LinkedList;

import javax.swing.JPanel;
import javax.swing.UIManager;

import types.Color;

public class ChessBoardPanel extends JPanel implements MouseListener,
		MouseMotionListener {
	static public final long serialVersionUID = 1;

	public static int gridWidth = 24, gridHeight = 24;

	private int width, height;
	private byte board[][];
	private BufferedImage bg = null;
	private BufferedImage bgImg = null;
	private BufferedImage bChess = null, wChess = null;
	private int[] cx = null, cy = null;
	private byte[] cc = null;
	private int count = 0;
	private LinkedList<BoardListener> list = null;

	private byte color = Color.NONE;

	public ChessBoardPanel(int w, int h, boolean auto) {
		try {
			UIManager.setLookAndFeel(UIManager
					.getCrossPlatformLookAndFeelClassName());
		} catch (Exception e) {
		}

		this.width = w;
		this.height = h;

		init();

		if (!auto) {
			addMouseMotionListener(this);
			addMouseListener(this);
		}

		setVisible(true);
	}

	public void init() {
		bgImg = ImageLoader.loadImageByName("board.bmp");
		bChess = ImageLoader.loadImageByName("black.png");
		wChess = ImageLoader.loadImageByName("white.png");
		bg = new BufferedImage(400, 400, BufferedImage.TYPE_INT_RGB);

		board = new byte[width][height];
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				board[i][j] = Color.NONE;

		color = Color.BLACK;
		cx = new int[500];
		cy = new int[500];
		cc = new byte[500];
		count = 0;

		list = new LinkedList<BoardListener>();
	}

	public void paint() {
		Graphics graphics = getGraphics();
		if (graphics != null) {
			Graphics2D bf = (Graphics2D) bg.getGraphics();
			bf.drawImage(bgImg, 0, 0, null);
			graphics.drawImage(bg, 0, 0, null);
		}
	}

	public void paint(Graphics g) {
		super.paint(g);
		drawGrid(g);
	}

	public synchronized void drawChess(int x, int y, byte color) {
		setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
		board[x][y] = color;
		cx[count] = x * gridWidth;
		cy[count] = y * gridHeight;
		cc[count] = color;
		++count;
		this.color = color;
		display(true);
		this.color = (byte) (Color.SUM - color);
		waitRes = false;
	}

	synchronized void display(boolean newChess) {
		Graphics bf = bg.getGraphics();
		drawGrid(bf);
		for (int i = 0; i < count; ++i)
			if (cc[i] == Color.BLACK) {
				bf.drawImage(bChess, cx[i], cy[i], null);
			} else if (cc[i] == Color.WHITE) {
				bf.drawImage(wChess, cx[i], cy[i], null);
			}
		if (newChess)
			bf.drawRoundRect(cx[count - 1], cy[count - 1], gridWidth,
					gridHeight, 5, 5);
		else {
			if (mouseInPanel && gridX >= 0) {
				bf.drawRoundRect(gridX * gridWidth, gridY * gridHeight,
						gridWidth, gridHeight, 5, 5);
				if (color == Color.BLACK) {
					bf.drawImage(bChess, gridX * gridWidth, gridY * gridHeight,
							null);
				} else if (color == Color.WHITE) {
					bf.drawImage(wChess, gridX * gridWidth, gridY * gridHeight,
							null);
				}
			}
		}
		getGraphics().drawImage(bg, 0, 0, null);
	}

	public synchronized void reset() {
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				board[i][j] = Color.NONE;
		count = 0;
		Graphics bf = bg.getGraphics();
		drawGrid(bf);
		getGraphics().drawImage(bg, 0, 0, null);
	}

	synchronized void drawGrid(Graphics g) {
		int x1, y1;
		g.setColor(java.awt.Color.decode("#B8CFE5"));
		g.fillRect(0, 0, 400, 400);
		g.setColor(java.awt.Color.BLACK);
		for (int i = 0; i < 15; ++i) {
			x1 = i * gridWidth + 12;
			g.drawLine(x1, 12, x1, 348);
		}
		for (int i = 0; i < 15; ++i) {
			y1 = i * gridWidth + 12;
			g.drawLine(12, y1, 348, y1);
		}
	}

	boolean mouseInPanel = false;
	boolean waitRes = false;
	int gridX, gridY;

	@Override
	public void mouseClicked(MouseEvent arg0) {
		handleMouse(arg0.getX(), arg0.getY());
		if (mouseInPanel && !waitRes) {
			if (board[gridX][gridY] != Color.NONE) {
				System.out.println("an occupied point");
			} else {
				byte oldColor = color;
				drawChess(gridX, gridY, color);
				for (BoardListener listener : list)
					listener.onClick(oldColor, (byte) gridX, (byte) gridY);
			}
		}
	}

	@Override
	public void mouseEntered(MouseEvent arg0) {
		handleMouse(arg0.getX(), arg0.getY());
	}

	@Override
	public void mouseExited(MouseEvent arg0) {
		handleMouse(arg0.getX(), arg0.getY());
	}

	@Override
	public void mousePressed(MouseEvent arg0) {
	}

	@Override
	public void mouseReleased(MouseEvent arg0) {
	}

	@Override
	public void mouseDragged(MouseEvent arg0) {
	}

	@Override
	public void mouseMoved(MouseEvent arg0) {
		handleMouse(arg0.getX(), arg0.getY());
	}

	public void addListener(BoardListener listener) {
		list.add(listener);
	}

	public void waitResponse() {
		waitRes = true;
		gridX = -1;
		setCursor(new Cursor(Cursor.WAIT_CURSOR));
	}

	void handleMouse(int xx, int yy) {
		if (waitRes == false) {
			if (xx >= 0 && xx <= 360 && yy >= 0 && yy <= 360) {
				gridX = xx / gridWidth;
				gridY = yy / gridHeight;
				setCursor(new Cursor(Cursor.HAND_CURSOR));
				mouseInPanel = true;
			} else {
				gridX = -1;
				setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
				mouseInPanel = false;
			}
			display(false);
		}
	}
}
