///
/// \file Radio.h
/// \brief Library header file for the radio libraries to control radio chips.
///
/// \author Matthias Hertel, http://www.mathertel.de
/// \copyright Copyright (c) 2014 by Matthias Hertel.\n
/// This work is licensed under a BSD style license.\n
/// See http://www.mathertel.de/License.aspx
///
/// \details
/// This library enables the use of diverse radio chips by sharing the same class definition.
/// Implementation for the following Radio Chips are available:
/// * RDA5807M
/// * SI4703
///
/// The following chip is planned to be supported too:
/// * TEA5767
///
/// More documentation and source code is available at http://www.mathertel.de/Arduino
///
/// History:
/// --------
/// * 08.07.2014 creation of the common radio class.
/// * 15.07.2014 examples working with RDA5807M.
/// * 26.08.2014 examples working with SI4703.
/// * 31.08.2014 Doxygen style comments added.

// multi-Band enabled

#ifndef __RADIO_h__
#define __RADIO_h__

#include <arduino.h>
#include <Wire.h>

// The DEBUG_xxx Macros enable Information to the Serial port.
// They can be enabled by setting the _debugEnabled variable to true disabled by using the debugEnable function.
// When the code has to be minimized they can be redefined without implementation like:
// #define DEBUG_STR(txt) {}

/// Used for Debugging text information.
#define DEBUG_STR(txt)           if (_debugEnabled) { Serial.println(txt); }

/// Used for Debugging function entries without parameters.
#define DEBUG_VAL(label, val)    if (_debugEnabled) { Serial.print(label);  Serial.print(':');  Serial.println(val); }

/// Used for Debugging function entries without parameters.
#define DEBUG_FUNC0(fn)          if (_debugEnabled) { Serial.print(fn); Serial.println("()"); }

/// Used for Debugging function entries with 1 parameter.
#define DEBUG_FUNC1(fn, p1)      if (_debugEnabled) { Serial.print(fn); Serial.print('('); Serial.print(p1); Serial.println(')'); }

/// Used for Debugging function entries with 1 parameters as hex Value.
#define DEBUG_FUNC1X(fn, p1) if (_debugEnabled) { Serial.print(fn); Serial.print("(0x"); Serial.print(p1, HEX); Serial.println(')'); }

/// Used for Debugging function entries with 2 parameters.
#define DEBUG_FUNC2(fn, p1, p2)  if (_debugEnabled) { Serial.print(fn); Serial.print('('); Serial.print(p1); Serial.print(", "); Serial.print(p2); Serial.println(')'); }

/// Used for Debugging function entries with 2 parameters and Hex Value.
#define DEBUG_FUNC2X(fn, p1, p2) if (_debugEnabled) { Serial.print(fn); Serial.print('('); Serial.print(p1); Serial.print(", 0x"); Serial.print(p2, HEX); Serial.println(')'); }


// ----- Callback function types -----

/// callback function for passing RDS data.
extern "C" {
  typedef void (*receiveRDSFunction)(uint16_t block1, uint16_t block2, uint16_t block3, uint16_t block4);
}


// ----- type definitions -----

/// Band datatype.
/// The BANDs a receiver probably can implement.
enum RADIO_BAND {
  RADIO_BAND_NONE    = 0, ///< No band selected.

  RADIO_BAND_FM      = 1, ///< FM band 87.5 � 108 MHz (USA, Europe) selected.
  RADIO_BAND_FMWORLD = 2, ///< FM band 76 � 108 MHz (Japan, Worldwide) selected.
  RADIO_BAND_AM      = 3, ///< AM band selected.
  RADIO_BAND_KW      = 4, ///< KW band selected.

  RADIO_BAND_MAX     = 4  ///< Maximal band enumeration value.
};


/// Frequency data type.
/// Only 16 bits are used for any frequency value (not the real one)
typedef uint16_t RADIO_FREQ;


/// A structure that contains information about the radio features from the chip.
typedef struct RADIO_INFO {
  bool active;   ///< receiving is active.
  uint8_t rssi;  ///< Radio Station Strength Information.
  bool rds;      ///< RDS information is available.
  bool tuned;    ///< A stable frequency is tuned.
  bool stereo;   ///< Stereo audio is available
};


