#include "WhatTheRack.hpp"
#include "plugin.hpp"
#include "window.hpp"
#include "CallbackButton.hpp"
#include <algorithm>
#include <functional>
#include <random>

static std::random_device rd;
static std::mt19937 g(rd());

void SpawnModel(Model* model) {
  ModuleWidget *moduleWidget = model->createModuleWidget();
  if (!moduleWidget) {
    return;
  }
  gRackWidget->addModule(moduleWidget);
  moduleWidget->box.pos = Vec(0, 0);
  gRackWidget->requestModuleBoxNearest(moduleWidget, moduleWidget->box);
  moduleWidget->randomize();
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

  WhatTheRack() :
    Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS)
  {
    for (const auto& p : rack::gPlugins) {
      for (const auto& m : p->models) {
	for (const auto& t : m->tags) {
	  switch (t) {
	  case rack::OSCILLATOR_TAG:
	    vcos.push_back(m); break;
	  case rack::LFO_TAG:
	    lfos.push_back(m); break;
	  case AMPLIFIER_TAG:
	  case ENVELOPE_FOLLOWER_TAG:
	    vcas.push_back(m); break;
	  case SEQUENCER_TAG:
	    sequencers.push_back(m); break;
	  case CLOCK_TAG:
	    clocks.push_back(m); break;
	  case COMPRESSOR_TAG:
	  case DELAY_TAG:
	  case DISTORTION_TAG:
	  case EFFECT_TAG:
	  case FLANGER_TAG:
	  case PANNING_TAG:
	  case PHASER_TAG:
	  case REVERB_TAG:
	  case RING_MODULATOR_TAG:
	  case VOCODER_TAG:
	  case WAVESHAPER_TAG:
	    effects.push_back(m); break;
	  case FILTER_TAG:
	    filters.push_back(m); break;
	  case ENVELOPE_GENERATOR_TAG:
	    envelopes.push_back(m); break;
	  case CLOCK_MODULATOR_TAG:
	  case SAMPLE_AND_HOLD_TAG:
	  case RECORDING_TAG:
	  case SLEW_LIMITER_TAG:
	  case SWITCH_TAG:
	  case UTILITY_TAG:
	    miscs.push_back(m); break;
	  case MIXER_TAG:
	    mixers.push_back(m); break;
	  default:
	    // Ignore unknown tags.
	    break;
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

  void step() override {}
};

typedef CallbackButton<WhatTheRack> CB;

struct WhatTheRackWidget : ModuleWidget {
  WhatTheRackWidget(WhatTheRack *module) : ModuleWidget(module) {
    setPanel(SVG::load(assetPlugin(plugin, "res/WhatTheRack.svg")));

    addChild(Widget::create<ScrewSilver>(Vec(0, 0)));
    addChild(Widget::create<ScrewSilver>(Vec(box.size.x - RACK_GRID_WIDTH, 0)));
    addChild(Widget::create<ScrewSilver>(Vec(0, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(Widget::create<ScrewSilver>(Vec(box.size.x - RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    std::shared_ptr<rack::SVG> idle = SVG::load(assetPlugin(plugin, "res/BoomButton/question_color.svg"));
    std::shared_ptr<rack::SVG> clicked = SVG::load(assetPlugin(plugin, "res/BoomButton/question_bw.svg"));
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
  WhatTheModWidget(WhatTheRack *module) : ModuleWidget(module) {
    setPanel(SVG::load(assetPlugin(plugin, "res/WhatTheMod.svg")));

    addChild(Widget::create<ScrewSilver>(Vec(0, 0)));
    addChild(Widget::create<ScrewSilver>(Vec(box.size.x - RACK_GRID_WIDTH, 0)));
    addChild(Widget::create<ScrewSilver>(Vec(0, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(Widget::create<ScrewSilver>(Vec(box.size.x - RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    std::shared_ptr<rack::SVG> idle = SVG::load(assetPlugin(plugin, "res/BoomButton/question_color.svg"));
    std::shared_ptr<rack::SVG> clicked = SVG::load(assetPlugin(plugin, "res/BoomButton/question_bw.svg"));

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

Model *modelWhatTheRack = Model::create<WhatTheRack, WhatTheRackWidget>("WhatTheRack", "WhatTheRack", "WhatTheRack - Rack randomizer", UTILITY_TAG);
Model *modelWhatTheMod = Model::create<WhatTheRack, WhatTheModWidget>("WhatTheRack", "WhatTheMod", "WhatTheMod - Module randomizer", UTILITY_TAG);
