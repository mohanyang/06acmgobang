package basis;

import java.util.Random;

public class Lib {
	/**
	 * Prevent instantiation.
	 */
	private Lib() {
	}

	private static final double epsilon = 1e-2;
	private static final double amplitude = 1 / epsilon;

	/**
	 * Test if two double are equal
	 * 
	 * @param a
	 * @param b
	 */
	public static boolean isEqual(double a, double b) {
		return Double.compare(a, b) == 0;
	}

	public static int doubleToInt(double a) {
		return (int) (a * amplitude);
	}

	private static Random random = null;

	/**
	 * Seed the random number generater. May only be called once.
	 * 
	 * @param randomSeed
	 *            the seed for the random number generator.
	 */
	public static void seedRandom(long randomSeed) {
		Debug.assertTrue(random == null);
		random = new Random(randomSeed);
	}

	/**
	 * Return a random integer between 0 and <i>range - 1</i>. Must not be
	 * called before <tt>seedRandom()</tt> seeds the random number generator.
	 * 
	 * @param range
	 *            a positive value specifying the number of possible return
	 *            values.
	 * @return a random integer in the specified range.
	 */
	public static int random(int range) {
		Debug.assertTrue(range > 0);
		return random.nextInt(range);
	}

	/**
	 * Return a random double between 0.0 (inclusive) and 1.0 (exclusive).
	 * 
	 * @return a random double between 0.0 and 1.0.
	 */
	public static double random() {
		return random.nextDouble();
	}

	/**
	 * Convert a short into its little-endian byte string representation.
	 * 
	 * @param array
	 *            the array in which to store the byte string.
	 * @param offset
	 *            the offset in the array where the string will start.
	 * @param value
	 *            the value to convert.
	 */
	public static void bytesFromShort(byte[] array, int offset, short value) {
		array[offset + 0] = (byte) ((value >> 0) & 0xFF);
		array[offset + 1] = (byte) ((value >> 8) & 0xFF);
	}

	/**
	 * Convert an int into its little-endian byte string representation.
	 * 
	 * @param array
	 *            the array in which to store the byte string.
	 * @param offset
	 *            the offset in the array where the string will start.
	 * @param value
	 *            the value to convert.
	 */
	public static void bytesFromInt(byte[] array, int offset, int value) {
		array[offset + 0] = (byte) ((value >> 0) & 0xFF);
		array[offset + 1] = (byte) ((value >> 8) & 0xFF);
		array[offset + 2] = (byte) ((value >> 16) & 0xFF);
		array[offset + 3] = (byte) ((value >> 24) & 0xFF);
	}

	/**
	 * Convert an int into its little-endian byte string representation, and
	 * return an array containing it.
	 * 
	 * @param value
	 *            the value to convert.
	 * @return an array containing the byte string.
	 */
	public static byte[] bytesFromInt(int value) {
		byte[] array = new byte[4];
		bytesFromInt(array, 0, value);
		return array;
	}

	public static byte[] bytesFromString(String str, int length) {
		byte[] array = new byte[length];
		for (int i = 0; i < length; ++i)
			array[i] = (byte) str.charAt(i);
		return array;
	}

	public static void bytesFromLong(byte[] array, int offset, long value) {
		array[offset + 0] = (byte) ((value >> 0) & 0xFF);
		array[offset + 1] = (byte) ((value >> 8) & 0xFF);
		array[offset + 2] = (byte) ((value >> 16) & 0xFF);
		array[offset + 3] = (byte) ((value >> 24) & 0xFF);
		array[offset + 4] = (byte) ((value >> 32) & 0xFF);
		array[offset + 5] = (byte) ((value >> 40) & 0xFF);
		array[offset + 6] = (byte) ((value >> 48) & 0xFF);
		array[offset + 7] = (byte) ((value >> 56) & 0xFF);
	}

	public static byte[] bytesFromLong(long value) {
		byte[] array = new byte[8];
		bytesFromLong(array, 0, value);
		return array;
	}

