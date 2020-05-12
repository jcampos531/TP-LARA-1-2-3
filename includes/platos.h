#ifndef PLATOS_H_INCLUDED
#define PLATOS_H_INCLUDED
struct Platos{
    int ID_Plato, tiempo, ID_Res, comRes, ID_Cat;
    char nombre [50];
    float costo, precio;
    bool estado;
};


//ERRORES DE EDICION/CREACION DE PLATOS

const int ID_ERROR = 0;
const int ID_REPETIDO = 1;
const int NAME_ERROR = 2;
const int COST_ERROR = 3;
const int PRICE_ERROR = 4;
const int TIME_ERROR = 5;
const int ID_REST_ERROR = 6;
const int COM_RES_ERROR = 7;
const int ID_CAT_ERROR = 8;
const int ID_INEXISTENTE = 9;
const int PLATO_INACTIVO_ERROR = 10;
const int ID_REST_SIN_PLATOS = 11;

const char ERROR_PLATO[][80] = {
    {"El ID debe ser un número entero positivo"},
    {"El ID ingresado ya existe"},
    {"El Nombre no puede ser una cadena vacia"},
    {"El costo de preparación no puede ser negativo"},
    {"El precio de venta no puede ser negativo ni menor al costo de preparación"},
    {"El tiempo de preparación debe ser un valor positivo"},
    {"El ID del restaurante debe ser un entero positivo"},
    {"La comisión del restaurante es un número entero entre 0 y 100 %"},
    {"El ID de la categoría debe ser un número entero positivo"},
    {"No existe un plato con ese número de ID"},
    {"Registro dado de baja, contactese con el administrador"},
    {"No existen platos para ese restaurante"},
    {"No hay memoria suficiente para cargar el archivo"}
};

//MENSAJES DE NOTIFICACION AL USUARIO

const int PLATO_GUARDADO = 0;
const int PLATO_MODIFICADO = 1;
const int PLATO_ELIMINADO = 2;
const int PLATO_ENCONTRADO = 3;

const char SUCCES_PLATO[][80] = {
    {"El plato se guardó correctamente"},
    {"El plato se modificó correctamente"},
    {"El plato ha sido eliminado"},
    {"El registro ha sido encontrado"}
};

//PROTOTIPOS
void errorPlato(const int);
void mensajePlato(const int);
bool crearPlato();
bool modificarPlato();
int buscar_Plato_x_id(int);
void mostrarPlato(struct Platos);
void listarPlato_x_ID();
void listarTodosPlatos();
void listarPlatos_x_Rest();
bool eliminarPlato();
int contarPlatosActivos(struct Platos *, int);
struct Platos obtenerPlato_x_Pos(int);
bool copiarFicheroPlatos(const char *, const char *);

//DEFINICIONES
bool crearPlato(){
    /** Crear un nuevo registro de struct Platos y lo graba en el fichero
        ARGUMENTOS: void.
        RETORNO: bool - True si se completó la operación con éxito*/
    int pos;
    struct Platos nuevoPlato;
    FILE *pArchivo;

    cls();
    showcursor();

    cout << "NUEVO PLATO" << endl;
    cout << "------------" << endl;
    cout << "ID: " ;
    cin >> nuevoPlato.ID_Plato;

    if (nuevoPlato.ID_Plato <= 0){
        errorPlato(ID_ERROR);
        return false;
    }

    pos = buscar_Plato_x_id(nuevoPlato.ID_Plato);

    if (pos >= 0){
        errorPlato(ID_REPETIDO);
        return false;
    }

    cout << "Nombre: ";
    cin.ignore();
    cin.getline(nuevoPlato.nombre, 50);
    if (strcmp(nuevoPlato.nombre, "") == 0){
        errorPlato(NAME_ERROR);
        return false;
    }

    cout << "Costo preparación: $" ;
    cin >> nuevoPlato.costo;
    if (nuevoPlato.costo < 0){
        errorPlato(COST_ERROR);
        return false;
    }

    cout << "Precio de Venta: $" ;
    cin >> nuevoPlato.precio;
    if (nuevoPlato.precio < 0 || nuevoPlato.precio < nuevoPlato.costo){
        errorPlato(PRICE_ERROR);
        return false;
    }
    cout << "Tiempo de preparación (min.): " ;
    cin >> nuevoPlato.tiempo;
    if (nuevoPlato.tiempo < 0){
        errorPlato(TIME_ERROR);
        return false;
    }
    cout << "ID Restaurente: " ;
    cin >> nuevoPlato.ID_Res;
    if (nuevoPlato.ID_Res < 0) {
        errorPlato(ID_REST_ERROR);
        return false;
    }
    cout << "Comisión (%): " ;
    cin >> nuevoPlato.comRes;
    if (nuevoPlato.comRes < 0 || nuevoPlato.comRes > 100){
        errorPlato(COM_RES_ERROR);
        return false;
    }
    cout << "ID Categoría: " ;
    cin >> nuevoPlato.ID_Cat;
    if (nuevoPlato.ID_Cat < 0){
        errorPlato(ID_CAT_ERROR);
        return false;
    }

    nuevoPlato.estado = true;

    pArchivo = fopen(ARCHIVO_PLATOS, "ab");
    if (pArchivo == NULL){
        errorFile(ARCHIVO_PLATOS, FILE_OPEN_ERROR);
        return false;
    }

    if (fwrite(&nuevoPlato, sizeof nuevoPlato, 1, pArchivo) == 0){
        errorFile(ARCHIVO_PLATOS, FILE_WRITE_ERROR);
        return false;
    }
    fclose(pArchivo);
    return true; //Todo OK
}

