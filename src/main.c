#include <signal.h>
#include "QDTFT_demo.h"
#include "spi.h"
#include "stdlib.h"

const char *appname = "spitest01";




void sigint_handler(int sig) {
    spi_close();
    gpiod_close();
    exit(1);
}


int main(int argc, char *argv[]) {

   
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
       // perror("signal SIGINT");
        exit(-1);
    }

    spi_init();
    QDTFT_Test_Demo();

    spi_close();
    gpiod_close();

    exit(0);
}
