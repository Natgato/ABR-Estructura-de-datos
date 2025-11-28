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
string intToString(int valor)
{
    stringstream ss;
    ss << valor;
    return ss.str();
}

// ==================== ESTRUCTURAS DE DATOS ====================

struct Nodo
{
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
    string profesion;    // Campesino, Artesano, Guerrero, Sabio, Curandero, Fundador
    string temperamento; // Calmado, Agresivo, Curioso, Perezoso, Sabio, Nervioso
    string rareza;       // Comun, Raro, Epico, Legendario

    // Estado dinámico
    string estadoEmocional; // Feliz, Triste, Enojado, Neutro, Motivado, Inspirado, Agotado, Ansioso
    string intencion;       // Descansar, Entrenar, Meditar, Socializar, Trabajar, Explorar, Reproducirse

    // Habilidades especiales (para rarezas altas)
    vector<string> habilidades;

    // Punteros del ABB
    Nodo *izq;
    Nodo *der;

    // Referencia al padre del ABB (BST). Mantener separado de la relación
    // genealógica evita mezclar la estructura de búsqueda con la familia.
    Nodo *padreBST;

    // Relaciones genealógicas
    Nodo *progenitor;     // padre biológico/genealógico
    vector<Nodo *> hijos; // lista de hijos genealógicos
};

// ==================== VARIABLES GLOBALES ====================

Nodo *raiz = NULL;
int contadorID = 1000; // IDs comenzarán desde 1000
int cicloActual = 0;
vector<Nodo *> listaCiudadanos; // Para balanceo

// ==================== PROTOTIPOS DE FUNCIONES ====================

// Operaciones básicas del ABB
Nodo *crearNodo(int id, string familia, int generacion);
void insertarNodo(Nodo *&raiz, Nodo *nuevo);
Nodo *buscarNodo(Nodo *raiz, int id);
Nodo *encontrarMinimo(Nodo *nodo);
void eliminarNodo(Nodo *&raiz, int id);

// Recorridos del árbol
void recorridoInorden(Nodo *raiz);
void recorridoPreorden(Nodo *raiz);
void recorridoPostorden(Nodo *raiz);
void recorridoPorNiveles(Nodo *raiz);

// Visualización
void mostrarCiudadanoCompacto(Nodo *nodo);
void mostrarCiudadanoCompleto(Nodo *nodo);
void mostrarArbolCompleto(Nodo *raiz);

// Inicialización y generación
void inicializarAtributosBase(Nodo *nodo);
void asignarRasgosDerivados(Nodo *nodo);
void crearFundadores(int cantidad);

// Simulación
void ejecutarCicloSimulacion();
void envejecerCiudadanos(Nodo *raiz);
void actualizarEstados(Nodo *raiz);
void procesarReproduccion();
bool puedeReproducirse(Nodo *ciudadano);
Nodo *generarHijo(Nodo *padre, string familiaBase);

// Eventos aleatorios
void aplicarEventosPersonal(Nodo *ciudadano);
void aplicarEventosAmbiental();
void procesarEvento(Nodo *ciudadano, int eventoID);

// Utilidades
int calcularAltura(Nodo *raiz);
int contarNodos(Nodo *raiz);
void balancearArbol();
void obtenerListaInorden(Nodo *raiz, vector<Nodo *> &lista);
Nodo *construirABBEquilibrado(vector<Nodo *> &lista, int inicio, int fin);

// Reportes
void generarReporteGlobal();
void mostrarDistribucionGeneraciones(Nodo *raiz);

// Menú
void mostrarMenu();
void mostrarMenuSimulacion();

// ==================== IMPLEMENTACIÓN DE FUNCIONES ====================

/**
 * Crea un nuevo nodo (ciudadano) con valores iniciales
 */
