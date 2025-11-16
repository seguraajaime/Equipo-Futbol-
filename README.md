# Equipo-Futbol-
Proyecto: Gestor de Equipo de Fútbol
Descripción Desarrolla una aplicación en C++ para gestionar la plantilla de un equipo de fútbol. El sistema debe permitir registrar jugadores, gestionar sus contratos, y llevar un registro de los partidos (programados y jugados).

El proyecto está modularizado, utiliza clases y punteros inteligentes (std::unique_ptr) para la gestión de la memoria. Gestiona fechas tanto con estructuras personalizadas (fecha) como con std::time_t, y permite guardar y cargar todos los datos en archivos de texto para persistencia.


Módulos y Cabeceras
  jugador.h / jugador.cpp Clase para representar a los jugadores del equipo (dorsal, nombre, posición, disponibilidad y fechas de contrato).

  contrato.h / contrato.cpp Clase para representar un contrato (asociado a un dorsal, equipo de procedencia, fechas de inicio/fin y cláusula).

  partido.h / partido.cpp Clase para representar un partido (ID, local, visitante, fecha, resultado y estado).

  menuPrincipal.cpp Contiene el menú de interacción principal y la lógica de gestión. Almacena los datos en memoria (std::vector de std::unique_ptr) y coordina las           
  operaciones de carga y guardado de archivos.
  

Ejemplo de Funcionalidades
  Gestión de Jugadores: Fichar (alta) y rescindir contrato (baja) de jugadores.

  Gestión de Contratos: Crear nuevos contratos para jugadores existentes y listarlos.

  Gestión de Partidos: Crear nuevos partidos, convocarlos (asignar fecha) y registrar sus resultados.

  Listados: Mostrar la plantilla actual, los contratos vigentes y el calendario de partidos.

  Persistencia de Datos: Guardar y cargar automáticamente los datos en archivos de texto (jugadores.txt, contratos.txt, partidos.txt).

Ejemplo de Atributos y Métodos
  Jugador
  Atributos: dorsal, nombre, posicion, disponible, fechaInicioContrato (struct fecha), fechaFinContrato (struct fecha).
  Métodos: firmarContrato(), finalizarContrato(), serializar(), deserializar(), getDorsal(), getNombre(), estaDisponible().

  Contrato
  Atributos: dorsal, equipoNombre, fechaInicio (time_t), fechaFin (time_t), clausula.
  Métodos: serializar(), deserializar(), guardarEnArchivo(), cargarDesdeArchivo(), getDorsal(), getFechaInicioStr(), getFechaFinStr().

  Partido
  Atributos: idPartido (static nextId), equipoLocal, equipoVisitante, fechaPartido (time_t), golesLocal, golesVisitante, estado (enum EstadoPartido).
  Métodos: convocarPartido(), registrarResultado(), obtenerInfo(), serializar(), deserializar(), setNextId().

Persistencia y Gestión de Memoria
  Guardar y cargar datos en tres archivos de texto separados: jugadores.txt, contratos.txt y partidos.txt.
  Gestión de fechas mixta: Se utiliza una struct fecha (dia, mes, ano) para la clase Jugador y std::time_t para las clases Contrato y Partido.
  Toda la gestión de memoria de los objetos principales se maneja a través de std::vector y std::unique_ptr (ej. g_plantilla, g_contratos, g_partidos).
