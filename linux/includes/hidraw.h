#ifndef _HID_RAW_H_
#define _HID_RAW_H_

#include <string>
#include <exception>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	class hidraw {
	private:
		int device_handle{ -1 };
		void hid_init();
		void hid_deinit();
		const std::string hid_dev_path;
	public:
		hidraw(std::string);
		void hid_write(const std::string&);
		void hid_read(std::string&);
		~hidraw();
		std::string get_error(const int&);
	};

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !_HID_RAW_H_

