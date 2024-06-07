#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void UART_init(uint16_t baud_rate) {
    // Configuración de la UART con baud_rate proporcionado
    uint16_t ubrr_value = F_CPU / (16 * baud_rate) - 1;
    UBRR0H = (uint8_t)(ubrr_value >> 8);
    UBRR0L = (uint8_t)ubrr_value;
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);  // Habilitar transmisión y recepción
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Modo asíncrono, sin paridad, 1 bit de parada, 8 bits de datos
}

void UART_transmit_char(uint8_t data) {
    // Esperar a que el buffer de transmisión esté vacío
    while (!(UCSR0A & (1 << UDRE0)));
    // Colocar el dato en el registro de datos
    UDR0 = data;
}

int main(void) {
    // Inicializar UART con baud rate de 9600
    UART_init(9600);

    // Mensaje a enviar
    const char* message = "Hola desde AVR!\n\r";

    // Bucle infinito
    while (1) {
        // Enviar cada caracter del mensaje
        for (const char* ptr = message; *ptr != '\0'; ptr++) {
            UART_transmit_char(*ptr);
            // Esperar un poco para que sea visible en la terminal virtual
            _delay_ms(50);
        }
        // Esperar un poco antes de enviar el mensaje de nuevo
        _delay_ms(1000);
    }
}