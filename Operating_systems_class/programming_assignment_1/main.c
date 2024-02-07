#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdbool.h>

// Fixed constant for process array
#define MAX_PROCESSES 10

// Function prototypes
// *Processes started
void listCurrentProcesses(pid_t arr[], int num);
void create_processes(int n);
void kill_all_child_processes();
void kill_process(int process_num);
void resume_process(int process_num);

// Helper functions throughout the program
void sig_handle();
int findIndex(pid_t arr[], int size, int value);
int stringToNumber(char command[]);
// This object is used to manage the states of all the processes, allows for easy look up of each process
struct state
{
    bool ready;
    bool running;
    bool waiting;
    bool terminated;
};

// IMPORTANT, the way this shell manages all the process is by using their index in the process_pids has their unique ids for loop up, makes it very easy to search for processes
pid_t process_pids[MAX_PROCESSES] = {0};   // Array to store process IDs, initialize values to 0
int num_processes = 0;                     // Counter to keep track of the processes
struct state processStates[MAX_PROCESSES]; // An array of state objects made so every process has their own state,
bool waiting = false;

int main()
{

    // newline gets rid of the newline character each time and command is our commands
    char newline;
    char command[20];
    // For the switch statements, variables for the numbers in the string
    int numForC;
    int numForK;
    int numForR;

    // Handles ctrl+c
    signal(SIGINT, sig_handle);
    while (1)
    {

        // Starting declarations
        printf("shell 5500>>>");
        scanf("%19s", command);
        scanf("%c", &newline);

        // Makes the values not matter on case sensitivity
        for (int i = 0; command[i] != '\0'; i++)
        {
            if (isalpha(command[i]))
            {
                command[i] = tolower(command[i]);
            }
        }

        // Our commands are only single characters, we only need to check the first value
        char first_char = command[0];
        switch (first_char)
        {

        // Case for ending the program
        case 'x':
            // Error handling
            if (strlen(command) > 1)
            {
                printf("Invalid command\n");
                break;
            }
            // Terminate all child processes
            kill_all_child_processes();
            // Terminate main process
            printf("All processes terminated");
            exit(0);
            break;

        // Case to create processes
        case 'c':

            numForC = stringToNumber(command);
            if (numForC >= 5 && numForC <= 10)
            {
                num_processes = 0;
                create_processes(numForC);
            }
            else
            {
                printf("Invalid number of processes. Please enter a number between 5 and 10.\n");
            }

            break;

        case 'l':
            // Error handling
            if (strlen(command) > 1)
            {
                printf("Invalid command\n");
                break;
            }
            // Error handling
            if (num_processes == 0)
            {
                printf("You have no processes made\n");
                break;
            }
            listCurrentProcesses(process_pids, num_processes);
            break;

        case 'k':
            // Error handling
            if (strlen(command) > 1 && !(isdigit(command[1])))
            {
                printf("Invalid command\n");
                break;
            }
            numForK = stringToNumber(command);
            kill_process(numForK);
            break;

        case 'r':
            // Error handling
            if (strlen(command) > 1 && !(isdigit(command[1])))
            {
                printf("Invalid command\n");
                break;
            }
            numForR = stringToNumber(command);
            resume_process(numForR);
            // Pause the while loop
            while (waiting)
                ;
            break;
        default:
            // User entered an invalid command
            printf("Invalid command\n");
        }
    }
}

void kill_all_child_processes()
{
    // Kill all child processes using SIGKILL
    kill(0, SIGKILL);
    // Wait for all child processes to terminate
    while (wait(NULL) > 0)
        ;
}

void create_processes(int n)

{
    // Loop to create processes
    for (int i = 0; i < n; i++)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            // Error handling
            perror("fork had failed making the child process");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // Child process
            fflush(stdout);
            // Execute the program defined in the first task
            execl("./task", "task", NULL);
            // If exec() fails
            perror("exec has failed");
        }
        else
        {
            // Parent process
            process_pids[i] = pid; // Store the PID in the array to be able to use
            processStates[i].ready = true;
            processStates[i].running = false;
            processStates[i].waiting = false;
            processStates[i].terminated = false;
            num_processes++; // Increment the number of created processes

            // Suspend the child process immediately
            kill(pid, SIGSTOP);
        }
    }
}

void listCurrentProcesses(pid_t arr[], int size)
{
    printf("Process IDs:\n");
    // Loop through the array and listCurrentProcesses each element
    for (int i = 0; i < size; i++)
    {
        if (processStates[i].ready)
        {
            printf("Process %d, %d is in the ready state\n", i + 1, arr[i]);
        }
        else if (processStates[i].running)
        {
            printf("Process %d, %d is in the running state\n", i + 1, arr[i]);
        }
        else if (processStates[i].waiting)
        {
            printf("Process %d, %d is in the waiting state\n", i + 1, arr[i]);
        }
        else if (processStates[i].terminated)
        {
            printf("Process %d, %d is now terminated\n", i + 1, arr[i]);
        }
    }
    printf("Total number of processes %d\n", num_processes);
}

void kill_process(int process_num)
{
    // If the terminate is successful, update the values of the process
    int exist = findIndex(process_pids, MAX_PROCESSES, process_num);
    if (exist != -1)
    {
        if (kill(process_num, SIGKILL) == 0)
        {
            printf("Process ID %d killed successfully and state is now set to terminated.\n", process_num);
            int index = findIndex(process_pids, MAX_PROCESSES, process_num);
            processStates[index].ready = false;
            processStates[index].running = false;
            processStates[index].waiting = false;
            processStates[index].terminated = true;
        }
        else
        {
            printf("Error: Unable to kill process %d, process does not exist\n", process_num);
        }
    }
    else
    {
        printf("Error: Unable to kill process %d, process does not exist\n", process_num);
    }
}

void resume_process(int process_num)
{
    // Resumes process
    int exist = findIndex(process_pids, MAX_PROCESSES, process_num);
    if (exist != -1)
    {
        if (kill(process_num, SIGCONT) == 0)
        {
            printf("Process ID %d resumed and state is now running.\n", process_num);
            int index = findIndex(process_pids, MAX_PROCESSES, process_num);
            processStates[index].ready = false;
            processStates[index].running = true;
            processStates[index].waiting = false;
            processStates[index].terminated = false;
            waiting = true;
        }
        else
        {
            printf("Error: Unable to resume process %d, process does not exist\n", process_num);
        }
    }
    else
    {
        printf("Error: Unable to resume process %d, process does not exist\n", process_num);
    }
}
// Helper function for ctrl + c override
void sig_handle()
{
    // Suspend the running process
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        // If the process is running, then turn it back into the ready state
        if (processStates[i].running)
        {
            processStates[i].running = false;
            processStates[i].ready = true;
            kill(process_pids[i], SIGSTOP);
        }
    }
    // Set program to continue while loop
    waiting = false;
}
// Helper function through out the program
int findIndex(pid_t arr[], int size, int value)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == value)
        {
            return i; // Return the index if the value is found
        }
    }
    return -1; // Return -1 if the value is not found
}

// Helper function in switch statement
int stringToNumber(char command[])
{
    char number_str[20];
    int j = 0;

    for (int i = 1; i < strlen(command); i++)
    {
        // If the character is a digit, add it to the number string
        if (isdigit(command[i]))
        {
            number_str[j++] = command[i];
        }
        else if (j > 0)
        {
            // If it's not a digit but we've already started recording digits
            // It means we've reached the end of the number
            break;
        }
    }

    number_str[j] = '\0'; // Null-terminate the number string
    // Convert the extracted string number to an integer using atoi()
    int number = atoi(number_str);
    return number;
}