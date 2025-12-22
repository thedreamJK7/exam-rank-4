# ⚡ Picoshell - 1-DAY CRASH COURSE

**NEW EXAM FORMAT: Much simpler than before! Just implement one function.**

## 🎯 What Changed
- **Function-based**: Implement `int picoshell(char *cmds[])` 
- **Pipeline only**: No semicolons, no cd builtin, just pipes!
- **Simpler parsing**: Commands already parsed for you
- **Limited functions**: Only 7 allowed functions
- **Return codes**: Return 0 on success, 1 on error

## 📋 Exam Requirements

**Function Signature:**
```c
int picoshell(char *cmds[]);
```

**Allowed Functions:** `close`, `fork`, `wait`, `exit`, `execvp`, `dup2`, `pipe`

**Input Format:**
- `cmds` is a null-terminated array of command arrays
- Each command is ready for `execvp()` (no parsing needed!)
- Commands are separated by `"|"` in the input

## ⚡ 1-DAY INTENSIVE ROADMAP

### Morning (2 hours): Theory & Basic Structure
```
🌅 9:00-10:00 | Quick Theory (60 min)
├── fork/execvp/wait basics
├── pipe() and dup2() mechanics  
├── File descriptor management
└── Return value handling

🔨 10:00-11:00 | Basic Structure (60 min)
├── Parse cmds array (find "|" separators)
├── Count number of commands
├── Basic error handling framework
└── Simple test with single command
```

### Afternoon (3 hours): Pipeline Implementation
```
🚰 11:00-13:00 | Core Pipeline Logic (120 min)
├── Create pipes for N-1 commands
├── Fork N processes
├── Connect pipes with dup2()
├── Execute commands with execvp()

🧪 13:00-14:00 | Testing & Debugging (60 min)
├── Test 2-command pipeline
├── Test 3+ command pipeline
├── Fix file descriptor leaks
└── Perfect error handling
```

### Evening (1 hour): Practice & Polish
```
🎯 14:00-15:00 | Final Practice
├── Clean implementation
├── Test edge cases
└── Verify return codes
```

## 🚨 CRITICAL SUCCESS FACTORS

### 1. Understanding the Input Format
```c
// Example input for: ls | grep picoshell
char *cmds[] = {
    (char*[]){"ls", NULL},           // First command
    "|",                             // Pipe separator  
    (char*[]){"grep", "picoshell", NULL}, // Second command
    NULL                             // End marker
};
```

### 2. Pipeline Creation Pattern
```c
int picoshell(char *cmds[]) {
    // 1. Count commands (skip "|" entries)
    // 2. Create N-1 pipes for N commands
    // 3. Fork N processes
    // 4. Connect pipes: cmd[i] stdout -> cmd[i+1] stdin
    // 5. Wait for all children
    // 6. Return 0 on success, 1 on error
}
```

### 3. File Descriptor Management (CRITICAL!)
```c
// For command i in pipeline:
// - If not first: dup2(prev_pipe_read, 0)
// - If not last:  dup2(curr_pipe_write, 1)
// - Close ALL pipe fds in child
// - Close ALL pipe fds in parent after forking
```

## 🔥 Implementation Strategy

### Step 1: Parse Commands (15 min)
```c
// Count actual commands (skip "|")
int count_commands(char *cmds[]) {
    int count = 0;
    for (int i = 0; cmds[i]; i++) {
        if (strcmp(cmds[i], "|") != 0)
            count++;
    }
    return count;
}
```

### Step 2: Create Pipes (15 min)
```c
// For N commands, need N-1 pipes
int pipes[cmd_count-1][2];
for (int i = 0; i < cmd_count-1; i++) {
    if (pipe(pipes[i]) == -1)
        return 1;  // Error
}
```

### Step 3: Fork and Execute (45 min)
```c
// Fork each command
for (int i = 0; i < cmd_count; i++) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child: setup pipes and exec
        setup_pipes(i, pipes, cmd_count);
        execvp(cmd[i][0], cmd[i]);
        exit(1);  // execvp failed
    }
}
```

### Step 4: Cleanup and Wait (15 min)
```c
// Close all pipes in parent
close_all_pipes(pipes, cmd_count-1);

// Wait for all children
for (int i = 0; i < cmd_count; i++) {
    wait(NULL);
}
return 0;
```

## 🎯 Key Differences from Old Microshell

| Old Microshell | New Picoshell |
|----------------|---------------|
| Full program | Single function |
| Parse arguments | Commands pre-parsed |
| Handle `;` and `\|` | Only pipelines |
| Built-in `cd` | No builtins |
| `execve` + env | `execvp` (simpler) |
| Complex error msgs | Simple return codes |

## 🧪 Test Cases

```c
// Test 1: Simple pipeline
char *test1[] = {
    (char*[]){"echo", "hello", NULL},
    "|",
    (char*[]){"cat", NULL},
    NULL
};

// Test 2: Three commands
char *test2[] = {
    (char*[]){"ls", NULL},
    "|", 
    (char*[]){"grep", "picoshell", NULL},
    "|",
    (char*[]){"wc", "-l", NULL},
    NULL
};

// Test 3: Single command (no pipes)
char *test3[] = {
    (char*[]){"echo", "no pipes", NULL},
    NULL
};
```

## ⚠️ Common Pitfalls

1. **Forgetting to close pipes**: Each pipe creates 2 FDs - close them all!
2. **Wrong dup2 order**: Set up pipes before execvp
3. **Not handling single command**: Edge case with no pipes
4. **execvp vs execve**: Use execvp (it handles PATH for you)
5. **Return codes**: Must return 1 on ANY error

## 🏆 This is Much Easier!

**Good news:**
- ✅ No argument parsing needed
- ✅ No built-in commands  
- ✅ No semicolon handling
- ✅ execvp handles PATH
- ✅ Simple return codes

**Focus on:**
- 🎯 Correct pipe setup
- 🎯 File descriptor management
- 🎯 Proper error handling

**You can definitely do this in 3 hours!** 🚀

The new format is much more focused - just master pipeline creation and you're golden!