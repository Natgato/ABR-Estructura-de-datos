/*
 * FACULTAD DE INGENIERÍA
 * TEMA: ABR ÁRBOL GENEALÓGICO
 * ASIGNATURA: ESTRUCTURA DE DATOS
 * DOCENTE: Yesenia Concha Ramos
 * ESTUDIANTES: José Enrique Mejía osis; ING de sistemas
 *              Brayan Aguilar Tinta; ING de sistemas
 * Cusco - Perú, 2024
 */

#include <iostream>
#include <string>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

// ==================== FUNCIONES AUXILIARES ====================

/**
 * Convierte un entero a string (compatible con C++98)
 */
string intToString(int valor) {
    stringstream ss;
    ss << valor;
    return ss.str();
}

// ==================== ESTRUCTURAS DE DATOS ====================

struct Nodo {
    // Identificación
    int id;
    string familia;
    int generacion;
    
    // Atributos base (numéricos)
    int edad;
    int salud;        // 0-100
    int energia;      // 0-100
    int fuerza;       // 1-10
    int inteligencia; // 1-10
    int carisma;      // 1-10
    
    // Rasgos derivados
    string profesion;      // Campesino, Artesano, Guerrero, Sabio, Curandero, Fundador
    string temperamento;   // Calmado, Agresivo, Curioso, Perezoso, Sabio, Nervioso
    string rareza;         // Comun, Raro, Epico, Legendario
    
    // Estado dinámico
    string estadoEmocional; // Feliz, Triste, Enojado, Neutro, Motivado, Inspirado, Agotado, Ansioso
    string intencion;       // Descansar, Entrenar, Meditar, Socializar, Trabajar, Explorar, Reproducirse
    
    // Habilidades especiales (para rarezas altas)
    vector<string> habilidades;
    
    // Punteros del ABB
    Nodo* izq;
    Nodo* der;
    
    // Referencia al padre (opcional, útil para navegación)
    Nodo* padre;
};

// ==================== VARIABLES GLOBALES ====================

Nodo* raiz = NULL;
int contadorID = 1000;  // IDs comenzarán desde 1000
int cicloActual = 0;
vector<Nodo*> listaCiudadanos; // Para balanceo

// ==================== PROTOTIPOS DE FUNCIONES ====================

// Operaciones básicas del ABB
Nodo* crearNodo(int id, string familia, int generacion);
void insertarNodo(Nodo*& raiz, Nodo* nuevo);
Nodo* buscarNodo(Nodo* raiz, int id);
Nodo* encontrarMinimo(Nodo* nodo);
void eliminarNodo(Nodo*& raiz, int id);

// Recorridos del árbol
void recorridoInorden(Nodo* raiz);
void recorridoPreorden(Nodo* raiz);
void recorridoPostorden(Nodo* raiz);
void recorridoPorNiveles(Nodo* raiz);

// Visualización
void mostrarCiudadanoCompacto(Nodo* nodo);
void mostrarCiudadanoCompleto(Nodo* nodo);
void mostrarArbolCompleto(Nodo* raiz);

// Inicialización y generación
void inicializarAtributosBase(Nodo* nodo);
void asignarRasgosDerivados(Nodo* nodo);
void crearFundadores(int cantidad);

// Simulación
void ejecutarCicloSimulacion();
void envejecerCiudadanos(Nodo* raiz);
void actualizarEstados(Nodo* raiz);
void procesarReproduccion();
bool puedeReproducirse(Nodo* ciudadano);
Nodo* generarHijo(Nodo* padre, string familiaBase);

// Eventos aleatorios
void aplicarEventosPersonal(Nodo* ciudadano);
void aplicarEventosAmbiental();
void procesarEvento(Nodo* ciudadano, int eventoID);

// Utilidades
int calcularAltura(Nodo* raiz);
int contarNodos(Nodo* raiz);
void balancearArbol();
void obtenerListaInorden(Nodo* raiz, vector<Nodo*>& lista);
Nodo* construirABBEquilibrado(vector<Nodo*>& lista, int inicio, int fin);

// Reportes
void generarReporteGlobal();
void mostrarDistribucionGeneraciones(Nodo* raiz);

// Menú
void mostrarMenu();
void mostrarMenuSimulacion();

