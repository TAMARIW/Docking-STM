#ifndef _CURRENT_CONTROL_H_
#define _CURRENT_CONTROL_H_

#include "satellite_config.h"

class current_control_thread : public StaticThread<>
{
private:
  int period = THREAD_PERIOD_CURRENT_CTRL_MILLIS; // millis

public:
  current_control_thread(const char* thread_name, int priority) : StaticThread(thread_name, priority){}

  bool stop_control = true;

  void init(void);
  void run(void);
};

extern current_control_thread tamariw_current_control_thread;

#endif // current_control.h