bool modificarPlato(){
    /** modifica un registro del fichero.
        ARGUMENTOS: void.
        RETORNO: bool - True si se completó la operación con éxito*/
    int id, pos, tiempo;
    float precio;
    struct Platos plato;
    FILE *pArchivo;

    cls();
    showcursor();

    cout << "MODIFICAR PLATO" << endl;
    cout << "---------------" << endl;

    cout << "ID: " ;
    cin >> id;

    if (id <= 0){
        errorPlato(ID_ERROR);
        return false;
    }

    pos = buscar_Plato_x_id(id);
    if (pos == -1){
        errorPlato(ID_INEXISTENTE);
        return false;
    }

    plato = obtenerPlato_x_Pos(pos);

    if (plato.ID_Plato == -1) return false;

    if (plato.estado == false){
        errorPlato(PLATO_INACTIVO_ERROR);
        return false;
    }

    mostrarPlato(plato);

    cout << endl << endl << "NUEVO precio de Venta: $" ;
    cin >> precio;
    if (precio < 0 || precio < plato.costo){
        errorPlato(PRICE_ERROR);
        return false;
    }

    cout << endl << "NUEVO tiempo de preparación (min.): " ;
    cin >> tiempo;
    if (tiempo < 0){
        errorPlato(TIME_ERROR);
        return false;
    }

    plato.precio = precio;
    plato.tiempo = tiempo;

    pArchivo = fopen(ARCHIVO_PLATOS, "rb+");
    if (pArchivo == NULL){
        errorFile(ARCHIVO_PLATOS, FILE_OPEN_ERROR);
        return false;
    }

    fseek(pArchivo, pos*(sizeof plato), 0);
    if (fwrite(&plato, sizeof plato, 1, pArchivo) == 0){
        errorFile(ARCHIVO_PLATOS, FILE_WRITE_ERROR);
        fclose(pArchivo);
        return false;
    }

    fclose(pArchivo);
    return true; //Todo OK
}

void listarPlato_x_ID(){
    /** Solicita al usuario ingresar un ID, si existe dentro del fichero, lo lista por pantalla.
        ARGUMENTOS: void.
        RETORNO: void */
    int id, pos;
    struct Platos plato;

    cls();
    showcursor();

    cout << "LISTAR PLATO" << endl;
    cout << "------------" << endl;
    cout << "ID: " ;
    cin >> id;

    pos = buscar_Plato_x_id(id);
    if (pos == -1){
        errorPlato(ID_INEXISTENTE);
        return;
    }

    plato = obtenerPlato_x_Pos(pos);

    if (plato.ID_Plato == -1) return;

    if (plato.estado == false){
        errorPlato(PLATO_INACTIVO_ERROR);
        return;
    }
    hidecursor();
    mostrarPlato(plato);
    getch();
}

