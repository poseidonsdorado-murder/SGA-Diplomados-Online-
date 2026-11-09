#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

// Estructura para registrar los cambios en el historial de notas (Pila LIFO)
struct HistorialNota {
    string cedula;
    int indiceNota;
    string notaAnterior;
    string notaNueva;
};

// Pila global para la opción de deshacer
stack<HistorialNota> pilaHistorial;

// Función auxiliar para dividir cadenas por comas (reutilizable y limpia)
vector<string> dividirLinea(const string& linea, char delimitador = ',') {
    vector<string> campos;
    stringstream ss(linea);
    string campo;
    while (getline(ss, campo, delimitador)) {
        campos.push_back(campo);
    }
    return campos;
}

// Función auxiliar para verificar duplicados de cédulas en archivos
bool cedulaExiste(const string& nombreArchivo, const string& cedulaBuscada) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) return false;
    
    string linea;
    while (getline(archivo, linea)) {
        vector<string> datos = dividirLinea(linea);
        if (!datos.empty() && datos[0] == cedulaBuscada) {
            archivo.close();
            return true;
        }
    }
    archivo.close();
    return false;
}

void registrarAlumno() {
    cout << "\n========================================" << endl;
    cout << " INSCRIPCION DE ALUMNO " << endl;
    cout << "========================================" << endl;

    string cedula, nombre, correo, opcionProg, programa;

    cout << "Ingrese la cedula: ";
    getline(cin, cedula);
    cout << "Ingrese el nombre: ";
    getline(cin, nombre);
    cout << "Ingrese el correo: ";
    getline(cin, correo);

    do {
        cout << "\n--- PROGRAMAS DISPONIBLES ---" << endl;
        cout << "1. Curso" << endl;
        cout << "2. Diplomado" << endl;
        cout << "3. Bootcamp" << endl;
        cout << "Seleccione una opcion (1-3): ";
        getline(cin, opcionProg);

        if (opcionProg == "1") programa = "Curso";
        else if (opcionProg == "2") programa = "Diplomado";
        else if (opcionProg == "3") programa = "Bootcamp";
        else cout << "❌ Opcion invalida. Intente nuevamente." << endl;
    } while (programa.empty());

    if (cedula.empty() || nombre.empty() || correo.empty() ||
        cedula.find(',') != string::npos || nombre.find(',') != string::npos || correo.find(',') != string::npos) {
        cout << "❌ Error: Campos vacios o con caracteres no permitidos (comas)." << endl;
        return;
    }

    if (cedulaExiste("alumnos.txt", cedula)) {
        cout << "❌ Error: Ya existe un alumno registrado con esta cedula." << endl;
        return;
    }

    ofstream archivo("alumnos.txt", ios::app);
    if (!archivo.is_open()) {
        cout << "❌ Error al abrir el archivo de alumnos." << endl;
        return;
    }

    archivo << cedula << "," << nombre << "," << correo << "," << programa << ",0,0,0\n";
    archivo.close();

    cout << "\n✓ Alumno registrado exitosamente en el programa: " << programa << endl;
}

void registrarProfesor() {
    cout << "\n========================================" << endl;
    cout << " INSCRIPCION DE PROFESOR " << endl;
    cout << "========================================" << endl;

    string cedula, nombre, correo, especialidad, materia;

    cout << "Ingrese la cedula: ";
    getline(cin, cedula);
    cout << "Ingrese el nombre: ";
    getline(cin, nombre);
    cout << "Ingrese el correo: ";
    getline(cin, correo);
    cout << "Ingrese la especialidad: ";
    getline(cin, especialidad);
    cout << "Ingrese la materia: ";
    getline(cin, materia);

    if (cedula.empty() || nombre.empty() || correo.empty() || especialidad.empty() || materia.empty() ||
        cedula.find(',') != string::npos || nombre.find(',') != string::npos || 
        correo.find(',') != string::npos || especialidad.find(',') != string::npos || materia.find(',') != string::npos) {
        cout << "❌ Error: Campos vacios o con comas no permitidas." << endl;
        return;
    }

    if (cedulaExiste("profesores.txt", cedula)) {
        cout << "❌ Error: Ya existe un profesor con esta cedula." << endl;
        return;
    }

    ofstream archivo("profesores.txt", ios::app);
    if (!archivo.is_open()) {
        cout << "❌ Error al abrir el archivo de profesores." << endl;
        return;
    }

    archivo << cedula << "," << nombre << "," << correo << "," << especialidad << "," << materia << "\n";
    archivo.close();

    cout << "✓ Profesor registrado correctamente." << endl;
}

