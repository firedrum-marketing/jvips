#include "io_github_libvips_Vips.h"
#include <cstdio>
#include <cstdlib>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <vips/vips8>

using vips::VError;
using vips::VImage;

/*
 * Convenience function to determine the extension of a given filename.
 */
const char* getFilenameExtension(const char* filename) {
	const char* dot = strrchr( filename, '.' );
	if( !dot || dot == filename ) return "";
	return dot + 1;
}

/*
 * Convenience function to help throw a RuntimeException.
 *
 * Input:
 *   message       error message
 */
void throwRuntimeException(JNIEnv* env, const char* message) {
	jclass runtimeExceptionClass;
	jmethodID consMethodID = 0;
	jobject newObj;
	jstring jmessage;
	int result;

	/* Find the class ID */
	runtimeExceptionClass = env->FindClass( "java/lang/RuntimeException" );
	if ( runtimeExceptionClass == 0 ) {
		return;
	}

	/* Find the constructor ID */
	consMethodID = env->GetMethodID( runtimeExceptionClass, "<init>", "(Ljava/lang/String;)V" );
	if ( consMethodID == 0 ) {
		return;
	}

	/* Obtain the string objects */
	jmessage = env->NewStringUTF( message );
	if ( jmessage == NULL ) {
		return;
	}

	/* Create the RuntimeException object */
	newObj = env->NewObject( runtimeExceptionClass, consMethodID, jmessage );
	if ( newObj == NULL ) {
		return;
	}

	/* Throw the exception. */
	result = env->Throw( (jthrowable) newObj );
}

/*
 * Class:     io_github_libvips_Vips
 * Method:    init
 * Signature: ()V
 */
extern "C" JNIEXPORT void JNICALL Java_io_github_libvips_Vips_init(JNIEnv *env, jclass cls) {
	vips_init( "jvips" );
}

/*
 * Class:     io_github_libvips_Vips
 * Method:    resizeToFile()
 * Signature: (DDLjava/lang/String;Ljava/lang/String;)V
 */
extern "C" JNIEXPORT void JNICALL Java_io_github_libvips_Vips_resizeToFile(JNIEnv *env, jclass cls, jdouble hscale, jdouble vscale, jstring jinputfilepath, jstring joutputfilepath) {
	const char* inputfilepath =  env->GetStringUTFChars( jinputfilepath, 0 );
	const char* outputfilepath =  env->GetStringUTFChars( joutputfilepath, 0 );
	try {
		VImage in = VImage::new_from_file( inputfilepath, VImage::option()->set( "access", VIPS_ACCESS_SEQUENTIAL )->set( "n", -1 ) );
		VImage out = in.resize( hscale, vscale == -1 ? NULL : VImage::option()->set( "vscale", vscale ) );
		if ( in.get_typeof( VIPS_META_PAGE_HEIGHT ) ) {
			int pageHeight = in.get_int( VIPS_META_PAGE_HEIGHT );
			int numFrames = in.height() / pageHeight;
			out.set( VIPS_META_PAGE_HEIGHT, out.height() / numFrames );
		}

		out.set( "xres", 3.779528 );
		out.set( "yres", 3.779528 );
		out.write_to_file( outputfilepath );
	} catch ( const VError& e ) {
		throwRuntimeException( env, e.what() );
	}
	env->ReleaseStringUTFChars( jinputfilepath, inputfilepath );
	env->ReleaseStringUTFChars( joutputfilepath, outputfilepath );
}

/*
 * Class:     io_github_libvips_Vips
 * Method:    resize()
 * Signature: (DDLjava/lang/String;)[B
 */
