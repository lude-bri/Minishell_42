<!-------Minishell_image-------->
<a name="readme-top"></a>
<p align="center">
	<img src="https://capsule-render.vercel.app/api?type=venom&height=200&color=0ABAB5&text=Minishell&fontAlign=50&fontAlignY=61&animation=fadeIn&fontColor=fcf3f2&fontSize=100" />
</p>

<!-------About-------->

## About

This project is the implementation of a simple Unix shell, written in C as part of the 42 School Common Core curriculum. The objective is to gain a deep understanding of how command-line shells work at a system level: parsing input, handling processes, managing environment variables, and dealing with pipes and redirections.
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
1. Shell
   1.1. What is a Shell? Definition and Core Functionality
   1.2. Historical Context and Evolution
   1.3. Archictecture of a Shell
   1.4. Types of Shell
   1.5. Why Shell matters anyways?


<!-------Index-------->

## <a name="#index-0">Index</a>

<ul>
	<li><strong><a href="#1-shell" style="color:white">1. Shell </a></strong></li>
	<ul style="list-style-type:disc">
		<li><a href="#11-what-is-a-shell-definition-and-core-funtionality"> 1.1. What is a Shell? Definition and Core Functionality</a></li>
		<li><a href="#12-static-library-and-dynamic-library"> 1.2. Historical Context and Evolution</a></li>
    <li><a href="#13-static-library-and-dynamic-library"> 1.3. Archictecture of a Shell</a></li>
    <li><a href="#14-static-library-and-dynamic-library"> 1.4. Types of Shell</a></li>
    <li><a href="#15-static-library-and-dynamic-library"> 1.5. Why Shell matters anyways?</a></li>
	</ul>
	<li><strong><a href="#-2-preprocessors-" style="color:white">2. Preprocessors </a></strong></li>
	<ul style="list-style-type:disc">
		<li><a href="#-21-preprocessor-directives-in-c-"> 2.1. Preprocessor Directives in C</a></li>
		<li><a href="#22-types-of-preprocessors-directives"> 2.2. Types of Preprocessors Directives</a></li>
  		<ul style="list-style-type:square">
  			<li><a href="#221-macros"> 2.1.1. Macros </a></li>
			<li><a href="#222-file-inclusion"> 2.1.2. File Inclusion </a></li>
			<li><a href="#223-conditional-compilation"> 2.1.3. Conditional Compilation </a></li>
		</ul>
		<li><a href="#23-about-libraries-again"> 2.3. About Libraries... again </a></li>
	</ul>
	<li><strong><a href="#-3-building-our-first-c-library-" style="color:white">3. Building our first C Library </a></strong></li>
	<ul style="list-style-type:disc">
		<li><a href="#-31-the-preprocessor-directives-for-libft-"> 3.1. The Preprocessor Directives for Libft </a></li>
		<li><a href="#-32-the-functions-"> 3.2. The Functions </a></li>	
	</ul>
	<li><strong><a href="#-4-makefile-" style="color:white">4. Makefile </a></strong></li>
 	<ul style="list-style-type:disc">
		<li><a href="#-41-what-is-makefile-"> 4.1. What is Makefile? </a></li>
		<li><a href="#-42-why-do-we-need-makefile-"> 4.2. Why do we need Makefile? </a></li>
		<li><a href="#-43-how-to-makefile-"> 4.3. How to Makefile? </a></li>
 	</ul>
	<li><strong><a href="#-conclusion-" style="color:white">5. Conclusion </a></strong></li>
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




But, long story short and from a software engineering perspective, a shell is actually a **command interpreter** that:

* **Parses** user input into executable instructions.
* **Invokes** system calls (e.g., `fork()`, `execve()`, `wait()`) to delegate tasks to the kernel.
* **Manages** process control (foreground/background execution, job control).
* **Handles** I/O redirections, pipelines, and environment variables.

>[!IMPORTANT]
> The shell is not part of the kernel but runs in user space, relying on system calls to interact with the OS (Ritchie & Thompson, 1974).



### License

This work is published under the terms of <a href="https://github.com/lude-bri/42_Common_Core/blob/main/LICENSE.md">42 Unlicense</a>.

<p align="right">(<a href="#readme-top">get to top</a>)</p>

