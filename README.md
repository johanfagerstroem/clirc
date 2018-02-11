CLIRC - Command Line Interface Radio Control
============================================
A super-basic command-line wrapper for sui77's RCSwitch library, intended
for Raspberry Pi (using the wiringPi library as backend for RCSwitch).

The purpose (for now) is to provide a CLI tool that exposes all the send
methods of the RCSwitch library in a way that can easily be called in
shell scripts.

Build:
------
Install wiringPi if you haven't already. Instructions on the official
web page: http://wiringpi.com/download-and-install/.

Now clone repository and run make:

```sh
$ make
```
Put the output binary in your path if you'd like.

If you'd like to tweak the default RCSwitch parameters, they can be set in
build time by prepending make with environment variables using their
respective name definitions, e.g. DEFAULT_SENDPIN=12

Run:
----
To run the application you need either to be root (e.g. sudo) or be a member
of the gpio group. To become a member of the gpio group, do the following:

```sh
$ sudo usermod -a -G gpio <your username>
```

Then you should be able to run the application:

```sh
$ ./clirc --help
Example: ./clirc selector 1 3 on
Example: ./clirc dip 01010 10100 on
Example: ./clirc intertechno a 2 3 on
Example: ./clirc raw 000000000000000101010100
```

RCSwitch parameters can also be set in runtime using environment variables,
example:

```sh
$ CLIRC_SENDPIN=12 ./clirc selector 1 3 on
```

Future work:
------------
* Write a more beginner-friendly guide on how to setup the entire thing,
  including hardware.

Note:
-----
* The RCSwitch library is great, see https://github.com/sui77/rc-switch/
* The wiringPi library is great, see https://wiringpi.com/
