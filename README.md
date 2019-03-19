
# What The Rack

WhatTheRack is a plugin for [VCV Rack](https://vcvrack.com). It
contains a single eponymous module with a single button. When you
press the button, What The Rack will add 19 random modules plus an
audio interface and a scope. All modules' parameters are randomized,
too.

The modules will include at least 2 VCOs, 2 LFOs, 2 VCAs, 1 sequencer,
1 clock, 3 effect modules, 2 filters, 2 envelopes, 1 mixer and 3
miscellaneous modules. This works by scanning for your installed
plugins, and going through the registered module models' tags.

## How to use

Easiest way to get started:

* Clear your patch (Ctrl+N)
* Add WhatTheRack
* Hit the *?* button
* Try to make music with what just appeared!
* Want more modules? Click the *?* button again. Or don't, and enjoy
  the challenge of working with what you got.

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
Rack - the APIs don't really encourage doing this, and I'm pretty sure
Andrew Belt said modules shouldn't do this kind of things although I
can't find that conversation anymore. Have fun.

## Contributing

I welcome Issues and Pull Requests to this repository if you have
suggestions for improvement.

This template is released under the WTF Public License.
