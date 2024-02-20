#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdbool.h>

// Fixed constants
#define MAX_PROCESSES 10000000
#define MAX_SIZE 10000000
// Function prototypes
// Core functions
void listCurrentProcesses(pid_t arr[], int num);
void create_processes(int n);
void kill_all_child_processes();
void kill_process(int process_num);
/// @brief
/// @param process_num
void run_process(int process_num);
void alarm_handler(int signum);
/// @brief
void sig_handle();
void put_in_ready_state(int process_num);

// Helper functions throughout the program
int findIndex(pid_t arr[], int size, int value);
int stringToNumber(char command[]);
int stringToNumberSRRQ(char command[]);

// Define the structure of a queue
typedef struct
{
    int items[MAX_SIZE];
    int front;
    int back;
} Queue;

// Prototypes for the queue
void initQueue(Queue *q);
int isEmpty(Queue *q);
void enqueue(Queue *q, int value);
int dequeue(Queue *q);
int front(Queue *q);

// This object is used to manage the states of all the processes, allows for easy look up of each process
struct state
{
    bool ready;
    bool running;
    bool terminated;
    bool suspended;
};

// IMPORTANT, the way this shell manages all the process is by using their index in the process_pids has their unique ids for loop up, makes it very easy to search for processes
pid_t process_pids[MAX_PROCESSES] = {0};   // Array to store process IDs, initialize values to 0
int num_processes = 0;                     // Counter to keep track of the processes
struct state processStates[MAX_PROCESSES]; // An array of state objects made so every process has their own state,

// Flags for the 2 modes
bool rrq = false;
bool fcfs = true;

// For the globally running process
int global_process_num;

// Number for alarm()
int numForS;

// The scheduling queue
Queue scheduler;

int main()
{
    // Creates the queue for the project
    initQueue(&scheduler);
    // newline gets rid of the newline character each time and command is our commands
    char newline;
    char command[20];
    // For the switch statements, variables for the numbers in the string
    int numForC;
    int numForK;
    int numForR;

    // Handles ctrl+c and alarm
    signal(SIGINT, sig_handle);
    signal(SIGALRM, alarm_handler);
    while (1)
    {

        // If the ready queue is not empty and no process is running, schedule a process to run
        if (!isEmpty(&scheduler) && global_process_num == 0)
        {
            run_process(front(&scheduler));
        }

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
            exit(0);
            break;

        // Case to create processes
        case 'c':

            numForC = stringToNumber(command);
            if (numForC >= 5 && numForC <= 10)
            {
                create_processes(numForC);
                printf("%d processes made\n", numForC);
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
            else if (num_processes == 0)
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

            if (strcmp(command, "rall") == 0)
            {
                // Schedule every process to run
                printf("Process already build in by default\n");
                break;
            }

            // Error handling
            if (strlen(command) > 1 && !(isdigit(command[1])))
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
            put_in_ready_state(numForR);

            break;

        case 's':
            if (strcmp(command, "sfcfs") == 0)
            {
                // Edge case for switching to fcfs
                if (global_process_num != 0)
                {

                    kill(global_process_num, SIGSTOP);
                    int i = findIndex(process_pids, num_processes, global_process_num);
                    processStates[i].ready = true;
                    processStates[i].running = false;
                    processStates[i].terminated = false;
                    processStates[i].suspended = false;
                    global_process_num = 0;
                }

                printf("Mode set to first come first serve\n");
                // How to switch to fcfs
                while (!isEmpty(&scheduler))
                {
                    dequeue(&scheduler);
                }
                for (int i = 0; i < num_processes; i++)
                {

                    enqueue(&scheduler, process_pids[i]);
                }

                fcfs = true;
                rrq = false;
                // Cancel any alarm
                alarm(0);
                break;
            }
            else if (strlen(command) > 3 && command[1] == 'r' && command[2] == 'r')
            {
                numForS = stringToNumberSRRQ(command);
                alarm(numForS);
                printf("Alarm set for %d seconds\n", numForS);
                rrq = true;
                fcfs = false;
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
            process_pids[num_processes] = pid; // Store the PID in the array to be able to use
            // Set state to ready
            processStates[num_processes].ready = true;
            processStates[num_processes].running = false;
            processStates[num_processes].terminated = false;
            processStates[num_processes].suspended = false;
            // Used to track the number of processes
            num_processes++;
            enqueue(&scheduler, pid);
            // Suspend the child process immediately
            kill(pid, SIGSTOP);
        }
    }
}
void alarm_handler(int signum)
{
    // For running processes while the alarm goes off
    if (global_process_num != 0)
    {
        // Stop the running process and put it in the back of the queue
        kill(global_process_num, SIGSTOP);
        int i = findIndex(process_pids, num_processes, global_process_num);
        processStates[i].ready = true;
        processStates[i].running = false;
        processStates[i].terminated = false;
        processStates[i].suspended = false;
        int top = dequeue(&scheduler);
        enqueue(&scheduler, top);
        global_process_num = 0;
        // Improves user experience
        run_process(front(&scheduler));
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
            printf("Process %d, %d is terminated\n", i + 1, arr[i]);
        }
        else if (processStates[i].suspended)
        {
            printf("Process %d, %d is suspended\n", i + 1, arr[i]);
        }
    }
    printf("Total number of processes %d\n", num_processes);
}

