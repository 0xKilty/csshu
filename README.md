# csshu
A command-line tool for students to pick a CSU machine to ssh into along with functionality to upload a .tar file which is useful as many courses require your .tar file works on the deparment machines. No passwords are ever input into the program, only the `ssh` and `scp` command.
## Dependancies
```
sudo apt-get install g++ curl libcurl4-openssl-dev 
```
* `g++` - compile the program
* `curl` - make the web request
* `libcurl4-openssl-dev` - use the curl library
## Installation
```bash
git clone https://github.com/0xKilty/csshu
cd csshu
g++ csshu -lcurl
```

## Usage
## Functionality
* `ssh` - for making the connection to the CSU machine
* `scp` - to securely copy a file to the CSU machine
## Disclaimer
This code is probably not the best, this program doesn't even need to be written in C++. But I decided to make it in C++ as it is a language I am still learning.
## Modivation
I always like picking the fastest machine to ssh into and I would often go to the CSU [machine stats page](https://www.cs.colostate.edu/machinestats/?column=percent_used_memory&order=asc), and pick the best machine. I was tired of this and decided to practice my C++ skills.