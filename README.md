# SX

a simple shortcuter program

## Usage

Create a `sx.conf` file in your Home Directory

and use the KeyValue shortcuts with 

```sh
sx hi
```

and in the config file

```conf
# Lines with # are comments
hi=echo Hallo World
```

## Config

There is the Option to add a Start Message with

```sh
# Option to change the Default Start Message to for example and
# Icon
# Set to true for this!
--overwrite-start-message=false
# Enter here the File name! ~ is equal to the Home Directory
# although on Windows!
--start-message-file=empty
```
