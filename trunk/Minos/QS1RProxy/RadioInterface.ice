module QS1R {

	sequence<float> SpecBuffer;
	
	struct MeterDataStruct {
		float Instantaneous;
		float Average;
	};
		
	enum AGCMODE { AGCOFF, AGCLONG, AGCSLOW, AGCMED, AGCFAST };
	enum DEMODMODE { AM, SAM, LSB, DSB, USB, FMN, FMW, CWL, CWU, DIGL, DIGU };
	
	interface RadioCommander {
	// AGC
		void setAgcMode(int channel, AGCMODE value);
		AGCMODE getAgcMode(int channel);
		void setAgcMaxGain(int channel, float value);
		float getAgcMaxGain(int channel);
		void setAgcFixedGain(int channel, float value);
		float getAgcFixedGain(int channel);	
		void setAgcAttack(int channel, float value);
		float getAgcAttack(int channel);
		void setAgcDecay(int channel, float value);
		float getAgcDecay(int channel);	
		void setAgcHangTime(int channel, float value);
		float getAgcHangTime(int channel);
		void setAgcHangThresh(int channel, float value);
		float getAgcHangThresh(int channel);
		void setAgcSlope(int channel, float value);		
		float getAgcSlope(int channel);
		float getAgcFastGain(int channel);
		float getAgcCurrentGain(int channel);
	// ANF	
		void setAnfSwitch(int channel, bool on);
		bool getAnfSwitch(int channel);
		void setAnfDelay(int channel, int value);
		int getAnfDelay(int channel);
		void setAnfTaps(int channel, int value);
		int getAnfTaps(int channel);
		void setAnfGain(int channel, float value);
		float getAnfGain(int channel);
		void setAnfLeak(int channel, float value);		
		float getAnfLeak(int channel);
	// BINAURAL
		void setBinaural(bool on);
		bool getBinaural();
	// BLOCKSIZE
		void setBlockSize(int value);
		int getBlockSize();
	// DEMODULATION MODE	
		void setDemodMode(int channel, DEMODMODE mode);
		DEMODMODE getDemodMode(int channel);
	// CONTROL REGISTER
		void setControlRegister(int value);
		int getControlRegister();
		void setDecimationRegister(int channel, int value);
		int getDecimationRegister(int channel);
		void setScalingRegister(int channel, int value);
		int getScalingRegister(int channel);
		string getEepromEncFreqRegister();
	// DITHER
		void setDither(bool on);
		bool getDither();
	// ENCODE CLOCK FREQ
		void setEncodeFreq(float value);				
		float getEncodeFreq();	
	// METER CALIBRATION
		void setMeterCalibrationOffset(float value);
		float getMeterCalibrationOffset();
	// SPECTRUM CALIBRATION
		void setSpectrumCalibrationOffset(float value);
		float getSpectrumCalibrationOffset();	
	// NR
		void setNrSwitch(int channel, bool on);
		bool getNrSwitch(int channel);
		void setNrTaps(int channel, int value);
		int getNrTaps(int channel);
		void setNrDelay(int channel, int value);
		int getNrDelay(int channel);
		void setNrGain(int channel, float value);
		float getNrGain(int channel);
		void setNrLeak(int channel, float value);
		float getNrLeak(int channel);
	// MISC
		void setExit(bool on);				
	// FREQ
		void setFrequency(int channel, float value);
		float getFrequency(int channel);
	// FILTER
		void setFilterLo(int channel, float value);
		float getFilterLo(int channel);
		void setFilterHi(int channel, float value);
		float getFilterHi(int channel);
		void setFilter(int channel, float loval, float hival);
	// MUTE	
		void setMute(int channel, bool on);
		bool getMute(int channel);
	// NB1
		void setNb1Switch(int channel, bool on);
		void setNb1Thresh(int channel, int value);
		bool getNb1Switch(int channel);
		int getNb1Thresh(int channel);
	// NB2
		void setNb2Switch(int channel, bool on);
		void setNb2Thresh(int channel, int value);
		bool getNb2Switch(int channel);
		int getNb2Thresh(int channel);
	// PGA
		void setPga(bool on);
		bool getPga();
	// POWER SPECTRUM
		void setInbandPsSwitch(int channel, bool on);
		bool getInbandPsSwitch(int channel);
		int getInbandPsSize(int channel);
		void setOutbandPsSwitch(int channel, bool on);
		bool getOutbandPsSwitch(int channel);
		int getOutbandPsSize(int channel);
		string getSpectrumBandwidth(int channel);
		void setSpectrumBandwidth(int channel, string value);
		string getSpectrumBandwidths(int channel);
	// RANDOMIZER
		void setRand(bool on);
		bool getRand();
	// REVISION/VERSION
		string getVersion();
	// SAMPLERATE
		void setSamplerate(int channel, float value);
		float getSamplerate(int channel);
	// S METER
		void setSMeterCorrection(int channel, float value);
		float getSMeterCorrection(int channel);
		float getSMeterInstantaneous(int channel);
		float getSMeterAverage(int channel);	
	// SQUELCH
		void setSquelchSwitch(int channel, bool on);
		bool getSquelchSwitch(int channel);
		void setSquelchThresh(int channel, float value);
		float getSquelchThresh(int channel);
	// START
		void setStart(bool on);
		bool getStart();
	// STOP
		void setStop(bool on);
		bool getStop();
	// VOLUME
		void setVolume(int channel, float valleft, float valright);
		void setVolumeLeft(int channel, float value);
		float getVolumeLeft(int channel);
		void setVolumeRight(int channel, float value);
		float getVolumeRight(int channel, float value);
	// LOAD FUNCTIONS	
		void setLoadFirmware(bool go);
		void setLoadFpga(bool go);	
	// INFO		
		string getFpgaFileId();
		string getFirmwareFileName();
		string getFpgaRbfName();
		string getDateTime();		
	};
	
	interface PowerSpectrumData {
		SpecBuffer getSpecData(int channel);
		int getSpecDataSize(int channel);
	};
	
	interface MeterData {
		MeterDataStruct getMeterData(int channel);
	};	
};