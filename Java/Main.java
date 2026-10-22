import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;
import java.util.Locale;
import java.util.Queue;
import java.util.LinkedList;
import java.util.Stack;
import java.util.Scanner;

public class Main {

    static final String ARCHIVO_ALUMNOS = "alumnos.txt";
    static final String ARCHIVO_PROFESORES = "profesores.txt";
    static final String ARCHIVO_CERTIFICADOS = "certificados_pendientes.txt";

    static Stack<RegistroNota> historial = new Stack<>();

    static class RegistroNota {
        String cedula;
        int indice;
        String valorAnterior;
        String valorNuevo;

        RegistroNota(String cedula, int indice,
                     String anterior, String nuevo) {
            this.cedula = cedula;
            this.indice = indice;
            this.valorAnterior = anterior;
            this.valorNuevo = nuevo;
        }
    }

    public static void main(String[] args) {
        Scanner teclado = new Scanner(System.in);
        int opcion = 0;

        while (opcion != 7) {
            System.out.println("\n==================================");
            System.out.println(" SGA-DO | MENU PRINCIPAL");
            System.out.println("==================================");
            System.out.println("1. Inscribir alumno");
            System.out.println("2. Inscribir profesor");
            System.out.println("3. Ingresar calificaciones");
            System.out.println("4. Revertir ultima calificacion");
            System.out.println("5. Procesar certificados");
            System.out.println("6. Consultar reporte general");
            System.out.println("7. Finalizar");
            System.out.println("==================================");
            System.out.print("Opcion: ");

            if (!teclado.hasNextInt()) {
                System.out.println("Introduzca una opcion numerica.");
                teclado.nextLine();
                continue;
            }

            opcion = teclado.nextInt();
            teclado.nextLine();

            switch (opcion) {
                case 1:
                    registrarAlumno(teclado);
                    break;
                case 2:
                    registrarProfesor(teclado);
                    break;
                case 3:
                    registrarNota(teclado);
                    break;
                case 4:
                    deshacerNota();
                    break;
                case 5:
                    generarCertificados();
                    break;
                case 6:
                    mostrarReporte();
                    break;
                case 7:
                    System.out.println("Sistema cerrado correctamente.");
                    break;
                default:
                    System.out.println("Seleccione una opcion del 1 al 7.");
            }
        }

        teclado.close();
    }

    // Comprueba si una identificacion ya existe en un archivo.
    static boolean existeCedula(String archivo, String cedula)
            throws IOException {

        Path ruta = Path.of(archivo);

        if (!Files.exists(ruta)) {
            return false;
        }

        for (String linea : Files.readAllLines(ruta)) {
            String[] campos = linea.split(",", -1);

            if (campos.length > 0 && campos[0].equals(cedula)) {
                return true;
            }
        }

        return false;
    }

    // Guarda un registro al final del archivo correspondiente.
    static void guardarRegistro(String archivo, String registro)
            throws IOException {

        try (FileWriter salida = new FileWriter(archivo, true)) {
            salida.write(registro + System.lineSeparator());
        }
    }

    public static void registrarAlumno(Scanner teclado) {
        System.out.println("\n--- NUEVA INSCRIPCION DE ALUMNO ---");

        System.out.print("Cedula: ");
        String cedula = teclado.nextLine().trim();

        if (cedula.isEmpty() || cedula.contains(",")) {
            System.out.println("Cedula no valida.");
            return;
        }

        try {
            if (existeCedula(ARCHIVO_ALUMNOS, cedula)) {
                System.out.println("Esta cedula ya esta registrada.");
                return;
            }
        } catch (IOException e) {
            System.out.println("No fue posible consultar los alumnos.");
            return;
        }

        System.out.print("Nombre completo: ");
        String nombre = teclado.nextLine().trim();

        System.out.print("Correo electronico: ");
        String correo = teclado.nextLine().trim();

        if (!campoValido(nombre) || !campoValido(correo)) {
            System.out.println("Nombre o correo no valido.");
            return;
        }

        String programa;

        while (true) {
            System.out.println("\nPrograma academico");
            System.out.println("1. Curso");
            System.out.println("2. Diplomado");
            System.out.println("3. Bootcamp");
            System.out.print("Seleccione: ");

            String seleccion = teclado.nextLine().trim();

            if (seleccion.equals("1")) {
                programa = "Curso";
                break;
            } else if (seleccion.equals("2")) {
                programa = "Diplomado";
                break;
            } else if (seleccion.equals("3")) {
                programa = "Bootcamp";
                break;
            } else {
                System.out.println("Opcion incorrecta. Intente de nuevo.");
            }
        }

        // El guion representa una nota pendiente.
        String registro = String.join(",",
                cedula, nombre, correo, programa, "-", "-", "-");

        try {
            guardarRegistro(ARCHIVO_ALUMNOS, registro);
            System.out.println("Alumno inscrito correctamente.");
        } catch (IOException e) {
            System.out.println("No se pudo guardar el alumno.");
        }
    }

