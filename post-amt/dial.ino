void dialHandler() {

  int dialSensorVal = digitalRead(PIN_DIAL);
  if (dialSensorVal == dialLastVal) {
    dialCount += 1;
    if (dialLastVal == 0 && dialCount > 5 && dialBuffer > 0) {
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
    if (dialLastVal == 0 && dialCount > 1000 && dialPtr > 0) {
      dialPtr = 0;
      dial_read = -1;
    }
  } else {
    if (dialLastVal == 1) {
      if (dialCount > 2) {
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