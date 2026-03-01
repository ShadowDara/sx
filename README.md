# SX

a simple shortcuter program!

## Usage

Create a `sx.conf` file in your Home Directory

and use the KeyValue shortcuts with 

```sh
sx hi
```

and in the config file is a KeyValue File. Entrys are seperated by `=`

```sh
# Lines with # are comments
hi=echo Hallo World
```

## Config

**There is the Option to add a Start Message to SX to customize it more!**

All Settings:

```sh
# Option to change the Default Start Message to for example and
# Icon
# Set to true for this!
--overwrite-start-message=false

# Enter here the File name! Path starts in the Home Directory
--start-message-file=empty.txt

# View my dotfiles for Examples!

# Change the Default Terminal Shell which is used on Windows
--windows-default-shell=cmd

# Print the Version and the Buildtime after the Custom Start Message
# true, or false - Default is false
--add-info-to-start-message=true
```

A Warning will appear when a entry which is already for settigs used,
is trying to be used as a Command!