/// a structure that contains information about the audio features
typedef struct AUDIO_INFO {
  bool bassBoost;
  bool mute;
  bool softmute;
};

// ----- common RADIO class definition -----

class RADIO {
  public:
  RADIO(); ///< create a new object from this class.

  virtual bool   init();  ///< initialize library and the chip.
  virtual void   term();  ///< terminate all radio functions.

  // ----- Audio features -----

  virtual void    setVolume(uint8_t newVolume); ///< Set the output volume of the chip to 0 .. 15.
  virtual uint8_t getVolume();                  ///< Retrieve the current output volume.

  virtual void   setBassBoost(bool switchOn);   ///< Set the bass boost function on or off.
  virtual bool   getBassBoost();                ///< Retrieve the current bass boost function setting.

  virtual void   setMono(bool switchOn);        ///< Set the mono mode on or off.
  virtual bool   getMono();                     ///< Retrieve the current mono mode setting.

  virtual void   setMute(bool switchOn);        ///< Set the mute mode on or off.
  virtual bool   getMute();                     ///< Retrieve the current mute mode setting.

  // ----- Receiver features -----

  virtual void setBand(RADIO_BAND newBand);     ///< Set the current band.
  virtual RADIO_BAND getBand();                 ///< Retrieve the current band settings from the chip.

  virtual void    setFrequency(RADIO_FREQ newF);  ///< Set the frequency to be tuned.
  virtual RADIO_FREQ getFrequency(void);          ///< Retrieve the current tuned frequency from the chip.

  virtual void setBandFrequency(RADIO_BAND newBand, RADIO_FREQ newFreq); ///< Set Band and Frequency in one call.

  virtual void    seekUp(bool toNextSender = true);   ///< start seek mode upwards
  virtual void    seekDown(bool toNextSender = true); ///< start seek mode downwards

  virtual RADIO_FREQ getMinFrequency();   ///< Get the minimum frequency of the current selected band.
  virtual RADIO_FREQ getMaxFrequency();   ///< Get the maximum frequency of the current selected band.
  virtual RADIO_FREQ getFrequencyStep();  ///< Get resolution of the current selected band.

  // ----- combined status functions -----

  virtual void getRadioInfo(RADIO_INFO *info); ///< Retrieve some information about the current radio function of the chip.

  virtual void getAudioInfo(AUDIO_INFO *info); ///< Retrieve some information about the current audio function of the chip.

  // ----- Supporting RDS for FM bands -----

  virtual void checkRDS(); ///< Check if RDS Data is available and good.
  virtual void attachReceiveRDS(receiveRDSFunction newFunction); ///< Register a RDS processor function.

  // ----- Utilitys -----

  /// Format the current frequency for display and printing.
  virtual void formatFrequency(char *s, uint8_t length);

  // ----- debug Helpers send information to Serial port

  virtual void debugEnable(bool enable = true);  ///< Enable sending debug information to the Serial port.
  virtual void debugRadioInfo(); ///< Print out all radio information.
  virtual void debugAudioInfo(); ///< Print out all audio information.
  virtual void debugStatus();    ///< Send debug information about actual available chip functionality and other internal things.

  protected:
  bool _debugEnabled; ///< Set by debugEnable() and controls debugging functionality.

  uint8_t _volume;    ///< Last set volume level.
  bool    _bassBoost; ///< Last set bass Boost effect.
  bool    _mono;      ///< Last set mono effect.
  bool    _mute;      ///< Last set mute effect.

  RADIO_BAND _band;   ///< Last set band.
  RADIO_FREQ _freq;   ///< Last set frequency.

  RADIO_FREQ _freqLow;    ///< Lowest frequency of the current selected band.
  RADIO_FREQ _freqHigh;   ///< Highest frequency of the current selected band.
  RADIO_FREQ _freqSteps;  ///< Resulution of the tuner.

  receiveRDSFunction _sendRDS; ///< Registered RDS Function that is called on new available data.

  void _printHex4(uint16_t val); ///> Prints a register as 4 character hexadecimal code with leading zeros.

  private:
  void int16_to_s(char *s, uint16_t val); ///< Converts a int16 number to a string, similar to itoa, but using the format "00000".

}; // class RADIO


#endif

// End.