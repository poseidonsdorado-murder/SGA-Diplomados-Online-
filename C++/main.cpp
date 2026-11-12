#include <stack>
#include <queue>
#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

// ESTRUCTURA PARA GUARDAR OPERACIONES DE NOTAS
struct OperacionNota {
    string cedula;
    int posicionNota;
    string notaAnterior;
    string notaNueva;
};

// Pila para almacenar las operaciones (LIFO)
stack<OperacionNota> pilaNotas;

// Función auxiliar para dividir cadenas por comas
vector<string> dividirLinea(const string& linea, char delimitador = ',') {
    vector<string> campos;
    stringstream ss(linea);
    string campo;
    while (getline(ss, campo, delimitador)) {
        campos.push_back(campo);
    }
    return campos;
}

// Función auxiliar para verificar si una cédula ya existe de inmediato
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
    string cedula, nombre, correo, programa, opcionProg;

    cout << "\n===== REGISTRO DE ALUMNO =====" << endl;
    cout << "Ingrese la cedula: ";
    getline(cin, cedula);

    if (cedula.empty() || cedula.find(',') != string::npos) {
        cout << "Error: Cedula vacia o con caracteres no permitidos." << endl;
        return;
    }

    // Validación inmediata de cédula duplicada
    if (cedulaExiste("alumnos.txt", cedula)) {
        cout << "Error: Ya existe un alumno registrado con esa cedula." << endl;
        return;
    }

    cout << "Ingrese el nombre: ";
    getline(cin, nombre);

    cout << "Ingrese el correo: ";
    getline(cin, correo);

    // Validación del programa académico
    do {
        cout << "\n--- PROGRAMAS DISPONIBLES ---" << endl;
        cout << "1. Curso" << endl;
        cout << "2. Diplomado" << endl;
        cout << "3. Bootcamp" << endl;

        cout << "Seleccione el programa (1-3): ";
        getline(cin, opcionProg);

        if (opcionProg == "1") programa = "Curso";
        else if (opcionProg == "2") programa = "Diplomado";
        else if (opcionProg == "3") programa = "Bootcamp";
        else cout << "Error: debe seleccionar una opcion valida." << endl;

    } while (programa.empty());

    if (nombre.empty() || correo.empty() ||
        nombre.find(',') != string::npos ||
        correo.find(',') != string::npos) {
        cout << "Error: datos vacios o con comas no permitidas." << endl;
        return;
    }

    ofstream archivo("alumnos.txt", ios::app);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo de alumnos." << endl;
        return;
    }

    archivo << cedula << "," << nombre << "," << correo << "," << programa << ",0,0,0\n";
    archivo.close();

    cout << "\nAlumno registrado correctamente." << endl;
    cout << "Programa asignado: " << programa << endl;
}

void registrarProfesor() {
    string cedula, nombre, correo, especialidad, materia;

    cout << "\n===== REGISTRO DE PROFESOR =====" << endl;
    cout << "Ingrese la cedula: ";
    getline(cin, cedula);

    if (cedula.empty() || cedula.find(',') != string::npos) {
        cout << "Error: Cedula vacia o con caracteres no permitidos." << endl;
        return;
    }

    // Validación inmediata de cédula duplicada
    if (cedulaExiste("profesores.txt", cedula)) {
        cout << "Error: Ya existe un profesor con esa cedula." << endl;
        return;
    }

    cout << "Ingrese el nombre: ";
    getline(cin, nombre);
    cout << "Ingrese el correo: ";
    getline(cin, correo);
    cout << "Ingrese la especialidad: ";
    getline(cin, especialidad);
    cout << "Ingrese la materia: ";
    getline(cin, materia);

    if (nombre.empty() || correo.empty() || especialidad.empty() ||
        materia.empty() || nombre.find(',') != string::npos ||
        correo.find(',') != string::npos || especialidad.find(',') != string::npos ||
        materia.find(',') != string::npos) {
        cout << "Error: datos vacios o con comas no permitidas." << endl;
        return;
    }

    ofstream archivo("profesores.txt", ios::app);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo de profesores." << endl;
        return;
    }

    archivo << cedula << "," << nombre << "," << correo << "," << especialidad << "," << materia << "\n";
    archivo.close();

    cout << "Profesor registrado correctamente." << endl;
}

