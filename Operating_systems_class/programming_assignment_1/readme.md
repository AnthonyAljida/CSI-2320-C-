# Instructions to run project

1. Download main.c, task.c, and Makefile and put all files into one folder
2. With all the files in the same directory, run the command
   ```bash
   make
   ```
   in the terminal with the terminal inside of the directory where all the files are stored.
3. There will now be 2 binary files, `main` and `task`, do not run task as that is the binary files for chile processes, instead run the command
   ```bash
   ./main
   ```
   to start the project

## Design for the project

My design for the project uses 2 arrays as the main reference for all commands in the project. The first array is on for all the processes ID's sorted in ascending order of their process ID. The second is an array for each processes state. I created a struct that has 3 properties, ready, running, and terminated. I then created the second array as an array of struct states. Each struct of state is referenced by their process ID. With the index of every element the unique value I used to cross reference the 2 arrays. This allowed me to have an easy way to keep track of all states and process ID's to manipulate through out the project so whenever any process related information changed, these 2 arrays were updated by finding the index of said process and with that, I had access to all the information needed to change in the process.

I used global variables for flags, counters, and constants through the project, such as the max number of processes which is 10.

The program runs in 1 infinite while loop that is always listening for commands from the user, the shell is on even as child processes are running. In order for this to happen, all commands needed to be implemented in a non-blocking way to go back to the infinite while loop. Therefore, conditionals and flags were favored instead of using nested while true loops.

For child processes, fork() and exec() system calls were used to run the binary file for task and they were suspended with a signal immediately.

Signals were used for all processes control when actions needed to happen to them.

The necessary print statements were chained with each command or action to make the user's experience seeable in the shell. Additionally, this is also true for error handling, every command and action the user can input in the shell has it's necessary error handling statements and logic to prevent commands not mentioned in the requirements for the project. Each command's main logic was split into its own function to provide readability for the code.

## Running experiments and commands

Below is walkthrough of the experiments and commands for the project. All these commands can be ran in the shell, note case sensitivity does not matter

1. `x`: Terminates and ends all processes.
   ```bash
   anthony@DESKTOP-VPHMSL1:~/AllCode/CSI-2320-C++/Operating_systems_class/programming_assignment_1 (master)$ ./main
   shell 5500>>>c5
   5 processes made
   shell 5500>>>l
   Process IDs:
   Process 1, 83592 is in the ready state
   Process 2, 83593 is in the ready state
   Process 3, 83594 is in the ready state
   Process 4, 83595 is in the ready state
   Process 5, 83596 is in the ready state
   Total number of processes 5
   shell 5500>>>x
   Killed
   anthony@DESKTOP-VPHMSL1:~/AllCode/CSI-2320-C++/Operating_systems_class/programming_assignment_1 (master)$
   ```
2. `cn`: Creates `n` processes and suspends them immediately, they are all made from the task binary file. `n` can only be from 5-10 inclusive
   ```bash
   anthony@DESKTOP-VPHMSL1:~/AllCode/CSI-2320-C++/Operating_systems_class/programming_assignment_1 (master)$ ./main
   shell 5500>>>c9
   9 processes made
   shell 5500>>>l
   Process IDs:
   Process 1, 84343 is in the ready state
   Process 2, 84344 is in the ready state
   Process 3, 84345 is in the ready state
   Process 4, 84346 is in the ready state
   Process 5, 84347 is in the ready state
   Process 6, 84348 is in the ready state
   Process 7, 84349 is in the ready state
   Process 8, 84350 is in the ready state
   Process 9, 84351 is in the ready state
   Total number of processes 9
   shell 5500>>>
   ```
3. `Ctrl + c`: Suspends the running child process, and puts it in the ready state. If used without one running, command has no effect. As shown below, control is only on the shell after ctrl + c is used.

   ```bash
   anthony@DESKTOP-VPHMSL1:~/AllCode/CSI-2320-C++/Operating_systems_class/programming_assignment_1 (master)$ ./main
   shell 5500>>>c8
   8 processes made
   shell 5500>>>l
   Process IDs:
   Process 1, 85326 is in the ready state
   Process 2, 85327 is in the ready state
   Process 3, 85328 is in the ready state
   Process 4, 85329 is in the ready state
   Process 5, 85330 is in the ready state
   Process 6, 85331 is in the ready state
   Process 7, 85332 is in the ready state
   Process 8, 85333 is in the ready state
   Total number of processes 8
   shell 5500>>>r85326
   Process ID 85326 resumed and state is now running.
   shell 5500>>>Process 85326 at iteration 0
   Process 85326 at iteration 1
   Process 85326 at iteration 2
   Process 85326 at iteration 3
   Process 85326 at iteration 4
   Process 85326 at iteration 5
   Process 85326 at iteration 6
   Process 85326 at iteration 7
   Process 85326 at iteration 8
   Process 85326 at iteration 9
   ^Cl
   Process IDs:
   Process 1, 85326 is in the ready state
   Process 2, 85327 is in the ready state
   Process 3, 85328 is in the ready state
   Process 4, 85329 is in the ready state
   Process 5, 85330 is in the ready state
   Process 6, 85331 is in the ready state
   Process 7, 85332 is in the ready state
   Process 8, 85333 is in the ready state
   Total number of processes 8
   shell 5500>>>
   ```

