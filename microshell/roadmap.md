# 🐚 Microshell - Rank 4 Exam Preparation

A simplified shell implementation for 42 School's Rank 4 exam. This README provides a complete roadmap and essential knowledge for building microshell.

## 📋 Exam Overview

**Time Limit**: 3 hours  
**Allowed Functions**: `malloc`, `free`, `write`, `close`, `fork`, `waitpid`, `signal`, `kill`, `exit`, `chdir`, `execve`, `dup`, `dup2`, `pipe`, `strcmp`, `strncmp`

## 🎯 Core Requirements

### Must Implement:
- ✅ Execute shell commands with absolute/relative paths
- ✅ Handle pipes (`|`) for command chaining
- ✅ Handle semicolons (`;`) for command separation
- ✅ Built-in `cd` command with proper error handling
- ✅ Manage hundreds of pipes with limited file descriptors

### Don't Need:
- ❌ PATH variable resolution
- ❌ Wildcards (`*`, `~`)
- ❌ Environment variables (`$VAR`)
- ❌ Complex cd options (`cd -`, `cd` without args)

## 🗺️ Study Roadmap

### Phase 1: Foundation (Week 1)
```
📚 Theory Review
├── Process Management
│   ├── fork() - Creating child processes
│   ├── execve() - Executing programs
│   ├── waitpid() - Waiting for children
│   └── exit() - Process termination
├── File Descriptors
│   ├── stdin (0), stdout (1), stderr (2)
│   ├── dup() & dup2() - Duplicating FDs
│   └── close() - Closing FDs
└── Inter-Process Communication
    └── pipe() - Creating pipes
```

### Phase 2: Core Concepts (Week 2)
```
🔧 Implementation Concepts
├── Command Parsing
│   ├── Tokenization (split by spaces)
│   ├── Operator identification (| and ;)
│   └── Command grouping
├── Process Flow
│   ├── Sequential execution (;)
│   ├── Pipeline execution (|)
│   └── Built-in handling (cd)
└── Error Management
    ├── System call failures
    ├── execve failures
    └── cd error messages
```

### Phase 3: Architecture Design (Week 3)
```
🏗️ Program Structure
├── main()
│   ├── Parse arguments
│   ├── Execute command groups
│   └── Handle errors
├── execute_pipeline()
│   ├── Create pipes
│   ├── Fork processes
│   └── Connect pipes
├── execute_builtin()
│   └── Handle cd command
└── cleanup()
    └── Close file descriptors
```

## 🔄 Execution Flow Visualization

### Simple Command
```
./microshell /bin/ls
     ↓
┌─────────────┐
│    fork()   │
└─────────────┘
     ↓
┌─────────────┐    ┌─────────────┐
│   Parent    │    │    Child    │
│  waitpid()  │    │  execve()   │
└─────────────┘    └─────────────┘
```

### Pipeline (cmd1 | cmd2)
```
./microshell /bin/ls "|" /usr/bin/grep test
     ↓
┌─────────────┐
│   pipe()    │  Create pipe: [read_fd, write_fd]
└─────────────┘
     ↓
┌─────────────┐
│   fork()    │  Create first child
└─────────────┘
     ↓
┌─────────────┐    ┌─────────────┐
│   Parent    │    │   Child 1   │
│             │    │ dup2(write) │
│             │    │ execve(ls)  │
└─────────────┘    └─────────────┘
     ↓
┌─────────────┐
│   fork()    │  Create second child
└─────────────┘
     ↓
┌─────────────┐    ┌─────────────┐
│   Parent    │    │   Child 2   │
│  waitpid()  │    │ dup2(read)  │
│  waitpid()  │    │execve(grep) │
└─────────────┘    └─────────────┘
```

### Multiple Pipes (cmd1 | cmd2 | cmd3)
```
     pipe1        pipe2
   [r1][w1]    [r2][w2]
      ↓           ↓
┌─────────┐ ┌─────────┐ ┌─────────┐
│  cmd1   │ │  cmd2   │ │  cmd3   │
│stdout→w1│ │r1→stdin │ │r2→stdin │
└─────────┘ │stdout→w2│ └─────────┘
            └─────────┘
```

## 🧠 Key Concepts to Master

### 1. File Descriptor Management
```c
// Essential pattern for pipes
int pipefd[2];
pipe(pipefd);  // Creates read/write pair

// In child process:
dup2(pipefd[1], STDOUT_FILENO);  // Redirect stdout to pipe
close(pipefd[0]);  // Close unused read end
close(pipefd[1]);  // Close original write end
```

### 2. Process Synchronization
```c
// Fork and wait pattern
pid_t pid = fork();
if (pid == 0) {
    // Child: execute command
    execve(path, args, env);
} else {
    // Parent: wait for child
    waitpid(pid, NULL, 0);
}
```

### 3. Built-in Commands
```c
// cd implementation
if (strcmp(args[0], "cd") == 0) {
    if (args[1] == NULL || args[2] != NULL) {
        write(2, "error: cd: bad arguments\n", 25);
    } else if (chdir(args[1]) != 0) {
        write(2, "error: cd: cannot change directory to ", 38);
        write(2, args[1], strlen(args[1]));
        write(2, "\n", 1);
    }
}
```

## ⚠️ Common Pitfalls

### File Descriptor Leaks
- Always close unused pipe ends
- Close file descriptors in both parent and child
- Remember: each pipe() creates 2 FDs

### Process Management
- Don't forget to wait for all children
- Handle execve failures properly
- Pass environment to execve

### Error Handling
- Check all system calls (except execve/chdir)
- Print exact error messages as specified
- Exit immediately on fatal errors

## 🎯 Practice Strategy

### Week 1-2: Build Foundation
1. **Simple executor**: Handle single commands
2. **Add semicolons**: Execute sequential commands
3. **Implement cd**: Built-in command handling

### Week 3: Add Complexity
1. **Single pipes**: Two-command pipelines
2. **Multiple pipes**: Chain multiple commands
3. **Stress testing**: Handle hundreds of pipes

### Week 4: Polish & Practice
1. **Error handling**: Perfect all error cases
2. **Memory management**: No leaks
3. **Timed practice**: Complete implementation in 3 hours

## 📚 Essential Functions Reference

| Function | Purpose | Key Points |
|----------|---------|------------|
| `fork()` | Create child process | Returns 0 in child, PID in parent |
| `execve()` | Execute program | Replaces current process image |
| `pipe()` | Create pipe | Returns array of 2 file descriptors |
| `dup2()` | Duplicate FD | Redirect input/output |
| `waitpid()` | Wait for child | Prevent zombie processes |
| `chdir()` | Change directory | Built-in cd implementation |

## 🏆 Success Metrics

- ✅ Handles simple commands
- ✅ Executes pipelines correctly
- ✅ Manages sequential commands (`;`)
- ✅ Implements cd with proper errors
- ✅ No file descriptor leaks
- ✅ Completes within 3-hour time limit

---

**Remember**: The exam tests your understanding of process management and IPC. Focus on clean, simple code that handles all edge cases correctly. Good luck! 🚀