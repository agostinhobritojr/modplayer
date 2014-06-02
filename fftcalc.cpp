#include "fftcalc.h"

#undef CLAMP
#define CLAMP(a,min,max) ((a) < (min) ? (min) : (a) > (max) ? (max) : (a))

// fftcalc class is designed to perform fft calculations
FFTCalc::FFTCalc(QObject *parent)
  :QObject(parent){
  processor.moveToThread(&processorThread);
  qRegisterMetaType< QVector<double> >("QVector<double>");
  connect(&processor, SIGNAL(calculatedSpectrum(QVector<double>)), SLOT(setSpectrum(QVector<double>)));
  connect(&processor, SIGNAL(allDone()),SLOT(freeCalc()));
  processorThread.start(QThread::LowestPriority);
  isBusy = false;
}

FFTCalc::~FFTCalc(){
  processorThread.quit();
  processorThread.wait(10000);
}

void FFTCalc::calc(QVector<double> &_array, int duration){
//  qDebug() << isBusy;
  if(isBusy)
    return;
  isBusy = true;
  QMetaObject::invokeMethod(&processor, "processBuffer",
                            Qt::QueuedConnection, Q_ARG(QVector<double>, _array), Q_ARG(int, duration));
}

void FFTCalc::setSpectrum(QVector<double> spectrum)
{
  emit calculatedSpectrum(spectrum);
}

void FFTCalc::freeCalc()
{
  isBusy = false;
}

BufferProcessor::BufferProcessor(QObject *parent){
  timer = new QTimer(this);
  connect(timer,SIGNAL(timeout()),this,SLOT(run()));
  window.resize(MSPEC);
  complexFrame.resize(MSPEC);
  spectrum.resize(MSPEC/2);
  logscale.resize(MSPEC/2+1);
  compressed = true;
  for(int i=0; i<MSPEC;i++){
    window[i] = 0.5 * (1 - cos((2*M_PI*i)/(MSPEC)));
  }
  for(int i=0; i<=MSPEC/2; i++){
    logscale[i] = powf (MSPEC/2, (float) 2*i / MSPEC) - 0.5f;
  }
  running = false;
  timer->start(100);
}

BufferProcessor::~BufferProcessor(){
  timer->stop();

}

void BufferProcessor::processBuffer(QVector<double> _array, int duration){
  // if the music is new, new sizes may arrive
  if(array.size() != _array.size()){
    chunks = _array.size()/MSPEC;
    array.resize(_array.size());
  }
  interval = duration/chunks;
  if(interval < 1)
    interval = 1;
  array = _array;
  pass = 0;
  timer->start(interval);
}
void BufferProcessor::run(){
  unsigned long bufferSize;
  qreal amplitude;
  //http://stackoverflow.com/questions/18958436/how-to-run-a-timer-inside-a-qthread
/*  if(!running)
    return;
*/  if(pass == chunks){
    emit allDone();
    return;
  }
  bufferSize = array.size();
  if(bufferSize < MSPEC){
    return;
  }
  for(uint i=0; i<MSPEC; i++){
    complexFrame[i] = Complex(window[i]*array[i+pass*MSPEC],0);
  }
  fft(complexFrame);
  for(uint i=0; i<MSPEC/2;i++){
    qreal SpectrumAnalyserMultiplier = 1e-2;
    amplitude = SpectrumAnalyserMultiplier*std::abs(complexFrame[i]);
    amplitude = qMax(qreal(0.0), amplitude);
    amplitude = qMin(qreal(1.0), amplitude);
    complexFrame[i] = amplitude;
  }

  if(compressed){
    for (int i = 0; i <MSPEC/2; i ++){
      /* sum up values in freq array between logscale[i] and logscale[i + 1],
         including fractional parts */
      int a = ceilf (logscale[i]);
      int b = floorf (logscale[i+1]);
      float sum = 0;

      if (b < a)
        sum += complexFrame[b].real()*(logscale[i+1]-logscale[i]);
      else{
        if (a > 0)
          sum += complexFrame[a-1].real()*(a-logscale[i]);
        for (; a < b; a++)
          sum += complexFrame[a].real();
        if (b < MSPEC/2)
          sum += complexFrame[b].real()*(logscale[i+1] - b);
      }

      /* fudge factor to make the graph have the same overall height as a
         12-band one no matter how many bands there are */
      sum *= (float) MSPEC/24;

      /* convert to dB */
      float val = 20*log10f (sum);

      /* scale (-DB_RANGE, 0.0) to (0.0, 1.0) */
      val = 1 + val / 40;
      spectrum[i] = CLAMP (val, 0, 1);
    }
  }
  else{
    for(int i=0; i<MSPEC/2; i++){
      spectrum[i] = CLAMP(complexFrame[i].real()*100,0,1);
    }
  }
  emit calculatedSpectrum(spectrum);
  pass++;
}