4. `l`: List the current user processes in the system including it PID (underlying process ID from the host OS - Ubuntu)

   ```bash
   anthony@DESKTOP-VPHMSL1:~/AllCode/CSI-2320-C++/Operating_systems_class/programming_assignment_1 (master)$ ./main
   shell 5500>>>c7
   7 processes made
   shell 5500>>>l
   Process IDs:
   Process 1, 86210 is in the ready state
   Process 2, 86211 is in the ready state
   Process 3, 86212 is in the ready state
   Process 4, 86213 is in the ready state
   Process 5, 86214 is in the ready state
   Process 6, 86215 is in the ready state
   Process 7, 86216 is in the ready state
   Total number of processes 7
   shell 5500>>>
   ```

5. `srrq`: Set the mode to be round robin where each process running will be suspended after `q` seconds. As shown in the sample below, after setting the alarm for 7 seconds and running all processes, without input from the user, each process is suspended after 7 seconds, where 7 is what is input by the user. The order of executing processes for this mode are by ID's.

   ```bash
   anthony@DESKTOP-VPHMSL1:~/AllCode/CSI-2320-C++/Operating_systems_class/programming_assignment_1 (master)$ ./main
   shell 5500>>>c7
   7 processes made
   shell 5500>>>l
   Process IDs:
   Process 1, 86601 is in the ready state
   Process 2, 86602 is in the ready state
   Process 3, 86603 is in the ready state
   Process 4, 86604 is in the ready state
   Process 5, 86605 is in the ready state
   Process 6, 86606 is in the ready state
   Process 7, 86607 is in the ready state
   Total number of processes 7
   shell 5500>>>srr7
   Alarm set for 7 seconds
   shell 5500>>>rall
   Process ID 86601 resumed and state is now running.
   shell 5500>>>Process 86601 at iteration 0
   Process 86601 at iteration 1
   Process 86601 at iteration 2
   Process ID 86602 resumed and state is now running.
   Process 86602 at iteration 0
   Process 86602 at iteration 1
   Process 86602 at iteration 2
   Process 86602 at iteration 3
   Process 86602 at iteration 4
   Process 86602 at iteration 5
   Process 86602 at iteration 6
   Process ID 86603 resumed and state is now running.
   Process 86603 at iteration 0
   Process 86603 at iteration 1
   Process 86603 at iteration 2
   Process 86603 at iteration 3
   Process 86603 at iteration 4
   Process 86603 at iteration 5
   Process 86603 at iteration 6
   Process ID 86604 resumed and state is now running.
   Process 86604 at iteration 0
   Process 86604 at iteration 1
   Process 86604 at iteration 2
   Process 86604 at iteration 3
   Process 86604 at iteration 4
   Process 86604 at iteration 5
   Process 86604 at iteration 6
   Process ID 86605 resumed and state is now running.
   Process 86605 at iteration 0
   Process 86605 at iteration 1
   Process 86605 at iteration 2
   ```

6. `sfcfs`: Set the scheduler to be first come first serve, the process number determines its arrival order. When running all the processes, their process ID will always determine their order of execution. Default mode of the shell and ctrl+c or k are the only ways to stop a child process as shown below

```bash
anthony@DESKTOP-VPHMSL1:~/AllCode/CSI-2320-C++/Operating_systems_class/programming_assignment_1 (master)$ ./main
shell 5500>>>c5
5 processes made
shell 5500>>>l
Process IDs:
Process 1, 90416 is in the ready state
Process 2, 90417 is in the ready state
Process 3, 90418 is in the ready state
Process 4, 90419 is in the ready state
Process 5, 90420 is in the ready state
Total number of processes 5
shell 5500>>>sfcfs
Mode set to first come first serve
shell 5500>>>rall
Process ID 90416 resumed and state is now running.
shell 5500>>>Process 90416 at iteration 0
Process 90416 at iteration 1
Process 90416 at iteration 2
Process 90416 at iteration 3
Process 90416 at iteration 4
Process 90416 at iteration 5
Process 90416 at iteration 6
Process 90416 at iteration 7
Process 90416 at iteration 8
Process 90416 at iteration 9
Process 90416 at iteration 10
Process 90416 at iteration 11
Process 90416 at iteration 12
Process 90416 at iteration 13
Process 90416 at iteration 14
Process 90416 at iteration 15
Process 90416 at iteration 16
Process 90416 at iteration 17
Process 90416 at iteration 18
Process 90416 at iteration 19
^CProcess ID 90417 resumed and state is now running.
Process 90417 at iteration 0
Process 90417 at iteration 1
Process 90417 at iteration 2
Process 90417 at iteration 3
Process 90417 at iteration 4
Process 90417 at iteration 5
Process 90417 at iteration 6
Process 90417 at iteration 7
Process 90417 at iteration 8
Process 90417 at iteration 9
x
Killed
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
   Total number of processes 5
   shell 5500>>>rall
   Process ID 91430 resumed and state is now running.
   shell 5500>>>Process 91430 at iteration 0
   Process 91430 at iteration 1
   Process 91430 at iteration 2
   Process 91430 at iteration 3
   Process 91430 at iteration 4
   Process 91430 at iteration 5
   Process 91430 at iteration 6
   Process 91430 at iteration 7
   Process 91430 at iteration 8
   ```

