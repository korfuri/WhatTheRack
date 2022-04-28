#include "WhatTheRack.hpp"
#include "plugin.hpp"
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

  WhatTheJack() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
  }

  void process(const ProcessArgs &args) override {}

  static void walkWidgetTree(Widget* root,
			     std::vector<PortWidget*>* inputs,
			     std::vector<PortWidget*>* outputs) {
    for (Widget* w : root->children) {
      PortWidget* p = dynamic_cast<PortWidget*>(w);
      if (p) {	
        if (p->type == engine::Port::INPUT) {
          if (!APP->scene->rack->getTopCable(p)) {
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
    std::vector<PortWidget*> inputs, outputs;
    walkWidgetTree(APP->scene->rack, &inputs, &outputs);

    if (outputs.size() < 1 || inputs.size() < 1) {
      return;
    }

    // Pick a random input and output port
    int iidx = (int)random::u32() % inputs.size();
    int oidx = (int)random::u32() % outputs.size();

    // Create a wire
    CableWidget *w = new CableWidget;
    w->inputPort = inputs[iidx];
    w->outputPort = outputs[oidx];
    w->color = APP->scene->rack->getNextCableColor();
    w->updateCable();
    if (w->isComplete()) APP->scene->rack->addCable(w);
    history::CableAdd *h = new history::CableAdd;
    h->name = "create cable";
    h->setCable(w);
    APP->history->push(h);
  }
};

typedef CallbackButton<WhatTheJack> CB;

struct WhatTheJackWidget : ModuleWidget {
  WhatTheJackWidget(WhatTheJack *module) {
    setModule(module);
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/WhatTheJack.svg")));

    addChild(createWidget<ScrewSilver>(Vec(0, 0)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(0, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    std::shared_ptr<rack::Svg> idle = APP->window->loadSvg(asset::plugin(pluginInstance, "res/BoomButton/question_color.svg"));
    std::shared_ptr<rack::Svg> clicked = APP->window->loadSvg(asset::plugin(pluginInstance, "res/BoomButton/question_bw.svg"));
    addChild(CB::create(Vec(50, 272), [](WhatTheJack* module){
	  module->spawnWire();
	}, module, idle, clicked));
  }
};

Model *modelWhatTheJack = createModel<WhatTheJack, WhatTheJackWidget>("WhatTheJack");
