# Assigment2Project

 Mathserver

REQUIREMENTS:
Server

1. TCP connection on specified port & IP
2. Handle forking when more than one client are connected
3. Print when client connects
4. Print client's command
5. Store computed result in 'problemtype'_'clientnmbr'_'solnmbr'.txt file (ex matinv_client1_soln2.txt)
6. Send result file to the correct client
7. Command -h, Print help text
8. Command -p port, Listen to port number "port"
9. Command -d, Run as daemon instead of as a normal program(optional)
10. Command -s strategy, Specify the request handling strategy(optional)
11. Compute selected problem (kmeans or matrix inverse)
12. Zombie processes are not allowed to be created

Client

1. TCP connection on specified port and IP
2. Send problem data to server
3. Recieve result data and able to send a new problem
4. Give feedback to user to user through the terminal

Kmeans

1. The program should be able to compute a problem in parallell using pthreads
2. Command -f, Specify the file containing the problem data, ex kmeans-data.txt
3. Command -k, Specify the number of clusters, ex 4, 9, etc

Matrix

1. The program should be able to compute a problem in parallell using pthreads
2. Program must support all the command-line options supported by the sequential code
