#pragma once

#include "config.h"

#include <ldtools/ttf_manager.h>

#include <dfw/state_driver_interface.h>

#include <memory>

//Controllers.
#include "../../include/controller/file_browser.h"
//[new-controller-header-mark]

//Specific app_config
//#include "../app/placeholder.h"

namespace dfwimpl {

class state_driver:
	public dfw::state_driver_interface {

	public:
							state_driver(dfw::kernel& kernel, dfwimpl::config& config);

	virtual void					common_pre_loop_input(dfw::input& input, float delta);
	virtual void					common_pre_loop_step(float delta);
	virtual void					common_loop_input(dfw::input& input, float delta);
	virtual void					common_loop_step(float delta);
	virtual void					prepare_state(int, int);
	virtual float					get_max_timestep() const {return 0.03f;}

	private:

	void						prepare_video(dfw::kernel&);
	void						prepare_audio(dfw::kernel&);
	void						prepare_input(dfw::kernel&);
	void						prepare_resources(dfw::kernel&);
	void						register_controllers(dfw::kernel&);
	void 						virtualize_input(dfw::input& input);

	//references
	dfwimpl::config&			config;
	lm::logger&					log;
	ldtools::ttf_manager		ttf_manager;

	typedef std::unique_ptr<dfw::controller_interface>	ptr_controller;
	ptr_controller					c_file_browser;
	//[new-controller-property-mark]
};

}
