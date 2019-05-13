#include "WhatTheRack.hpp"
#include "plugin.hpp"
#include "window.hpp"
#include "CallbackButton.hpp"

struct WhatTheJack : Module {
  enum ParamIds {
    GENERATE_BUTTON,
    NUM_PARAMS
  };
  enum InputIds {
    NUM_INPUTS
  };
  enum OutputIds {
    NUM_OUTPUTS
  };
  enum LightIds {
    NUM_LIGHTS
  };

  WhatTheJack() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

  void step() override {}

  static void walkWidgetTree(Widget* root,
			     std::vector<Port*>* inputs,
			     std::vector<Port*>* outputs) {
    for (Widget* w : root->children) {
      Port* p = dynamic_cast<Port*>(w);
      if (p) {
	if (p->type == Port::INPUT) {
	  if (!gRackWidget->wireContainer->getTopWire(p)) {
	    inputs->push_back(p);
	  }
	} else {
	  outputs->push_back(p);
	}
      } else {
	walkWidgetTree(w, inputs, outputs);
      }
    }
  }

  void spawnWire() {
    // List all input and output ports in the Rack
    std::vector<Port*> inputs, outputs;
    walkWidgetTree(gRackWidget, &inputs, &outputs);

    if (outputs.size() < 1 || inputs.size() < 1) {
      return;
    }

    // Pick a random input and output port
    int iidx = randomu32() % inputs.size();
    int oidx = randomu32() % outputs.size();

    // Create a wire
    WireWidget* w = new WireWidget();
    w->inputPort = inputs[iidx];
    gRackWidget->wireContainer->setActiveWire(w);
    w->hoveredOutputPort = outputs[oidx];
    gRackWidget->wireContainer->commitActiveWire();
  }
};

typedef CallbackButton<WhatTheJack> CB;

struct WhatTheJackWidget : ModuleWidget {
  WhatTheJackWidget(WhatTheJack *module) : ModuleWidget(module) {
    setPanel(SVG::load(assetPlugin(plugin, "res/WhatTheJack.svg"))); // fixme

    addChild(Widget::create<ScrewSilver>(Vec(0, 0)));
    addChild(Widget::create<ScrewSilver>(Vec(box.size.x - RACK_GRID_WIDTH, 0)));
    addChild(Widget::create<ScrewSilver>(Vec(0, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(Widget::create<ScrewSilver>(Vec(box.size.x - RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    std::shared_ptr<rack::SVG> idle = SVG::load(assetPlugin(plugin, "res/BoomButton/question_color.svg"));
    std::shared_ptr<rack::SVG> clicked = SVG::load(assetPlugin(plugin, "res/BoomButton/question_bw.svg"));
    addChild(CB::create(Vec(50, 272), [](WhatTheJack* module){
	  module->spawnWire();
	}, module, idle, clicked));
  }
};

Model *modelWhatTheJack = Model::create<WhatTheJack, WhatTheJackWidget>("WhatTheRack", "WhatTheJack", "WhatTheJack - Wire randomizer", UTILITY_TAG);
