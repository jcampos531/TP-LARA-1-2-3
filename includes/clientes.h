#ifndef CLIENTES_H_INCLUDED
#define CLIENTES_H_INCLUDED

struct Clientes{
    int ID, CP;
    char apellidos[50], nombres[50], mail[50], domicilio[100];
    struct Fecha fecha_nac;
    bool estado;
};

//ERRORES DE EDICION/CREACION DE CLIENTES
const int NOM_CLIENT_ERROR = 0;
const int APE_CLIENT_ERROR = 1;
const int MAIL_CLIENT_ERROR = 2;
const int DOMIC_CLIENT_ERROR = 3;
const int CP_CLIENT_ERROR = 4;
const int FECHA_NAC_CLIENT_ERROR = 5;
const int ID_CLIENTE_INEXISTENTE = 6;
const int CLIENTE_INACTIVO_ERROR = 7;

const char  ERROR_CLIENTE[][80] = {
    {"El campo 'nombres' no puede ser una cadena vacía"},
    {"El campo 'apellidos' no puede ser una cadena vacía"},
    {"El campo 'mail' no es válido"},
    {"El campo 'domicilio' no puede ser una cadena vacía"},
    {"El campo 'CP' debe ser un número entre 1000 y 9999"},
    {"La fecha ingresada no es válida"},
    {"El id ingresado no existe en la base de clientes"},
    {"El cliente no está activo, comuniquese con el administrador"}
};

//MENSAJES DE NOTIFICACION AL USUARIO
const int CLIENTE_GUARDADO = 0;
const int CLIENTE_MODIFICADO = 1;
const int CLIENTE_ELIMINADO = 2;

const char SUCCES_CLIENTE[][80] = {
    {"El cliente se guardó correctamente"},
    {"El cliente ha sido modificado correctamente"},
    {"El cliente ha sido eliminado"}
};

//PROTOTIPOS
bool crearCliente();
bool modificarCliente();
void errorCliente(const int);
void mensajeCliente(const int);
bool verificaMail(char *);
bool verificarEdad(int);
void mostrarCliente(struct Clientes);
void listarCliente_x_ID();
void listarTodosClientes();
bool eliminarCliente();
int buscar_Cliente_x_id(int);
struct Clientes obtenerCliente_x_Pos(int);
int contarClientesActivos(struct Clientes *, int);
void ordenar_x_Apellido(struct Clientes *, int);
bool copiarFicheroClientes(const char *, const char *);

//DEFINICIONES
bool crearCliente(){
    /** Crea un nuevo registro de struct Clientes y lo graba en el fichero
        ARGUMENTOS: void.
        RETORNO: bool - True si se completó la operación con éxito*/
    int ultReg, dia, mes, anio;
    struct Clientes cliente;
    FILE *pArchivo;

    cls();
    showcursor();

    ultReg = contarRegistros(ARCHIVO_CLIENTES, sizeof (Clientes));

    cout << "NUEVO CLIENTE" << endl;
    cout << "-------------" << endl;
    cout << "ID: " << ultReg + 1 << endl;
    cliente.ID = ultReg + 1;
    cout << "Nombres: " ;

    fflush( stdin );
    cin.getline(cliente.nombres, 50);

    if (strcmp(cliente.nombres, "") == 0){
        errorCliente(NOM_CLIENT_ERROR);
        return false;
    }
    cout << "Apellidos: " ;
    cin.getline(cliente.apellidos, 50);
    if (strcmp(cliente.apellidos, "") == 0){
        errorCliente(APE_CLIENT_ERROR);
        return false;
    }
    cout << "Mail: " ;
    cin >> cliente.mail;
    if (!verificaMail(cliente.mail)){
        errorCliente(MAIL_CLIENT_ERROR);
        return false;
    }
    cout << "Domicilio: " ;
    cin.ignore();
    cin.getline(cliente.domicilio, 100);
    if (strcmp(cliente.domicilio, "") == 0){
        errorCliente(DOMIC_CLIENT_ERROR);
        return false;
    }
    cout << "CP: ";
    cin >> cliente.CP ;
    if (cliente.CP > 9999 || cliente.CP < 1000){
        errorCliente(CP_CLIENT_ERROR);
        return false;
    }
    cout << "Fecha Nac.: " << endl;
    cout << "\t" << "Dia (dd): ";
    cin >> dia;
    cout << "\t" << "Mes (mm): ";
    cin >> mes;
        cout << "\t" << "Año (aaaa): ";
    cin >> anio;
    if (!verificarEdad(anio) || !verificarFecha(dia, mes, anio)){
        errorCliente(FECHA_NAC_CLIENT_ERROR);
        return false;
    }

    cliente.fecha_nac.dia = dia;
    cliente.fecha_nac.mes = mes;
    cliente.fecha_nac.anio = anio;

    cliente.estado = true;

    pArchivo = fopen(ARCHIVO_CLIENTES, "ab");
    if (pArchivo == NULL){
        errorFile(ARCHIVO_CLIENTES, FILE_OPEN_ERROR);
        return false; // Usamos -2 para error de Archivo y -1 si no existe
    }
    fwrite(&cliente, sizeof cliente, 1, pArchivo);
    fclose(pArchivo);

    return true;
}