	/**
	 * Convert an int into a little-endian byte string representation of the
	 * specified length.
	 * 
	 * @param array
	 *            the array in which to store the byte string.
	 * @param offset
	 *            the offset in the array where the string will start.
	 * @param length
	 *            the number of bytes to store (must be 1, 2, or 4).
	 * @param value
	 *            the value to convert.
	 */
	public static void bytesFromInt(byte[] array, int offset, int length,
			int value) {
		Debug.assertTrue(length == 1 || length == 2 || length == 4);

		switch (length) {
		case 1:
			array[offset] = (byte) value;
			break;
		case 2:
			bytesFromShort(array, offset, (short) value);
			break;
		case 4:
			bytesFromInt(array, offset, value);
			break;
		}
	}

	/**
	 * Convert to a short from its little-endian byte string representation.
	 * 
	 * @param array
	 *            the array containing the byte string.
	 * @param offset
	 *            the offset of the byte string in the array.
	 * @return the corresponding short value.
	 */
	public static short bytesToShort(byte[] array, int offset) {
		return (short) ((((short) array[offset + 0] & 0xFF) << 0) | (((short) array[offset + 1] & 0xFF) << 8));
	}

	/**
	 * Convert to an unsigned short from its little-endian byte string
	 * representation.
	 * 
	 * @param array
	 *            the array containing the byte string.
	 * @param offset
	 *            the offset of the byte string in the array.
	 * @return the corresponding short value.
	 */
	public static int bytesToUnsignedShort(byte[] array, int offset) {
		return (((int) bytesToShort(array, offset)) & 0xFFFF);
	}

	/**
	 * Convert to an int from its little-endian byte string representation.
	 * 
	 * @param array
	 *            the array containing the byte string.
	 * @param offset
	 *            the offset of the byte string in the array.
	 * @return the corresponding int value.
	 */
	public static int bytesToInt(byte[] array, int offset) {
		return (int) ((((int) array[offset + 0] & 0xFF) << 0)
				| (((int) array[offset + 1] & 0xFF) << 8)
				| (((int) array[offset + 2] & 0xFF) << 16) | (((int) array[offset + 3] & 0xFF) << 24));
	}

	public static long bytesToLong(byte[] array, int offset) {
		return (long) ((((long) array[offset + 0] & 0xFF) << 0)
				| (((long) array[offset + 1] & 0xFF) << 8)
				| (((long) array[offset + 2] & 0xFF) << 16)
				| (((long) array[offset + 3] & 0xFF) << 24)
				| (((long) array[offset + 4] & 0xFF) << 32)
				| (((long) array[offset + 5] & 0xFF) << 40)
				| (((long) array[offset + 6] & 0xFF) << 48) | (((long) array[offset + 7] & 0xFF) << 56));
	}

	/**
	 * Convert to an int from a little-endian byte string representation of the
	 * specified length.
	 * 
	 * @param array
	 *            the array containing the byte string.
	 * @param offset
	 *            the offset of the byte string in the array.
	 * @param length
	 *            the length of the byte string.
	 * @return the corresponding value.
	 */
	public static int bytesToInt(byte[] array, int offset, int length) {
		Debug.assertTrue(length == 1 || length == 2 || length == 4);

		switch (length) {
		case 1:
			return array[offset];
		case 2:
			return bytesToShort(array, offset);
		case 4:
			return bytesToInt(array, offset);
		default:
			return -1;
		}
	}

	/**
	 * Convert to a string from a possibly null-terminated array of bytes.
	 * 
	 * @param array
	 *            the array containing the byte string.
	 * @param offset
	 *            the offset of the byte string in the array.
	 * @param length
	 *            the maximum length of the byte string.
	 * @return a string containing the specified bytes, up to and not including
	 *         the null-terminator (if present).
	 */
	public static String bytesToString(byte[] array, int offset, int length) {
		int i;
		for (i = 0; i < length; i++) {
			if (array[offset + i] == 0)
				break;
		}

		return new String(array, offset, i);
	}

	/**
	 * Mask out and shift a bit substring.
	 * 
	 * @param bits
	 *            the bit string.
	 * @param lowest
	 *            the first bit of the substring within the string.
	 * @param size
	 *            the number of bits in the substring.
	 * @return the substring.
	 */
	public static int extract(int bits, int lowest, int size) {
		if (size == 32)
			return (bits >> lowest);
		else
			return ((bits >> lowest) & ((1 << size) - 1));
	}

