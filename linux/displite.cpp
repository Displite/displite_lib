#include <systemd/sd-device.h>
#include <systemd/sd-journal.h>
#include "displite.h"
#include <cstring>
#include "exceptions.h"
#include <stdexcept>
#include <thread>
#include <chrono>
#include "hidraw.h"


bool displite::get_dev_path() {
	bool result{ false };

	sd_device_enumerator* enumerator = nullptr;
	sd_device_enumerator_new(&enumerator);
	sd_device_enumerator_add_match_subsystem(enumerator, "hidraw", 1);

	for (sd_device* device = sd_device_enumerator_get_device_first(enumerator); device;
		device = sd_device_enumerator_get_device_next(enumerator)) {
		const char* devpath, * syspath, * vid, * pid, * id_serial, *product;
		sd_device *pdev, *sdev;

		sd_device_get_syspath(device, &syspath);

		sd_device_new_from_syspath(&sdev, syspath);
		sd_device_get_parent_with_subsystem_devtype(sdev, "usb", "usb_device", &pdev);
		sd_device_get_sysattr_value(pdev, "idVendor", &vid);
		sd_device_get_sysattr_value(pdev, "idProduct", &pid);

		if (strcmp(vid, "cafe") != 0 || strcmp(pid, "4004") != 0) {
			sd_device_unref(pdev);
			continue;
		}

		sd_device_get_sysattr_value(pdev, "serial", &id_serial);
		sd_device_get_sysattr_value(pdev, "product", &product);

		#ifdef DEBUG
		sd_journal_print(LOG_INFO, "Found displite device having product: %s; Serial: %s", product, id_serial);
		#endif

		if (strcmp(id_serial, info.serial.c_str()) != 0 || strcmp(product, info.product.c_str()) != 0) {
			sd_device_unref(pdev);
			continue;
		}

		sd_device_get_devname(device, &devpath);
		info.dev_path = std::string(devpath);
		result = true;
		sd_device_unref(pdev);

		#ifndef DEBUG
		break;
		#endif
	}

	sd_device_enumerator_unref(enumerator);

	return result;
}

void displite::init() {
	#ifdef DEBUG
		sd_journal_print(LOG_INFO, "finding device");
	#endif
	if (!get_dev_path()) {
		#ifdef DEBUG
			sd_journal_print(LOG_ERR, "Could not find device");
		#endif
		throw(device_not_found("device with serial, product - "+ info.serial + ", " + info.product +"not found"));
	}

	#ifdef DEBUG
		sd_journal_print(LOG_INFO, "device found");
	#endif

	device = std::make_unique<hidraw>(info.dev_path);
}

displite::displite(std::string serial, std::string product) {
	if (serial.empty() || product.empty()) {
		throw(std::invalid_argument("serial and gui name should not be empty"));
	}

	info.product = product;
	info.serial = serial;

	init();
	reset();
}

displite::~displite() {
	try {
		device->hid_write(reset_cmd);
	} catch(std::exception &e) {
		
	}
}

bool displite::send_data(const std::string &data) {
	if (data.empty()) return false;

	std::string result{ "" };

	result = "d" + data;

	device->hid_write(result);
	device->hid_read(result);

	return result != "0";
}

std::string displite::send_cmd(const std::string &cmd) {
	if (cmd.empty() || cmd[0] == 'r') return "0";

	device->hid_write(cmd);

	std::string result;

	device->hid_read(result);

	return result;
}

void displite::reset() {
	std::string temp{""};
	device->hid_write(reset_cmd);
	#ifdef DEBUG
		sd_journal_print(LOG_INFO, "reset command sent");
	#endif
	device->hid_read(temp);
	#ifdef DEBUG
		sd_journal_print(LOG_INFO, "waiting for device to reset");
	#endif
	std::this_thread::sleep_for(std::chrono::seconds(3));
	device.reset();
	init();
}