void registrarNota() {
    string cedula, linea;
    vector<string> alumnos;
    bool encontrado = false;
    bool notaRegistrada = false;
    OperacionNota operacion;

    cout << "\n===== REGISTRO DE NOTAS =====" << endl;
    cout << "Ingrese la cedula del alumno: ";
    getline(cin, cedula);

    ifstream archivoLectura("alumnos.txt");
    if (!archivoLectura.is_open()) {
        cout << "No existe el archivo de alumnos." << endl;
        return;
    }

    while (getline(archivoLectura, linea)) {
        vector<string> datos = dividirLinea(linea);

        if (datos.size() != 7) {
            alumnos.push_back(linea);
            continue;
        }

        if (datos[0] == cedula) {
            encontrado = true;

            cout << "\nAlumno: " << datos[1] << endl;
            cout << "Programa: " << datos[3] << endl;
            cout << "Nota 1: " << datos[4] << endl;
            cout << "Nota 2: " << datos[5] << endl;
            cout << "Nota 3: " << datos[6] << endl;

            int posicion = -1;
            for (int i = 4; i <= 6; i++) {
                if (datos[i] == "0") {
                    posicion = i;
                    break;
                }
            }

            if (posicion == -1) {
                cout << "El alumno ya tiene tres notas." << endl;
                alumnos.push_back(linea);
                continue;
            }

            double nota;
            cout << "Ingrese la nueva nota (0 a 20): ";
            if (!(cin >> nota)) {
                cout << "Error: debe introducir un numero." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                alumnos.push_back(linea);
                archivoLectura.close();
                return;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (nota < 0 || nota > 20) {
                cout << "La nota debe estar entre 0 y 20." << endl;
                alumnos.push_back(linea);
                archivoLectura.close();
                return;
            }

            stringstream conversor;
            conversor << fixed << setprecision(1) << nota;
            string notaAnterior = datos[posicion];
            string notaNueva = conversor.str();

            datos[posicion] = notaNueva;

            operacion.cedula = cedula;
            operacion.posicionNota = posicion;
            operacion.notaAnterior = notaAnterior;
            operacion.notaNueva = notaNueva;

            string nuevaLinea = "";
            for (size_t i = 0; i < datos.size(); i++) {
                if (i > 0) nuevaLinea += ",";
                nuevaLinea += datos[i];
            }

            alumnos.push_back(nuevaLinea);
            notaRegistrada = true;
            cout << "Nota registrada correctamente." << endl;
        } else {
            alumnos.push_back(linea);
        }
    }
    archivoLectura.close();

    if (!encontrado) {
        cout << "No se encontro un alumno con esa cedula." << endl;
        return;
    }

    if (!notaRegistrada) return;

    ofstream archivoEscritura("alumnos.txt");
    if (!archivoEscritura.is_open()) {
        cout << "Error al guardar las notas." << endl;
        return;
    }

    for (size_t i = 0; i < alumnos.size(); ++i) {
        archivoEscritura << alumnos[i] << "\n";
    }
    archivoEscritura.close();

    pilaNotas.push(operacion);
    cout << "Nota guardada en el historial de operaciones." << endl;
}

void deshacerUltimaNota() {
    if (pilaNotas.empty()) {
        cout << "No hay notas para deshacer en esta sesion." << endl;
        return;
    }

    OperacionNota operacion = pilaNotas.top();
    ifstream archivoLectura("alumnos.txt");

    if (!archivoLectura.is_open()) {
        cout << "No se pudo abrir alumnos.txt." << endl;
        return;
    }

    vector<string> alumnos;
    string linea;
    bool encontrado = false;

    while (getline(archivoLectura, linea)) {
        vector<string> datos = dividirLinea(linea);

        if (datos.size() == 7 && datos[0] == operacion.cedula) {
            if (datos[operacion.posicionNota] != operacion.notaNueva) {
                cout << "No se puede deshacer: la nota fue modificada." << endl;
                archivoLectura.close();
                return;
            }

            datos[operacion.posicionNota] = operacion.notaAnterior;
            string nuevaLinea = "";
            for (size_t i = 0; i < datos.size(); i++) {
                if (i > 0) nuevaLinea += ",";
                nuevaLinea += datos[i];
            }

            alumnos.push_back(nuevaLinea);
            encontrado = true;
        } else {
            alumnos.push_back(linea);
        }
    }
    archivoLectura.close();

    if (!encontrado) {
        cout << "No se encontro el alumno." << endl;
        return;
    }

    ofstream archivoEscritura("alumnos.txt");
    if (!archivoEscritura.is_open()) {
        cout << "Error al guardar los cambios." << endl;
        return;
    }

    for (vector<string>::const_iterator registro = alumnos.begin(); registro != alumnos.end(); ++registro) {
        archivoEscritura << *registro << "\n";
    }
    archivoEscritura.close();

    pilaNotas.pop();
    cout << "Ultima nota deshecha correctamente." << endl;
    cout << "Se restauro el valor anterior: " << operacion.notaAnterior << endl;
}

void generarColaCertificados() {
    cout << "\n========================================" << endl;
    cout << " GENERACION DE CERTIFICADOS (FIFO) " << endl;
    cout << "========================================" << endl;

    queue<string> colaCertificados;
    ifstream archivoAlumnos("alumnos.txt");

    if (!archivoAlumnos.is_open()) {
        cout << "No se pudo abrir alumnos.txt" << endl;
        return;
    }

    string linea;
    int elegibles = 0;

    while (getline(archivoAlumnos, linea)) {
        vector<string> datos = dividirLinea(linea);
        if (datos.size() != 7) continue;

        if (datos[4] == "0" || datos[5] == "0" || datos[6] == "0") continue;

        double nota1 = stod(datos[4]);
        double nota2 = stod(datos[5]);
        double nota3 = stod(datos[6]);
        double promedio = (nota1 + nota2 + nota3) / 3.0;

        bool aprobado = false;
        if (datos[3] == "Curso" && promedio >= 10) aprobado = true;
        else if (datos[3] == "Diplomado" && promedio >= 14) aprobado = true;
        else if (datos[3] == "Bootcamp" && nota1 >= 14 && nota2 >= 14 && nota3 >= 14) aprobado = true;

        if (aprobado) {
            // Guardamos en formato temporal interno: cedula|nombre|programa|promedio
            stringstream certificado;
            certificado << datos[0] << "|" << datos[1] << "|" << datos[3] << "|" << fixed << setprecision(1) << promedio;
            colaCertificados.push(certificado.str());
            elegibles++;
        }
    }
    archivoAlumnos.close();

    // Creación del archivo con el formato exacto del proyecto.pdf
    ofstream archivoCertificados("certificados_pendientes.txt");
    if (!archivoCertificados.is_open()) {
        cout << "No se pudo crear el archivo de certificados." << endl;
        return;
    }

    archivoCertificados << "========================================\n";
    archivoCertificados << "REPORTE DE CERTIFICADOS PENDIENTES\n";
    archivoCertificados << "========================================\n";
    archivoCertificados << "Total de graduandos en cola: " << elegibles << "\n\n";

    cout << "\n--- ALUMNOS ELEGIBLES EN COLA ---" << endl;
    int contador = 1;

    while (!colaCertificados.empty()) {
        string registroCert = colaCertificados.front();
        vector<string> partes = dividirLinea(registroCert, '|');

        string cedula = partes[0];
        string nombre = partes[1];
        string programa = partes[2];
        string promedio = partes[3];

        // Escribir exactamente como indica el proyecto.pdf
        archivoCertificados << contador << ". [" << cedula << "] " << nombre << "\n";
        archivoCertificados << " - Programa: " << programa << "\n";
        archivoCertificados << " - Promedio Final: " << promedio << "\n";

        if (programa == "Bootcamp") {
            archivoCertificados << " - Estatus: APROBADO (Cumple regla de ninguna nota < 14)\n\n";
        } else {
            archivoCertificados << " - Estatus: APROBADO\n\n";
        }

        cout << contador << ". [" << cedula << "] " << nombre << " - APROBADO" << endl;

        colaCertificados.pop();
        contador++;
    }

    archivoCertificados << "========================================\n";
    archivoCertificados << "* Fin del reporte - Generado por SGA-DO *\n";
    archivoCertificados.close();

    cout << "\nTotal de graduandos en cola: " << elegibles << endl;
    cout << "Archivo generado con el formato exacto: certificados_pendientes.txt" << endl;
    cout << "========================================" << endl;
}

void reporteGeneral() {
    cout << "\n========================================" << endl;
    cout << " REPORTE GENERAL SGA-DO" << endl;
    cout << "========================================\n" << endl;

    // REPORTE DE ALUMNOS
    cout << "------------- ALUMNOS ------------------" << endl;
    ifstream archivoAlumnos("alumnos.txt");
    if (!archivoAlumnos.is_open()) {
        cout << "No se pudo abrir alumnos.txt" << endl;
    } else {
        string linea;
        int totalAlumnos = 0;

        while (getline(archivoAlumnos, linea)) {
            vector<string> datos = dividirLinea(linea);
            if (datos.size() != 7) continue;

            totalAlumnos++;
            cout << "\nCedula: " << datos[0] << endl;
            cout << "Nombre: " << datos[1] << endl;
            cout << "Correo: " << datos[2] << endl;
            cout << "Programa: " << datos[3] << endl;

            double notas[3] = {0, 0, 0};
            int cantidadNotas = 0;
            double suma = 0;

            for (int i = 0; i < 3; i++) {
                stringstream flujoNota(datos[i + 4]);
                flujoNota >> notas[i];
                cout << "Nota " << i + 1 << ": ";
                if (datos[i + 4] == "0") {
                    cout << "Pendiente" << endl;
                } else {
                    cout << fixed << setprecision(1) << notas[i] << endl;
                    suma += notas[i];
                    cantidadNotas++;
                }
            }

            if (cantidadNotas < 3) {
                cout << "Promedio: Pendiente de completar notas" << endl;
                cout << "Estado: NOTAS PENDIENTES" << endl;
            } else {
                double promedio = suma / 3;
                cout << "Promedio: " << fixed << setprecision(1) << promedio << endl;

                bool aprobado = false;
                if (datos[3] == "Curso" && promedio >= 10) aprobado = true;
                else if (datos[3] == "Diplomado" && promedio >= 14) aprobado = true;
                else if (datos[3] == "Bootcamp" && notas[0] >= 14 && notas[1] >= 14 && notas[2] >= 14) aprobado = true;

                cout << "Estado: " << (aprobado ? "APROBADO" : "NO APROBADO") << endl;
            }
            cout << "----------------------------------------" << endl;
        }
        cout << "\nTotal de alumnos registrados: " << totalAlumnos << endl;
        archivoAlumnos.close();
    }

    // REPORTE DE PROFESORES
    cout << "\n------------- PROFESORES ---------------" << endl;
    ifstream archivoProfesores("profesores.txt");
    if (!archivoProfesores.is_open()) {
        cout << "No se pudo abrir profesores.txt" << endl;
    } else {
        string linea;
        int totalProfesores = 0;

        while (getline(archivoProfesores, linea)) {
            vector<string> datos = dividirLinea(linea);
            if (datos.size() != 5) continue;

            totalProfesores++;
            cout << "\nCedula: " << datos[0] << endl;
            cout << "Nombre: " << datos[1] << endl;
            cout << "Correo: " << datos[2] << endl;
            cout << "Especialidad: " << datos[3] << endl;
            cout << "Materia: " << datos[4] << endl;
            cout << "----------------------------------------" << endl;
        }
        cout << "\nTotal de profesores registrados: " << totalProfesores << endl;
        archivoProfesores.close();
    }
    cout << "\n========================================" << endl;
}

int main() {
    int opcion;
    do {
        cout << "\n========================================" << endl;
        cout << " SGA-DO: SISTEMA DE GESTION ACADEMICA" << endl;
        cout << "========================================" << endl;
        cout << "1. Registrar Alumno" << endl;
        cout << "2. Registrar Profesor" << endl;
        cout << "3. Registrar Notas" << endl;
        cout << "4. Deshacer Ultimo Registro de Nota" << endl;
        cout << "5. Cola de Certificados" << endl;
        cout << "6. Reporte General" << endl;
        cout << "7. Salir" << endl;
        cout << "\nSeleccione una opcion: ";

        if (!(cin >> opcion)) {
            cout << "Debe ingresar un numero." << endl;
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
            case 7: cout << "Gracias por utilizar el sistema SGA-DO." << endl; break;
            default: cout << "Opcion no valida. Intente nuevamente." << endl;
        }
    } while (opcion != 7);

    return 0;
}

