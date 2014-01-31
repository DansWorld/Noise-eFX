#include "NoiseyFX.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

const int kNumPrograms = 2;

enum EParams
{
  kGain = 0,
  kWet = 1,
  kWet2 = 2,
  kWet3 = 3,
  kMasterGain = 4,
  kNumParams
};

enum ELayout
{
  kWidth = GUI_WIDTH,
  kHeight = GUI_HEIGHT,
  //We have the option to use this through out the CPP file but is not neccessary.
  //The enum could be used for the layout or you could simply place the coordinates in yourself
  //like below...
  kGainX = 25,
  kGainY = 50,
  kKnobFrames = 60
};

NoiseyFX::NoiseyFX(IPlugInstanceInfo instanceInfo)  //init the settings
  :	IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo), mGain(1.), mWet(1.), mWet2(1.), mWet3(1.), mMasterGain(1.)
{
  TRACE;

  //arguments are: name, defaultVal, minVal, maxVal, step, label (Label is optional)
  GetParam(kGain)->InitDouble("Dry", 50., 0., 100.0, 0.01, "%");
  GetParam(kGain)->SetShape(2.);
  GetParam(kWet)->InitDouble("White Wet", 50., 0., 100., 0.01, "%");
  GetParam(kWet)->SetShape(2.);
  GetParam(kWet2)->InitDouble("Brown Wet", 50., 0., 100., 0.01, "%");
  GetParam(kWet2)->SetShape(2.);
  GetParam(kWet3)->InitDouble("Pink Wet", 50., 0., 100., 0.01, "%");
  GetParam(kWet3)->SetShape(2.);
  GetParam(kMasterGain)->InitDouble("Gain", 100., 0., 100.0, 0.01, "%");
  GetParam(kMasterGain)->SetShape(2.);

  IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  pGraphics->AttachPanelBackground(&COLOR_GRAY);

  IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, kKnobFrames);
  //Attach graphics to knobcontroller class
  pGraphics->AttachControl(new IKnobMultiControl(this, kGainX, kGainY, kGain, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, 25, 100, kWet, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, 25, 150, kWet2, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, 25, 200, kWet3, &knob));
  pGraphics->AttachControl(new IKnobMultiControl(this, 125, 50, kMasterGain, &knob));
  
  AttachGraphics(pGraphics);

  //MakePreset("preset 1", ... );
  MakeDefaultPreset((char *) "-", kNumPrograms);
}

NoiseyFX::~NoiseyFX() {}

void NoiseyFX::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
  // Mutex is already locked for us.

  double* in1 = inputs[0];
  double* in2 = inputs[1];
  double* out1 = outputs[0];
  double* out2 = outputs[1];
  



  for (int s = 0; s < nFrames; ++s, ++in1, ++in2, ++out1, ++out2)
  { //
    //Our final output.  mGain is functioning as our dry signal.
    //For the sake of simplicity and overall effectiveness of the plugin...
    //We will have all the possible types of combinations always able to mix into
    //Though this may need to change due to filter implementation and envelopes.
    
    
    *out1 = (mGain * *in1 + (*in1 * mWet * white())+(*in1 * mWet2 * brown())+(*in1 * mWet3 * pink()))*mMasterGain;
    *out2 = (mGain * *in2 + (*in2 * mWet * white())+(*in2 * mWet2 * brown())+(*in2 * mWet3 * pink()))*mMasterGain;

    
  }
}

void NoiseyFX::Reset()
{
  TRACE;
  IMutexLock lock(this);
  Sync::master().spu(GetSampleRate());
}

void NoiseyFX::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);

  switch (paramIdx)
  {
    case kGain:
      mGain = GetParam(kGain)->Value() / 100.;
      break;
    case kWet:
      mWet = GetParam(kWet)->Value() / 100;
      break;
    case kWet2:
      mWet2  = GetParam(kWet2)->Value()/100;
      break;
    case kWet3:
      mWet3 = GetParam(kWet3)->Value()/100;
      break;
    case kMasterGain:
      mMasterGain = GetParam(kMasterGain)->Value()/100;
      break;
    default:
      break;
  }
}

double NoiseyFX::GaussianNoise(int nFrames){
  double scaledown = 0.125;
  double finalresult;
  double temp1;
  double temp2;
  double result;
  int p;
  
  p = 1;
  
//Online example of generating Gaussian Whitenoise.
for (int i = 0; i < nFrames; i++)
{
  while( p > 0 )
  {
    temp2 = ( rand() / ( (double)RAND_MAX ) ); /*  rand() function generates an
                                                integer between 0 and  RAND_MAX,
                                                which is defined in stdlib.h.
                                                */
    
    if ( temp2 == 0 )
    {// temp2 is >= (RAND_MAX / 2)
      p = 1;
    }// end if
    else
    {// temp2 is < (RAND_MAX / 2)
      p = -1;
    }// end else
    
  }// end while()
  
  temp1 = cos( ( 2.0 * (double)PI ) * rand() / ( (double)RAND_MAX ) );
  result = sqrt( -2.0 * log( temp2 ) ) * temp1;
  finalresult = result * scaledown; //Scale the result down so we aren't clipping...
  
}
  return finalresult; // send the Final result to the buffer...

}

