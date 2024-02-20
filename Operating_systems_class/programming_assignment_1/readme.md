# Instructions to run project

1. Download main.c, task.c, and Makefile and put all files into one folder
2. With all the files in the same directory, run the command
   ```bash
   make
   ```
   in the terminal with the terminal inside of the directory where all the files are stored.
3. There will now be 2 binary files, `main` and `task`, do not run task as that is the binary files for child processes, instead run the command
   ```bash
   ./main
   ```
   to start the project

## Design for the project

My design for the project uses 2 arrays as the main reference for all commands in the project. The first array is on for all the processes ID's sorted in ascending order of their process ID. The second is an array for each processes state. I created a struct that has 4 properties, ready, running, suspended, and terminated. I then created the second array as an array of struct states. Each struct of state is referenced by their process ID. With the index of every element the unique value I used to cross reference the 2 arrays. This allowed me to have an easy way to keep track of all states and process ID's to manipulate through out the project so whenever any process related information changed, these 2 arrays were updated by finding the index of said process and with that, I had access to all the information needed to change in the process.

The whole project revolves around a queue data structure designed to keep track of all order and algorithms.

I used global variables for flags, counters, and constants through the project, such as the max number of processes.

The program runs in 1 infinite while loop that is always listening for commands from the user, the shell is on even as child processes are running. In order for this to happen, all commands needed to be implemented in a non-blocking way to go back to the infinite while loop. Therefore, conditionals and flags were favored instead of using nested while true loops. Additionally, because the the user needs to input a command, the prompt is only displayed once and the process text will make the commands input scroll up the screen, the user will just need to type quickly, all commands will function properly

For child processes, fork() and exec() system calls were used to run the binary file for task and they were suspended with a signal immediately.

Signals were used for all processes control when actions needed to happen to them.

The necessary print statements were chained with each command or action to make the user's experience seeable in the shell. Additionally, this is also true for error handling, every command and action the user can input in the shell has it's necessary error handling statements and logic to prevent commands not mentioned in the requirements for the project. Each command's main logic was split into its own function to provide readability for the code.

## Running experiments and commands

Below is walkthrough of the experiments and commands for the project. All these commands can be ran in the shell, note case sensitivity does not matter

1. `x`: Terminates and ends all processes.

   ```bash
   anthony@DESKTOP-VPHMSL1:~/AllCode/CSI-2320-C++/Operating_systems_class/programming_assignment_1 (master)$ ./main
   shell 5500>>>c5
   Process 95950 at iteration 0
   5 processes made
   Process ID 95950 resumed and state is now running.
   shell 5500>>>l
   Process IDs:
   Process 95950 is in the running state
   Process 95951 is in the ready state
   Process 95952 is in the ready state
   Process 95953 is in the ready state
   Process 95954 is in the ready state
   Total number of processes 5
   shell 5500>>>Process 95950 at iteration 1
   xProcess 95950 at iteration 2

   Killed
   ```

2. `cn`: Creates `n` processes and suspends them immediately, they are all made from the task binary file. `n` can only be from 5-10 inclusive
   ```bash
   anthony@DESKTOP-VPHMSL1:~/AllCode/CSI-2320-C++/Operating_systems_class/programming_assignment_1 (master)$ ./main
   shell 5500>>>c5
   Process 96354 at iteration 0
   5 processes made
   ```
3. `Ctrl + c`: Suspends the running child process, and puts it in the suspended state. If used without one running, command has no effect. As shown below, control is only on the shell after ctrl + c is used.

   ```bash
   anthony@DESKTOP-VPHMSL1:~/AllCode/CSI-2320-C++/Operating_systems_class/programming_assignment_1 (master)$ ./main
   shell 5500>>>c5
   Process 98397 at iteration 0
   5 processes made
   Process ID 98397 resumed and state is now running.
   shell 5500>>>l
   Process IDs:
   Process 1, 98397 is in the running state
   Process 2, 98398 is in the ready state
   Process 3, 98399 is in the ready state
   Process 4, 98400 is in the ready state
   Process 5, 98401 is in the ready state
   Total number of processes 5
   shell 5500>>>Process 98397 at iteration 1
   Process 98397 at iteration 2
   Process 98397 at iteration 3
   Process 98397 at iteration 4
   ^Cl
   Process IDs:
   Process 1, 98397 is suspended
   Process 2, 98398 is in the ready state
   Process 3, 98399 is in the ready state
   Process 4, 98400 is in the ready state
   Process 5, 98401 is in the ready state
   Total number of processes 5
   Process ID 98398 resumed and state is now running.
   shell 5500>>>Process 98398 at iteration 0
   ```

4. `l`: List the current user processes in the system including it PID (underlying process ID from the host OS - Ubuntu)

   ```bash
   Process IDs:
   Process 1, 98397 is suspended
   Process 2, 98398 is in the running state
   Process 3, 98399 is in the ready state
   Process 4, 98400 is in the ready state
   Process 5, 98401 is in the ready state
   ```

