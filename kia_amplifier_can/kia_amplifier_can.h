#include <mcp2515.h>
#include <can.h>
#ifndef KiaAmplifierCan_h
#define KiaAmplifierCan_h

class KiaAmplifierCan {
	public:
        MCP2515 mcp;
        KiaAmplifierCan(CAN_SPEED bound_rate, CAN_CLOCK mcp_freq, int CS_PIN) : 
           mcp(CS_PIN), 
           FFF {255, 255, 255, 255, 255, 255, 255, 255},
           INIT {0xFC, 0xFF, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
           ADDR {0x00f} {
           BOUND_RATE = bound_rate;
           MCP_FREQ = mcp_freq;
        }
        void init();
        void sendCAN(byte sendingData[]);

	private:

        int ADDR;
        byte FFF[8];
        byte INIT[8];

		CAN_SPEED BOUND_RATE;
	    CAN_CLOCK MCP_FREQ;	
	    struct can_frame generateCanMessage(byte canId, byte canDlc, byte data[]);
};

#endif