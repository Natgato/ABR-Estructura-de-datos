
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

string intToString(int valor) {
    stringstream ss;
    ss << valor;
    return ss.str();
}


struct Nodo {
    // IdentificaciÃ³n
    int id;
    string familia;
    int generacion;
    
    // Atributos base (numÃ©ricos)
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
    
    // Estado dinÃ¡mico
    string estadoEmocional; // Feliz, Triste, Enojado, Neutro, Motivado, Inspirado, Agotado, Ansioso
    string intencion;       // Descansar, Entrenar, Meditar, Socializar, Trabajar, Explorar, Reproducirse
    
    // Habilidades especiales (para rarezas altas)
    vector<string> habilidades;
    
    // Punteros del ABB
    Nodo* izq;
    Nodo* der;
    
    // Referencia al padre (opcional, Ãºtil para navegaciÃ³n)
    Nodo* padre;
};

// ==================== VARIABLES GLOBALES ====================

Nodo* raiz = NULL;
int contadorID = 1000;  // IDs comenzarÃ¡n desde 1000
int cicloActual = 0;
vector<Nodo*> listaCiudadanos; // Para balanceo

// ==================== PROTOTIPOS DE FUNCIONES ====================

// Operaciones bÃ¡sicas del ABB
Nodo* crearNodo(int id, string familia, int generacion);
void insertarNodo(Nodo*& raiz, Nodo* nuevo);
Nodo* buscarNodo(Nodo* raiz, int id);
Nodo* encontrarMinimo(Nodo* nodo);
void eliminarNodo(Nodo*& raiz, int id);

// Recorridos del Ã¡rbol
void recorridoInorden(Nodo* raiz);
void recorridoPreorden(Nodo* raiz);
void recorridoPostorden(Nodo* raiz);
void recorridoPorNiveles(Nodo* raiz);

// VisualizaciÃ³n
void mostrarCiudadanoCompacto(Nodo* nodo);
void mostrarCiudadanoCompleto(Nodo* nodo);
void mostrarArbolCompleto(Nodo* raiz);

// InicializaciÃ³n y generaciÃ³n
void inicializarAtributosBase(Nodo* nodo);
void asignarRasgosDerivados(Nodo* nodo);
void crearFundadores(int cantidad);

// SimulaciÃ³n
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

// MenÃº
void mostrarMenu();
void mostrarMenuSimulacion();

// ==================== IMPLEMENTACIÃ“N DE FUNCIONES ====================

/**
 * Crea un nuevo nodo (ciudadano) con valores iniciales
 */
Nodo* crearNodo(int id, string familia, int generacion) {
    Nodo* nuevo = new Nodo;
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
    // Determinar profesiÃ³n
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
    
    // Determinar rareza (80% comÃºn, 15% raro, 4% Ã©pico, 1% legendario)
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
    
    // Asignar habilidades segÃºn rareza
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
 * Inserta un nodo en el ABB manteniendo la propiedad de bÃºsqueda
 */
void insertarNodo(Nodo*& raiz, Nodo* nuevo) {
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
 * Encuentra el nodo con el valor mÃ­nimo en un subÃ¡rbol
 */
Nodo* encontrarMinimo(Nodo* nodo) {
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
            raiz->padre = temp->padre;
            delete temp;
        } else if (raiz->der == NULL) {
            Nodo* temp = raiz;
            raiz = raiz->izq;
            raiz->padre = temp->padre;
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
 * Muestra una representaciÃ³n compacta del ciudadano
 */
void mostrarCiudadanoCompacto(Nodo* nodo) {
    if (nodo == NULL) return;
    
    cout << "ID: " << setw(5) << nodo->id 
         << " | " << setw(8) << nodo->familia
         << " | G" << nodo->generacion
         << " | PROF: " << setw(10) << nodo->profesion
         << " | RAREZA: " << setw(10) << nodo->rareza
         << " | ESTADO: " << setw(10) << nodo->estadoEmocional
         << " / INT: " << setw(12) << nodo->intencion;
    
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
 * Muestra la informaciÃ³n completa de un ciudadano
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
    cout << "Edad: " << nodo->edad << " aÃ±os" << endl;
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
    
    // Mostrar vÃ­nculos genealÃ³gicos
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

