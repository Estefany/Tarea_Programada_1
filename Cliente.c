    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
    #include <sys/select.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
     
    int main(int argc, char **argv) {
     
     
    FILE *f1;
    char buffer2[1024000];
    int leidos;
    char buf2[30];
     
            /* Creo el Socket: SOCK_STREAM para TCP y SOCK_DGRAM par UDP */
            int descriptor = socket(AF_INET, SOCK_STREAM, 0);
     
            /* Direccion Local */
            struct sockaddr_in *local_address = malloc(sizeof(struct sockaddr_in));
            /* Direccion remota ( a la que me quiero conectar )*/
            struct sockaddr_in *remote_address = malloc(sizeof(struct sockaddr_in));
     
            {       /* Con esto fuerzo a que el puerto local sea el 5201 y que tome la IP por defecto de la PC */
                    local_address->sin_family = AF_INET;
                    local_address->sin_addr.s_addr = INADDR_ANY;
                    local_address->sin_port = htons(5301);
     
                    bind(descriptor, (struct sockaddr *)local_address, sizeof(struct sockaddr_in));
            }
     
            {       /* Con esto indico que me quiero conectar al puerto 5200 de la IP 127.0.0.1 (localhost) */
                    remote_address->sin_family = AF_INET;
                    remote_address->sin_addr.s_addr = inet_addr("127.0.0.1");
                    remote_address->sin_port = htons(5300);
            }
     
            /* Me conecto al servidor */
            connect(descriptor, (struct sockaddr *)remote_address , sizeof(struct sockaddr_in) );
     
     
    f1 = fopen ("1.jpg", "rb");
    if (f1==NULL)
    {
       perror("No se puede abrir fichero.dat");
       return -1;
    }
     
    leidos = fread (buffer2, 1, 1024000, f1);
    sprintf(buf2,"%d",leidos);
     
            /* Le envio un Hola Mundo! */
            send(descriptor, buf2, strlen(buf2) + 1, 0);
        send(descriptor,buffer2, strlen(buffer2)+1,0);
            /* Cierro el socket y por ende la conexion */
            close(descriptor);
     
            free(local_address);
            free(remote_address);
     
            return EXIT_SUCCESS;
    }
