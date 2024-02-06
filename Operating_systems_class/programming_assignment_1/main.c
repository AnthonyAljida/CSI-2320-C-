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
int findIndex(pid_t arr[], int size, int value);
int stringToNumber(char command[]);

void handle_ctrl_c(int signum);
void set_scheduler_rr(int quantum);
void set_scheduler_fcfs();
void resume_process(int process_num);
void run_all_processes();

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
int main()
{
    // newline gets rid of the newline character each time and command is our commands
    char newline;
    char command[20];
    int num;
    int num2;
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

        case 'x':
            kill_all_child_processes();
            exit(0);
            break;

        case 'c':
            num_processes = 0;
            int userEnteredProcesses = command[1] - '0';
            if (userEnteredProcesses >= 5 && userEnteredProcesses <= 10)
            {
                create_processes(userEnteredProcesses);
            }
            else
            {
                printf("%d\n", userEnteredProcesses);
                printf("Invalid number of processes. Please enter a number between 5 and 10.\n");
            }

            break;

        case 'l':
            if (num_processes == 0)
            {
                printf("You have no processes made\n");
                break;
            }
            listCurrentProcesses(process_pids, num_processes);
            break;

        case 'k':
            num = stringToNumber(command);
            kill_process(num);
            break;

        case 'r':
            num2 = stringToNumber(command);
            resume_process(num2);
        default:
            // User entered an invalid command
            printf("Invalid command\n");
        }
    }
}

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
void kill_all_child_processes()
{
    // Kill all child processes using SIGKILL
    kill(0, SIGKILL);
    // Wait for all child processes to terminate
    while (wait(NULL) > 0)
        ;
}
void handle_ctrl_c(int signum)
{
    // Handle Ctrl-C
    // Suspend the currently running process and transfer control to scheduler
}

void create_processes(int n)

{

    for (int i = 1; i <= n; i++)
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
            process_pids[i - 1] = pid; // Store the PID in the array to be able to use
            processStates[i - 1].ready = true;
            processStates[i - 1].running = false;
            processStates[i - 1].waiting = false;
            processStates[i - 1].terminated = false;
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

void set_scheduler_rr(int quantum)
{
    // Set scheduler to round robin with time quantum
    // Use alarm() system call
}

void set_scheduler_fcfs()
{
    // Set scheduler to first come first serve
}

void kill_process(int process_num)
{
    // If the terminate is successful, update the values of the process
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
        printf("Error: Unable to kill process %d\n, process does not exist", process_num);
    }
}

void resume_process(int process_num)
{
    if (kill(process_num, SIGCONT) == 0)
    {
        printf("Process ID %d resumed and state is now running.\n", process_num);
        int index = findIndex(process_pids, MAX_PROCESSES, process_num);
        processStates[index].ready = false;
        processStates[index].running = true;
        processStates[index].waiting = false;
        processStates[index].terminated = false;
    }
    else
    {
        printf("Error: Unable to kill process %d\n, process does not exist", process_num);
    }
}

void run_all_processes()
{
    // Run all processes after configurations
}