bool modificarCliente(){
    /** modifica un registro del fichero.
        ARGUMENTOS: void.
        RETORNO: bool - True si se completó la operación con éxito*/
    int id, pos;
    struct Clientes cliente;
    FILE *pArchivo;

    cls();
    showcursor();

    cout << "MODIFICAR CLIENTE" << endl;
    cout << "-----------------" << endl;
    cout << "ID: " ;
    cin >> id;

    pos = buscar_Cliente_x_id(id);
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

    mostrarCliente(cliente);
    cout << endl << "Ingrese nuevo 'domicilio': ";
    cin.ignore();
    cin.getline(cliente.domicilio, 100);
    if (strcmp(cliente.domicilio, "") == 0){
        errorCliente(DOMIC_CLIENT_ERROR);
        return false;
    }

    pArchivo = fopen(ARCHIVO_CLIENTES, "rb+");
    if (pArchivo == NULL){
        errorFile(ARCHIVO_CLIENTES, FILE_OPEN_ERROR);
        return false; // Usamos -2 para error de Archivo y -1 si no existe
    }

    fseek(pArchivo, pos * sizeof(cliente), 0);
    fwrite(&cliente, sizeof cliente, 1, pArchivo);
    fclose(pArchivo);

    return true;
}

void listarCliente_x_ID(){
    /** Solicita al usuario ingresar un ID, si existe dentro del fichero, lo lista por pantalla.
        ARGUMENTOS: void.
        RETORNO: void */
    int id, pos;
    struct Clientes cliente;

    cls();
    showcursor();

    cout << "LISTAR CLIENTE" << endl;
    cout << "--------------" << endl;
    cout << "ID: " ;
    cin >> id;

    pos = buscar_Cliente_x_id(id);
    if (pos == -1){
        errorCliente(ID_CLIENTE_INEXISTENTE);
        return;
    }

    cliente = obtenerCliente_x_Pos(pos);

    if (cliente.ID == -1) return;

    if (cliente.estado == false){
        errorCliente(CLIENTE_INACTIVO_ERROR);
        return;
    }
    hidecursor();
    mostrarCliente(cliente);
    getch();
}

