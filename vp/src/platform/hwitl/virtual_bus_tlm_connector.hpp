/*
 * virtual_bus_tlm_connector.cpp
 *
 *  Created on: Oct 20, 2022
 *      Author: dwd
 */


#include <tlm_utils/simple_target_socket.h>
#include <systemc>

#include "bus.h"
#include "virtual-bus/initiator.hpp"

struct VirtualBusMember : public sc_core::sc_module {
	tlm_utils::simple_target_socket<VirtualBusMember> tsock;

	Initiator& virtual_bus;
	const hwitl::Address base_address;
	sc_core::sc_time m_read_delay;
	sc_core::sc_time m_write_delay;

	/**
	 * @input base_address will be added to the local (!) address
	 */
	VirtualBusMember(sc_core::sc_module_name, Initiator& virtual_bus_connector,
			hwitl::Address base_address = 0);

	void set_delay(sc_core::sc_time read_delay, sc_core::sc_time write_delay);

	void transport(tlm::tlm_generic_payload &trans, sc_core::sc_time &delay);
};
