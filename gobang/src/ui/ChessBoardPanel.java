package ui;

import java.awt.Cursor;
import java.awt.Graphics;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.image.BufferedImage;
import java.util.LinkedList;

import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.UIManager;

import types.Color;
import basis.Debug;

public class ChessBoardPanel extends JPanel implements MouseListener,
		MouseMotionListener {
	static public final long serialVersionUID = 1;

	public static int gridWidth = 24, gridHeight = 24;

	private int width, height, lineLimit;
	private int xl, xr, yl, yr;
	private byte board[][];
	private byte line[];
	private BufferedImage bgImg = null;
	private BufferedImage bg = null;
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
		bChess = ImageLoader.loadImageByName("black.png");
		wChess = ImageLoader.loadImageByName("white.png");
		bgImg = ImageLoader.loadImageByName("board.gif");
		bg = new BufferedImage(400, 400, BufferedImage.TYPE_INT_RGB);

		board = new byte[width][height];
		line = new byte[Math.max(width, height)];
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				board[i][j] = Color.NONE;
		isOver = false;
		xl = gridWidth / 2;
		xr = xl + width * gridWidth;
		yl = gridHeight / 2;
		yr = yl + height * gridHeight;

		color = Color.BLACK;
		cx = new int[500];
		cy = new int[500];
		cc = new byte[500];
		count = 0;

		list = new LinkedList<BoardListener>();
	}

	public void paint(Graphics g) {
		super.paint(g);
		display(false);
	}

	public synchronized void drawChess(int x, int y, byte color,
			boolean internal) {
		Debug.assertTrue(board[x][y] == Color.NONE);
		setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
		board[x][y] = color;
		cx[count] = x * gridWidth + xl;
		cy[count] = y * gridHeight + yl;
		cc[count] = color;
		++count;
		this.color = color;
		display(true);
		this.color = (byte) (Color.SUM - color);
		waitRes = false;

		if (!internal) {
			judge();
			if (isOver) {
				for (BoardListener listener : list)
					listener.reset(wins);
			}
		}
	}

	synchronized void display(boolean newChess) {
		Graphics bf = bg.getGraphics();
		drawGrid(bf);
		for (int i = 0; i < count; ++i)
			if (cc[i] == Color.BLACK) {
				bf.drawImage(bChess, cx[i], cy[i], null);
				bf.setColor(java.awt.Color.WHITE);
				if (i < 10)
					bf.drawString(String.valueOf(i + 1), cx[i] + 8, cy[i] + 16);
				else if (i < 100)
					bf.drawString(String.valueOf(i + 1), cx[i] + 4, cy[i] + 16);
				else
					bf.drawString(String.valueOf(i + 1), cx[i] + 0, cy[i] + 16);
			} else if (cc[i] == Color.WHITE) {
				bf.drawImage(wChess, cx[i], cy[i], null);
				bf.setColor(java.awt.Color.BLACK);
				if (i < 10)
					bf.drawString(String.valueOf(i + 1), cx[i] + 8, cy[i] + 16);
				else if (i < 100)
					bf.drawString(String.valueOf(i + 1), cx[i] + 4, cy[i] + 16);
				else
					bf.drawString(String.valueOf(i + 1), cx[i] + 0, cy[i] + 16);
			}
		bf.setColor(java.awt.Color.BLACK);
		if (newChess)
			bf.drawRoundRect(cx[count - 1], cy[count - 1], gridWidth,
					gridHeight, 5, 5);
		else {
			if (mouseInPanel && gridX >= 0) {
				bf.drawRoundRect(gridX * gridWidth + xl, gridY * gridHeight
						+ yl, gridWidth, gridHeight, 5, 5);
				if (color == Color.BLACK) {
					bf.drawImage(bChess, gridX * gridWidth + xl, gridY
							* gridHeight + yl, null);
				} else if (color == Color.WHITE) {
					bf.drawImage(wChess, gridX * gridWidth + xl, gridY
							* gridHeight + yl, null);
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
		isOver = false;
		Graphics bf = bg.getGraphics();
		drawGrid(bf);
		getGraphics().drawImage(bg, 0, 0, null);
	}

	synchronized void drawGrid(Graphics g) {
		int x1, y1;
		// g.setColor(java.awt.Color.decode("#B8CFE5"));
		// g.fillRect(0, 0, 400, 400);
		g.drawImage(bgImg, 0, 0, null);
		g.setColor(java.awt.Color.BLACK);
		for (int i = 0; i < width; ++i) {
			x1 = (i + 1) * gridHeight;
			g.drawString(String.valueOf((char) (i + 'A')), x1 - 4,
					gridHeight / 2);
			g.drawLine(x1, gridWidth, x1, height * gridWidth);
		}
		for (int i = 0; i < height; ++i) {
			y1 = (i + 1) * gridWidth;
			if (i < 9)
				g.drawString(String.valueOf(i + 1), gridWidth / 2 - 8, y1 + 4);
			else
				g.drawString(String.valueOf(i + 1), gridWidth / 2 - 12, y1 + 4);
			g.drawLine(gridHeight, y1, width * gridHeight, y1);
		}
		for (int i = 1; i <= 3; ++i)
			for (int j = 1; j <= 3; ++j)
				g.fillOval(4 * i * gridHeight - 4, 4 * j * gridWidth - 4, 8, 8);
	}

	boolean mouseInPanel = false;
	boolean waitRes = false;
	int gridX, gridY;
	boolean isOver = false;
	byte wins = Color.NONE;

	@Override
	public void mouseClicked(MouseEvent arg0) {
		handleMouse(arg0.getX(), arg0.getY());
		if (mouseInPanel && !waitRes) {
			if (board[gridX][gridY] != Color.NONE) {
				JOptionPane.showMessageDialog(null,
						"This point has been occupied!", "Gobang",
						JOptionPane.WARNING_MESSAGE);
			} else {
				byte oldColor = color;
				drawChess(gridX, gridY, color, true);
				judge();
				for (BoardListener listener : list)
					listener.onClick(oldColor, (byte) gridY, (byte) gridX);
				if (isOver) {
					for (BoardListener listener : list)
						listener.reset(wins);
				}
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
			if (xx >= xl && xx <= xr && yy >= yl && yy <= yr) {
				gridX = (xx - xl) / gridWidth;
				gridY = (yy - yl) / gridHeight;
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

	void judge() {
		boolean ret = false;
		for (int i = 0; i < height; ++i) {
			System.arraycopy(board[i], 0, line, 0, width);
			lineLimit = width;
			ret = judgeLine();
			if (ret)
				return;
		}
		for (int j = 0; j < width; ++j) {
			for (int i = 0; i < height; ++i)
				line[i] = board[i][j];
			lineLimit = height;
			ret = judgeLine();
			if (ret)
				return;
		}
		for (int k = width - 5; k >= 5 - height; --k) {
			if (k >= 0) {
				lineLimit = height - k;
				for (int i = 0; i < lineLimit; ++i)
					line[i] = board[i][i + k];
			} else {
				lineLimit = width + k;
				for (int i = 0; i < lineLimit; ++i)
					line[i] = board[i - k][i];
			}
			ret = judgeLine();
			if (ret)
				return;
		}

		for (int k = 4; k < width; ++k) {
			lineLimit = k + 1;
			for (int i = 0; i < lineLimit; ++i)
				line[i] = board[i][k - i];
			ret = judgeLine();
			if (ret)
				return;
		}
		for (int k = 1; k <= height - 5; ++k) {
			lineLimit = width - k;
			for (int i = k; i < width; ++i)
				line[i] = board[height - i][i];
			ret = judgeLine();
			if (ret)
				return;
		}
	}

	boolean judgeLine() {
		byte state = Color.NONE;
		int len = 0;
		for (int i = 0; i < lineLimit; ++i)
			if (state == Color.NONE) {
				if (line[i] != Color.NONE) {
					state = line[i];
					len = 1;
				}
			} else if (state == line[i]) {
				++len;
				if (len == 5) {
					isOver = true;
					wins = state;
					return true;
				}
			} else {
				state = line[i];
				if (line[i] != Color.NONE)
					len = 1;
				else
					len = 0;
			}
		return false;
	}
}
