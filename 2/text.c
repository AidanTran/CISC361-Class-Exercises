#include <stdio.h>
#include <stdbool.h>

int main()
{
    // Initialize variables
    int i = 0, num_processes = 0, total_time = 0, x, output_flag = 0,
        time_quantum = 0;
    int arrival_time[10], burst_time[10],
        temp[10], pids[10];
    float average_wait_time = 0, average_turnaround_time = 0;
    bool round_robin_algorithm = true;
    for (i = 0; i < 10; i++)
    {
        arrival_time[i] = 0;
        burst_time[i] = 0;
        pids[i] = i;
        temp[i] = 0;
    }

    // Define Number of Processes
    while (num_processes < 1 || num_processes > 10)
    {
        printf("\nEnter Total Number of Processes (1 - 10):\t");
        scanf("%d", &num_processes);
        if (num_processes < 1 || num_processes > 10)
        {
            printf("Incorrect Value Entered");
        }
    }
    // Define Process Details
    x = num_processes;
    for (i = 0; i < num_processes; i++)
    {
        printf("\nEnter Details of Process[%d]\n", i + 1);
        printf("Arrival Time (>-1):\t");
        scanf("%d", &arrival_time[i]);

        printf("Burst Time (>0):\t");
        scanf("%d", &burst_time[i]);
        temp[i] = burst_time[i];

        if (arrival_time[i] < 0 && burst_time[i] < 1)
        {
            printf("Incorrect Values Entered");
            i--;
        }
    }

    // Define Quantum
    if (round_robin_algorithm)
    {
        while (time_quantum < 1)
        {
            printf("\nEnter Time Quantum (>0):\t");
            scanf("%d", &time_quantum);
            if (time_quantum < 1)
            {
                printf("Incorrect Value Entered");
            }
        }
    }
    // Prepare Output
    printf("\nProcess ID\t\tArrival Time\t\tBurst Time\t Turnaround Time\t Waiting Time\n");

    int wait_times[num_processes];
    int turnaround_time[num_processes];
    for (int i = 0; i < num_processes; i++)
    {
        wait_times[i] = 0;
        turnaround_time[i] = 0;
    }

    // Sort by arrival time.
    for (int i = 1; i < num_processes; i++)
    {
        int j = i;
        while (j > 0 && arrival_time[j - 1] > arrival_time[j])
        {
            int temp = arrival_time[j];
            arrival_time[j] = arrival_time[j - 1];
            arrival_time[j - 1] = temp;
            temp = burst_time[j];
            burst_time[j] = burst_time[j - 1];
            burst_time[j - 1] = temp;
            temp = pids[j];
            pids[j] = pids[j - 1];
            pids[j - 1] = temp;
            j -= 1;
        }
    }
    // Perform Scheduling Calculations
    if (!round_robin_algorithm)
    {
        wait_times[0] = 0;
        turnaround_time[0] = burst_time[0];
        for (int i = 1; i < num_processes; i++)
        {
            wait_times[i] = turnaround_time[i - 1] + arrival_time[i - 1] - arrival_time[i];
            if (wait_times[i] < 0)
                wait_times[i] = 0; // If process arrived after the termination of the one before it.
            turnaround_time[i] = burst_time[i] + wait_times[i];
        }

        for (int i = 0; i < num_processes; i++)
        {
            int j = pids[i];
            printf("\n%10d\t\t%12d\t\t%11d\t %15d\t %12d\n", i + 1, arrival_time[j], burst_time[j], turnaround_time[j], wait_times[j]);
        }
    }
    else
    {
        int curr_time = 0;
        int temp_burst_time[num_processes];
        for (int i = 0; i < num_processes; i++)
        {
            temp_burst_time[i] = burst_time[i];
        }
        while (true)
        {
            int min_arrival_time = __INT_MAX__;
            for (int i = 0; i < num_processes; i++)
            {
                // printf("Evaluating procces %d curr time = %d\n", i, curr_time);
                if (arrival_time[i] < min_arrival_time) // Update the min arrival time
                    min_arrival_time = arrival_time[i];
                if (curr_time >= arrival_time[i] && temp_burst_time[i] > 0) // If a process has yet to be completed, and it is valid due to arrival time
                {
                    if (time_quantum < temp_burst_time[i]) // If the process has more time left than the quantum
                    {
                        for (int j = 0; j < num_processes; j++)
                        {
                            // update the wait times for all active but not worked on processes.
                            if (j != i && (curr_time + time_quantum) >= arrival_time[j] && temp_burst_time[j] > 0)
                            {
                                if (curr_time < arrival_time[j])
                                {
                                    // printf("1 adding wait time to process %d, %d\n", j, (curr_time + time_quantum) - arrival_time[j]);
                                    wait_times[j] += (curr_time + time_quantum) - arrival_time[j];
                                }
                                else
                                {
                                    // printf("2 adding wait time to process %d, %d\n", j, time_quantum);
                                    wait_times[j] += time_quantum;
                                }
                            }
                        }
                        temp_burst_time[i] -= time_quantum; // Remove a quantum amount of time
                        curr_time += time_quantum;          // Update the current time
                    }
                    else // If the quantum would finish off the process burst.
                    {
                        for (int j = 0; j < num_processes; j++)
                        {
                            // update the wait times for all active but not worked on processes.
                            if (j != i && (curr_time + temp_burst_time[i]) >= arrival_time[j] && temp_burst_time[j] > 0)
                            {
                                if (curr_time < arrival_time[j])
                                {
                                    // printf("3 adding wait time to process %d, %d\n", i, (curr_time + temp_burst_time[i]) - arrival_time[i]);
                                    wait_times[j] += (curr_time + temp_burst_time[i]) - arrival_time[j];
                                }
                                else
                                {
                                    // printf("4 adding wait time to process %d, %d\n", i, temp_burst_time[i]);
                                    wait_times[j] += temp_burst_time[i];
                                }
                            }
                        }
                        curr_time += temp_burst_time[i];
                        temp_burst_time[i] = 0;
                        turnaround_time[i] = curr_time - arrival_time[i]; // Update the processes turnaround time.
                    }
                }
            }
            if (curr_time < min_arrival_time)
                curr_time = min_arrival_time;

            bool break_flag = true;
            for (int i = 0; i < num_processes; i++)
            {
                if (temp_burst_time[i] > 0)
                {
                    break_flag = false;
                    break;
                }
            }
            if (break_flag)
            {
                break;
            }
        }
        for (int i = 0; i < num_processes; i++)
        {
            int j = pids[i];
            printf("\n%10d\t\t%12d\t\t%11d\t %15d\t %12d\n", i + 1, arrival_time[j], burst_time[j], turnaround_time[j], wait_times[j]);
        }
    }
    // Calculate & Print Average Wait and Turnaround Times
    average_wait_time = 0;
    average_turnaround_time = 0;
    for (int i = 0; i < num_processes; i++)
    {
        average_turnaround_time += turnaround_time[i];
        average_wait_time += wait_times[i];
    }
    average_turnaround_time /= num_processes;
    average_wait_time /= num_processes;
    printf("\n\nAverage Waiting Time:\t%f", average_wait_time);
    printf("\nAvg Turnaround Time:\t%f\n", average_turnaround_time);
    return 0;
}