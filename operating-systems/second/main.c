#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <getopt.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/utsname.h>


extern char **environ;
void solveQuadratic();
void processOptions();
void processLongOptions();
void printEnvironment();
void printEnvVariable();
void setEnvVariable();
void removeEnvVariable();
void getUserInfo();
void getComputerInfo();

void menu() {
    int choice;
    while (1) {
        printf("\nMenu\n");
        printf("Enter number of the task (1-9) or 0 for exit:");
        scanf("%d", &choice);

        switch (choice) {
            case 1: solveQuadratic(); break;
            case 2: processOptions(); break;
            case 3: processLongOptions(); break;
            case 4: printEnvironment(); break;
            case 5: printEnvVariable(); break;
            case 6: setEnvVariable(); break;
            case 7: removeEnvVariable(); break;
            case 8: getUserInfo(); break;
            case 9: getComputerInfo(); break;
            case 0: exit(0);
            default: printf("Invalid choice. Please try again.\n");
        }
    }
}

//1
void solveQuadratic() {
    double a, b, c;
    printf("Enter coefficients a, b, and c: ");
    scanf("%lf %lf %lf", &a, &b, &c);

    double discriminant = b * b - 4 * a * c;
    if (discriminant > 0) {
        double x1 = (-b + sqrt(discriminant)) / (2 * a);
        double x2 = (-b - sqrt(discriminant)) / (2 * a);
        printf("Roots: x1 = %.2f, x2 = %.2f\n", x1, x2);
    } else if (discriminant == 0) {
        double x = -b / (2 * a);
        printf("One root: x = %.2f\n", x);
    } else {
        printf("No real roots\n");
    }
}

//2
void processOptions() {
    printf("Enter options (-h for help, -o <file>, -c): ");

    char args[100];
    scanf(" %[^\n]", args);

    if (args[0] == '\0') {
        printf("No options entered. Returning to menu.\n");
        return;
    }

    int argc = 0;
    char *argv[10];
    argv[argc++] = "./program";  // pustyshka

    char *token = strtok(args, " ");
    while (token && argc < 10) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }

    optind = 1;

    int opt;
    while ((opt = getopt(argc, argv, "ho:c")) != -1) {
        switch (opt) {
            case 'h':
                printf("Usage: -h -o <file> -c\n");
            return;
            case 'o':
                printf("Output file: %s\n", optarg);
            break;
            case 'c':
                printf("Special mode activated\n");
            break;
            default:
                printf("Invalid option\n");
        }
    }
}



//3
void processLongOptions() {
    printf("Enter options (--help, --output <file>, --compile): ");

    char args[100];
    scanf(" %[^\n]", args);

    if (args[0] == '\0') {
        printf("No options entered. Returning to menu.\n");
        return;
    }

    int argc = 0;
    char *argv[10];
    argv[argc++] = "./program";

    char *token = strtok(args, " ");
    while (token && argc < 10) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }

    optind = 1;

    int opt;
    struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"output", required_argument, 0, 'o'},
        {"compile", no_argument, 0, 'c'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "ho:c", long_options, NULL)) != -1) {
        switch (opt) {
            case 'h':
                printf("Usage: --help --output <file> --compile\n");
            return;
            case 'o':
                printf("Output file: %s\n", optarg);
            break;
            case 'c':
                printf("Compile mode activated\n");
            break;
            default:
                printf("Invalid option\n");
        }
    }
}

//4
void printEnvironment() {
    for (char **env = environ; *env != NULL; env++) {
        printf("%s\n", *env);
    }
}

//5
void printEnvVariable() {
    char var[50];
    printf("Enter the name of the environment variable: ");
    scanf("%s", var);

    char *value = getenv(var);
    if (value) {
        printf("Value of %s: %s\n", var, value);
    } else {
        printf("Environment variable %s not found\n", var);
    }
}

//6
void setEnvVariable() {
    char var[50], value[50];
    printf("Enter the variable name: ");
    scanf("%s", var);
    printf("Enter the value: ");
    scanf("%s", value);

    if (setenv(var, value, 1) == 0) {
        printf("Set %s = %s\n", var, value);
    } else {
        perror("Error setting environment variable");
    }
}

//7
void removeEnvVariable() {
    char var[50];
    printf("Enter the variable name to remove (or leave blank to remove all): ");
    scanf("%s", var);

    if (var[0] == '\0') {
        clearenv();
        printf("All environment variables removed\n");
    } else {
        unsetenv(var);
        printf("Environment variable %s removed\n", var);
    }
}

//8
void getUserInfo() {
    uid_t uid = getuid();
    printf("User UID: %d\n", uid);

    struct passwd *pw = getpwuid(uid);
    if (pw) {
        printf("Username: %s\n", pw->pw_name);
        printf("Home directory: %s\n", pw->pw_dir);
        printf("Shell: %s\n", pw->pw_shell);
    } else {
        printf("User information not found\n");
    }
}

//9
void getComputerInfo() {
    struct utsname buffer;
    if (uname(&buffer) == 0) {
        printf("System name: %s\n", buffer.sysname);
        printf("Node name: %s\n", buffer.nodename);
        printf("Release: %s\n", buffer.release);
        printf("Version: %s\n", buffer.version);
        printf("Machine: %s\n", buffer.machine);
    } else {
        perror("Error retrieving computer information");
    }
}

int main() {
    menu();
    return 0;
}
