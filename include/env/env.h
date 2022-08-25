#pragma once

#include <string>

namespace env {

class env_interface {

	public:

/**
* Must return a string representing a filesystem path for application data
* stuff.
*/

	virtual const std::string build_data_path(const std::string& _file) const=0;

/**
* Must return a string representing a filesystem path for user data.
*/

	virtual const std::string build_user_path(const std::string& _file) const=0;
};

/**
* to be used specifically under the directory it was built, with all
* dependencies under the same dir.
*/


class dir_env:
	public env_interface {

	public:

				dir_env(const std::string&, const std::string&);
	virtual const std::string build_data_path(const std::string& _file) const;
	virtual const std::string build_user_path(const std::string& _file) const;

	private:

	const std::string   exec_dir,
	                    user_dir;
};

/**
*  app image env interface, for appimages.
*/

class appimage_env:
	public env_interface {

	public:

				appimage_env(const std::string&, const std::string&);
	virtual const std::string build_data_path(const std::string& _file) const;
	virtual const std::string build_user_path(const std::string& _file) const;

	private:

	const std::string   exec_dir,
	                    user_dir;
};

} //end of namespace
