
# What The Rack

WhatTheRack is a plugin for [VCV Rack](https://vcvrack.com). It
contains three modules:

* WhatTheRack, a rack randomizer
* WhatTheMod, a module randomizer
* WhatTheJack, a wire randomizer

## WhatTheRack, a rack randomizer

The eponymous module has a single button. When you press the button,
WhatTheRack will spawn 19 random modules plus an audio interface and a
scope. All modules' parameters are randomized, too.

The modules will include at least 2 VCOs, 2 LFOs, 2 VCAs, 1 sequencer,
1 clock, 3 effect modules, 2 filters, 2 envelopes, 1 mixer and 3
miscellaneous modules. This works by scanning for your installed
plugins, and going through the registered module models' tags.

This is designed to get you started with a Rack that should always be
able to make some music. It may not be intuitive how, but discovering
new modules is part of the fun, and it can inspire you to create
completely new patches you wouldn't have experienced otherwise.

### How to use

Easiest way to get started:

* Clear your patch (Ctrl+N)
* Add WhatTheRack
* Hit the *?* button
* Try to make music with what just appeared!
* Want more modules? Click the *?* button again. Or don't, and enjoy
  the challenge of working with what you got.

## WhatTheMod, a module randomizer

This module is similar to WhatTheRack, but it will spawn a single
module of a type of your choice (VCO, LFO, VCA, Sequencer, Clock,
Effect, Filter, Envelope generator, Mixer or Misc). Like WhatTheRack,
all parameters are randomized on the newly created module.

This is designed to help you get out of your routine - need a clock?
Why not use one you've never tried! Need a VCO and not sure which one?
Let fortune decide. I find it's more enjoyable if I actually commit to
using the modules it gives me instead of rolling again when I don't
immediately like what I got, but rerolling is always an option.

## WhatTheJack, a wire randomizer

WhatTheJack has a single button. When you press the button, a new wire
will be created at random. Any output from any module may be used, and
any free input from any module may also be used. If no input is
available, no wire is created.

This module is a work in progress. It tends to be a bit too random to
really stimulare creativity - patching cables completely at random
doesn't generally give you a very useful modulation or audio
path. Ideas on how to make this more fun to use are welcome, please
file Github issues or contact me directly!

## Is this serious?

Kinda. It started as a joke: VCV Rack is missing an essential part of
modular synthesis, that is, puzzling over someone else's rack. Why the
hell do you have 5 sample and holds and a single VCO? What were you
doing to set that sequencer to 13/32 time signature? How do you even
use this to make more than white noise? What The Rack recreates this
wonderful experience.

But as I developed it, it actually made me realize a few things about
how I use VCV Rack. I have a ton of installed modules I never used,
because I downloaded the plugin for a single module and ignored
everything else. I tend to always use certain chains of modules (this
clock, these sequencers, these oscillators, this filter, etc.) and
ignore the rest. Using the module gave me some new and fun challenges:
what can I do with these modules and only these modules? How can I use
this random selection of stuff to make good-sounding beats? This
brought me back to my first experience of modular synthesis, where I
had no idea what was going on, and it was great.

How you use it is up to you. I'm not 100% sure it can't crash your
Rack - the APIs don't really encourage doing this. But Andrew Belt
allowed this plugin on the plugin store, so it can't be all that
bad. Have fun.

## Contributing

I welcome Issues and Pull Requests to this repository if you have
suggestions for improvement. Note that I generally won't change a
module significantly after it's been released (i.e. after I've tagged
a version in the repo), so if your idea involves changing the
semantics of an existing module, please chat me up first.

This plugin is released under the WTF Public License.
