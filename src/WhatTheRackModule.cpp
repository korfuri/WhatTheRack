#include "WhatTheRack.hpp"
#include "plugin.hpp"
#include <algorithm>
#include <random>
#include <thread>

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
    // std::thread t ([m](){SpawnModel(m);});
    // t.detach();
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

struct BoomButton : SVGButton {
  BoomButton() {
    setSVGs(SVG::load(assetPlugin(plugin, "res/BoomButton/question_color.svg")),
	    SVG::load(assetPlugin(plugin, "res/BoomButton/question_bw.svg")));
  }

  virtual void onAction(EventAction &e) override {
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
  }

  WhatTheRack* module;
};

struct WhatTheRackWidget : ModuleWidget {
  WhatTheRackWidget(WhatTheRack *module) : ModuleWidget(module) {
    setPanel(SVG::load(assetPlugin(plugin, "res/WhatTheRack.svg")));

    addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(Widget::create<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    BoomButton* bb = Widget::create<BoomButton>(Vec(50, 272));
    bb->module = module;
    addChild(bb);
  }
};

Model *modelWhatTheRack = Model::create<WhatTheRack, WhatTheRackWidget>("WhatTheRack", "WhatTheRack", "WhatTheRack - Rack randomizer", UTILITY_TAG);
