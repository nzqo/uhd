//
// Copyright 2018 Ettus Research, a National Instruments Company
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

#pragma once

#include <boost/filesystem.hpp>
#include <string>

namespace uhd {

/*! Expand environment variables in paths, like Python's
 *  os.path.expandvars().
 *
 * If expansion fails, will simply return the original path.
 */
std::string path_expandvars(const std::string& path);

//! Compatibility function for deprecated CSV file reader. Remove this when CSV
// format for IQ/DC cal gets removed.
std::string get_appdata_path(void);

//! Return a path to XDG_DATA_HOME
//
// https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
//
// Even on non-Linux systems, this should return the place where app data is
// written to. For UHD, this is data such as calibration data.
//
// There are valid scenarios when there is no such variable, e.g., when a process
// is being spawned as a system process (there is no 'user' specified). If no
// such path can be found, an empty string is returned.
boost::filesystem::path get_xdg_data_home();

//! Return a path to XDG_CONFIG_HOME
//
// https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html
//
// Even on non-Linux systems, this should return the place where the
// configuration file can be stored.
//
// There are valid scenarios when there is no such variable, e.g., when a process
// is being spawned as a system process (there is no 'user' specified). If no
// such path can be found, an empty string is returned.
boost::filesystem::path get_xdg_config_home();

//! Return a path to ~/.uhd
//
// If no home directory can be found, an empty string is returned.
boost::filesystem::path get_legacy_config_home();

//! Return a list of paths to module.d/ directories.
//
// These directories contain files that are loaded by UHD at runtime. Every
// file may contain a list of modules to load. Every line in each file is
// the name or path of a library to load.
std::vector<boost::filesystem::path> get_module_d_paths(void);

//! Return the path to a UHD command.
//
// UHD commands are executables that typically get installed into a runtime
// path, e.g., /usr/bin.
std::string find_uhd_command(const std::string& command);

} /* namespace uhd */
