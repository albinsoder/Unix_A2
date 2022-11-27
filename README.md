# Assigment2Project Mathserver

## How to compile:

```
$ cd /Assigment2Project/mathserver

$ make all

$ make clean all
```
## Server:
```
$ cd mathserver/objects

$ ./server

```
* Option: -p specify port

* Directory computed_results/server_results will contain any resulting files
if send was unsuccessful, computed_results/server_input contain input files from the
client until command is executed.

## Client:
```
cd mathserver/objects

./client
```
* Option: -p specify server port

* Option: -ip specify server ip

* Directory computed_results/ will contain any resulting files from the server upon command completion.
