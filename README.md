# hyperloop_passenger_booking
Assumptions 
1. All the hyperloop routes &amp; the starting station will be given as input. (See the below input section for details) 
2. Assumptions is that, the distance and time taken travel a route is constant. 
3. A pod can travel from one connection to another in any direction in the given route. (i.e both A to B and B to A) 
4. One pod can accommodate only one passenger at a time. 
5. Passengers will be booked to their pods one by one. 
  a. All passengers will start from the given starting station. 
  b. Whenever a pod is started, the oldest person in the queue will boarding pod first.  
6. Passengers can arrive at any time as well as pods can start at any time. 
7. Assume infinite supply of pods and collision will never happen. 
System Logic : Whenever a pod is started, the pod should pickup 
    ● the oldest person from the passenger queue and 
    ● take the route which has minimum interconnections

Data Structure and Algorithms used in code:
1. Dijkstra's Shortest Path Algorithm
2. Linked List

Programming Language :
C
