/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class io_github_libvips_Vips */

#ifndef _Included_io_github_libvips_Vips
#define _Included_io_github_libvips_Vips
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     io_github_libvips_Vips
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_io_github_libvips_Vips_init
  (JNIEnv *, jclass);

/*
 * Class:     io_github_libvips_Vips
 * Method:    resizeToFile
 * Signature: (DDLjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_io_github_libvips_Vips_resizeToFile
  (JNIEnv *, jclass, jdouble, jdouble, jstring, jstring);

/*
 * Class:     io_github_libvips_Vips
 * Method:    resize
 * Signature: (DDLjava/lang/String;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_io_github_libvips_Vips_resize
  (JNIEnv *, jclass, jdouble, jdouble, jstring);

/*
 * Class:     io_github_libvips_Vips
 * Method:    convertImageToSRGBAndStrip
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_io_github_libvips_Vips_convertImageToSRGBAndStrip
  (JNIEnv *, jclass, jstring);

/*
 * Class:     io_github_libvips_Vips
 * Method:    getDimension
 * Signature: (Ljava/lang/String;)Ljava/awt/Dimension;
 */
JNIEXPORT jobject JNICALL Java_io_github_libvips_Vips_getDimension
  (JNIEnv *, jclass, jstring);

#ifdef __cplusplus
}
#endif
#endif
