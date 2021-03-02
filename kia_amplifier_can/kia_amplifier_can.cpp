#include <mcp2515.h>
#include <kia_amplifier_can.h>
#include <can.h>

void KiaAmplifierCan::init() {
  mcp.reset();
  mcp.setBitrate (BOUND_RATE, MCP_FREQ);
  mcp.setNormalMode();
  Serial.println("CAN successfully initialized");	
}

struct can_frame KiaAmplifierCan::generateCanMessage(byte canId, byte canDlc, byte data[]) {
  struct can_frame canMsg; 
  canMsg.can_id  = canId;
  canMsg.can_dlc = canDlc;
  Serial.print("Message: " + String(canId) + " " + String(canDlc));
  for(int ii = 0; ii < 8; ii++) {
    canMsg.data[ii] = data[ii];
    Serial.print(" " + String(data[ii]));
  }
  Serial.println("");
  return canMsg;
}

void KiaAmplifierCan::sendCAN(byte sendingData[]) {

  struct can_frame canMsg;

  canMsg = generateCanMessage(0x009,8,INIT);
  mcp.sendMessage(&canMsg);

  canMsg = generateCanMessage(ADDR, 8, sendingData);
  mcp.sendMessage(&canMsg);
  
  delay(18);
  canMsg = generateCanMessage(ADDR, 8, FFF);
  mcp.sendMessage(&canMsg);
   
  delay(18);
  mcp.sendMessage(&canMsg);
   
  delay(18);
  mcp.sendMessage(&canMsg);
  
  delay(100);
}