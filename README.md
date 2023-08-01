# Scheduler
This is a customised CPU scheduler algorithm
The target is to balance the total waiting time, priority customer waiting time, longest response and switches.
The compute_stats is used to calculate the metric data
data_1111, data_2222, and data_3333 are input data for testing
## Input data format
c00 0 6 23: customer 01, VIP customer, arrives at time 6, and need to use 23-time slots
## metrics
total_wait0: total waiting time for VIP customers
total_wait1: total waiting time for normal customers
longest_response: longest waiting time to get the service
switches: switch times between two customers