// ==================== IMPLEMENTACIÓN DE FUNCIONES ====================

/**
 * Crea un nuevo nodo (ciudadano) con valores iniciales
 */
Nodo* crearNodo(int id, string familia, int generacion) {
    if (id <= 0) return NULL;
    if (familia.empty()) return NULL;
    if (generacion < 0) return NULL;
    
    Nodo* nuevo = new Nodo;
    if (nuevo == NULL) return NULL;
    
    nuevo->id = id;
    nuevo->familia = familia;
    nuevo->generacion = generacion;
    nuevo->edad = 0;
    nuevo->izq = NULL;
    nuevo->der = NULL;
    nuevo->padre = NULL;
    nuevo->habilidades.clear();
    
    // Inicializar atributos base
    inicializarAtributosBase(nuevo);
    
    // Asignar rasgos derivados
    asignarRasgosDerivados(nuevo);
    
    return nuevo;
}

/**
 * Inicializa los atributos base del ciudadano con valores aleatorios o por defecto
 */
void inicializarAtributosBase(Nodo* nodo) {
    if (nodo == NULL) return;
    
    nodo->salud = 80 + rand() % 21;      // 80-100
    nodo->energia = 70 + rand() % 31;    // 70-100
    nodo->fuerza = 1 + rand() % 10;      // 1-10
    nodo->inteligencia = 1 + rand() % 10; // 1-10
    nodo->carisma = 1 + rand() % 10;     // 1-10
    
    // Estado inicial
    nodo->estadoEmocional = "Neutro";
    nodo->intencion = "Trabajar";
}

/**
 * Asigna los rasgos derivados basados en los atributos base
 */
void asignarRasgosDerivados(Nodo* nodo) {
    if (nodo == NULL) return;
    
    // Determinar profesión
    if (nodo->generacion == 0) {
        nodo->profesion = "Fundador";
    } else if (nodo->fuerza >= 7) {
        nodo->profesion = "Guerrero";
    } else if (nodo->inteligencia >= 7) {
        nodo->profesion = (rand() % 2 == 0) ? "Sabio" : "Artesano";
    } else if (nodo->carisma >= 7) {
        nodo->profesion = "Artesano";
    } else {
        nodo->profesion = "Campesino";
    }
    
    // Determinar temperamento
    int temp = rand() % 6;
    string temperamentos[] = {"Calmado", "Agresivo", "Curioso", "Perezoso", "Sabio", "Nervioso"};
    nodo->temperamento = temperamentos[temp];
    
    // Determinar rareza (80% común, 15% raro, 4% épico, 1% legendario)
    int probRareza = rand() % 100;
    if (probRareza < 80) {
        nodo->rareza = "Comun";
    } else if (probRareza < 95) {
        nodo->rareza = "Raro";
    } else if (probRareza < 99) {
        nodo->rareza = "Epico";
    } else {
        nodo->rareza = "Legendario";
    }
    
    // Asignar habilidades según rareza
    if (nodo->rareza == "Raro") {
        nodo->habilidades.push_back("Resistencia");
    } else if (nodo->rareza == "Epico") {
        nodo->habilidades.push_back("Maestria");
    } else if (nodo->rareza == "Legendario") {
        nodo->habilidades.push_back("Inmortalidad");
        nodo->habilidades.push_back("Sabiduria Eterna");
    }
}

/**
 * Inserta un nodo en el ABB manteniendo la propiedad de búsqueda
 */
void insertarNodo(Nodo*& raiz, Nodo* nuevo) {
    if (nuevo == NULL) return;
    
    if (raiz == NULL) {
        raiz = nuevo;
        return;
    }
    
    if (nuevo->id < raiz->id) {
        if (raiz->izq == NULL) {
            raiz->izq = nuevo;
            nuevo->padre = raiz;
        } else {
            insertarNodo(raiz->izq, nuevo);
        }
    } else if (nuevo->id > raiz->id) {
        if (raiz->der == NULL) {
            raiz->der = nuevo;
            nuevo->padre = raiz;
        } else {
            insertarNodo(raiz->der, nuevo);
        }
    } else {
        // ID duplicado - por ahora lo ignoramos
        cout << "ADVERTENCIA: ID duplicado " << nuevo->id << endl;
    }
}

/**
 * Busca un nodo por su ID en el ABB
 */
