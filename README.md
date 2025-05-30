# SHA-256 tool

## Introduction

This tool was developed as a project for a university C++ course. It's written to be easily understood, although juggling bytes between different types might not be that clear.
I'm pretty sure this type juggling can be avoided, though it's a bit above my current proficiency in C++.

## Usage

The tool itself has a command line interface which takes the following arguments:
```
  -i <input_path>    Input file path
  -o <output_path>   Output file path (if you'd like to write the output to a file)
  -c <hash>          Check calculated hash against the one provided after -c
  -v                 Verbose
  -vv                Very verbose
  -h                 Display help message
```
They can be supplied in any order, although if `-h` is detected anywhere, the help will be shown, and the program will close without calculating anything.

## Explanation

### Input path

If the input path is not specified using `-i`, you will be prompted to provide it during program runtime. The file under this path is read as bits and hashed.

### Output path

The program will produce an output to the specified file. If `-o` is omitted, the calculated hash will be displayed in the console

### Check against known hash

This option enables automatic checking against a known hash. This can be used to verify file integrity. This option provides a human-friendly output for easy manual comparison along with the check's result.

> Note: You can use `-c` along with `-o`!  

### Verbose

Prints messages when major hashing steps are underway. Every print resulting from `-v` being set is indented and preceded by `[v]` for legibility. Some steps print out multi-line messages - those use only one `[v]` at the start, and keep consistent indentation to aid judging progress.

### Very verbose

These messages expand upon the output of `-v` with added details. These are preceded with `[vv]` and might get a bit overwhelming when hashing LARGE files. Don't say I didn't warn you :p

### Help

Displays a help message with a brief overwiew of options, akin to the explanation in [Usage](#usage).

## Thanks for downloading (° ∇^d)!!