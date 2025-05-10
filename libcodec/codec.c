#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include "codec.h"

int compile_and_run(const char* code) {
    printf("\n--- Código C recibido ---\n%s\n-----------------------\n", code);

    // 1. Generar nombres únicos para los archivos
    time_t t = time(NULL);
    char source_file[50];
    char binary_file[50];
    snprintf(source_file, sizeof(source_file), "temp_%ld.c", t);
    snprintf(binary_file, sizeof(binary_file), "temp_%ld", t);

    // 2. Crear archivo fuente
    FILE *file = fopen(source_file, "w");
    if (!file) {
        perror("Error al crear archivo fuente");
        return -1;
    }

    printf("\n---Codigo C---\n");
    fprintf(file, "#include <stdio.h>\nint main() {\n%s\nreturn 0;\n}\n", code);
    printf("\n---Fin---\n");  
  fclose(file);
    

    // 3. Compilar
    pid_t pid = fork();
    if (pid == 0) {
        execlp("gcc", "gcc", "-o", binary_file, source_file, NULL);
        perror("Error al compilar");
        exit(EXIT_FAILURE);
    }
    
    int status;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        unlink(source_file); // Eliminar archivo fuente
        return -2; // Error de compilación
    }

    // 4. Ejecutar y capturar salida
    printf("\n--- Salida del programa ---\n");
    pid = fork();
    if (pid == 0) {
        execlp(binary_file, binary_file, NULL);
        perror("Error al ejecutar");
        exit(EXIT_FAILURE);
    }
    
    waitpid(pid, &status, 0);
    printf("\n--------------------------\n");

    // 5. Limpieza
    unlink(source_file);
    unlink(binary_file);

    return WEXITSTATUS(status);
}