Nodo* buscarNodo(Nodo* raiz, int id) {
    if (raiz == NULL || raiz->id == id) {
        return raiz;
    }
    
    if (id < raiz->id) {
        return buscarNodo(raiz->izq, id);
    } else {
        return buscarNodo(raiz->der, id);
    }
}

/**
 * Encuentra el nodo con el valor mínimo en un subárbol
 */
Nodo* encontrarMinimo(Nodo* nodo) {
    if (nodo == NULL) return NULL;
    while (nodo->izq != NULL) {
        nodo = nodo->izq;
    }
    return nodo;
}

/**
 * Elimina un nodo del ABB sin romper su estructura
 */
void eliminarNodo(Nodo*& raiz, int id) {
    if (raiz == NULL) {
        return;
    }
    
    if (id < raiz->id) {
        eliminarNodo(raiz->izq, id);
    } else if (id > raiz->id) {
        eliminarNodo(raiz->der, id);
    } else {
        // Nodo encontrado
        // Caso 1: Nodo hoja
        if (raiz->izq == NULL && raiz->der == NULL) {
            delete raiz;
            raiz = NULL;
        }
        // Caso 2: Nodo con un solo hijo
        else if (raiz->izq == NULL) {
            Nodo* temp = raiz;
            raiz = raiz->der;
            if (raiz != NULL) {
                raiz->padre = temp->padre;
            }
            delete temp;
        } else if (raiz->der == NULL) {
            Nodo* temp = raiz;
            raiz = raiz->izq;
            if (raiz != NULL) {
                raiz->padre = temp->padre;
            }
            delete temp;
        }
        // Caso 3: Nodo con dos hijos
        else {
            Nodo* sucesor = encontrarMinimo(raiz->der);
            // Copiar datos del sucesor al nodo actual
            raiz->id = sucesor->id;
            raiz->familia = sucesor->familia;
            raiz->generacion = sucesor->generacion;
            raiz->edad = sucesor->edad;
            raiz->salud = sucesor->salud;
            raiz->energia = sucesor->energia;
            raiz->fuerza = sucesor->fuerza;
            raiz->inteligencia = sucesor->inteligencia;
            raiz->carisma = sucesor->carisma;
            raiz->profesion = sucesor->profesion;
            raiz->temperamento = sucesor->temperamento;
            raiz->rareza = sucesor->rareza;
            raiz->estadoEmocional = sucesor->estadoEmocional;
            raiz->intencion = sucesor->intencion;
            raiz->habilidades = sucesor->habilidades;
            
            // Eliminar el sucesor
            eliminarNodo(raiz->der, sucesor->id);
        }
    }
}

/**
 * Recorrido inorden: muestra los ciudadanos ordenados por ID
 */
void recorridoInorden(Nodo* raiz) {
    if (raiz == NULL) return;
    
    recorridoInorden(raiz->izq);
    mostrarCiudadanoCompacto(raiz);
    recorridoInorden(raiz->der);
}

/**
 * Recorrido preorden: muestra desde los ancestros hacia abajo
 */
void recorridoPreorden(Nodo* raiz) {
    if (raiz == NULL) return;
    
    mostrarCiudadanoCompacto(raiz);
    recorridoPreorden(raiz->izq);
    recorridoPreorden(raiz->der);
}

/**
 * Recorrido postorden: muestra primero descendientes, luego ancestros
 */
void recorridoPostorden(Nodo* raiz) {
    if (raiz == NULL) return;
    
    recorridoPostorden(raiz->izq);
    recorridoPostorden(raiz->der);
    mostrarCiudadanoCompacto(raiz);
}

/**
 * Recorrido por niveles: agrupa por generaciones
 */
void recorridoPorNiveles(Nodo* raiz) {
    if (raiz == NULL) return;
    
    queue<Nodo*> cola;
    cola.push(raiz);
    
    while (!cola.empty()) {
        Nodo* actual = cola.front();
        cola.pop();
        
        mostrarCiudadanoCompacto(actual);
        
        if (actual->izq != NULL) {
            cola.push(actual->izq);
        }
        if (actual->der != NULL) {
            cola.push(actual->der);
        }
    }
}

/**
 * Muestra una representación compacta del ciudadano
 */
