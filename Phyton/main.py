#SISTEMA DE GESTIÓN ACADÉMICA SGA DO

#CLASE PERSONA
class Persona:
     def __init__(self, cedula, nombre, correo):
         self.cedula = cedula
         self.nombre = nombre
         self.correo = correo

#CLASE ALUMNO 
#ESTA HEREDA DE PERSONA 
class Alumno(Persona):
        def __init__(self, cedula, nombre, correo, programa, nota1, nota2, nota3):
            super().__init__(cedula, nombre, correo)
            self.programa = programa
            self.nota1 = nota1
            self.nota2 = nota2
            self.nota3 = nota3
#CLASE PROFESOR
#ESTA HEREDA DE PERSONA
class Profesor(Persona):
        def __init__(self, cedula, nombre, correo, especialidad,materia):
            super().__init__(cedula, nombre, correo)
            self.especialidad = especialidad
#CLASE PROGRAMA ACADEMICO
class ProgramaAcademico:
    def evaluaraprobacion(self,notas):
        return False
    
#CLASE CURSO
#HEREDA DE PROGRAMA ACADEMICO
class Curso(ProgramaAcademico): 
     def evaluaraprobacion(self,notas):
          promedio = sum(notas) / len(notas)
          if promedio >= 10.0:
              return True

#CLASE DIPLOMADO
#HEREDA DE PROGRAMA ACADEMICO
class Diplomado(ProgramaAcademico):
    def evaluaraprobacion(self, notas):
        promedio = sum(notas) / len(notas)
        if promedio >= 14.0:
            return True
             
#CLASE BOOTCAMP
#HEREDA DE PROGRAMA ACADEMICO
class Bootcamp(ProgramaAcademico):
    def evaluaraprobacion(self, notas):
        return all(nota>=14 for nota in notas)
#FUNCION PARA CREAR EL PROGRAMA
def crear_programa(tipo):
    tipo = tipo.lower()

    if tipo == "curso":
        return Curso()

    elif tipo == "diplomado":
        return Diplomado()

    elif tipo == "bootcamp":
        return Bootcamp()

    return None
#PILA PARA GUARDAR LAS ULTIMAS NOTAS
pila_notas = []
def registrar_alumno():
    print("\nRegistro de Alumno")

    cedula = input("Ingrese la cédula: ").strip()

    # 1. Verificar que la cédula no esté registrada
    try:
        with open("alumnos.txt", "r", encoding="utf-8") as archivo:
            for linea in archivo:
                datos = linea.strip().split(",")

                if datos[0] == cedula:
                    print("Error: ya existe un alumno con esa cédula.")
                    return

    except FileNotFoundError:
        # Si el archivo todavía no existe, podemos registrar el primer alumno.
        pass

    nombre = input("Ingrese el nombre: ").strip()
    correo = input("Ingrese el correo: ").strip()

    # 2. Validar que el programa sea uno de los permitidos
    while True:
        programa = input(
            "Ingrese el programa (Curso/Diplomado/Bootcamp): "
        ).strip().lower()

        if programa in ("curso", "diplomado", "bootcamp"):
            programa = programa.capitalize()
            break

        print("Error: programa inválido.")
        print("Opciones válidas: Curso, Diplomado o Bootcamp.")

    # Las notas comienzan en cero porque aún no se han registrado
    nota1 = 0
    nota2 = 0
    nota3 = 0

    with open("alumnos.txt", "a", encoding="utf-8") as archivo:
        archivo.write(
            f"{cedula},{nombre},{correo},{programa},"
            f"{nota1},{nota2},{nota3}\n"
        )

    print("Alumno registrado con éxito.")
def registrar_profesor():
        
        print("/n Registro de Profesor")
        cedula = input("Ingrese la cedula:")
        nombre = input("Ingrese el nombre:")
        correo = input("Ingrese el correo:")
        especialidad = input("Ingrese la especialidad:")
        materia = input("Ingrese la materia:")

        with open("profesores.txt","a", encoding="utf-8") as archivo:
            archivo.write(f"{cedula},{nombre},{correo},{especialidad},{materia}\n")

        print("Profesor registrado con exito")


        encontrado = True