extern "C" JNIEXPORT jbyteArray JNICALL Java_io_github_libvips_Vips_resize(JNIEnv *env, jclass cls, jdouble hscale, jdouble vscale, jstring jinputfilepath) {
	size_t size = 0;
	void* buffer = NULL;

	const char* inputfilepath =  env->GetStringUTFChars( jinputfilepath, 0 );
	try {
		VImage in = VImage::new_from_file( inputfilepath, VImage::option()->set( "access", VIPS_ACCESS_SEQUENTIAL )->set( "n", -1 ) );
		VImage out = in.resize( hscale, vscale == -1 ? NULL : VImage::option()->set( "vscale", vscale ) );
		if ( in.get_typeof( VIPS_META_PAGE_HEIGHT ) ) {
			int pageHeight = in.get_int( VIPS_META_PAGE_HEIGHT );
			int numFrames = in.height() / pageHeight;
			out.set( VIPS_META_PAGE_HEIGHT, out.height() / numFrames );
		}

		out.set( "xres", 3.779528 );
		out.set( "yres", 3.779528 );
		out.write_to_buffer( inputfilepath, &buffer, &size, VImage::option()->set( "strip", true ) );
	} catch ( const VError& e ) {
		throwRuntimeException( env, e.what() );
	}
	env->ReleaseStringUTFChars( jinputfilepath, inputfilepath );

	jbyteArray bytes = NULL;
	if ( buffer != NULL ) {
		bytes = env->NewByteArray( size );
		env->SetByteArrayRegion( bytes, 0, size, (jbyte*) buffer );
		g_free( buffer );
	}

	return bytes;
}

/*
 * Class:     io_github_libvips_Vips
 * Method:    extractArea()
 * Signature: (IIIILjava/lang/String;)[B
 */
extern "C" JNIEXPORT jbyteArray JNICALL Java_io_github_libvips_Vips_extractArea(JNIEnv *env, jclass cls, jint x, jint y, jint width, jint height, jstring jinputfilepath) {
	size_t size = 0;
	void* buffer = NULL;

	const char* inputfilepath =  env->GetStringUTFChars( jinputfilepath, 0 );
	try {
		VImage in = VImage::new_from_file( inputfilepath, VImage::option()->set( "access", VIPS_ACCESS_SEQUENTIAL )->set( "n", -1 ) );
		VImage out = in.extract_area( x, y, width, height, NULL );
		if ( in.get_typeof( VIPS_META_PAGE_HEIGHT ) ) {
			int pageHeight = in.get_int( VIPS_META_PAGE_HEIGHT );
			int numFrames = in.height() / pageHeight;
			for ( int i = 1; i < numFrames; i++ ) {
				out = out.join( in.extract_area( x, i * pageHeight + y, width, height, NULL ), VIPS_DIRECTION_VERTICAL, NULL );
			}
			out.set( VIPS_META_PAGE_HEIGHT, out.height() / numFrames );
		}

		out.set( "xres", 3.779528 );
		out.set( "yres", 3.779528 );
		out.write_to_buffer( inputfilepath, &buffer, &size, VImage::option()->set( "strip", true ) );
	} catch ( const VError& e ) {
		throwRuntimeException( env, e.what() );
	}
	env->ReleaseStringUTFChars( jinputfilepath, inputfilepath );

	jbyteArray bytes = NULL;
	if ( buffer != NULL ) {
		bytes = env->NewByteArray( size );
		env->SetByteArrayRegion( bytes, 0, size, (jbyte*) buffer );
		g_free( buffer );
	}

	return bytes;
}

/*
 * Class:     io_github_libvips_Vips
 * Method:    resizeForCover()
 * Signature: (DDIILjava/lang/String;)[B
 */
