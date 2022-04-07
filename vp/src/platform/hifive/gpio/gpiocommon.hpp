/*
 * gpio.hpp
 *
 *  Created on: 7 Nov 2018
 *      Author: dwd
 */

#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <limits>       // std::numeric_limits

void hexPrint(unsigned char* buf, size_t size);
void bitPrint(unsigned char* buf, size_t size);

namespace gpio {
	enum class Pinstate : uint8_t {
		UNSET = 0,
		LOW,
		HIGH,
		//TODO: Maybe _weak for pullups/downs

		IOF_SPI = 4,
		IOF_I2C,	// not yet used
		IOF_PWM,	// planned to be used
		IOF_UART,	// not yet used
	};

	enum class Tristate : uint8_t {
		UNSET = 0,
		LOW,
		HIGH,
		//TODO: Maybe _weak for pullups/downs
	};

	inline Pinstate toPinstate (const Tristate from) {
		// this is safe because first 2 bits are identical;
		return static_cast<Pinstate>(from);
	}
	inline Tristate toTristate (const Pinstate from) {
		// this is only safe if a !isIof(from) was done
		return static_cast<Tristate>(from);
	}

	bool isIOF(const Pinstate s);

	static constexpr unsigned default_port = 1400;
	static constexpr unsigned max_num_pins = 64;

	typedef uint8_t PinNumber;
	static_assert(std::numeric_limits<PinNumber>::max() >= max_num_pins);

	typedef uint8_t SPI_Command;
	typedef uint8_t SPI_Response;

	struct State {
		//TODO somehow packed?
		union {
			gpio::Pinstate pins[max_num_pins];
			uint64_t port[(sizeof(gpio::Pinstate) * sizeof(pins) + 1) / sizeof(uint64_t)];
		};
		static_assert(sizeof(gpio::Pinstate) * sizeof(pins) != sizeof(port) * sizeof(uint64_t),
				"Warning: Convenience-function port is causing State to be bigger than necessary");
	};

	struct Request {
		enum class Type : uint8_t {
			GET_BANK = 1,
			SET_BIT,
			REQ_IOF,
			REQ_LOGSTATE = REQ_IOF,
			END_IOF,
			END_LOGSTATE = END_IOF
		} op;
		union {
			struct {
				uint8_t pin : 6;	// max num pins: 64
				gpio::Tristate val : 2;
			} setBit;

			struct {
				// Todo: Decide how to determine SPI's Chip Select
				// Perhaps pin shall be one of the hardware CS pins
				uint8_t pin;
			} reqIOF;
		};
	};

	struct Req_IOF_Response {
		uint16_t port = 0;	// zero is error condition
	};
};

struct GpioCommon {

	static void printRequest(const gpio::Request& req);
	static void printPinstate(const gpio::Pinstate& state);
	static void printState(const gpio::State& state);

	gpio::State state;

	GpioCommon();
};
