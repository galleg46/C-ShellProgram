#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void parser(char *input, char **cmd)
{
    while (*input != '\0')
    {
        while (*input == ' ' || *input == '\t' || *input == '\n') *input++ = '\0';
        *cmd++ = input;
        while (*input != '\0' && *input != ' ' && *input != '\t' && *input != '\n') input++;
    }

    *cmd = '\0';
}

void executeCommand(char **argv)
{
    int x = execvp(*argv, argv);

    if (x < 0)
    {
        printf("*** ERROR: Invalid command, exec call failed\n");
        exit(1);
    }
}

int main (int argc, char **argv)
{
    char inputString[1024];
    char *cmd[64];

    while(1)
    {
        int status;
        int childPID;

        printf("\n$mySimpleShell-> ");
        fgets(inputString, sizeof(inputString), stdin);
        inputString[strlen(inputString)-1] = '\0';
        parser(inputString, cmd);

        if (strcmp(cmd[0], "quit") == 0)
            break;

        childPID = fork();

        if (childPID == 0)
        {
            executeCommand(cmd);
        }
        else
        {
            waitpid(childPID, &status, 0);
        }
    }
}

