#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024
#define MAX_CMD_SIZE 4096
#define SH_PATH "C:\\Program Files\\Git\\usr\\bin\\sh.exe"

#define COLOR_GREEN "\033[1;32m"
#define COLOR_CYAN  "\033[1;36m"
#define COLOR_RESET "\033[0m"

int main() {
    char input[MAX_INPUT_SIZE];
    char python_command[MAX_CMD_SIZE];
    char ai_response[MAX_CMD_SIZE];
    char wrapper[MAX_CMD_SIZE];
    char cwd[MAX_INPUT_SIZE];

    system("cls");

    // ASCII Art "DEEPSHELL" (Slant font, escaped for C)
    printf(COLOR_CYAN);
    printf("    ____  _________________  _____ __  __________    __ \n");
    printf("   / __ \\/ ____/ ____/ __ \\/ ___// / / / ____/ /   / /  \n");
    printf("  / / / / __/ / __/ / /_/ /\\__ \\/ /_/ / __/ / /   / /   \n");
    printf(" / /_/ / /___/ /___/ ____/___/ / __  / /___/ /___/ /___ \n");
    printf("/_____/_____/_____/_/    /____/_/ /_/_____/_____/_____/ \n");
    printf(COLOR_RESET);
    printf("      DEEPSHELL POSIX ENGINE by Hector  \n\n");

    while (1) {
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            strcpy(cwd, "unknown");
        }

        // Prompt actualizado a deepshell
        printf(COLOR_GREEN "deepshell [%s] > " COLOR_RESET, cwd);

        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) break;
        input[strcspn(input, "\r\n")] = 0;

        if (strcmp(input, "exit") == 0) break;
        if (strlen(input) == 0) continue;

        snprintf(python_command, sizeof(python_command), "python bridge/brain.py \"%s\"", input);

        FILE *fp = popen(python_command, "r");
        if (fp == NULL) continue;

        if (fgets(ai_response, sizeof(ai_response), fp) != NULL) {
            ai_response[strcspn(ai_response, "\r\n")] = 0;

            if (strlen(ai_response) > 0) {
                printf(COLOR_CYAN "   >> %s" COLOR_RESET "\n", ai_response);

                FILE *script = fopen(".trampoline.sh", "w");
                if (script != NULL) {
                    fprintf(script, "%s\n", ai_response);
                    fprintf(script, "pwd -W > .shell_cwd\n");
                    fclose(script);

                    snprintf(wrapper, sizeof(wrapper), "\"\"%s\" .trampoline.sh\"", SH_PATH);
                    system(wrapper);

                    FILE *cwd_file = fopen(".shell_cwd", "r");
                    if (cwd_file != NULL) {
                        char new_dir[MAX_CMD_SIZE];
                        if (fgets(new_dir, sizeof(new_dir), cwd_file) != NULL) {
                            new_dir[strcspn(new_dir, "\r\n")] = 0;
                            chdir(new_dir);
                        }
                        fclose(cwd_file);
                        remove(".shell_cwd");
                    }

                    remove(".trampoline.sh");
                    printf("\n");
                }
            }
        }
        pclose(fp);
    }
    return 0;
}
