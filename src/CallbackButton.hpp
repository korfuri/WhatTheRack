#ifndef CALLBACK_BUTTON_HPP__
#define CALLBACK_BUTTON_HPP__

#include "plugin.hpp"
#include "window.hpp"
#include <functional>

template<typename T>
struct CallbackButton : SVGButton {
  static CallbackButton* create(const Vec& v,
				std::function<void (T*)> c,
				T* m,
				std::shared_ptr<rack::SVG> button_idle,
				std::shared_ptr<rack::SVG> button_clicked) {
    CallbackButton* cb = Widget::create<CallbackButton<T> >(v);
    cb->callback = c;
    cb->module = m;
    cb->setSVGs(button_idle, button_clicked);
    return cb;
  }

  CallbackButton() {}

  virtual void onAction(EventAction &e) override {
    sw->setSVG(activeSVG);
    callback(module);
  }

  std::function<void (T*)> callback;
  T* module;
};


#endif
