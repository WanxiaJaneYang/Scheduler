# Scheduler
This is a customised CPU scheduler algorithm<br />
The target is to balance the total waiting time, priority customer waiting time, longest response and switches.<br />
The compute_stats is used to calculate the metric data<br />
data_1111, data_2222, and data_3333 are input data for testing<br />

## Input data format
c00 0 6 23: <br />customer 01, VIP customer, arrives at time 6, and need to use 23-time slots

## metrics
total_wait0: total waiting time for VIP customers<br />
total_wait1: total waiting time for normal customers<br />
longest_response: longest waiting time to get the service<br />
switches: switch times between two customers
