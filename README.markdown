Amati
======

Amati is a VST plugin for live-coding effects in the [Faust](https://faust.grame.fr/) programming language.

Status of the project
----------------------

Amati is functional and allows the creation and use of effects. Some features are still missing, see the issues labeled with "todo" in the issue tracker. The most notable missing feature according to me is the inability to take MIDI as input.

Building
---------

As of now, only the VST version has been built successfully, on Linux and MacOS. Any feedback about building attempts is appreciated.

### Dependencies

Amati is built using CMake. You need a recent enough version of CMake (3.16 or later).

You'll need to have [libfaust](https://faust.grame.fr/downloads/) installed.

### Getting the source code

Clone this repository:
```
git clone https://github.com/glocq/Amati.git
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

* On the _Editor_ tab, you can input your Faust code.
    - Clicking the _Compile_ compiles the code currently in the editor. If the compilation is successful, a success message is displayed in the _Console_ tab, and the corresponding code is saved and used for the audio processing.
    - Clicking _Import..._ loads the source from an external file.
    - Clicking _Export..._ saves the code into an external file.
    - Clicking _Revert..._ button reverts loads the source code of the program currently in use.
* On the _Parameters_ tab, you can edit the parameters of your effect just like in a traditional audio plugin. Please note the following two things:
    - By default, the number of possible parameters is restricted to 16. If your Faust program needs more, you will need to have set the `PARAM_COUNT` variable to another value when building Amati (see "Building" section above). Also, if you are using a DAW to automate some parameters, 16 parameters will appear to be automatable. If your effect uses, say, 3 parameters, the relevant DAW parameters are the first 3 ones. This is due to the number of parameters in a VST not being dynamically editable.
    - Parameters are restricted to values between 0 and 1, at least for now. So make sure the parameters defined in your Faust code are set to be between 0 and 1 too!
* On the _Console_ tab, messages regarding Faust compilation are displayed.

Contributing
-------------

The issues labeled with "todo" describe features I'd like to see implemented. Feel free to tackle them, or just weigh in or ask for clarification by commenting on them.

If you do write some code:

* Be as clear as possible, including to non-experts. I'm not the most experienced with JUCE, or even C++, and I'd like to understand the code I integrate. Still, if you contribute, be assured that I appreciate it, and I will put in a reasonable amount of effort to understand what you did.
* Keep it to one feature/fix per commit.
* Be OK with having your code changed before integration.
* Please add your name to the copyright notice of the files you modified.
* Add yourself to the contributor list below.

Contributors
-------------

Author: [Grégoire Locqueville](https://github.com/glocq/)

Contributors: 
 * [Juan Sierra](https://www.juansaudio.com/)
 * [Kamil Kisiel](https://www.kamilkisiel.net/)
 * [GRAME](https://www.grame.fr/)


Credits
--------

`FaustCodeTokenizer.h` and `FaustCodeTokenizer.cpp` files copied (and slightly modified) from [Oliver Larkin](https://github.com/olilarkin)'s [juce_faustllvm](https://github.com/olilarkin/juce_faustllvm) project.