void listarTodosClientes(){
    /** Lista todos los registros del fichero, los cuales se pueden recorrer en pantalla.
        ARGUMENTOS: void.
        RETORNO: void */
    FILE *pArchivo;
    struct Clientes *vClientes;
    int cantReg, pos = 0, indDir = 1, nReg = 1, teclaPulsada, clActivos;

    cantReg = contarRegistros(ARCHIVO_CLIENTES, sizeof(struct Clientes));
    if (cantReg > 0){
        vClientes = (Clientes *) malloc(cantReg * sizeof(Clientes));
        if (vClientes == NULL){
            errorFile(ARCHIVO_CLIENTES, MEM_INSUF_ERROR);
            return;
        }

        pArchivo = fopen(ARCHIVO_CLIENTES, "rb");
        if (pArchivo == NULL){
            errorFile(ARCHIVO_CLIENTES, FILE_OPEN_ERROR);
            free(vClientes); /** LIBERAR MEMORIA */
            return;
        }

        fread(&vClientes[0], sizeof (Clientes), cantReg, pArchivo);
        fclose(pArchivo);
    }

    ordenar_x_Apellido(vClientes, cantReg);
    clActivos = contarClientesActivos(vClientes, cantReg); // Para usarlo en el contador de registros

    while (true){
        if (vClientes[pos].estado == true) {
            cls();
            cout << "CLIENTES REGISTRADOS" << endl;
            cout << "--------------------" << endl;

            mostrarCliente(vClientes[pos]);

            cout << endl << endl << "<--          " << nReg << " de " << clActivos <<"              -->";
            cout << endl << endl << "            Esc: Salir            ";

            teclaPulsada = getkey();

            switch (teclaPulsada){
                case KEY_ESCAPE:
                    free(vClientes); /**Liberamos la memoria**/
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

bool eliminarCliente(){
    /** BAJA LOGICA de un registro.
        ARGUMENTOS: void.
        RETORNO: bool --> true si se completó la operación con éxito */
    int id, pos;
    struct Clientes cliente;
    FILE *pArchivo;

    cls();
    showcursor();

    cout << "ELIMINAR CLIENTE" << endl;
    cout << "----------------" << endl;

    cout << "ID: " ;
    cin >> id;

    pos = buscar_Cliente_x_id(id);
    if (pos == -1){
        errorCliente(ID_CLIENTE_INEXISTENTE);
        return false;
    }

    cliente= obtenerCliente_x_Pos(pos);

    if (cliente.ID == -1) return false;

    if (cliente.estado == false){
        errorCliente(CLIENTE_INACTIVO_ERROR);
        return false;
    }

    cliente.estado = false; //BAJA LOGICA
    mostrarCliente (cliente);

    pArchivo = fopen(ARCHIVO_CLIENTES, "rb+");
    if (pArchivo == NULL){
        errorFile(ARCHIVO_CLIENTES, FILE_OPEN_ERROR);
        return false;
    }

    fseek(pArchivo, pos*(sizeof cliente), 0);

    if (fwrite(&cliente, sizeof cliente, 1, pArchivo) == 0){
        errorFile(ARCHIVO_CLIENTES, FILE_WRITE_ERROR);
        fclose(pArchivo);
        return false;
    }

    fclose(pArchivo);

    return true; //Todo OK
}
int buscar_Cliente_x_id(int id){
    /** Buscar un registro del fichero por el campo 'ID'.
        ARGUMENTOS: int id --> un entero con el ID.
        RETORNO: Devuelve 1 si el registro existe, -1 si no existe, -2 si hubo algún error al manipular el fichero */
    FILE *pArchivo;
    struct Clientes cliente;
    int pos;

    pArchivo = fopen(ARCHIVO_CLIENTES, "rb");
    if (pArchivo == NULL){
        errorFile(ARCHIVO_CLIENTES, FILE_OPEN_ERROR);
        return -2; // Usamos -2 para error de Archivo y -1 si no existe
    }

    while (fread(&cliente, sizeof cliente, 1, pArchivo) != 0){
        if (cliente.ID == id){
            fclose(pArchivo);
            return pos;
        }
        pos++;
    }
    fclose(pArchivo);
    return -1; // id no encontrado
}
struct Clientes obtenerCliente_x_Pos(int pos){
    /** Extrae del fichero los datos de un registro y lo guarda en memoria para manipularlo.
        ARGUMENTOS: int pos -->La posición del registro dentro del fichero.
        RETORNO: struct Platos --> El plato buscado */
    struct Clientes cliente;
    FILE *pArchivo;


    pArchivo = fopen(ARCHIVO_CLIENTES, "rb");
    if (pArchivo == NULL){
        errorFile(ARCHIVO_CLIENTES, FILE_OPEN_ERROR);
        cliente.ID = -1; //Un cliente Nulo
        return cliente;
    }

    fseek(pArchivo, pos*(sizeof cliente), 0);
    if (fread(&cliente, sizeof cliente, 1, pArchivo) == 0){
        errorFile(ARCHIVO_CLIENTES, FILE_READ_ERROR);
        fclose(pArchivo);
        cliente.ID = -1; //Un cliente Nulo
        return cliente;
    }

    fclose(pArchivo);
    return cliente;
}

void mostrarCliente(struct Clientes cliente){
    /** Muestra un registro por pantalla.
        ARGUMENTOS: el registro a mostrar.
        RETORNO: void */
    cout << endl << "ID: " << cliente.ID ;
    cout << endl << "Nombres: " << cliente.nombres ;
    cout << endl << "Apellidos: " << cliente.apellidos ;
    cout << endl << "Mail: " << cliente.mail ;
    cout << endl << "Domicilio: " << cliente.domicilio ;
    cout << endl << "CP: " << cliente.CP ;
    cout << endl << "Fecha Nac.: " << cliente.fecha_nac.dia << "/" << cliente.fecha_nac.mes << "/" << cliente.fecha_nac.anio << endl;
}
bool verificarEdad(int anio){
    /** Verifica que la edad del cliente este dentro de un valor determinado.
        ARGUMENTOS: un entero con el año de nacimiento.
        RETORNO: bool */
    time_t fechaActual;
    int edad;
    struct tm *tmPtr;

    fechaActual = time(NULL); // FECHA del sistema
    tmPtr = localtime(&fechaActual);

    edad = (tmPtr->tm_year)+ 1900 - anio;

    if (edad < 13 || edad > 120) return false; /** El cliente no será menor a 13 años de edad
                                                            y hasta 120 años de edad */
    return true;
}

bool verificaMail(char *mail){
    /** Verifica que la dirección de e-mail sea válida.
        ARGUMENTOS: char *mail --> la dirección de mail ingresada por el usuario.
        RETORNO: bool */
    int i = 1, cantPuntos = 0, cantAt = 0;
    if (mail[0] == '@' || mail[0] == '.' || (int)mail[0] == 0) return false;

    do {
        if (mail[i] == '.') {
            cantPuntos++;
            if (mail[i-1] == '.' || mail[i+1] == '.') return false;
        }
        if (mail[i] == '@'){
            cantAt++;
            if (!(isalnum(mail[i-1]) && isalnum(mail[i+1]) )) return false;
            if (cantAt > 1) return false; // no puede haber más de un '@'
        }
        i++;
    } while ((int)mail[i] != 0);

    if (cantPuntos < 1) return false; // Al menos debe haber un '.'
    if (mail[i-1] == '.') return false; // el último no debe ser un '.'


    return true;
}

int contarClientesActivos(struct Clientes *vClientes, int tam){
    /** Contar registros, solo con estado activo.
        ARGUMENTOS: un vector con todos los registros del fichero, int tam --> La cantidad de registros del fichero.
        RETORNO: Un valor entero con la cantidad de registros con estado activo */
    int i, cont = 0;

    for (i=0;i<tam;i++){
        if (vClientes[i].estado == true){
            cont++;
        }
    }
    return cont;
}
void ordenar_x_Apellido(struct Clientes *vClientes, int tam){
    /** Ordena por Apellido un vector en memoria que contiene todos los registros de Clientes del fichero.
        ARGUMENTOS: un vector con todos los registros del fichero, int tam --> La cantidad de registros del fichero.
        RETORNO: void */
    int posMin, i, j;
    struct Clientes aux;

    for (i=0;i<tam-1;i++){
        posMin = i;
        for (j=i+1;j<tam;j++){
            if(strcasecmp(vClientes[j].apellidos, vClientes[posMin].apellidos) < 0){
                posMin = j;
            }
        }
        if (posMin != i){
            aux = vClientes[i];
            vClientes[i] = vClientes[posMin];
            vClientes[posMin] = aux;
        }
    }
}

void errorCliente(const int nError){
    /** Muestra por pantalla un mensaje de error.
        ARGUMENTOS: int nError --> El número de error.
        RETORNO: void */
    setBackgroundColor(RED);
    hidecursor();
    cout << endl << ERROR_CLIENTE[nError];
    resetColor();
    getch();
}
void mensajeCliente(const int nMensaje){
    /** Muestra por pantalla un mensaje informativo.
        ARGUMENTOS: int nMensaje --> El número de mensaje.
        RETORNO: void */
    setBackgroundColor(GREEN);
    hidecursor();
    cout << endl << SUCCES_CLIENTE[nMensaje];
    resetColor();
    getch();
}

bool copiarFicheroClientes(const char *srcFile, const char *dstFile){
    /** Realiza una copia del archivo *srcFile en memoria y luego lo guarda como *dstFile en Disco.
        ARGUMENTOS: const char *srcFile --> El fichero fuente.
                    const char *dstFile --> El fichero destino. Si existe lo reemplaza, y si no, lo crea.
        RETORNO: bool --> TRUE si la operación se completó con éxito */
    struct Clientes *vClientes;
    int cantReg;
    FILE *pArchivo;

    cantReg = contarRegistros(srcFile, sizeof (Clientes));
    if (cantReg > 0){
        vClientes = (Clientes *) malloc(cantReg * sizeof (Clientes));
        if (vClientes == NULL){
            errorFile(srcFile, MEM_INSUF_ERROR);
            return false;
        }

        pArchivo = fopen(srcFile, "rb");
        if (pArchivo == NULL){
            errorFile(srcFile, FILE_OPEN_ERROR);
            free(vClientes);
            return false;
        }
        fread(&vClientes[0], sizeof (Clientes), cantReg, pArchivo);
        fclose(pArchivo);

        pArchivo = fopen(dstFile, "wb");
        if (pArchivo == NULL){
            errorFile(dstFile, FILE_OPEN_ERROR);
            free(vClientes);
            return false;
        }
        fwrite(&vClientes[0], sizeof (Clientes), cantReg, pArchivo);
        fclose(pArchivo);
    }
    else {
        errorFile(srcFile, ARCHIVO_VACIO_ERROR);
        return false;
    }
    free(vClientes);
    return true;
}
#endif // CLIENTES_H_INCLUDED
