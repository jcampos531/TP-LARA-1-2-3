#ifndef PEDIDOS_H_INCLUDED
#define PEDIDOS_H_INCLUDED
struct Pedidos{
    int ID, IDCliente, IDPlato, cantidad, estado;
    float precio, valoracion;
    Fecha fechaPedido;
};

//ERRORES DE EDICION/CREACION DE PEDIDOS
const int CANT_PEDIDO_ERROR = 0;
const int FECHA_PEDIDO_ERROR = 1;
const int VALORACION_PEDIDO_ERROR = 2;
const int ID_PEDIDO_INEXISTENTE = 3;
const int ESTADO_PEDIDO_ERROR = 4;

const char  ERROR_PEDIDO[][80] = {
    {"La cantidad del pedido debe ser mayor a cero"},
    {"La fecha del pedido ingresada no es válida"},
    {"La valoración del pedido debe ser un número real entre 0 y 10"},
    {"No existe un pedido con ese número de ID"},
    {"El estado del pedido es: 1-En curso, 2-Completado, 3-Cancelado."}
};

//MENSAJES DE NOTIFICACION AL USUARIO
const int PEDIDO_GUARDADO = 0;
const int PEDIDO_MODIFICADO = 1;
const int PEDIDO_ELIMINADO = 2;

const char SUCCES_PEDIDO[][80] = {
    {"El pedido se guardó correctamente"},
    {"El pedido ha sido modificado correctamente"},
    {"El pedido ha sido eliminado"}
};

//PROTOTIPOS
bool crearPedido();
bool modificarPedido();
void listarPedido_x_ID();
void listarTodosPedidos();
int buscar_Pedido_x_id(int);
struct Pedidos obtenerPedido_x_Pos(int);
void mostrarPedido(struct Pedidos);
bool verificarAnioPedido(int);
void errorPedido(const int);
bool copiarFicheroPedidos(const char *, const char *);
//DEFINICIONES
bool crearPedido(){
    /** Crea un nuevo registro de struct Pedidos y lo graba en el fichero.
        ARGUMENTOS: void.
        RETORNO: bool - True si se completó la operación con éxito*/
    int ultReg, dia, mes, anio, pos;
    struct Pedidos pedido;
    struct Clientes cliente;
    struct Platos plato;
    FILE *pArchivo;

    cls();
    showcursor();

    ultReg = contarRegistros(ARCHIVO_PEDIDOS, sizeof (Pedidos));

    cout << "NUEVO PEDIDO" << endl;
    cout << "------------" << endl;
    cout << "ID: " << ultReg + 1 << endl;
    pedido.ID = ultReg + 1;

    cout << "ID del cliente: ";
    cin >> pedido.IDCliente;
    pos = buscar_Cliente_x_id(pedido.IDCliente);
    if (pos == -1){
        errorCliente(ID_CLIENTE_INEXISTENTE);
        return false;
    }
    cliente = obtenerCliente_x_Pos(pos);

    if (cliente.ID == -1) return false;

    if (cliente.estado == false){
        errorCliente(CLIENTE_INACTIVO_ERROR);
        return false;
    }
    cout << "---> Cliente: " << cliente.apellidos << ", " << cliente.nombres ;

    cout << endl << "ID de Plato: ";
    cin >> pedido.IDPlato;
    pos = buscar_Plato_x_id(pedido.IDPlato);
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
    cout << "---> Plato: " << plato.nombre << endl;
    cout << "---> Precio Unitario: $" << plato.precio << endl;

    cout << endl << "Cantidad: ";
    cin >> pedido.cantidad;
    if (pedido.cantidad < 0){
        errorPedido(CANT_PEDIDO_ERROR);
        return false;
    }

    pedido.precio = plato.precio;

    cout << "Fecha: " << endl;
    cout << "\t" << "Dia (dd): ";
    cin >> dia;
    cout << "\t" << "Mes (mm): ";
    cin >> mes;
    cout << "\t" << "Año (aaaa): ";
    cin >> anio;

    if (!verificarAnioPedido(anio) || !verificarFecha(dia, mes, anio)){
        errorPedido(FECHA_PEDIDO_ERROR);
        return false;
    }
    pedido.fechaPedido.dia = dia;
    pedido.fechaPedido.mes = mes;
    pedido.fechaPedido.anio = anio;

    cout << endl << "Valoración: ";
    cin >> pedido.valoracion;
    if (pedido.valoracion > 10 || pedido.valoracion < 0){
        errorPedido(VALORACION_PEDIDO_ERROR);
        return false;
    }

    pedido.estado = 1; //PEDIDO EN CURSO

    pArchivo = fopen(ARCHIVO_PEDIDOS, "ab");
    if (pArchivo == NULL){
        errorFile(ARCHIVO_PEDIDOS, FILE_OPEN_ERROR);
        return false;
    }

    if (fwrite(&pedido, sizeof pedido, 1, pArchivo) == 0){
        errorFile(ARCHIVO_PEDIDOS, FILE_WRITE_ERROR);
        return false;
    }
    fclose(pArchivo);
    return true; //Todo OK
}

