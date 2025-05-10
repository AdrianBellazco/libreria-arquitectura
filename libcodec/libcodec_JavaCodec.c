
#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include "libcodec_JavaCodec.h"




/*
 * Class:     libcodec_JavaCodec
 * Method:    codigo
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */

void limpiarArchivosTemporales() {
    remove("temp_code.c");
    remove("temp_prog");
    remove("compile_error.txt");
}

JNIEXPORT jstring JNICALL Java_libcodec_JavaCodec_codigo
  (JNIEnv * env, jobject obj, jstring codigoJava){

	 const char *codigoC = (*env)->GetStringUTFChars(env, codigoJava, NULL);
    if (codigoC == NULL) {
        limpiarArchivosTemporales();
        return NULL; // Error en conversión
    }

    const char *archivoFuente = "temp_code.c";
    const char *archivoBinario = "temp_prog";
    const char *archivoErrores = "compile_error.txt";

    FILE *archivo = fopen(archivoFuente, "w");
    if (archivo == NULL) {
        (*env)->ReleaseStringUTFChars(env, codigoJava, codigoC);
        limpiarArchivosTemporales();
        return (*env)->NewStringUTF(env, "✗ Error al crear archivo temporal.\n");
    }
 printf("--------Codigo C--------\n\n");
    fprintf(archivo, "%s", codigoC);
    fclose(archivo);
printf("--------Fin del codigo--------\n\n");

    char comandoCompilar[256];
    snprintf(comandoCompilar, sizeof(comandoCompilar), 
             "gcc %s -o %s 2> %s", archivoFuente, archivoBinario, archivoErrores);

    int resultadoCompilacion = system(comandoCompilar);

    char buffer[512];

    if (resultadoCompilacion != 0) {
        FILE *errFile = fopen(archivoErrores, "r");
        if (errFile != NULL) {
            char errores[256];
            fread(errores, 1, sizeof(errores) - 1, errFile);
            errores[255] = '\0';
            fclose(errFile);
            snprintf(buffer, sizeof(buffer), "✗ Error de compilación:\n%s", errores);
        } else {
            snprintf(buffer, sizeof(buffer), "✗ Error de compilación (no se pudo leer el archivo de errores).");
        }

        (*env)->ReleaseStringUTFChars(env, codigoJava, codigoC);
        limpiarArchivosTemporales();
        return (*env)->NewStringUTF(env, buffer);
    }

    char comandoEjecutar[128];
    snprintf(comandoEjecutar, sizeof(comandoEjecutar), "./%s", archivoBinario);
    int resultadoEjecucion = system(comandoEjecutar);

    snprintf(buffer, sizeof(buffer), "✓ Ejecución exitosa. Código retornó: %d", resultadoEjecucion);

    (*env)->ReleaseStringUTFChars(env, codigoJava, codigoC);
    limpiarArchivosTemporales();

    return (*env)->NewStringUTF(env, buffer);

} 
