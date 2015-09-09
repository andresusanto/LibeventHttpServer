# Libevent Http Server
This project will show how to make a Simple HTTP Server by using Libevent in unix environment under 100 lines of codes for Educational Purposes.

## Warning
The main goal of this project is to show how to use libevent to create an event driven web server. Thus, I tried to make this project as simple as possible (by ignoring security factors). Therfore, this project contains some security issues (including non validation of users' input).


## How to compile
```bash
g++ server.cpp -o server -std=c++11 -levent
```
## How to run
```bash
./server [HOME DIR] [PORT]
```
usage example
```bash
./server /var/www/html 8080
```

## License
MIT License.