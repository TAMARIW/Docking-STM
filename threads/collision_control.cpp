#include "utils.h"
#include "topics.h"
#include "magnet.h"
#include "config_fsm.h"
#include "current_control.h"
#include "satellite_config.h"
#include "collision_control.h"

static CommBuffer<data_tof_range> cb_tof;
static Subscriber subs_tof(topic_tof_range, cb_tof);
static data_tof_range rx_tof;
static data_collision_ctrl tx_tof;
static data_desired_current tx_current;

pid dpid[4]; // Distance PID controllers.
static double time = 0; // Thread timekeeper.
bool control_mode = false; // true: control and false: pull mode.
float dsp = FSM_DISTANCE_PID_SP_MM; // Distance setpoint, mm.

void collision_control_thread::init()
{
  for(uint8_t i = 0; i < 4; i++)
  {
    dpid[i].set_kp(PID_DISTANCE_KP);
    dpid[i].set_ki(PID_DISTANCE_KI);
    dpid[i].set_control_limits(PID_DISTANCE_UMIN, PID_DISTANCE_UMAX);
  }
}

void collision_control_thread::run()
{
  TIME_LOOP (THREAD_START_COLLISION_CTRL_MILLIS * MILLISECONDS, period * MILLISECONDS)
  {
    time = NOW();

    // Disable thread
    if(stop_thread)
    {
      for(uint8_t i = 0; i < 4; i++)
      {
        dpid[i].reset_memory();
      }

      tx_tof.dt = 0.0;
      topic_collision_ctrl.publish(tx_tof);
      suspendCallerUntil(END_OF_TIME);
    }

    // Read data from tof_range thread.
    cb_tof.getOnlyIfNewData(rx_tof);

    // Decide the course of action and execute it.
    tamariw_state state = fsm::transit_state(rx_tof.dm, rx_tof.vm, rx_tof.approach);
    execute_fsm(state, rx_tof.d, rx_tof.v);


  // if(fsm::get_state() == START_CONTROL)
  // {
  //   tx_current.i[0] = -tx_current.i[0];
  //   tx_current.i[2] = -tx_current.i[2];
  // }

// Sets one of the satellite to constant polarity.
#ifdef CONSTANT_POLE
    for(uint8_t i = 0; i < 4; i++)
    {
      tx_current.i[i] = fabs(tx_current.i[i]);
    }

    // if(fsm::get_state() == START_CONTROL)
    // {
    //   tx_current.i[1] = -tx_current.i[1];
    //   tx_current.i[3] = -tx_current.i[3];
    // }
#endif

  /* Add one '/' to uncomment for testing magnets.
    tx_current.i[0] = 2500;
    tx_current.i[1] = 2500;
    tx_current.i[2] = 2500;
    tx_current.i[3] = 2500;
  //*/

    // Publish desired current to current control thread.
    topic_desired_current.publish(tx_current);

    // Publish telemetry data
    tx_tof.dk[0] = dpid[0].kp,
    tx_tof.dk[1] = dpid[0].ki,
    tx_tof.dt = (NOW() - time) / MILLISECONDS;
    topic_collision_ctrl.publish(tx_tof);
  }
}

/**
 * @brief Execute the input STATE of Tamariw FSM.
 * @param state FSM state to be executed.
 * @param d[4] Relative distances, mm.
 * @param v[4] Relative velocities, mm.
 * @param is_approach true if satellites are approaching each other.
 */
void collision_control_thread::execute_fsm(const tamariw_state state,
                                           const int d[4], const float v[4])
{
  switch (state)
  {
    case STANDBY:
    {
      tamariw_current_control_thread.stop_control = true;
      break;
    }

    // Nothing to do as START_DOCKING is only
    // used to get out of the STANDBY state.
    case START_DOCKING:
    {
      break;
    }

    case ACTUATE_FULL:
    {
      tamariw_current_control_thread.stop_control = false;

      for(uint8_t i = 0; i < 4; i++)
      {
        tx_current.i[i] = FSM_MAX_CURRENT_MILLI_AMP;
      }
      break;
    }

    case ACTUATE_ZERO:
    {
      tamariw_current_control_thread.stop_control = true;
      break;
    }

    case START_CONTROL:
    {
      tamariw_current_control_thread.stop_control = false;

      for(uint8_t i = 0; i < 4; i++)
      {
        float error = dsp - d[i];
        float isq = dpid[i].update(error, period / 1000.0); // Current squared
        tx_current.i[i] = sign(isq) * sqrt(fabs(isq)); // Signed current
      }
      break;
    }

    case LATCH:
    {
      tamariw_current_control_thread.stop_control = false;

      for(uint8_t i = 0; i < 4; i++)
      {
        tx_current.i[i] = FSM_LATCH_CURRENT_MILLI_AMP;
      }
      break;
    }

    case STOP: // check_me
    {
      tamariw_current_control_thread.stop_control = false;

      for(uint8_t i = 0; i < 4; i++)
      {
        tx_current.i[i] = FSM_LATCH_CURRENT_MILLI_AMP;
      }
      break;
    }
  }

  // Reset memory if control used no more
  if(fsm::get_last_state() == START_CONTROL)
  {
    for(uint8_t i = 0; i < 4; i++)
    {
      dpid[i].reset_memory();
    }
  }
}

collision_control_thread tamariw_collision_control_thread("collision_control_thread", THREAD_PRIO_COLLISION_CTRL);
