#ifndef _DISPLITE_H_
#define _DISPLITE_H_

#include <string>
#include <memory>

#ifdef __cplusplus
extern "C" {
#endif

	class hidraw;

	struct displite_device {
		std::string serial{ "" };
		std::string product{ "" };
		std::string dev_path{ "" };
	};

	class displite {
	private:
		std::unique_ptr<hidraw> device;
	protected:
		displite_device info;
		bool get_dev_path();
		void init();
		std::string reset_cmd{ "r" };
	public:
		displite(std::string, std::string);
		~displite();
		bool send_data(const std::string&);
		std::string send_cmd(const std::string&);
		void reset();
	};

#ifdef __cplusplus
}
#endif

#endif