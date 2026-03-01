#pragma once

// Only write when the TX buffer has space; avoids blocking when no host is draining it.
// HardwareSerial has no setTxTimeoutMs — availableForWrite() is the correct guard.
#define LOG_PRINT(x)   do { if (Serial.availableForWrite()) Serial.print(x);   } while(0)
#define LOG_PRINTLN(x) do { if (Serial.availableForWrite()) Serial.println(x); } while(0)
