# Blades of Exile
 Blades of Exile refactor exercise

# What is this all about?
This repository is an exercise in refactoring an old C codebase into something cleaner and leveraging the as much of the (post?) modern C++ language and tooling as possible to help me on that journey.

This really is just a technological puzzle to be solved using C++ as much as possible.

I'm hoping it might be interesting to folks intrigued about how to safely modernize legacy codebases and possible persuade the occasional diehard C engineer that C++ might just have some advantages over C :grinning:

I hope to explain why I am making specific changes and refactors as I go along.

# What isn't this all about?
The objective is not to produce an improved Blades of Exile! There's a great group of people who did that years ago here: https://github.com/calref/cboe

# Where did the original source come from?
The legend that is indie game developer Jeff Vogel of https://spiderwebsoftware.com/ developed this a long, long time ago (1997). In 2007 he did a wonderful, wonderful thing and decided to release the source to the community so that it could serve as instruction and inspiration. See https://www.spiderwebsoftware.com/blades/opensource.html for that post.

# Why did you choose this source?
I really like Jeff's games and I really like his humility in releasing source which, in his own words: "It is old and outdated. I wrote it early in my career, and parts of it are pretty darn clunky."

It's also got plenty of problems, and challenging ones too. At the very least it was build as a 16-bit application, so that is likely to cause us some challenges as we try to build it for 32-bit/64-bit architectures.

It's also written in C. Religious warning alert!!! I'm not about to bash C it has plenty of excellent qualities, but it also can be difficult to work safely with and it doesn't give us a huge amount of help on with matters like type safety without us doing lots of extra work.

# Why did you choose C++?
I think that (post?) modern C++ can aid refactoring and clarity of code without necessarily introducing huge amounts of complexity.

Specifically I hope to show that relatively minor substitutions of C++ mechanisms can not only improve runtime error detection, but also spot more issues at compile time.

Fan's of almost any modern language (rust?) will possibly be chuckling at the apparent irony of a someone choosing to rewrite C as C++ to improve it's safety, but I hope to show it is possible to write safe'ish C++ all the same.

Am I right? You'll be the judges! And you can always rewrite it in rust :grinning:

# But why did you really choose C++?
In my day job I write primarily in C++, C and Python - but old platform support means the compilers I'm stuck with are *very* out of date and even something as simple as C++11 standard is more than I can hope for. This also tends to mean I can't always leverage some of the amazing tooling that comes with modern C++ toolchains, such as all sorts of compile time checking and runtime sanitizers.

In my personal projects I try and use the latest and greatest C++ features and libraries, so here's yet another chance to do that and stretch my brain muscles.

# Was the initial push a straight upload of the original?
No, I confess I cut a few corners here to make things easier for myself and anyone else trying to follow what I am doing. Specifically:

1) The Mac source code had old school Mac line endings in the source (a single linefeed). This tends to confuse even modern source control, so I converted any files with those line endings.
2) The Windows filenames were almost all in uppercase (presumably as it came as DOS 8.3 filename format) but the source included it typically as lower case, and occasionally a mixture. Since I ultimately want to be able to build the source on any platform, including those with case sensitive filenames, I renamed those upper case files to be lower case to match the #include entries in the source.
3) Technically this source is actually C rather than C++ yet some of the files have C++ file extensions. Since it's actually my plan to compile this all as C++ to immediately benefit from some of that type safety, I decided to rename all C extensions to C++. This is actually not generally what I would recommend to someone working with a legacy codebase: I'd suggest getting it compiling and have some tests against it first before moving to compile it as C++, but as this is old "clunky" code (Jeff's word!) I really want some of that C++ goodness ASAP.

# Do you have a plan of attack?
One of the reasons I like source control systems like Git is that I can try things out and see if they work before ultimately deciding if they are a good idea, or at least for now a good idea.

Anyone used to refactoring code is aware how difficult it can be to review code where multiple elements have changed at the same time: the differences can be very hard to validate that the code is still doing the same thing. Now I'm sure you're all very good developers who write unit tests to prove that, but that isn't the situation we're in here - so we're going to want to make very easily observable changes so that anyone reviewing can see exactly what we did.

I've often found the best way to do this is just to try it out a refactoring, and if it makes sense repeat the process in a series of straightforward steps. This can look a little odd since sometimes you might appear to be changing code for no particularly obvious reason, and it won't be apparent why until the final change slots everything into place - but with legacy code I find this to work well for me and for my poor reviewer.