void mostrarCiudadanoCompacto(Nodo* nodo) {
    if (nodo == NULL) return;
    
    cout << "ID: " << setw(5) << nodo->id 
         << " | " << setw(8) << (nodo->familia.empty() ? "N/A" : nodo->familia)
         << " | G" << nodo->generacion
         << " | PROF: " << setw(10) << (nodo->profesion.empty() ? "N/A" : nodo->profesion)
         << " | RAREZA: " << setw(10) << (nodo->rareza.empty() ? "N/A" : nodo->rareza)
         << " | ESTADO: " << setw(10) << (nodo->estadoEmocional.empty() ? "N/A" : nodo->estadoEmocional)
         << " / INT: " << setw(12) << (nodo->intencion.empty() ? "N/A" : nodo->intencion);
    
    // Mostrar hijos si existen
    cout << " | HIJOS: ";
    if (nodo->izq != NULL) {
        cout << nodo->izq->id;
    } else {
        cout << "-";
    }
    cout << ", ";
    if (nodo->der != NULL) {
        cout << nodo->der->id;
    } else {
        cout << "-";
    }
    cout << endl;
}

/**
 * Muestra la información completa de un ciudadano
 */
void mostrarCiudadanoCompleto(Nodo* nodo) {
    if (nodo == NULL) {
        cout << "Ciudadano no encontrado." << endl;
        return;
    }
    
    cout << "\n========== FICHA COMPLETA DEL CIUDADANO ==========" << endl;
    cout << "ID: " << nodo->id << endl;
    cout << "Familia: " << nodo->familia << endl;
    cout << "Generacion: " << nodo->generacion << endl;
    cout << "\n--- Atributos Base ---" << endl;
    cout << "Edad: " << nodo->edad << " años" << endl;
    cout << "Salud: " << nodo->salud << "/100" << endl;
    cout << "Energia: " << nodo->energia << "/100" << endl;
    cout << "Fuerza: " << nodo->fuerza << "/10" << endl;
    cout << "Inteligencia: " << nodo->inteligencia << "/10" << endl;
    cout << "Carisma: " << nodo->carisma << "/10" << endl;
    cout << "\n--- Rasgos Derivados ---" << endl;
    cout << "Profesion: " << nodo->profesion << endl;
    cout << "Temperamento: " << nodo->temperamento << endl;
    cout << "Rareza: " << nodo->rareza << endl;
    cout << "\n--- Estado Actual ---" << endl;
    cout << "Estado Emocional: " << nodo->estadoEmocional << endl;
    cout << "Intencion: " << nodo->intencion << endl;
    
    if (!nodo->habilidades.empty()) {
        cout << "\n--- Habilidades Especiales ---" << endl;
        for (size_t i = 0; i < nodo->habilidades.size(); i++) {
            cout << "- " << nodo->habilidades[i] << endl;
        }
    }
    
    // Mostrar vínculos genealógicos
    cout << "\n--- Vinculos Genealogicos ---" << endl;
    if (nodo->padre != NULL) {
        cout << "Padre: ID " << nodo->padre->id << " (" << nodo->padre->familia << ")" << endl;
    } else {
        cout << "Padre: Fundador" << endl;
    }
    cout << "Hijo Izquierdo: ";
    if (nodo->izq != NULL) {
        cout << "ID " << nodo->izq->id << " (" << nodo->izq->familia << ")" << endl;
    } else {
        cout << "Ninguno" << endl;
    }
    cout << "Hijo Derecho: ";
    if (nodo->der != NULL) {
        cout << "ID " << nodo->der->id << " (" << nodo->der->familia << ")" << endl;
    } else {
        cout << "Ninguno" << endl;
    }
    cout << "==============================================\n" << endl;
}

/**
 * Crea ciudadanos fundadores (generación 0)
 */
void crearFundadores(int cantidad) {
    if (cantidad <= 0) {
        cout << "Error: La cantidad debe ser mayor que 0." << endl;
        return;
    }
    if (cantidad > 100) {
        cout << "Advertencia: Cantidad muy grande (" << cantidad << "). Limitando a 100." << endl;
        cantidad = 100;
    }
    
    for (int i = 0; i < cantidad; i++) {
        int id = contadorID++;
        string familia = "F" + intToString(i + 1);
        Nodo* fundador = crearNodo(id, familia, 0);
        if (fundador != NULL) {
            fundador->profesion = "Fundador";
            insertarNodo(raiz, fundador);
        }
    }
    cout << cantidad << " fundadores creados exitosamente." << endl;
}

