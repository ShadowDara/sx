# SX

a simple shortcuter program!

## Usage

Create a `sx.conf` file in your Home Directory

and use the KeyValue shortcuts with 

```sh
sx hi
```

and in the config file is a KeyValue File. Entrys are seperated by `=`

```ini
# Lines with # are comments
hi=echo Hallo World
```

## SX Config

**There is the Option to add a Start Message to SX to customize it more!**

All SX Settings:

```ini
# Option to change the Default Start Message to for example and
# Icon
# Set to true for this!
--overwrite-start-message=false

# Enter here the File name! Path starts in the Home Directory
--start-message-file=empty.txt

# View my dotfiles for Examples!

# Change the Default Terminal Shell which is used on Windows
--windows-default-shell=cmd

# Change the Default Terminal Shell which is used on Linux
--linux-default-shell=bash

# Print the Version and the Buildtime after the Custom Start Message
# true, or false - Default is false
--add-info-to-start-message=true
```

A Warning will appear when a entry which is already for settigs used,
is trying to be used as a Command!

## Multi

SX although has a multi Tools with a lot of options which can be called via SX,
but must be configured separately in the `sx.conf` file.

### NumberGuess Game

A simple Number Guessing Game which can be called with `multi --numberguess`
or `multi -n`

**Settings**

```sh
# Min and Max Number for the Game, Default is 1 and 1000
--numberguess-min=1
--numberguess-max=1000
```

## UI

SX has little UI Wrapper with WinForms on Windows which can be used to call
shortcuts with a simple click on the Button

Commands which will be called on UI Button Click `sx --windows-sx-ui-button1`

So define these 16 Commands in your `sx.conf` file to use the UI
and set the Command you want to call on Button Click

```ini
--windows-sx-ui-button1=hi
--windows-sx-ui-button2=hi
--windows-sx-ui-button3=hi
--windows-sx-ui-button4=hi
--windows-sx-ui-button5=hi
--windows-sx-ui-button6=hi
--windows-sx-ui-button7=hi
--windows-sx-ui-button8=hi
--windows-sx-ui-button9=hi
--windows-sx-ui-button10=hi
--windows-sx-ui-button11=hi
--windows-sx-ui-button12=hi
--windows-sx-ui-button13=hi
--windows-sx-ui-button14=hi
--windows-sx-ui-button15=hi
--windows-sx-ui-button16=hi
```
