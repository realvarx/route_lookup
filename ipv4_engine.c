#include "ipv4_engine.h"

/* void ipv4_addr_str (ipv4_addr_t addr, char* str);
 *
 * DESCRIPCIÓN:
 *   Esta función genera una cadena de texto que representa la dirección IPv4
 *   indicada.
 *
 * PARÁMETROS:
 *   'addr': La dirección IP que se quiere representar textualente.
 *    'str': Memoria donde se desea almacenar la cadena de texto generada.
 *           Deben reservarse al menos 'IPv4_STR_MAX_LENGTH' bytes.
 */
void ipv4_addr_str(ipv4_addr addr, char *str)
{
    if (str != NULL) {
        sprintf(str, "%d.%d.%d.%d", addr.u8[0], addr.u8[1], addr.u8[2], addr.u8[3]);
    }
}

/* int ipv4_str_addr (char* str, ipv4_addr_t addr);
 *
 * DESCRIPCIÓN:
 *   Esta función analiza una cadena de texto en busca de una dirección IPv4.
 *
 * PARÁMETROS:
 *    'str': La cadena de texto que se desea procesar.
 *   'addr': Memoria donde se almacena la dirección IPv4 encontrada.
 *
 * VALOR DEVUELTO:
 *   Se devuelve 0 si la cadena de texto representaba una dirección IPv4.
 *
 * ERRORES:
 *   La función devuelve -1 si la cadena de texto no representaba una
 *   dirección IPv4.
 */
int ipv4_str_addr(char *str, ipv4_addr addr)
{
    int err = -1;

    if (str != NULL) {
        unsigned int addr_int[IPv4_ADDR_SIZE];
        int len = sscanf(str, "%d.%d.%d.%d", &addr_int[0], &addr_int[1], &addr_int[2], &addr_int[3]);

        if (len == 4) {
            int i;
            for (i = 0; i < 4; i++) {
                addr.u8[i] = (unsigned char)addr_int[i];
            }

            err = 0;
        }
    }

    return err;
}