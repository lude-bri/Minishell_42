<!-------Minishell_image-------->
<a name="readme-top"></a>
<p align="center">
	<img src="https://capsule-render.vercel.app/api?type=venom&height=200&color=0ABAB5&text=Minishell&fontAlign=50&fontAlignY=61&animation=fadeIn&fontColor=fcf3f2&fontSize=100" />
</p>

<!-------About-------->

## About

This project is the implementation of a simple Unix shell, written in C as part of the 42 School Common Core curriculum. The objective is to gain a deep understanding of how command-line shells work at a system level: parsing input, handling processes, managing environment variables, and dealing with pipes and redirections. **Minishell** is not just a coding project, it is a structured software engineering study, focused on systems programming and OS-level interactions.

## How it is organized 

But beyond its functional implementation, this project is also structured as a pedagogical tool.

It is organized in a modular and **chapter-based** way, inspired by the structure of books. Each section of the project is grouped using a numerical system:

  * `100_*.c`: Initial setup, main, initialization, and environment.
  * `200_*.c`: Tokenization and lexical analysis.
  * `300_*.c`: Parsing and syntax trees.\
  * `400_*.c`: Execution and command evaluation.
  * `500_*.c`: Built-in commands (cd, echo, export, etc).
  * `600_*.c`: Pipe management and process forking.
  * `700_*.c`: Redirections and heredocs.
  * `800_*.c`: Error management (reserved for future additions).
  * `900_*.c`: Memory freeing and file descriptor closing.

Each major group can have subchapters (e.g., 110, 111) and specific segments (e.g., 531, 742) for logical segmentation.

This design allows you to read the source code almost as if it were a textbook, understanding the shell in layers: from how input is taken and split into tokens, all the way to forking processes and redirecting I/O.

----

<!-------Index-------->

## <a name="#index-0">Index</a>

<ul>
	<li><strong><a href="#1-shell" style="color:white">1. Shell </a></strong></li>
	<ul style="list-style-type:disc">
		<li><a href="#11-historical-context-and-evolution"> 1.1. Historical Context and Evolution</a></li>
		<li><a href="#12-archictecture-of-a-shell"> 1.2. Archictecture of a Shell</a></li>
    		<li><a href="#13-types-of-shell"> 1.3. Types of Shell</a></li>
    		<li><a href="#14-why-shell-matters-anyways"> 1.4. Why Shell matters anyways?</a></li>
	</ul>
	<li><strong><a href="#2-minishell-or-msh-" style="color:white">2. Minishell (or msh) </a></strong></li>
	<ul style="list-style-type:disc">
		<li><a href="#21-msh-a-replica"> 2.1. msh: a Replica! </a></li>
		<li><a href="#22-msh-how-does-it-works"> 2.2. msh: How does it works? </a></li>
		<li><a href="#23-why-a-minishell-anyways"> 2.3. Why a Minishell anyways? </a></li>
	</ul>
	<li><strong><a href="#sources" style="color:white"> Sources </a></strong></li>
</ul>

<div align="center">
<ul>
	<li><strong><a href="#usage"> # Usage of Minishell #</a></strong></li>
</ul>
	</div>
 
------------------------------------------------------------------
----
# <a name="#index-1">1. Shell</a>

In this project we’re going to create a Shell. So.. what’s a SHELL?

No, it's not (only) a british multinational oil and gas company. It is a **computer program**! Provides relatively broad and direct access to the system on which it runs. 

A shell is a specialized software component that acts as an intermediary between the user and the operating system (OS) kernel. It provides an **interface—either command-line** (CLI) or **graphical** (GUI) to execute programs, manage system resources, and control process execution.

But basically, a shell is a command-line interpreter that gives orders to the operating system! That's it.

The term **shell** refers to how it is a relatively thin layer around an operating system. The shell operates as a critical intermediary layer in the operating system (OS) stack, sitting between the user and the kernel. 