/**
 * Verifica si un ciudadano puede reproducirse
 */
bool puedeReproducirse(Nodo* ciudadano) {
    if (ciudadano == NULL) return false;
    
    // Condiciones básicas
    if (ciudadano->edad < 18 || ciudadano->edad > 50) return false;
    if (ciudadano->salud < 40) return false;
    if (ciudadano->energia < 50) return false;
    
    // Verificar si ya tiene dos hijos
    if (ciudadano->izq != NULL && ciudadano->der != NULL) return false;
    
    // Probabilidad según atributos y estado
    int probabilidad = 30; // Base 30%
    
    if (ciudadano->estadoEmocional == "Motivado") probabilidad += 20;
    if (ciudadano->estadoEmocional == "Feliz") probabilidad += 10;
    if (ciudadano->salud > 80) probabilidad += 10;
    if (ciudadano->energia > 80) probabilidad += 10;
    
    int roll = rand() % 100;
    return roll < probabilidad;
}

/**
 * Genera un hijo para un ciudadano
 */
Nodo* generarHijo(Nodo* padre, string familiaBase) {
    if (padre == NULL) return NULL;
    
    int id = contadorID++;
    int nuevaGeneracion = padre->generacion + 1;
    string familiaHijo = familiaBase + "-" + intToString(nuevaGeneracion);
    
    Nodo* hijo = crearNodo(id, familiaHijo, nuevaGeneracion);
    if (hijo == NULL) return NULL;
    
    // Herencia: el hijo hereda parcialmente los atributos del padre
    hijo->fuerza = max(1, min(10, padre->fuerza + (rand() % 5 - 2))); // -2 a +2
    hijo->inteligencia = max(1, min(10, padre->inteligencia + (rand() % 5 - 2)));
    hijo->carisma = max(1, min(10, padre->carisma + (rand() % 5 - 2)));
    hijo->salud = max(60, min(100, padre->salud + (rand() % 11 - 5)));
    
    // Probabilidad de heredar rareza
    if (padre->rareza != "Comun" && rand() % 100 < 30) {
        hijo->rareza = padre->rareza;
    }
    
    // Probabilidad de mutación
    if (rand() % 100 < 5) {
        int statMutado = rand() % 3;
        int cambio = 1 + rand() % 2;
        if (statMutado == 0) hijo->fuerza = min(10, hijo->fuerza + cambio);
        else if (statMutado == 1) hijo->inteligencia = min(10, hijo->inteligencia + cambio);
        else hijo->carisma = min(10, hijo->carisma + cambio);
    }
    
    // Reasignar profesión según nuevos atributos
    asignarRasgosDerivados(hijo);
    
    hijo->padre = padre;
    
    // Insertar como hijo izquierdo o derecho
    if (padre->izq == NULL) {
        insertarNodo(raiz, hijo);
        padre->izq = hijo;
    } else if (padre->der == NULL) {
        insertarNodo(raiz, hijo);
        padre->der = hijo;
    }
    
    return hijo;
}

/**
 * Procesa la reproducción de todos los ciudadanos
 */
void procesarReproduccion() {
    if (raiz == NULL) return;
    
    vector<Nodo*> candidatos;
    
    // Obtener todos los ciudadanos
    obtenerListaInorden(raiz, candidatos);
    
    for (size_t i = 0; i < candidatos.size(); i++) {
        Nodo* ciudadano = candidatos[i];
        if (ciudadano == NULL) continue;
        
        if (puedeReproducirse(ciudadano) && 
            (ciudadano->izq == NULL || ciudadano->der == NULL)) {
            
            // Determinar la familia base
            string familiaBase = ciudadano->familia;
            if (familiaBase.empty()) familiaBase = "F1";
            
            size_t pos = familiaBase.find('-');
            if (pos != string::npos) {
                familiaBase = familiaBase.substr(0, pos);
            }
            
            Nodo* hijo = generarHijo(ciudadano, familiaBase);
            if (hijo != NULL) {
                ciudadano->energia -= 30; // Consume energía al reproducirse
                if (ciudadano->energia < 0) ciudadano->energia = 0;
                ciudadano->intencion = "Reproducirse";
                
                cout << "Nacimiento: " << hijo->id << " de padre " << ciudadano->id << endl;
            }
        }
    }
}

