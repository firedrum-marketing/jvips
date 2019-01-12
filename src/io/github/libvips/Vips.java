package io.github.libvips;

import java.awt.Dimension;

public class Vips {
	static {
		try {
			System.loadLibrary( "JVips" );
			init();
		} catch ( UnsatisfiedLinkError e ) {
			e.printStackTrace();
		}
	}

	private Vips() {
	}

	/**
	 * Initializes the VIPS system
	 */
	private static native void init();

	/**
	 * Resizes an image file.
	 *
	 * @param horizontalScaleFactor
	 *            the horizontal scaling factor
	 * @param verticalScaleFactor
	 *            the vertical scaling factor
	 * @param inputFilePath
	 *            the input filename
	 * @param outputFilePath
	 *            the output filename
	 */
	public static native void resizeToFile(final double horizontalScaleFactor, final double verticalScaleFactor, final String inputFilePath, final String outputFilePath);

	/**
	 * Resizes an image file.
	 *
	 * @param horizontalScaleFactor
	 *            the horizontal scaling factor
	 * @param verticalScaleFactor
	 *            the vertical scaling factor
	 * @param inputFilePath
	 *            the input filename
	 * @return the image data
	 */
	public static native byte[] resize(final double horizontalScaleFactor, final double verticalScaleFactor, final String inputFilePath);

	/**
	 * Extracts a portion of an image file.
	 *
	 * @param xOffset
	 *            the horizontal offset for extraction, from the left
	 * @param yOffset
	 *            the vertical offset for extraction, from the top
	 * @param width
	 *            the number of pixel columns to extract
	 * @param height
	 *            the number of pixel rows to extract
	 * @param inputFilePath
	 *            the input filename
	 * @return the image data
	 */
	public static native byte[] extractArea(final int xOffset, final int yOffset, final int width, final int height, final String inputFilePath);

	/**
	 * Resizes an image file and extracts a portion suitable for use in CSS "cover" scenarios.
	 *
	 * @param horizontalScaleFactor
	 *            the horizontal scaling factor
	 * @param verticalScaleFactor
	 *            the vertical scaling factor
	 * @param width
	 *            the number of pixel columns to extract
	 * @param height
	 *            the number of pixel rows to extract
	 * @param inputFilePath
	 *            the input filename
	 * @return the image data
	 */
	public static native byte[] resizeForCover(final double horizontalScaleFactor, final double verticalScaleFactor, final int width, final int height, final String inputFilePath);

	/**
	 * Converts an image file to sRGB colorspace and strips metadata.
	 *
	 * @param inputFilePath
	 *            the input filename
	 */
	public static native void convertImageToSRGBAndStrip(final String inputFilePath);

	/**
	 * Retrieves an image file's dimensions.
	 *
	 * @param inputFilePath
	 *            the input filename
	 */
	public static native Dimension getDimension(final String inputFilePath);
}
