package ui;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class ImageLoader {

	public static BufferedImage systemIcon = null;
	private static ImageLoader instance = null;

	private ImageLoader() {

	}

	public static synchronized ImageLoader getInstance() {
		if (instance == null) {
			instance = new ImageLoader();
			systemIcon = loadImageByName("icon.png");
		}
		return instance;
	}

	public static BufferedImage loadImageByName(String name) {
		try {
			return ImageIO.read(new File("./image/" + name));
		} catch (IOException e) {
			e.printStackTrace();
			return null;
		}
	}
}