void listarTodosPlatos(){
    /** Lista todos los registros del fichero, los cuales se pueden recorrer en pantalla.
        ARGUMENTOS: void.
        RETORNO: void */
    FILE *pArchivo;
    struct Platos *vPlatos;
    int cantReg, pos = 0, indDir = 1, nReg = 1, teclaPulsada, plActivos;

    cantReg = contarRegistros(ARCHIVO_PLATOS, sizeof(struct Platos));
    if (cantReg > 0){
        vPlatos = (Platos *) malloc(cantReg * sizeof(Platos));
        if (vPlatos == NULL){
            errorFile(ARCHIVO_PLATOS, MEM_INSUF_ERROR);
            return;
        }

        pArchivo = fopen(ARCHIVO_PLATOS, "rb");
        if (pArchivo == NULL){
            errorFile(ARCHIVO_PLATOS, FILE_OPEN_ERROR);
            free(vPlatos); /**LIBERAR MEMORIA*/
            return;
        }

        fread(&vPlatos[0], sizeof (Platos), cantReg, pArchivo);
        fclose(pArchivo);
    }

    plActivos = contarPlatosActivos(vPlatos, cantReg); // Para usarlo en el contador de registros

    while (true){
        if (vPlatos[pos].estado == true) {
            cls();
            cout << "PLATOS REGISTRADOS" << endl;
            cout << "------------------" << endl;

            mostrarPlato(vPlatos[pos]);

            cout << endl << endl << "<--          " << nReg << " de " << plActivos <<"              -->";
            cout << endl << endl << "            Esc: Salir            ";

            teclaPulsada = getkey();

            switch (teclaPulsada){
                case KEY_ESCAPE:
                    free(vPlatos); /**Liberamos la memoria**/
                    return;
                break;
                case KEY_LEFT:
                    if (pos > 0){
                        indDir = -1;
                        pos--;
                        nReg--;
                    }
                break;
                case KEY_RIGHT:
                    if (pos < cantReg-1){
                        indDir = 1;
                        pos++;
                        nReg++;
                    }
                break;
            }
        } else{
            if (pos > 0 && pos < cantReg) pos += indDir; //Avanzamos al siguiente registro
            else pos = pos + indDir*-1; //Invertimos el indicador de dirección para seguir en el ultimo registro activo.
        }
    }
}
void listarPlatos_x_Rest(){
    /** Solicita al usuario el ingreso de un ID de restaurante y lista todos los platos asociados a dicho ID.
        ARGUMENTOS: void.
        RETORNO: void */
    int idR, c=0;
    struct Platos plato;
    FILE *pArchivo;

    cls();
    showcursor();

    cout << "PLATOS POR RESTAURANTE" << endl;
    cout << "----------------------" << endl;

    cout << "ID del Restaurante: " ;
    cin >> idR;

        if (idR <= 0){
        errorPlato(ID_REST_ERROR);
        return;
    }

    pArchivo = fopen(ARCHIVO_PLATOS, "rb");
    if (pArchivo == NULL){
        errorFile(ARCHIVO_PLATOS, FILE_OPEN_ERROR);
        return;
    }

    while (fread(&plato, sizeof plato, 1, pArchivo)){
        if (plato.estado == true){
            if (plato.ID_Res == idR){
                c++;
                mostrarPlato(plato);
                cout << endl ;
            }
        }
    }
    if (c == 0) errorPlato(ID_REST_SIN_PLATOS);
    else{
        hidecursor();
        setBackgroundColor(GREEN);
        cout << endl << c << " platos encontrados para el ID Restaurante: " << idR;
        resetColor();
        getch();
    }
    fclose(pArchivo);
}
struct Platos obtenerPlato_x_Pos(int pos){
    /** Extrae del fichero los datos de un registro y lo guarda en memoria para manipularlo.
        ARGUMENTOS: int pos -->La posición del registro dentro del fichero.
        RETORNO: struct Platos --> El plato buscado */
    struct Platos plato;
    FILE *pArchivo;

    pArchivo = fopen(ARCHIVO_PLATOS, "rb+");
    if (pArchivo == NULL){
        errorFile(ARCHIVO_PLATOS, FILE_OPEN_ERROR);
        plato.ID_Plato = -1; //Un plato Nulo
        return plato;
    }