bool modificarPedido(){
    /** modifica un registro del fichero.
        ARGUMENTOS: void.
        RETORNO: bool - True si se completó la operación con éxito*/
    int id, pos;
    struct Pedidos pedido;
    FILE *pArchivo;

    cls();
    showcursor();

    cout << "MODIFICAR PEDIDO" << endl;
    cout << "-----------------" << endl;
    cout << "ID: " ;
    cin >> id;

    pos = buscar_Pedido_x_id(id);
    if (pos == -1){
        errorPedido(ID_PEDIDO_INEXISTENTE);
        return false;
    }

    pedido = obtenerPedido_x_Pos(pos);

    if (pedido.ID == -1) return false;

    mostrarPedido(pedido);

    cout << endl << endl << "Modificar estado del pedido: ";
    cin >> pedido.estado ;
    if (pedido.estado != 1 && pedido.estado != 2 && pedido.estado != 3){
        errorPedido(ESTADO_PEDIDO_ERROR);
        return false;
    }

    pArchivo = fopen(ARCHIVO_PEDIDOS, "rb+");
    if (pArchivo == NULL){
        errorFile(ARCHIVO_PEDIDOS, FILE_OPEN_ERROR);
        return false;
    }

    fseek(pArchivo, pos * sizeof(pedido), 0);
    fwrite(&pedido, sizeof pedido, 1, pArchivo);
    fclose(pArchivo);

    return true;
}

void listarPedido_x_ID(){
    /** Solicita al usuario ingresar un ID, si existe dentro del fichero, lo lista por pantalla.
        ARGUMENTOS: void.
        RETORNO: void */
    int id, pos;
    struct Pedidos pedido;

    cls();
    showcursor();

    cout << "LISTAR PEDIDO" << endl;
    cout << "--------------" << endl;
    cout << "ID: " ;
    cin >> id;

    pos = buscar_Pedido_x_id(id);
    if (pos == -1){
        errorPedido(ID_PEDIDO_INEXISTENTE);
        return;
    }

    pedido = obtenerPedido_x_Pos(pos);

    if (pedido.ID == -1) return;

    hidecursor();
    mostrarPedido(pedido);
    getch();
}

void listarTodosPedidos(){
    /** Lista todos los registros del fichero, los cuales se pueden recorrer en pantalla.
        ARGUMENTOS: void.
        RETORNO: void */
    FILE *pArchivo;
    struct Pedidos *vPedidos;
    int cantReg, pos = 0, teclaPulsada;

    cantReg = contarRegistros(ARCHIVO_PEDIDOS, sizeof(struct Pedidos));
    if (cantReg > 0){
        vPedidos = (Pedidos *) malloc(cantReg * sizeof(Pedidos));
        if (vPedidos == NULL){
            errorFile(ARCHIVO_PEDIDOS, MEM_INSUF_ERROR);
            return;
        }

        pArchivo = fopen(ARCHIVO_PEDIDOS, "rb");
        if (pArchivo == NULL){
            errorFile(ARCHIVO_PEDIDOS, FILE_OPEN_ERROR);
            free(vPedidos); /** LIBERAR MEMORIA */
            return;
        }

        fread(&vPedidos[0], sizeof (Pedidos), cantReg, pArchivo);
        fclose(pArchivo);
    }

    while (true){
        cls();
        cout << "PEDIDOS REGISTRADOS" << endl;
        cout << "--------------------" << endl;

        mostrarPedido(vPedidos[pos]);

        cout << endl << endl << "<--          " << pos + 1 << " de " << cantReg <<"              -->";
        cout << endl << endl << "            Esc: Salir            ";

        teclaPulsada = getkey();

        switch (teclaPulsada){
            case KEY_ESCAPE:
                free(vPedidos); /**Liberamos la memoria**/
                return;
            break;
            case KEY_LEFT:
                if (pos > 0) pos--;
            break;
            case KEY_RIGHT:
                if (pos < cantReg-1) pos++;
            break;
        }
    }
}

int buscar_Pedido_x_id(int id){
    /** Buscar un registro del fichero por el campo 'ID'.
        ARGUMENTOS: int id --> un entero con el ID.
        RETORNO: Devuelve 1 si el registro existe, -1 si no existe, -2 si hubo algún error al manipular el fichero */
    FILE *pArchivo;
    struct Pedidos pedido;
    int pos;

    pArchivo = fopen(ARCHIVO_PEDIDOS, "rb");
    if (pArchivo == NULL){
        errorFile(ARCHIVO_PEDIDOS, FILE_OPEN_ERROR);
        return -2; // Usamos -2 para error de Archivo y -1 si no existe
    }

    while (fread(&pedido, sizeof pedido, 1, pArchivo) != 0){
        if (pedido.ID == id){
            fclose(pArchivo);
            return pos;
        }
        pos++;
    }
    fclose(pArchivo);
    return -1; // id no encontrado
}

