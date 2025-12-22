# ⚡ Microshell - 1-DAY CRASH COURSE

**You have minishell experience - let's leverage that for a focused 1-day prep!**

## 🎯 Key Differences from Minishell
- **Simpler**: No PATH, no env vars, no wildcards, no quotes
- **Focused**: Only `|`, `;`, and `cd` builtin
- **Tricky**: Must handle hundreds of pipes with <30 file descriptors

## ⚡ 1-DAY INTENSIVE ROADMAP

### Morning (3-4 hours): Core Theory & Simple Implementation
```
🌅 9:00-10:30 | Quick Theory Review (90 min)
├── fork/execve/waitpid basics (you know this!)
├── pipe() and dup2() mechanics  
├── File descriptor management (CRITICAL!)
└── Error handling patterns

☕ 10:30-11:00 | Break

🔨 11:00-12:30 | Build Simple Executor (90 min)
├── Parse arguments (split by spaces)
├── Handle single commands
├── Implement cd builtin
└── Add basic error handling
```

### Afternoon (4-5 hours): Pipes & Integration
```
🍽️ 12:30-13:30 | Lunch Break

🔧 13:30-15:00 | Add Semicolon Support (90 min)
├── Split commands by ";"
├── Execute sequentially
└── Test with multiple commands

☕ 15:00-15:15 | Break

🚰 15:15-17:00 | Implement Pipes (105 min)
├── Single pipe (cmd1 | cmd2)
├── Multiple pipes (cmd1 | cmd2 | cmd3)
├── File descriptor cleanup (CRUCIAL!)
└── Handle hundreds of pipes

🧪 17:00-18:00 | Testing & Debugging (60 min)
├── Test all combinations
├── Fix memory leaks
├── Perfect error messages
└── Stress test with many pipes
```

### Evening (2 hours): Practice & Polish
```
🎯 18:00-20:00 | Timed Practice
├── Complete fresh implementation (2.5 hours max)
├── Focus on clean, simple code
└── No debugging tools - just like exam!
```

## 🚨 CRITICAL SUCCESS FACTORS

### 1. File Descriptor Management (Most Important!)
```c
// The key pattern you MUST master:
int pipefd[2];
pipe(pipefd);

// Child 1 (writer)
dup2(pipefd[1], 1);  // stdout -> pipe write
close(pipefd[0]);    // CLOSE unused read
close(pipefd[1]);    // CLOSE original write

// Child 2 (reader)  
dup2(pipefd[0], 0);  // stdin <- pipe read
close(pipefd[0]);    // CLOSE original read
close(pipefd[1]);    // CLOSE unused write

// Parent MUST close both ends!
close(pipefd[0]);
close(pipefd[1]);
```

### 2. The "Hundreds of Pipes" Challenge
**Problem**: `cmd1 | cmd2 | cmd3 | ... | cmd100` with only 30 FDs available

**Solution**: Execute pipes in chunks, not all at once
```
Instead of: fork all 100 processes
Do this:   fork -> execute -> wait -> fork next batch
```

### 3. Exact Error Messages (Points Deducted If Wrong!)
```c
// cd errors - EXACT text required:
"error: cd: bad arguments\n"
"error: cd: cannot change directory to path_to_change\n"

// execve error:
"error: cannot execute executable_that_failed\n"

// System call error:
"error: fatal\n"
```

## 🔥 Speed Implementation Strategy

### Core Structure (30 min)
```c
int main(int ac, char **av, char **env) {
    // Skip program name
    execute_commands(av + 1, env);
    return 0;
}
```

### Command Parsing (45 min)
```c
// Split by ";" first, then handle each group
// Within each group, split by "|" for pipes
```

### Execution Engine (90 min)
```c
// 1. Check if builtin (cd)
// 2. If single command -> fork + execve
// 3. If pipeline -> create pipes + fork chain
```

## 🎯 Practice Tests (Use These!)

```bash
# Basic tests
./microshell /bin/echo hello
./microshell /bin/echo hello ";" /bin/echo world

# Pipe tests  
./microshell /bin/ls "|" /usr/bin/head -5
./microshell /bin/cat /etc/passwd "|" /usr/bin/grep root "|" /usr/bin/wc -l

# cd tests
./microshell cd /tmp ";" /bin/pwd
./microshell cd /nonexistent  # Should show error
./microshell cd              # Should show "bad arguments"

# Stress test (create this!)
./microshell /bin/echo 1 "|" /bin/cat "|" /bin/cat "|" ... (repeat 50 times)
```

## ⚠️ Exam Day Mindset

**Time Management:**
- 0-30min: Parse and understand the subject completely
- 30-150min: Core implementation (single commands + cd)
- 150-180min: Add pipes and testing

**Code Style:**
- Keep functions small and focused
- Don't over-engineer - simple is better
- Comment your pipe logic (easy to get confused)

**Testing Strategy:**
- Test each feature as you build it
- Don't wait until the end to test pipes
- Use the provided example as your first test

## 🏆 You've Got This!

**Your minishell experience gives you:**
- ✅ Process management knowledge
- ✅ Understanding of fork/execve patterns
- ✅ Error handling experience

**Focus on:**
- 🎯 File descriptor management
- 🎯 Simple, clean pipe implementation  
- 🎯 Exact error message formatting

**Remember**: This is simpler than minishell - no parsing complexity, no environment variables, no complex builtins. You can do this! 🚀