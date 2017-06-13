#ifndef PI300_BMS_MSG
#define PI300_BMS_MSG

#include <stdint.h>

// following Martin Ruckdaeschel BA p. 85 (not complete)
typedef struct{
	uint8_t source_addr;			// source address (170 -> BMS 1)
	uint8_t target_addr;			// target address (85 -> PI300)
	uint8_t length;						// message length in bytes (142)
	uint32_t total_voltage;		// total voltage in mV
	uint16_t cell_voltage[18];	// voltage of cells in mV
	uint32_t balancer;				// ?
	uint16_t cell_voltage_min;	// lowest cell voltage in mV
	uint8_t cell_id_min;				// index of cell with lowest voltage
	uint16_t cell_voltage_max;	// highest cell voltage in mV
	uint8_t cell_id_max;				// index of cell with highest voltage
	uint16_t cell_voltage_diff;	// voltage difference between highest and lowest cell in mV
	uint32_t preload_voltage;		// Vorladespannung in mV
	int32_t	charge_current;			// Ladestrom in A*0.1
	int32_t discharge_current;	// Entladestrom in A*0.1
} pi300_msg;

#endif
