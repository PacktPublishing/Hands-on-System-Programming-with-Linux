


# Hands-On System Programming with Linux

<a href="https://www.packtpub.com/networking-and-servers/hands-system-programming-linux?utm_source=github&utm_medium=repository&utm_campaign=9781788998475 "><img src="https://d255esdrn735hr.cloudfront.net/sites/default/files/imagecache/ppv4_main_book_cover/B09991_New_cover.png" alt="Hands-On System Programming with Linux" height="256px" align="right"></a>

This is the code repository for [Hands-On System Programming with Linux](https://www.packtpub.com/networking-and-servers/hands-system-programming-linux?utm_source=github&utm_medium=repository&utm_campaign=9781788998475), published by Packt.

**Explore Linux system programming interfaces, theory, and practice**

## What is this book about?
The Linux OS and its embedded and server applications are critical components of today’s software infrastructure in a decentralized, networked universe. The industry's demand for proficient Linux developers is only rising with time. Hands-On System Programming with Linux gives you a solid theoretical base and practical industry-relevant descriptions, and covers the Linux system programming domain. It delves into the art and science of Linux application programming— system architecture, process memory and management, signaling, timers, pthreads, and file IO.

This book covers the following exciting features:
* Explore the theoretical underpinnings of Linux system architecture 
* Understand why modern OSes use virtual memory and dynamic memory APIs 
* Get to grips with dynamic memory issues and effectively debug them 
* Learn key concepts and powerful system APIs related to process management 
* Effectively perform file IO and use signaling and timers 
Deeply understand multithreading concepts, pthreads APIs, synchronization and scheduling 

If you feel this book is for you, get your [copy](https://www.amazon.com/dp/1788998472) today!

<a href="https://www.packtpub.com/?utm_source=github&utm_medium=banner&utm_campaign=GitHubBanner"><img src="https://raw.githubusercontent.com/PacktPublishing/GitHub/master/GitHub.png" 
alt="https://www.packtpub.com/" border="5" /></a>

## Instructions and Navigations

- ***ch'n'/*** : a directory for each chapter, named as *ch'n' ; (n=1, 2, ..., 18), A_fileio, B_daemon*.
  Each chapter directory will further contain:
   - the source code: Makefile, 'C' source and header files for that chapter
   - a text file named in the format  ***assignment_ch'n'.txt*** ; which of course
     will contain all prescribed assignments to the reader for this chapter.

- ***solutions\_to\_assgn/ch'n'*** : a directory, which will contain only *selected solutions* to assignments prescribed. The assignments themselves are in the *ch'n'* directory with the name *assignment_ch'n'.txt*. The solutions are organized by chapter,  each chapter (that requires this) will have a directory within which you can find selected solutions. The 'solutions' are mostly in the form of code (along with the associated Makefile, if required); on occasion, the   assignment is a "question type" and the answer can be found in a text file  named as *ch'n'\_solutions.txt* .

- ***Hardware\_Software\_List.pdf*** : details on the hardware and software requirements for getting the best experience with working with the book's codebase [*[download link](https://github.com/PacktPublishing/Hands-on-System-Programming-with-Linux/blob/master/Hardware_Software_List.pdf)*].

- the ***Further\_reading.md*** file : details, in a chapter-wise manner,
  useful online references as well as books; these will benefit the reader
  in learning more [*[download link](https://github.com/PacktPublishing/Hands-on-System-Programming-with-Linux/blob/master/Further_reading.md)*].


We definitely encourage the reader to clone (and try out the code!) like so:

    git clone https://github.com/PacktPublishing/Hands-on-System-Programming-with-Linux

Once cloned, we suggest you first browse through the [*Hardware\_Software\_List.pdf*](https://github.com/PacktPublishing/Hands-on-System-Programming-with-Linux/blob/master/Hardware_Software_List.pdf) file; it details the hardware and software requirements for getting the best experience with working with the book's codebase. It mentions which packages are required, which are optional, and, wherever possible, how to install it.

Next, we suggest you proceed to build and try out the code examples.
Also, a lot can be learned by working on the prescribed assignments for each chapter.


The code will look like the following:
```
include <pthread.h>
int pthread_mutexattr_gettype(const pthread_mutexattr_t *restrict attr,     int *restrict type);
int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);
```

**Following is what you need for this book:**
Hands-On System Programming with Linux is for Linux system engineers, programmers, or anyone who wants to go beyond using an API set to understanding the theoretical underpinnings and concepts behind powerful Linux system programming APIs. To get the most out of this book, you should be familiar with Linux at the user-level logging in, using shell via the command line interface, the ability to use tools such as find, grep, and sort. Working knowledge of the C programming language is required. No prior experience with Linux systems programming is assumed.


We also provide a PDF file that has color images of the screenshots/diagrams used in this book. [Click here to download it](https://www.packtpub.com/sites/default/files/downloads/9781788998475_ColorImages.pdf).

### Related products
* Hands-On Linux Administration on Azure [[Packt]](https://www.packtpub.com/virtualization-and-cloud/hands-linux-administration-azure?utm_source=github&utm_medium=repository&utm_campaign=9781789130966) [[Amazon]](https://www.amazon.com/dp/1789130964)

* Practical Linux Security Cookbook - Second Edition [[Packt]](https://www.packtpub.com/networking-and-servers/practical-linux-security-cookbook-second-edition?utm_source=github&utm_medium=repository&utm_campaign=9781789138399) [[Amazon]](https://www.amazon.com/dp/1789138396)

## Get to Know the Author
**Kaiwan N Billimoria**
taught himself programming on his dad's IBM PC back in 1983. He was programming in C and Assembly on DOS until he discovered the joys of Unix (via Richard Steven's iconic book, UNIX Network Programming, and by writing C code on SCO Unix).
Kaiwan has worked on many aspects of the Linux system programming stack, including Bash scripting, system programming in C, kernel internals, and embedded Linux work. He has actively worked on several commercial/OSS projects. His contributions include drivers to the mainline Linux OS, and many smaller projects hosted on GitHub. His Linux passion feeds well into his passion for teaching these topics to engineers, which he has done for over two decades now. It doesn't hurt that he is a recreational ultra-marathoner too.

## Other books by the authors
### Suggestions and Feedback
[Click here](https://docs.google.com/forms/d/e/1FAIpQLSdy7dATC6QmEL81FIUuymZ0Wy9vH1jHkvpY57OiMeKGqib_Ow/viewform) if you have any feedback or suggestions.


### Download a free PDF

 <i>If you have already purchased a print or Kindle version of this book, you can get a DRM-free PDF version at no cost.<br>Simply click on the link to claim your free PDF.</i>
<p align="center"> <a href="https://packt.link/free-ebook/9781788998475">https://packt.link/free-ebook/9781788998475 </a> </p>