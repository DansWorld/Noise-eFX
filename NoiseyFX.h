#ifndef __NOISEYFX__
#define __NOISEYFX__

#include "IPlug_include_in_plug_hdr.h"



#include "Gamma.h"		// core functions
//#include "Gamma/Access.h"
//#include "Gamma/Delay.h"
//#include "Gamma/DFT.h"
//#include "Gamma/Effects.h"
//#include "Gamma/Envelope.h"
//#include "Gamma/FormantData.h"
#include "Noise.h"
#include "Oscillator.h"
//#include "Gamma/SamplePlayer.h"
//#include "Gamma/Print.h"
//#include "Gamma/Recorder.h"
//#include "Gamma/SoundFile.h"
//#include "Gamma/Timer.h"
//#include "Gamma/Types.h"
//#include "Gamma/UnitMaps.h"



using namespace gam;

// using namespace gam;

class NoiseyFX : public IPlug
{
public:
  NoiseyFX(IPlugInstanceInfo instanceInfo);
  ~NoiseyFX();

  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);
  double GaussianNoise(int nFrames);
private:
  double mGain;
  double mWet, mWet2, mWet3;
  double mMasterGain; //mGain should be mDry but for now we'll leave it...
  
  NoiseWhite<> white;		// 1/f^0 noise
  NoisePink<> pink;
  NoiseBrown<> brown;
};

#endif
