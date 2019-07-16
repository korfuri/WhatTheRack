#include "WhatTheRack.hpp"
#include "plugin.hpp"
#include "window.hpp"
#include "logger.hpp"
#include "CallbackButton.hpp"
#include <algorithm>
#include <functional>
#include <random>

static std::mt19937 g(random::u32());

void SpawnModel(Model* model) {
  INFO("WhatTheRack will spawn a %s/%s module.", model->plugin->slug.c_str(), model->slug.c_str());
  ModuleWidget *moduleWidget = model->createModuleWidget();
  if (!moduleWidget) {
    WARN("WhatTheRack was unable to spawn a %s/%s module.", model->plugin->slug.c_str(), model->slug.c_str());
    return;
  }
  APP->scene->rack->addModuleAtMouse(moduleWidget);
  history::ModuleAdd *h = new history::ModuleAdd;
  h->name = "create module";
  h->setModule(moduleWidget);
  APP->history->push(h);
  moduleWidget->randomizeAction();
  INFO("WhatTheRack successfully spawned a %s/%s module.", model->plugin->slug.c_str(), model->slug.c_str());
}

void SpawnAFewModels(std::vector<Model*>& models, int n) {
  std::shuffle(models.begin(), models.end(), g);
  for (auto it = models.begin(); it != models.end() && n > 0; ++it, --n) {
    Model* m = *it;
    SpawnModel(m);
  }
}

struct WhatTheRack : Module {
  enum ParamIds {
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

  WhatTheRack() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    for (const auto& p : rack::plugin::plugins) {
      for (const auto& m : p->models) {
	for (const auto& t : m->tags) {
	  if (t == "VCO") {
	    vcos.push_back(m);
	  }
	  if (t == "LFO") {
	    lfos.push_back(m);
	  }
	  if (t == "VCA" ||
	      t == "Envelope follower") {
	    vcas.push_back(m);
	  }
	  if (t == "Sequencer") {
	    sequencers.push_back(m);
	  }
	  if (t == "Clock generator") {
	    clocks.push_back(m);
	  }
	  if (t == "Compressor" ||
	      t == "Delay" ||
	      t == "Distortion" ||
	      t == "Effect" ||
	      t == "Flanger" ||
	      t == "Panning" ||
	      t == "Phaser" ||
	      t == "Reverb" ||
	      t == "Ring modulator" ||
	      t == "Vocoder" ||
	      t == "Waveshaper") {
	    effects.push_back(m);
	  }
	  if (t == "VCF") {
	    filters.push_back(m);
	  }
	  if (t == "Envelope generator") {
	    envelopes.push_back(m);
	  }
	  if (t == "Clock modulator" ||
	      t == "Sample and hold" ||
	      t == "Recording" ||
	      t == "Slew limiter" ||
	      t == "Switch" ||
	      t == "Utility") {
	    miscs.push_back(m);
	  }
	  if (t == "Mixer") {
	    mixers.push_back(m);
	  }
	}

	if ((p->slug == "Core" && m->slug == "AudioInterface") ||
	    (p->slug == "Fundamental" && m->slug == "Scope")) {
	  basics.push_back(m); break;
	}
      }
    }
  }

  std::vector<Model*> vcos;
  std::vector<Model*> lfos;
  std::vector<Model*> vcas;
  std::vector<Model*> sequencers;
  std::vector<Model*> clocks;
  std::vector<Model*> effects;
  std::vector<Model*> filters;
  std::vector<Model*> envelopes;
  std::vector<Model*> miscs;
  std::vector<Model*> mixers;
  std::vector<Model*> basics;

  void process(const ProcessArgs &args) override {}
};

typedef CallbackButton<WhatTheRack> CB;

struct WhatTheRackWidget : ModuleWidget {
  WhatTheRackWidget(WhatTheRack *module) {
    setModule(module);
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/WhatTheRack.svg")));

    addChild(createWidget<ScrewSilver>(Vec(0, 0)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(0, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    std::shared_ptr<rack::Svg> idle = APP->window->loadSvg(asset::plugin(pluginInstance, "res/BoomButton/question_color.svg"));
    std::shared_ptr<rack::Svg> clicked = APP->window->loadSvg(asset::plugin(pluginInstance, "res/BoomButton/question_bw.svg"));
    addChild(CB::create(Vec(50, 272), [](WhatTheRack* module){
	  SpawnAFewModels(module->vcos, 2);
	  SpawnAFewModels(module->lfos, 2);
	  SpawnAFewModels(module->vcas, 2);
	  SpawnAFewModels(module->sequencers, 1);
	  SpawnAFewModels(module->clocks, 1);
	  SpawnAFewModels(module->effects, 3);
	  SpawnAFewModels(module->filters, 2);
	  SpawnAFewModels(module->envelopes, 2);
	  SpawnAFewModels(module->miscs, 3);
	  SpawnAFewModels(module->mixers, 1);
	  SpawnAFewModels(module->basics, 10);
	}, module, idle, clicked));
  }
};

struct WhatTheModWidget : ModuleWidget {
  WhatTheModWidget(WhatTheRack *module) {
		setModule(module);
    setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/WhatTheMod.svg")));

    addChild(createWidget<ScrewSilver>(Vec(0, 0)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(0, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    std::shared_ptr<rack::Svg> idle = APP->window->loadSvg(asset::plugin(pluginInstance, "res/BoomButton/question_color.svg"));
    std::shared_ptr<rack::Svg> clicked = APP->window->loadSvg(asset::plugin(pluginInstance, "res/BoomButton/question_bw.svg"));
    addChild(CB::create(Vec(20, 63), [](WhatTheRack* m){ SpawnAFewModels(m->vcos, 1); }, module, idle, clicked));
    addChild(CB::create(Vec(20, 95), [](WhatTheRack* m){ SpawnAFewModels(m->lfos, 1); }, module, idle, clicked));
    addChild(CB::create(Vec(20, 127), [](WhatTheRack* m){ SpawnAFewModels(m->vcas, 1); }, module, idle, clicked));
    addChild(CB::create(Vec(20, 159), [](WhatTheRack* m){ SpawnAFewModels(m->sequencers, 1); }, module, idle, clicked));
    addChild(CB::create(Vec(20, 191), [](WhatTheRack* m){ SpawnAFewModels(m->clocks, 1); }, module, idle, clicked));
    addChild(CB::create(Vec(20, 223), [](WhatTheRack* m){ SpawnAFewModels(m->effects, 1); }, module, idle, clicked));
    addChild(CB::create(Vec(20, 255), [](WhatTheRack* m){ SpawnAFewModels(m->filters, 1); }, module, idle, clicked));
    addChild(CB::create(Vec(20, 287), [](WhatTheRack* m){ SpawnAFewModels(m->envelopes, 1); }, module, idle, clicked));
    addChild(CB::create(Vec(20, 319), [](WhatTheRack* m){ SpawnAFewModels(m->mixers, 1); }, module, idle, clicked));
    addChild(CB::create(Vec(20, 351), [](WhatTheRack* m){ SpawnAFewModels(m->miscs, 1); }, module, idle, clicked));
  }
};

Model *modelWhatTheRack = createModel<WhatTheRack, WhatTheRackWidget>("WhatTheRack");
Model *modelWhatTheMod = createModel<WhatTheRack, WhatTheModWidget>("WhatTheMod");