    public static void registrarProfesor(Scanner teclado) {
        System.out.println("\n--- NUEVA INSCRIPCION DE PROFESOR ---");

        System.out.print("Cedula: ");
        String cedula = teclado.nextLine().trim();

        if (cedula.isEmpty() || cedula.contains(",")) {
            System.out.println("Cedula no valida.");
            return;
        }

        try {
            if (existeCedula(ARCHIVO_PROFESORES, cedula)) {
                System.out.println("Esta cedula ya esta registrada.");
                return;
            }
        } catch (IOException e) {
            System.out.println("No fue posible consultar los profesores.");
            return;
        }

        System.out.print("Nombre completo: ");
        String nombre = teclado.nextLine().trim();

        System.out.print("Correo electronico: ");
        String correo = teclado.nextLine().trim();

        System.out.print("Especialidad: ");
        String especialidad = teclado.nextLine().trim();

        System.out.print("Materia asignada: ");
        String materia = teclado.nextLine().trim();

        if (!campoValido(nombre) || !campoValido(correo)
                || !campoValido(especialidad) || !campoValido(materia)) {
            System.out.println("Todos los campos son obligatorios.");
            return;
        }

        String registro = String.join(",",
                cedula, nombre, correo, especialidad, materia);

        try {
            guardarRegistro(ARCHIVO_PROFESORES, registro);
            System.out.println("Profesor registrado correctamente.");
        } catch (IOException e) {
            System.out.println("No se pudo guardar el profesor.");
        }
    }

    static boolean campoValido(String texto) {
        return texto != null
                && !texto.trim().isEmpty()
                && !texto.contains(",");
    }

    public static void registrarNota(Scanner teclado) {
        System.out.println("\n--- INGRESO DE CALIFICACIONES ---");

        Path ruta = Path.of(ARCHIVO_ALUMNOS);

        if (!Files.exists(ruta)) {
            System.out.println("Todavia no hay alumnos registrados.");
            return;
        }

        System.out.print("Cedula del alumno: ");
        String cedula = teclado.nextLine().trim();

        try {
            List<String> lineas = Files.readAllLines(ruta);

            for (int i = 0; i < lineas.size(); i++) {
                String[] datos = lineas.get(i).split(",", -1);

                if (datos.length != 7 || !datos[0].equals(cedula)) {
                    continue;
                }

                System.out.println("Alumno: " + datos[1]);
                System.out.println("Programa: " + datos[3]);

                int posicion = -1;

                for (int j = 4; j < 7; j++) {
                    System.out.println("Nota " + (j - 3) + ": "
                            + (datos[j].equals("-")
                            ? "Pendiente" : datos[j]));

                    if (posicion == -1 && datos[j].equals("-")) {
                        posicion = j;
                    }
                }

                if (posicion == -1) {
                    System.out.println("El alumno ya tiene tres notas.");
                    return;
                }

                System.out.print("Nueva nota (0 a 20): ");
                String entradaNota = teclado.nextLine().trim();

                double nota;

                try {
                    nota = Double.parseDouble(
                            entradaNota.replace(',', '.'));
                } catch (NumberFormatException e) {
                    System.out.println("Debe introducir un numero valido.");
                    return;
                }

                if (!Double.isFinite(nota) || nota < 0 || nota > 20) {
                    System.out.println("La nota debe estar entre 0 y 20.");
                    return;
                }

                String anterior = datos[posicion];
                String nueva = String.valueOf(nota);

                datos[posicion] = nueva;
                lineas.set(i, String.join(",", datos));

                Files.write(ruta, lineas);

                historial.push(new RegistroNota(
                        cedula, posicion, anterior, nueva));

                System.out.println("Calificacion guardada correctamente.");
                return;
            }

            System.out.println("No se encontro el alumno indicado.");

        } catch (IOException e) {
            System.out.println("Error al actualizar las calificaciones.");
        }
    }