Nodo *crearNodo(int id, string familia, int generacion)
{
    if (id <= 0)
        return NULL;
    if (familia.empty())
        return NULL;
    if (generacion < 0)
        return NULL;

    Nodo *nuevo = new Nodo;
    if (nuevo == NULL)
        return NULL;

    nuevo->id = id;
    nuevo->familia = familia;
    nuevo->generacion = generacion;
    nuevo->edad = 0;
    nuevo->izq = NULL;
    nuevo->der = NULL;
    nuevo->padreBST = NULL;
    nuevo->progenitor = NULL;
    nuevo->hijos.clear();
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
void inicializarAtributosBase(Nodo *nodo)
{
    if (nodo == NULL)
        return;

    nodo->salud = 80 + rand() % 21;       // 80-100
    nodo->energia = 70 + rand() % 31;     // 70-100
    nodo->fuerza = 1 + rand() % 10;       // 1-10
    nodo->inteligencia = 1 + rand() % 10; // 1-10
    nodo->carisma = 1 + rand() % 10;      // 1-10

    // Estado inicial
    nodo->estadoEmocional = "Neutro";
    nodo->intencion = "Trabajar";
}

/**
 * Asigna los rasgos derivados basados en los atributos base
 */
void asignarRasgosDerivados(Nodo *nodo)
{
    if (nodo == NULL)
        return;

    // Determinar profesión
    if (nodo->generacion == 0)
    {
        nodo->profesion = "Fundador";
    }
    else if (nodo->fuerza >= 7)
    {
        nodo->profesion = "Guerrero";
    }
    else if (nodo->inteligencia >= 7)
    {
        nodo->profesion = (rand() % 2 == 0) ? "Sabio" : "Artesano";
    }
    else if (nodo->carisma >= 7)
    {
        nodo->profesion = "Artesano";
    }
    else
    {
        nodo->profesion = "Campesino";
    }

    // Determinar temperamento
    int temp = rand() % 6;
    string temperamentos[] = {"Calmado", "Agresivo", "Curioso", "Perezoso", "Sabio", "Nervioso"};
    nodo->temperamento = temperamentos[temp];

    // Determinar rareza (80% común, 15% raro, 4% épico, 1% legendario)
    int probRareza = rand() % 100;
    if (probRareza < 80)
    {
        nodo->rareza = "Comun";
    }
    else if (probRareza < 95)
    {
        nodo->rareza = "Raro";
    }
    else if (probRareza < 99)
    {
        nodo->rareza = "Epico";
    }
    else
    {
        nodo->rareza = "Legendario";
    }

    // Asignar habilidades según rareza
    if (nodo->rareza == "Raro")
    {
        nodo->habilidades.push_back("Resistencia");
    }
    else if (nodo->rareza == "Epico")
    {
        nodo->habilidades.push_back("Maestria");
    }
    else if (nodo->rareza == "Legendario")
    {
        nodo->habilidades.push_back("Inmortalidad");
        nodo->habilidades.push_back("Sabiduria Eterna");
    }
}

/**
 * Inserta un nodo en el ABB manteniendo la propiedad de búsqueda
 */
void insertarNodo(Nodo *&raiz, Nodo *nuevo)
{
    if (nuevo == NULL)
        return;

    if (raiz == NULL)
    {
        raiz = nuevo;
        return;
    }

    if (nuevo->id < raiz->id)
    {
        if (raiz->izq == NULL)
        {
            raiz->izq = nuevo;
            nuevo->padreBST = raiz;
        }
        else
        {
            insertarNodo(raiz->izq, nuevo);
        }
    }
    else if (nuevo->id > raiz->id)
    {
        if (raiz->der == NULL)
        {
            raiz->der = nuevo;
            nuevo->padreBST = raiz;
        }
        else
        {
            insertarNodo(raiz->der, nuevo);
        }
    }
    else
    {
        // ID duplicado - por ahora lo ignoramos
        cout << "ADVERTENCIA: ID duplicado " << nuevo->id << endl;
    }
}

/**
 * Busca un nodo por su ID en el ABB
 */
Nodo *buscarNodo(Nodo *raiz, int id)
{
    if (raiz == NULL || raiz->id == id)
    {
        return raiz;
    }

    if (id < raiz->id)
    {
        return buscarNodo(raiz->izq, id);
    }
    else
    {
        return buscarNodo(raiz->der, id);
    }
}

/**
 * Encuentra el nodo con el valor mínimo en un subárbol
 */
Nodo *encontrarMinimo(Nodo *nodo)
{
    if (nodo == NULL)
        return NULL;
    while (nodo->izq != NULL)
    {
        nodo = nodo->izq;
    }
    return nodo;
}

/**
 * Elimina un nodo del ABB sin romper su estructura
 */
void eliminarNodo(Nodo *&raiz, int id)
{
    if (raiz == NULL)
    {
        return;
    }

    if (id < raiz->id)
    {
        eliminarNodo(raiz->izq, id);
    }
    else if (id > raiz->id)
    {
        eliminarNodo(raiz->der, id);
    }
    else
    {
        // Nodo encontrado
        // Caso 1: Nodo hoja
        if (raiz->izq == NULL && raiz->der == NULL)
        {
            delete raiz;
            raiz = NULL;
        }
        // Caso 2: Nodo con un solo hijo
        else if (raiz->izq == NULL)
        {
            Nodo *temp = raiz;
            raiz = raiz->der;
            if (raiz != NULL)
            {
                raiz->padreBST = temp->padreBST;
            }
            delete temp;
        }
        else if (raiz->der == NULL)
        {
            Nodo *temp = raiz;
            raiz = raiz->izq;
            if (raiz != NULL)
            {
                raiz->padreBST = temp->padreBST;
            }
            delete temp;
        }
        // Caso 3: Nodo con dos hijos
        else
        {
            Nodo *sucesor = encontrarMinimo(raiz->der);
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
void recorridoInorden(Nodo *raiz)
{
    if (raiz == NULL)
        return;

    recorridoInorden(raiz->izq);
    mostrarCiudadanoCompacto(raiz);
    recorridoInorden(raiz->der);
}

/**
 * Recorrido preorden: muestra desde los ancestros hacia abajo
 */
void recorridoPreorden(Nodo *raiz)
{
    if (raiz == NULL)
        return;

    mostrarCiudadanoCompacto(raiz);
    recorridoPreorden(raiz->izq);
    recorridoPreorden(raiz->der);
}

/**
 * Recorrido postorden: muestra primero descendientes, luego ancestros
 */
void recorridoPostorden(Nodo *raiz)
{
    if (raiz == NULL)
        return;

    recorridoPostorden(raiz->izq);
    recorridoPostorden(raiz->der);
    mostrarCiudadanoCompacto(raiz);
}

/**
 * Recorrido por niveles: agrupa por generaciones
 */
void recorridoPorNiveles(Nodo *raiz)
{
    if (raiz == NULL)
        return;

    queue<Nodo *> cola;
    cola.push(raiz);

    while (!cola.empty())
    {
        Nodo *actual = cola.front();
        cola.pop();

        mostrarCiudadanoCompacto(actual);

        if (actual->izq != NULL)
        {
            cola.push(actual->izq);
        }
        if (actual->der != NULL)
        {
            cola.push(actual->der);
        }
    }
}

/**
 * Muestra una representación compacta del ciudadano
 */
void mostrarCiudadanoCompacto(Nodo *nodo)
{
    if (nodo == NULL)
        return;

    cout << "ID: " << setw(5) << nodo->id
         << " | " << setw(8) << (nodo->familia.empty() ? "N/A" : nodo->familia)
         << " | G" << nodo->generacion
         << " | PROF: " << setw(10) << (nodo->profesion.empty() ? "N/A" : nodo->profesion)
         << " | RAREZA: " << setw(10) << (nodo->rareza.empty() ? "N/A" : nodo->rareza)
         << " | ESTADO: " << setw(10) << (nodo->estadoEmocional.empty() ? "N/A" : nodo->estadoEmocional)
         << " / INT: " << setw(12) << (nodo->intencion.empty() ? "N/A" : nodo->intencion);

    // Mostrar hijos genealógicos (lista)
    cout << " | HIJOS: ";
    if (!nodo->hijos.empty())
    {
        for (size_t i = 0; i < nodo->hijos.size(); ++i)
        {
            if (i)
                cout << ", ";
            cout << nodo->hijos[i]->id;
        }
    }
    else
    {
        cout << "-";
    }
    cout << endl;
}

/**
 * Muestra la información completa de un ciudadano
 */
void mostrarCiudadanoCompleto(Nodo *nodo)
{
    if (nodo == NULL)
    {
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

    if (!nodo->habilidades.empty())
    {
        cout << "\n--- Habilidades Especiales ---" << endl;
        for (size_t i = 0; i < nodo->habilidades.size(); i++)
        {
            cout << "- " << nodo->habilidades[i] << endl;
        }
    }

    // Mostrar vínculos genealógicos
    cout << "\n--- Vinculos Genealogicos ---" << endl;
    if (nodo->progenitor != NULL)
    {
        cout << "Padre: ID " << nodo->progenitor->id << " (" << nodo->progenitor->familia << ")" << endl;
    }
    else
    {
        cout << "Padre: Fundador" << endl;
    }
    cout << "Hijos genealógicos: ";
    if (!nodo->hijos.empty())
    {
        for (size_t i = 0; i < nodo->hijos.size(); ++i)
        {
            if (i)
                cout << ", ";
            cout << "ID " << nodo->hijos[i]->id << " (" << nodo->hijos[i]->familia << ")";
        }
        cout << endl;
    }
    else
    {
        cout << "Ninguno" << endl;
    }
    cout << "==============================================\n"
         << endl;
}

/**
 * Crea ciudadanos fundadores (generación 0)
 */
void crearFundadores(int cantidad)
{
    if (cantidad <= 0)
    {
        cout << "Error: La cantidad debe ser mayor que 0." << endl;
        return;
    }
    if (cantidad > 100)
    {
        cout << "Advertencia: Cantidad muy grande (" << cantidad << "). Limitando a 100." << endl;
        cantidad = 100;
    }

    for (int i = 0; i < cantidad; i++)
    {
        int id = contadorID++;
        string familia = "F" + intToString(i + 1);
        Nodo *fundador = crearNodo(id, familia, 0);
        if (fundador != NULL)
        {
            fundador->profesion = "Fundador";
            insertarNodo(raiz, fundador);
        }
    }
    cout << cantidad << " fundadores creados exitosamente." << endl;
}

/**
 * Verifica si un ciudadano puede reproducirse
 */
bool puedeReproducirse(Nodo *ciudadano)
{
    if (ciudadano == NULL)
        return false;

    // Condiciones básicas
    if (ciudadano->edad < 18 || ciudadano->edad > 50)
        return false;
    if (ciudadano->salud < 40)
        return false;
    if (ciudadano->energia < 50)
        return false;

    // Verificar si ya tiene dos hijos (genealógicos)
    if (ciudadano->hijos.size() >= 2)
        return false;

    // Probabilidad según atributos y estado
    int probabilidad = 30; // Base 30%

    if (ciudadano->estadoEmocional == "Motivado")
        probabilidad += 20;
    if (ciudadano->estadoEmocional == "Feliz")
        probabilidad += 10;
    if (ciudadano->salud > 80)
        probabilidad += 10;
    if (ciudadano->energia > 80)
        probabilidad += 10;

    int roll = rand() % 100;
    return roll < probabilidad;
}

/**
 * Genera un hijo para un ciudadano
 */
Nodo *generarHijo(Nodo *padre, string familiaBase)
{
    if (padre == NULL)
        return NULL;

    int id = contadorID++;
    int nuevaGeneracion = padre->generacion + 1;
    string familiaHijo = familiaBase + "-" + intToString(nuevaGeneracion);

    Nodo *hijo = crearNodo(id, familiaHijo, nuevaGeneracion);
    if (hijo == NULL)
        return NULL;

    // Herencia: el hijo hereda parcialmente los atributos del padre
    hijo->fuerza = max(1, min(10, padre->fuerza + (rand() % 5 - 2))); // -2 a +2
    hijo->inteligencia = max(1, min(10, padre->inteligencia + (rand() % 5 - 2)));
    hijo->carisma = max(1, min(10, padre->carisma + (rand() % 5 - 2)));
    hijo->salud = max(60, min(100, padre->salud + (rand() % 11 - 5)));

    // Probabilidad de heredar rareza
    if (padre->rareza != "Comun" && rand() % 100 < 30)
    {
        hijo->rareza = padre->rareza;
    }

    // Probabilidad de mutación
    if (rand() % 100 < 5)
    {
        int statMutado = rand() % 3;
        int cambio = 1 + rand() % 2;
        if (statMutado == 0)
            hijo->fuerza = min(10, hijo->fuerza + cambio);
        else if (statMutado == 1)
            hijo->inteligencia = min(10, hijo->inteligencia + cambio);
        else
            hijo->carisma = min(10, hijo->carisma + cambio);
    }

    // Reasignar profesión según nuevos atributos
    asignarRasgosDerivados(hijo);

    // Registrar relación genealógica (padre biológico) y lista de hijos
    hijo->progenitor = padre;
    if (padre != NULL)
    {
        padre->hijos.push_back(hijo);
    }

    // Insertar en el ABB — insertarNodo mantiene padreBST correctamente.
    insertarNodo(raiz, hijo);

    return hijo;
}

/**
 * Procesa la reproducción de todos los ciudadanos
 */
void procesarReproduccion()
{
    if (raiz == NULL)
        return;

    vector<Nodo *> candidatos;

    // Obtener todos los ciudadanos
    obtenerListaInorden(raiz, candidatos);

    for (size_t i = 0; i < candidatos.size(); i++)
    {
        Nodo *ciudadano = candidatos[i];
        if (ciudadano == NULL)
            continue;

        if (puedeReproducirse(ciudadano) && (ciudadano->hijos.size() < 2))
        {

            // Determinar la familia base
            string familiaBase = ciudadano->familia;
            if (familiaBase.empty())
                familiaBase = "F1";

            size_t pos = familiaBase.find('-');
            if (pos != string::npos)
            {
                familiaBase = familiaBase.substr(0, pos);
            }

            Nodo *hijo = generarHijo(ciudadano, familiaBase);
            if (hijo != NULL)
            {
                ciudadano->energia -= 30; // Consume energía al reproducirse
                if (ciudadano->energia < 0)
                    ciudadano->energia = 0;
                ciudadano->intencion = "Reproducirse";

                cout << "Nacimiento: " << hijo->id << " de padre " << ciudadano->id << endl;
            }
        }
    }
}

/**
 * Aplica eventos aleatorios personales a un ciudadano
 */
void aplicarEventosPersonal(Nodo *ciudadano)
{
    if (ciudadano == NULL)
        return;

    // Validar rangos de atributos
    if (ciudadano->salud < 0)
        ciudadano->salud = 0;
    if (ciudadano->salud > 100)
        ciudadano->salud = 100;
    if (ciudadano->energia < 0)
        ciudadano->energia = 0;
    if (ciudadano->energia > 100)
        ciudadano->energia = 100;
    if (ciudadano->fuerza < 1)
        ciudadano->fuerza = 1;
    if (ciudadano->fuerza > 10)
        ciudadano->fuerza = 10;
    if (ciudadano->inteligencia < 1)
        ciudadano->inteligencia = 1;
    if (ciudadano->inteligencia > 10)
        ciudadano->inteligencia = 10;
    if (ciudadano->carisma < 1)
        ciudadano->carisma = 1;
    if (ciudadano->carisma > 10)
        ciudadano->carisma = 10;

    // Enfermedad leve (E1)
    if (rand() % 100 < 5 && ciudadano->salud > 20)
    {
        int perdida = 5 + rand() % 11;
        ciudadano->salud = max(0, ciudadano->salud - perdida);
        cout << "  Evento: " << ciudadano->id << " sufre enfermedad leve (-" << perdida << " salud)" << endl;
    }

    // Recuperación milagrosa (E4)
    if (rand() % 100 < 2 && ciudadano->salud < 60)
    {
        int recuperacion = 10 + rand() % 16;
        ciudadano->salud = min(100, ciudadano->salud + recuperacion);
        cout << "  Evento: " << ciudadano->id << " tiene recuperacion milagrosa (+" << recuperacion << " salud)" << endl;
    }

    // Inspiración (E7)
    if (rand() % 100 < 2 && ciudadano->inteligencia > 6)
    {
        ciudadano->inteligencia = min(10, ciudadano->inteligencia + 1);
        ciudadano->estadoEmocional = "Inspirado";
        cout << "  Evento: " << ciudadano->id << " tiene inspiracion (+1 inteligencia)" << endl;
    }

    // Mutación menor (E11)
    if (rand() % 100 < 1)
    {
        int stat = rand() % 3;
        if (stat == 0)
            ciudadano->fuerza = min(10, ciudadano->fuerza + 1);
        else if (stat == 1)
            ciudadano->inteligencia = min(10, ciudadano->inteligencia + 1);
        else
            ciudadano->carisma = min(10, ciudadano->carisma + 1);
        cout << "  Evento: " << ciudadano->id << " experimenta mutacion menor" << endl;
    }
}

/**
 * Aplica eventos ambientales que afectan a toda la población
 */
void aplicarEventosAmbiental()
{
    if (raiz == NULL)
        return;

    vector<Nodo *> todos;
    obtenerListaInorden(raiz, todos);

    if (todos.empty())
        return;

    // Año próspero (E18)
    if (rand() % 100 < 5)
    {
        int mejora = 5 + rand() % 6;
        for (size_t i = 0; i < todos.size(); i++)
        {
            if (todos[i] != NULL)
            {
                Nodo *ciudadano = todos[i];
                ciudadano->salud = min(100, ciudadano->salud + mejora);
                ciudadano->energia = min(100, ciudadano->energia + mejora);
            }
        }
        cout << "  EVENTO AMBIENTAL: Ano prospero (+" << mejora << " salud/energia a todos)" << endl;
    }

    // Año desastroso (E19)
    if (rand() % 100 < 3)
    {
        int perdida = 5 + rand() % 6;
        for (size_t i = 0; i < todos.size(); i++)
        {
            if (todos[i] != NULL)
            {
                Nodo *ciudadano = todos[i];
                ciudadano->salud = max(0, ciudadano->salud - perdida);
                ciudadano->energia = max(0, ciudadano->energia - perdida);
            }
        }
        cout << "  EVENTO AMBIENTAL: Ano desastroso (-" << perdida << " salud/energia a todos)" << endl;
    }
}

/**
 * Envejece a todos los ciudadanos
 */
void envejecerCiudadanos(Nodo *raiz)
{
    if (raiz == NULL)
        return;

    envejecerCiudadanos(raiz->izq);
    raiz->edad++;

    // Regresión por edad (E14)
    if (raiz->edad > 60 && rand() % 100 < 3)
    {
        int stat = rand() % 3;
        int perdida = 1 + rand() % 3;
        if (stat == 0)
            raiz->fuerza = max(1, raiz->fuerza - perdida);
        else if (stat == 1)
            raiz->inteligencia = max(1, raiz->inteligencia - perdida);
        else
            raiz->carisma = max(1, raiz->carisma - perdida);
    }

    envejecerCiudadanos(raiz->der);
}

/**
 * Actualiza los estados de todos los ciudadanos
 */
void actualizarEstados(Nodo *raiz)
{
    if (raiz == NULL)
        return;

    actualizarEstados(raiz->izq);

    // Determinar intención según estado
    if (raiz->energia < 30)
    {
        raiz->intencion = "Descansar";
        raiz->energia = min(100, raiz->energia + 10);
    }
    else if (raiz->fuerza < 5 && rand() % 2 == 0)
    {
        raiz->intencion = "Entrenar";
        raiz->fuerza = min(10, raiz->fuerza + 1);
    }
    else if (raiz->inteligencia < 5 && rand() % 2 == 0)
    {
        raiz->intencion = "Meditar";
        raiz->inteligencia = min(10, raiz->inteligencia + 1);
    }
    else if (raiz->carisma < 5 && rand() % 2 == 0)
    {
        raiz->intencion = "Socializar";
        raiz->carisma = min(10, raiz->carisma + 1);
    }
    else
    {
        raiz->intencion = "Trabajar";
    }

    // Actualizar estado emocional
    if (raiz->salud > 80 && raiz->energia > 80)
    {
        raiz->estadoEmocional = "Feliz";
    }
    else if (raiz->salud < 30 || raiz->energia < 30)
    {
        raiz->estadoEmocional = "Agotado";
    }
    else
    {
        raiz->estadoEmocional = "Neutro";
    }

    actualizarEstados(raiz->der);
}

/**
 * Verifica y elimina ciudadanos que deben morir
 */
void verificarMuerte()
{
    if (raiz == NULL)
        return;

    vector<Nodo *> muertos;
    vector<Nodo *> todos;
    obtenerListaInorden(raiz, todos);

    for (size_t i = 0; i < todos.size(); i++)
    {
        Nodo *ciudadano = todos[i];
        if (ciudadano == NULL)
            continue;

        bool debeMorir = false;

        // Muerte por edad avanzada
        if (ciudadano->edad > 90)
        {
            int probabilidad = 10 + (ciudadano->edad - 90) * 5;
            if (rand() % 100 < probabilidad)
            {
                debeMorir = true;
            }
        }

        // Muerte por salud crítica
        if (ciudadano->salud <= 0)
        {
            debeMorir = true;
        }

        // Inanición (E15)
        if (ciudadano->energia == 0 && rand() % 100 < 30)
        {
            ciudadano->salud = max(0, ciudadano->salud - (10 + rand() % 11));
            if (ciudadano->salud <= 0)
            {
                debeMorir = true;
            }
        }

        if (debeMorir)
        {
            muertos.push_back(ciudadano);
        }
    }

    // Eliminar muertos del árbol
    for (size_t i = 0; i < muertos.size(); i++)
    {
        Nodo *muerto = muertos[i];
        cout << "Muerte: " << muerto->id << " (edad: " << muerto->edad << ", salud: " << muerto->salud << ")" << endl;

        // Desconectar vínculo genealógico (si existe) — quitarlo de la lista de hijos
        if (muerto->progenitor != NULL)
        {
            vector<Nodo *> &hermanos = muerto->progenitor->hijos;
            for (size_t k = 0; k < hermanos.size(); ++k)
            {
                if (hermanos[k] == muerto)
                {
                    hermanos.erase(hermanos.begin() + k);
                    break;
                }
            }
        }

        eliminarNodo(raiz, muerto->id);
    }
}

/**
 * Ejecuta un ciclo completo de simulación
 */
void ejecutarCicloSimulacion()
{
    if (raiz == NULL)
    {
        cout << "Error: No hay ciudadanos en el arbol. Cree fundadores primero." << endl;
        return;
    }

    cicloActual++;
    cout << "\n========== CICLO " << cicloActual << " ==========" << endl;

    // 1. Envejecer ciudadanos
    envejecerCiudadanos(raiz);

    // 2. Actualizar estados
    actualizarEstados(raiz);

    // 3. Aplicar eventos ambientales
    aplicarEventosAmbiental();

    // 4. Aplicar eventos personales
    vector<Nodo *> todos;
    obtenerListaInorden(raiz, todos);
    for (size_t i = 0; i < todos.size(); i++)
    {
        if (todos[i] != NULL)
        {
            aplicarEventosPersonal(todos[i]);
        }
    }

    // 5. Procesar reproducción
    procesarReproduccion();

    // 6. Verificar muertes
    verificarMuerte();

    // 7. Mostrar resumen
    int poblacion = contarNodos(raiz);
    int altura = calcularAltura(raiz);
    cout << "\nResumen del ciclo: Poblacion = " << poblacion
         << ", Altura del arbol = " << altura << endl;
}

/**
 * Calcula la altura del árbol
 */
int calcularAltura(Nodo *raiz)
{
    if (raiz == NULL)
        return 0;

    int alturaIzq = calcularAltura(raiz->izq);
    int alturaDer = calcularAltura(raiz->der);

    return 1 + max(alturaIzq, alturaDer);
}

/**
 * Cuenta el número de nodos en el árbol
 */
int contarNodos(Nodo *raiz)
{
    if (raiz == NULL)
        return 0;
    return 1 + contarNodos(raiz->izq) + contarNodos(raiz->der);
}

/**
 * Obtiene una lista ordenada de todos los nodos mediante inorden
 */
void obtenerListaInorden(Nodo *raiz, vector<Nodo *> &lista)
{
    if (raiz == NULL)
        return;
    obtenerListaInorden(raiz->izq, lista);
    lista.push_back(raiz);
    obtenerListaInorden(raiz->der, lista);
}

/**
 * Construye un ABB equilibrado desde una lista ordenada
 */
Nodo *construirABBEquilibrado(vector<Nodo *> &lista, int inicio, int fin)
{
    if (inicio > fin)
        return NULL;
    if (inicio < 0 || fin < 0)
        return NULL;
    if (fin >= (int)lista.size())
        return NULL;

    int medio = (inicio + fin) / 2;
    if (medio < 0 || medio >= (int)lista.size() || lista[medio] == NULL)
        return NULL;

    Nodo *raiz = lista[medio];

    raiz->izq = construirABBEquilibrado(lista, inicio, medio - 1);
    raiz->der = construirABBEquilibrado(lista, medio + 1, fin);

    if (raiz->izq != NULL)
        raiz->izq->padreBST = raiz;
    if (raiz->der != NULL)
        raiz->der->padreBST = raiz;

    return raiz;
}

/**
 * Balancea el árbol reconstruyéndolo de forma equilibrada
 */
void balancearArbol()
{
    if (raiz == NULL)
    {
        cout << "Error: El arbol esta vacio. No hay nada que balancear." << endl;
        return;
    }

    vector<Nodo *> lista;
    obtenerListaInorden(raiz, lista);

    if (lista.empty())
    {
        cout << "Error: No se pudo obtener la lista de nodos." << endl;
        return;
    }

    // Limpiar referencias padre-hijo
    for (size_t i = 0; i < lista.size(); i++)
    {
        if (lista[i] != NULL)
        {
            Nodo *nodo = lista[i];
            nodo->izq = NULL;
            nodo->der = NULL;
            nodo->padreBST = NULL;
        }
    }

    raiz = construirABBEquilibrado(lista, 0, lista.size() - 1);

    cout << "Arbol balanceado exitosamente." << endl;
}

/**
 * Genera un reporte global del estado de la civilización
 */
void generarReporteGlobal()
{
    cout << "\n========== REPORTE GLOBAL ==========" << endl;

    if (raiz == NULL)
    {
        cout << "El arbol esta vacio. No hay datos para mostrar." << endl;
        cout << "=====================================\n"
             << endl;
        return;
    }

    int poblacion = contarNodos(raiz);
    int altura = calcularAltura(raiz);

    cout << "\n--- Estadisticas Generales ---" << endl;
    cout << "Poblacion total: " << poblacion << endl;
    cout << "Altura del arbol: " << altura << endl;
    cout << "Ciclo actual: " << cicloActual << endl;

    // Distribución por generaciones
    vector<Nodo *> todos;
    obtenerListaInorden(raiz, todos);

    if (todos.empty())
    {
        cout << "No hay ciudadanos en el arbol." << endl;
        cout << "=====================================\n"
             << endl;
        return;
    }

    int maxGeneracion = 0;
    for (size_t i = 0; i < todos.size(); i++)
    {
        if (todos[i]->generacion > maxGeneracion)
        {
            maxGeneracion = todos[i]->generacion;
        }
    }

    cout << "\n--- Distribucion por Generaciones ---" << endl;
    for (int g = 0; g <= maxGeneracion; g++)
    {
        int count = 0;
        for (size_t i = 0; i < todos.size(); i++)
        {
            if (todos[i]->generacion == g)
                count++;
        }
        cout << "Generacion " << g << ": " << count << " ciudadanos" << endl;
    }

    // Distribución por profesión
    cout << "\n--- Distribucion por Profesion ---" << endl;
    string profesiones[] = {"Fundador", "Campesino", "Artesano", "Guerrero", "Sabio", "Curandero"};
    int numProfesiones = 6;
    for (int j = 0; j < numProfesiones; j++)
    {
        string prof = profesiones[j];
        int count = 0;
        for (size_t i = 0; i < todos.size(); i++)
        {
            if (todos[i]->profesion == prof)
                count++;
        }
        if (count > 0)
        {
            cout << prof << ": " << count << endl;
        }
    }

    // Distribución por rareza
    cout << "\n--- Distribucion por Rareza ---" << endl;
    string rarezas[] = {"Comun", "Raro", "Epico", "Legendario"};
    int numRarezas = 4;
    for (int j = 0; j < numRarezas; j++)
    {
        string rar = rarezas[j];
        int count = 0;
        for (size_t i = 0; i < todos.size(); i++)
        {
            if (todos[i]->rareza == rar)
                count++;
        }
        if (count > 0)
        {
            cout << rar << ": " << count << endl;
        }
    }

    cout << "\n=====================================\n"
         << endl;
}

/**
 * Muestra el menú principal
 */
void mostrarMenu()
{
    cout << "\n========== MENU PRINCIPAL ==========" << endl;
    cout << "1. Crear fundadores" << endl;
    cout << "2. Insertar ciudadano manualmente" << endl;
    cout << "3. Buscar ciudadano por ID" << endl;
    cout << "4. Eliminar ciudadano" << endl;
    cout << "5. Mostrar arbol (inorden)" << endl;
    cout << "6. Mostrar arbol (preorden)" << endl;
    cout << "7. Mostrar arbol (postorden)" << endl;
    cout << "8. Mostrar arbol (por niveles)" << endl;
    cout << "9. Ejecutar simulacion" << endl;
    cout << "10. Balancear arbol" << endl;
    cout << "11. Generar reporte global" << endl;
    cout << "12. Calcular altura del arbol" << endl;
    cout << "0. Salir" << endl;
    cout << "=====================================" << endl;
    cout << "Seleccione una opcion: ";
}

/**
 * Muestra el menú de simulación
 */
void mostrarMenuSimulacion()
{
    cout << "\n========== MENU DE SIMULACION ==========" << endl;
    cout << "1. Ejecutar un ciclo" << endl;
    cout << "2. Ejecutar N ciclos" << endl;
    cout << "3. Volver al menu principal" << endl;
    cout << "=========================================" << endl;
    cout << "Seleccione una opcion: ";
}

// ==================== FUNCIÓN PRINCIPAL ====================

int main()
{
    srand(time(NULL)); // Inicializar semilla aleatoria

    int opcion;
    bool enSimulacion = false;

    do
    {
        if (!enSimulacion)
        {
            mostrarMenu();
            cin >> opcion;

            switch (opcion)
            {
            case 1:
            {
                int cantidad;
                cout << "Ingrese cantidad de fundadores a crear: ";
                cin >> cantidad;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Error: Entrada invalida. Debe ingresar un numero entero." << endl;
                }
                else
                {
                    crearFundadores(cantidad);
                }
                break;
            }

            case 2:
            {
                int id;
                string familia;
                int generacion;

                cout << "Ingrese ID: ";
                cin >> id;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Error: ID invalido." << endl;
                    break;
                }

                if (id <= 0)
                {
                    cout << "Error: El ID debe ser mayor que 0." << endl;
                    break;
                }

                if (buscarNodo(raiz, id) != NULL)
                {
                    cout << "Error: Ya existe un ciudadano con ese ID." << endl;
                    break;
                }

                cout << "Ingrese familia (ej. F1): ";
                cin >> familia;
                if (familia.empty())
                {
                    cout << "Error: La familia no puede estar vacia." << endl;
                    break;
                }

                cout << "Ingrese generacion: ";
                cin >> generacion;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Error: Generacion invalida." << endl;
                    break;
                }

                if (generacion < 0)
                {
                    cout << "Error: La generacion no puede ser negativa." << endl;
                    break;
                }

                Nodo *nuevo = crearNodo(id, familia, generacion);
                if (nuevo != NULL)
                {
                    insertarNodo(raiz, nuevo);
                    cout << "Ciudadano insertado exitosamente." << endl;
                }
                else
                {
                    cout << "Error: No se pudo crear el ciudadano." << endl;
                }
                break;
            }

            case 3:
            {
                if (raiz == NULL)
                {
                    cout << "Error: El arbol esta vacio. No hay ciudadanos para buscar." << endl;
                    break;
                }

                int id;
                cout << "Ingrese ID a buscar: ";
                cin >> id;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Error: ID invalido." << endl;
                    break;
                }

                Nodo *encontrado = buscarNodo(raiz, id);
                mostrarCiudadanoCompleto(encontrado);
                break;
            }

            case 4:
            {
                if (raiz == NULL)
                {
                    cout << "Error: El arbol esta vacio. No hay ciudadanos para eliminar." << endl;
                    break;
                }

                int id;
                cout << "Ingrese ID a eliminar: ";
                cin >> id;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Error: ID invalido." << endl;
                    break;
                }

                Nodo *encontrado = buscarNodo(raiz, id);
                if (encontrado != NULL)
                {
                    eliminarNodo(raiz, id);
                    cout << "Ciudadano eliminado exitosamente." << endl;
                }
                else
                {
                    cout << "Ciudadano no encontrado." << endl;
                }
                break;
            }

            case 5:
                if (raiz == NULL)
                {
                    cout << "El arbol esta vacio. No hay ciudadanos para mostrar." << endl;
                }
                else
                {
                    cout << "\n--- Recorrido Inorden ---" << endl;
                    recorridoInorden(raiz);
                }
                break;

            case 6:
                if (raiz == NULL)
                {
                    cout << "El arbol esta vacio. No hay ciudadanos para mostrar." << endl;
                }
                else
                {
                    cout << "\n--- Recorrido Preorden ---" << endl;
                    recorridoPreorden(raiz);
                }
                break;

            case 7:
                if (raiz == NULL)
                {
                    cout << "El arbol esta vacio. No hay ciudadanos para mostrar." << endl;
                }
                else
                {
                    cout << "\n--- Recorrido Postorden ---" << endl;
                    recorridoPostorden(raiz);
                }
                break;

            case 8:
                if (raiz == NULL)
                {
                    cout << "El arbol esta vacio. No hay ciudadanos para mostrar." << endl;
                }
                else
                {
                    cout << "\n--- Recorrido por Niveles ---" << endl;
                    recorridoPorNiveles(raiz);
                }
                break;

            case 9:
                if (raiz == NULL)
                {
                    cout << "Error: No hay ciudadanos en el arbol. Cree fundadores primero." << endl;
                }
                else
                {
                    enSimulacion = true;
                }
                break;

            case 10:
                balancearArbol();
                break;

            case 11:
                generarReporteGlobal();
                break;

            case 12:
            {
                if (raiz == NULL)
                {
                    cout << "Altura del arbol: 0 (arbol vacio)" << endl;
                }
                else
                {
                    int altura = calcularAltura(raiz);
                    cout << "Altura del arbol: " << altura << endl;
                }
                break;
            }

            case 0:
                cout << "Saliendo del programa..." << endl;
                break;

            default:
                cout << "Opcion invalida. Intente nuevamente." << endl;
            }
        }
        else
        {
            mostrarMenuSimulacion();
            int opcionSim;
            cin >> opcionSim;

            switch (opcionSim)
            {
            case 1:
                ejecutarCicloSimulacion();
                break;

            case 2:
            {
                int n;
                cout << "Ingrese cantidad de ciclos a ejecutar: ";
                cin >> n;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Error: Entrada invalida. Debe ingresar un numero entero." << endl;
                    break;
                }
                if (n <= 0)
                {
                    cout << "Error: La cantidad debe ser mayor que 0." << endl;
                    break;
                }
                if (n > 1000)
                {
                    cout << "Advertencia: Cantidad muy grande (" << n << "). Limitando a 1000." << endl;
                    n = 1000;
                }
                for (int i = 0; i < n; i++)
                {
                    ejecutarCicloSimulacion();
                }
                break;
            }

            case 3:
                enSimulacion = false;
                break;

            default:
                cout << "Opcion invalida." << endl;
            }
        }

    } while (opcion != 0);

    return 0;
}