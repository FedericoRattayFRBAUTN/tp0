#include "client.h"

//Federico Rattay TP 0 - 2025

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un Log.");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// Loggeamos el valor de config
	log_info(logger, "Obteniendo Config File...");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");
	log_info(logger, "IP: %s",ip);
	log_info(logger, "Puerto: %s",puerto);
	log_info(logger, "Clave: %s",valor);
	log_info(logger, "Config File Listo");

	/* ---------------- LEER DE CONSOLA ---------------- */

	//leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él
	
	// Creamos una conexión hacia el servidor
	log_info(logger, "Creando conexion...");
	conexion = crear_conexion(ip, puerto);
	
	if(conexion == -1){
		perror("Fallo la conexion al servidor");
		exit(EXIT_FAILURE);
	}

	// Enviamos al servidor el valor de CLAVE como mensaje
	log_info(logger, "Enviando valor de CLAVE...");
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	log_info(logger, "Enviando paquete...");
	paquete(conexion);
	log_info(logger, "Paquete enviado");

	log_info(logger, "Terminando programa...");
	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log","tp0", 1, LOG_LEVEL_INFO);
	if(nuevo_logger == NULL){
		perror("Error en creacion del log");
		exit(EXIT_FAILURE);
	}
	return 	nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");
	if(nuevo_config == NULL){
		perror("Error en creacion del config");
		exit(EXIT_FAILURE);
	}
	return 	nuevo_config;
}

void leer_consola(t_log* logger)
{
	char *linea;
	linea = readline("> ");
	log_info(logger, "%s\n", linea);

    while(strcmp(linea, "") != 0){
		free(linea);
		linea = readline("> ");
		log_info(logger, "%s\n", linea);
    };

	free(linea);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	leido = readline("> ");
	agregar_a_paquete(paquete, leido, sizeof(leido));

    while(strcmp(leido, "") != 0){
		//Leer siguiente linea
		free(leido);
		leido = readline("> ");
		//Agregar linea a paquete
		agregar_a_paquete(paquete, leido, sizeof(leido));
    };

	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
}