8. `r####`: Similar coding logic as `k` except this command resume's a specific process that is in the ready state and sets it to running.

```bash
anthony@DESKTOP-VPHMSL1:~/AllCode/CSI-2320-C++/Operating_systems_class/programming_assignment_1 (master)$ ./main
shell 5500>>>c5
5 processes made
shell 5500>>>l
Process IDs:
Process 1, 92062 is in the ready state
Process 2, 92063 is in the ready state
Process 3, 92064 is in the ready state
Process 4, 92065 is in the ready state
Process 5, 92066 is in the ready state
Total number of processes 5
shell 5500>>>r92062
Process ID 92062 resumed and state is now running.
shell 5500>>>Process 92062 at iteration 0
Process 92062 at iteration 1
Process 92062 at iteration 2
Process 92062 at iteration 3
Process 92062 at iteration 4
Process 92062 at iteration 5
Process 92062 at iteration 6
l
Process IDs:
Process 1, 92062 is in the running state
Process 2, 92063 is in the ready state
Process 3, 92064 is in the ready state
Process 4, 92065 is in the ready state
Process 5, 92066 is in the ready state
Total number of processes 5
shell 5500>>>Process 92062 at iteration 7
Process 92062 at iteration 8
Process 92062 at iteration 9
Process 92062 at iteration 10
Process 92062 at iteration 11
^Cl
Process IDs:
Process 1, 92062 is in the ready state
Process 2, 92063 is in the ready state
Process 3, 92064 is in the ready state
Process 4, 92065 is in the ready state
Process 5, 92066 is in the ready state
Total number of processes 5
shell 5500>>>
```

9. `rall`: Run all processes after the mode is set for the scheduler, terminated processes are skipped and all processes will be ran in the ready state and once all have ran, control is back to only the shell. Below is an example output using the srrq mode, all processes are run once then control is back to the shell.

```bash
aanthony@DESKTOP-VPHMSL1:~/AllCode/CSI-2320-C++/Operating_systems_class/programming_assignment_1 (master)$ ./main
shell 5500>>>c5
5 processes made
shell 5500>>>l
Process IDs:
Process 1, 92693 is in the ready state
Process 2, 92694 is in the ready state
Process 3, 92695 is in the ready state
Process 4, 92696 is in the ready state
Process 5, 92697 is in the ready state
Total number of processes 5
shell 5500>>>srr5
Alarm set for 5 seconds
shell 5500>>>rall
Process ID 92693 resumed and state is now running.
shell 5500>>>Process 92693 at iteration 0
Process 92693 at iteration 1
Process 92693 at iteration 2
Process 92693 at iteration 3
Process ID 92694 resumed and state is now running.
Process 92694 at iteration 0
Process 92694 at iteration 1
Process 92694 at iteration 2
Process 92694 at iteration 3
Process 92694 at iteration 4
Process ID 92695 resumed and state is now running.
Process 92695 at iteration 0
Process 92695 at iteration 1
Process 92695 at iteration 2
Process 92695 at iteration 3
Process 92695 at iteration 4
Process ID 92696 resumed and state is now running.
Process 92696 at iteration 0
Process 92696 at iteration 1
Process 92696 at iteration 2
Process 92696 at iteration 3
Process 92696 at iteration 4
Process ID 92697 resumed and state is now running.
Process 92697 at iteration 0
Process 92697 at iteration 1
Process 92697 at iteration 2
Process 92697 at iteration 3
Process 92697 at iteration 4
l
Process IDs:
Process 1, 92693 is in the ready state
Process 2, 92694 is in the ready state
Process 3, 92695 is in the ready state
Process 4, 92696 is in the ready state
Process 5, 92697 is in the ready state
Total number of processes 5
shell 5500>>>
```
