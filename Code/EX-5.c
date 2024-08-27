#include <stdio.h>

struct Process {
    int pid;                // Process ID
    int burst_time;         // Burst time
    int priority;           // Priority
    int wait_time;          // Waiting time
    int turn_around_time;   // Turnaround time
    int completion_time;    // Completion time
    int response_time;      // Response time
    int remaining_time;     // Remaining time (only for preemptive scheduling)
    int start_time;         // Start time (for response time calculation)
    int arrival_time;       // Arrival time
};

void nonPreemptivePriorityScheduling(struct Process proc[], int n) {
    int i, j;
    struct Process temp;

    // Sort processes based on arrival time
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (proc[i].arrival_time > proc[j].arrival_time) {
                temp = proc[i];
                proc[i] = proc[j];
                proc[j] = temp;
            }
        }
    }

    // Sort by priority among processes that have arrived
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (proc[i].priority > proc[j].priority && proc[i].completion_time <= proc[j].arrival_time) {
                temp = proc[i];
                proc[i] = proc[j];
                proc[j] = temp;
            }
        }
    }

    proc[0].completion_time = proc[0].arrival_time + proc[0].burst_time;
    proc[0].turn_around_time = proc[0].completion_time - proc[0].arrival_time;
    proc[0].wait_time = proc[0].turn_around_time - proc[0].burst_time;
    proc[0].response_time = proc[0].arrival_time;

    for (i = 1; i < n; i++) {
        proc[i].completion_time = proc[i-1].completion_time + proc[i].burst_time;
        proc[i].turn_around_time = proc[i].completion_time - proc[i].arrival_time;
        proc[i].wait_time = proc[i].turn_around_time - proc[i].burst_time;
        proc[i].response_time = proc[i].completion_time - proc[i].burst_time;
    }

    printf("Non-Preemptive Priority Scheduling:\n");
    printf("PID\tBurst Time\tPriority\tArrival Time\tCompletion Time\tTurnaround Time\tWaiting Time\tResponse Time\n");
    for (i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", proc[i].pid, proc[i].burst_time,
               proc[i].priority, proc[i].arrival_time, proc[i].completion_time, proc[i].turn_around_time,
               proc[i].wait_time, proc[i].response_time);
    }
}

void preemptivePriorityScheduling(struct Process proc[], int n) {
    int time = 0, completed = 0, i, min_priority;
    int shortest = 0, finish_time;
    int all_arrived = 0;

    for (i = 0; i < n; i++) {
        proc[i].remaining_time = proc[i].burst_time;
        proc[i].start_time = -1;
    }

    while (completed != n) {
        min_priority = 9999;
        shortest = -1;
        all_arrived = 0;

        for (i = 0; i < n; i++) {
            if (proc[i].arrival_time <= time) {
                all_arrived = 1;
                if (proc[i].remaining_time > 0 && proc[i].priority < min_priority) {
                    min_priority = proc[i].priority;
                    shortest = i;
                }
            }
        }

        if (!all_arrived) {
            time++;
            continue;
        }

        if (shortest == -1) {
            time++;
            continue;
        }

        if (proc[shortest].start_time == -1) {
            proc[shortest].start_time = time;
            proc[shortest].response_time = time - proc[shortest].arrival_time;
        }

        proc[shortest].remaining_time--;
        time++;

        if (proc[shortest].remaining_time == 0) {
            completed++;
            finish_time = time;
            proc[shortest].completion_time = finish_time;
            proc[shortest].turn_around_time = proc[shortest].completion_time - proc[shortest].arrival_time;
            proc[shortest].wait_time = proc[shortest].turn_around_time - proc[shortest].burst_time;
        }
    }

    printf("Preemptive Priority Scheduling:\n");
    printf("PID\tBurst Time\tPriority\tArrival Time\tCompletion Time\tTurnaround Time\tWaiting Time\tResponse Time\n");
    for (i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", proc[i].pid, proc[i].burst_time,
               proc[i].priority, proc[i].arrival_time, proc[i].completion_time, proc[i].turn_around_time,
               proc[i].wait_time, proc[i].response_time);
    }
}

void roundRobinScheduling(struct Process proc[], int n, int quantum) {
    int time = 0, i, remaining = n;
    int process_count = 0;

    for (i = 0; i < n; i++) {
        proc[i].remaining_time = proc[i].burst_time;
        proc[i].start_time = -1;
    }

    while (remaining != 0) {
        process_count = 0;
        for (i = 0; i < n; i++) {
            if (proc[i].arrival_time <= time && proc[i].remaining_time > 0) {
                process_count = 1;
                if (proc[i].start_time == -1) {
                    proc[i].start_time = time;
                    proc[i].response_time = time - proc[i].arrival_time;
                }

                if (proc[i].remaining_time > quantum) {
                    time += quantum;
                    proc[i].remaining_time -= quantum;
                } else {
                    time += proc[i].remaining_time;
                    proc[i].completion_time = time;
                    proc[i].turn_around_time = proc[i].completion_time - proc[i].arrival_time;
                    proc[i].wait_time = proc[i].turn_around_time - proc[i].burst_time;
                    proc[i].remaining_time = 0;
                    remaining--;
                }
            }
        }
        if (process_count == 0) {
            time++;
        }
    }

    printf("Round Robin Scheduling:\n");
    printf("PID\tBurst Time\tArrival Time\tCompletion Time\tTurnaround Time\tWaiting Time\tResponse Time\n");
    for (i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", proc[i].pid, proc[i].burst_time,
               proc[i].arrival_time, proc[i].completion_time, proc[i].turn_around_time,
               proc[i].wait_time, proc[i].response_time);
    }
}

int main() {
    int n, i, quantum, choice, type;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process proc[n];

    printf("Choose the Scheduling Algorithm:\n");
    printf("1. Priority Scheduling\n");
    printf("2. Round Robin Scheduling\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    for (i = 0; i < n; i++) {
        proc[i].pid = i + 1;
        printf("Enter burst time for process %d: ", proc[i].pid);
        scanf("%d", &proc[i].burst_time);
        printf("Enter arrival time for process %d: ", proc[i].pid);
        scanf("%d", &proc[i].arrival_time);
        if (choice == 1) {
            printf("Enter priority for process %d: ", proc[i].pid);
            scanf("%d", &proc[i].priority);
        }
    }

    if (choice == 1) {
        printf("Choose the type of Priority Scheduling:\n");
        printf("1. Non-Preemptive\n");
        printf("2. Preemptive\n");
        printf("Enter your choice: ");
        scanf("%d", &type);
        if (type == 1) {
            nonPreemptivePriorityScheduling(proc, n);
        } else if (type == 2) {
            preemptivePriorityScheduling(proc, n);
        } else {
            printf("Invalid choice!\n");
        }
    } else if (choice == 2) {
        printf("Enter the time quantum for Round Robin: ");
        scanf("%d", &quantum);
        roundRobinScheduling(proc, n, quantum);
    } else {
        printf("Invalid choice!\n");
    }

    return 0;
}
