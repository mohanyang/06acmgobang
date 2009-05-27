package test;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.PrintWriter;

public class test {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		try {
			PrintWriter out = new PrintWriter(new File("open.txt"));
			for (int i = 1; i <= 4223; ++i) {
				String name = "open/RenLib0";
				if (i < 10)
					name = name + "000" + i;
				else if (i < 100)
					name = name + "00" + i;
				else if (i < 1000)
					name = name + "0" + i;
				else
					name = name + i;
				name = name + ".txt";
				BufferedReader reader = new BufferedReader(new FileReader(
						new File(name)));
				for (int j = 0; j < 22; ++j)
					reader.readLine();
				String line = null;
				StringBuilder ret = new StringBuilder();
				boolean ok = true;
				while ((line = reader.readLine()) != null && ok) {
					line = line.trim();
					String[] tmp = line.split(" ");
					int cc = 0;
					for (int k = 0; k < tmp.length; ++k) {
						if (tmp[k].length() > 0) {
							++cc;
							if (cc % 2 == 0 && tmp[k].length() > 1) {
								int t = 0;
								try {
									t = Integer.valueOf(tmp[k].substring(1));
									ret.append((char) tmp[k].charAt(0) + ""
											+ (char) ('a' + t - 1));
								} catch (RuntimeException e) {
									ok = false;
								}
							}
						}
					}
				}
				if (ok)
					out.println(ret.toString());
				out.flush();
				reader.close();
			}
			out.close();
		} catch (Exception e) {
			e.printStackTrace();
		}

	}
}