    fseek(pArchivo, pos*(sizeof plato), 0);
    if (fread(&plato, sizeof plato, 1, pArchivo) == 0){
        errorFile(ARCHIVO_PLATOS, FILE_READ_ERROR);
        fclose(pArchivo);
        plato.ID_Plato = -1; //Un plato Nulo
        return plato;
    }
    fclose (pArchivo);
    return plato;
}
bool eliminarPlato(){
    /** BAJA LOGICA de un registro.
        ARGUMENTOS: void.
        RETORNO: bool --> true si se completó la operación con éxito */
    int id, pos;
    struct Platos plato;
    FILE *pArchivo;

    cls();
    showcursor();

    cout << "ELIMINAR PLATO" << endl;
    cout << "---------------" << endl;

    cout << "ID: " ;
    cin >> id;

    if (id <= 0){
        errorPlato(ID_ERROR);
        return false;
    }

    pos = buscar_Plato_x_id(id);
    if (pos == -1){
        errorPlato(ID_INEXISTENTE);
        return false;
    }

    plato = obtenerPlato_x_Pos(pos);

    if (plato.ID_Plato == -1) return false;

    if (plato.estado == false){
        errorPlato(PLATO_INACTIVO_ERROR);
        return false;
    }

    plato.estado = false; //BAJA LOGICA

    pArchivo = fopen(ARCHIVO_PLATOS, "rb+");
    if (pArchivo == NULL){
        errorFile(ARCHIVO_PLATOS, FILE_OPEN_ERROR);
        plato.ID_Plato = -1; //Un plato Nulo
        return false;
    }

    fseek(pArchivo, pos*(sizeof plato), 0);
    if (fwrite(&plato, sizeof plato, 1, pArchivo) == 0){
        errorFile(ARCHIVO_PLATOS, FILE_WRITE_ERROR);
        fclose(pArchivo);
        return false;
    }

    fclose(pArchivo);
    return true; //Todo OK
}
void mostrarPlato(struct Platos plato){
    /** Muestra un registro por pantalla.
        ARGUMENTOS: el registro a mostrar.
        RETORNO: void */
    cout << endl << "ID: " << plato.ID_Plato ;
    cout << endl << "Nombre: " << plato.nombre ;
    cout << endl << "Costo preparación: $" << plato.costo ;
    cout << endl << "Precio de Venta: $" << plato.precio ;
    cout << endl << "Tiempo de preparación (min.): " << plato.tiempo ;
    cout << endl << "ID Restaurente: " << plato.ID_Res;
    cout << endl << "Comisión (%): " << plato.comRes;
    cout << endl << "ID Categoría: " << plato.ID_Cat;
}
int buscar_Plato_x_id(int id){
    /** Buscar un registro del fichero por el campo 'ID'.
        ARGUMENTOS: int id --> un entero con el ID.
        RETORNO: Devuelve 1 si el registro existe, -1 si no existe, -2 si hubo algún error al manipular el fichero */
    FILE *pArchivo;
    struct Platos plato;
    int pos;

    pArchivo = fopen(ARCHIVO_PLATOS, "rb");
    if (pArchivo == NULL){
        errorFile(ARCHIVO_PLATOS, FILE_OPEN_ERROR);
        return -2; // Usamos -2 para error de Archivo y -1 si no existe
    }

    while (fread(&plato, sizeof plato, 1, pArchivo) != 0){
        if (plato.ID_Plato == id){
            fclose(pArchivo);
            return pos;
        }
        pos++;
    }
    fclose(pArchivo);
    return -1; // id no encontrado
}

int contarPlatosActivos(struct Platos *vPlatos, int tam){
    /** Contar registros, solo con estado activo.
        ARGUMENTOS: un vector con todos los registros del fichero, int tam --> La cantidad de registros del fichero.
        RETORNO: Un valor entero con la cantidad de registros con estado activo */
    int i, cont = 0;

    for (i=0;i<tam;i++){
        if (vPlatos[i].estado == true){
            cont++;
        }
    }
    return cont;
}

void errorPlato(const int nError){
    /** Muestra por pantalla un mensaje de error.
        ARGUMENTOS: int nError --> El número de error.
        RETORNO: void */
    setBackgroundColor(RED);
    hidecursor();
    cout << endl << ERROR_PLATO[nError];
    resetColor();
    getch();
}
void mensajePlato(const int nMensaje){
    /** Muestra por pantalla un mensaje informativo.
        ARGUMENTOS: int nMensaje --> El número de mensaje.
        RETORNO: void */
    setBackgroundColor(GREEN);
    hidecursor();
    cout << endl << SUCCES_PLATO[nMensaje];
    resetColor();
    getch();
}
bool copiarFicheroPlatos(const char *srcFile, const char *dstFile){
    /** Realiza una copia del archivo *srcFile en memoria y luego lo guarda como *dstFile en Disco.
        ARGUMENTOS: const char *srcFile --> El fichero fuente.
                    const char *dstFile --> El fichero destino. Si existe lo reemplaza, y si no, lo crea.
        RETORNO: bool --> TRUE si la operación se completó con éxito */
    struct Platos *vPlatos;
    int cantReg;
    FILE *pArchivo;

    cantReg = contarRegistros(srcFile, sizeof (Platos));
    if (cantReg > 0){
        vPlatos = (Platos *) malloc(cantReg * sizeof (Platos));
        if (vPlatos == NULL){
            errorFile(srcFile, MEM_INSUF_ERROR);
            return false;
        }

        pArchivo = fopen(srcFile, "rb");
        if (pArchivo == NULL){
            errorFile(srcFile, FILE_OPEN_ERROR);
            free(vPlatos);
            return false;
        }
        fread(&vPlatos[0], sizeof (Platos), cantReg, pArchivo);
        fclose(pArchivo);

        pArchivo = fopen(dstFile, "wb");
        if (pArchivo == NULL){
            errorFile(dstFile, FILE_OPEN_ERROR);
            free(vPlatos);
            return false;
        }
        fwrite(&vPlatos[0], sizeof (Platos), cantReg, pArchivo);
        fclose(pArchivo);
    }
    else {
        errorFile(srcFile, ARCHIVO_VACIO_ERROR);
        return false;
    }
    free(vPlatos);
    return true;
}

#endif // PLATOS_H_INCLUDED
