# Reproductor de Música NFC con Raspberry Pi Pico

Un reproductor de música interactivo programado en C++ (Arduino IDE) que utiliza tarjetas o discos RFID/NFC para activar listas de reproducción específicas. Construido alrededor de una Raspberry Pi Pico, este proyecto destaca por su capacidad de **multitarea real (código no bloqueante)**, permitiendo leer sensores, reproducir audio y procesar botones físicos simultáneamente sin pausas ni interrupciones.

## Características Principales

* **Activación por RFID:** Cada tarjeta/disco NFC tiene un UID único asignado a una carpeta específica en la tarjeta MicroSD.
* **Multitarea sin retrasos:** Lógica basada en banderas de estado (cambio de flanco) para los botones, eliminando la necesidad de comandos `delay()` largos que congelan el sistema.
* **Controles Físicos:** Botones dedicados para Play/Pausa, Subir Volumen y Bajar Volumen con sistema anti-rebote por software.
* **Indicadores Visuales:** LEDs de estado (Verde = Acceso concedido / Leyendo; Rojo = Etiqueta desconocida).

## Hardware Requerido

* 1x Raspberry Pi Pico (RP2040)
* 1x Módulo Lector RFID MFRC522
* 1x Módulo DFRobot DFPlayer Mini
* 1x Altavoz (compatible con los 3W del DFPlayer)
* 1x Tarjeta MicroSD (Máximo 32GB, formateada en FAT32)
* 3x Botones pulsadores (Push buttons)
* 2x LEDs (1 Verde, 1 Rojo) y 2x Resistencias (220Ω - 330Ω)
* 1x Resistencia de 1kΩ - 2.2kΩ (Para la línea RX del DFPlayer)

## Esquema de Conexiones (Pinout)

### Lector RFID MFRC522 (Bus SPI0)

| MFRC522 | Raspberry Pi Pico | Nota |
| --- | --- | --- |
| **3.3V** | Pin 36 (3V3 OUT) | Alimentación |
| **GND** | Pin 38 (GND) | Tierra |
| **SCK** | GP18 (Pin 24) | SPI0 SCK |
| **MOSI** | GP19 (Pin 25) | SPI0 TX |
| **MISO** | GP16 (Pin 21) | SPI0 RX |
| **SDA (CS)** | GP17 (Pin 22) | Pin esclavo (Configurable) |
| **RST** | GP20 (Pin 26) | Pin de reinicio (Configurable) |

### DFPlayer Mini (UART0)

| DFPlayer Mini | Raspberry Pi Pico | Nota |
| --- | --- | --- |
| **VCC** | Pin 40 (VBUS) | Alimentación a 5V desde USB |
| **GND** | Pin 38 (GND) | Tierra |
| **RX** | GP0 (Pin 1) | **Importante:** Usar resistencia de 2.2kΩ en serie |
| **TX** | GP1 (Pin 2) | Conexión directa |
| **SPK1 / SPK2** | Altavoz | Salida directa de audio |

### Botones e Indicadores (Pull-up interno)

| Componente | Raspberry Pi Pico | Acción |
| --- | --- | --- |
| **Botón 1** | GP27 (Pin 32) | Play / Pausa |
| **Botón 2** | GP26 (Pin 31) | Subir Volumen |
| **Botón 3** | GP2 (Pin 4) | Bajar Volumen |
| **LED Verde** | GP22 (Pin 29) | Éxito / Lectura correcta |
| **LED Rojo** | GP28 (Pin 34) | Error / UID Desconocido |

## Preparación de la Tarjeta MicroSD

El DFPlayer Mini es estricto con el formato y la nomenclatura de los archivos.

1. Formatea la memoria MicroSD en **FAT32**.
2. Crea carpetas numeradas con dos dígitos (`01`, `02`, `03`...).
3. Nombra los archivos de audio dentro de cada carpeta con tres dígitos consecutivos (`001.mp3`, `002.mp3`...).

```text
SD (Raíz)
├── 01/
│   ├── 001.mp3
│   └── 002.mp3
├── 02/
│   └── 001.mp3

```

## Configuración del Entorno (Software)

1. **Instalar el núcleo de la Pico:** En Arduino IDE, ve a *Preferencias* y añade la URL del gestor de tarjetas de Earle F. Philhower:
`[https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json](https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json)`
Luego instala *Raspberry Pi Pico/RP2040* desde el Gestor de Tarjetas.
2. **Instalar Librerías:**
* `MFRC522` (por GithubCommunity)
* `DFRobotDFPlayerMini` (por DFRobot)


3. **Registrar UIDs:** Antes de cargar el código final, utiliza el monitor serie para escanear tus tarjetas RFID, obtén el código numérico de 32 bits y asígnalo a las variables `OC1`, `OC2`, etc., dentro del código.

## Notas de Desarrollo

* **Anti-rebote y Bloqueos:** Se evitó intencionalmente el uso de funciones `delay()` mayores a 50ms (usados únicamente como debounce físico) para asegurar que la lectura del MFRC522 nunca se interrumpa.
* **Little-Endian:** La lectura de los bytes del UID del MFRC522 se invierte matemáticamente mediante desplazamientos de bits (`<<`) para generar un número entero limpio de 32 bits, idéntico al estándar arrojado por sistemas Python.
