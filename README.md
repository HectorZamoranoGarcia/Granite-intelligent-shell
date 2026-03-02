# DeepShell (AI-Native POSIX Engine)

**Advanced hybrid command-line interface developed in C and Python that utilizes local Artificial Intelligence to translate natural language into native POSIX system commands.**

# DEEPSHELL (AI-Native POSIX Engine)

```text
    ____  _________________  _____ __  __________    __
   / __ \/ ____/ ____/ __ \/ ___// / / / ____/ /   / /
  / / / / __/ / __/ / /_/ /\__ \/ /_/ / __/ / /   / /
 / /_/ / /___/ /___/ ____/___/ / __  / /___/ /___/ /___
/_____/_____/_____/_/    /____/_/ /_/_____/_____/_____/
      DEEPSHELL POSIX ENGINE by Hector

## Description

DeepShell (formerly Granite Shell) is a stateful, AI-driven terminal environment. It interprets user intent expressed in natural language and executes the corresponding operations using standard Unix tools.

The architecture relies on a highly decoupled hybrid model: a low-level core written in **C** manages the UI, process execution, and directory state; a cognitive bridge in **Python** handles IPC (Inter-Process Communication) and prompt engineering; and a local **DeepSeek** LLM performs the translation.

Built primarily for Windows environments heavily reliant on Unix toolchains (like Git Bash), it implements advanced subshell mechanics to bypass Windows CMD parsing limitations and guarantee native POSIX execution.

## Core Mechanics & Architecture

### 1. The Core Engine (C)

The `main.c` executable handles the Read-Eval-Print Loop (REPL), ANSI-based UI rendering, and process orchestration. It spawns the Python cognitive layer via `popen()` and manages the bidirectional state synchronization.

### 2. The Cognitive Bridge (Python)

The `brain.py` script interfaces with a local instance of Ollama. It receives the user's natural language input along with the current spatial context (working directory) injected by the C core. It enforces strict heuristics via zero-shot prompting to strip all markdown and conversational artifacts, returning pure executable code.

### 3. Trampoline Execution Pattern

To avoid the standard Windows `system()` CMD string-escaping hell (which breaks logical operators like `&&` or pipes `|`), the system uses an ephemeral script pattern:

1. The C core writes the LLM's raw output to a hidden `.trampoline.sh` file.
2. The core invokes `sh.exe` to execute the file natively, fully supporting complex POSIX syntax.

### 4. Bidirectional State Synchronization

Subshells natively destroy their state upon exit. To maintain spatial persistence (e.g., when a user asks to "navigate to a folder"):

1. The trampoline script appends `pwd -W > .shell_cwd` to its execution block.
2. Upon subshell termination, the C parent process reads `.shell_cwd`, executes a system-level `chdir()`, and dynamically updates the terminal prompt.

### 5. Audit Subsystem

All translations from natural language to POSIX are silently appended to `docs/history.log` with precise timestamps, ensuring full traceability of the LLM's operational decisions.

## Repository Structure

```text
DeepShell/
├── bin/
│   └── dshell.exe             # Compiled binary executable
├── bridge/
│   └── brain.py               # Python cognitive layer (LLM Bridge)
├── docs/
│   └── history.log            # Execution audit trail (Auto-generated)
├── src/
│   └── main.c                 # C core and orchestration logic
├── .gitignore                 # Exclusion rules for binaries and ephemeral state
└── README.md                  # System documentation

## Technologies

* **Core:** C (Standard C99)
* **Bridge:** Python 3.x (`requests`, `sys`, `os`, `re`)
* **Inference Engine:** Ollama running `deepseek-coder:6.7b`
* **Execution Environment:** Git Bash (`sh.exe`)
* **Build Tool:** GCC (GNU Compiler Collection)

## Deployment & Execution

### 1. Prerequisites

* **Ollama** installed and running on default port `11434`.
* The AI model pulled into VRAM: `ollama run deepseek-coder:6.7b`.
* **Python 3** and **GCC** configured in the system PATH.
* **Git Bash** installed at `C:\Program Files\Git\usr\bin\sh.exe`.

### 2. Compilation

Compile the orchestration core:

```bash
gcc src/main.c -o bin/dshell.exe

### 3. Initialization
Launch the binary. To mitigate initial LLM cold-start latency, the system assumes the Ollama daemon is pre-loaded:

```bash
.\bin\dshell.exe

## Author
**Hector**

## Notes
* This project was developed for educational purposes to demonstrate the interoperability between low-level system programming and modern AI APIs.
* The project requires an active local instance of Ollama to function.
* Occasional AI assistance was used for generating regex patterns and debugging syntax errors during the development of the Python bridge.

## License
Standard MIT License.