void registrarNota() {
    cout << "\n========================================" << endl;
    cout << " REGISTRO DE CALIFICACIONES " << endl;
    cout << "========================================" << endl;

    string cedulaBusqueda;
    cout << "Ingrese la cedula del alumno: ";
    getline(cin, cedulaBusqueda);

    ifstream archivoLectura("alumnos.txt");
    if (!archivoLectura.is_open()) {
        cout << "⚠️ No se encontro el archivo de alumnos." << endl;
        return;
    }

    vector<string> lineasAlumnos;
    string linea;
    bool encontrado = false;
    bool notaAgregada = false;
    HistorialNota operacion;

    while (getline(archivoLectura, linea)) {
        vector<string> datos = dividirLinea(linea);

        if (datos.size() != 7 || datos[0] != cedulaBusqueda) {
            lineasAlumnos.push_back(linea);
            continue;
        }

        encontrado = true;
        cout << "\nAlumno encontrado: " << datos[1] << " (" << datos[3] << ")" << endl;
        cout << "Notas actuales -> 1: " << datos[4] << " | 2: " << datos[5] << " | 3: " << datos[6] << endl;

        int indiceNota = -1;
        for (int i = 4; i <= 6; i++) {
            if (datos[i] == "0") {
                indiceNota = i;
                break;
            }
        }

        if (indiceNota == -1) {
            cout << "⚠️ El alumno ya tiene las 3 notas registradas." << endl;
            lineasAlumnos.push_back(linea);
            continue;
        }

        double valorNota;
        cout << "Ingrese la calificacion (0 a 20): ";
        if (!(cin >> valorNota)) {
            cout << "❌ Error: Debe introducir un valor numerico valido." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            lineasAlumnos.push_back(linea);
            archivoLectura.close();
            return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (valorNota < 0 || valorNota > 20) {
            cout << "❌ Error: La nota debe estar en el rango de 0 a 20." << endl;
            lineasAlumnos.push_back(linea);
            archivoLectura.close();
            return;
        }

        stringstream ss;
        ss << fixed << setprecision(2) << valorNota;
        string notaAnterior = datos[indiceNota];
        string notaNueva = ss.str();

        datos[indiceNota] = notaNueva;

        string lineaActualizada = "";
        for (size_t i = 0; i < datos.size(); i++) {
            lineaActualizada += datos[i] + (i + 1 < datos.size() ? "," : "");
        }

        lineasAlumnos.push_back(lineaActualizada);
        
        operacion.cedula = cedulaBusqueda;
        operacion.indiceNota = indiceNota;
        operacion.notaAnterior = notaAnterior;
        operacion.notaNueva = notaNueva;
        notaAgregada = true;

        cout << "✓ Calificacion guardada correctamente." << endl;
    }
    archivoLectura.close();

    if (!encontrado) {
        cout << "❌ No se encontro ningun alumno con la cedula indicada." << endl;
        return;
    }

    if (!notaAgregada) return;

    ofstream archivoEscritura("alumnos.txt");
    if (!archivoEscritura.is_open()) {
        cout << "❌ Error al actualizar el archivo de alumnos." << endl;
        return;
    }

    for (const string& l : lineasAlumnos) {
        archivoEscritura << l << "\n";
    }
    archivoEscritura.close();

    pilaHistorial.push(operacion);
    cout << "Registro almacenado en la pila de historial." << endl;
}

void deshacerUltimaNota() {
    cout << "\n========================================" << endl;
    cout << " DESHACER ULTIMA NOTA (LIFO) " << endl;
    cout << "========================================" << endl;

    if (pilaHistorial.empty()) {
        cout << "⚠️ No hay operaciones pendientes en la pila para deshacer." << endl;
        return;
    }

    HistorialNota ultimaOp = pilaHistorial.top();

    ifstream archivoLectura("alumnos.txt");
    if (!archivoLectura.is_open()) {
        cout << "❌ No se pudo abrir el archivo de alumnos." << endl;
        return;
    }

    vector<string> lineasAlumnos;
    string linea;
    bool modificado = false;

    while (getline(archivoLectura, linea)) {
        vector<string> datos = dividirLinea(linea);

        if (datos.size() == 7 && datos[0] == ultimaOp.cedula) {
            if (datos[ultimaOp.indiceNota] != ultimaOp.notaNueva) {
                cout << "⚠️ Advertencia: La nota fue modificada posteriormente; no se puede deshacer." << endl;
                archivoLectura.close();
                return;
            }

            datos[ultimaOp.indiceNota] = ultimaOp.notaAnterior;

            string lineaReconstruida = "";
            for (size_t i = 0; i < datos.size(); i++) {
                lineaReconstruida += datos[i] + (i + 1 < datos.size() ? "," : "");
            }
            lineasAlumnos.push_back(lineaReconstruida);
            modificado = true;
        } else {
            lineasAlumnos.push_back(linea);
        }
    }
    archivoLectura.close();

    if (!modificado) {
        cout << "❌ No se encontro al alumno asociado en la operacion." << endl;
        return;
    }

    ofstream archivoEscritura("alumnos.txt");
    if (!archivoEscritura.is_open()) {
        cout << "❌ Error al guardar los cambios." << endl;
        return;
    }

    for (const string& l : lineasAlumnos) {
        archivoEscritura << l << "\n";
    }
    archivoEscritura.close();

    pilaHistorial.pop();
    cout << "✓ Ultima operacion deshecha con exito." << endl;
    cout << "Valor restaurado a posicion " << (ultimaOp.indiceNota - 3) << ": " << ultimaOp.notaAnterior << endl;
}

void generarColaCertificados() {
    cout << "\n========================================" << endl;
    cout << " GENERACION DE CERTIFICADOS (FIFO) " << endl;
    cout << "========================================" << endl;

    queue<string> colaGraduandos;
    ifstream archivoAlumnos("alumnos.txt");

    if (!archivoAlumnos.is_open()) {
        cout << "⚠️ No se pudo abrir alumnos.txt" << endl;
        return;
    }

    string linea;
    int totalElegibles = 0;

    while (getline(archivoAlumnos, linea)) {
        vector<string> datos = dividirLinea(linea);
        if (datos.size() != 7) continue;

        if (datos[4] == "0" || datos[5] == "0" || datos[6] == "0") continue;

        double n1 = stod(datos[4]);
        double n2 = stod(datos[5]);
        double n3 = stod(datos[6]);
        double promedio = (n1 + n2 + n3) / 3.0;

        bool aprobado = false;
        string programa = datos[3];

        if (programa == "Curso" && promedio >= 10.0) {
            aprobado = true;
        } else if (programa == "Diplomado" && promedio >= 14.0) {
            aprobado = true;
        } else if (programa == "Bootcamp" && n1 >= 14.0 && n2 >= 14.0 && n3 >= 14.0) {
            aprobado = true;
        }

        if (aprobado) {
            stringstream certStream;
            certStream << datos[0] << "," << datos[1] << "," << programa << "," << fixed << setprecision(2) << promedio;
            colaGraduandos.push(certStream.str());
            totalElegibles++;
        }
    }
    archivoAlumnos.close();

    ofstream archivoCert("certificados_pendientes.txt");
    if (!archivoCert.is_open()) {
        cout << "❌ Error al crear el archivo certificados_pendientes.txt" << endl;
        return;
    }

    cout << "\n--- LISTA DE ALUMNOS APROBADOS (COLA FIFO) ---" << endl;
    while (!colaGraduandos.empty()) {
        string registroCert = colaGraduandos.front();
        archivoCert << registroCert << endl;
        cout << "-> " << registroCert << endl;
        colaGraduandos.pop();
    }
    archivoCert.close();

    cout << "\nTotal de certificados emitidos: " << totalElegibles << endl;
    cout << "Archivo guardado exitosamente: certificados_pendientes.txt" << endl;
}

void reporteGeneral() {
    cout << "\n========================================" << endl;
    cout << " REPORTE GENERAL DEL SISTEMA " << endl;
    cout << "========================================" << endl;

    // --- REPORTE DE ALUMNOS ---
    cout << "\n[ ALUMNOS REGISTRADOS ]" << endl;
    ifstream archivoAlumnos("alumnos.txt");
    if (!archivoAlumnos.is_open()) {
        cout << "⚠️ No hay registros de alumnos disponibles." << endl;
    } else {
        string linea;
        int contadorAlumnos = 0;

        while (getline(archivoAlumnos, linea)) {
            vector<string> datos = dividirLinea(linea);
            if (datos.size() != 7) continue;

            contadorAlumnos++;
            cout << "\nCedula: " << datos[0] << endl;
            cout << "Nombre: " << datos[1] << endl;
            cout << "Correo: " << datos[2] << endl;
            cout << "Programa: " << datos[3] << endl;

            double notas[3];
            double sumaNotas = 0;
            bool completas = true;

            for (int i = 0; i < 3; i++) {
                notas[i] = stod(datos[i + 4]);
                cout << "Nota " << (i + 1) << ": ";
                if (datos[i + 4] == "0") {
                    cout << "Pendiente" << endl;
                    completas = false;
                } else {
                    cout << notas[i] << endl;
                    sumaNotas += notas[i];
                }
            }

            if (!completas) {
                cout << "Promedio: N/A (Notas incompletas)" << endl;
                cout << "Estado: PENDIENTE" << endl;
            } else {
                double promedio = sumaNotas / 3.0;
                cout << "Promedio: " << fixed << setprecision(2) << promedio << endl;

                bool aprobado = false;
                string prog = datos[3];

                if (prog == "Curso" && promedio >= 10.0) aprobado = true;
                else if (prog == "Diplomado" && promedio >= 14.0) aprobado = true;
                else if (prog == "Bootcamp" && notas[0] >= 14.0 && notas[1] >= 14.0 && notas[2] >= 14.0) aprobado = true;

                cout << "Estado: " << (aprobado ? "APROBADO" : "REPROBADO") << endl;
            }
            cout << "----------------------------------------" << endl;
        }
        cout << "Total general de alumnos: " << contadorAlumnos << endl;
        archivoAlumnos.close();
    }

    // --- REPORTE DE PROFESORES ---
    cout << "\n[ PROFESORES REGISTRADOS ]" << endl;
    ifstream archivoProfesores("profesores.txt");
    if (!archivoProfesores.is_open()) {
        cout << "⚠️ No hay registros de profesores disponibles." << endl;
    } else {
        string linea;
        int contadorProfesores = 0;

        while (getline(archivoProfesores, linea)) {
            vector<string> datos = dividirLinea(linea);
            if (datos.size() != 5) continue;

            contadorProfesores++;
            cout << "\nCedula: " << datos[0] << endl;
            cout << "Nombre: " << datos[1] << endl;
            cout << "Correo: " << datos[2] << endl;
            cout << "Especialidad: " << datos[3] << endl;
            cout << "Materia: " << datos[4] << endl;
            cout << "----------------------------------------" << endl;
        }
        cout << "Total general de profesores: " << contadorProfesores << endl;
        archivoProfesores.close();
    }
    cout << "========================================" << endl;
}

int main() {
    int opcion;

    do {
        cout << "\n========================================" << endl;
        cout << " SGA-DO: SISTEMA DE GESTION ACADEMICA " << endl;
        cout << "========================================" << endl;
        cout << "1. Registrar Alumno" << endl;
        cout << "2. Registrar Profesor" << endl;
        cout << "3. Registrar Notas" << endl;
        cout << "4. Deshacer Ultimo Registro de Nota" << endl;
        cout << "5. Cola de Certificados" << endl;
        cout << "6. Reporte General" << endl;
        cout << "7. Salir" << endl;
        cout << "========================================" << endl;
        cout << "Seleccione una opcion (1-7): ";

        if (!(cin >> opcion)) {
            cout << "❌ Error: Debe ingresar un valor numerico valido." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcion) {
            case 1: registrarAlumno(); break;
            case 2: registrarProfesor(); break;
            case 3: registrarNota(); break;
            case 4: deshacerUltimaNota(); break;
            case 5: generarColaCertificados(); break;
            case 6: reporteGeneral(); break;
            case 7: cout << "\n¡Gracias por usar el sistema SGA-DO! Saliendo..." << endl; break;
            default: cout << "❌ Opcion no valida. Seleccione entre 1 y 7." << endl;
        }

    } while (opcion != 7);

    return 0;
}