extern "C" JNIEXPORT jbyteArray JNICALL Java_io_github_libvips_Vips_resizeForCover(JNIEnv *env, jclass cls, jdouble hscale, jdouble vscale, jint width, jint height, jstring jinputfilepath) {
	size_t size = 0;
	void* buffer = NULL;

	const char* inputfilepath =  env->GetStringUTFChars( jinputfilepath, 0 );
	try {
		VImage in = VImage::new_from_file( inputfilepath, VImage::option()->set( "access", VIPS_ACCESS_SEQUENTIAL )->set( "n", -1 ) );
		VImage tmp = in.resize( hscale, vscale == -1 ? NULL : VImage::option()->set( "vscale", vscale ) );
		int numFrames = 0;
		if ( in.get_typeof( VIPS_META_PAGE_HEIGHT ) ) {
			int pageHeight = in.get_int( VIPS_META_PAGE_HEIGHT );
			numFrames = in.height() / pageHeight;
			tmp.set( VIPS_META_PAGE_HEIGHT, tmp.height() / numFrames );
		}

		int x = ( tmp.width() - width ) / 2;
		int y = ( tmp.height() - height ) / 2;
		VImage out = tmp.extract_area( x, y, width, height, NULL );
		if ( tmp.get_typeof( VIPS_META_PAGE_HEIGHT ) ) {
			int pageHeight = tmp.get_int( VIPS_META_PAGE_HEIGHT );
			for ( int i = 1; i < numFrames; i++ ) {
				out = out.join( tmp.extract_area( x, i * pageHeight + y, width, height, NULL ), VIPS_DIRECTION_VERTICAL, NULL );
			}
			out.set( VIPS_META_PAGE_HEIGHT, out.height() / numFrames );
		}

		out.set( "xres", 3.779528 );
		out.set( "yres", 3.779528 );
		out.write_to_buffer( inputfilepath, &buffer, &size, VImage::option()->set( "strip", true ) );
	} catch ( const VError& e ) {
		throwRuntimeException( env, e.what() );
	}
	env->ReleaseStringUTFChars( jinputfilepath, inputfilepath );

	jbyteArray bytes = NULL;
	if ( buffer != NULL ) {
		bytes = env->NewByteArray( size );
		env->SetByteArrayRegion( bytes, 0, size, (jbyte*) buffer );
		g_free( buffer );
	}

	return bytes;
}

/*
 * Class:     io_github_libvips_Vips
 * Method:    convertSVGtoPNG()
 * Signature: ([B)[B
 */
extern "C" JNIEXPORT jbyteArray JNICALL Java_io_github_libvips_Vips_convertSVGtoPNG(JNIEnv *env, jclass cls, jbyteArray jsvgdata) {
	size_t size = 0;
	void* buffer = NULL;

	jbyte* svgdata =  env->GetByteArrayElements( jsvgdata, 0 );
	const jsize svgdatalength = env->GetArrayLength( jsvgdata );

	try {
		VImage out = VImage::new_from_buffer( svgdata, svgdatalength, "" );
		out.set( "xres", 3.779528 );
		out.set( "yres", 3.779528 );
		out.write_to_buffer( ".png", &buffer, &size, VImage::option()->set( "strip", true ) );
	} catch ( const VError& e ) {
		throwRuntimeException( env, e.what() );
	}

	env->ReleaseByteArrayElements( jsvgdata, svgdata, JNI_ABORT );

	jbyteArray bytes = NULL;
	if ( buffer != NULL ) {
		bytes = env->NewByteArray( size );
		env->SetByteArrayRegion( bytes, 0, size, (jbyte*) buffer );
		g_free( buffer );
	}

	return bytes;
}

