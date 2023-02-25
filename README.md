# csshu
A command-line tool for students to pick a CSU machine to ssh into along with functionality to upload a .tar file which is useful as many courses require your .tar file works on the deparment machines. No passwords are ever input into the program, only the `ssh` and `scp` command.
## Dependancies
Debian/Ubuntu
```
sudo apt-get install g++ curl libcurl4-openssl-dev
```
Fedora/Red-Hat
```
sudo dnf install g++ curl libcurl-devel
```
* `g++` - compile the program
* `curl` - make the web request
* `libcurl` - use the curl library (Whichever OS equivelant will work)
## Installation
```bash
git clone https://github.com/0xKilty/csshu
cd csshu
cmake . && make
./csshu 
```
You can add csshu to your `$PATH` by putting the following in your `.bashrc` file.
```
export PATH="$PATH:/path/to/csshu"
```

## Usage
```
csshu: a tool to connect automatically to the fastest machine on the CSU network

Options:

-u <username> specify a username

-f <file>     select a file for upload

-h            show help message

-q <quantity> the number of machines in the list

-l[tcamg]     list machines by specification
-[tcamg]      connect automatically to a machine by the specification

(t) uptime (c) CPU (a) Load Average
(m) Memory (g) GPU (default) Load Average
```
## Functionality
* `ssh` - for making the connection to the CSU machine
* `scp` - to securely copy a file to the CSU machine
## Disclaimer
This code is probably not the best, this program doesn't even need to be written in C++. But I decided to make it in C++ as it is a language I am still learning and please make any pull requests to change what you see fit.
## Modivation
I always like picking the fastest machine to ssh into and I would often go to the CSU [machine stats page](https://www.cs.colostate.edu/machinestats/?column=percent_used_memory&order=asc), and pick the best machine. I was tired of this and decided to practice my C++ skills.
