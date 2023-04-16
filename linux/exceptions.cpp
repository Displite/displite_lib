#include "exceptions.h"

displite_error::displite_error(std::string msg) : message{ msg } {}

const char* displite_error::what() const throw() {
	return message.c_str();
}

hid_error::hid_error(std::string msg) : displite_error(msg) {}

device_not_found::device_not_found(std::string msg) : displite_error(msg) {}

device_not_supported::device_not_supported(std::string msg) : displite_error(msg) {}
