void updateIcons() {
  
  // Read RSSI from Spark
  iRSSI = ble_getRSSI();
            
  // Show BT icon if connected
  // Use graduated scale based on the following
  // 0 bars (very poor) < -95db
  // 1 bar (poor) = -75db to -95db
  // 2 bars (fair) = -60db to -75db
  // 3 bars (good) = -40db to -60db
  // 4 bars (excellent) = > -40db
  if(connected_sp){
    Heltec.display->drawXbm(btlogo_pos, 0, bt_width, bt_height, bt_bits);
    // Display BT RSSI icon depending on actual signal
    if (iRSSI > -40) {
      Heltec.display->drawXbm(rssi_pos, 0, rssi_width, rssi_height, rssi_4);
    }
    else if (iRSSI > -60) {
      Heltec.display->drawXbm(rssi_pos, 0, rssi_width, rssi_height, rssi_3);
    }
    else if (iRSSI > -75) {
      Heltec.display->drawXbm(rssi_pos, 0, rssi_width, rssi_height, rssi_2);
    }
     else if (iRSSI > -95) {
      Heltec.display->drawXbm(rssi_pos, 0, rssi_width, rssi_height, rssi_1);
    }
    // else no bars 
  }
  // Update drive status icons once data available
  if(isHWpresetgot){ 
    // Drive icon
    if (presets[5].effects[2].OnOff){
      Heltec.display->drawXbm(drive_pos, 0, icon_width, icon_height, drive_on_bits);
    }
    else {
      Heltec.display->drawXbm(drive_pos, 0, icon_width, icon_height, drive_off_bits);   
    }
    // Mod icon
    if (presets[5].effects[4].OnOff) {
      Heltec.display->drawXbm(mod_pos, 0, icon_width, icon_height, mod_on_bits);
    }
    else {
       Heltec.display->drawXbm(mod_pos, 0, icon_width, icon_height, mod_off_bits);   
    }
    // Delay icon
    if (presets[5].effects[5].OnOff) {
      Heltec.display->drawXbm(delay_pos, 0, icon_width, icon_height, delay_on_bits);
    }
    else {
       Heltec.display->drawXbm(delay_pos, 0, icon_width, icon_height, delay_off_bits);   
    }
    // Reverb icon
    if (presets[5].effects[6].OnOff) {
      Heltec.display->drawXbm(rev_pos, 0, icon_width, icon_height, rev_on_bits);
    }
    else {
       Heltec.display->drawXbm(rev_pos, 0, icon_width, icon_height, rev_off_bits);
    }
  }
  // Battery icon control - measured periodically via a 1s timer
  // Average readings to reduce noise
  if (isTimeout) {
    vbat_result = analogRead(VBAT_AIN); // Read battery voltage

    // To speed up the display when a battery is connected from scratch
    // ignore/fudge any readings below the lower threshold
    if (vbat_result < BATTERY_LOW) vbat_result = BATTERY_LOW;
    temp = vbat_result;

    // While collecting data
    if (vbat_ring_count < VBAT_NUM) {
      vbat_ring_sum += vbat_result;
      vbat_ring_count++;
      vbat_result = vbat_ring_sum / vbat_ring_count;
    }
    // Once enough is gathered, do a decimating average
    else {
      vbat_ring_sum *= 0.9;
      vbat_ring_sum += vbat_result;
      vbat_result = vbat_ring_sum / VBAT_NUM;
    }

    chrg_result = analogRead(CHRG_AIN); // Check state of /CHRG output
    isTimeout = false;
  }

  // Start by showing the empty icon. drawXBM writes OR on the screen so care
  // must be taken not to graphically block out some symbols. This is why the
  // battery full but not charging is the last in the chain.

   // No battery monitoring so just show the empty symbol
  #ifdef BATT_CHECK_0
    Heltec.display->drawXbm(bat_pos, 0, bat_width, bat_height, bat00_bits);
  #endif
  
  // For level-based charge detection (not very reliable)
  #ifdef BATT_CHECK_1
    if (vbat_result >= BATTERY_CHRG) {
      Heltec.display->drawXbm(bat_pos, 0, bat_width, bat_height, batcharging_bits);
    }
  #endif
      
  // If advanced charge detection available, and charge detected
  #ifdef BATT_CHECK_2
    if (chrg_result < CHRG_LOW) {
      Heltec.display->drawXbm(bat_pos, 0, bat_width, bat_height, batcharging_bits);
    }
  #endif
  
  // Basic battery detection available. Coarse cut-offs for visual 
  // guide to remaining capacity. Surprisingly complex feedback to user.
  // No bars = 0% (should not be discharged further)
  // Full symbol = >85%
  #ifndef BATT_CHECK_0
    else if (vbat_result < BATTERY_LOW) {
      Heltec.display->drawXbm(bat_pos, 0, bat_width, bat_height, bat00_bits);
    }
    else if (vbat_result < BATTERY_10) {
      Heltec.display->drawXbm(bat_pos, 0, bat_width, bat_height, bat10_bits);
    }
    else if (vbat_result < BATTERY_20) {
      Heltec.display->drawXbm(bat_pos, 0, bat_width, bat_height, bat20_bits);
    }
    else if (vbat_result < BATTERY_30) {
      Heltec.display->drawXbm(bat_pos, 0, bat_width, bat_height, bat30_bits);
    }    
    else if (vbat_result < BATTERY_40) {
      Heltec.display->drawXbm(bat_pos, 0, bat_width, bat_height, bat40_bits);
    }
    else if (vbat_result < BATTERY_50) {
      Heltec.display->drawXbm(bat_pos, 0, bat_width, bat_height, bat50_bits);
    }
    else if (vbat_result < BATTERY_60) {
      Heltec.display->drawXbm(bat_pos, 0, bat_width, bat_height, bat60_bits);
    }
    else if (vbat_result < BATTERY_70) {
      Heltec.display->drawXbm(bat_pos, 0, bat_width, bat_height, bat70_bits);
    }        
    else if (vbat_result < BATTERY_80) {
      Heltec.display->drawXbm(bat_pos, 0, bat_width, bat_height, bat80_bits);
    }
    else if (vbat_result < BATTERY_90) {
      Heltec.display->drawXbm(bat_pos, 0, bat_width, bat_height, bat90_bits);
    }
    else {
      Heltec.display->drawXbm(bat_pos, 0, bat_width, bat_height, bat100_bits);
    } 
  #endif
}