def registrar_nota():

    print("\nRegistro de Notas")

    cedula_buscada = input(
        "Ingrese la cédula del alumno: "
    ).strip()

    try:
        with open("alumnos.txt", "r", encoding="utf-8") as archivo:
            lineas = archivo.readlines()
    except FileNotFoundError:
        print("Error: todavía no existe el archivo de alumnos.")
        return

    encontrado = False
    posicion_nota = None
    lineas_nuevas = []
    nota = None

    for linea in lineas:
        datos = linea.strip().split(",")

        if len(datos) != 7:
            lineas_nuevas.append(linea)
            continue

        if datos[0] == cedula_buscada:
            encontrado = True

            print("\nAlumno encontrado.")
            print("Nombre:", datos[1])
            print("Programa:", datos[3])
            print("Notas actuales:", datos[4], datos[5], datos[6])

            if "0" not in datos[4:7]:
                print("Error: el alumno ya tiene las 3 notas registradas.")
                lineas_nuevas.append(linea)
                continue

            # Validar la nota antes de modificar el registro
            while True:
                entrada = input(
                    "Ingrese la nueva nota (de 0 a 20): "
                ).strip()

                try:
                    nota = float(entrada)

                    if 0 <= nota <= 20:
                        break

                    print("Error: la nota debe estar entre 0 y 20.")

                except ValueError:
                    print("Error: debe ingresar un número válido.")

            # Buscar el primer espacio de nota disponible
            for posicion in (4, 5, 6):
                if float(datos[posicion]) == 0:
                    datos[posicion] = str(nota)
                    posicion_nota = posicion
                    break

            linea = ",".join(datos) + "\n"

        lineas_nuevas.append(linea)

    if not encontrado:
        print("\nAlumno no encontrado.")
        return

    with open("alumnos.txt", "w", encoding="utf-8") as archivo:
        archivo.writelines(lineas_nuevas)

    if posicion_nota is not None:
        pila_notas.append(
            (cedula_buscada, posicion_nota, str(nota))
        )
        print("\nNota registrada correctamente.")
def deshacer_nota():
     # Verificamos si la pila está vacía
         if not pila_notas:
             print("\nNo hay notas para deshacer.")
             return
     
         # Sacamos la última acción de la pila
         ultima_accion = pila_notas.pop()
     
         # Recuperamos los datos guardados
         cedula = ultima_accion[0]
         posicion = ultima_accion[1]
     
         lineas_nuevas = []
     
         # Abrimos el archivo para leerlo
         with open("alumnos.txt", "r", encoding="utf-8") as archivo:
             lineas = archivo.readlines()
     
             for linea in lineas:
     
                 datos = linea.strip().split(",")
     
                 # Buscamos al alumno correspondiente
                 if datos[0] == cedula:
     
                     # Eliminamos la última nota registrada
                     datos[posicion] = "0"
     
                 # Reconstruimos la línea
                 linea_actualizada = ",".join(datos) + "\n"
                 lineas_nuevas.append(linea_actualizada)
     
         # Guardamos nuevamente el archivo
         with open("alumnos.txt", "w", encoding="utf-8") as archivo:
             archivo.writelines(lineas_nuevas)
     
         print("\nÚltimo registro de nota deshecho correctamente.")