    public static void deshacerNota() {
        if (historial.empty()) {
            System.out.println("No hay calificaciones que deshacer.");
            return;
        }

        RegistroNota ultima = historial.peek();
        Path ruta = Path.of(ARCHIVO_ALUMNOS);

        try {
            if (!Files.exists(ruta)) {
                System.out.println("No existe el archivo de alumnos.");
                return;
            }

            List<String> lineas = Files.readAllLines(ruta);

            for (int i = 0; i < lineas.size(); i++) {
                String[] datos = lineas.get(i).split(",", -1);

                if (datos.length != 7 || !datos[0].equals(ultima.cedula)) {
                    continue;
                }

                if (!datos[ultima.indice].equals(ultima.valorNuevo)) {
                    System.out.println(
                            "No se puede revertir: la nota fue modificada.");
                    return;
                }

                datos[ultima.indice] = ultima.valorAnterior;
                lineas.set(i, String.join(",", datos));

                Files.write(ruta, lineas);
                historial.pop();

                System.out.println("Ultima calificacion revertida.");
                return;
            }

            System.out.println("No se encontro el alumno de la operacion.");

        } catch (IOException e) {
            System.out.println("Error al revertir la calificacion.");
        }
    }

    public static void generarCertificados() {
        Path ruta = Path.of(ARCHIVO_ALUMNOS);
        Queue<String> cola = new LinkedList<>();

        if (!Files.exists(ruta)) {
            System.out.println("No existe el archivo de alumnos.");
            return;
        }

        try {
            List<String> lineas = Files.readAllLines(ruta);

            for (String linea : lineas) {
                String[] datos = linea.split(",", -1);

                if (datos.length != 7) {
                    continue;
                }

                // No procesar registros con notas pendientes.
                if (datos[4].equals("-") || datos[5].equals("-")
                        || datos[6].equals("-")) {
                    continue;
                }

                double n1, n2, n3;

                try {
                    n1 = Double.parseDouble(datos[4]);
                    n2 = Double.parseDouble(datos[5]);
                    n3 = Double.parseDouble(datos[6]);
                } catch (NumberFormatException e) {
                    System.out.println(
                            "Notas invalidas para: " + datos[0]);
                    continue;
                }

                if (!Double.isFinite(n1) || !Double.isFinite(n2)
                        || !Double.isFinite(n3)
                        || n1 < 0 || n1 > 20
                        || n2 < 0 || n2 > 20
                        || n3 < 0 || n3 > 20) {
                    continue;
                }

                double promedio = (n1 + n2 + n3) / 3.0;
                boolean aprobado;

                switch (datos[3].toLowerCase(Locale.ROOT)) {
                    case "curso":
                        aprobado = promedio >= 10;
                        break;

                    case "diplomado":
                        aprobado = promedio >= 14;
                        break;

                    case "bootcamp":
                        aprobado = n1 >= 14 && n2 >= 14 && n3 >= 14;
                        break;

                    default:
                        System.out.println(
                                "Programa desconocido: " + datos[0]);
                        continue;
                }

                if (aprobado) {
                    cola.offer(datos[0] + "," + datos[1] + "," + datos[3]);
                }
            }

            try (FileWriter salida =
                    new FileWriter(ARCHIVO_CERTIFICADOS, false)) {
                while (!cola.isEmpty()) {
                    salida.write(cola.poll() + System.lineSeparator());
                }
            }

            System.out.println("Proceso de certificados completado.");
            System.out.println("Archivo generado: " + ARCHIVO_CERTIFICADOS);

        } catch (IOException e) {
            System.out.println("Error al generar los certificados.");
        }
    }

