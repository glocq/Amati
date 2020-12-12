Amati
======

Amati is a VST plugin for live-coding effects in the [Faust](https://faust.grame.fr/) programming language.
As of now it only provides basic functionality. I am making it public so as to get potential feedback.

Building
---------

_Amati has only been built on Linux yet. Do tell me if you tried to build it on something else, whether you were successful or not!_

Clone this repository with its submodule:
```
git clone --recurse-submodules https://github.com/glocq/Amati.git
```

Using the [Projucer](https://juce.com/discover/projucer), open and save the `Amati.jucer` file in the `Amati/` directory. If Amati will possibly be used to compile an effect with more than 16 parameters, edit the `PARAM_COUNT` variable accordingly before saving.

Note that if the Projucer has not been built with the `JUCER_ENABLE_GPL_MODE` set to 1, it will complain about the splash screen option. To deal with that, you can either:

* Re-enable the JUCE splash screen: from the Projucer, click on the small gear on the right of "Amati" in the top left corner of the interface, find the option "Display the JUCE Splash Screen", select "Enabled", and re-save the project,
* Or you can rebuild the Projucer with the GPL mode flag set to 1, and save the project with that version of the Projucer. That way you'll have a splash-screen-free plugin.

You can now close the Projucer. You should now have a directory `Builds/LinuxMakefile/` in `Amati/`. Navigate to that directory, and run
```
make
```

This will build Amati as a VST3 plugin. The files are in the `build/` directory.

Using
------

The graphic interface is composed of three tabs.

* On the _Editor_ tab, you can input your Faust code. Once you're done, compile it by clicking on _Compile_. If the compilation is successful, a success message will display in the _Console_ tab, and the corresponding code will be saved and used for the audio processing.
* On the _Parameters_ tab, you can edit the parameters of your effect just like in a traditional audio plugin. Please note the following two things:
    - By default, the number of possible parameters is restricted to 16. If your Faust program needs more, you will need to have set the `PARAM_COUNT` variable to another value when building Amati (see "Building" section above). Also, if you are using a DAW to automate some parameters, 16 parameters will appear to be automatable. If your effect uses, say, 3 parameters, the relevant DAW parameters are the first 3 ones. This is due to the number of parameters in a VST not being dynamically editable.
    - Parameters are restricted to values between 0 and 1, at least for now. So make sure the parameters defined in your Faust code are set to be between 0 and 1 too!
* On the _Console_ tab, messages regarding Faust compilation are displayed.

"Planned" features
-------------------

Those are features I would like to implement at some point, but I can't guarantee they ever will be.

* MIDI support. This would allow definition of instruments, not just audio effects.
* Displaying Faust diagrams. Diagrams are a crucial tool for coding in Faust, so that would be nice.

