# 🐚 Minishell

A lightweight UNIX shell implementation written in C, recreating the core functionality of bash. This project is part of the 42 school curriculum and demonstrates deep understanding of process management, file descriptors, and UNIX system calls.

[![42 School Project](https://img.shields.io/badge/42-Project-blue)](https://42.fr)
[![Norminette](https://img.shields.io/badge/norminette-passing-success)](https://github.com/42School/norminette)

## 📋 Table of Contents

- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Technical Implementation](#technical-implementation)
- [Project Structure](#project-structure)
- [Testing](#testing)
- [Known Limitations](#known-limitations)
- [Authors](#authors)

## ✨ Features

### Core Shell Functionality
- **Interactive prompt** with custom display showing user, hostname, and current directory
- **Command execution** with path resolution from `$PATH`
- **Pipeline support** (`|`) for chaining multiple commands
- **Redirections**:
  - Input redirection (`<`)
  - Output redirection (`>`)
  - Append mode (`>>`)
  - Here documents (`<<`) with and without variable expansion
- **Environment variable expansion** (`$VAR`, `$?`)
- **Quote handling** (single `'` and double `"` quotes)
- **Signal handling** (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`)
- **Command history** using readline library

### Built-in Commands
- `echo` with `-n` option
- `cd` with relative and absolute paths
- `pwd` without options
- `export` for environment variables
- `unset` to remove environment variables
- `env` to display environment
- `exit` with numeric exit codes

## 🚀 Installation

### Prerequisites
- GCC or Clang compiler
- GNU Make
- readline library (`libreadline-dev` on Debian/Ubuntu)
- Git

### Build Instructions

```bash
# Clone the repository
git clone [your-repo-url] minishell
cd minishell

# Build the project
make

# The executable will be created as 'minishell'
./minishell
```

### Makefile Targets

```bash
make          # Build the project
make clean    # Remove object files
make fclean   # Remove object files and executable
make re       # Rebuild the project
make fcleanall # Remove everything including cloned libraries
make reall    # Rebuild everything from scratch
```

## 💻 Usage

### Starting Minishell

```bash
./minishell
```

You'll see a prompt like:
```
user@hostname:~$
```

### Basic Examples

```bash
# Simple command
ls -la

# Pipeline
cat file.txt | grep "search" | wc -l

# Redirections
echo "Hello World" > output.txt
cat < input.txt
echo "Appended text" >> output.txt

# Here document
cat << EOF
This is a here document
It supports multiple lines
EOF

# Environment variables
echo $HOME
echo $?  # Exit status of last command
export MY_VAR="value"
echo $MY_VAR

# Quotes
echo "Double quotes allow $expansion"
echo 'Single quotes prevent $expansion'

# Built-in commands
cd /tmp
pwd
env
exit 0
```

## 🔧 Built-in Commands

### `echo`
```bash
echo [-n] [string ...]
```
Print arguments to standard output. Option `-n` suppresses the trailing newline.

### `cd`
```bash
cd [directory]
```
Change the current directory. Without arguments, changes to `$HOME`.

### `pwd`
```bash
pwd
```
Print the current working directory.

### `export`
```bash
export [name[=value] ...]
```
Set environment variables. Without arguments, displays all exported variables.

### `unset`
```bash
unset [name ...]
```
Remove environment variables.

### `env`
```bash
env
```
Display all environment variables.

### `exit`
```bash
exit [n]
```
Exit the shell with exit status `n`. Without arguments, exits with status 0.

## 🏗️ Technical Implementation

### Architecture

The shell is organized into several modules:

1. **Lexer**: Tokenizes input into tokens (words, pipes, redirections)
2. **Parser**: Builds command structures from tokens
3. **Executor**: Executes commands with proper process management
4. **Environment**: Hash table-based environment variable management
5. **Built-ins**: Implementation of shell built-in commands
6. **Signals**: Proper signal handling for interactive use

### Key Features

#### Hash Table Environment
- O(1) average-case lookup for environment variables
- Efficient memory management
- 127-bucket hash table implementation

#### Variable Expansion
- Supports `$VAR` and `${VAR}` syntax
- Special variable `$?` for last exit status
- Quote-aware expansion (disabled in single quotes)

#### Process Management
- Fork/exec pattern for external commands
- Proper file descriptor management
- Pipeline implementation with multiple processes
- Signal handling in both parent and child processes

#### Memory Management
- No memory leaks (verified with valgrind)
- Proper cleanup on exit and error conditions
- RAII-style resource management

## 📁 Project Structure

```
minishell/
├── inc/                      # Header files
│   ├── builtin_commands.h    # Built-in command declarations
│   ├── commands.h            # Command execution functions
│   ├── environment.h         # Environment variable management
│   ├── executor.h            # Execution pipeline
│   ├── lexer.h               # Tokenization
│   ├── main.h                # Main program structures
│   ├── parser.h              # Parsing structures
│   ├── pipes.h               # Pipeline implementation
│   ├── signals.h             # Signal handling
│   └── utils.h               # Utility functions
├── src/                      # Source files
│   ├── builtin_commands/     # Built-in implementations
│   ├── environment/          # Environment variable handling
│   ├── executor/             # Command execution
│   ├── heredoc/              # Here document implementation
│   ├── lexer/                # Tokenization logic
│   ├── main/                 # Main loop and prompt
│   ├── parser/               # Command parsing
│   ├── signals/              # Signal handlers
│   └── utils/                # Utility functions
├── libft/                    # Custom C library (submodule)
├── Makefile                  # Build configuration
└── README.md                 # This file
```

## 🧪 Testing

### Manual Testing
```bash
# Test pipelines
echo "test" | cat | cat | cat

# Test redirections
ls > files.txt
cat < files.txt
echo "new line" >> files.txt

# Test here documents
cat << EOF
multiple
lines
EOF

# Test environment
export TEST=value
echo $TEST
unset TEST
echo $TEST

# Test exit codes
ls /nonexistent
echo $?

# Test quotes
echo "Hello $USER"
echo 'Hello $USER'
```

### Automated Testing

You can use external testers:
- [42_minishell_tester](https://github.com/LucasKuhn/minishell_tester)
- [minishell_test](https://github.com/mbenbouzid/minishell_test)

### Memory Leak Testing
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./minishell
```

## ⚠️ Known Limitations

### Not Implemented
- Logical operators (`&&`, `||`)
- Wildcards (`*`)
- Backslash escaping (`\`)
- Semicolon separator (`;`)
- Subshells (`()`)
- Command substitution (`` `command` `` or `$(command)`)
- Job control (`&`, `fg`, `bg`, `jobs`)
- Advanced redirections (`&>`, `>&`, `<>`, etc.)

### Differences from Bash
- Some edge cases in quote handling may differ
- Error messages may have slightly different wording
- History is not persistent across sessions
- No completion features beyond basic file path completion

## 👥 Authors

- Robert Celiowski - https://github.com/rceliows
- Daniel Robert - https://github.com/damiensrobert1

## 📝 License

This project is part of the 42 school curriculum. Feel free to use and modify for educational purposes.

## 🙏 Acknowledgments

- 42 School for the project subject
- The readline library maintainers
- All the students who shared their debugging experiences

## 📚 Resources

- [GNU Bash Manual](https://www.gnu.org/software/bash/manual/)
- [Advanced Programming in the UNIX Environment](https://www.amazon.com/Advanced-Programming-UNIX-Environment-3rd/dp/0321637739)
- [The Linux Programming Interface](https://man7.org/tlpi/)
- [Beej's Guide to Unix IPC](https://beej.us/guide/bgipc/)

---

**Note**: This is an educational project and is not intended for production use. For actual shell needs, please use bash, zsh, or other mature shell implementations.