    public static void mostrarReporte() {
        System.out.println("\n========== REPORTE GENERAL ==========");

        mostrarAlumnos();
        mostrarProfesores();

        System.out.println("\n========== FIN DEL REPORTE ==========");
    }

    static void mostrarAlumnos() {
        System.out.println("\n--- ALUMNOS REGISTRADOS ---");

        Path ruta = Path.of(ARCHIVO_ALUMNOS);

        if (!Files.exists(ruta)) {
            System.out.println("No hay archivo de alumnos.");
            return;
        }

        try {
            List<String> lineas = Files.readAllLines(ruta);

            if (lineas.isEmpty()) {
                System.out.println("No hay alumnos registrados.");
                return;
            }

            for (String linea : lineas) {
                String[] datos = linea.split(",", -1);

                if (datos.length != 7) {
                    System.out.println("Registro de alumno invalido.");
                    continue;
                }

                System.out.println("\nCedula: " + datos[0]);
                System.out.println("Nombre: " + datos[1]);
                System.out.println("Correo: " + datos[2]);
                System.out.println("Programa: " + datos[3]);

                boolean completa = true;
                double[] notas = new double[3];

                for (int j = 0; j < 3; j++) {
                    String valor = datos[j + 4];

                    System.out.println("Nota " + (j + 1) + ": "
                            + (valor.equals("-") ? "Pendiente" : valor));

                    if (valor.equals("-")) {
                        completa = false;
                    } else {
                        try {
                            notas[j] = Double.parseDouble(valor);

                            if (!Double.isFinite(notas[j])
                                    || notas[j] < 0 || notas[j] > 20) {
                                completa = false;
                            }
                        } catch (NumberFormatException e) {
                            completa = false;
                        }
                    }
                }

                if (!completa) {
                    System.out.println("Estado: notas incompletas o invalidas");
                    continue;
                }

                double promedio = (notas[0] + notas[1] + notas[2]) / 3.0;

                System.out.printf(Locale.ROOT,
                        "Promedio: %.2f/20%n", promedio);

                boolean aprobado;

                switch (datos[3].toLowerCase(Locale.ROOT)) {
                    case "curso":
                        aprobado = promedio >= 10;
                        break;

                    case "diplomado":
                        aprobado = promedio >= 14;
                        break;

                    case "bootcamp":
                        aprobado = notas[0] >= 14
                                && notas[1] >= 14
                                && notas[2] >= 14;
                        break;

                    default:
                        System.out.println("Estado: programa desconocido");
                        continue;
                }

                System.out.println("Estado: "
                        + (aprobado ? "APROBADO" : "NO APROBADO"));
            }

        } catch (IOException e) {
            System.out.println("No fue posible leer los alumnos.");
        }
    }

    static void mostrarProfesores() {
        System.out.println("\n--- PROFESORES REGISTRADOS ---");

        Path ruta = Path.of(ARCHIVO_PROFESORES);

        if (!Files.exists(ruta)) {
            System.out.println("No hay archivo de profesores.");
            return;
        }

        try {
            List<String> lineas = Files.readAllLines(ruta);

            if (lineas.isEmpty()) {
                System.out.println("No hay profesores registrados.");
                return;
            }

            for (String linea : lineas) {
                String[] datos = linea.split(",", -1);

                if (datos.length != 5) {
                    System.out.println("Registro de profesor invalido.");
                    continue;
                }

                System.out.println("\nCedula: " + datos[0]);
                System.out.println("Nombre: " + datos[1]);
                System.out.println("Correo: " + datos[2]);
                System.out.println("Especialidad: " + datos[3]);
                System.out.println("Materia: " + datos[4]);
            }

        } catch (IOException e) {
            System.out.println("No fue posible leer los profesores.");
        }
    }
}

