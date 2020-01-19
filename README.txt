This is a project for my interest.

In this project, we provide some functions.

The server process, privide 3 app-protocol service and 2 http service.

3 app-protocol services:
	Using client (compiled from client.cc) process,

	1) echo service:
		Echo what you press on terminal, e.g. press "Interest", you will get a echo "Interest" from server.
		 
	2) put service:
		You can put a string to server and the server will store it. If you put many times, the server will overwrite the old value.

	3) get service:
		You cat get a string from the server that you put before. If you get before put, a meaningless return will happen.

2 http protocol services:
	1) http put service:
		The server will store the data that followed by a http put request.

	2) http get service:
		The server will return the data stored before when get a http get request.

