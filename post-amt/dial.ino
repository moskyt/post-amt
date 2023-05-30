// this is called every 10 ms
void dialHandler() {

  const int pulse_length_low = 5;
  const int pulse_length_high = 2;
  const int reset_timeout = 1000;

  int dialSensorVal = digitalRead(PIN_DIAL);
  if (dialSensorVal == dialLastVal) {
    dialCount += 1;
    if (dialLastVal == 0 && dialCount > pulse_length_low && dialBuffer > 0) {
      if (dialBuffer == 10) dialBuffer = 0;
      dialNumber[dialPtr] = dialBuffer;
      dialPtr++;

      if (dialPtr == dialNMax) {
        dial_read = dialNumber[0] * 1000 + dialNumber[1] * 100 + dialNumber[2] * 10 + dialNumber[3];
        dialPtr = 0;
      } else {
        dial_read = -1;
      }

      dialBuffer = 0;
    }
    if (dialLastVal == 0 && dialCount > reset_timeout && dialPtr > 0) {
      dialPtr = 0;
      dial_read = -1;
    }
  } else {
    if (dialLastVal == 1) {
      // new high pulse, increment current number
      if (dialCount > pulse_length_high) {
        dialBuffer += 1;
      }
    }
    if (dialLastVal == 0) {
      if (dialCount > 10) {
        dialBuffer = 0;
      }
    }
    dialCount = 0;
    dialLastVal = dialSensorVal;
  }
}