5. `srrq`: Set the mode to be round robin where each process running will be suspended after `q` seconds. As shown in the sample below, after setting the alarm for 7 seconds and running all processes, without input from the user, each process is suspended after 7 seconds, where 7 is what is input by the user. The order of executing processes for this mode are by ID's.

   ```bash
   anthony@DESKTOP-VPHMSL1:~/AllCode/CSI-2320-C++/Operating_systems_class/programming_assignment_1 (master)$ ./main
   shell 5500>>>srr10
   Alarm set for 10 seconds
   shell 5500>>>c5
   5 processes made
   Process ID 99857 resumed and state is now running.
   shell 5500>>>Process 99857 at iteration 0
   Process 99857 at iteration 1
   Process 99857 at iteration 2
   Process 99857 at iteration 3
   Process 99857 at iteration 4
   Process 99857 at iteration 5
   Process 99857 at iteration 6
   Process 99857 at iteration 7
   Process ID 99858 resumed and state is now running.
   Process 99858 at iteration 0
   Process 99858 at iteration 1
   Process 99858 at iteration 2
   Process 99858 at iteration 3
   Process 99858 at iteration 4
   Process 99858 at iteration 5
   Process 99858 at iteration 6
   Process 99858 at iteration 7
   Process 99858 at iteration 8
   Process 99858 at iteration 9
   Process ID 99859 resumed and state is now running.
   Process 99859 at iteration 0
   Process 99859 at iteration 1
   l
   Process IDs:
   Process 1, 99857 is in the ready state
   Process 2, 99858 is in the ready state
   Process 3, 99859 is in the running state
   Process 4, 99860 is in the ready state
   Process 5, 99861 is in the ready state
   Total number of processes 5
   ```

6. `sfcfs`: Set the scheduler to be first come first serve, the process number determines its arrival order. When running all the processes, their process ID will always determine their order of execution. Default mode of the shell and ctrl+c or k are the only ways to stop a child process as shown below

   ```bash
      anthony@DESKTOP-VPHMSL1:~/AllCode/CSI-2320-C++/Operating_systems_class/programming_assignment_1 (master)$ ./main
      shell 5500>>>sfcfs
      Mode set to first come first serve
      shell 5500>>>c5
      5 processes made
      Process ID 101235 resumed and state is now running.
      shell 5500>>>Process 101235 at iteration 0
      Process 101235 at iteration 1
      lProcess 101235 at iteration 2
      Process IDs:
      Process 1, 101235 is in the running state
      Process 2, 101236 is in the ready state
      Process 3, 101237 is in the ready state
      Process 4, 101238 is in the ready state
      Process 5, 101239 is in the ready state
      Total number of processes 5
      shell 5500>>>Process 101235 at iteration 3
      Process 101235 at iteration 4
      Process 101235 at iteration 5
      ^Cl
      Process IDs:
      Process 1, 101235 is suspended
      Process 2, 101236 is in the ready state
      Process 3, 101237 is in the ready state
      Process 4, 101238 is in the ready state
      Process 5, 101239 is in the ready state
      Total number of processes 5
      Process ID 101236 resumed and state is now running.
      shell 5500>>>Process 101236 at iteration 0
      Process 101236 at iteration 1
      Process 101236 at iteration 2
      Process 101236 at iteration 3
   ```

7. `k####`: Terminated the process id after `k`, returns an error message if process ID is not found, the terminated process is no longer resumable for execution as shown below,

   ```bash

      anthony@DESKTOP-VPHMSL1:~/AllCode/CSI-2320-C++/Operating_systems_class/programming_assignment_1 (master)$ ./main
      shell 5500>>>c5
      5 processes made
      shell 5500>>>l
      Process IDs:
      Process 1, 91429 is in the ready state
      Process 2, 91430 is in the ready state
      Process 3, 91431 is in the ready state
      Process 4, 91432 is in the ready state
      Process 5, 91433 is in the ready state
      Total number of processes 5
      shell 5500>>>k91429
      Process ID 91429 killed successfully and state is now set to terminated.
      shell 5500>>>l
      Process IDs:
      Process 1, 91429 is now terminated
      Process 2, 91430 is in the ready state
      Process 3, 91431 is in the ready state
      Process 4, 91432 is in the ready state
      Process 5, 91433 is in the ready state
   ```

8. `r####`: Similar coding logic as `k` except this command resume's a specific process that is in the suspended state and sets it to ready.

   ```bash
      Process IDs:
      Process 1, 101792 is suspended
      Process 2, 101793 is suspended
      Process 3, 101794 is suspended
      Process 4, 101795 is suspended
      Process 5, 101796 is suspended
      Total number of processes 5
      shell 5500>>>r101792
      Process ID 101792 is put back into ready queue
      Process ID 101792 resumed and state is now running.
      shell 5500>>>Process 101792 at iteration 2
      Process 101792 at iteration 3
      Process 101792 at iteration 4
      Process 101792 at iteration 5
      Process 101792 at iteration 6
      l
      Process IDs:
      Process 1, 101792 is in the running state
      Process 2, 101793 is suspended
      Process 3, 101794 is suspended
      Process 4, 101795 is suspended
      Process 5, 101796 is suspended
      Total number of processes 5
   ```

9. `rall`: Process is built in by default in the scheduler, not needed in my design