def generar_cola_certificados():


    cola_certificados = []

    # Abrimos el archivo de alumnos
    with open("alumnos.txt", "r", encoding="utf-8") as archivo:

        lineas = archivo.readlines()

    # Revisamos todos los alumnos
    for linea in lineas:

        datos = linea.strip().split(",")

        cedula = datos[0]
        nombre = datos[1]
        correo = datos[2]
        tipo_programa = datos[3]

        # Convertimos las notas a números
        notas = [
            float(datos[4]),
            float(datos[5]),
            float(datos[6])
        ]

        # Verificamos que tenga las 3 notas
        if 0 in notas:
            continue

        # Creamos el objeto correspondiente
        programa = crear_programa(tipo_programa)

        # Si el programa existe, evaluamos
        if programa is not None:

            aprobado = programa.evaluaraprobacion(notas)

            if aprobado:

                cola_certificados.append(
                    (
                        cedula,
                        nombre,
                        correo,
                        tipo_programa
                    )
                )

    # Crear / sobrescribir archivo de certificados
    with open(
        "certificados_pendientes.txt",
        "w",
        encoding="utf-8"
    ) as archivo:

        for certificado in cola_certificados:

            archivo.write(
                f"{certificado[0]},"
                f"{certificado[1]},"
                f"{certificado[2]},"
                f"{certificado[3]}\n"
            )

    print("\nCola de certificados generada correctamente.")

    print(
        f"Total de alumnos aprobados: "
        f"{len(cola_certificados)}"
    )
def mostrar_reporte_general():

    print("\n REPORTE GENERAL")

    # REPORTE DE ALUMNOS
    print("\n ALUMNOS REGISTRADOS ")

    try:
        with open("alumnos.txt", "r", encoding="utf-8") as archivo:
            lineas = archivo.readlines()

        for linea in lineas:

            datos = linea.strip().split(",")

            cedula = datos[0]
            nombre = datos[1]
            correo = datos[2]
            programa = datos[3]

            nota1 = float(datos[4])
            nota2 = float(datos[5])
            nota3 = float(datos[6])

            print("\nCédula:", cedula)
            print("Nombre:", nombre)
            print("Correo:", correo)
            print("Programa:", programa)
            print("Notas:", nota1, nota2, nota3)

            # Verificamos si tiene las 3 notas
            if 0 in [nota1, nota2, nota3]:
                print("Estado: Pendiente de notas")

            else:
                promedio = (nota1 + nota2 + nota3) / 3

                print("Promedio:", round(promedio, 2))

                if programa.lower() == "curso":
                    if promedio >= 10:
                        print("Estado: Aprobado")
                    else:
                        print("Estado: Reprobado")

                elif programa.lower() == "diplomado":
                    if promedio >= 14:
                        print("Estado: Aprobado")
                    else:
                        print("Estado: Reprobado")

                elif programa.lower() == "bootcamp":
                    if nota1 >= 14 and nota2 >= 14 and nota3 >= 14:
                        print("Estado: Aprobado")
                    else:
                        print("Estado: Reprobado")

    except FileNotFoundError:
        print("No existe el archivo alumnos.txt")

    # REPORTE DE PROFESORES
    print("\n----- PROFESORES REGISTRADOS -----")

    try:
        with open("profesores.txt", "r", encoding="utf-8") as archivo:
            lineas = archivo.readlines()

        for linea in lineas:

            datos = linea.strip().split(",")

            print("\nCédula:", datos[0])
            print("Nombre:", datos[1])
            print("Correo:", datos[2])
            print("Especialidad:", datos[3])

    except FileNotFoundError:
        print("No existe el archivo profesores.txt")

    print("\n====================================")
while True:
    print("SGA Diplomados Online")
    print("1. Registrar Alumno")
    print("2. Registrar Profesor")
    print("3. Registrar Notas")
    print("4. Deshacer ultimo registro de notas")
    print("5. Generar cola de certificados")
    print("6. Mostrar reporte general")
    print("7. Salir")

    opcion = input("Seleccione una opcion del 1 al 7: ")


    if opcion == "1":
            registrar_alumno()


    if opcion == "2":
        registrar_profesor()

    if opcion == "3":
              registrar_nota()

    if opcion == "4":
             deshacer_nota()

    if opcion == "5":
           generar_cola_certificados()

    if opcion == "6":
        mostrar_reporte_general()
   
    if opcion == "7":
        print("Cerrando SGA Diplomados Online")
        break 