void kill_process(int process_num)
{
    // If the terminate is successful, update the values of the process
    int exist = findIndex(process_pids, num_processes, process_num);

    // Conditionals to check for a successful terminate
    if (exist != -1 && (processStates[exist].ready || processStates[exist].running || processStates[exist].suspended))
    {
        if (kill(process_num, SIGKILL) == 0)
        {
            // Edge case if you kill the running process
            if (global_process_num == process_num)
            {
                global_process_num = 0;
            }
            printf("Process ID %d killed successfully and state is now set to terminated.\n", process_num);
            int index = findIndex(process_pids, num_processes, process_num);
            processStates[index].ready = false;
            processStates[index].running = false;
            processStates[index].terminated = true;
            processStates[index].suspended = false;
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

void put_in_ready_state(int process_num)
{
    // If the terminate is successful, update the values of the process
    int exist = findIndex(process_pids, num_processes, process_num);

    if (exist != -1)
    {

        if (processStates[exist].suspended)
        {

            int index = findIndex(process_pids, num_processes, process_num);
            processStates[index].ready = true;
            processStates[index].running = false;
            processStates[index].terminated = false;
            processStates[index].suspended = false;
            enqueue(&scheduler, process_num);
            printf("Process ID %d is put back into ready queue\n", process_num);
        }
        else if (processStates[exist].terminated)
        {
            printf("Process %d was terminated, unable to resume process\n", process_num);
        }
        else if (processStates[exist].ready)
        {
            printf("Process %d is already in the ready queue\n", process_num);
        }
    }
    else
    {
        printf("Process %d does not exist, please try again\n", process_num);
    }
}

void run_process(int process_num)
{
    // Resumes process
    int exist = findIndex(process_pids, num_processes, process_num);
    // If the process exists and is also not terminated
    if (exist != -1 && !(processStates[exist].terminated || processStates[exist].suspended || processStates[exist].running))
    {
        if (kill(process_num, SIGCONT) == 0)
        {
            printf("Process ID %d resumed and state is now running.\n", process_num);
            processStates[exist].ready = false;
            processStates[exist].running = true;
            processStates[exist].terminated = false;
            processStates[exist].suspended = false;

            // Logic for when all processes are scheduled to run
            global_process_num = process_num;
        }
        else
        {
            printf("Error: Unable to resume process %d, process does not exist\n", process_num);
        }
    }
    // When all processes are running but a process in the queue is terminated or suspended, use recursion to skip it
    else if (processStates[exist].terminated)
    {
        dequeue(&scheduler);
        run_process(front(&scheduler));
    }
    else if (processStates[exist].suspended)
    {
        dequeue(&scheduler);
        run_process(front(&scheduler));
    }
    else
    {
        printf("No processes left in the ready queue, please resume a process to enable the scheduler\n");
    }
}

// Helper function for ctrl + c override
void sig_handle()
{
    if (global_process_num == 0)
    {
        return;
    }

    // If the process is running, then turn it back into the suspended state
    int top = dequeue(&scheduler);
    int i = findIndex(process_pids, num_processes, top);

    if (processStates[i].running)
    {
        processStates[i].running = false;
        processStates[i].ready = false;
        processStates[i].suspended = true;
        processStates[i].terminated = false;

        kill(process_pids[i], SIGSTOP);

        // Keeps track of the current process
        global_process_num = 0;
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

// Initialize the queue
void initQueue(Queue *q)
{
    q->front = -1;
    q->back = -1;
}

// Check if the queue is empty
int isEmpty(Queue *q)
{
    return q->back == -1;
}

// Add an element to the back of the queue
void enqueue(Queue *q, int value)
{

    if (isEmpty(q))
    {
        q->front = 0;
        q->back = 0;
    }
    else
    {
        q->back++;
    }
    q->items[q->back] = value;
}

// Remove an element from the front of the queue
int dequeue(Queue *q)
{
    int item;
    if (isEmpty(q))
    {
        printf("Queue is empty\n");
        return -1;
    }
    item = q->items[q->front];
    if (q->front == q->back)
    {
        q->front = -1;
        q->back = -1;
    }
    else
    {
        q->front++;
    }
    return item;
}

// Get the value of the front element of the queue without dequeuing
int front(Queue *q)
{
    if (isEmpty(q))
    {
        printf("Queue is empty.\n");
        return -1; // or any appropriate value indicating an error
    }
    return q->items[q->front];
}
