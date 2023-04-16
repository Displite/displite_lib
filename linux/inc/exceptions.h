#ifndef _DISPLITE_EXCEPTIONS_H_
#define _DISPLITE_EXCEPTIONS_H_

#include <string>
#include <exception>

#ifdef __cplusplus
extern "C" {
#endif

	class displite_error : public std::exception {
	private:
		std::string message{};
	public:
		displite_error(std::string);
		const char* what() const throw();
	};

	class hid_error : public displite_error {
	public:
		hid_error(std::string);
	};

	class device_not_found : public displite_error {
	public:
		device_not_found(std::string);
	};

	class device_not_supported : public displite_error {
	public:
		device_not_supported(std::string);
	};


#ifdef __cplusplus
}
#endif

#endif