This architectural positioning is often described metaphorically as a "shell" because it wraps around the kernel, providing a human-accessible interface while insulating users from low-level system complexities (Ritchie & Thompson, 1974).

<div align=center>
	<image src=img/img_1.png>
</div>

But, long story short and from a software engineering perspective, a shell is actually a **command interpreter** that:

* **Parses** user input into executable instructions.
* **Invokes** system calls (e.g., `fork()`, `execve()`, `wait()`) to delegate tasks to the kernel.
* **Manages** process control (foreground/background execution, job control).
* **Handles** I/O redirections, pipelines, and environment variables.

>[!IMPORTANT]
> The shell is not part of the kernel but runs in user space, relying on system calls to interact with the OS (Ritchie & Thompson, 1974).

## <a name="#index-1">1.1. Historical Context and Evolution</a>

The shell, as a fundamental component of Unix-like operating systems, has undergone significant evolution since its inception in the late 1960s. The earliest Unix shell, the Thompson shell, was developed by Ken Thompson in 1971 and provided basic command execution but lacked scripting capabilities(Ritchie, 1978). Its limitations spurred the creation of the **Bourne shell**(sh) by **Stephen Bourne in 1977**, which introduced structured programming constructs, making it the first shell suitable for complex scripting (Bourne, 1978).

The late 1970s and 1980s saw diversification in shell design philosophies. The C shell(csh), developed by Bill Joy at UC Berkeley, introduced a C-like syntax and job control, allowing users to manage background and foreground processes interactively (Joy, 1980). However, its deviation from Bourne shell syntax led to compatibility issues, prompting the development of the Korn shell (ksh) by David Korn in 1983, which combined Bourne shell compatibility with enhanced interactive features (Korn, 1983).

The **GNU Project’s Bourne-Again Shell (bash)**, released in 1989, became the de facto standard due to its POSIX compliance, extensibility, and integration with Linux distributions (Fox & Ramey, 1994). Meanwhile, the Z shell (zsh), developed in 1990, introduced advanced customization, autocompletion, and plugin support, later popularized by frameworks like Oh-My-Zsh (Falco, 2015).

This evolution reflects broader trends in computing:

* **From CLI to Hybrid Interfaces**: Modern shells (e.g., PowerShell, fish) blend traditional CLI with object-oriented pipelines.
* **Scripting as a First-Class Feature**: Shells transitioned from simple command runners to full-fledged scripting engines.
* **Security and Sandboxing**: Restricted shells (e.g., rbash) mitigate privilege escalation risks (Kerrisk, 2010).

## <a name="#index-1">1.2. Archictecture of a Shell</a>

Essentially, a shell operates in a **read-eval-print loop (REPL**) cycle, that being:

1. **Lexical Analysis & Parsing**
   * Tokenizes input (e.g., splitting `ls -l | grep ".txt"` into commands, arguments, and operators).
   * Constructs an abstract syntax tree (AST) for execution flow (Aho et al., 2006).

2. **Command Execution**
   * Built-ins (e.g., `cd`, `export`) are handled internally.
   * External programs (e.g., `/bin/ls`) are executed via `fork()` + `exec()`.

3. **Process Management**
   * Foreground processes block the shell until completion.
   * Background processes (`&`) run asynchronously.
   * Signals (e.g., `SIGINT`, `SIGTERM`) allow process control.

4. **I/O Redirection & Pipes**
   * `<`, `>`, `<<`, `>>` modify file descriptors (FDs).
   * Pipes (`|`) chain processes via inter-process communication (IPC).

## <a name="#index-1">1.3. Types of Shell</a>

So, since shell started to be a essencial tool for users to interact with the OS, the Unix-like ecosystem has spawned a diverse array of shells, each optimized for specific use cases—ranging from minimalist scripting to interactive power use. Take a look at some different shell's:

| Shell	| Strengths | Weaknesses | Ideal Use |
-------------- | ---------------  | ---------- |--------- |
| Bash  | POSIX-compliant, scripting-heavy, default on most Linux systems | Heavyweight | General-purpose scripting | 
| Zsh	| Improved autocomplete, plugins | Complex config | Developer workflows | 
| Fish	| User-friendly syntax, web-based config | Non-POSIX | Interactive exploration | 
| Dash	| Lightweight, minimal |  Feature-poor | System boot scripts |
| PowerShell | Object pipelines | High latency | Windows automation |

## <a name="#index-1">1.4. Why Shell matters anyways?</a>

Shells remain indispensable in contemporary computing due to their **automation**, **interoperability**, and **low-level control capabilities**. In system administration, shells facilitate batch processing, log analysis (e.g., `grep`, `awk`), and remote management via `ssh` (Nemeth et al., 2017). The rise of **DevOps** has further cemented their role, with tools like Ansible and Docker relying on shell commands for orchestration (Vaughn, 2020).

In **software development**, shells underpin build systems (e.g., `make`) and version control hooks (e.g., Git pre-commit scripts). The **POSIX shell standard** ensures cross-platform compatibility, enabling scripts to run on Unix, Linux, and macOS without modification (IEEE Std 1003.1, 2018).

Embedded systems leverage lightweight shells (e.g., BusyBox) for resource-constrained environments, where full-featured shells like `bash` are impractical (Yaghmour, 2019). Conversely, high-performance computing (HPC) clusters use shells to distribute tasks across nodes via job schedulers (e.g., Slurm).

Emerging trends include:

* **Secure Shells**: Restricted shells (`rbash`) and sandboxed environments (e.g., `firejail`) mitigate command injection risks (Kerrisk, 2010).
* **Shells as APIs**: Projects like Microsoft’s PowerShell treat shell commands as object pipelines, bridging CLI and programmatic interfaces (Snover, 2006).
* **Containerization**: Shells are embedded in container images (e.g., Alpine Linux’s `ash`) for debugging and initialization (Mouat, 2015).

# <a name="#index-1">2. MiniShell (or msh) </a>

Ok! Now that we know a little bit more about what is a shell, what about this project? The minishell?

Well, **Minishell** is a simplified reimplementation of a **Unix shell** — such as `bash` or `zsh` — developed from **scratch** using the **C programming language**. Its primary goal is to demystify the inner workings of a shell environment by recreating its most fundamental features in a minimal yet functional program.

## <a name="#index-1">2.1. msh: A Replica!</a>

Minishell replicates the core behavior of traditional Unix shells:
* Reading user input via a command-line prompt,
* Tokenizing and parsing commands with arguments, redirections, and pipes,
* Executing both built-in and external programs,
* Managing environment variables,
* Handling system signals like `Ctrl+C`,
* Supporting features such as heredoc, I/O redirections (`>`, `>>`, `<`, `<<`), and pipes (`|`).

> [!NOTE]
> However, unlike `bash` or `zsh`, which are mature, feature-rich, and maintained by large development communities, **Minishell** is built entirely by me and my friend Manuel (students from 42) using only the C standard library and a limited set of allowed system calls (`fork`, `execve`, `dup2`, `pipe`, `waitpid`, etc.). There is no reliance on GNU Readline internals or external libraries, aside from what is explicitly permitted in the project.

This means every aspect (from handling quotes and escapes in strings  to building a syntax tree for commands, to manually managing child processes) is implemented and maintained by hand.

## <a name="#index-1">2.2. msh: How does it works?</a>

Even though it is a "mini" version, the shell must support many complex behaviors:

| Feature	| Descriptions |
-------------- | ---------------  |
| Prompt  | Custom prompt display using dynamic environment values like `USER` or `PWD`. |
| Built-ins	| Implementation of `cd`, `echo`, `pwd`, `export`, `unset`, `env`, and `exit`. |
| Execution	| Support for system binaries (e.g., `ls`, `/usr/bin/env`, `cat`) | 
| Pipes	| Ability to chain commands using pipes and file descriptors | 
| Redirections | Handle `<`, `>`, `>>`, and heredocs `<<`. |
| Environment | Dynamic modification and inheritance of environment variables.|
| Error and syntax handling | Custom parsing logic with error checking for unclosed quotes, bad pipes, etc. |
| Signals | Robust reaction to keyboard input (e.g., stopping heredocs with `Ctrl+C`).|