	/**
	 * Mask out and shift a bit substring.
	 * 
	 * @param bits
	 *            the bit string.
	 * @param lowest
	 *            the first bit of the substring within the string.
	 * @param size
	 *            the number of bits in the substring.
	 * @return the substring.
	 */
	public static long extract(long bits, int lowest, int size) {
		if (size == 64)
			return (bits >> lowest);
		else
			return ((bits >> lowest) & ((1L << size) - 1));
	}

	/**
	 * Mask out and shift a bit substring; then sign extend the substring.
	 * 
	 * @param bits
	 *            the bit string.
	 * @param lowest
	 *            the first bit of the substring within the string.
	 * @param size
	 *            the number of bits in the substring.
	 * @return the substring, sign-extended.
	 */
	public static int extend(int bits, int lowest, int size) {
		int extra = 32 - (lowest + size);
		return ((extract(bits, lowest, size) << extra) >> extra);
	}

	/**
	 * Test if a bit is set in a bit string.
	 * 
	 * @param flag
	 *            the flag to test.
	 * @param bits
	 *            the bit string.
	 * @return <tt>true</tt> if <tt>(bits & flag)</tt> is non-zero.
	 */
	public static boolean test(long flag, long bits) {
		return ((bits & flag) != 0);
	}

	/**
	 * Creates a padded upper-case string representation of the integer argument
	 * in base 16.
	 * 
	 * @param i
	 *            an integer.
	 * @return a padded upper-case string representation in base 16.
	 */
	public static String toHexString(int i) {
		return toHexString(i, 8);
	}

	/**
	 * Creates a padded upper-case string representation of the integer argument
	 * in base 16, padding to at most the specified number of digits.
	 * 
	 * @param i
	 *            an integer.
	 * @param pad
	 *            the minimum number of hex digits to pad to.
	 * @return a padded upper-case string representation in base 16.
	 */
	public static String toHexString(int i, int pad) {
		String result = Integer.toHexString(i).toUpperCase();
		while (result.length() < pad)
			result = "0" + result;
		return result;
	}

	/**
	 * Divide two non-negative integers, round the quotient up to the nearest
	 * integer, and return it.
	 * 
	 * @param a
	 *            the numerator.
	 * @param b
	 *            the denominator.
	 * @return <tt>ceiling(a / b)</tt>.
	 */
	public static int divRoundUp(int a, int b) {
		Debug.assertTrue(a >= 0 && b > 0);

		return ((a + (b - 1)) / b);
	}

	/**
	 * Load and return the named class, or return <tt>null</tt> if the class
	 * could not be loaded.
	 * 
	 * @param className
	 *            the name of the class to load.
	 * @return the loaded class, or <tt>null</tt> if an error occurred.
	 */
	public static Class tryLoadClass(String className) {
		try {
			return ClassLoader.getSystemClassLoader().loadClass(className);
		} catch (Throwable e) {
			return null;
		}
	}

	/**
	 * Load and return the named class
	 * 
	 * @param className
	 *            the name of the class to load.
	 * @return the loaded class.
	 */
	public static Class loadClass(String className) {
		try {
			return ClassLoader.getSystemClassLoader().loadClass(className);
		} catch (Throwable e) {
			return null;
		}
	}

	/**
	 * Create and return a new instance of the named class, using the
	 * constructor that takes no arguments.
	 * 
	 * @param className
	 *            the name of the class to instantiate.
	 * @return a new instance of the class.
	 */
	public static Object constructObject(String className) {
		try {
			Class[] param_types = new Class[0];
			Object[] params = new Object[0];
			return loadClass(className).getConstructor(param_types)
					.newInstance(params);
		} catch (Throwable e) {
			return null;
		}
	}

	/**
	 * Create and return a new instance of the named class, using the
	 * constructor that takes params as arguments.
	 * 
	 * @param className
	 *            the name of the class to instantiate.
	 * @param params
	 *            the parameters needed for the instantiation of the class
	 * @return a new instance of the class.
	 */
	public static Object constructObject(String className, Object[] params) {
		try {
			Class[] param_types = new Class[0];
			return loadClass(className).getConstructor(param_types)
					.newInstance(params);
		} catch (Throwable e) {
			return null;
		}
	}

	public static int alignTo(int length, int unit) {
		if (length % unit == 0)
			return length / unit;
		else
			return length / unit + 1;
	}
}
