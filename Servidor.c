    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
    #include <sys/select.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
     
    #define SOCKET_MAX_BUFFER 100
     
    int main(int argc, char **argv) {
     
        FILE *f2;
        char buf2[30];
        char buffer2[1024000];
       
        int leidos;
            int addrlen = sizeof(struct sockaddr_in);
            char buffer[SOCKET_MAX_BUFFER];
     
            /* Creo el Socket: SOCK_STREAM para TCP y SOCK_DGRAM par UDP */
            int descriptor = socket(AF_INET, SOCK_STREAM, 0);
            int remote_client;
     
            /* Direccion Local */
            struct sockaddr_in *local_address = malloc(sizeof(struct sockaddr_in));
            /* Direccion remota ( a la que me quiero conectar )*/
            struct sockaddr_in *remote_address = malloc(sizeof(struct sockaddr_in));
            printf("Conexion Abierta\n");
     
            {       /* Con esto fuerzo a que el puerto local sea el 5201 y que tome la IP por defecto de la PC */
                    local_address->sin_family = AF_INET;
                    local_address->sin_addr.s_addr = inet_addr("192.168.1.136");
                    local_address->sin_port = htons(5300);
     
                    bind(descriptor, (struct sockaddr *)local_address, sizeof(struct sockaddr_in));
                    printf("Conexion Abierta\n");
            }
     
            /* Activo la escucha de conexiones entrantes a traves del puerto 5200 y como maximo 100 conexiones.*/
            listen(descriptor, 100);
     
            /* Acepto nueva conexion entrante */
            remote_client = accept(descriptor, (struct sockaddr *)remote_address, (void *)&addrlen);
     
            /* Leo e imprimo la informacion recivida a traves de la nueva conexion */
            recv(remote_client, buf2, 30, 0);
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
    recv(remote_client, buffer2, 1024000, 0);
   
     
    fwrite (buffer2, 1, leidos, f2);
    printf("%d", leidos);
    read(remote_client,buf2,50);
            printf("%s", buf2);
    
    printf("Archivo Recibido/n");
     
            /* Cierro el socket y por ende la conexion */
            close(descriptor);
     
            free(local_address);
            free(remote_address);
     
            return EXIT_SUCCESS;
    }