## <a name="#index-1">2.3. Why a Minishell anyways?</a>

The significance of implementing a shell like Minishell lies in its unique ability to bridge the gap between abstract systems theory and practical programming experience. While many programming projects focus on user-facing features or algorithms, a shell forces the developer to work directly at the boundary between user and operating system — a critical frontier in software development.

By developing a shell from scratch, students gain firsthand experience with the fundamental building blocks of modern computing. They do not rely on abstraction layers or frameworks but instead must engage directly with system calls, process management, memory handling, and file descriptors. This interaction fosters a deeper understanding of how operating systems function internally and how user-level applications interact with the kernel.

Moreover, the shell serves as an ideal case study in the design of interpreters. The task of reading user input, parsing it into tokens, building a syntax tree, handling edge cases like quoted strings or redirection syntax, and finally executing it mirrors the same design challenges found in compilers and interpreters for programming languages. The student effectively builds a real-world language interpreter, albeit for a shell-like syntax.

Additionally, the project cultivates a mindset of defensive programming and error awareness. Unlike academic exercises with clean inputs, shell interaction involves unpredictable user input, nested command structures, and asynchronous events like signals or system interruptions. Handling these gracefully requires careful planning, robust logic, and a mature approach to software architecture.

In essence, Minishell is important because it brings together critical topics — systems programming, process control, parsing, execution, and user experience — into one cohesive and challenging project. It encourages autonomy, resilience, and craftsmanship, all while producing something that is both educational and fundamentally useful.

Writing a shell is a rite of passage in systems programming. By following a textbook-style layout, this implementation of minishell aims not only to provide functionality but also to tell a story — one that goes from user input to forked processes, redirections, and execution.

This project is an excellent resource for anyone wishing to understand how the shell works under the hood — and how the operating system responds to it.
Whether you're a beginner or an experienced developer, there's something to learn at every chapter.

## <a name="#index-1">Sources</a>

* Aho, A. V., et al. (2006). Compilers: Principles, Techniques, and Tools. Pearson.
* Bourne, S. R. (1978). The Unix Shell. Bell System Technical Journal.
* Bryant, R. E., & O'Hallaron, D. R. (2015). Computer Systems: A Programmer's Perspective. Pearson
* Debian Wiki. (2023). Dash as /bin/sh. https://wiki.debian.org/DashAsBinSh
* Falco, R. (2015). Oh-My-Zsh: A Community-Driven Framework. GitHub.
* Fox, B., & Ramey, C. (1994). Bash Reference Manual. GNU Press.
* IEEE Std 1003.1. (2018). *POSIX.1-2017: Standard for Information Technology*. IEEE.
* Kerrisk, M. (2010). The Linux Programming Interface. No Starch Press.
* Ritchie, D. M., & Thompson, K. (1974). The UNIX Time-Sharing System. CACM.
* Robbins, A. (2021). Bash Pocket Reference. O’Reilly.
* Snover, J. (2017). PowerShell: Unix’s Successor?. Microsoft DevBlogs.
* Stevens, W. R., & Rago, S. A. (2013). Advanced Programming in the Unix Environment. Addison-Wesley.
* Tanenbaum, A. S., & Bos, H. (2015). Modern Operating Systems. Pearson.

# Usage

## Setup & Compilation

1. Clone repository:

```sh
git clone git@github.com:lude-bri/Minishell_42.git
```

2. Go inside the project folder and run `make`:

```sh
cd Minishell_42
make
```

3. To use minishell, run the following command and have fun!

```bash
./minishell
```

### License

This work is published under the terms of <a href="https://github.com/lude-bri/42_Common_Core/blob/main/LICENSE.md">42 Unlicense</a>.

<p align="right">(<a href="#readme-top">get to top</a>)</p>