/*
 * Class:     io_github_libvips_Vips
 * Method:    convertImageToSRGBAndStrip()
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */
extern "C" JNIEXPORT void JNICALL Java_io_github_libvips_Vips_convertImageToSRGBAndStrip(JNIEnv *env, jclass cls, jstring jinputfilepath, jstring jcmykiccprofilepath, jstring jsrgbiccprofilepath) {
	const char* inputfilepath =  env->GetStringUTFChars( jinputfilepath, 0 );
	const char* cmykiccprofilepath =  env->GetStringUTFChars( jcmykiccprofilepath, 0 );
	const char* srgbiccprofilepath =  env->GetStringUTFChars( jsrgbiccprofilepath, 0 );

	FILE* sourceFile = fopen( inputfilepath , "rb" );
	char* buffer[1024];
	if ( sourceFile  != NULL ) {
		const char* extension = getFilenameExtension( inputfilepath );
		size_t extensionLength = strlen( extension );
		char* temporaryFileName = (char*) malloc( strlen( inputfilepath ) + extensionLength + 9 );
		strcpy( temporaryFileName, inputfilepath );
#ifndef HAVE_MKSTEMPS
		strcat( temporaryFileName, ".XXXXXX" );
		int tmpFile = mkstemp( temporaryFileName );
#else
		strcat( temporaryFileName, ".XXXXXX." );
		strcat( temporaryFileName, extension );
		int tmpFile = mkstemps( temporaryFileName, extensionLength + 1 );
#endif
		while (! feof( sourceFile ) ) {
			size_t bytesRead = fread( buffer, sizeof( char ), sizeof( buffer ), sourceFile );
			if ( bytesRead == 0 ) {
				break;
			}
			write( tmpFile, buffer, bytesRead );
		}

		close( tmpFile );
		fclose( sourceFile );

#ifndef HAVE_MKSTEMPS
		char* temporaryFileName2 = (char*) malloc( strlen( inputfilepath ) + extensionLength + 9 );
		strcpy( temporaryFileName2, temporaryFileName );
		strcat( temporaryFileName, "." );
		strcat( temporaryFileName, extension );
		rename( temporaryFileName2, temporaryFileName );
		free( temporaryFileName2 );
#endif
		try {
			VImage in = VImage::new_from_file( temporaryFileName, VImage::option()->set( "access", VIPS_ACCESS_SEQUENTIAL )->set( "n", -1 ) );
			bool hasEmbeddedICCProfile = in.get_typeof( VIPS_META_ICC_NAME ) != 0;
			bool applyRGBProfile = in.interpretation() != VIPS_INTERPRETATION_sRGB || !hasEmbeddedICCProfile;

			VImage out;
			if ( applyRGBProfile ) {
				if ( hasEmbeddedICCProfile ) {
					try {
						out = in.icc_transform( (char*) srgbiccprofilepath, VImage::option()->set( "embedded", true )->set( "intent", VIPS_INTENT_PERCEPTUAL ) );
					} catch (...) {
						// Ignore failure of embedded profile
					}
					out = out.colourspace( VIPS_INTERPRETATION_sRGB );
				} else if ( in.interpretation() == VIPS_INTERPRETATION_CMYK ) {
					out = in.icc_transform( (char*) srgbiccprofilepath, VImage::option()->set( "input_profile", cmykiccprofilepath )->set( "intent", VIPS_INTENT_PERCEPTUAL ) ).colourspace( VIPS_INTERPRETATION_sRGB );
				} else {
					out = in.colourspace( VIPS_INTERPRETATION_sRGB );
				}
			} else {
				out = in;
			}

			out.set( "xres", 3.779528 );
			out.set( "yres", 3.779528 );
			out.write_to_file( inputfilepath, VImage::option()->set( "strip", true ) );
		} catch ( const VError& e ) {
			throwRuntimeException( env, e.what() );
		}

		unlink( temporaryFileName );
		free( temporaryFileName );
	}

	env->ReleaseStringUTFChars( jinputfilepath, inputfilepath );
	env->ReleaseStringUTFChars( jcmykiccprofilepath, cmykiccprofilepath );
	env->ReleaseStringUTFChars( jsrgbiccprofilepath, srgbiccprofilepath );
}

/*
 * Class:     io_github_libvips_Vips
 * Method:    getDimension
 * Signature: (Ljava/lang/String;)Ljava/awt/Dimension;
 */
extern "C" JNIEXPORT jobject JNICALL Java_io_github_libvips_Vips_getDimension(JNIEnv *env, jclass vipsClass, jstring jinputfilepath) {
	jclass dimensionClass = env->FindClass( "java/awt/Dimension" );
	if ( dimensionClass == NULL) {
		throwRuntimeException( env, "Unable to locate class java.awt.Dimension" );
		return NULL;
	}

	jmethodID consMethodID = env->GetMethodID( dimensionClass, "<init>", "(II)V" );
	if ( consMethodID == NULL ) {
		throwRuntimeException( env, "Unable to construct java.awt.Dimension");
		return NULL;
	}

	const char* inputfilepath =  env->GetStringUTFChars( jinputfilepath, 0 );

	jobject dimension = NULL;
	try {
		VImage in = VImage::new_from_file( inputfilepath, VImage::option()->set( "access", VIPS_ACCESS_SEQUENTIAL ) );
		dimension = env->NewObject( dimensionClass, consMethodID, in.width(), in.height() );
		if ( dimension == NULL ) {
			throwRuntimeException( env, "Unable to construct java.awt.Dimension" );
		}
	} catch ( const VError& e ) {
		throwRuntimeException( env, e.what() );
	}

	env->ReleaseStringUTFChars( jinputfilepath, inputfilepath );

	return dimension;
}
