# PROJECT SPECIFICATION: DeepShell (AI-Native POSIX Engine)

## 1. Project Overview
Desarrollo de una **Interfaz de Línea de Comandos (CLI) de Alto Rendimiento** escrita en **C** que integra el modelo **DeepSeek-Coder** (ejecutándose localmente vía Ollama) para traducir peticiones en lenguaje natural en comandos POSIX ejecutables.

**Objetivo:** Crear un "terminal inteligente" donde el usuario interactúe en lenguaje natural (ej. "crea una carpeta llamada tests y entra en ella") y el shell gestione la ejecución y la persistencia del estado del sistema.

## 2. Tech Stack & Architecture
* **Core Logic:** C (Standard C99). Gestiona el REPL, la renderización de la UI con ANSI colors y la orquestación de procesos.
* **Cognitive Bridge:** Python 3.x. Intermediario para la comunicación HTTP con la API de Ollama y el saneamiento de respuestas mediante Regex.
* **AI Engine:** Ollama (Local Inference).
* **Model:** `deepseek-coder:6.7b` (Seleccionado por su superioridad en generación de código y baja latencia).

## 3. Advanced Pipeline (State-Sync & Trampoline)
1.  **Input:** El usuario introduce un prompt en el shell (ej. `deepshell [C:\...] > navega a src`).
2.  **Inference:** C invoca `bridge/brain.py` pasando el prompt y el contexto del directorio actual (`cwd`).
3.  **Trampoline Pattern:** Para evitar errores de escape en Windows CMD, la respuesta de la IA se escribe en un script efímero `.trampoline.sh`.
4.  **Execution:** El núcleo de C invoca `sh.exe` (Git Bash) para ejecutar el script, permitiendo operadores lógicos (`&&`, `|`) nativos de POSIX.
5.  **State Sync:** El script exporta el nuevo directorio mediante `pwd -W > .shell_cwd`. El proceso padre en C lee este archivo y ejecuta `chdir()` para mantener la persistencia de la navegación.
6.  **Audit:** Cada par `Input -> POSIX` se registra con timestamp en `docs/history.log`.

## 4. Repository Standards
* **Architecture:** Separación estricta de responsabilidades en carpetas `/src`, `/bin`, `/bridge`, y `/docs`.
* **Version Control:** Uso de *Conventional Commits* para el historial de Git.
* **Documentation:** README detallado con diseño ASCII y especificaciones de despliegue.

## 5. Constraints & Philosophy
* **Local-First:** Privacidad total y funcionamiento sin internet.
* **Senior Engineering:** Gestión manual de memoria, comprobación de punteros `NULL` y uso de `snprintf` para seguridad de buffers.
* **Style:** Explicaciones en **Español**, comentarios de código en **Inglés** (estándar de industria).

## 6. Author & License
* **Author:** Hector
* **License:** Standard MIT License