struct Pedidos obtenerPedido_x_Pos(int pos){
    /** Extrae del fichero los datos de un registro y lo guarda en memoria para manipularlo.
        ARGUMENTOS: int pos -->La posición del registro dentro del fichero.
        RETORNO: struct Platos --> El plato buscado */
    struct Pedidos pedido;
    FILE *pArchivo;

    pArchivo = fopen(ARCHIVO_PEDIDOS, "rb");
    if (pArchivo == NULL){
        errorFile(ARCHIVO_PEDIDOS, FILE_OPEN_ERROR);
        pedido.ID = -1; //Un pedido Nulo
        return pedido;
    }

    fseek(pArchivo, pos*(sizeof pedido), 0);
    if (fread(&pedido, sizeof pedido, 1, pArchivo) == 0){
        errorFile(ARCHIVO_PEDIDOS, FILE_READ_ERROR);
        fclose(pArchivo);
        pedido.ID = -1; //Un pedido Nulo
        return pedido;
    }

    fclose(pArchivo);
    return pedido;
}

void mostrarPedido(struct Pedidos pedido){
    /** Muestra un registro por pantalla.
        ARGUMENTOS: el registro a mostrar.
        RETORNO: void */
    cout << endl << "ID Pedido: " << pedido.ID ;
    cout << endl << "ID Cliente: " << pedido.IDCliente ;
    cout << endl << "ID Plato: " << pedido.IDPlato ;
    cout << endl << "Cantidad: " << pedido.cantidad ;
    cout << endl << "Precio unitario: $" << pedido.precio ;
    cout << endl << "Fecha Pedido: " << pedido.fechaPedido.dia << "/" << pedido.fechaPedido.mes << "/" << pedido.fechaPedido.anio << endl;
    cout << endl << "Valoración: " << pedido.valoracion ;
    cout << endl << "Estado: " << pedido.estado ;
}

bool verificarAnioPedido(int anio){
    /** Verifica que el pedido que se está cargando corresponde al año en curso.
        ARGUMENTOS: un entero con el año de la fecha del pedido.
        RETORNO: bool */
    time_t fechaActual;
    struct tm *tmPtr;

    fechaActual = time(NULL); // FECHA del sistema
    tmPtr = localtime(&fechaActual);

    if (anio != (tmPtr->tm_year + 1900)) return false; /** Los pedidos serán dentro del año en curso */

    return true;
}

void errorPedido(const int nError){
    /** Muestra por pantalla un mensaje de error.
        ARGUMENTOS: int nError --> El número de error.
        RETORNO: void */
    setBackgroundColor(RED);
    hidecursor();
    cout << endl << ERROR_PEDIDO[nError];
    resetColor();
    getch();
}

void mensajePedido(const int nMensaje){
    /** Muestra por pantalla un mensaje informativo.
        ARGUMENTOS: int nMensaje --> El número de mensaje.
        RETORNO: void */
    setBackgroundColor(GREEN);
    hidecursor();
    cout << endl << SUCCES_PEDIDO[nMensaje];
    resetColor();
    getch();
}

bool copiarFicheroPedidos(const char *srcFile, const char *dstFile){
    /** Realiza una copia del archivo *srcFile en memoria y luego lo guarda como *dstFile en Disco.
        ARGUMENTOS: const char *srcFile --> El fichero fuente.
                    const char *dstFile --> El fichero destino. Si existe lo reemplaza, y si no, lo crea.
        RETORNO: bool --> TRUE si la operación se completó con éxito */
    struct Pedidos *vPedidos;
    int cantReg;
    FILE *pArchivo;

    cantReg = contarRegistros(srcFile, sizeof (Pedidos));
    if (cantReg > 0){
        vPedidos = (Pedidos *) malloc(cantReg * sizeof (Pedidos));
        if (vPedidos == NULL){
            errorFile(srcFile, MEM_INSUF_ERROR);
            return false;
        }

        pArchivo = fopen(srcFile, "rb");
        if (pArchivo == NULL){
            errorFile(srcFile, FILE_OPEN_ERROR);
            free(vPedidos);
            return false;
        }
        fread(&vPedidos[0], sizeof (Pedidos), cantReg, pArchivo);
        fclose(pArchivo);

        pArchivo = fopen(dstFile, "wb");
        if (pArchivo == NULL){
            errorFile(dstFile, FILE_OPEN_ERROR);
            free(vPedidos);
            return false;
        }
        fwrite(&vPedidos[0], sizeof (Pedidos), cantReg, pArchivo);
        fclose(pArchivo);
    }
    else {
        errorFile(srcFile, ARCHIVO_VACIO_ERROR);
        return false;
    }
    free(vPedidos);
    return true;
}
#endif // PEDIDOS_H_INCLUDED