// Print out the requested preset data
void dump_preset(SparkPreset preset) {
  int i,j;

  Serial.print(preset.curr_preset); Serial.print(" ");
  Serial.print(preset.preset_num); Serial.print(" ");
  Serial.print(preset.Name); Serial.print(" ");
  Serial.println(preset.Description);

  for (j=0; j<7; j++) {
    Serial.print("    ");
    Serial.print(preset.effects[j].EffectName); Serial.print(" ");
    if (preset.effects[j].OnOff == true) Serial.print(" On "); else Serial.print (" Off ");
    for (i = 0; i < preset.effects[j].NumParameters; i++) {
      Serial.print(preset.effects[j].Parameters[i]); Serial.print(" ");
    }
    Serial.println();
  }
}

// Pushbutton handling
void dopushbuttons(void)
{
  // Debounce and long press code
  for (i = 0; i < NUM_SWITCHES; i++) {
    // If the button pin reads HIGH, the button is pressed (VCC)
    if (digitalRead(sw_pin[i]) == HIGH)
    {
      // If button was previously off, mark the button as active, and reset the timer
      if (buttonActive[i] == false){
          buttonActive[i] = true;
          buttonTimer[i] = millis();
       }
       
      // Calculate the button press duration by subtracting the button time from the boot time
      buttonPressDuration[i] = millis() - buttonTimer[i];
      
      // Mark the button as long-pressed if the button press duration exceeds the long press threshold
      if ((buttonPressDuration[i] > longPressThreshold) && (longPressActive[i] == false)) {
          longPressActive[i] = true;
          latchpress = true;
      }
    }
      
    // The button either hasn't been pressed, or has been released
    else {
      // Reset switch register here so that switch is not repeated    
      sw_val[i] = LOW; 
      
      // If the button was marked as active, it was recently pressed
      if (buttonActive[i] == true){
        // Reset the long press active state
        if (longPressActive[i] == true){
          longPressActive[i] = false;
          latchpress = false;
        }
        
        // The button was previously active. We either need to debounce the press (noise) or register a normal/short press
        else {
          // if the button press duration exceeds our bounce threshold, then we register a short press
          if (buttonPressDuration[i] > debounceThreshold){
            sw_val[i] = HIGH;
          }
        }
        
        // Reset the button active status
        buttonActive[i] = false;
      }
    }
  }    

  // OR all the long press flags so any of the four main footswitches can switch modes
  AnylongPressActive = (longPressActive[0] || longPressActive[1] || longPressActive[2] || longPressActive[3]);
  // Has any button been held down
  if (AnylongPressActive && (latchpress == true)){
      Serial.println("Switching pedal mode");
      isOLEDUpdate = true;
      latchpress = false;
      if (isPedalMode) isPedalMode = false;   // Toggle pedal mode
      else isPedalMode = true;
  }
}

// Refresh UI
void refreshUI(void)
{
    // Flip GUI flash bool
    if (isTimeout){
      flash_GUI = !flash_GUI;
    }
  
    // if a change has been made or the timer timed out, we have the preset and connected...
//    if ((isOLEDUpdate || isTimeout)){
    if ((isOLEDUpdate || isTimeout) && !sp_resend_preset_info && isHWpresetgot){
    isOLEDUpdate = false;  
    Heltec.display->clear();
    Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    if (!isPedalMode) {
      Heltec.display->drawString(0, 20, "Preset mode");
    }
    else {
      Heltec.display->drawString(0, 20, "Effect mode");    
    }
    Heltec.display->setFont(Roboto_Mono_Bold_52);
    Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
    
    if (flash_GUI || !setting_modified) {
      // In-joke to the "I saw 5 on the display!" folk
      if (display_preset_num > 3) {
        display_preset_num = 3; 
      }
      Heltec.display->drawString(110, 12, String(display_preset_num+1));
    }
    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);

    // Sanity length check on name string
    if (strlen(presets[5].Name) > MAXNAME){
       presets[5].Name[MAXNAME - 1]  = '\0';  // Rudely truncate
    }
    Heltec.display->drawString(0, 50, presets[5].Name);

    // Flash "Connect App" message when no app connected
    if (flash_GUI && !connected_app){
      Heltec.display->setFont(ArialMT_Plain_10);
      Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
      Heltec.display->drawString(15, 37, "Connect App");
    }
       
    updateIcons();
    Heltec.display->display();
  }
  if (!connected_sp) {
    // Show reconnection message
    Heltec.display->clear();
    Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
    Heltec.display->drawString(64, 10, "Reconnecting");
    Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
    Heltec.display->drawString(64, 35, "Please wait");
    Heltec.display->display();
  }
}
