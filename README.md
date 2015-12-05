# Scissum
This is a project to make a game engine that can play Sierra SCI32 adventure games.
I am currently using Leisure Suit Larry 7 as a testing platform.

We'll see if I find the time to finish it :).

# Progress
* Resource Loading
  - Extraction and parsing of Picture, View and Script (needs a little work) resources is working.
* Virtual Machine
  - Most instructions are implemented, a few are remaining. Using a simple big switch mechanism for
    running the instructions. I am thinking of trying to convert the script to LLVM, because I like
    to learn more about LLVM as well, but for now decided that I first should create a working
    engine before doing that.
  - Kernel functions still need to be implemented.
  - Implemented a custom memory heap allocator for the VM.
    - For now this is a first-fit allocator, but I'm working on a bit wise trie (already working, but needs so refactoring and optimisation) to make an efficient best-fit version.
- Also created a STL allocator for the custom heap so STL objects can be store inside the VM heap (which is actually kind of cool ;)).
* Graphics
  - Only some containers implemented (Color, Image, Palette) and an ImageSaver for the PNM format.
  - Plan on using OpenGL for the graphics. I don't plan on mimicking the same drawing techniques used
    by the original engine. I want the engine to support higher resolution graphics as well, just in
    case...
* Sound
  - Nothing yet...

# Existing Engines
ScummVM already plays most of the SCI based games and I can see in the sources
that there is support for SCI32, so LSL7 support might be close.

#But, Why??
For now this project was started because I liked to make a game engine but
lack the skills and time for content creation, so why not start by making
a game engine for a game that already exists? :). Also I always really liked LSL7
when I was younger, so that is a good motivation (and I'm ignoring that fact that
DosBox exists...).

# References
Most information I gathered from the following places (together with a good Hex Editor ;)).

- ScummVM source code.
- wiki.scummvm.org
- https://web.archive.org/web/20070608065707/http://freesci.linuxgames.com/scihtml/x971.html
- Sources from: http://sci.sierrahelp.com/Tools/SCITools.html (Most notably SCI Resources Dumper 1.2).

The LZS (STACT) compression information I found at the following locations.
- ANSI X3.241-1994: http://masters.donntu.org/2003/fvti/boykov/library/lzs.pdf
- RFC1967: PPP LZS-DCP Compression Protocol (LZS-DCP)(https://tools.ietf.org/html/rfc1967#section-2.5.7)
