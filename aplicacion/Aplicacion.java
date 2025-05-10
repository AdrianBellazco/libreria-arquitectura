package aplicacion;

import libcodec.JavaCodec;

public class Aplicacion {
    public static void main(String[] args) {
        // Cargar la librería nativa (si es JNI)
        // System.loadLibrary("codec");  // Descomenta si es necesario

        JavaCodec codec = new JavaCodec();

        // Usar un text block (Java 15+) para el código C
        String codigo = """
            #include <stdio.h>
            int main() {
                int num1, num2;
                printf("Ingrese primer número: ");
                fflush(stdout);
                scanf("%d", &num1);
                printf("Ingrese segundo número: ");
                fflush(stdout);
                scanf("%d", &num2);
                printf("Suma: %d\\n", num1 + num2);
                return 0;
            }
            """;

        String resultado = codec.codigo(codigo);
        System.out.println(resultado);
    }
}
