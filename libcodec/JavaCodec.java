package libcodec;

public class JavaCodec {

  static {
System.out.println("Cargando librería nativa...");
        System.loadLibrary("codec");  // Sin "lib" ni ".so"
    }

    public native String codigo(String codigo);
}