/**
 * Aplica eventos aleatorios personales a un ciudadano
 */
void aplicarEventosPersonal(Nodo* ciudadano) {
    if (ciudadano == NULL) return;
    
    // Validar rangos de atributos
    if (ciudadano->salud < 0) ciudadano->salud = 0;
    if (ciudadano->salud > 100) ciudadano->salud = 100;
    if (ciudadano->energia < 0) ciudadano->energia = 0;
    if (ciudadano->energia > 100) ciudadano->energia = 100;
    if (ciudadano->fuerza < 1) ciudadano->fuerza = 1;
    if (ciudadano->fuerza > 10) ciudadano->fuerza = 10;
    if (ciudadano->inteligencia < 1) ciudadano->inteligencia = 1;
    if (ciudadano->inteligencia > 10) ciudadano->inteligencia = 10;
    if (ciudadano->carisma < 1) ciudadano->carisma = 1;
    if (ciudadano->carisma > 10) ciudadano->carisma = 10;
    
    // Enfermedad leve (E1)
    if (rand() % 100 < 5 && ciudadano->salud > 20) {
        int perdida = 5 + rand() % 11;
        ciudadano->salud = max(0, ciudadano->salud - perdida);
        cout << "  Evento: " << ciudadano->id << " sufre enfermedad leve (-" << perdida << " salud)" << endl;
    }
    
    // Recuperación milagrosa (E4)
    if (rand() % 100 < 2 && ciudadano->salud < 60) {
        int recuperacion = 10 + rand() % 16;
        ciudadano->salud = min(100, ciudadano->salud + recuperacion);
        cout << "  Evento: " << ciudadano->id << " tiene recuperacion milagrosa (+" << recuperacion << " salud)" << endl;
    }
    
    // Inspiración (E7)
    if (rand() % 100 < 2 && ciudadano->inteligencia > 6) {
        ciudadano->inteligencia = min(10, ciudadano->inteligencia + 1);
        ciudadano->estadoEmocional = "Inspirado";
        cout << "  Evento: " << ciudadano->id << " tiene inspiracion (+1 inteligencia)" << endl;
    }
    
    // Mutación menor (E11)
    if (rand() % 100 < 1) {
        int stat = rand() % 3;
        if (stat == 0) ciudadano->fuerza = min(10, ciudadano->fuerza + 1);
        else if (stat == 1) ciudadano->inteligencia = min(10, ciudadano->inteligencia + 1);
        else ciudadano->carisma = min(10, ciudadano->carisma + 1);
        cout << "  Evento: " << ciudadano->id << " experimenta mutacion menor" << endl;
    }
}

/**
 * Aplica eventos ambientales que afectan a toda la población
 */
void aplicarEventosAmbiental() {
    if (raiz == NULL) return;
    
    vector<Nodo*> todos;
    obtenerListaInorden(raiz, todos);
    
    if (todos.empty()) return;
    
    // Año próspero (E18)
    if (rand() % 100 < 5) {
        int mejora = 5 + rand() % 6;
        for (size_t i = 0; i < todos.size(); i++) {
            if (todos[i] != NULL) {
                Nodo* ciudadano = todos[i];
                ciudadano->salud = min(100, ciudadano->salud + mejora);
                ciudadano->energia = min(100, ciudadano->energia + mejora);
            }
        }
        cout << "  EVENTO AMBIENTAL: Ano prospero (+" << mejora << " salud/energia a todos)" << endl;
    }
    
    // Año desastroso (E19)
    if (rand() % 100 < 3) {
        int perdida = 5 + rand() % 6;
        for (size_t i = 0; i < todos.size(); i++) {
            if (todos[i] != NULL) {
                Nodo* ciudadano = todos[i];
                ciudadano->salud = max(0, ciudadano->salud - perdida);
                ciudadano->energia = max(0, ciudadano->energia - perdida);
            }
        }
        cout << "  EVENTO AMBIENTAL: Ano desastroso (-" << perdida << " salud/energia a todos)" << endl;
    }
}

