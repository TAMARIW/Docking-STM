#ifndef _TOPICS_H_
#define _TOPICS_H_

#include "rodos.h"

struct tof_t
{
  int d[4] = {0, 0, 0, 0};            // Distance, mm
  float kf_d[4] {0.0, 0.0, 0.0, 0.0}; // KF relative position estimates, mm
  float kf_v[4] {0.0, 0.0, 0.0, 0.0}; // KF relative velocity estimates, mm/s
  float dt = 0;                       // Thread period, millis
};

struct coil_t
{
  float i[4];   // Current, milli Amp
  float dt = 0; // Thread period, millis
};

struct input_t
{
  float i[4]; // Current set_points, milli Amp
  bool stop_coils;  // Disable all coils
  bool stop_coil[4]; // Disable individual coils
  float kp;
  float ki;
  float q_pos;
  float q_vel;
  float r;
};

extern input_t rx;
extern Topic<tof_t> topic_tof;
extern Topic<coil_t> topic_coil;

extern CommBuffer<tof_t> cb_tof;
extern CommBuffer<coil_t> cb_coil;

extern Subscriber subs_tof;
extern Subscriber subs_coil;

#endif // telecommand.h
