#include "../../include/env/env.h"

using namespace env;

dir_env::dir_env(
	const std::string& _path
)
	:exec_dir{_path}
{}

const std::string dir_env::build_data_path(const std::string& _file) const {

	return exec_dir+std::string{"data/"}+_file;
}

const std::string dir_env::build_log_path(const std::string& _file) const {

	return exec_dir+std::string{"logs/"}+_file;
}

const std::string dir_env::build_user_path(const std::string& /*_file*/) const {

	return exec_dir+"";
}

appimage_env::appimage_env(
	const std::string& _path
)
	:exec_dir{std::string{"../share"}+_path}
{}

const std::string appimage_env::build_data_path(const std::string& _file) const {


	return exec_dir+std::string{"data/"}+_file;
}

const std::string appimage_env::build_log_path(const std::string& _file) const {


	return exec_dir+std::string{"logs/"}+_file;
}

const std::string appimage_env::build_user_path(const std::string& /*_file*/) const {

	return exec_dir+"";
}
