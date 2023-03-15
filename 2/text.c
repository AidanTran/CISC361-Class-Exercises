#include<stdio.h>
#include <stdbool.h>
 
int main()
{
    // Initialize variables
    int i = 0, num_processes = 0, total_time = 0, x, output_flag = 0,
time_quantum =0;
    int arrival_time[10], burst_time[10],
temp[10], pids[10];
    float average_wait_time = 0, average_turnaround_time = 0;
    bool round_robin_algorithm = true;
    for(i = 0; i < 10; i++)
    {
        arrival_time[i] = 0;
        burst_time[i] = 0;
        pids[i] = i;
        temp[i] = 0;
    }
     
    // Define Number of Processes
    while(num_processes < 1 || num_processes > 10)
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
    for(i = 0; i < num_processes; i++)
    {
        printf("\nEnter Details of Process[%d]\n", i + 1);
        printf("Arrival Time (>-1):\t");
        scanf("%d", &arrival_time[i]);
           
        printf("Burst Time (>0):\t");
        scanf("%d", &burst_time[i]);
        temp[i] = burst_time[i];
       
        if (arrival_time[i] < 0 && burst_time[i] < 1) {
            printf("Incorrect Values Entered");
            i--;
        }
    }
 
    // Define Quantum
    if (round_robin_algorithm)
    {
        while(time_quantum < 1)
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
    // Perform Scheduling Calculations
    if (!round_robin_algorithm)
    {
        for (int i = 1; i < num_processes; i++) {
            int j = i;
            while (j > 0 && arrival_time[j-1] > arrival_time[j]) {
                int temp = arrival_time[j];
                arrival_time[j] = arrival_time[j-1];
                arrival_time[j-1] = temp;
                temp = burst_time[j];
                burst_time[j] = burst_time[j-1];
                burst_time[j-1] = temp;
                temp = pids[j];
                pids[j] = pids[j-1];
                pids[j-1] = temp;
                j -= 1;
            }
        }

        wait_times[0] = 0;
        turnaround_time[0] = burst_time[0];
        for (int i = 1; i < num_processes; i++) {
            wait_times[i] = turnaround_time[i-1] + arrival_time[i-1] - arrival_time[i];
            if (wait_times[i] < 0) wait_times[i] = 0; // If process arrived after the termination of the one before it.
            turnaround_time[i] = burst_time[i] + wait_times[i];
        }


        for (int i = 0; i < num_processes; i++) {
            int j = pids[i];
            printf("\n%10d\t\t%12d\t\t%11d\t %15d\t %12d\n", i, arrival_time[j], burst_time[j], turnaround_time[j], wait_times[j]);
        }
    }
    else
    {
        int time = 0;
        int rIdx = 0;
        int qIdx = 0;
        int ready_queue[num_processes];
        for (int i = 0; i < num_processes; i++) {
            ready_queue[i] = -1;
        }
        
        while (true) {
            for (int i = 0; i < num_processes; i++) { // Quantum check
                if (arrival_time[i] <= time) {
                    ready_queue[rIdx] = i;
                    rIdx++;
                }
            }
            while (ready_queue[qIdx] == -1) {
                
            }
            time += time_quantum;
        }
       
    }
    // Calculate & Print Average Wait and Turnaround Times
    average_wait_time = 0;
    average_turnaround_time = 0;
    for (int i = 0; i < num_processes; i++) {
        average_turnaround_time += turnaround_time[i];
        average_wait_time += wait_times[i];
    }
    average_turnaround_time /= num_processes;
    average_wait_time /= num_processes;
    printf("\n\nAverage Waiting Time:\t%f", average_wait_time);
    printf("\nAvg Turnaround Time:\t%f\n", average_turnaround_time);
    return 0;
}