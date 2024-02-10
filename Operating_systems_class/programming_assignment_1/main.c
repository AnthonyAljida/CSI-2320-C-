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
// Core functions
void listCurrentProcesses(pid_t arr[], int num);
void create_processes(int n);
void kill_all_child_processes();
void kill_process(int process_num);
void resume_process(int process_num);
void alarm_handler(int signum);
void sig_handle();
void r_from_r(int process_num);

// Helper functions throughout the program
int findIndex(pid_t arr[], int size, int value);
int stringToNumber(char command[]);
int stringToNumberSRRQ(char command[]);

// This object is used to manage the states of all the processes, allows for easy look up of each process
struct state
{
    bool ready;
    bool running;
    bool terminated;
};

// IMPORTANT, the way this shell manages all the process is by using their index in the process_pids has their unique ids for loop up, makes it very easy to search for processes
pid_t process_pids[MAX_PROCESSES] = {0};   // Array to store process IDs, initialize values to 0
int num_processes = 0;                     // Counter to keep track of the processes
struct state processStates[MAX_PROCESSES]; // An array of state objects made so every process has their own state,
bool waiting = false;
// For Cn command
bool is_a_process_running = false;

// For managing run all command
int current = 0;
bool rall_pressed = false;
bool is_running = false;

// Flags for the 2 modes
bool rrq = false;
bool fcfs = true;

// For the globally running process
int global_process_num;

// Number for alarm()
int numForS;

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
    signal(SIGALRM, alarm_handler);
    while (1)
    {
        // Initial start up
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

            // If a child process is running, do not create new processes
            for (int i = 0; i < MAX_PROCESSES; i++)
            {
                if (processStates[i].running)
                {
                    is_a_process_running = true;
                }
            }
            // If no process is running, create processes
            if (!is_a_process_running)
            {
                numForC = stringToNumber(command);
                if (numForC >= 5 && numForC <= 10)
                {
                    num_processes = 0;
                    create_processes(numForC);
                    printf("%d processes made\n", numForC);
                }
                else
                {
                    printf("Invalid number of processes. Please enter a number between 5 and 10.\n");
                }

                break;
            }
            else if (is_a_process_running)
            {
                printf("Switch back to shell to make more processes\n");
                is_a_process_running = false;
                break;
            }

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
            if (strlen(command) == 1 && command[0] == 'k')
            {
                printf("Please enter an ID number\n");
                break;
            }
            numForK = stringToNumber(command);
            kill_process(numForK);
            break;

        case 'r':

            // Tells program rall is pressed
            if (strcmp(command, "rall") == 0)
            {
                current = 0;
                rall_pressed = true;
                if (global_process_num != 0)
                {

                    r_from_r(global_process_num);
                }
                resume_process(process_pids[current]);
                break;
            }

            // Error handling
            else if (strlen(command) > 1 && !(isdigit(command[1])))
            {
                printf("Invalid command\n");
                break;
            }
            // Error handling
            else if (strlen(command) == 1 && command[0] == 'r')
            {
                printf("Please enter an ID number\n");
                break;
            }
            numForR = stringToNumber(command);

            // When a process is runnning and you run another one, this code block handles that logic
            if (findIndex(process_pids, MAX_PROCESSES, numForR) != -1 && is_running)
            {
                r_from_r(global_process_num);
                if (rall_pressed)
                {
                    current = findIndex(process_pids, MAX_PROCESSES, numForR);
                }
                resume_process(numForR);
                break;
            }

            resume_process(numForR);
            break;
        case 's':
            if (strcmp(command, "sfcfs") == 0)
            {
                printf("Mode set to first come first serve\n");
                fcfs = true;
                alarm(0);
                break;
            }
            else if (strlen(command) > 3 && command[1] == 'r' && command[2] == 'r')
            {
                numForS = stringToNumberSRRQ(command);
                alarm(numForS);
                printf("Alarm set for %d seconds\n", numForS);
                rrq = true;
                break;
            }
            else
            {
                printf("Invalid command\n");
                break;
            }
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
            processStates[i].terminated = false;
            num_processes++; // Increment the number of created processes

            // Suspend the child process immediately
            kill(pid, SIGSTOP);
        }
    }
}
void alarm_handler(int signum)
{
    // For running processes while the alarm goes off
    if (is_running)
    {
        sig_handle();
    }
    alarm(numForS);
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

    // Conditionals to check for a successful terminate
    if (exist != -1 && (processStates[exist].ready || processStates[exist].running))
    {
        if (kill(process_num, SIGKILL) == 0)
        {
            printf("Process ID %d killed successfully and state is now set to terminated.\n", process_num);
            int index = findIndex(process_pids, MAX_PROCESSES, process_num);
            processStates[index].ready = false;
            processStates[index].running = false;
            processStates[index].terminated = true;

            is_running = false;
        }
        // Error handling
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
void r_from_r(int process_num)
{
    // If the terminate is successful, update the values of the process
    int exist = findIndex(process_pids, MAX_PROCESSES, process_num);

    // Conditionals to check for a successful terminate
    if (exist != -1 && processStates[exist].running)
    {
        if (kill(process_num, SIGSTOP) == 0)
        {
            int index = findIndex(process_pids, MAX_PROCESSES, process_num);
            processStates[index].ready = true;
            processStates[index].running = false;
            processStates[index].terminated = false;
            // No process is running right now set reset flags
            global_process_num = 0;
            is_running = false;
            // If rall is pressed, go back to the first element to rereun processes
            if (rall_pressed)
            {
                current = 0;
            }
        }
        // Error handling

        else
        {
            printf("Error\n");
        }
    }
    else
    {
        printf("Error: Unable to find process %d\n", process_num);
    }
}

void resume_process(int process_num)
{
    // Resumes process
    int exist = findIndex(process_pids, MAX_PROCESSES, process_num);
    // If the process exists and is also not terminated
    if (exist != -1 && !(processStates[exist].terminated))
    {
        if (kill(process_num, SIGCONT) == 0)
        {
            global_process_num = process_num;
            printf("Process ID %d resumed and state is now running.\n", process_num);
            int index = findIndex(process_pids, MAX_PROCESSES, process_num);
            processStates[index].ready = false;
            processStates[index].running = true;
            processStates[index].terminated = false;

            // Logic for when all processes are scheduled to run
            is_running = true;
            if (rall_pressed)
            {
                current++;

                if (current == num_processes)
                {
                    current = 0;
                    rall_pressed = false;
                }
            }
        }
        else
        {
            printf("Error: Unable to resume process %d, process does not exist\n", process_num);
        }
    }
    // When all processes are running but a process in the queue is terminated, use recursion to skip it
    else if (rall_pressed)
    {
        current = current + 1;
        // Recursion to skip terminated processes with the schedule
        resume_process(process_pids[current]);
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

            // Keeps track of the current process
            is_running = false;
            global_process_num = 0;
            break;
        }
    }
    // For managing rall
    if (rall_pressed)
    {
        resume_process(process_pids[current]);
    }
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
    // Example length of string
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
int stringToNumberSRRQ(char command[])
{
    // Example length of string
    char number_str[20];
    int j = 0;

    for (int i = 3; i < strlen(command); i++)
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