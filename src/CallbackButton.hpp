#ifndef CALLBACK_BUTTON_HPP__
#define CALLBACK_BUTTON_HPP__

#include "plugin.hpp"
#include "window.hpp"
#include <functional>

template<typename T>
struct CallbackButton : SvgButton {
  static CallbackButton* create(const Vec& v,
				std::function<void (T*)> c,
				T* m,
				std::shared_ptr<rack::Svg> button_idle,
				std::shared_ptr<rack::Svg> button_clicked) {
    CallbackButton* cb = createWidget<CallbackButton<T> >(v);
    cb->callback = c;
    cb->module = m;
    cb->addFrame(button_idle);
    cb->addFrame(button_clicked);
    return cb;
  }

  CallbackButton() {}

  virtual void onAction(const event::Action &e) override {
    e.consume(this);
    callback(module);
    SvgButton::onAction(e);
  }

  std::function<void (T*)> callback;
  T* module;
};


#endif
