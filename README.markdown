Amati
======

Amati is a VST plugin for live-coding effects in the [Faust](https://faust.grame.fr/) programming language.
As of now it only provides basic functionality. I am making it public so as to get potential feedback.

Building
---------

_Amati has only been built on Linux yet. Do tell me if you tried to build it on something else, whether you were successful or not!_

### Dependencies

Amati is built using CMake. You need a recent enough version of CMake (3.16 or later).

You'll need to have [libfaust](https://faust.grame.fr/downloads/) installed.

### Getting the source code

Clone this repository with its submodule:
```
git clone --recurse-submodules https://github.com/glocq/Amati.git
```

### Building

* Create a directory `build` (or whatever name you see fit) inside the repository, and enter it:
```
cd Amati
mkdir build
cd build
```

* Run cmake with the parent directory as the source directory
```
cmake ..
```

* _If on Linux_, run make:
```
make
```
Your plugin should now be in `Amati_artefacts/VST3/`.

* _If on other OSs..._ There should be project files somewhere, that you can open and build using some IDE.


Using
------

The graphic interface is composed of three tabs.

* On the _Editor_ tab, you can input your Faust code. Once you're done, compile it by clicking on _Compile_. If the compilation is successful, a success message will display in the _Console_ tab, and the corresponding code will be saved and used for the audio processing.
* On the _Parameters_ tab, you can edit the parameters of your effect just like in a traditional audio plugin. Please note the following two things:
    - By default, the number of possible parameters is restricted to 16. If your Faust program needs more, you will need to have set the `PARAM_COUNT` variable to another value when building Amati (see "Building" section above). Also, if you are using a DAW to automate some parameters, 16 parameters will appear to be automatable. If your effect uses, say, 3 parameters, the relevant DAW parameters are the first 3 ones. This is due to the number of parameters in a VST not being dynamically editable.
    - Parameters are restricted to values between 0 and 1, at least for now. So make sure the parameters defined in your Faust code are set to be between 0 and 1 too!
* On the _Console_ tab, messages regarding Faust compilation are displayed.

Contributing
-------------

The issues labeled with "todo" describe features I'd like to see implemented. Feel free to tackle them, or just weigh in or ask for clarification by commenting on them.

If you do write some code:

* Be as clear as possible. As you would expect, I only intend to integrate code I understand, and I'm not the most experienced with JUCE, or even C++. Still, if you contribute, be assured that I appreciate it, and I will put in a reasonable amount of effort to understand what you did.
* Keep it to one feature/fix per commit.
* Be OK with having your code changed before integration.

Of course, I'll credit you.

Author
-------

[Grégoire Locqueville](https://github.com/glocq/)


Credits
--------

`FaustCodeTokenizer.h` and `FaustCodeTokenizer.cpp` files copied (and slightly modified) from [Oliver Larkin](https://github.com/olilarkin)'s [juce_faustllvm](https://github.com/olilarkin/juce_faustllvm) project.
