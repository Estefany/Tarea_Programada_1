#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
/*
Curso: Lenguajes de programación
Tarea Programada 1
Profesor: Andrei Fuentes L.

Grupo:


*/

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

/*
Funcion de impresion de errores
Imprime errores en pantalla y termina el proceso
Recibe un char* como mensaje de error y cierra el programa
*/
void error(char *mensaje_error){
	printf("%s\nIntente de nuevo!\n",mensaje_error);
	exit(0);
	}

/*Funcion Main
Entradas: 3
1- Puerto de escucha del servidor interno
2- Puerto de escucha 
3- Ip del otro 

Salidas: int = 1 al finalizar bien.
*/

int main(int argc, char *argv[]){
	
	 FILE *f1;
	  FILE *f2;
    char buffer2[1024000];
    int leidos;
    char buf2[30];
    
	//Validaciones de 3 argumentos(4 contando la invocacion)
	if (argc != 4) error("Ingrese los 3 argumentos necesarios");
	pid_t pID=fork();//Inicia el fork y guarda el identificador del proceso
	
	// Proceso hijo: Aca ejecuta el codigo cliente
	if (pID == 0) {
		int socket_cliente;//ID del socket de conexion
		struct sockaddr_in direc_servidor;//Estructura para la direccion del otro

		

		
		socket_cliente = socket(AF_INET, SOCK_STREAM, 0);//Pide el socket tcp/ip
		//Construccion de direccion 
		
		direc_servidor.sin_family = AF_INET;
		//transforma la direccion tipo char ej: "127.0.0.1" en tipo direccion para guardarlo
		inet_pton(AF_INET, argv[3], &(direc_servidor.sin_addr.s_addr));
		direc_servidor.sin_port = htons(atoi(argv[2]));
		
		//Inicia el proceso de conexion 
		printf("Iniciando conexio\n");
		printf("Espere...\n");
		int conectado=-1;//Identificador de conexion
		int intentos_conex= 0; 
		while(conectado<0){
			//Intenta conectarse (max 30 intentos en 30 segundos)
			conectado = connect(socket_cliente,
			(struct sockaddr *) &direc_servidor,
			sizeof(direc_servidor));
			intentos_conex++;
			if (intentos_conex > 30){
				close(socket_cliente);
				kill(getppid(),9);
				error("Limite de intentos de conexion 30");
				}
				sleep(1);//Pausa de 1 segundo
		}
		//Conexion establecida
		/*Comandos:
		* \033[2J Limpia la pantalla
		* \033[1;1H Se coloca en la esquina superior irzq de la misma
	
		* \033[01;37m Color Blanco
		* \\033[01;32m Color Verde*/
		printf(	
		"\033[01;32mConexion Establecida\n"
		"Ingrese l\033[01;37m\n");
		//Separacion de procesos envio de mensajes y cierre de socket
		pid_t pID2=fork();//Inicia el fork 2
				 
		//Proceso de lectura de teclado y envio de mensajes
		if (pID2 == 0) {
			pid_t otro_proceso = (getppid()-1);
			int ciclo = 1;
			while(ciclo == 1){
				if(getppid()==1){//Revisa estado del proceso padre para cerrar sockets
				close(socket_cliente);
				int muerto;
				muerto = kill(otro_proceso,9);
				usleep(10000);
				ciclo = 0;
				char *borre_killed = "";
				if(muerto==0) borre_killed ="\033[A\033[2K";
				printf(	"\033[2K\r"
				"\033[01;32m%s"
				"Conexion Finalizada..."
				"\nPresione Enter para salir"
				"\033[01;37m!!!",borre_killed);
				}
		    }
		}
		else {	
			   f1 = fopen ("1.jpg", "rb");
    if (f1==NULL)
    {
       perror("No se puede abrir fichero.dat");
       return -1;
    }
     
    leidos = fread (buffer2, 1, 1024000, f1);
    sprintf(buf2,"%d",leidos);
     
            /* Le envio un Hola Mundo! */
            send(conectado, buf2, strlen(buf2) + 1, 0);
        send(conectado,buffer2, strlen(buffer2)+1,0);
            /* Cierro el socket y por ende la conexion */
            close(conectado);
            
     
           
     
            return EXIT_SUCCESS;
			}
		 }
		 // Proceso padre: Aca ejecuta el codigo servidor
		 else {
			 int socket_serv, socket_serv_conectado;
			
			 /*Estructuras para direcciones de este servidor y del cliente
			  * externo.*/
			  struct sockaddr_in direc_servidor, direc_cliente;
			  //Pide el socket
			  //Tipo internet tcp/ip
			  socket_serv = socket(AF_INET, SOCK_STREAM, 0);
			  
			  //Construccion de direccion de este servidor
			  direc_servidor.sin_family = AF_INET;
			  direc_servidor.sin_addr.s_addr = INADDR_ANY;//Su propio IP
			  direc_servidor.sin_port = htons(atoi(argv[1]));
			  
			  //Union del socket con esta direccion
			  bind(socket_serv, (struct sockaddr *) &direc_servidor,
			  sizeof(direc_servidor));
			  //Le indica al socket que espere conexiones
			  listen(socket_serv,5);
			  //Inicial el proceso de aceptar conexiones y almacenarla en un nuevo
			  //identificador de socket
			  socklen_t largo_dir_cliente = sizeof(direc_cliente);
			  socket_serv_conectado = accept(socket_serv,
			  (struct sockaddr *) &direc_cliente,
			  &largo_dir_cliente);
			  //Separacion de procesos recepcion de mensajes y cierre de socket
			  pid_t pID2=fork();//Inicia el fork 2
			  //Proceso recepcion de mensajes
		if (pID2 == 0) {
			pid_t otro_proceso = (getppid()+1);
			int ciclo = 1;
			while(ciclo == 1){
				if(getppid()==1){//Revisa estado del proceso padre para cerrar sockets
				close(socket_serv);
				close(socket_serv_conectado);
				kill(otro_proceso,9);
				ciclo = 0;
				}
			}
		}
		else {
		           /* Leo e imprimo la informacion recivida a traves de la nueva conexion */
            recv(socket_serv_conectado, buf2, 30, 0);
             printf("buf2 en recv = %s\n ", buf2);
             
        leidos = sizeof(buf2);
        printf("buf2 = %s \n", buf2);
        printf("leidos = %d \n ", leidos);
       
       
     
    f2 = fopen ("nueva.txt", "wb");
    if (f2==NULL)
    {
       perror("No se puede abrir fichero");
       return -1;
    }
    recv(socket_serv_conectado, buffer2, 1024000, 0);
   
     
    fwrite (buffer2, 1, leidos, f2);
    printf("%d", leidos);
    read(socket_serv_conectado,buf2,50);
            printf("%s", buf2);
    
    printf("Archivo Recibido/n");
     
          close(socket_serv);
				close(socket_serv_conectado);
        
            return EXIT_SUCCESS;
    }
	  }
	
	return 1;
}
