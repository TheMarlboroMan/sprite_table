#include "../../include/dfwimpl/config.h"

using namespace dfwimpl;

config::config(
	const env::env_interface& _env
):
	dfw::base_config(get_file_path(_env)),
	env{_env}
{}

