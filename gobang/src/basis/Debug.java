package basis;

import java.io.PrintStream;
import java.util.Collection;

/**
 * Provide routines to debug <tt>Fatworm</tt>.
 * 
 * @author xrchen
 */
public final class Debug {
	private static final int MAX_FLAGS = 0x80;
	private static boolean[] flags;

	private Debug() {
	}

	/**
	 * Initialize the debugger with <tt>debugFlags</tt>, enables all debug
	 * flags in it.
	 * 
	 * @param debugFlags
	 */
	public static void initialize(String debugFlags) {
		flags = new boolean[MAX_FLAGS];
		char[] charArray = debugFlags.toCharArray();
		for (char f : charArray)
			if (0 <= f && f < MAX_FLAGS)
				flags[(int) f] = true;
	}

	/**
	 * Test if <tt>flag</tt> was enabled.
	 * 
	 * @param flag
	 * @return <tt>true</tt> if flag was enabled.
	 */
	public static boolean testFlag(char flag) {
		if (flags == null)
			return false;
		else if (flags[(int) '+'])
			return true;
		else
			return 0 <= flag && flag < MAX_FLAGS && flags[(int) flag];
	}

	/**
	 * Print <i>message</i> if <i>flag</i> was enabled on the command line,
	 * using the specified output stream.
	 * 
	 * @param flag
	 *            the debug flag that must be set to print this message.
	 * @param message
	 *            the debug message.
	 * @param out
	 *            the output stream to print.
	 */
	public static void debug(char flag, String message, PrintStream out) {
		if (testFlag(flag))
			out.println(message);
	}

	/**
	 * Print <i>message</i> if <i>flag</i> was enabled on the command line.
	 * 
	 * @param flag
	 *            the debug flag that must be set to print this message.
	 * @param message
	 *            the debug message.
	 */
	public static void debug(char flag, String message) {
		if (testFlag(flag))
			System.out.println(message);
	}

	/**
	 * Asserts that <i>expression</i> is <tt>true</tt>. If not, then Fatworm
	 * exits with an error message.
	 * 
	 * @param expression
	 *            the expression to assert.
	 */
	public static void assertTrue(boolean expression) {
		if (!expression)
			throw new AssertionFailureError();
	}

	/**
	 * Asserts that <i>expression</i> is <tt>true</tt>. If not, then Fatworm
	 * exits with the specified error message.
	 * 
	 * @param expression
	 *            the expression to assert.
	 * @param message
	 *            the error message.
	 */
	public static void assertTrue(boolean expression, String msg) {
		if (!expression)
			throw new AssertionFailureError(msg);
	}

	/**
	 * Asserts that this call is never made. Same as <tt>assertTrue(false)</tt>.
	 */
	public static void assertNotReached() {
		assertTrue(false);
	}

	/**
	 * Asserts that this call is never made, with the specified error message.
	 * Same as <tt>assertTrue(false, message)</tt>.
	 * 
	 * @param message
	 *            the error message.
	 */
	public static void assertNotReached(String msg) {
		assertTrue(false, msg);
	}

	public static String getString(Object obj, Object... objects) {
		StringBuilder sb = new StringBuilder();
		sb.append(obj.getClass().getSimpleName());
		sb.append("(");
		for (int i = 0; i < objects.length; ++i) {
			if (i > 0)
				sb.append(",");
			sb.append(objects[i]);
		}
		sb.append(")");
		return sb.toString();
	}

	@SuppressWarnings("unchecked")
	public static String getStringFromCollection(Collection collect) {
		StringBuilder sb = new StringBuilder();
		boolean first = true;
		sb.append(collect.getClass().getSimpleName() + "(");
		for (Object obj : collect) {
			if (first)
				first = false;
			else
				sb.append(",");
			sb.append(obj);
		}
		sb.append(")");
		return sb.toString();
	}

	public static void displayContent(byte[] rec) {
		for (int i = 0; i < rec.length; ++i) {
			if (i % 32 == 0)
				System.out.println();
			System.out.print(toHex(rec[i]) + " ");
		}
		System.out.println();
	}

	private static String toHex(byte b) {
		String ret = Integer.toHexString(b & 0xff);
		if (ret.length() == 1)
			ret = "0" + ret;
		return ret;
	}
}

@SuppressWarnings("serial")
class AssertionFailureError extends Error {
	AssertionFailureError() {
		super();
	}

	AssertionFailureError(String message) {
		super(message);
	}
}
